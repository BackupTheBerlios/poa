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
 * $Id: scheduledialog.cpp,v 1.1 2004/01/05 15:48:49 kilgus Exp $
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
#include <qtable.h> 
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

const int CANVAS_WIDTH = 800;		// Canvas width
const int X_ORIGIN = 50;			// Blocks start at this origin
const int BOX_HEIGHT = 10;			// Height of one box in diagram
const int BOX_YSPACING = 20;		// Space between two boxes
const double PIX_PER_NS = 1.0;		// Pixels per nanosecond

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
			} else {	// No mux, straight connection
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

	QTableItem *i2 = new QTableItem(timingTable, QTableItem::Never, 
		QString::number(bt->getRuntime()));
	timingTable->setItem(i, 1, i2);

	QTableItem *i3 = new QTableItem(timingTable, QTableItem::Never, 
		QString::number(bt->getClock()));
	timingTable->setItem(i, 2, i3);

	QTableItem *i4 = new QTableItem(timingTable, QTableItem::Never, 
		QString::number(bt->getOffset()));
	timingTable->setItem(i, 3, i4);

	for (QPtrListIterator<BlockTree> it(*bt->getBranches()); it != 0; ++it) {
		if (!(*it)->getBackReference()) 
			fillTimingTable(*it);
	}
}

void ScheduleDialog::initLayout()
{
    dialogLayout = new QVBoxLayout(this, WIDGET_SPACING);
    topWidget = new QWidget(this);
    topLayout = new QHBoxLayout(topWidget, WIDGET_SPACING);
    middleWidget = new QWidget(this);
    middleLayout = new QVBoxLayout(middleWidget, WIDGET_SPACING);
    bottomWidget = new QWidget(this);

    initTimingWidget();
    initGraphWidget();
    initBottomWidget();

    dialogLayout->addWidget(topWidget);
    dialogLayout->addWidget(middleWidget);
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

	for (QPtrListIterator<BlockTree> it(inputBlocks); it != 0; ++it) {
		fillTimingTable(*it);
    }
    topLayout->addWidget(timingTable);
}

void ScheduleDialog::initGraphWidget()
{
	canvas = new QCanvas();
	canvas->setBackgroundColor(QObject::white);
	canvasView = new QCanvasView(canvas, middleWidget);
	middleLayout->addWidget(canvasView);

	// determine canvas size
	int cnt = 0;
	int worstTime = 0;
	for (QPtrListIterator<BlockTree> inpit(inputBlocks); inpit != 0; ++inpit) {
		cnt += (*inpit)->count();

    }
	canvas->resize(CANVAS_WIDTH, cnt * (BOX_HEIGHT + BOX_YSPACING));

	int Y = 0;
	for (QPtrListIterator<BlockTree> it(inputBlocks); it != 0; ++it) {
		int time = 0;
		drawTimings(*it, &Y, &time);
	}
}

bool ScheduleDialog::drawTimings(BlockTree* bt, int* Y, int* time)
{
	if (*time == 0) {
		QCanvasText* text = new QCanvasText(bt->getBlock()->name(), canvas);
		text->move(0, *Y);
		text->show();
	}

	int t = bt->getOffset();

	int X = rint(t * PIX_PER_NS + 50);
	while (X < canvas->width()) {

		if (bt->getClock() <= 0) {
			return false;
		}

		QCanvasRectangle* box = new QCanvasRectangle(0, 0, 0, 0, canvas);
		box->move(X, *Y);
		box->setSize(rint(/*bt->getRuntime() * PIX_PER_NS*/ 20), BOX_HEIGHT);
		box->show();

//		QCanvasLine* line = new QCanvasLine(canvas);
//		(*lit)->setPoints(100, 100, 150, 150);
//		(*lit)->show();

		t += bt->getClock();
		X = rint(t * PIX_PER_NS + 50);
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

