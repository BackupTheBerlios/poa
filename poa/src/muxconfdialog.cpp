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
 * $Id: muxconfdialog.cpp,v 1.37 2004/01/28 02:20:40 garbeam Exp $
 *
 *****************************************************************************/

#include <qbuttongroup.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qheader.h>
#include <qimage.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qmessagebox.h>
#include <qpixmap.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qspinbox.h>
#include <qtooltip.h>
#include <qvariant.h>
#include <qwhatsthis.h>

#include "muxconfdialog.h"

#include "blockconfwidget.h"
#include "muxmappingconfdialog.h"
#include "muxmodel.h"
#include "pinlistviewitem.h"
#include "poa.h"

MuxMappingListViewItem::MuxMappingListViewItem(
        QListView *parent, QListViewItem *after,
        MuxMapping *mapping)
    : QListViewItem(parent, after)
{
    setOpen(false);
    mapping_ = mapping;

    update();
}

MuxMappingListViewItem::~MuxMappingListViewItem() {
}

MuxMapping *MuxMappingListViewItem::mapping() const {
    return mapping_;
}

void MuxMappingListViewItem::update() {
    if (mapping_ != 0) {
        setText(0, mapping_->input()->name());
        setText(1, QString::number(mapping_->firstInputBit()) + " - "
                   + QString::number(mapping_->lastInputBit()));
        setText(2, mapping_->output()->name());
        setText(3, QString::number(mapping_->firstOutputBit()) + " - "
                   + QString::number(mapping_->lastOutputBit()));
    }
}

///////////////////////////////////////////////////////////////////////////////

MuxConfDialog::MuxConfDialog(MuxModel *model, QWidget* parent,
                             const char* name, bool modal, WFlags fl)
    : QDialog(parent, name, modal, fl)
{
    model_ = model;

    if (!name) {
        setName( "MuxConfDialog" );
        resize(400, 500);
    }
    setCaption(tr("Mux configuration"));

    initLayout();
    initConnections();
    listViewSelectionChanged();
    sync();

}

void MuxConfDialog::initLayout() {

    dialogLayout_ = new QVBoxLayout(this, WIDGET_SPACING);

    initTopWidget();
    blockConfWidget_ = new BlockConfWidget(model_, this);
    dialogLayout_->addWidget(blockConfWidget_);
    initMappingWidget();
    initBottomWidget();
}

void MuxConfDialog::initTopWidget() {
    QWidget *topWidget = new QWidget(this);
    QBoxLayout *topLayout = new QHBoxLayout(topWidget, WIDGET_SPACING);

    nameLineEdit = new QLineEdit(topWidget, "nameLineEdit");

    topLayout->addWidget(new QLabel(tr("name"), topWidget));
    topLayout->addWidget(nameLineEdit);

    dialogLayout_->addWidget(topWidget);

}

void MuxConfDialog::initMappingWidget() {

    QGroupBox *mappingGroupBox = new QGroupBox(this, "mappingGroupBox");
    mappingGroupBox->setTitle(tr("Mappings"));
    QBoxLayout *mappingLayout = new QVBoxLayout(mappingGroupBox, 3 * WIDGET_SPACING);

    // prepare buttons to put into grid layout
    QWidget *buttonWidget = new QWidget(mappingGroupBox);
    QBoxLayout *buttonLayout =
        new QHBoxLayout(buttonWidget, WIDGET_SPACING);

    QPushButton *newMappingButton = new QPushButton(buttonWidget);
    newMappingButton->setText(tr("&New"));
    connect(newMappingButton, SIGNAL(clicked()),
            this, SLOT(addMapping()));

    QPushButton *removeMappingButton = new QPushButton(buttonWidget);
    removeMappingButton->setText(tr("Remove"));
    connect(removeMappingButton, SIGNAL(clicked()),
            this, SLOT(removeMapping()));

    buttonLayout->addWidget(newMappingButton);
    buttonLayout->addWidget(removeMappingButton);

    // prepare mapping ListView
    mappingListView = new QListView(mappingGroupBox, "mappingsListView");
    mappingListView->setAllColumnsShowFocus(TRUE);
    mappingListView->addColumn(tr("Input"));
    mappingListView->addColumn(tr("Range"));
    mappingListView->addColumn(tr("Output"));
    mappingListView->addColumn(tr("Range"));

    // finish mapping layout
    mappingLayout->addWidget(mappingListView);
    mappingLayout->addWidget(buttonWidget);

    dialogLayout_->addWidget(mappingGroupBox);
}


