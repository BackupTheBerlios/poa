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
 * $Id: scheduledialog.cpp,v 1.62 2004/03/01 12:07:36 kilgus Exp $
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
#include <qpaintdevicemetrics.h> 
#include <qpixmap.h>
#include <qlayout.h>
#include <qslider.h>
#include <qsplitter.h>
#include <qtable.h>
#include <qwmatrix.h>
#include <cmath>
#include <qtoolbutton.h>

#include "scheduledialog.h"
#include "canvasview.h"
#include "muxmodel.h"
#include "codemanager.h"
#include "cpumodel.h"
#include "mainwindow.h"
#include "autoschedulingdialog.h"
#include "pixmapbutton.h"
#include "poa.h"
#include "project.h"
#include "util.h"
#include "printmanager.h"

const int CANVAS_WIDTH = 800;       // Canvas width
const int PRINT_ITEM_HEIGHT = 20;   // Table item height
const int PRINT_NAME_WIDTH = 100;   // Width of name space left of graph
const int PRINT_SPACING = 5;        // Space e.g. between lines and text
const int BOX_HEIGHT = 12;          // Height of one box in diagram
const int BOX_YSPACING = 20;        // Space between two boxes
const int RULER_HEIGHT = 25;
const int BLOCKS_PER_CANVAS = 10;
const int RULER_SNAP = 250;         // nanoseconds to snap the rulerbar to.
const int RULER_INTERVAL = 75;      // show a rulerbar every X pixels (+snap)
const double RAD2DEG = 57.2958;

///////////////////////////////////////////////////////////////////////////////

ScheduleDialog::ScheduleDialog(GridCanvas *canvas, Project* pro, QWidget* parent, const char* name,
                               bool modal, WFlags fl)
    : QDialog(parent, name, modal, fl)
{
    if (!name) {
        setName("ScheduleDialog");
    }
    resize(700, 500);
    setCaption(tr("Scheduling"));

    project_ = pro;
    colormanager_ = canvas->colorManager();
    graph_ = 0;
    modified_ = false;
    zoom_ = 1.0;

    initLayout();
}

ScheduleDialog::~ScheduleDialog()
{
    if (graph_ != 0) {
        delete graph_;
    }
}

void ScheduleDialog::updateModel()
{
    for (QValueList<BlockNode*>::Iterator it = blocks_.begin();
         it != blocks_.end(); ++it) {

        (*it)->commit();
    }

    if (modified_) {
        project_->setModified(true);
    }
}

