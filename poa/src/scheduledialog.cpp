/*****************************************************************************
 * POA - Programmierumgebung Offener Antrieb
 *
 * Copyright (C) 2003 Necati Aydin, Anselm Garbe, Stefan Hauser,
 * Steffen Keul, Marcel Kilgus, Steffen Pingel, Tammo van Lessen
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * $Id: scheduledialog.cpp,v 1.28 2004/01/13 14:46:32 squig Exp $
 *
 *****************************************************************************/

#include <qvariant.h>
#include <qapplication.h>
#include <qcanvas.h>
#include <qheader.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qptrlist.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qimage.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qlayout.h>
#include <qslider.h>
#include <qsplitter.h>
#include <qtable.h>
#include <qwmatrix.h>
#include <math.h>

#include "scheduledialog.h"
#include "blocktree.h"
#include "canvasview.h"
#include "blockmodel.h"
#include "muxmodel.h"
#include "codemanager.h"
#include "cpumodel.h"
#include "mainwindow.h"
#include "poa.h"
#include "project.h"

const int CANVAS_WIDTH = 800;       // Canvas width
const int X_ORIGIN = 50;            // Blocks start at this origin
const int BOX_HEIGHT = 10;          // Height of one box in diagram
const int BOX_YSPACING = 20;        // Space between two boxes
const int RULER_HEIGHT = 25;
const int RULER_TICK = 25;         // Shows every RULER_TICK ns a tick
const double PIX_PER_NS = 1.0;      // Pixels per nanosecond

const double ArrowLine::RAD2DEG = 57.2958;

///////////////////////////////////////////////////////////////////////////////

ScheduleDialog::ScheduleDialog(Project* pro, QWidget* parent, const char* name,
                               bool modal, WFlags fl)
    : QDialog(parent, name, modal, fl)
{
    if (!name) {
        setName("ScheduleDialog");
    }
    resize(700, 500);
    setCaption(tr("Scheduling"));

    project_ = pro;
    modified_ = false;
    zoom_ = 1.0;
    initLayout();
}

ScheduleDialog::~ScheduleDialog()
{
    delete canvas;
    delete labelCanvas;
}

// Recursively build tree
void ScheduleDialog::buildBranch(BlockTree *bt)
{
    QValueList<PinModel*> l = bt->getBlock()->pins();
    for (QValueList<PinModel*>::Iterator pin = l.begin(); pin != l.end(); ++pin) {
        if (((*pin)->type() == PinModel::OUTPUT) && (*pin)->connected()) {
            BlockModel* block = (*pin)->connected()->parent();

            // In case of Mux get list of connecting blocks from Mux
            if (INSTANCEOF(block, MuxModel)) {
                QPtrList<PinModel> pins =
                    ((MuxModel*)block)->connectionsForInputPin((*pin)->connected());
                // Check all output pins for this input pin
                for (QPtrListIterator<PinModel> muxit(pins); muxit != 0; ++muxit) {
                    if (!(*muxit)->connected()) continue;
                    block = (*muxit)->connected()->parent();
                    // Only add model recursively if not already in tree

                    if (blocksToTree_.contains(block)) {
                        // add back reference only and stop recursion.
                        bt->addBranch(block)->setBackReference(true);
                    } else {
                        BlockTree *lb = bt->addBranch(block);
                        blocksToTree_.insert(block, lb);
                        blocks_.append(lb);
                        buildBranch(lb);
                    }
                }
            } else {    // No mux, straight connection

                if (blocksToTree_.contains(block)) {
                    // add back reference only and stop recursion.
                    bt->addBranch(block)->setBackReference(true);
                } else {
                    BlockTree *lb = bt->addBranch(block);
                    blocksToTree_.insert(block, lb);
                    blocks_.append(lb);
                    buildBranch(lb);
                }
            }
        }
    }
}

