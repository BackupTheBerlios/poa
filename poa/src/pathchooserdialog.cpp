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
 * $Id: pathchooserdialog.cpp,v 1.8 2004/03/05 14:48:24 keulsn Exp $
 *
 *****************************************************************************/

#include "pathchooserdialog.h"

#include "blockmodel.h"
#include "blockgraph.h"

#include <qhbuttongroup.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>


PathChooserDialog::PathChooserDialog(BlockGraph *graph)
{
    Q_ASSERT(graph != 0);
    graph_ = graph;
    result_ = 0;
    outPins_ = 0;
    inPins_ = 0;
    blocks_ = 0;
    paths_ = 0;
    pathsCount_ = 0;

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
    gridLayout->addWidget(inputLabel, 1, 3);
    gridLayout->addWidget(targetPin_, 1, 4);
    gridLayout->addItem(new QSpacerItem(5, 0, QSizePolicy::Fixed), 1, 2);
    targetBlock_ = new QComboBox(this);
    QLabel *targetLabel = new QLabel(targetBlock_, tr("&Target Block"), this);
    gridLayout->addWidget(targetLabel, 1, 0);
    gridLayout->addWidget(targetBlock_, 1, 1);

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

    QHButtonGroup *mainButtons = new QHButtonGroup(this);
    //    QBoxLayout *buttonPane = new QHBoxLayout(topPane);
    QPushButton *okButton = new QPushButton(tr("&OK"), mainButtons);
    okButton->setDefault(true);
    //    buttonPane->addWidget(okButton);
    QPushButton *cancelButton = new QPushButton(tr("&Cancel"), mainButtons);
    //    buttonPane->addWidget(cancelButton);
    topPane->addWidget(mainButtons);

    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    loadBlocks();
}

Path *PathChooserDialog::path()
{
    return result_;
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
    freePaths();
}

void PathChooserDialog::freePaths()
{
    if (paths_ != 0) {
        for (unsigned i = 0; i < pathsCount_; ++i) {
            delete paths_[i];
        }
        delete [] paths_;
        paths_ = 0;
        pathsCount_ = 0;
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
    unsigned i;

    freePaths();
    QValueList<Path*> pathList;
    int fromIndex = sourceBlock_->currentItem();
    int toIndex = targetBlock_->currentItem();
    int fromPinIndex = sourcePin_->currentItem();
    int toPinIndex = targetPin_->currentItem();
    if (fromIndex > 0 && toIndex > 0) {
        PathQueue queue;
        PinModel *fromPin = 0;
        PinModel *toPin = 0;
        if (fromPinIndex > 0) {
            fromPin = outPins_[fromPinIndex];
        }
        if (toPinIndex > 0) {
            toPin = inPins_[toPinIndex];
        }
        BlockNode *from = blocks_[fromIndex];
        BlockNode *to = blocks_[toIndex];
        Path::allPaths(queue, from, to);
        // filter paths not beginning or ending with correct pin
        while (!queue.isEmpty()) {
            Path *path = queue.removeHead();
            bool legal = true;
            if (path->length() > 1) {
                if (legal && fromPin != 0) {
                    PinModel *secondPin = fromPin->connected();
                    legal = (secondPin != 0)
                        && (secondPin->parent() == path->front(+1)->model());
                }
                if (legal && toPin != 0) {
                    PinModel *preLastPin = toPin->connected();
                    legal = (preLastPin != 0)
                        && (preLastPin->parent() == path->end(-1)->model());
                }
            }

            if (legal) {
                pathList.prepend(path);
            }
            else {
                delete path;
            }
        }
    }

    pathsCount_ = pathList.size();
    paths_ = new Path*[pathsCount_];
    for (i = 0; i < pathsCount_; ++i) {
        paths_[i] = pathList.front();
        pathList.pop_front();
    }

    while (pathChooser_->count() > 0) {
        pathChooser_->removeItem(0);
    }
    for (i = 0; i < pathsCount_; ++i) {
        pathChooser_->insertItem(paths_[i]->getText(), i);
    }
}

void PathChooserDialog::sourceBlockActivated(int)
{
    updateOutPins();
    updatePaths();
}

void PathChooserDialog::sourcePinActivated(int)
{
    updatePaths();
}

void PathChooserDialog::targetBlockActivated(int)
{
    updateInPins();
    updatePaths();
}

void PathChooserDialog::targetPinActivated(int)
{
    updatePaths();
}

void PathChooserDialog::accept()
{
    int index = pathChooser_->currentItem();
    if (index >= 0 && index < (int) pathsCount_) {
        result_ = paths_[index];
        --pathsCount_;
        for (int i = index; i < (int) pathsCount_; ++i) {
            paths_[i] = paths_[i + 1];
        }
        paths_[pathsCount_] = 0;
        pathChooser_->removeItem(index);
        QDialog::accept();
    }
    else {
        return;
    }
}
