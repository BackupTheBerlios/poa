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
 * $Id: blockconfdialog.cpp,v 1.6 2003/09/11 17:53:41 garbeam Exp $
 *
 *****************************************************************************/

#include "blockconfdialog.h"
#include "pinvector.h"

#include <qvariant.h>
#include <qbuttongroup.h>
#include <qgroupbox.h>
#include <qheader.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qlayout.h>

#define EPISODIC_IO_TEXT "episodic inputs"

PinListViewItem::PinListViewItem(QListView *parent,
                                 QListViewItem *after,
                                 PinModel::PinType type)
    : QListViewItem(parent, after)
{
    setOpen(true);
    type_ = type;
    item_ = 0;
}

PinListViewItem::PinListViewItem(QListViewItem *parent,
                                 PinModel *item)
    : QListViewItem(parent)
{
    setOpen(false);
    if (item != 0) {
        type_ = item->type();
    }
    item_ = item;

    update();
}

void PinListViewItem::update() {

    if (item_ != 0) {
        setText(0, QString::number(item_->id(), 10));
        setText(1, item_->name());
        if (item_->type() != PinModel::EPISODIC) {
            setText(2, QString::number(item_->address(), 16));
        }
        setText(3, QString::number(item_->bits(), 10));
    }
}

PinListViewItem::~PinListViewItem()
{
    delete item_;
}

PinModel *PinListViewItem::data() const
{
    return item_;
}

PinModel::PinType PinListViewItem::type() {
    return type_;
}

bool PinListViewItem::isRoot() {
    return isOpen();
}