void ScheduleDialog::buildTree()
{
    // First look for all input blocks
    for (QPtrListIterator<AbstractModel> it(*project_->blocks()); it != 0; ++it) {
        if (INSTANCEOF(*it, BlockModel))  {
            BlockModel* bm = dynamic_cast<BlockModel*>(*it);
            if (!bm->hasInputPins() && !bm->hasEpisodicPins()) {
                BlockTree* bt = new BlockTree(bm);
                inputBlocks.append(bt);
                if (!blocks_.contains(bt)) {
                    blocks_.append(bt);
                }
            }
        }
    }

    // Then built the trees from the input blocks on
    blocksToTree_.clear();
    for (QPtrListIterator<BlockTree> inpit(inputBlocks); inpit != 0; ++inpit) {
        buildBranch(*inpit);
    }
}

void ScheduleDialog::updateModel()
{
    for (QPtrListIterator<BlockTree> it(blocks_); it != 0; ++it) {
        (*it)->commit();
    }
    project_->setModified(project_->isModified() || modified_);
}

void ScheduleDialog::fillTimingTable(BlockTree* bt)
{
    int i = timingTable->numRows();
    timingTable->setNumRows(i + 1);

    QTableItem *i1 = new QTableItem(timingTable, QTableItem::Never,
        bt->getBlock()->name());
    timingTable->setItem(i, 0, i1);

    QTableItem *i2 = new SpinBoxItem(timingTable, QTableItem::OnTyping, bt, SpinBoxItem::RUNTIME);
    timingTable->setItem(i, 1, i2);

    QTableItem *i3 = new SpinBoxItem(timingTable, QTableItem::OnTyping, bt, SpinBoxItem::CLOCK);
    timingTable->setItem(i, 2, i3);

    QTableItem *i4 = new SpinBoxItem(timingTable, QTableItem::OnTyping, bt, SpinBoxItem::OFFSET);
    timingTable->setItem(i, 3, i4);
}

void ScheduleDialog::initLayout()
{
    dialogLayout = new QVBoxLayout(this, WIDGET_SPACING);
    splitterWidget = new QSplitter(QSplitter::Vertical, this);
    topWidget = new QWidget(splitterWidget);
    topLayout = new QHBoxLayout(topWidget, WIDGET_SPACING);
    middleWidget = new QWidget(splitterWidget);
    middleLayout = new QHBoxLayout(middleWidget, 0);
    bottomWidget = new QWidget(this);

    initTimingWidget();
    initGraphWidget();
    initBottomWidget();

    dialogLayout->addWidget(splitterWidget);
    dialogLayout->addWidget(bottomWidget);
}

void ScheduleDialog::initTimingWidget()
{
    buildTree();
    timingTable = new QTable(0, 4, topWidget, "timingWidget");
    timingTable->horizontalHeader()->setLabel(0, tr( "Block" ));
    timingTable->horizontalHeader()->setLabel(1, tr( "Laufzeit" ));
    timingTable->horizontalHeader()->setLabel(2, tr( "Takt" ));
    timingTable->horizontalHeader()->setLabel(3, tr( "Offset" ));

    timingTable->setSelectionMode(QTable::SingleRow);
    timingTable->setReadOnly(false);
    timingTable->setFocusStyle(QTable::FollowStyle);
    timingTable->setRowMovingEnabled(true);

    connect(timingTable, SIGNAL(valueChanged(int, int)),
            this, SLOT(modelChanged(int,int)));

    connect(timingTable->verticalHeader(), SIGNAL(indexChange(int, int, int)),
            this, SLOT(rowMoved(int, int, int)));

    for (QPtrListIterator<BlockTree> it(blocks_); it != 0; ++it) {
        fillTimingTable(*it);
    }
    topLayout->addWidget(timingTable);
}

