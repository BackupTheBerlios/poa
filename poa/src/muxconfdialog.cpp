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
 * $Id: muxconfdialog.cpp,v 1.42 2004/01/29 14:27:22 garbeam Exp $
 *
 *****************************************************************************/

#include <qvariant.h>
#include <qbuttongroup.h>
#include <qcombobox.h>
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
#include <qvgroupbox.h>
#include <qwhatsthis.h>

#include "muxconfdialog.h"

#include "blockconfwidget.h"
#include "muxmappingconfdialog.h"
#include "muxmodel.h"
#include "pinlistviewitem.h"
#include "poa.h"

MuxMappingListViewItem::MuxMappingListViewItem(
        QListView *parent, QListViewItem *after,
        PinListViewItem *input, PinListViewItem *output,
        MuxMapping *mapping)
    : QListViewItem(parent, after)
{
    setOpen(false);
    mapping_ = mapping;
    input_ = input;
    output_ = output;
    firstInputBit_ = 0;
    firstOutputBit_ = 0;
    lastInputBit_ = 0;
    lastOutputBit_ = 0;

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
    else {
        setText(0, input_->text(1));
        setText(1, QString::number(firstInputBit_) + " - "
                   + QString::number(lastInputBit_));
        setText(2, output_->text(1));
        setText(3, QString::number(firstOutputBit_) + " - "
                   + QString::number(lastOutputBit_));
    }
}

void MuxMappingListViewItem::setInputPinListViewItem(
        PinListViewItem *input)
{
    input_ = input;
}

void MuxMappingListViewItem::setOutputPinListViewItem(
        PinListViewItem *output)
{
    output_ = output;
}

PinListViewItem *MuxMappingListViewItem::inputPinListViewItem() {
    return input_;
}

PinListViewItem *MuxMappingListViewItem::outputPinListViewItem() {
    return output_;
}

void MuxMappingListViewItem::setFirstInputBit(unsigned firstInputBit) {
    firstInputBit_ = firstInputBit;
}

void MuxMappingListViewItem::setFirstOutputBit(unsigned firstOutputBit)
{
    firstOutputBit_ = firstOutputBit;
}

void MuxMappingListViewItem::setLastInputBit(unsigned lastInputBit) {
    lastInputBit_ = lastInputBit;
}

void MuxMappingListViewItem::setLastOutputBit(unsigned lastOutputBit) {
    lastOutputBit_ = lastOutputBit;
}

unsigned MuxMappingListViewItem::firstInputBit() {
    return firstInputBit_;
}

unsigned MuxMappingListViewItem::firstOutputBit() {
    return firstOutputBit_;
}

unsigned MuxMappingListViewItem::lastInputBit() {
    return lastInputBit_;
}

unsigned MuxMappingListViewItem::lastOutputBit() {
    return lastOutputBit_;
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
    mappingSelectionChanged();
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

    topLayout->addWidget(new QLabel(tr("Name"), topWidget));
    topLayout->addWidget(nameLineEdit);

    dialogLayout_->addWidget(topWidget);

}

void MuxConfDialog::initMappingWidget() {

    QVGroupBox *mappingGroupBox = new QVGroupBox(this, "mappingGroupBox");
    mappingGroupBox->setTitle(tr("Mappings"));

    // prepare mapping ListView
    mappingListView = new QListView(mappingGroupBox, "mappingsListView");
    mappingListView->setAllColumnsShowFocus(TRUE);
    mappingListView->addColumn(tr("Input"));
    mappingListView->addColumn(tr("Range"));
    mappingListView->addColumn(tr("Output"));
    mappingListView->addColumn(tr("Range"));

    // prepare buttons to put into grid layout
    QWidget *buttonWidget = new QWidget(mappingGroupBox);
    QBoxLayout *buttonLayout = new QHBoxLayout(buttonWidget, WIDGET_SPACING);

    newMappingButton = new QPushButton(buttonWidget);
    newMappingButton->setText(tr("&New"));
    connect(newMappingButton, SIGNAL(clicked()),
            this, SLOT(addMapping()));

    updateMappingButton = new QPushButton(buttonWidget);
    updateMappingButton->setText(tr("&Update"));
    connect(updateMappingButton, SIGNAL(clicked()),
            this, SLOT(updateMapping()));

    removeMappingButton = new QPushButton(buttonWidget);
    removeMappingButton->setText(tr("Remove"));
    connect(removeMappingButton, SIGNAL(clicked()),
            this, SLOT(removeMapping()));

    buttonLayout->addStretch(1);
    buttonLayout->addWidget(newMappingButton);
    buttonLayout->addWidget(updateMappingButton);
    buttonLayout->addWidget(removeMappingButton);
    buttonLayout->addStretch(1);

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
    blockConfWidget_->sync();
    if (model_ != 0) {
        nameLineEdit->setText(model_->name());

        for (QPtrListIterator<MuxMapping> it2(*model_->mappings());
             it2 != 0; ++it2)
        {
            MuxMapping *mapping = *it2;
            PinListViewItem *input =
                blockConfWidget_->pinListViewItemForPin(mapping->input());
            PinListViewItem *output =
                blockConfWidget_->pinListViewItemForPin(mapping->output());
            new MuxMappingListViewItem(mappingListView, input, output,
                                       0, mapping);
        }
    }
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

    for (QListViewItemIterator it(mappingListView); it.current(); ++it) {
        MuxMappingListViewItem *item = (MuxMappingListViewItem *)it.current();
        MuxMapping *mapping = item->mapping();
        if (!mapping) { // new Mapping
            // determine Pins
//LAST            mapping = new MuxMapping(item->inp);
        }
        else {
            // determine Pins, etc.
        }
    }

    mappingListView->clear();
    deletedMappings_.clear();

    // Notify model about update, so the view will be
    // repaint.
    model_->updatePerformed();

    // sync again
    sync();
}

///////////////////////////////////////////////////////////////////////////////
// List view selection stuff

void MuxConfDialog::mappingSelectionChanged() {
    MuxMappingListViewItem *item =
        (MuxMappingListViewItem *)mappingListView->selectedItem();
    bool selectedMapping = (item != 0);

    removeMappingButton->setEnabled(selectedMapping);
    updateMappingButton->setEnabled(selectedMapping);
}

///////////////////////////////////////////////////////////////////////////////
// Mapping manipulation stuff
//

void MuxConfDialog::addMapping() {

    MuxMappingListViewItem *item
        = new MuxMappingListViewItem(mappingListView, 0, 0, 0, 0);
    MuxMappingConfDialog *dialog
        = new MuxMappingConfDialog(
                QListViewItemIterator(blockConfWidget_->ioListView()),
                item, this);
    if (dialog->exec() == QDialog::Rejected) {
        mappingListView->takeItem(item);
    }
    else {
        item->update();
    }
    delete dialog;
}

void MuxConfDialog::updateMapping() {

    MuxMappingListViewItem *item =
        (MuxMappingListViewItem *)mappingListView->selectedItem();

    if (item) {
        MuxMappingConfDialog *dialog
            = new MuxMappingConfDialog(
                    QListViewItemIterator(blockConfWidget_->ioListView()),
                    item, this);
        if (dialog->exec() == QDialog::Accepted) {
            item->update();
        }
        delete dialog;
    }
}

void MuxConfDialog::removeMapping() {
    MuxMappingListViewItem *selected =
        (MuxMappingListViewItem *)mappingListView->selectedItem();

    if (!selected) {
        return;
    }

    MuxMapping *mapping = selected->mapping();
    if (mapping) {
        deletedMappings_.append(mapping);
    }
    mappingListView->takeItem(selected);
    delete selected;
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
