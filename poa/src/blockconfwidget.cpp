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
 * $Id: blockconfwidget.cpp,v 1.16 2004/02/04 10:29:37 papier Exp $
 *
 *****************************************************************************/

#include <qlayout.h>
#include <qlistview.h>
#include <qmessagebox.h>
#include <qpoint.h>
#include <qpushbutton.h>
#include <qwidget.h>

#include "blockconfwidget.h"
#include "blockmodel.h"
#include "cpumodel.h"
#include "pinlistviewitem.h"
#include "pixmapbutton.h"
#include "util.h"


BlockConfWidget::BlockConfWidget(BlockModel *model, QWidget* parent) :
    QWidget(parent)
{
    model_ = model;
    inputRoot_ = outputRoot_ = episodicRoot_ = 0;
    currentRenameItem = 0;

    initLayout();
    ioSelectionChanged();
}

BlockConfWidget::~BlockConfWidget()
{
    if (inputRoot_) {
        delete inputRoot_;
    }
    if (outputRoot_) {
        delete outputRoot_;
    }
    if (episodicRoot_) {
        delete episodicRoot_;
    }
}

void BlockConfWidget::initLayout() {

    // base layout of this widget
    QBoxLayout *baseLayout
        = new QHBoxLayout(this, WIDGET_SPACING);

    // edit widget
    QWidget *editWidget = new QWidget(this);
    QBoxLayout *editLayout
        = new QVBoxLayout(editWidget, WIDGET_SPACING);

    QWidget *editButtonWidget = new QWidget(editWidget);
    QBoxLayout *editButtonLayout
        = new QHBoxLayout(editButtonWidget, WIDGET_SPACING);

    newIoPushButton_
        = new QPushButton(tr("New"), editButtonWidget);
    connect(newIoPushButton_, SIGNAL(clicked()),
            this, SLOT(newIo()));

    removeIoPushButton_
        = new QPushButton(tr("Remove"), editButtonWidget);
    connect(removeIoPushButton_, SIGNAL(clicked()),
            this, SLOT(removeIo()));

    editButtonLayout->addStretch(1);
    editButtonLayout->addWidget(newIoPushButton_);
    editButtonLayout->addWidget(removeIoPushButton_);
    editButtonLayout->addStretch(1);

    // I/O list view
    ioListView_ = new IOListView(editWidget, "ioListView");
    ioListView_->addColumn(tr("Position"));
    ioListView_->addColumn(tr("Name"));
    ioListView_->addColumn(tr("Bits"));
    ioListView_->addColumn(tr("Address"));
    ioListView_->setAllColumnsShowFocus(TRUE);
    ioListView_->setMinimumWidth(300);
    ioListView_->setDefaultRenameAction(QListView::Accept);
    connect(ioListView_, SIGNAL(selectionChanged()),
            this, SLOT(ioSelectionChanged()));
    connect(ioListView_,
            SIGNAL(mouseButtonClicked(int,QListViewItem *,const QPoint &,int)),
            this,
            SLOT(mouseButtonClicked(int,QListViewItem *,const QPoint &,int)));

    editLayout->addWidget(ioListView_);
    editLayout->addWidget(editButtonWidget);

    // order button widget
    QWidget *orderButtonWidget = new QWidget(this);
    QBoxLayout *orderLayout =
        new QVBoxLayout(orderButtonWidget, WIDGET_SPACING);

    QPushButton *upPushButton
        = new PixmapButton(QPixmap(Util::findIcon("1uparrow.png")),
                           orderButtonWidget);
    connect(upPushButton, SIGNAL(clicked()), this, SLOT(moveRowUp()));

    QPushButton *downPushButton
        = new PixmapButton(QPixmap(Util::findIcon("1downarrow.png")),
                          orderButtonWidget);
    connect(downPushButton, SIGNAL(clicked()), this, SLOT(moveRowDown()));

    orderLayout->addStretch(1);
    orderLayout->addWidget(upPushButton);
    orderLayout->addWidget(downPushButton);
    orderLayout->addStretch(1);

    // put everything together
    baseLayout->add(editWidget);
    baseLayout->add(orderButtonWidget);
}

QListView *BlockConfWidget::ioListView() const {
    return ioListView_;
}

void BlockConfWidget::sync()
{
    cancelRename();

    if (inputRoot_) {
        delete inputRoot_;
    }
    if (outputRoot_) {
        delete outputRoot_;
    }
    if (episodicRoot_) {
        delete episodicRoot_;
    }
    ioListView_->clear();

    if (model_->hasInputPins()) {
        inputRoot_ = new PinListViewItem(ioListView_, 0, PinModel::INPUT);
        inputRoot_->setText(0, tr("Input Pins"));
    }
    if (model_->hasOutputPins()) {
        outputRoot_ = new PinListViewItem(ioListView_, 0, PinModel::OUTPUT);
        outputRoot_->setText(0, tr("Output Pins"));
    }
    if (model_->hasEpisodicPins()) {
        episodicRoot_ = new PinListViewItem(ioListView_, 0, PinModel::EPISODIC);
        episodicRoot_->setText(0, tr("Episodic Pins"));
    }

    QValueList<PinModel *> pinList = model_->pins();
    for (QValueList<PinModel *>::iterator it = pinList.begin();
         it != pinList.end(); ++it) {

        PinModel *pin = (*it);
        switch (pin->type()) {
        case PinModel::INPUT:
            new PinListViewItem(inputRoot_, 0, PinModel::INPUT, pin);
            break;
        case PinModel::OUTPUT:
            new PinListViewItem(outputRoot_, 0, PinModel::OUTPUT, pin);
            break;
        case PinModel::EPISODIC:
            new PinListViewItem(episodicRoot_, 0, PinModel::EPISODIC, pin);
            break;
        }
    }

    resort();
}