void ScheduleDialog::initGraphWidget()
{
    labelCanvas = new QCanvas();
    labelCanvas->setBackgroundColor(lightGray);
    labelCanvasView = new QCanvasView(labelCanvas, middleWidget);
    labelCanvasView->setVScrollBarMode(QScrollView::AlwaysOff);
    labelCanvasView->setHScrollBarMode(QScrollView::AlwaysOn);
    middleLayout->addWidget(labelCanvasView);

    canvas = new QCanvas();
    canvas->setBackgroundColor(QObject::white);
    canvasView = new QCanvasView(canvas, middleWidget);

    middleLayout->addWidget(canvasView);

    connect(canvasView, SIGNAL(contentsMoving(int, int)),
            labelCanvasView, SLOT(setContentsPos(int, int)));

    // determine canvas size
    int cnt = blocks_.count();

    canvas->resize(CANVAS_WIDTH,
                   RULER_TICK + cnt * (BOX_HEIGHT + BOX_YSPACING));
    labelCanvas->resize(50, RULER_TICK + cnt * (BOX_HEIGHT + BOX_YSPACING));

    initCanvas();
}

void ScheduleDialog::initCanvas()
{
    drawRuler();
    for (QPtrListIterator<BlockTree> it(blocks_); it != 0; ++it) {
        drawTimings(*it);
    }

    // create highlighter
    highlightCanvasRectangle = new QCanvasRectangle(canvas);
    highlightCanvasRectangle->setSize(canvas->width(),
                                      BOX_HEIGHT + BOX_YSPACING);
    highlightCanvasRectangle->setBrush(QBrush(qApp->palette().active().highlight()));
    highlightCanvasRectangle->setPen(QPen(white));
    highlightCanvasRectangle->move(0, RULER_HEIGHT - BOX_YSPACING / 2);
    highlightCanvasRectangle->setZ(0);
    highlightCanvasRectangle->show();

    connect(timingTable, SIGNAL(currentChanged(int, int)),
            this, SLOT(updateHighlighter(int, int)));

    canvas->update();
    labelCanvas->update();
}

void ScheduleDialog::clearCanvas()
{
    QCanvasItemList::iterator it;
    QCanvasItemList canvasItems = canvas->allItems();
    for (it = canvasItems.begin(); it != canvasItems.end(); ++it) {
        delete *it;
    }

    canvasItems = labelCanvas->allItems();
    for (it = canvasItems.begin(); it != canvasItems.end(); ++it) {
        delete *it;
    }
}

void ScheduleDialog::drawRuler()
{
    // draw ruler
    int x = WIDGET_SPACING;
    QCanvasText *text = new QCanvasText(QString::number(RULER_TICK)+" ns",
                                        canvas);
    text->move(x, 1);
    text->setColor(gray);

    text->show();
    while (x < canvas->width()) {
        x += (int)(PIX_PER_NS * zoom_ * RULER_TICK);
        // draw short line
        QCanvasLine *tick = new QCanvasLine(canvas);
        tick->setPoints(x, 0, x, 10);
        tick->setPen(gray);
        tick->show();
        // draw long line
        tick = new QCanvasLine(canvas);
        tick->setPoints(x, 10, x, canvas->height());
        tick->setPen(lightGray);
        tick->show();
    }
}