void MuxConfDialog::initBottomWidget() {

    QWidget *bottomWidget = new QWidget(this);
    QBoxLayout *bottomLayout = new QHBoxLayout(bottomWidget, WIDGET_SPACING);

    helpPushButton = new QPushButton(bottomWidget, "helpPushButton");
    helpPushButton->setText(tr("&Help"));

    okPushButton = new QPushButton(bottomWidget, "okPushButton");
    okPushButton->setText(tr("&OK"));
    okPushButton->setDefault(TRUE);

    applyPushButton = new QPushButton(bottomWidget, "applyPushButton");
    applyPushButton->setText(tr("&Apply"));

    cancelPushButton = new QPushButton(bottomWidget, "cancelPushButton");
    cancelPushButton->setText(tr("&Cancel"));

    bottomLayout->addStretch(1);
    bottomLayout->addWidget(helpPushButton);
    bottomLayout->addWidget(okPushButton);
    bottomLayout->addWidget(applyPushButton);
    bottomLayout->addWidget(cancelPushButton);

    // finish layout
    dialogLayout_->addWidget(bottomWidget);
}

void MuxConfDialog::initConnections() {

    connect(okPushButton, SIGNAL(clicked()), this, SLOT(ok()));
    connect(applyPushButton, SIGNAL(clicked()), this, SLOT(apply()));
    connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(mappingListView, SIGNAL(selectionChanged()),
            this, SLOT(mappingSelectionChanged()));
}

/**
 *  Destroys the object and frees any allocated resources
 */
MuxConfDialog::~MuxConfDialog()
{
    mappingListView->clear();
}

void MuxConfDialog::apply() {
    commit();
}

void MuxConfDialog::ok() {
    commit();
    accept();
}

void MuxConfDialog::sync() {

    Q_ASSERT(model_ != 0);
    if (model_ != 0) {
        nameLineEdit->setText(model_->name());

        for (QPtrListIterator<MuxMapping> it2(*model_->mappings());
             it2 != 0; ++it2)
        {
            MuxMapping *mapping = *it2;
            new MuxMappingListViewItem(mappingListView, 0, mapping);
        }
    }
    blockConfWidget_->sync();
}


void MuxConfDialog::commit() {
    Q_ASSERT(model_ != 0);

    model_->setName(nameLineEdit->text());

    blockConfWidget_->commit();

    ////////////////////////////////////////////////////////////
    // mapping handling

    // deleted mappings
    for (QPtrListIterator<MuxMapping> dmit(deletedMappings_);
         dmit != 0; ++dmit)
    {
        model_->removeMuxMapping(*dmit);
    }

    // TODO: other mappings in listview

    mappingListView->clear();
    deletedMappings_.clear();

    // Notify model about update, so the view will be
    // repaint.
    ((AbstractModel *)model_)->updatePerformed();

    // sync again
    sync();
}

///////////////////////////////////////////////////////////////////////////////
// List view selection stuff

void MuxConfDialog::mappingSelectionChanged() {
#if 0
    MuxMappingListViewItem *item =
        (MuxMappingListViewItem *)mappingListView->selectedItem();
    bool selectedMapping = (item != 0);

    if (selectedMapping) {
        PinListViewItem *pinItem =
            findPinListViewItemByPinModel(item->data()->input());
        inputListView->setSelected(pinItem, TRUE);
        pinItem =
            findPinListViewItemByPinModel(item->data()->output());
        outputListView->setSelected(pinItem, TRUE);
    }
    listViewSelectionChanged();
#endif
}