void BlockConfWidget::cancelRename()
{
    if (currentRenameItem != 0) {
        currentRenameItem->cancelRename();
    }

//      if (ioListView_->isRenaming()) {
//          PinListViewItem *currentItem
//              = dynamic_cast<PinListViewItem *>(ioListView_->currentItem());
//          if (currentItem != 0) {
//              currentItem->cancelRename();
//          }
//      }
}

void BlockConfWidget::commit() {

    // deleted pins
    for (QPtrListIterator<PinModel> dit(deletedPins_); dit != 0; ++dit) {
        model_->deletePin(*dit);
    }
    deletedPins_.clear();

    // new & updated pins
    QListViewItemIterator it(ioListView_);
    for ( ; it.current(); ++it) {
        PinListViewItem *item = (PinListViewItem *)it.current();
        if (!item->isRoot()) {
            PinModel *pin = item->pin();
            if (!pin) {
                // new pin
                pin = item->createPin(model_);
                model_->addPin(pin);
            }
            else {
                // update pin
                item->commit();
            }
        }
    }
}

void BlockConfWidget::updatePositions(PinModel::PinType type)
{
    unsigned position = BlockModel::FIRST_PIN_POSISION;
    for (QListViewItemIterator it(ioListView_); it.current(); ++it) {
        PinListViewItem *item = (PinListViewItem *)it.current();
        if (!item->isRoot()) {
            if (item->type() == type) {
                item->setText(0, QString::number(position));
                ++position;
            }
        }
    }

    resort();
}

void BlockConfWidget::mouseButtonClicked(int button,
                                         QListViewItem *item,
                                         const QPoint &, int c)
{
    cancelRename();
    if (button == Qt::LeftButton && item != 0) {
        item->startRename(c);
        currentRenameItem = (PinListViewItem*)item;
    }
}

void BlockConfWidget::newIo()
{
    PinListViewItem *selectedItem
        = (PinListViewItem *)ioListView_->selectedItem();
    if (selectedItem != 0) {
        PinListViewItem *item
            = (selectedItem->isRoot())
            ? new PinListViewItem(selectedItem, 0, selectedItem->type(), 0)
            : new PinListViewItem(selectedItem->parent(), selectedItem,
                                  selectedItem->type(), 0);
        updatePositions(selectedItem->type());
        cancelRename();
        item->startRename(1);
        currentRenameItem = item;
    }
}

void BlockConfWidget::removeIo()
{
    PinListViewItem *item = (PinListViewItem *)ioListView_->selectedItem();
    if (item != 0) {
        cancelRename();

        PinModel *pin = item->pin();
        if (pin != 0) {
            if (pin->connected() == 0) {
                // Save deleted pins, to clean up views
                // if the changes will be applied.
                deletedPins_.append(pin);
            }
            else {
                // there exists a connection between the selected pin
                // and another one
                PinModel *connected = pin->connected();
                switch(QMessageBox::warning(this, "POA",
                           tr("The selected I/O is connected to I/O <i>id=") +
                            QString::number(connected->id()) + tr(" name=") +
                            connected->name() + tr(" of ") +
                            connected->parent()->name() + "</i>.\n\n" +
					    tr("The connection will be removed if you apply "
                            "your changes.\n\n"),
                            tr("&OK"),
                            tr("&Cancel"), 0, 0, 1 ) )
                {
                    case 0: // The user clicked OK, so all related connections
                            // will be removed after applying changes.
                        deletedPins_.append(pin);
                        break;
                    case 1: // Cancel removal.
                        return;
                        break;
                }
            }
        }

        item->parent()->takeItem(item);
        updatePositions(item->type());
        delete item;
    }
}

void BlockConfWidget::resort()
{
    ioListView_->setSorting(0);
    ioListView_->sort();

    for (QListViewItemIterator it(ioListView_); it.current(); ++it) {
        PinListViewItem *item = (PinListViewItem *)it.current();
        item->flushSortOrder();
    }

    ioListView_->setSorting(-1);
}

void BlockConfWidget::moveRowUp()
{
    PinListViewItem *item = (PinListViewItem *)ioListView_->selectedItem();
    if (item != 0 && !item->isRoot()) {
        PinListViewItem *itemAbove = (PinListViewItem *)item->itemAbove();
        if (!itemAbove->isRoot()) {
            cancelRename();
            itemAbove->moveItem(item);
            updatePositions(item->type());
        }
    }
}

void BlockConfWidget::moveRowDown()
{
    PinListViewItem *item = (PinListViewItem *)ioListView_->selectedItem();
    if (item && !item->isRoot()) {
        PinListViewItem *itemBelow = (PinListViewItem *)item->itemBelow();
        if (itemBelow && !itemBelow->isRoot()) {
            cancelRename();
            item->moveItem(itemBelow);
            updatePositions(item->type());
        }
    }
}

void BlockConfWidget::ioSelectionChanged() {
    PinListViewItem *item = (PinListViewItem *)ioListView_->selectedItem();
    //PinListViewItem *root = item;

    bool enabled = item != 0;
    bool isChild = enabled && !item->isRoot();
    bool isEpisodic = false;

    if (isChild) {
        isEpisodic = item->type() == PinModel::EPISODIC;
    }

    newIoPushButton_->setEnabled(enabled);
    removeIoPushButton_->setEnabled(isChild);
}

PinListViewItem *BlockConfWidget::pinListViewItemForPin(PinModel *pin) {

    for (QListViewItemIterator it(ioListView_); it.current(); ++it) {
        PinListViewItem *item = (PinListViewItem *)it.current();

        PinModel *p = item->pin();
        if (p && p == pin) {
            return item;
        }
    }

    return 0;
}
