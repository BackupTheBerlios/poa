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
 * $Id: scheduledialog.cpp,v 1.5 2004/01/09 22:04:45 squig Exp $
 *
 *****************************************************************************/

#include <qvariant.h>
#include <qcanvas.h>
#include <qheader.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qptrlist.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qimage.h>
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
const double PIX_PER_NS = 1.0;      // Pixels per nanosecond

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
                    if (!bt->contains(block)) {
                        buildBranch(bt->addBranch(block));
                    } else {
                        // Add back reference only and stop recursion
                        bt->addBranch(block)->setBackReference(true);
                    }
                }
            } else {    // No mux, straight connection
                // Only add model recursively if not already in tree
                if (!bt->contains(block)) {
                    buildBranch(bt->addBranch(block));
                } else {
                    // Add back reference only and stop recursion
                    bt->addBranch(block)->setBackReference(true);
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
            }
        }
    }

    // Then built the trees from the input blocks on
    for (QPtrListIterator<BlockTree> inpit(inputBlocks); inpit != 0; ++inpit) {
        buildBranch(*inpit);
    }
}

void ScheduleDialog::fillTimingTable(BlockTree* bt)
{
    int i = timingTable->numRows();
    timingTable->setNumRows(i + 1);

    QTableItem *i1 = new QTableItem(timingTable, QTableItem::Never,
        bt->getBlock()->name());
    timingTable->setItem(i, 0, i1);

    //    QTableItem *i2 = new SpinBoxItem(timingTable, QTableItem::OnTyping,
    //  QString::number(bt->getRuntime()));
    QTableItem *i2 = new SpinBoxItem(timingTable, QTableItem::OnTyping, bt, SpinBoxItem::RUNTIME);
    timingTable->setItem(i, 1, i2);

    //    QTableItem *i3 = new SpinBoxItem(timingTable, QTableItem::OnTyping,
    //        QString::number(bt->getClock()));
    QTableItem *i3 = new SpinBoxItem(timingTable, QTableItem::OnTyping, bt, SpinBoxItem::CLOCK);
    timingTable->setItem(i, 2, i3);

    //    QTableItem *i4 = new SpinBoxItem(timingTable, QTableItem::OnTyping,
    //        QString::number(bt->getOffset()));
    QTableItem *i4 = new SpinBoxItem(timingTable, QTableItem::OnTyping, bt, SpinBoxItem::OFFSET);
    timingTable->setItem(i, 3, i4);

    for (QPtrListIterator<BlockTree> it(*bt->getBranches()); it != 0; ++it) {
        if (!(*it)->getBackReference())
            fillTimingTable(*it);
    }
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

    //    dialogLayout->addWidget(topWidget);
    //    dialogLayout->addWidget(middleWidget);
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

    for (QPtrListIterator<BlockTree> it(inputBlocks); it != 0; ++it) {
        fillTimingTable(*it);
    }
    topLayout->addWidget(timingTable);
}

