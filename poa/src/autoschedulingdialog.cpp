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
 * $Id: autoschedulingdialog.cpp,v 1.3 2004/03/05 14:48:24 keulsn Exp $
 *
 *****************************************************************************/


#include "autoschedulingdialog.h"

#include "pathchooserdialog.h"
#include "pixmapbutton.h"
#include "util.h"

#include <qhbuttongroup.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <pixmapbutton.h>


AutoSchedulingDialog::AutoSchedulingDialog(BlockGraph *graph)
{
    Q_ASSERT(graph != 0);
    graph_ = graph;
    QValueList<BlockNode*> allBlocks = graph_->blocks();
    QValueList<BlockNode*>::iterator it = allBlocks.begin();
    for (; it != allBlocks.end(); ++it) {
        // Remove the flag on all blocks, as soon as a block is contained
        // in a path, the flag is set on that block.
        (*it)->setFlag(false);
    }
    paths_.setAutoDelete(true);

    setCaption(tr("Auto Scheduling"));
    QBoxLayout *topPane = new QVBoxLayout(this, 10, 5);

    blockBox_ = new QListBox(this);
    blockBox_->setSelectionMode(QListBox::NoSelection);
    QLabel *blockLabel = new QLabel(blockBox_, tr("Remaining Blocks"), this);
    topPane->addWidget(blockLabel);
    topPane->addWidget(blockBox_);

    topPane->addItem(new QSpacerItem(0, 5, QSizePolicy::Minimum,
                                     QSizePolicy::Fixed));

    pathBox_ = new QListBox(this);
    QLabel *pathLabel = new QLabel(pathBox_, tr("&Paths (priority order)"),
                                   this);
    topPane->addWidget(pathLabel);

    QBoxLayout *prPane = new QHBoxLayout(topPane);

    QWidget *prButtonSpace = new QWidget(this);
    QBoxLayout *priorityButtonLayout = new QVBoxLayout(prButtonSpace);
    QPushButton *upButton =
        new PixmapButton(QPixmap(Util::findIcon("1uparrow.png")),
                         prButtonSpace);
    QPushButton *downButton =
        new PixmapButton(QPixmap(Util::findIcon("1downarrow.png")),
                         prButtonSpace);
    priorityButtonLayout->addWidget(upButton);
    priorityButtonLayout->addWidget(downButton);
    prPane->addWidget(pathBox_);
    prPane->addWidget(prButtonSpace);


    QHButtonGroup *mainButtons = new QHButtonGroup(this);
    QPushButton *add = new QPushButton(tr("&Add"), mainButtons);
    QPushButton *remove = new QPushButton(tr("&Remove"), mainButtons);
    //editLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding,
    //                                  QSizePolicy::Fixed));
    //    QBoxLayout *buttonPane = new QHBoxLayout(topPane);
    QPushButton *okButton = new QPushButton(tr("&OK"), mainButtons);
    okButton->setDefault(true);
    //    buttonPane->addWidget(okButton);
    QPushButton *cancelButton = new QPushButton(tr("&Cancel"), mainButtons);
    //    buttonPane->addWidget(cancelButton);
    topPane->addWidget(mainButtons);

    connect(add, SIGNAL(clicked()), this, SLOT(addPath()));
    connect(remove, SIGNAL(clicked()), this, SLOT(removePath()));
    connect(upButton, SIGNAL(clicked()), this, SLOT(raisePrio()));
    connect(downButton, SIGNAL(clicked()), this, SLOT(lowerPrio()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    updateBlockList();
}


AutoSchedulingDialog::~AutoSchedulingDialog()
{
}

void AutoSchedulingDialog::updateBlockList()
{
    blockBox_->clear();
    QValueList<BlockNode*> allBlocks = graph_->blocks();
    QValueList<BlockNode*>::iterator it = allBlocks.begin();
    for (; it != allBlocks.end(); ++it) {
        if (!(*it)->flag()) {
            blockBox_->insertItem((*it)->model()->name(), -1);
        }
    }
}

void AutoSchedulingDialog::accept()
{
    // flag all blocks that still need automatic offset calculation
    // unflag all block that already have an offset
    QValueList<BlockNode*> allBlocks = graph_->blocks();
    QValueList<BlockNode*>::iterator it = allBlocks.begin();
    for (; it != allBlocks.end(); ++it) {
        (*it)->setFlag(!(*it)->autoOffset());
    }
    for (Path *current = paths_.first(); current != 0;
         current = paths_.next()) {

        current->optimize();
    }
    QDialog::accept();
}

void AutoSchedulingDialog::addPath()
{
    PathChooserDialog *dialog = new PathChooserDialog(graph_);
    int result = dialog->exec();
    if (result == QDialog::Accepted) {
        Path *newPath = dialog->path();
        Q_ASSERT(newPath != 0);
        pathBox_->insertItem(newPath->getText(), -1);
        paths_.append(newPath);
        newPath->setNodeFlag(true);
        updateBlockList();
    }
}

void AutoSchedulingDialog::removePath()
{
    int index = pathBox_->currentItem();
    Path *old = paths_.at(index);
    if (old != 0) {
        // Note that this might reset the flag on nodes where we still
        // want it to be set, because those nodes are contained in another
        // path.
        old->setNodeFlag(false);
        paths_.remove(index);
        pathBox_->removeItem(index);
        // Now ensure all node flags to their correct state
        for (Path *current = paths_.first(); current != 0;
             current = paths_.next()) {

            current->setNodeFlag(true);
        }
        updateBlockList();
    }
}

void AutoSchedulingDialog::raisePrio()
{
    int index = pathBox_->currentItem();
    if (index > 0) {
        Path *old = paths_.take(index - 1);
        if (old != 0) {
            paths_.insert(index, old);
            pathBox_->removeItem(index - 1);
            pathBox_->insertItem(old->getText(), index);
        }
    }
}

void AutoSchedulingDialog::lowerPrio()
{
    int index = pathBox_->currentItem();
    if (index < ((int) paths_.count()) - 1) {
        Path *old = paths_.take(index);
        if (old != 0) {
            paths_.insert(index + 1, old);
            pathBox_->removeItem(index);
            pathBox_->insertItem(old->getText(), index + 1);
            pathBox_->setSelected(index + 1, true);
        }
    }
}