void ScheduleDialog::fillTimingTable(BlockNode* node)
{
    int i = timingTable->numRows();
    timingTable->setNumRows(i + 1);

    QTableItem *i1 = new QTableItem(timingTable, QTableItem::Never,
                                    node->model()->name());
    timingTable->setItem(i, 0, i1);

    QTableItem *i2 = new SpinBoxItem(timingTable, QTableItem::OnTyping, node,
                                     SpinBoxItem::RUNTIME);
    timingTable->setItem(i, 1, i2);

    QTableItem *i3 = new SpinBoxItem(timingTable, QTableItem::OnTyping, node,
                                     SpinBoxItem::CLOCK);
    timingTable->setItem(i, 2, i3);

    QTableItem *i4 = new SpinBoxItem(timingTable, QTableItem::OnTyping, node,
                                     SpinBoxItem::OFFSET);
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
    graph_ = new BlockGraph(project_);
    blocks_ = graph_->blocks();

    // remove all MuxModel nodes from list
    QValueList<BlockNode*>::Iterator it;
    for (it = blocks_.begin(); it != blocks_.end(); ++it) {
        if (INSTANCEOF((*it)->model(), MuxModel)) {
            it = blocks_.remove(it);
        }
    }

    timingTable = new QTable(0, 4, topWidget, "timingWidget");
    timingTable->horizontalHeader()->setLabel(0, tr( "Block" ));
    timingTable->horizontalHeader()->setLabel(1, tr( "Runtime" ));
    timingTable->horizontalHeader()->setLabel(2, tr( "Clock" ));
    timingTable->horizontalHeader()->setLabel(3, tr( "Offset" ));

    timingTable->setSelectionMode(QTable::SingleRow);
    timingTable->setReadOnly(false);
    timingTable->setFocusStyle(QTable::FollowStyle);
    timingTable->setRowMovingEnabled(false);

    connect(timingTable, SIGNAL(valueChanged(int, int)),
            this, SLOT(modelChanged(int,int)));

    for (it = blocks_.begin(); it != blocks_.end(); ++it) {
        fillTimingTable(*it);
    }

    topLayout->addWidget(timingTable);

    rightWidget_ = new QWidget(topWidget);
    rightLayout_ = new QVBoxLayout(rightWidget_);
    topLayout->addWidget(rightWidget_);

    QPushButton *upPushButton_
        = new PixmapButton(QPixmap(Util::findIcon("1uparrow.png")),
                           rightWidget_);
    connect(upPushButton_, SIGNAL(clicked()), this, SLOT(moveRowUp()));

    QPushButton *downPushButton_
        = new PixmapButton(QPixmap(Util::findIcon("1downarrow.png")),
                          rightWidget_);
    connect(downPushButton_, SIGNAL(clicked()), this, SLOT(moveRowDown()));

    rightLayout_->addStretch(1);
    rightLayout_->addWidget(upPushButton_);
    rightLayout_->addWidget(downPushButton_);
    rightLayout_->addStretch(1);
}

void ScheduleDialog::initGraphWidget()
{
    int height = RULER_HEIGHT + blocks_.count() * (BOX_HEIGHT + BOX_YSPACING);

    labelCanvas = new QCanvas();
    labelCanvas->setBackgroundColor(lightGray);
    labelCanvasView = new FitCanvasView(labelCanvas, middleWidget, height);
    labelCanvasView->setVScrollBarMode(QScrollView::AlwaysOff);
    labelCanvasView->setHScrollBarMode(QScrollView::AlwaysOn);
    middleLayout->addWidget(labelCanvasView);

    canvas = new QCanvas();
    canvas->setBackgroundColor(QObject::white);
    canvasView = new FitCanvasView(canvas, middleWidget, height);

    middleLayout->addWidget(canvasView);

    connect(canvasView, SIGNAL(contentsMoving(int, int)),
            labelCanvasView, SLOT(setContentsPos(int, int)));

    canvas->resize(CANVAS_WIDTH, height);
    labelCanvas->resize(50, height);
    initCanvas();
}