BlockConfDialog::BlockConfDialog(BlockModel *model, QWidget* parent,
                                 const char* name, bool modal, WFlags fl)
    : QDialog(parent, name, modal, fl)
{
    if (!name) {
        setName("BlockConfDialog");
    }
    resize(400, 500);
    setCaption(tr("Block configuration"));

    model_ = model;

    QBoxLayout *dialogLayout = new QVBoxLayout(this, 5);
    QWidget *topWidget = new QWidget(this);
    QBoxLayout *topLayout = new QHBoxLayout(topWidget, 5);
    QWidget *leftWidget = new QWidget(topWidget);
    QBoxLayout *leftLayout = new QVBoxLayout(leftWidget, 5);
    QWidget *rightWidget = new QWidget(topWidget);
    QBoxLayout *rightLayout = new QVBoxLayout(rightWidget, 5);

    ////////////////////////////////////////////////////////////
    // top widget

    // I/O list view
    ioListView = new QListView(leftWidget, "ioListView");
    ioListView->addColumn(tr("I/O"));
    ioListView->addColumn(tr("name"));
    ioListView->addColumn(tr("address"));
    ioListView->addColumn(tr("bits"));
    ioListView->setAllColumnsShowFocus(TRUE);
    connect(ioListView, SIGNAL(selectionChanged()),
            this, SLOT(ioSelectionChanged()));

    // inputs root
    inputRoot = new PinListViewItem(ioListView, 0, PinModel::INPUT);
    inputRoot->setText(0, tr("inputs"));

    // outputs root
    outputRoot = new PinListViewItem(ioListView, inputRoot, PinModel::OUTPUT);
    outputRoot->setText(0, tr("outputs"));

    // episodic root
    episodicRoot = new PinListViewItem(ioListView, outputRoot, PinModel::EPISODIC);
    episodicRoot->setText(0, tr(EPISODIC_IO_TEXT));

    leftLayout->addWidget(ioListView);

    // I/O list view manipulation widget
    QWidget *editIoWidget = new QWidget(leftWidget);
    QGridLayout *editIoLayout =
        new QGridLayout(editIoWidget, 3, 4, 5);

    ioNumberLineEdit = new QLineEdit(editIoWidget, "ioNumberLineEdit");
    ioNameLineEdit = new QLineEdit(editIoWidget, "ioNameLineEdit");
    addressLineEdit = new QLineEdit(editIoWidget, "addressLineEdit");
    bitsLineEdit = new QLineEdit(editIoWidget, "bitsLineEdit");

    editIoLayout->addWidget(new QLabel(tr("I/O"), editIoWidget), 0, 0);
    editIoLayout->addWidget(ioNumberLineEdit, 0, 1);
    editIoLayout->addWidget(new QLabel(tr("data"), editIoWidget), 0, 2);
    editIoLayout->addWidget(ioNameLineEdit, 0, 3);
    editIoLayout->addWidget(
        new QLabel(tr("address"), editIoWidget), 1, 0);
    editIoLayout->addWidget(addressLineEdit, 1, 1);
    editIoLayout->addWidget(new QLabel(tr("bits"), editIoWidget), 1, 2);
    editIoLayout->addWidget(bitsLineEdit, 1, 3);


    // I/O manipulation buttons
    QWidget *ioButtonsWidget = new QWidget(leftWidget);
    QBoxLayout *ioButtonsLayout = new QHBoxLayout(ioButtonsWidget, 5);

    newIoPushButton = new QPushButton(ioButtonsWidget, "newIoPushButton");
    newIoPushButton->setText(tr("&New"));
    connect(newIoPushButton, SIGNAL(clicked()), this, SLOT(newIo()));

    updateIoPushButton =
        new QPushButton(ioButtonsWidget, "updateIoPushButton");
    updateIoPushButton->setText(tr("&Update"));
    connect(updateIoPushButton, SIGNAL(clicked()),
            this, SLOT(updateIo()));

    removeIoPushButton =
        new QPushButton(ioButtonsWidget, "removeIoPushButton");
    removeIoPushButton->setText(tr("&Remove"));
    connect(removeIoPushButton, SIGNAL(clicked()), this, SLOT(removeIo()));

    ioButtonsLayout->addWidget(newIoPushButton);
    ioButtonsLayout->addWidget(updateIoPushButton);
    ioButtonsLayout->addWidget(removeIoPushButton);

    leftLayout->addWidget(ioButtonsWidget);
    leftLayout->addWidget(editIoWidget);

    // block widget
    QGroupBox *blockGroupBox = new QGroupBox(rightWidget, "blockGroupBox");
    blockGroupBox->setTitle(tr("block"));
    blockGroupBox->setMaximumHeight(130);
    blockGroupBox->setMinimumHeight(130);

    QGridLayout *blockLayout =
        new QGridLayout(blockGroupBox, 3, 3, 5);

    blockNameLineEdit = new QLineEdit(blockGroupBox, "blockNameLineEdit" );
    blockDescrLineEdit = new QLineEdit(blockGroupBox, "blockDescrLineEdit");
    blockClockSpinBox = new QSpinBox(blockGroupBox, "blockClockSpinBox");

    blockLayout->addWidget(new QLabel(tr("name"), blockGroupBox), 0, 0);
    blockLayout->addWidget(blockNameLineEdit, 0, 1);
    blockLayout->addWidget(new QLabel(tr("description"), blockGroupBox), 1, 0);
    blockLayout->addWidget(blockDescrLineEdit, 1, 1);
    blockLayout->addWidget(new QLabel(tr("clock"), blockGroupBox), 2, 0);
    blockLayout->addWidget(blockClockSpinBox, 2, 1);
    blockLayout->addWidget(new QLabel(tr("ms"), blockGroupBox), 2, 2);

    // offset widget
    QButtonGroup *offsetButtonGroup =
        new QButtonGroup(rightWidget, "offsetButtonGroup" );
    offsetButtonGroup->setTitle(tr("offset"));
    offsetButtonGroup->setMaximumHeight(100);
    offsetButtonGroup->setMinimumHeight(100);

    QGridLayout *offsetLayout = new QGridLayout(offsetButtonGroup, 2, 3, 5);

    offsetAutoCalcRadioButton =
        new QRadioButton(offsetButtonGroup, "offsetAutoCalcRadioButton");
    offsetAutoCalcRadioButton->setText(tr("automatic calculation"));
    offsetAutoCalcRadioButton->setChecked(TRUE);
    connect(offsetAutoCalcRadioButton, SIGNAL(clicked()),
            this, SLOT(toggleManualOffset()));

    offsetRadioButton =
        new QRadioButton(offsetButtonGroup, "offsetRadioButton");
    offsetRadioButton->setText(tr("offset"));
    connect(offsetRadioButton, SIGNAL(clicked()),
            this, SLOT(toggleManualOffset()));

    offsetSpinBox = new QSpinBox(offsetButtonGroup, "offsetSpinBox");

    offsetLayout->addWidget(offsetAutoCalcRadioButton, 0, 0);
    offsetLayout->addWidget(offsetRadioButton, 1, 0);
    offsetLayout->addWidget(offsetSpinBox, 1, 1);
    offsetLayout->addWidget(new QLabel(tr("ms"), offsetButtonGroup), 1, 2);

    // runtime widget
    QButtonGroup *runtimeButtonGroup =
        new QButtonGroup(rightWidget, "runtimeButtonGroup");
    runtimeButtonGroup->setTitle(tr("runtime"));
    runtimeButtonGroup->setMaximumHeight(100);
    runtimeButtonGroup->setMinimumHeight(100);

    QGridLayout *runtimeLayout = new QGridLayout(runtimeButtonGroup, 2, 3, 5);

    runtimeAutoCalcRadioButton =
        new QRadioButton(runtimeButtonGroup, "runtimeAutoCalcRadioButton");
    runtimeAutoCalcRadioButton->setText(tr("automatic calculation"));
    runtimeAutoCalcRadioButton->setChecked(TRUE);
    connect(runtimeAutoCalcRadioButton, SIGNAL(clicked()),
            this, SLOT(toggleManualRuntime()));

    runtimeRadioButton = 
        new QRadioButton(runtimeButtonGroup, "runtimeRadioButton");
    runtimeRadioButton->setText(tr("runtime"));
    connect(runtimeRadioButton, SIGNAL(clicked()),
            this, SLOT(toggleManualRuntime()));

    runtimeSpinBox = new QSpinBox(runtimeButtonGroup, "runtimeSpinBox");

    runtimeLayout->addWidget(runtimeAutoCalcRadioButton, 0, 0);
    runtimeLayout->addWidget(runtimeRadioButton, 1, 0);
    runtimeLayout->addWidget(runtimeSpinBox, 1, 1);
    runtimeLayout->addWidget(new QLabel(tr("ms"), runtimeButtonGroup), 1, 2);

    // finish right layout
    rightLayout->addWidget(blockGroupBox);
    rightLayout->addWidget(offsetButtonGroup);
    rightLayout->addWidget(runtimeButtonGroup);

    // edit/compile button widget
    QWidget *compileEditButtonsWidget = new QWidget(rightWidget);
 
    QBoxLayout *compileEditButtonsLayout =
        new QHBoxLayout(compileEditButtonsWidget, 5);

    editCodePushButton =
        new QPushButton(compileEditButtonsWidget, "editCodePushButton" );
    editCodePushButton->setText(tr("&Edit code"));

    compilePushButton =
        new QPushButton(compileEditButtonsWidget, "compilePushButton");
    compilePushButton->setText(tr("Co&mpile"));


    compileEditButtonsLayout->addWidget(editCodePushButton);
    compileEditButtonsLayout->addWidget(compilePushButton);

    rightLayout->addWidget(compileEditButtonsWidget);

    // finish top layout
    topLayout->addWidget(leftWidget);
    topLayout->addWidget(rightWidget);

    ////////////////////////////////////////////////////////////
    // bottom widget

    QWidget *bottomWidget = new QWidget(this);
    QBoxLayout *bottomLayout = new QHBoxLayout(bottomWidget, 5);

    // ok button
    okPushButton = new QPushButton(bottomWidget, "okPushButton");
    okPushButton->setText(tr("&OK"));
    okPushButton->setDefault(TRUE);
    connect(okPushButton, SIGNAL(clicked()), this, SLOT(accept()));

    // help button
    helpPushButton = new QPushButton(bottomWidget, "helpPushButton");
    helpPushButton->setText(tr("&Help"));

    // apply button
    applyPushButton = new QPushButton(bottomWidget, "applyPushButton");
    applyPushButton->setText(tr("&Apply"));

    // cancel button
    cancelPushButton = new QPushButton(bottomWidget, "cancelPushButton" );
    cancelPushButton->setText(tr("&Cancel"));
    connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(reject()));

    bottomLayout->addWidget(okPushButton);
    bottomLayout->addWidget(helpPushButton);
    bottomLayout->addWidget(applyPushButton);
    bottomLayout->addWidget(cancelPushButton);

    ////////////////////////////////////////////////////////////
    // dialog
    dialogLayout->addWidget(topWidget);
    dialogLayout->addWidget(bottomWidget);

    // disable widgets, if neccessary
    update();
    ioSelectionChanged();
    toggleManualOffset();
    toggleManualRuntime();
}

