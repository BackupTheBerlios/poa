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
 * $Id: muxconfdialog.cpp,v 1.33 2004/01/09 16:48:15 garbeam Exp $
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

#include "muxmodel.h"
#include "pinlistviewitem.h"
#include "poa.h"

MuxMappingListViewItem::MuxMappingListViewItem(
        QListView *parent, QListViewItem *after,
        MuxMapping *clone, MuxMapping *origin)
    : QListViewItem(parent, after)
{
    setOpen(false);
    clone_ = clone;
    origin_ = origin;

    update();
}

MuxMappingListViewItem::~MuxMappingListViewItem() {
}

MuxMapping *MuxMappingListViewItem::data() const {
    return clone_;
}

MuxMapping *MuxMappingListViewItem::origData() const {
    return origin_;
}

void MuxMappingListViewItem::setOrigMapping(MuxMapping *mapping) {
    origin_ = mapping;
}

void MuxMappingListViewItem::update() {
    if (clone_ != 0) {
        setText(0, clone_->input()->name());
        setText(1, QString::number(clone_->firstInputBit()) + " - "
                   + QString::number(clone_->lastInputBit()));
        setText(2, clone_->output()->name());
        setText(3, QString::number(clone_->firstOutputBit()) + " - "
                   + QString::number(clone_->lastOutputBit()));
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
    if (model_->muxType() == MuxModel::MUX) {
        setCaption(tr("Mux configuration"));
    }
    else {
        setCaption(tr("Demux configuration"));
    }

    initLayout();
    initConnections();
    listViewSelectionChanged();
    syncModel();

}

void MuxConfDialog::initLayout() {

    dialogLayout_ = new QVBoxLayout(this, WIDGET_SPACING);

    initTopWidget();
    initIOWidget();
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

void MuxConfDialog::initIOWidget() {

    QGroupBox *ioGroupBox = new QGroupBox(this, "ioGroupBox");
    ioGroupBox->setTitle(tr("I/Os"));
    QBoxLayout *ioLayout = new QVBoxLayout(ioGroupBox, 3 * WIDGET_SPACING);

    QWidget *ioTopWidget = new QWidget(ioGroupBox);
    QBoxLayout *ioTopLayout =
        new QHBoxLayout(ioTopWidget, WIDGET_SPACING);

    QButtonGroup *ioChooser = new QButtonGroup(ioGroupBox);
    QBoxLayout *ioChooserLayout =
        new QHBoxLayout(ioChooser, WIDGET_SPACING);

    QWidget *editWidget = new QWidget(ioGroupBox);
    QBoxLayout *editLayout =
        new QHBoxLayout(editWidget, WIDGET_SPACING);

    QWidget *buttonWidget = new QWidget(ioGroupBox);
    QBoxLayout *buttonLayout =
        new QHBoxLayout(buttonWidget, WIDGET_SPACING);

    inputListView = new QListView(ioTopWidget, "inputListView");
    inputListView->setAllColumnsShowFocus(TRUE);
    inputListView->addColumn(tr("Position"));
    inputListView->addColumn(tr("I/O name"));
    inputListView->addColumn(tr("Bits"));

    outputListView = new QListView(ioTopWidget, "outputListView");
    outputListView->setAllColumnsShowFocus(TRUE);
    outputListView->addColumn(tr("Position"));
    outputListView->addColumn(tr("I/O name"));
    outputListView->addColumn(tr("Bits"));

    ioTopLayout->addWidget(inputListView);
    ioTopLayout->addWidget(outputListView);

    ioNameLineEdit = new QLineEdit(editWidget, "ioNameLineEdit");

    inputRadioButton_ = new QRadioButton("input", ioChooser);
    inputRadioButton_->setChecked(true);
    outputRadioButton_ = new QRadioButton("output", ioChooser);

    ioChooserLayout->addWidget(inputRadioButton_);
    ioChooserLayout->addWidget(outputRadioButton_);

    ioBitsSpinBox = new QSpinBox(editWidget, "ioBitsSpinBox");
    ioBitsSpinBox->setMinValue(1);

    addIoPushButton =
        new QPushButton(buttonWidget, "addIoPushButton");
    addIoPushButton->setText(tr("Add I/O"));
    updateIoPushButton_ =
        new QPushButton(buttonWidget, "updateIoPushButton_");
    updateIoPushButton_->setText(tr("Update I/O"));
    removeIoPushButton_ =
        new QPushButton(buttonWidget, "removeIoPushButton_");
    removeIoPushButton_->setText(tr("Remove I/O"));

    editLayout->addWidget(new QLabel(tr("I/O name"), editWidget));
    editLayout->addWidget(ioNameLineEdit);
    editLayout->addWidget(new QLabel(tr("bits"), editWidget));
    editLayout->addWidget(ioBitsSpinBox);

    buttonLayout->addWidget(addIoPushButton);
    buttonLayout->addWidget(updateIoPushButton_);
    buttonLayout->addWidget(removeIoPushButton_);

    ioLayout->addWidget(ioTopWidget);
    ioLayout->addWidget(ioChooser);
    ioLayout->addWidget(editWidget);
    ioLayout->addWidget(buttonWidget);

    dialogLayout_->addWidget(ioGroupBox);
}

void MuxConfDialog::initMappingWidget() {

    QGroupBox *mappingGroupBox = new QGroupBox(this, "mappingGroupBox");
    mappingGroupBox->setTitle(tr("Mappings"));
    QBoxLayout *mappingLayout = new QVBoxLayout(mappingGroupBox, 3 * WIDGET_SPACING);

    // range widget
    QWidget *rangeWidget = new QWidget(mappingGroupBox);
    QBoxLayout *rangeLayout = new QHBoxLayout(rangeWidget, WIDGET_SPACING);

    firstInputBitSpinBox =
        new QSpinBox(rangeWidget, "firstInputBitSpinBox");
    lastInputBitSpinBox =
        new QSpinBox(rangeWidget, "lastInputBitSpinBox");

    rangeLayout->addWidget(
        new QLabel(tr("first input bit"), rangeWidget));
    rangeLayout->addWidget(firstInputBitSpinBox);
    rangeLayout->addWidget(
        new QLabel(tr("last input bit"), rangeWidget));
    rangeLayout->addWidget(lastInputBitSpinBox);

    firstOutputBitSpinBox =
        new QSpinBox(rangeWidget, "firstOutputBitSpinBox");
    lastOutputBitSpinBox =
        new QSpinBox(rangeWidget, "lastOutputBitSpinBox");

    rangeLayout->addWidget(
        new QLabel(tr("first output bit"), rangeWidget));
    rangeLayout->addWidget(firstOutputBitSpinBox);
    rangeLayout->addWidget(
        new QLabel(tr("last output bit"), rangeWidget));
    rangeLayout->addWidget(lastOutputBitSpinBox);

    // prepare buttons to put into grid layout
    QWidget *buttonWidget = new QWidget(mappingGroupBox);
    QBoxLayout *buttonLayout =
        new QHBoxLayout(buttonWidget, WIDGET_SPACING);

    addMappingPushButton_ = new QPushButton(buttonWidget, "addMappingPushButton");
    addMappingPushButton_->setText(tr("&Add"));

    updateMappingPushButton_ = new QPushButton(buttonWidget, "updateMappingPushButton_");
    updateMappingPushButton_->setText(tr("&Update"));

    removeMappingPushButton_ =
        new QPushButton(buttonWidget, "removeMappingPushButton_");
    removeMappingPushButton_->setText(tr("Remove"));

    buttonLayout->addWidget(addMappingPushButton_);
    buttonLayout->addWidget(updateMappingPushButton_);
    buttonLayout->addWidget(removeMappingPushButton_);

    // prepare mapping ListView
    mappingListView = new QListView(mappingGroupBox, "mappingsListView");
    mappingListView->setAllColumnsShowFocus(TRUE);
    mappingListView->addColumn(tr("Input"));
    mappingListView->addColumn(tr("Range"));
    mappingListView->addColumn(tr("Output"));
    mappingListView->addColumn(tr("Range"));

    // finish mapping layout
    mappingLayout->addWidget(mappingListView);
    mappingLayout->addWidget(rangeWidget);
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

    bottomLayout->addWidget(helpPushButton);
    bottomLayout->addWidget(okPushButton);
    bottomLayout->addWidget(applyPushButton);
    bottomLayout->addWidget(cancelPushButton);

    // finish layout
    dialogLayout_->addWidget(bottomWidget);
}

void MuxConfDialog::initConnections() {

    connect(addIoPushButton, SIGNAL(clicked()), this, SLOT(addIo()));
    connect(updateIoPushButton_, SIGNAL(clicked()), this, SLOT(updateIo()));
    connect(removeIoPushButton_, SIGNAL(clicked()), this, SLOT(removeIo()));
    connect(addMappingPushButton_, SIGNAL(clicked()), this, SLOT(addMapping()));
    connect(updateMappingPushButton_, SIGNAL(clicked()),
        this, SLOT(updateMapping()));
    connect(removeMappingPushButton_, SIGNAL(clicked()),
        this, SLOT(removeMapping()));
    connect(okPushButton, SIGNAL(clicked()), this, SLOT(ok()));
    connect(applyPushButton, SIGNAL(clicked()), this, SLOT(apply()));
    connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(inputListView, SIGNAL(selectionChanged()),
            this, SLOT(listViewSelectionChanged()));
    connect(outputListView, SIGNAL(selectionChanged()),
            this, SLOT(listViewSelectionChanged()));
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
    updateModel();
}

void MuxConfDialog::ok() {
    updateModel();
    accept();
}

void MuxConfDialog::syncModel() {

    Q_ASSERT(model_ != 0);
    if (model_ != 0) {
        nameLineEdit->setText(model_->name());

        QValueList<PinModel *> pins = model_->pins();

        for (QValueListIterator<PinModel *> it1 = pins.begin();
             it1 != pins.end(); ++it1)
        {
            PinModel *pin = *it1;

            if (pin->type() == PinModel::INPUT) {
                new PinListViewItem(inputListView, 0, pin->clone(), pin);
            }
            else if (pin->type() == PinModel::OUTPUT) {
                new PinListViewItem(outputListView, 0, pin->clone(), pin);
            }
            else {
                qWarning("MuxModel has PinModel with incompatible type.");
            }
        }

        for (QPtrListIterator<MuxMapping> it2(*model_->mappings());
             it2 != 0; ++it2)
        {
            MuxMapping *mapping = *it2;
            PinModel *clonedInput =
                findPinById(PinModel::INPUT, mapping->input()->id());
            Q_ASSERT(clonedInput);
            PinModel *clonedOutput =
                findPinById(PinModel::OUTPUT, mapping->output()->id());
            Q_ASSERT(clonedOutput);

            new MuxMappingListViewItem(mappingListView, 0,
                                       mapping->clone(clonedInput, clonedOutput),
                                       mapping);
        }
    }
}


void MuxConfDialog::updateModel() {
    Q_ASSERT(model_ != 0);

    model_->setName(nameLineEdit->text());

    ////////////////////////////////////////////////////////////
    // IO handling

    // deleted pins
    for (QPtrListIterator<PinModel> dit(deletedPins_); dit != 0; ++dit) {
        model_->deletePin(*dit);
    }

    // updated pins
    for (QPtrListIterator<PinModel> uit(updatedPins_); uit != 0; ++uit) {
        PinModel *pin = *uit;
        PinModel *origPin = model_->findPinById(pin->id());
        origPin->setName(pin->name());
        origPin->setBits(pin->bits());
        origPin->setPosition(pin->position());
    }

    // new pins
    for (QPtrListIterator<PinModel> nit(newPins_); nit != 0; ++nit) {
        PinModel *pin = *nit;

        PinModel *clone = pin->clone();
        model_->addPin(clone);
        // id has been set by addPin
        pin->setId(clone->id());
    }

    ////////////////////////////////////////////////////////////
    // mapping handling

    // deleted mappings
    for (QPtrListIterator<MuxMapping> dmit(deletedMappings_);
         dmit != 0; ++dmit)
    {
        model_->removeMuxMapping(*dmit);
    }

    // updated mappings
    for (QPtrListIterator<MuxMapping> umit(updatedMappings_);
         umit != 0; ++umit)
    {
        MuxMapping *mapping = *umit;
        MuxMapping *origMapping = model_->findMuxMappingById(mapping->id());
        origMapping->setInput(mapping->input());
        origMapping->setOutput(mapping->output());
        origMapping->setFirstInputBit(mapping->firstInputBit());
        origMapping->setLastInputBit(mapping->lastInputBit());
        origMapping->setFirstOutputBit(mapping->firstOutputBit());
        origMapping->setLastOutputBit(mapping->lastOutputBit());
    }

    // new mapping
    for (QPtrListIterator<MuxMapping> nmit(newMappings_);
         nmit != 0; ++nmit)
    {
        MuxMapping *mapping = *nmit;

        PinModel *input = model_->findPinById(mapping->input()->id());
        Q_ASSERT(input != 0);
        PinModel *output = model_->findPinById(mapping->output()->id());
        Q_ASSERT(output != 0);
        model_->addMuxMapping(mapping->clone(input, output));
    }

    // clears list views, will also delete all dedicated cloned models!
    inputListView->clear();
    outputListView->clear();
    mappingListView->clear();

    deletedPins_.clear();
    updatedPins_.clear();
    newPins_.clear();
    deletedMappings_.clear();
    updatedMappings_.clear();
    newMappings_.clear();

    // Notify model about update, so the view will be
    // repaint.
    ((AbstractModel *)model_)->updatePerformed();

    // sync again
    syncModel();
}

///////////////////////////////////////////////////////////////////////////////
// List view selection stuff

void MuxConfDialog::mappingSelectionChanged() {

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
}

void MuxConfDialog::listViewSelectionChanged() {

    MuxMappingListViewItem *item =
        (MuxMappingListViewItem *)mappingListView->selectedItem();
    bool selectedMapping = (item != 0);

    updateMappingPushButton_->setEnabled(selectedMapping);
    removeMappingPushButton_->setEnabled(selectedMapping);

    addMappingPushButton_->setEnabled(inputListView->selectedItem() &&
                                      outputListView->selectedItem());

    PinListViewItem *pinItem = 0;
    if (inputRadioButton_->isChecked()) {
        pinItem = (PinListViewItem *)inputListView->selectedItem();
    }
    else {
        pinItem = (PinListViewItem *)outputListView->selectedItem();
    }

    if (pinItem) {
        PinModel *pin = pinItem->data();
        ioNameLineEdit->setText(pin->name());
        ioBitsSpinBox->setValue(pin->bits());
    }
    else {
        ioNameLineEdit->setText("");
        ioBitsSpinBox->setValue(1);
    }

    updateIoPushButton_->setEnabled(pinItem);
    removeIoPushButton_->setEnabled(pinItem);


}

///////////////////////////////////////////////////////////////////////////////
// Mapping manipulation stuff
//

void MuxConfDialog::addMapping() {

    PinListViewItem *inputItem =
        (PinListViewItem *)inputListView->selectedItem();
    PinListViewItem *outputItem =
        (PinListViewItem *)outputListView->selectedItem();

    if (!inputItem || !outputItem) {
        return;
    }

    PinModel *input = inputItem->data();
    PinModel *output = outputItem->data();

    unsigned fIBit = firstInputBitSpinBox->value();
    unsigned lIBit = lastInputBitSpinBox->value();
    unsigned fOBit = firstOutputBitSpinBox->value();
    unsigned lOBit = lastOutputBitSpinBox->value();


    // check if choosen ranges are valid with IOs
    bool proceed = ((fIBit < input->bits()) &&
                    (lIBit < input->bits()) &&
                    (fIBit <= lIBit)) &&
                   ((fOBit < output->bits()) &&
                    (lOBit < output->bits()) &&
                    (fOBit <= lOBit));

    if (!proceed) {

        switch (QMessageBox::warning(this, "POA",
                "The selected I/O bit ranges are not valid.\n\n"
                "Proceed anyway?\n\n",
                "Yes",
                "No", 0, 0, 1 ))
        {
        case 1: // don't proceed.
            return;
            break;
        }
    }

    MuxMapping *mapping =
        new MuxMapping(input, output, firstInputBitSpinBox->value(),
                       lastInputBitSpinBox->value(),
                       firstOutputBitSpinBox->value(),
                       lastOutputBitSpinBox->value());

    MuxMappingListViewItem *predecessor =
        (MuxMappingListViewItem *)mappingListView->selectedItem();
    new MuxMappingListViewItem(mappingListView, predecessor, mapping, 0);
    newMappings_.append(mapping);
}

void MuxConfDialog::removeMapping() {

    MuxMappingListViewItem *selected =
        (MuxMappingListViewItem *)mappingListView->selectedItem();

    if (!selected) {
        return;
    }

    MuxMapping *origMapping = selected->origData();

    if (origMapping != 0) {
        deletedMappings_.append(origMapping);
    }

    mappingListView->takeItem(selected);
    delete selected;
}

void MuxConfDialog::updateMapping() {

    MuxMappingListViewItem *selected =
        (MuxMappingListViewItem *)mappingListView->selectedItem();
    PinListViewItem *inputItem =
        (PinListViewItem *)inputListView->selectedItem();
    PinListViewItem *outputItem =
        (PinListViewItem *)outputListView->selectedItem();

    if (!selected || !inputItem || !outputItem) {
        return;
    }

    MuxMapping *mapping = selected->data();
    if (mapping) {
        mapping->setInput(inputItem->data());
        mapping->setOutput(outputItem->data());
        mapping->setFirstInputBit(firstInputBitSpinBox->value());
        mapping->setLastInputBit(lastInputBitSpinBox->value());
        mapping->setFirstOutputBit(firstOutputBitSpinBox->value());
        mapping->setLastOutputBit(lastOutputBitSpinBox->value());

        if (selected->origData() != 0) {
            updatedMappings_.append(mapping);
        }
        selected->update();
    }
}

///////////////////////////////////////////////////////////////////////////////
// IO manipulation stuff
//
void MuxConfDialog::addIo() {
    // determine list view
    QListView *parent = 0;
    PinModel::PinType type;
    if (inputRadioButton_->isChecked()) {
        parent = inputListView;
        type = PinModel::INPUT;
    }
    else {
        parent = outputListView;
        type = PinModel::OUTPUT;
    }

    if (ioNameLineEdit->text() == "") {
        switch(QMessageBox::warning(this, "POA",
                "The new I/O has no name.\n\n"
                "Proceed anyway?\n\n",
                "Yes",
                "No", 0, 0, 1 ) )
        {
        case 1: // don't proceed.
            return;
            break;
        }
    }

    PinListViewItem *predecessor = (PinListViewItem *)parent->selectedItem();
    int childCount = 0;
    if (predecessor) {
        childCount = predecessor->childCount() + 1;
    }
    PinModel *pin = new PinModel(model_,
            ioNameLineEdit->text(),
            0, ioBitsSpinBox->value(), type);
    newPins_.append(pin);
    new PinListViewItem(parent, predecessor, pin, 0);
    updatePositions(parent);
    ioNameLineEdit->setText("");
    ioBitsSpinBox->setValue(0);
}

void MuxConfDialog::updateIo() {

    QListView *parent = 0;
    if (inputRadioButton_->isChecked()) {
        parent = inputListView;
    }
    else {
        parent = outputListView;
    }

    if (ioNameLineEdit->text() == "") {
        switch(QMessageBox::warning(this, "POA",
                "The I/O has no name.\n\n"
                "Proceed anyway?\n\n",
                "Yes",
                "No", 0, 0, 1 ) )
        {
        case 1: // don't proceed.
            return;
            break;
        }
    }

    PinListViewItem *selected =
        (PinListViewItem *)parent->selectedItem();

    if (!selected) {
        return;
    }

    PinModel *pin = selected->data();
    if (pin != 0) {
        pin->setName(ioNameLineEdit->text());
        pin->setBits(ioBitsSpinBox->value());
        if (selected->origData() != 0) {
            updatedPins_.append(pin);
        }
        updatePositions(parent);
        selected->update();
    }

    updateMappings();
}

void MuxConfDialog::removeIo() {

    QListView *parent = 0;
    if (inputRadioButton_->isChecked()) {
        parent = inputListView;
    }
    else {
        parent = outputListView;
    }
    PinListViewItem *selected = (PinListViewItem *)parent->selectedItem();

    if (!selected) {
        return;
    }

    PinModel *pin = selected->data();
    if (existDependentMapping(pin)) {
        switch(QMessageBox::warning(this, "POA",
                "The selected I/O is dependent for at least one mapping.\n\n"
                "All dependent mappings will be deleted.\n\n",
                "Ok",
                "Cancel", 0, 0, 1 ) )
        {
        case 0: // The user clicked OK, so all related connections
            // will be removed after applying changes.
            removeDependentMappings(pin);
            break;
        case 1: // Cancel removal.
            return;
            break;
        }
    }

    PinModel *origPin = selected->origData();
    if (origPin != 0) {
        if (origPin->connected() == 0) {
            // Save deleted pins, to clean up views
            // if the changes will be applied.
            deletedPins_.append(origPin);
        }
        else {
            // there exists a connection between the selected pin
            // and another one
            PinModel *connected = origPin->connected();
            switch(QMessageBox::warning(this, "POA",
                        "The selected I/O is connected to I/O <i>id=" +
                        QString::number(connected->id()) + " name=" +
                        connected->name() + " of " +
                        connected->parent()->name() + "</i>.\n\n"
                        "The connection will be removed if you apply "
                        "your changes.\n\n",
                        "Ok",
                        "Cancel", 0, 0, 1 ) )
            {
            case 0: // The user clicked OK, so all related connections
                // will be removed after applying changes.
                deletedPins_.append(origPin);
                break;
            case 1: // Cancel removal.
                return;
                break;
            }
        }
    }

    parent->takeItem(selected);
    updatePositions(parent);
    delete selected;
}

///////////////////////////////////////////////////////////////////////////////
// Helper
//

PinListViewItem *MuxConfDialog::findPinListViewItemByPinModel(PinModel *pin) {

    QListViewItemIterator it(
        (pin->type() == PinModel::INPUT) ? inputListView : outputListView);

    for ( ; it.current(); ++it) {
        PinListViewItem *item = (PinListViewItem *)it.current();

        if (pin == item->data()) {
            return item;
        }
    }

    // not found
    return 0;
}

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

void MuxConfDialog::updateMappings() {

    for (QListViewItemIterator it(mappingListView); it.current(); ++it) {
        MuxMappingListViewItem *item = (MuxMappingListViewItem *)it.current();
        item->update();
    }
}

bool MuxConfDialog::existDependentMapping(PinModel *pin) {

    for (QListViewItemIterator it(mappingListView); it.current(); ++it) {
        MuxMappingListViewItem *item = (MuxMappingListViewItem *)it.current();
        MuxMapping *mapping = item->data();
        if (mapping->input() == pin || mapping->output() == pin) {
            return true;
        }
    }

    return false;
}

void MuxConfDialog::removeDependentMappings(PinModel *pin) {

    for (QListViewItemIterator it(mappingListView); it.current(); ++it) {
        MuxMappingListViewItem *item = (MuxMappingListViewItem *)it.current();

        MuxMapping *mapping = item->data();

        if (mapping->input() == pin || mapping->output() == pin) {

            MuxMapping *origMapping = item->origData();
            if (origMapping != 0) {
                deletedMappings_.append(origMapping);
            }

            mappingListView->takeItem(item);
            delete item;
        }
    }
}