void ScheduleDialog::initCanvas()
{
    DrawProperties p;

    p.canvas = canvas;
    p.nameWidth = 0;        // name labels are on other canvas
    p.x = WIDGET_SPACING;
    p.y = 0;
    p.width = canvas->width();
    p.height = canvas->height();

    // calculate pix per ns (find block with highest clock to draw it
    // BLOCKS_PER_CANVAS times.
    unsigned int max_clock = 0;
    QValueList<BlockNode*>::Iterator it;
    for (it = blocks_.begin(); it != blocks_.end(); ++it) {
        max_clock = QMAX(max_clock, (*it)->clock());
    }
    p.pixPerNs = (max_clock > 0)
        ? (double)p.width / (max_clock * BLOCKS_PER_CANVAS)
        : 1.0;

    drawRuler(&p);
    for (it = blocks_.begin(); it != blocks_.end(); ++it) {
        drawTimings(&p, *it);
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

void ScheduleDialog::drawRuler(DrawProperties *p)
{
    // draw ruler
    QFont font = QApplication::font();
    font.setPointSize(font.pointSize() - 2);

    int x = p->x + p->nameWidth;
    int currNs = 0;

    double nsPer100 = RULER_INTERVAL * (1.0 / (p->pixPerNs * zoom_));
    int rulerTick = (((int)nsPer100 / RULER_SNAP)+1) * RULER_SNAP;

    while (x < p->width) {
        x += (int)(p->pixPerNs * zoom_ * rulerTick);
        currNs += rulerTick;

        // draw short line
        QCanvasLine *tick = new QCanvasLine(p->canvas);
        tick->setPoints(x, p->y, x, p->y + 10);
        tick->setPen(gray);
        tick->show();

        // draw long line
        tick = new QCanvasLine(p->canvas);
        tick->setPoints(x, p->y + 10, x, p->y + p->height);
        tick->setPen(lightGray);
        tick->show();

        QCanvasText *text = new QCanvasText(formatTimeProperly(currNs),
                                            p->canvas);
        text->move(x + WIDGET_SPACING / 2, p->y + 1);
        text->setColor(gray);

        text->setFont(font);

        text->show();

    }
}

void ScheduleDialog::drawTimings(DrawProperties *p, BlockNode* node)
{
    int line = blocks_.findIndex(node);
    int y = p->y + line * (BOX_HEIGHT + BOX_YSPACING) + RULER_HEIGHT;
    // if nameWidth is set the name should be drawn within the graph canvas
    if (p->nameWidth == 0) {
        // draw labels
        QCanvasText* text = new QCanvasText(node->model()->name(), labelCanvas);
        text->move(WIDGET_SPACING, y);
        text->show();

        // resize canvas if label doesn't fit.
        if ((int)(text->boundingRect().width() + 2 * WIDGET_SPACING)
            > labelCanvas->width()) 
        {    
            labelCanvas->resize(text->boundingRect().width()
                                + (2 * WIDGET_SPACING), labelCanvas->height());
            labelCanvasView->setFixedWidth(labelCanvas->width() + 4);
        }
    } else {
        // draw labels
        QCanvasText* text = new QCanvasText(node->model()->name(), p->canvas);
        text->move(p->x, y);
        text->show();
    }

    // check if block is configured correctly
    if (node->clock() <= 0) {
        // draw info and skip
        QCanvasRectangle *box = new QCanvasRectangle(p->canvas);
        box->setSize(p->width - p->nameWidth, BOX_HEIGHT + BOX_YSPACING);
        box->setBrush(white);
        box->setPen(lightGray);
        box->move(p->x + p->nameWidth, RULER_HEIGHT - BOX_YSPACING
                  / 2 + line * (BOX_YSPACING + BOX_HEIGHT));
        box->setZ(2);
        box->show();

        QCanvasText *text = new QCanvasText(tr("Missing clock value."), 
            p->canvas);
        text->move(p->x + p->nameWidth, y);
        text->setColor(black);
        text->setZ(3);
        text->show();
        return;
    }

    // draw blocks
    int t = node->offset();
    double X = t * p->pixPerNs;
    while (X < p->width - p->nameWidth) {
        // draw block
        QRect thisBlock = calcBlockPosition(p, node, t);
        QCanvasRectangle* box = new FancyRectangle(thisBlock, p->canvas);
        //        box->setBrush(QBrush(white));
        box->setBrush(QBrush(colormanager_->color(node->model())));
        box->setZ(2);
        box->show();

        // draw lines for all connected 'children'
        QPtrList<BlockNode> neighbours = node->neighbours();
        for (QPtrListIterator<BlockNode> it(neighbours); it != 0; ++it) {
            BlockNode *target = *it;

            // skip if child has no clock
            if (target->clock() <= 0) {
                continue;
            }

            // find next start time for the target block
            unsigned int targetTime = target->offset();
            while (targetTime <= t + node->runtime()) {
                targetTime += target->clock();
            }

            // check if this block is the next source for the target block
            if (t + (2 * node->runtime()) + node->clock() < targetTime) {
                continue;
            }

            // calculate position of the target block
            QRect targetBlock = calcBlockPosition(p, target, targetTime);

            // draw a line with arrowhead between this block and the next
            // target block.
            QCanvasLine *line = new ArrowLine(p->canvas);
            line->setPoints(thisBlock.right(),
                            thisBlock.y() + thisBlock.height(),
                            targetBlock.x(),
                            targetBlock.y());
            line->setZ(1);
            line->show();
        }

        t += node->clock();
        X = rint(t * p->pixPerNs);
    }

    return;
}

QRect ScheduleDialog::calcBlockPosition(DrawProperties *p, 
    BlockNode *node, int time)
{
    int line = blocks_.findIndex(node);
    Q_ASSERT(line != -1);

    int x = p->x + p->nameWidth + (int)(rint(time * p->pixPerNs * zoom_));
    int y = p->y + line * (BOX_HEIGHT + BOX_YSPACING) + RULER_HEIGHT;
    int w = (int)QMAX(5, rint(node->runtime() * p->pixPerNs * zoom_));
    int h = BOX_HEIGHT;

    return QRect(x, y, w, h);
}

void ScheduleDialog::initBottomWidget()
{
    QBoxLayout *bottomLayout = new QHBoxLayout(bottomWidget, WIDGET_SPACING);

    // ok button
    QPushButton *okPushButton = new QPushButton(bottomWidget);
    okPushButton->setText(tr("&OK"));
    okPushButton->setDefault(TRUE);
    connect(okPushButton, SIGNAL(clicked()), this, SLOT(ok()));

    // auto button
    QPushButton *autoPushButton = new QPushButton(bottomWidget);
    autoPushButton->setText(tr("A&uto"));
    connect(autoPushButton, SIGNAL(clicked()), this, SLOT(autoSchedule()));

    // print button
    QPushButton *printPushButton = new QPushButton(bottomWidget);
    printPushButton->setText(tr("&Print"));
    connect(printPushButton, SIGNAL(clicked()), this, SLOT(print()));

    // apply button
    QPushButton *applyPushButton = new QPushButton(bottomWidget);
    applyPushButton->setText(tr("&Apply"));
    connect(applyPushButton, SIGNAL(clicked()), this, SLOT(apply()));

    // cancel button
    QPushButton *cancelPushButton = new QPushButton(bottomWidget);
    cancelPushButton->setText(tr("&Cancel"));
    connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(cancel()));

    // zoom slider
    zoomSlider = new QSlider(Horizontal, bottomWidget, "slider");
    zoomSlider->setRange(0, 200);
    zoomSlider->setValue(0);
    connect(zoomSlider, SIGNAL(valueChanged(int)),
            SLOT(zoomChanged(int)));

    bottomLayout->addWidget(zoomSlider);

    bottomLayout->addWidget(okPushButton);
    bottomLayout->addWidget(autoPushButton);
    bottomLayout->addWidget(printPushButton);
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

void ScheduleDialog::autoSchedule()
{
    AutoSchedulingDialog *dialog = new AutoSchedulingDialog(graph_);

    int result = dialog->exec();
    delete dialog;
    if (result == QDialog::Accepted) {
	int count = timingTable->numRows();
	for (int i = 0; i < count; ++i) {
	    static_cast<SpinBoxItem*>(timingTable->item(i, 3))->updateText();
	}
	clearCanvas();
	initCanvas();
    }
}

void ScheduleDialog::print()
{
    PrintManager printer(project_->name());
    if (!printer.setup()) return;

    QPaintDeviceMetrics *metrics = printer.getMetrics();    
    int printCanvasWidth = metrics->width();
    delete metrics;

    int tableHeight = (blocks_.count() + 1) * PRINT_ITEM_HEIGHT;
    QCanvas *printCanvas = new QCanvas(printCanvasWidth, 
        tableHeight + PRINT_ITEM_HEIGHT + canvas->height());

    // first draw timing table
    int y = 0;
    Q_ASSERT(timingTable->numCols() == 4);
    int xOfs[6] = {0,       // x offsets of the columns
                   printCanvasWidth * 1/20,
                   printCanvasWidth * 4/10, 
                   printCanvasWidth * 6/10, 
                   printCanvasWidth * 8/10,
                   printCanvasWidth - 1};

    QCanvasLine *line = new QCanvasLine(printCanvas);
    line->setPoints(xOfs[0], y, xOfs[5], y);
    line->setPen(gray);
    line->show();        

    // print header and vertical lines
    for (int i = 0; i <= 5; i++) {
        if ((i > 0) && (i < 5)) {
            QCanvasText* text = new QCanvasText(
                timingTable->horizontalHeader()->label(i - 1), printCanvas);
            text->move(xOfs[i] + PRINT_SPACING, y + PRINT_SPACING);
            text->show();
        }

        line = new QCanvasLine(printCanvas);
        line->setPoints(xOfs[i], y, xOfs[i], y + tableHeight);
        line->setPen(gray);
        line->show();        
    }
    y += PRINT_ITEM_HEIGHT;

    // now print contents
    int row = 0;
    QValueList<BlockNode*>::Iterator it;
    for (it = blocks_.begin(); it != blocks_.end(); ++it) {
        // number
        QCanvasText* text;
        text = new QCanvasText(QString::number(row + 1), printCanvas);
        text->move(xOfs[0] + PRINT_SPACING, y + PRINT_SPACING);
        text->show();

        // other contents
        for (int i = 1; i <= 4; i++) {
            text = new QCanvasText(timingTable->text(row, i - 1), printCanvas);
            text->move(xOfs[i] + PRINT_SPACING, y + PRINT_SPACING);
            text->show();
        }

        // horizontal line
        line = new QCanvasLine(printCanvas);
        line->setPoints(xOfs[0], y, xOfs[5], y);
        line->setPen(gray);
        line->show();

        y += PRINT_ITEM_HEIGHT;
        row++;
    }

    line = new QCanvasLine(printCanvas);
    line->setPoints(xOfs[0], y, xOfs[5], y);
    line->setPen(gray);
    line->show();
    y += PRINT_ITEM_HEIGHT;

    // properties for the graph    
    DrawProperties p;
    p.nameWidth = PRINT_NAME_WIDTH;
    p.x = 0;
    p.y = y;
    p.width = printCanvasWidth;
    p.height = canvas->height();
    p.canvas = printCanvas;

    // now draw graph
    unsigned int max_clock = 0;
    for (it = blocks_.begin(); it != blocks_.end(); ++it) {
        max_clock = QMAX(max_clock, (*it)->clock());
    }
    p.pixPerNs = (max_clock > 0)
        ? (double)canvas->width() / (max_clock * BLOCKS_PER_CANVAS)
        : 1.0;
    drawRuler(&p);
    for (it = blocks_.begin(); it != blocks_.end(); ++it) {
        drawTimings(&p, *it);
    }

    // finally print canvas
    printer.print(printCanvas);
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

void ScheduleDialog::swapRows(int index1, int index2)
{
    Q_ASSERT(index1 != index2);
    Q_ASSERT(index1 >= 0);
    Q_ASSERT(index1 < (int)blocks_.size());
    Q_ASSERT(index2 >= 0);
    Q_ASSERT(index2 < (int)blocks_.size());
    Q_ASSERT((int)blocks_.size() == timingTable->numRows());

    BlockNode *node = blocks_[index1];
    blocks_[index1] = blocks_[index2];
    blocks_[index2] = node;

    timingTable->swapRows(index1, index2);
    timingTable->clearSelection();

    timingTable->updateContents();

    // set row selection
    // doesnt work with qt 3.0.6:
    //    timingTable->selectRow(index2);
    // workaround:
    QTableSelection selection;
    selection.init(index2, 0);
    selection.expandTo(index2, timingTable->numCols() -1);
    timingTable->addSelection(selection);


    // redraw canvas
    clearCanvas();
    initCanvas();
    // sync selection
    updateHighlighter(index2, 0);
}

void ScheduleDialog::moveRowUp()
{
    int i = timingTable->currentRow();
    if (i > 0) {
        swapRows(i, i - 1);
    }
}

void ScheduleDialog::moveRowDown()
{
    int i = timingTable->currentRow();
    if (i < timingTable->numRows() - 1) {
        swapRows(i, i + 1);
    }
}

SpinBoxItem::SpinBoxItem(QTable *t, EditType et, BlockNode *node,
                         NodeField field)
    : QTableItem(t, et, "0"), node_(node), field_(field)
{
    setText(value());
    // we do not want this item to be replaced
    setReplaceable(false);
}

QWidget *SpinBoxItem::createEditor() const
{
    // create a spinbox editor
    QSpinBox *spinbox = new QSpinBox(table()->viewport());
    spinbox->setSuffix(" ns");
    if (field_ == OFFSET) {
        spinbox->setRange(-1, INT_MAX);
        spinbox->setValue(node_->autoOffset() ? -1 : (int)node_->offset());
        spinbox->setSpecialValueText
            (QString("Auto (%1 ns)").arg(node_->offset()));
    }
    else {
        spinbox->setRange(0, INT_MAX);
        spinbox->setValue(field_ == CLOCK
                           ? node_->clock()
                           : node_->runtime());
    }

    return spinbox;
}

void SpinBoxItem::setContentFromEditor( QWidget *w )
{
    if ( w->inherits( "QSpinBox" )) {
        setValue(((QSpinBox*)w)->value());
	updateText();
    } else {
        QTableItem::setContentFromEditor(w);
    }
}

void SpinBoxItem::updateText()
{
    setText(value());
}

QString SpinBoxItem::value() const
{
    switch (field_) {
    case RUNTIME: return formatTimeProperly(node_->runtime());
    case CLOCK: return formatTimeProperly(node_->clock());
    case OFFSET: return node_->autoOffset()
                     ? QString("Auto (%1)").
                     arg(formatTimeProperly(node_->offset()))
                     : formatTimeProperly(node_->offset());
    default: return QString::null;
    }
}

void SpinBoxItem::setValue(int value)
{
    switch (field_) {
    case RUNTIME: node_->setRuntime(value); break;
    case CLOCK: node_->setClock(value); break;
    case OFFSET:
        if (value != -1) {
            node_->setOffset(value);
            node_->setAutoOffset(false);
        }
        else {
            node_->setAutoOffset(true);
        }
        break;
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

FitCanvasView::FitCanvasView(QCanvas *canvas, QWidget *parent, int height)
    : QCanvasView(canvas, parent), height_(height)
{

}

void FitCanvasView::resizeEvent(QResizeEvent *event)
{
    QCanvasView::resizeEvent(event);

    if (visibleHeight() > height_) {
        canvas()->resize(canvas()->width(), visibleHeight());
    }
}

FancyRectangle::FancyRectangle(QRect rect, QCanvas *canvas)
    : QCanvasRectangle(rect, canvas)
{
}

void FancyRectangle::drawShape(QPainter &p)
{
    QPen oldPen = p.pen();

    // draw rectangle
    p.setPen(QPen(QColor(118, 118, 118), 1));
    p.drawRect((int)x(), (int)y(),
               width(), height());

    // draw decorations
    p.setPen(QPen(QColor(192, 192, 192), 1));
    p.drawPoint((int)x(), (int)y());
    p.drawPoint((int)x(), (int)y() + height() - 1);
    p.drawPoint((int)x() + width() - 1, (int)y());
    p.drawPoint((int)x() + width() - 1, (int)y() + height() - 1);

    //    p.setPen(QPen(QColor(196, 194, 205), 1));
    p.setPen(QPen(p.brush().color().dark(120), 1));
    p.drawLine((int)x() + 1, (int)y() + height() - 2,
               (int)x() + width() - 2, (int)y() + height() - 2);
    p.drawLine((int)x() + width() - 2, (int)y() + height() - 1,
               (int)x() + width() - 2, (int)y() + 1);

    p.setPen(oldPen);
}