void ScheduleDialog::initGraphWidget()
{
    labelCanvas = new QCanvas();
    labelCanvas->setBackgroundColor(QObject::lightGray);
    labelCanvasView = new QCanvasView(labelCanvas, middleWidget);
    labelCanvasView->setVScrollBarMode(QScrollView::AlwaysOff);
    labelCanvasView->setHScrollBarMode(QScrollView::AlwaysOn);
    middleLayout->addWidget(labelCanvasView);

    canvas = new QCanvas();
    canvas->setBackgroundColor(QObject::white);
    canvasView = new QCanvasView(canvas, middleWidget);
    zoomMatrix = QWMatrix(1,0,0,1,0,0);
    middleLayout->addWidget(canvasView);

    connect(canvasView, SIGNAL(contentsMoving(int, int)),
            labelCanvasView, SLOT(setContentsPos(int, int)));

    // determine canvas size
    int cnt = 0;
    //int worstTime = 0;
    for (QPtrListIterator<BlockTree> inpit(inputBlocks); inpit != 0; ++inpit) {
        cnt += (*inpit)->count();

    }
    canvas->resize(CANVAS_WIDTH,
                   BOX_YSPACING + cnt * (BOX_HEIGHT + BOX_YSPACING));
    labelCanvas->resize(50, BOX_YSPACING + cnt * (BOX_HEIGHT + BOX_YSPACING));

    int Y = BOX_YSPACING;
    for (QPtrListIterator<BlockTree> it(inputBlocks); it != 0; ++it) {
        int time = 0;
        drawTimings(*it, &Y, &time);
    }

    // create highlighter
    highlightCanvasRectangle = new QCanvasRectangle(canvas);
    highlightCanvasRectangle->setSize(canvas->width(),
                                      BOX_HEIGHT + BOX_YSPACING);
    highlightCanvasRectangle->setBrush(QBrush(lightGray));
    highlightCanvasRectangle->setPen(QPen(white));
    highlightCanvasRectangle->move(0, BOX_YSPACING / 2);
    highlightCanvasRectangle->setZ(0);
    highlightCanvasRectangle->show();

    connect(timingTable, SIGNAL(currentChanged(int, int)),
            this, SLOT(updateHighlighter(int, int)));
}

bool ScheduleDialog::drawTimings(BlockTree* bt, int* Y, int* time)
{
    if (*time == 0) {
        QCanvasText* text = new QCanvasText(bt->getBlock()->name(), labelCanvas);
        text->move(WIDGET_SPACING, *Y);
        text->show();

        // resize label canvas if label doesnt fit.
        if (text->boundingRect().width() +
            (2 * WIDGET_SPACING) > labelCanvas->width()) {
            qDebug("Resize canvas");
            labelCanvas->resize(text->boundingRect().width()
                                + (2 * WIDGET_SPACING), labelCanvas->height());
        }
    }

    int t = bt->getOffset();

    int X = WIDGET_SPACING + rint(t * PIX_PER_NS);
    while (X < canvas->width()) {

        if (bt->getClock() <= 0) {
            return false;
        }

        QCanvasRectangle* box = new QCanvasRectangle(0, 0, 0, 0, canvas);
        box->move(X, *Y);
        int w = QMAX(5, rint(bt->getRuntime() * PIX_PER_NS));
        box->setBrush(QBrush(white));
        box->setSize(w, BOX_HEIGHT);
        box->setZ(1);
        box->show();

//      QCanvasLine* line = new QCanvasLine(canvas);
//      (*lit)->setPoints(100, 100, 150, 150);
//      (*lit)->show();

        t += bt->getClock();
        X = rint(t * PIX_PER_NS);
    }

    *Y += BOX_HEIGHT + BOX_YSPACING;

    for (QPtrListIterator<BlockTree> it(*bt->getBranches()); it != 0; ++it) {
        drawTimings(*it, Y, time);
    }

    return true;
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
    zoomSlider = new QSlider( Horizontal, bottomWidget, "slider" );
    zoomSlider->setRange( 0, 99 );
    zoomSlider->setValue( 0 );
    connect( zoomSlider, SIGNAL(valueChanged(int)),
                 SLOT(zoomChanged(int)) );


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
//    updateModel();
}

void ScheduleDialog::ok()
{
//    updateModel();
    accept();
}

void ScheduleDialog::updateHighlighter(int row, int)
{
    highlightCanvasRectangle->move
        (0, BOX_YSPACING / 2 + row * (BOX_YSPACING + BOX_HEIGHT));
    canvas->update();
}

void ScheduleDialog::zoomChanged(int zoom)
{
    zoomMatrix.setMatrix(1.0 + (zoom/5),0,0,1,0,0);
    canvasView->setWorldMatrix(zoomMatrix);
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
    QObject::connect(spinbox_, SIGNAL(valueChanged(int)), table(), SLOT(doValueChanged()));
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