void MuxConfDialog::listViewSelectionChanged() {
#if 0
    MuxMappingListViewItem *item =
        (MuxMappingListViewItem *)mappingListView->selectedItem();
    bool selectedMapping = (item != 0);

    updateMappingPushButton_->setEnabled(selectedMapping);
    removeMappingPushButton_->setEnabled(selectedMapping);

    addMappingPushButton_->setEnabled(inputListView->selectedItem() &&
                                      outputListView->selectedItem());

    PinListViewItem *inputPinItem =
        (PinListViewItem *)inputListView->selectedItem();
    PinListViewItem *outputPinItem =
        (PinListViewItem *)inputListView->selectedItem();

    if (inputPinItem) {
        PinModel *pin = inputPinItem->data();
        inputNameLineEdit->setText(pin->name());
        inputBitsSpinBox->setValue(pin->bits());
    }
    else {
        inputNameLineEdit->setText("");
        inputBitsSpinBox->setValue(1);
    }
    if (outputPinItem) {
        PinModel *pin = outputPinItem->data();
        outputNameLineEdit->setText(pin->name());
        outputBitsSpinBox->setValue(pin->bits());
    }
    else {
        outputNameLineEdit->setText("");
        outputBitsSpinBox->setValue(1);
    }

    updateInputPushButton_->setEnabled(inputPinItem);
    removeInputPushButton_->setEnabled(inputPinItem);
    updateOutputPushButton_->setEnabled(outputPinItem);
    removeOutputPushButton_->setEnabled(outputPinItem);
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Mapping manipulation stuff
//

void MuxConfDialog::addMapping() {

    MuxMappingListViewItem *item
        = new MuxMappingListViewItem(mappingListView, 0, 0);
    MuxMappingConfDialog *dialog
        = new MuxMappingConfDialog(
                QListViewItemIterator(blockConfWidget_->ioListView()),
                item, this);
    if (dialog->exec() == QDialog::Rejected) {
        mappingListView->takeItem(item);
    }
    delete dialog;
}

void MuxConfDialog::removeMapping() {
#if 0
    MuxMappingListViewItem *selected =
        (MuxMappingListViewItem *)mappingListView->selectedItem();

    if (!selected) {
        return;
    }

    MuxMapping *mapping = selected->data();
    MuxMapping *origMapping = selected->origData();

    if (origMapping != 0) {
        if (updatedMappings_.contains(origMapping)) {
            updatedMappings_.remove(origMapping);
        }

        deletedMappings_.append(origMapping);
    }

    if (newMappings_.contains(mapping)) {
        newMappings_.remove(mapping);
    }
    mappingListView->takeItem(selected);
    delete selected;
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Helper
//
#if 0
PinModel *MuxConfDialog::findPinById(PinModel::PinType type, unsigned id)
{

    QListViewItemIterator it(
            type == PinModel::INPUT ? inputListView : outputListView);

    for ( ; it.current(); ++it) {
        PinListViewItem *item = (PinListViewItem *)it.current();

        PinModel *pin = item->data(); //item->origData should also do

        if (pin && pin->id() == id) {
            return pin;
        }
    }

    // not found
    return 0;
}
#endif

#if 0
void MuxConfDialog::updatePositions(QListView *lv) {

    unsigned position = 0;
    for (QListViewItemIterator it(lv); it.current(); ++it) {
        PinListViewItem *item = (PinListViewItem *)it.current();
        PinModel *pin = item->data();
        pin->setPosition(position);
        PinModel *origPin = item->origData();
        if (origPin && origPin->position() != position) {
            updatedPins_.append(pin);
        }
        position++;
        item->update();
    }
}
#endif

void MuxConfDialog::updateMappings() {

    for (QListViewItemIterator it(mappingListView); it.current(); ++it) {
        MuxMappingListViewItem *item = (MuxMappingListViewItem *)it.current();
        item->update();
    }
}

bool MuxConfDialog::existDependentMapping(PinModel *pin) {

    for (QListViewItemIterator it(mappingListView); it.current(); ++it) {
        MuxMappingListViewItem *item = (MuxMappingListViewItem *)it.current();
        MuxMapping *mapping = item->mapping();
        if (mapping) {
            if (mapping->input() == pin || mapping->output() == pin) {
                return true;
            }
        }
    }

    return false;
}

void MuxConfDialog::removeDependentMappings(PinModel *pin) {

    for (QListViewItemIterator it(mappingListView); it.current(); ++it) {
        MuxMappingListViewItem *item = (MuxMappingListViewItem *)it.current();

        MuxMapping *mapping = item->mapping();

        if (mapping) {
            if (mapping->input() == pin || mapping->output() == pin) {

                deletedMappings_.append(mapping);
                mappingListView->takeItem(item);
                delete item;
            }
        }
    }
}