void BlockConfDialog::update() {
    if (model_ != 0) {
        blockDescrLineEdit->setText(model_->description());


        PinVector &pins = *(model_->inputPins());

        for (unsigned i = 0; i < pins.size(); ++i) {
            PinListViewItem *child =
                new PinListViewItem((QListViewItem *)inputRoot,
                        (PinModel *)pins[i]);
            child->setVisible(true);
        }

        pins = *(model_->outputPins());

        for (unsigned i = 0; i < pins.size(); ++i) {
            PinListViewItem *child =
                new PinListViewItem((QListViewItem *)outputRoot,
                        (PinModel *)pins[i]);
            child->setVisible(true);
        }

        pins = *(model_->episodicPins());

        for (unsigned i = 0; i < pins.size(); ++i) {
            PinListViewItem *child =
                new PinListViewItem((QListViewItem *)episodicRoot,
                        (PinModel *)pins[i]);
            child->setVisible(true);
        }

    }
}

/**
 *  Destroys the object and frees any allocated resources
 */
BlockConfDialog::~BlockConfDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

void BlockConfDialog::newIo()
{
    PinListViewItem *item = (PinListViewItem *)ioListView->selectedItem();

    if (item != 0) {
        while (!item->isRoot()) {
            item = (PinListViewItem *)item->parent();
        }
        int childCount = item->childCount();
        PinModel *pin = new PinModel(model_, childCount,
                "data" + QString::number(childCount),
                childCount * 100, 32, item->type());
        model_->addPin(pin);

        PinListViewItem *child = new PinListViewItem(item, pin);
        child->setVisible(true);
    }
}

