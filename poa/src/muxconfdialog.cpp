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
 * $Id: muxconfdialog.cpp,v 1.29 2003/12/17 17:19:30 garbeam Exp $
 *
 *****************************************************************************/

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
        setText(1, QString::number(clone_->firstOutputBit()) + " - "
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
    //initConnections();
    //mappingSelectionChanged();
    //syncModel();

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
    QBoxLayout *ioLayout = new QHBoxLayout(ioGroupBox, 3 * WIDGET_SPACING);

    QWidget *leftWidget = new QWidget(ioGroupBox);
    QBoxLayout *leftLayout =
        new QVBoxLayout(leftWidget, 3 * WIDGET_SPACING);

    inputListView = new QListView(leftWidget, "inputListView");
    inputListView->addColumn(tr("Position"));
    inputListView->addColumn(tr("I/O name"));
    inputListView->addColumn(tr("Bits"));

    QWidget *rightWidget = new QWidget(ioGroupBox);
    QBoxLayout *rightLayout =
        new QVBoxLayout(rightWidget, 3 *WIDGET_SPACING);

    outputListView = new QListView(rightWidget, "outputListView");
    outputListView->addColumn(tr("Position"));
    outputListView->addColumn(tr("I/O name"));
    outputListView->addColumn(tr("Bits"));

    QWidget *nameWidget = new QWidget(leftWidget);
    QBoxLayout *nameLayout =
        new QHBoxLayout(nameWidget, WIDGET_SPACING);
    ioNameLineEdit =
        new QLineEdit(nameWidget, "ioNameLineEdit");
    nameLayout->addWidget(new QLabel(tr("I/O name"), nameWidget));
    nameLayout->addWidget(ioNameLineEdit);

    QWidget *bitsWidget = new QWidget(rightWidget);
    QBoxLayout *bitsLayout =
        new QHBoxLayout(bitsWidget, WIDGET_SPACING);
    ioBitsSpinBox = new QSpinBox(bitsWidget, "ioBitsSpinBox");
    bitsLayout->addWidget(new QLabel(tr("bits"), bitsWidget));
    bitsLayout->addWidget(ioBitsSpinBox);

    addIoPushButton =
        new QPushButton(leftWidget, "addIoPushButton");
    addIoPushButton->setText("Add I/O");
    removeIoPushButton =
        new QPushButton(rightWidget, "removeIoPushButton");
    removeIoPushButton->setText("Remove I/O");

    leftLayout->addWidget(new QRadioButton(tr("inputs"), leftWidget));
    leftLayout->addWidget(inputListView);
    leftLayout->addWidget(nameWidget);
    leftLayout->addWidget(addIoPushButton);

    rightLayout->addWidget(new QRadioButton(tr("outputs"), rightWidget));
    rightLayout->addWidget(outputListView);
    rightLayout->addWidget(bitsWidget);
    rightLayout->addWidget(removeIoPushButton);

    ioLayout->addWidget(leftWidget);
    ioLayout->addWidget(rightWidget);

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

    addPushButton = new QPushButton(buttonWidget, "addPushButton");
    addPushButton->setText(tr("&Add"));

    updatePushButton = new QPushButton(buttonWidget, "updatePushButton");
    updatePushButton->setText(tr("&Update"));

    removePushButton = new QPushButton(buttonWidget, "removePushButton");
    removePushButton->setText(tr("Remove"));

    buttonLayout->addWidget(addPushButton);
    buttonLayout->addWidget(updatePushButton);
    buttonLayout->addWidget(removePushButton);

    // prepare mapping ListView
    mappingListView = new QListView(mappingGroupBox, "mappingsListView");
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

    
    connect(updatePushButton, SIGNAL(clicked()), this, SLOT(updateMapping()));
    connect(removePushButton, SIGNAL(clicked()), this, SLOT(removeIoOrMapping()));
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

void MuxConfDialog::syncModel() {

    Q_ASSERT(model_ != 0);
    if (model_ != 0) {
        nameLineEdit->setText(model_->name());

        QValueList<PinModel *> pins = model_->pins();

        for (QValueListIterator<PinModel *> it = pins.begin();
             it != pins.end(); ++it)
        {
            PinModel *pin = *it;

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

        for (QPtrListIterator<MuxMapping> it(*model_->mappings()); it != 0;)
        {
            ++it;
            MuxMapping *mapping = it.current();
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
#if 0
    Q_ASSERT(model_ != 0);
    if (model_ != 0) {

        model_->setName(nameLineEdit->text());

        // update output pins
        // Note: MuxModel::addOutput emits also a pinAdded signal,
        // so this is used only for new Outputs!
        QPtrList<PinModel> *pinModels = model_->outputPins();
        pinModels->clear();
        QPtrListIterator<MapToComboBoxItem> iter(mappedToIos_);
        MapToComboBoxItem *item;
        while ((item = iter.current()) != 0) {
            ++iter;
            PinModel *origPin = item->origData();
            PinModel *currPin = item->data();

            if (origPin != 0) {
                origPin->setId(currPin->id());
                origPin->setName(currPin->name());
                origPin->setAddress(currPin->address());
                origPin->setBits(currPin->bits());
                origPin->setType(currPin->type());
                origPin->setParent(model_);
                pinModels->append(origPin);
            }
            else {
                // new pin
                PinModel *newOuput = currPin->clone();
                model_->addPin(newOuput);
                item->setOrigData(newOuput);
            }
        }

        //////////////////////////////////////////////////////////////
        // delete everything which was deleted by the user
        QPtrListIterator<PinModel> delMux(deletedPins_);
        PinModel *pin;
        while ((pin = delMux.current()) != 0) {
            ++delMux;
            model_->removePin(pin);
        }
        deletedPins_.clear();

        // Flush mux input pin list. Already deleted PinModels
        // won't resist anymore in the model, not deleted
        // PinModels will be restored in the next step.
        QPtrList<PinModel> *inputPins = model_->inputPins();
        inputPins->clear();

        // rebuild mux pin list.
        QListViewItemIterator it(mappingListView);
        for ( ; it.current(); ++it) {
            QListViewItem *item = it.current();
            if (item->isOpen()) { // only input pins are open
                MuxListViewItem *muxItem =  (MuxListViewItem *)item;

                PinModel *origPin = muxItem->origData();
                PinModel *currPin = muxItem->data();
                PinModel *newPin;

                if (origPin != 0) {
                    // use original input pin and change its mux mappings
                    // to the current model
                    QPtrListIterator<MuxMapping> origMapIter(*model_->mappings());
                    MuxMapping *m;
                    while ((m = origMapIter.current()) != 0) {
                        ++origMapIter;
                        delete m;
                    }
                    //origPin->mappings()->clear();
                    model_->addPin(origPin, true);
                    QPtrListIterator<MuxMapping> currMapIter(*model_->mappings());
                    while ((m = currMapIter.current()) != 0) {
                        ++currMapIter;
                        PinModel *output = 0; //model_->outputForName(m->output()->name());
                        Q_ASSERT(output != 0);
                        MuxMapping *newMapping =
                            m->clone(origPin, output);
                        model_->addMuxMapping(newMapping);
                    }
                    newPin = origPin;
                }
                else {
                    // It's a new input pin, just add a cloned one
                    PinModel *inputPin = currPin->clone();
                    model_->addPin(inputPin);
                    muxItem->setOrigData(inputPin);
                    newPin = inputPin;
                }
                // Update listView MuxMapping items
                QListViewItem *child = muxItem->firstChild();
                while (child != 0) {

                    MuxMappingListViewItem *mapItem =
                        (MuxMappingListViewItem *)child;

                    if (mapItem->data()->input() == currPin) {

                        MuxMapping *currMapping = mapItem->data();

                        // reset origin info
                        mapItem->setOrigMapping(
                                newPin->findEqual(currMapping));
                    }

                    child = child->nextSibling();
                }

            }
        }
        // Reset outputs on model
        model_->updateOutputs();

        // Notify model about update, so the view will be
        // repaint.
        ((AbstractModel *)model_)->updatePerformed();
    }
#endif
}

void MuxConfDialog::mappingSelectionChanged() {

    QListViewItem *item = mappingListView->selectedItem();
    bool selectedMapping = (item != 0);
    bool selectedChild = selectedMapping && !item->isOpen();

    updatePushButton->setEnabled(selectedChild);
    addPushButton->setEnabled(!selectedChild);
    removePushButton->setEnabled(selectedMapping);
    //beginSpinBox->setEnabled(selectedMapping);


    //beginSpinBox->setValue(selectedChild ?
    //        ((MuxMappingListViewItem *)item)->data()->firstInputBit() : 0);

    if (selectedMapping) {
        addPushButton->setText(tr("&Add"));
    }
    else {
        if (model_->muxType() == MuxModel::MUX) {
            addPushButton->setText(tr("&New Input"));
        }
        else {
            addPushButton->setText(tr("&New Output"));
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Add IO/Mapping Slot/Helpers
//
void MuxConfDialog::addIo() {

}

void MuxConfDialog::addMapping() {

    PinModel::PinType type;
    QString name;

    if (model_->muxType() == MuxModel::MUX) {
        type = PinModel::OUTPUT;
        name = "output";
    }
    else {
        type = PinModel::INPUT;
        name = "input";
    }

    QString mapToName = "";
    unsigned id =0;// mappedToIos_.count() + 1;

    if (mapToName == "") {
        mapToName = QString("%1 %2").arg(name).arg(id);
        switch(QMessageBox::warning(this, "POA",
                    "Cannot create a new mapping without a valid\n" +
                    name + " name.\n\n"
                    "Should I create a new Mapping with\n" +
                    name + ": " + mapToName,
                    "Yes",
                    "No", 0, 0, 1 ) )
        {
        case 0: // YES
            break;
        case 1: // NO
            return;
            break;
        }
    }

    ////////////////////////////////////////////////////////////
    // Proceeds if the selected mapTo IO exist or should be
    // created.
    //

    // Get model for name, if it already exists
    PinModel *mapTo = 0;//ioForString(mapToName);

    if (mapTo == 0) {
        // the given mapTo name does not exist yet,
        // so we create a new PinModel

        mapTo =
            new PinModel(model_, mapToName, id * 100, 0, type);
//        mappedToIos_.append(new MapToComboBoxItem(mapTo, 0));
    }

    //PinModel *currPin = item->data();
    //PinModel *origPin = item->origData();
    MuxMapping *mapping;

/*
    if (origPin != 0) {
        mapping = new MuxMapping(origPin, mapTo,
                beginSpinBox->value(),
    }
    else {
        mapping = new MuxMapping(currPin, mapTo,
                beginSpinBox->value(),
    }
*/
    QPtrList<MuxMapping> *mappings = model_->mappings();
    mappings->append(mapping);

    // update ListView
    new MuxMappingListViewItem(0, 0, mapping, 0);

}

///////////////////////////////////////////////////////////////////////////////
// Remove IO/Mapping Slot/Helpers
//
void MuxConfDialog::removeIo() {

    PinModel *origInputPin = 0;//item->origData();
    PinModel *connected = 0;
    if (origInputPin != 0) {
        PinModel *origPin = 0; //origInputPin->model();
        if (origPin != 0) {
            connected = origPin->connected();
        }
    }
#if 0
    if ((item->childCount() > 0) || connected != 0) {
        QString warnMessage = (item->childCount() > 0) ?
            item->text(0) + " has " +
            QString::number(item->childCount()) +
            " outgoing mappings.\n\n"
            "All mappings and any outgoing or incoming\n"
            "connections will be removed.\n":
            "All mappings and any outgoing or incoming\n"
            "connections will be removed.\n";

        switch(QMessageBox::warning(this, "POA",
               warnMessage, "Ok", "Cancel", 0, 0, 1))
        {
        case 0: // The user clicked OK, so all related connections
            // will be removed after applying changes.
            deletedPins_.append(origInputPin);
            break;
        case 1: // Cancel removal.
            return;
            break;
        }
    }
    else if (origInputPin != 0) {
        deletedPins_.append(origInputPin);
    }

    mappingListView->takeItem(item);
    // Note: item will be deleted by its parent mappingListView.
#endif
}

void MuxConfDialog::removeMapping(MuxMappingListViewItem *item) {

    MuxMapping *origMapping = item->origData();

    QListViewItem *iter = (QListViewItem *)item;
    // determine parent
    while (!iter->isOpen()) {
        iter = iter->parent();
    }

    if (origMapping != 0) {
        PinModel *origPin = origMapping->output();
        if (origPin != 0) {
            PinModel *connected = origPin->connected();
            if (connected != 0) {
                switch(QMessageBox::warning(this, "POA",
                            item->text(0) + " is connected to " +
                            connected->name() + " of " +
                            connected->parent()->name() + ".\n\n"
                            "This connection will be removed after you"
                            "apply your changes.\n",
                            "Ok", "Cancel", 0, 0, 1))
                {
                case 0: // The user clicked OK, so all related connections
                    // will be removed after applying changes.
                    break;
                case 1: // Cancel removal.
                    return;
                    break;
                }
            }
        }
    }

    // remove mapping also from current parent
    MuxMapping *mapping = item->data();
    //PinModel *pin = 0; //parentItem->data();
    QPtrList<MuxMapping> *mappings = model_->mappings();
    mappings->remove(mapping);

    // finally, we delete the item from the list
    //parentItem->takeItem(item);

}

void MuxConfDialog::removeIoOrMapping() {

    QListViewItem *item = mappingListView->selectedItem();

    if (item != 0) {

        if (item->isOpen()) {
        }
        else {
            removeMapping((MuxMappingListViewItem *)item);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update IO/Mapping
//
void MuxConfDialog::updateMapping() {

    // It's only possible to update a mapping, not a parent IO
    QListViewItem *item = mappingListView->selectedItem();

    if (item != 0) {
        if (!item->isOpen()) {

            //MuxMapping *mapping = ((MuxMappingListViewItem *)item)->data();

            //((MuxMappingListViewItem *)item)->update();
        }
    }
}

void MuxConfDialog::apply() {
    updateModel();
}

void MuxConfDialog::ok() {
    updateModel();
    accept();
}

void MuxConfDialog::newIo()
{
    // determine list view
#if 0
    PinListViewItem *item = (PinListViewItem *)ioListView->selectedItem();

    if (item != 0) {
        PinListViewItem *parentItem = item;
        while (!parentItem->isOpen()) {
            parentItem = (PinListViewItem *)parentItem->parent();
        }
        int childCount = parentItem->childCount() + 1;
        PinModel *pin = new PinModel(model_,
                "data" + QString::number(childCount),
                childCount * 100, 32, parentItem->type());
        newPins_.append(pin);
        new PinListViewItem(parentItem, item != parentItem ? item : 0, pin);
        updatePositions(parentItem->type());
    }
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Helper
//
PinModel *MuxConfDialog::findPinById(PinModel::PinType type, unsigned id)
{

    QListViewItemIterator it(
            type == PinModel::INPUT ? inputListView : outputListView);

    for ( ; it.current(); ++it) {
        PinListViewItem *item = (PinListViewItem *)it.current();

        PinModel *clone = item->data();

        if (clone->id() == id) {
            return clone;
        }
    }

    // not found
    return 0;
}