void ScheduleDialog::drawTimings(BlockTree* bt)
{
    // draw labels
    QCanvasText* text = new QCanvasText(bt->getBlock()->name(),
                                        labelCanvas);
    int line = blocks_.find(bt);
    int y = line * (BOX_HEIGHT + BOX_YSPACING) + RULER_HEIGHT;
    text->move(WIDGET_SPACING, y);
    text->show();

    // resize canvas if label doesn't fit.
    if ((int)(text->boundingRect().width() + 2 * WIDGET_SPACING)
        > labelCanvas->width()) {

        labelCanvas->resize(text->boundingRect().width()
                            + (2 * WIDGET_SPACING), labelCanvas->height());
    }

    // check if block is configured correctly
    if (bt->getClock() <= 0) {
        // draw info and skip
        QCanvasRectangle *box = new QCanvasRectangle(canvas);
        box->setSize(canvas->width(),
                     BOX_HEIGHT + BOX_YSPACING);
        box->setBrush(white);
        box->setPen(lightGray);
        box->move(0, RULER_HEIGHT - BOX_YSPACING
                  / 2 + line * (BOX_YSPACING + BOX_HEIGHT));
        box->setZ(2);
        box->show();

        QCanvasText *text = new QCanvasText(tr("Missing clock value."), canvas);
        text->move(WIDGET_SPACING, y);
        text->setColor(black);
        text->setZ(3);
        text->show();
        return;
    }

    // draw blocks
    int t = bt->getOffset();
    double X = t * PIX_PER_NS;
    while (X < canvas->width()) {
        // draw block
        QRect thisBlock = calcBlockPosition(bt, t);
        QCanvasRectangle* box = new QCanvasRectangle(thisBlock, canvas);
        box->setBrush(QBrush(white));
        box->setZ(2);
        box->show();

        // draw lines for all connected 'children'
        for (QPtrListIterator<BlockTree> it(*bt->getBranches()); it != 0; ++it) {
            BlockTree *target = *it;

            // block lookup
            if (target->getBackReference()) {
                target = blocksToTree_[target->getBlock()];
            }

            // skip if child has no clock
            if (target->getClock() <= 0) {
                continue;
            }

            // find next start time for the target block
            int targetTime = target->getOffset();
            while (targetTime <= t + bt->getRuntime()) {
                targetTime += target->getClock();
            }

            // calculate position of the target block
            QRect targetBlock = calcBlockPosition(target, targetTime);

            // draw a line with arrowhead between this block and the next
            // target block.
            QCanvasLine *line = new ArrowLine(canvas);
            line->setPoints(thisBlock.right(),
                            thisBlock.y() + thisBlock.height(),
                            targetBlock.x(),
                            targetBlock.y());
            line->setZ(1);
            line->show();

        }

        t += bt->getClock();
        X = rint(t * PIX_PER_NS);
    }

    return;
}

QRect ScheduleDialog::calcBlockPosition(BlockTree *bt, int time)
{
    int line = blocks_.find(bt);
    Q_ASSERT(line != -1);

    int x = (int)(WIDGET_SPACING + rint(time * PIX_PER_NS * zoom_));
    int y = line * (BOX_HEIGHT + BOX_YSPACING) + RULER_HEIGHT;
    int w = (int)QMAX(5, rint(bt->getRuntime() * PIX_PER_NS * zoom_));
    int h = BOX_HEIGHT;

    return QRect(x, y, w, h);
}

void ScheduleDialog::initBottomWidget()
{
    QBoxLayout *bottomLayout = new QHBoxLayout(bottomWidget, WIDGET_SPACING);

    // ok button
    okPushButton = new QPushButton(bottomWidget, "okPushButton");
    okPushButton->setText(tr("&OK"));
    okPushButton->setDefault(TRUE);
    connect(okPushButton, SIGNAL(clicked()), this, SLOT(ok()));

    // help button
    helpPushButton = new QPushButton(bottomWidget, "helpPushButton");
    helpPushButton->setText(tr("&Help"));

    // apply button
    applyPushButton = new QPushButton(bottomWidget, "applyPushButton");
    applyPushButton->setText(tr("&Apply"));
    connect(applyPushButton, SIGNAL(clicked()), this, SLOT(apply()));

    // cancel button
    cancelPushButton = new QPushButton(bottomWidget, "cancelPushButton" );
    cancelPushButton->setText(tr("&Cancel"));
    connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(cancel()));

    // zoom slider
    zoomSlider = new QSlider(Horizontal, bottomWidget, "slider");
    zoomSlider->setRange(0, 99);
    zoomSlider->setValue(0);
    connect(zoomSlider, SIGNAL(valueChanged(int)),
            SLOT(zoomChanged(int)));


    bottomLayout->addWidget(zoomSlider);

    bottomLayout->addWidget(okPushButton);
    bottomLayout->addWidget(helpPushButton);
    bottomLayout->addWidget(applyPushButton);
    bottomLayout->addWidget(cancelPushButton);
}