void BlockConfDialog::updateIo()
{
    PinListViewItem *item =
        (PinListViewItem *)ioListView->selectedItem();

    if (item != 0 && !item->isRoot()) {
        PinModel *pin = item->data();
        if (pin != 0) {
            pin->setName(ioNameLineEdit->text());
            bool *ok;
            pin->setId(ioNumberLineEdit->text().toUInt(ok, 10));
            pin->setAddress(addressLineEdit->text().toUInt(ok, 16));
            // TODO: check ok and pop up an error dialog, if ok is false
            pin->setBits(bitsLineEdit->text().toUInt(ok, 10));
            item->update();
        }
    }
}

void BlockConfDialog::removeIo()
{
    PinListViewItem *item = (PinListViewItem *)ioListView->selectedItem();
    PinListViewItem *root = item;

    if (root != 0) {
        while (!root->isRoot()) {
            root = (PinListViewItem *)root->parent();
        }
        root->takeItem(item);
        delete item;
    }
}

void BlockConfDialog::ioSelectionChanged() {
    PinListViewItem *item = (PinListViewItem *)ioListView->selectedItem();
    PinListViewItem *root = item;

    bool enabled = item != 0;
    bool isChild = enabled && !item->isOpen();
    bool isPeriodical = false;

    if (isChild) {
        while (!root->isOpen()) {
            root = (PinListViewItem *)root->parent();
        }
        isPeriodical = root->type() == PinModel::EPISODIC;
    }

    newIoPushButton->setEnabled(enabled);
    updateIoPushButton->setEnabled(isChild);
    removeIoPushButton->setEnabled(isChild);

    ioNumberLineEdit->setEnabled(isChild);
    ioNameLineEdit->setEnabled(isChild);
    addressLineEdit->setEnabled(isChild && !isPeriodical);
    bitsLineEdit->setEnabled(isChild);

    ioNumberLineEdit->setText(isChild ? item->text(0) : QString(""));
    ioNameLineEdit->setText(isChild ? item->text(1) : QString(""));
    addressLineEdit->setText(isChild ? item->text(2) : QString(""));
    bitsLineEdit->setText(isChild ? item->text(3) : QString(""));
}

void BlockConfDialog::toggleManualOffset() {
    offsetSpinBox->setEnabled(offsetRadioButton->isChecked());
}

void BlockConfDialog::toggleManualRuntime() {
    runtimeSpinBox->setEnabled(runtimeRadioButton->isChecked());
}
