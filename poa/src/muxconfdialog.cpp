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
 * $Id: muxconfdialog.cpp,v 1.10 2003/09/26 16:34:43 garbeam Exp $
 *
 *****************************************************************************/

#include "muxconfdialog.h"

#include "muxmodel.h"
#include "pinmodel.h"
#include "poa.h"

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
#include <qspinbox.h>
#include <qtooltip.h>
#include <qvariant.h>
#include <qwhatsthis.h>

MuxMappingListViewItem::MuxMappingListViewItem(QListViewItem *parent,
                                               MuxMapping *clone,
                                               MuxMapping *origin)
    : QListViewItem(parent)
{
    setOpen(false);
    clone_ = clone;
    origin_ = origin;

    update();
}

MuxMappingListViewItem::~MuxMappingListViewItem() {
    if (clone_ != 0) {
        //delete clone_;
    }
}

MuxMapping *MuxMappingListViewItem::data() const {
    return clone_;
}

MuxMapping *MuxMappingListViewItem::origData() const {
    return origin_;
}

void MuxMappingListViewItem::update() {
    if (clone_ != 0) {
        setText(0, clone_->output()->name());
        setText(1, QString::number(clone_->begin()));
        setText(2, QString::number(clone_->end()));
    }
}

///////////////////////////////////////////////////////////////////////////////

MuxListViewItem::MuxListViewItem(QListView *parent, QListViewItem *after,
                                 MuxPin *clone, MuxPin *origin)
    : QListViewItem(parent, after)
{
    setOpen(true);
    clone_ = clone;
    origin_ = origin;

    update();
}

MuxListViewItem::~MuxListViewItem() {
    if (clone != 0) {
        delete clone_;
    }
}

MuxPin *MuxListViewItem::data() const {
    return clone_;
}

MuxPin *MuxListViewItem::origData() const {
    return origin_;
}

void MuxListViewItem::update() {
    if (clone_ != 0) {
        setText(0, clone_->model()->name());
    }
}

///////////////////////////////////////////////////////////////////////////////

MapToComboBoxItem::MapToComboBoxItem(PinModel *clone, PinModel *origin) {
    clone_ = clone;
    origin_ = origin;
}

MapToComboBoxItem::~MapToComboBoxItem() {
    if (clone_ != 0) {
        delete clone_;
    }
}

PinModel *MapToComboBoxItem::data() const {
    return clone_;
}

PinModel *MapToComboBoxItem::origData() const {
    return origin_;
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
    mappingSelectionChanged();
    syncModel();

}

void MuxConfDialog::initLayout() {

    dialogLayout_ = new QVBoxLayout(this, WIDGET_SPACING);

    initTopWidget();
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

    QWidget *manipulationWidget = new QWidget(mappingGroupBox);
    QGridLayout *manipulationLayout =
        new QGridLayout(manipulationWidget, 2, 3, 3 * WIDGET_SPACING);

    // begin
    QWidget *beginWidget = new QWidget(manipulationWidget);
    QBoxLayout *beginLayout = new QHBoxLayout(beginWidget, WIDGET_SPACING);

    beginSpinBox = new QSpinBox(beginWidget, "beginSpinBox");

    beginLayout->addWidget(new QLabel(tr("Begin"), beginWidget));
    beginLayout->addWidget(beginSpinBox);

    // end
    QWidget *endWidget = new QWidget(manipulationWidget);
    QBoxLayout *endLayout = new QHBoxLayout(endWidget, WIDGET_SPACING);

    endSpinBox = new QSpinBox(endWidget, "endSpinBox");

    endLayout->addWidget(new QLabel(tr("End"), endWidget));
    endLayout->addWidget(endSpinBox);

    // prepare combobox and buttons to put into grid layout
    ioComboBox_ = new QComboBox(true, manipulationWidget, "ioComboBox_");
    ioComboBox_->setAutoCompletion(true);

    addPushButton = new QPushButton(manipulationWidget, "addPushButton");
    addPushButton->setText(tr("&Add"));

    updatePushButton = new QPushButton(manipulationWidget, "updatePushButton");
    updatePushButton->setText(tr("&Update"));

    removePushButton = new QPushButton(manipulationWidget, "removePushButton");
    removePushButton->setText(tr("Remove"));

    // put everything together into grid layout
    manipulationLayout->addWidget(beginWidget, 0, 0);
    manipulationLayout->addWidget(endWidget, 0, 1);
    manipulationLayout->addWidget(ioComboBox_, 0, 2);
    manipulationLayout->addWidget(addPushButton, 1, 0);
    manipulationLayout->addWidget(updatePushButton, 1, 1);
    manipulationLayout->addWidget(removePushButton, 1, 2);

    // prepare mapping ListView
    mappingListView = new QListView(mappingGroupBox, "mappingsListView");
    mappingListView->addColumn(tr("Mapping"));
    mappingListView->addColumn(tr("Begin"));
    mappingListView->addColumn(tr("End"));

    // finish mapping layout
    mappingLayout->addWidget(mappingListView);
    mappingLayout->addWidget(manipulationWidget);

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

    connect(addPushButton, SIGNAL(clicked()), this, SLOT(addIoOrMapping()));
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

        QPtrList<PinModel> *outputPins = model_->outputPins();
        for (unsigned i = 0; i < outputPins->count(); i++) {
            PinModel *pin = outputPins->at(i);

            mappedToIos_.append(new MapToComboBoxItem(pin->clone(), pin));
            ioComboBox_->insertItem(pin->name());
        }

        QPtrList<MuxPin> *pins = model_->muxPins();
        QListViewItem *last = 0;

        for (unsigned i = 0; i < pins->count(); i++) {
            MuxPin *pin = pins->at(i);

            MuxPin *cloned = pin->clone();

            last = new MuxListViewItem(mappingListView, last,
                                       cloned, pin);

            QPtrList<MuxMapping> *clonedMappings = cloned->mappings();
            QPtrList<MuxMapping> *origMappings = pin->mappings();

            for (unsigned j = 0; j < clonedMappings->count(); j++) {
                new MuxMappingListViewItem(last, clonedMappings->at(j),
                                           origMappings->at(j));
            }
        }
    }
}