void ScheduleDialog::cancel()
{
    reject();
}

void ScheduleDialog::apply()
{
    updateModel();
}

void ScheduleDialog::ok()
{
    updateModel();
    accept();
}

void ScheduleDialog::updateHighlighter(int row, int)
{
    if (row >= 0) {
        highlightCanvasRectangle->move
            (0, RULER_HEIGHT - BOX_YSPACING / 2 + row * (BOX_YSPACING + BOX_HEIGHT));
        canvas->update();
    }
}

void ScheduleDialog::zoomChanged(int zoom)
{
    zoom_ = 1.0 + (zoom/5);
    clearCanvas();
    initCanvas();
}

void ScheduleDialog::modelChanged(int, int)
{
    modified_ = true;
    clearCanvas();
    initCanvas();
}

void ScheduleDialog::rowMoved(int section, int fromIndex, int toIndex)
{
    qDebug("Move: Section:"+QString::number(section)+",from: "+QString::number(fromIndex)+", to: "+QString::number(toIndex));
    //    inputBlocks.insert(toIndex, inputBlocks.take(fromIndex));
    modelChanged(0,0);
}

SpinBoxItem::SpinBoxItem(QTable *t, EditType et, BlockTree *bt, BTField field )
    : QTableItem(t, et, "0"), spinbox_(0), blocktree_(bt), field_(field)
{
    setText(QString::number(value()));
    // we do not want this item to be replaced
    setReplaceable(false);
}

QWidget *SpinBoxItem::createEditor() const
{
    // create a spinbox editor
    ((SpinBoxItem*)this)->spinbox_ = new QSpinBox(table()->viewport());
    spinbox_->setValue(value());
    spinbox_->setRange(0, INT_MAX);
    return spinbox_;
}

void SpinBoxItem::setContentFromEditor( QWidget *w )
{
    if ( w->inherits( "QSpinBox" )) {
        setText(QString::number(((QSpinBox*)w)->value()));
        setValue(((QSpinBox*)w)->value());
    } else {
        QTableItem::setContentFromEditor(w);
    }
}

int SpinBoxItem::value() const
{
    switch (field_) {
    case RUNTIME: return blocktree_->getRuntime();
    case CLOCK: return blocktree_->getClock();
    case OFFSET: return blocktree_->getOffset();
    default: return 0;
    }
}

void SpinBoxItem::setValue(int value)
{
    switch (field_) {
    case RUNTIME: blocktree_->setRuntime(value); break;
    case CLOCK: blocktree_->setClock(value); break;
    case OFFSET: blocktree_->setOffset(value); break;
    }
}

ArrowLine::ArrowLine(QCanvas *canvas)
    : QCanvasLine(canvas)
{
}

void ArrowLine::drawShape(QPainter &p)
{
    p.setPen(darkGray);
    QCanvasLine::drawShape(p);

    double angle = computeAngle(startPoint().x(),
                                startPoint().y(),
                                endPoint().x(),
                                endPoint().y());
    QPointArray pts(3);

    QWMatrix m;
    int x, y;
    m.rotate(angle);
    m.map(-5, -2, &x, &y);
    pts.setPoint(0, x, y);
    m.map(-5, 2, &x, &y);
    pts.setPoint(1, x, y);
    m.map(0, 0, &x, &y);
    pts.setPoint(2, x, y);

    pts.translate(endPoint().x(), endPoint().y());

    p.setBrush(QColor(darkGray));
    p.drawPolygon(pts);
}

double ArrowLine::computeAngle(int sx, int sy, int ex, int ey)
{

    double run = ex - sx;
    double rise = ey - sy;

    double angle;
    if (run == 0)
        if (rise < 0)
            angle = 270;
        else
            angle = 90;
    else
        {
            angle = RAD2DEG * atan(rise/run);
            if (run < 0)
                angle += 180;
            if (rise < 0)
                angle += 360;
        }
    return angle;
}
