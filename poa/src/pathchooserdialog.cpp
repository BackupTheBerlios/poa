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
 * $Id: pathchooserdialog.cpp,v 1.2 2004/02/10 17:09:05 kilgus Exp $
 *
 *****************************************************************************/

#include "pathchooserdialog.h"

#include "blockmodel.h"
#include "blockgraph.h"

#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>


PathChooserDialog::PathChooserDialog(BlockGraph *graph)
{
    Q_ASSERT(graph != 0);
    graph_ = graph;
    outPins_ = 0;
    inPins_ = 0;
    blocks_ = 0;

    setCaption(tr("Path Chooser"));
    QBoxLayout *topPane = new QVBoxLayout(this, 10, 5);

    QGridLayout *gridLayout = new QGridLayout(topPane, 2, 5);
    sourceBlock_ = new QComboBox(this);
    QLabel *sourceLabel = new QLabel(sourceBlock_, tr("&Source Block"), this);
    gridLayout->addWidget(sourceLabel, 0, 0);
    gridLayout->addWidget(sourceBlock_, 0, 1);
    gridLayout->addItem(new QSpacerItem(5, 0, QSizePolicy::Fixed), 0, 2);
    sourcePin_ = new QComboBox(this);
    QLabel *outputLabel = new QLabel(sourcePin_, tr("&Output Pin"), this);
    gridLayout->addWidget(outputLabel, 0, 3);
    gridLayout->addWidget(sourcePin_, 0, 4);

    targetPin_ = new QComboBox(this);
    QLabel *inputLabel = new QLabel(targetPin_, tr("&Input Pin"), this);
    gridLayout->addWidget(inputLabel, 1, 0);
    gridLayout->addWidget(targetPin_, 1, 1);
    gridLayout->addItem(new QSpacerItem(5, 0, QSizePolicy::Fixed), 1, 2);
    targetBlock_ = new QComboBox(this);
    QLabel *targetLabel = new QLabel(targetBlock_, tr("&Target Block"), this);
    gridLayout->addWidget(targetLabel, 1, 3);
    gridLayout->addWidget(targetBlock_, 1, 4);

    connect(sourceBlock_, SIGNAL(activated(int)),
	    this, SLOT(sourceBlockActivated(int)));
    connect(sourcePin_, SIGNAL(activated(int)),
	    this, SLOT(sourcePinActivated(int)));
    connect(targetBlock_, SIGNAL(activated(int)),
	    this, SLOT(targetBlockActivated(int)));
    connect(targetPin_, SIGNAL(activated(int)),
	    this, SLOT(targetPinActivated(int)));

    topPane->addItem(new QSpacerItem(0, 5, QSizePolicy::Minimum,
				     QSizePolicy::Fixed));

    pathChooser_ = new QListBox(this);
    QLabel *pathLabel = new QLabel(pathChooser_, tr("&Available Paths"), this);
    topPane->addWidget(pathLabel);
    topPane->addWidget(pathChooser_);

    QBoxLayout *buttonPane = new QHBoxLayout(topPane);
    QPushButton *okButton = new QPushButton(tr("&OK"), this);
    buttonPane->addWidget(okButton);
    QPushButton *cancelButton = new QPushButton(tr("&Cancel"), this);
    buttonPane->addWidget(cancelButton);

    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    loadBlocks();
}

void PathChooserDialog::loadBlocks()
{
    Q_ASSERT(blocks_ == 0);
    QValueList<BlockNode*> blockList = graph_->blocks();
    int blockCount = blockList.size() + 1;
    blocks_ = new BlockNode*[blockCount];
    int i = 0;
    blocks_[i] = 0;
    for (QValueList<BlockNode*>::iterator node = blockList.begin();
	 node != blockList.end(); node++) {

	blocks_[++i] = *node;
    }
    // sort?
    //sourceBlock_->sort();
    //targetBlock_->sort();
    // insert texts into listboxes
    for (i = 0; i < blockCount; ++i) {
	if (blocks_[i] == 0) {
	    sourceBlock_->insertItem(tr("<none>"), i);
	    targetBlock_->insertItem(tr("<none>"), i);
	}
	else {
	    sourceBlock_->insertItem(blocks_[i]->model()->name(), i);
	    targetBlock_->insertItem(blocks_[i]->model()->name(), i);
	}
    }
}


PathChooserDialog::~PathChooserDialog()
{
    if (blocks_ != 0) {
	delete [] blocks_;
    }
    if (inPins_ != 0) {
	delete [] inPins_;
    }
    if (outPins_ != 0) {
	delete [] outPins_;
    }
}

void PathChooserDialog::updatePinCombo(QComboBox *box,
				       BlockNode *block,
				       PinModel::PinType type,
				       PinModel ***array)
{
    // clear the combo box and delete array
    if (*array != 0) {
	while (box->count()) box->removeItem(0);
	delete [] (*array);
	*array = 0;
    }

    if (block != 0) {
	const QValueList<PinModel*> pins = block->model()->pins();
	QValueList<PinModel*> filtered;
	QValueList<PinModel*>::const_iterator it = pins.begin();
	while (it != pins.end()) {
	    PinModel *current = *it;
	    if (current->type() == type) {
		filtered.prepend(current);
	    }
	    ++it;
	}

	// create new array and insert into combo box
	int filterCount = filtered.count() + 1;
	*array = new PinModel*[filterCount];
	int i = 0;
	(*array)[i++] = 0;
	box->insertItem(tr("<none>"), -1);
	it = filtered.end();
	while (i < filterCount) {
	    --it;
	    PinModel *current = *it;
	    (*array)[i++] = current;
	    box->insertItem(current->name(), -1);
	}
	box->setCurrentItem(0);
    }
}

void PathChooserDialog::updateInPins()
{
    updatePinCombo(targetPin_, blocks_[targetBlock_->currentItem()],
		   PinModel::INPUT, &inPins_);
}

void PathChooserDialog::updateOutPins()
{
    updatePinCombo(sourcePin_, blocks_[sourceBlock_->currentItem()],
		   PinModel::OUTPUT, &outPins_);
}

void PathChooserDialog::updatePaths()
{
}

void PathChooserDialog::sourceBlockActivated(int)
{
    updateOutPins();
    updatePaths();
}

void PathChooserDialog::sourcePinActivated(int)
{
}

void PathChooserDialog::targetBlockActivated(int)
{
    updateInPins();
    updatePaths();
}

void PathChooserDialog::targetPinActivated(int)
{
}