void MuxConfDialog::updateModel() {

    Q_ASSERT(model_ != 0);
    if (model_ != 0) {

        model_->setName(nameLineEdit->text());

        //////////////////////////////////////////////////////////////
        // delete everything which was deleted by the user
        for (unsigned i = 0; i < deletedMuxPins_.count(); i++) {
            MuxPin *pin = deletedMuxPins_.at(i);
            model_->removeMuxPin(pin);
        }

        for (unsigned i = 0; i < deletedMappings_.count(); i++) {
            MuxMapping *mapping = deletedMappings_.at(i);
            model_->removeMuxMapping(mapping);
        }

        // flush mux pin list.
        QPtrList<MuxPin> *muxPins = model_->muxPins();
        muxPins->clear();

        // rebuild mux pin list.
        QListViewItemIterator it(mappingListView);
        for ( ; it.current(); ++it) {
            QListViewItem *item = it.current();
            if (item->isOpen()) { // MuxPin
                MuxListViewItem *muxItem =  (MuxListViewItem *)item;

                if (muxItem->origData() != 0) {
                    // use original MuxPin and change its mux mappings
                    // to the current model
                    MuxPin *origPin = muxItem->origData();
                    QPtrList<MuxMapping> *origMappings = origPin->mappings();
                    for (unsigned i = 0; i < origMappings->count(); i++) {
                        MuxMapping *mapping = origMappings->at(i);
                        origMappings->remove(i);
                        delete mapping;
                    }
                    model_->addMuxPin(origPin);
                    QPtrList<MuxMapping> *currMapping = muxItem->data()->mappings();
                    for (unsigned i = 0; i < currMapping->count(); i++) {
                        MuxMapping *mapping = currMapping->at(i);
                        model_->addMuxMapping(mapping);
                    }
                }
                else {
                    // It's a new MuxPin, just add a cloned one
                    model_->addMuxPin(muxItem->data()->clone());
                }
            }
        }
        // Notify model about update, so the view will be
        // repaint.
        ((AbstractModel *)model_)->updatePerformed();

        // Note: Don't clear the mappingListView here, because the apply
        // action uses also updateModel.
    }
}

void MuxConfDialog::mappingSelectionChanged() {

    QListViewItem *item = mappingListView->selectedItem();
    bool selectedMapping = (item != 0);
    bool selectedChild = selectedMapping && !item->isOpen();

    updatePushButton->setEnabled(selectedChild);
    addPushButton->setEnabled(!selectedChild);
    removePushButton->setEnabled(selectedMapping);
    beginSpinBox->setEnabled(selectedMapping);
    endSpinBox->setEnabled(selectedMapping);
    ioComboBox_->setEnabled(selectedMapping);


    beginSpinBox->setValue(selectedChild ? 
            ((MuxMappingListViewItem *)item)->data()->begin() : 0);
    endSpinBox->setValue(selectedChild ? 
            ((MuxMappingListViewItem *)item)->data()->end() : 0);
    ioComboBox_->setCurrentText(selectedChild ? 
            ((MuxMappingListViewItem *)item)->data()->output()->name() : "");

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

PinModel *MuxConfDialog::ioForString(QString name) {

    for (unsigned i = 0; i < mappedToIos_.count(); i++) {
        PinModel *model = mappedToIos_.at(i)->data();
        if (model->name() == name) {
            return model;
        }
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Add IO/Mapping Slot/Helpers
//
void MuxConfDialog::addIo() {

    PinModel::PinType type;
    QString name;

    if (model_->muxType() == MuxModel::MUX) {
        type = PinModel::INPUT;
        name = "input";
    }
    else {
        type = PinModel::OUTPUT;
        name = "output";
    }

    unsigned bits = 32;
    unsigned id = mappingListView->childCount() + 1;

    MuxPin *pin =
        new MuxPin(new PinModel(model_, id,
                    QString("%1 %2").arg(name).arg(id),
                    0, bits, type));

    new MuxListViewItem(mappingListView, 0, pin, 0);

}

void MuxConfDialog::addMapping(MuxListViewItem *item) {

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

    QString mapToName = ioComboBox_->currentText();
    unsigned id = mappedToIos_.count() + 1;
    bool proceed = true;

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
            proceed = true;
            break;
        case 1: // NO
            proceed = false;
            break;
        }
    }

    ////////////////////////////////////////////////////////////
    // Proceeds if the selected mapTo IO exist or should be
    // created.
    //
    if (proceed) {

        // Get model for name, if it already exists
        PinModel *mapTo = ioForString(mapToName);

        if (mapTo == 0) {
            // the given mapTo name does not exist yet,
            // so we create a new PinModel
            mapTo =
                new PinModel(model_, id, mapToName, id * 100, 0,
                        (type == PinModel::INPUT) ? PinModel::OUTPUT : 
                        PinModel::INPUT);
            mappedToIos_.append(new MapToComboBoxItem(mapTo, 0));
            ioComboBox_->insertItem(mapToName);
        }

        MuxPin *pin = item->data();
        MuxMapping *mapping;

        if (item->origData() != 0) {
            mapping = new MuxMapping(item->origData(), mapTo,
                                     beginSpinBox->value(),
                                     endSpinBox->value());
        }
        else {
            mapping = new MuxMapping(pin, mapTo, beginSpinBox->value(),
                                     endSpinBox->value());
        }

        QPtrList<MuxMapping> *mappings = pin->mappings();
        mappings->append(mapping);

        // update ListView
        new MuxMappingListViewItem(item, mapping, 0);

    }

}

void MuxConfDialog::addIoOrMapping() {

    MuxListViewItem *item = (MuxListViewItem *)mappingListView->selectedItem();

    if (item != 0) {
        addMapping(item);
    }
    else {
        addIo();
    }
}

///////////////////////////////////////////////////////////////////////////////
// Remove IO/Mapping Slot/Helpers
//
void MuxConfDialog::removeIo(MuxListViewItem *item) {

    MuxPin *origMuxPin = item->origData();
    PinModel *connected = 0;
    if (origMuxPin != 0) {
        PinModel *origPin = origMuxPin->model();
        if (origPin != 0) {
            connected = origPin->connected();
        }
    }

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
            deletedMuxPins_.append(origMuxPin);
            break;
        case 1: // Cancel removal.
            return;
            break;
        }
    }

    mappingListView->takeItem(item);
    // Note: item will be deleted by its parent mappingListView.
}

void MuxConfDialog::removeMapping(MuxMappingListViewItem *item) {

    MuxMapping *origMapping = item->origData();

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
                    deletedMappings_.append(origMapping);
                    break;
                case 1: // Cancel removal.
                    return;
                    break;
                }
            }
        }
    }

    QListViewItem *iter = (QListViewItem *)item;
    // determine parent
    while (!iter->isOpen()) {
        iter = iter->parent();
    }

    // finally, we delete the item from the list
    ((MuxListViewItem *)iter)->takeItem(item);
}

void MuxConfDialog::removeIoOrMapping() {

    QListViewItem *item = mappingListView->selectedItem();

    if (item != 0) {

        if (item->isOpen()) {
            // parent MuxListViewItem
            removeIo((MuxListViewItem *)item);
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

            MuxMapping *mapping = ((MuxMappingListViewItem *)item)->data();

            mapping->output()->setName(ioComboBox_->currentText());
            mapping->setEnd(endSpinBox->value());
            mapping->setBegin(beginSpinBox->value());

            ((MuxMappingListViewItem *)item)->update();
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
