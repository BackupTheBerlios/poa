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
 * $Id: blockconfdialog.cpp,v 1.56 2004/01/20 19:25:29 squig Exp $
 *
 *****************************************************************************/

#include <qvariant.h>
#include <qbuttongroup.h>
#include <qgroupbox.h>
#include <qheader.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlistview.h>
#include <qmessagebox.h>
#include <qptrlist.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qlayout.h>
#include <qvgroupbox.h>

#include "blockconfdialog.h"
#include "canvasview.h"
#include "blockmodel.h"
#include "codemanager.h"
#include "cpumodel.h"
#include "mainwindow.h"
#include "pinlistviewitem.h"
#include "poa.h"
#include "runtimemanager.h"

///////////////////////////////////////////////////////////////////////////////

BlockConfDialog::BlockConfDialog(BlockModel *model, QWidget* parent,
                                 const char* name, bool modal, WFlags fl)
    : QDialog(parent, name, modal, fl)
{
    if (!name) {
        setName("BlockConfDialog");
    }
    resize(600, 400);
    setCaption(tr("Block configuration"));

    model_ = model;

    initLayout();

    // disable widgets, if neccessary
    syncModel();
    ioSelectionChanged();
}

BlockConfDialog::~BlockConfDialog()
{
    ioListView->clear();
}

void BlockConfDialog::initLayout()
{
    dialogLayout = new QVBoxLayout(this, WIDGET_SPACING);
    topWidget = new QWidget(this);
    topLayout = new QHBoxLayout(topWidget, WIDGET_SPACING);
    leftWidget = new QWidget(topWidget);
    leftLayout = new QVBoxLayout(leftWidget, WIDGET_SPACING);
    rightWidget = new QWidget(topWidget);
    rightLayout = new QVBoxLayout(rightWidget, WIDGET_SPACING);
    bottomWidget = new QWidget(this);

    initListView();
    initBlockWidget();

    if (INSTANCEOF(model_, CpuModel)) {
        initCompileEditButtonWidget();
    }

    rightLayout->addStretch(1);
    topLayout->addWidget(leftWidget);
    topLayout->addWidget(rightWidget);

    initBottomWidget();

    // dialog
    dialogLayout->addWidget(topWidget);
    dialogLayout->addWidget(bottomWidget);
}

void BlockConfDialog::initBlockWidget()
{

    // block widget
    QGroupBox *blockGroupBox = new QVGroupBox(rightWidget, "blockGroupBox");
    blockGroupBox->setTitle(tr(model_->type()));

    // add a container, because the layout of the blockGroupBox cannot
    // be changed
    QWidget *blockWidget = new QWidget(blockGroupBox);
    QGridLayout *blockLayout
        = new QGridLayout(blockWidget, WIDGET_SPACING, WIDGET_SPACING,
                          WIDGET_SPACING, WIDGET_SPACING);

    if (INSTANCEOF(model_, CpuModel)) {
        cpuIdSpinBox = new QSpinBox(blockWidget, "cpuIdSpinBox");
        blockLayout->addWidget(new QLabel(tr("ID"), blockWidget), 0, 0);
        blockLayout->addWidget(cpuIdSpinBox, 0, 1);
    }

    blockNameLineEdit = new QLineEdit(blockWidget, "blockNameLineEdit" );
    blockDescrLineEdit = new QLineEdit(blockWidget, "blockDescrLineEdit");
    offsetSpinBox = new QSpinBox(blockWidget, "offsetSpinBox");
    offsetSpinBox->setRange(-1, INT_MAX);
    offsetSpinBox->setSuffix(tr(" ns"));
    clockSpinBox = new QSpinBox(blockWidget, "clockSpinBox");
    clockSpinBox->setRange(0, INT_MAX);
    clockSpinBox->setSuffix(tr(" ns"));

    blockLayout->addWidget(new QLabel(tr("Name"), blockWidget), 1, 0);
    blockLayout->addMultiCellWidget(blockNameLineEdit, 1, 1, 1, 2);
    blockLayout->addWidget(new QLabel(tr("Description"), blockWidget), 2, 0);
    blockLayout->addMultiCellWidget(blockDescrLineEdit, 2, 2, 1, 2);
    blockLayout->addWidget(new QLabel(tr("Offset"), blockWidget), 3, 0);
    blockLayout->addWidget(offsetSpinBox, 3, 1);
    blockLayout->addWidget(new QLabel(tr("Clock"), blockWidget), 4, 0);
    blockLayout->addWidget(clockSpinBox, 4, 1);

    if (model_->hasRuntime()) {
        runtimeSpinBox = new QSpinBox(blockWidget, "runtimeSpinBox");
        runtimeSpinBox->setRange(0, INT_MAX);
        runtimeSpinBox->setSuffix(tr(" ns"));

        blockLayout->addWidget(new QLabel(tr("Runtime"), blockWidget), 5, 0);
        blockLayout->addWidget(runtimeSpinBox, 5, 1);
    }

    rightLayout->addWidget(blockGroupBox);
}

void BlockConfDialog::initCompileEditButtonWidget()
{

    // edit/compile button widget
    QWidget *compileEditButtonsWidget = new QWidget(rightWidget);

    QBoxLayout *compileEditButtonsLayout =
        new QHBoxLayout(compileEditButtonsWidget, WIDGET_SPACING);

    QPushButton *editCodePushButton =
        new QPushButton(compileEditButtonsWidget, "editCodePushButton" );
    editCodePushButton->setText(tr("&Edit Code"));
    connect(editCodePushButton, SIGNAL(clicked()), this, SLOT(edit()));

    QPushButton *compilePushButton =
        new QPushButton(compileEditButtonsWidget, "compilePushButton");
    compilePushButton->setText(tr("Co&mpile"));
    connect(compilePushButton, SIGNAL(clicked()), this, SLOT(compile()));

    QPushButton *calcRuntimePushButton =
        new QPushButton(compileEditButtonsWidget, "calcRuntimeButton");
    calcRuntimePushButton->setEnabled(false);
    calcRuntimePushButton->setText(tr("Runtime"));
    connect(calcRuntimePushButton, SIGNAL(clicked()), this, SLOT(calcRuntime()));

    compileEditButtonsLayout->addWidget(editCodePushButton);
    compileEditButtonsLayout->addWidget(compilePushButton);
    compileEditButtonsLayout->addWidget(calcRuntimePushButton);
    compileEditButtonsLayout->addStretch(1);

    rightLayout->addWidget(compileEditButtonsWidget);

}

void BlockConfDialog::initBottomWidget()
{

    QBoxLayout *bottomLayout = new QHBoxLayout(bottomWidget, WIDGET_SPACING);

    // ok button
    QPushButton *okPushButton = new QPushButton(bottomWidget, "okPushButton");
    okPushButton->setText(tr("&OK"));
    okPushButton->setDefault(TRUE);
    connect(okPushButton, SIGNAL(clicked()), this, SLOT(ok()));

    // help button
    QPushButton *helpPushButton = new QPushButton(bottomWidget, "helpPushButton");
    helpPushButton->setText(tr("&Help"));

    // apply button
    QPushButton *applyPushButton
        = new QPushButton(bottomWidget, "applyPushButton");
    applyPushButton->setText(tr("&Apply"));
    connect(applyPushButton, SIGNAL(clicked()), this, SLOT(apply()));

    // cancel button
    QPushButton *cancelPushButton
        = new QPushButton(bottomWidget, "cancelPushButton" );
    cancelPushButton->setText(tr("&Cancel"));
    connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(cancel()));

    bottomLayout->addStretch(1);
    bottomLayout->addWidget(okPushButton);
    bottomLayout->addWidget(helpPushButton);
    bottomLayout->addWidget(applyPushButton);
    bottomLayout->addWidget(cancelPushButton);

}

void BlockConfDialog::initListView()
{
    // I/O list view
    ioListView = new QListView(leftWidget, "ioListView");
    ioListView->addColumn(tr("Position"));
    ioListView->addColumn(tr("Name"));
    ioListView->addColumn(tr("Bits"));
    ioListView->addColumn(tr("Address"));
    ioListView->setAllColumnsShowFocus(TRUE);
    ioListView->setMinimumWidth(300);
    ioListView->setSorting(10); // 10 > number of columns
    ioListView->setDefaultRenameAction(QListView::Accept);
    connect(ioListView, SIGNAL(selectionChanged()),
            this, SLOT(ioSelectionChanged()));
    connect(ioListView,
            SIGNAL(mouseButtonClicked(int,QListViewItem *,const QPoint &,int)),
            this,
            SLOT(mouseButtonClicked(int,QListViewItem *,const QPoint &,int)));

    // I/O list view manipulation widget
    QWidget *editIoWidget = new QWidget(leftWidget);
    QGridLayout *editIoLayout =
        new QGridLayout(editIoWidget, 3, 3, WIDGET_SPACING);

    ioNameLineEdit = new QLineEdit(editIoWidget, "ioNameLineEdit");
    addressLineEdit = new QLineEdit(editIoWidget, "addressLineEdit");
    bitsLineEdit = new QLineEdit(editIoWidget, "bitsLineEdit");

    editIoLayout->addWidget(new QLabel(tr("Name"), editIoWidget), 0, 0);
    editIoLayout->addWidget(ioNameLineEdit, 0, 1);
    editIoLayout->addWidget(new QLabel(tr("Bits"), editIoWidget), 1, 0);
    editIoLayout->addWidget(bitsLineEdit, 1, 1);
    editIoLayout->addWidget
        (new QLabel(tr("Address"), editIoWidget), 1, 2);
    editIoLayout->addWidget(addressLineEdit, 1, 3);


    // I/O manipulation buttons
    QWidget *ioButtonsWidget = new QWidget(leftWidget);
    QBoxLayout *ioButtonsLayout =
        new QHBoxLayout(ioButtonsWidget, WIDGET_SPACING);

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
    ioButtonsLayout->addStretch(1);

    leftLayout->addWidget(ioListView);
    leftLayout->addWidget(ioButtonsWidget);
    leftLayout->addWidget(editIoWidget);
}

void BlockConfDialog::syncModel() {

    // add pin root list item
    inputRoot_ = new PinListViewItem(ioListView, 0, PinModel::INPUT);
    inputRoot_->setText(0, tr("Inputs"));
    inputRoot_->setVisible(model_->hasInputPins());

    outputRoot_ = new PinListViewItem(ioListView, 0, PinModel::OUTPUT);
    outputRoot_->setText(0, tr("Outputs"));
    outputRoot_->setVisible(model_->hasOutputPins());

    episodicRoot_ =
            new PinListViewItem(ioListView, 0, PinModel::EPISODIC);
    episodicRoot_->setText(0, tr("Episodic Inputs"));
    episodicRoot_->setVisible(model_->hasEpisodicPins());

    blockNameLineEdit->setText(model_->name());
    blockDescrLineEdit->setText(model_->description());
    clockSpinBox->setValue(model_->clock());
    offsetSpinBox->setValue
        (model_->autoOffset() ? -1 : (int)model_->offset());
    offsetSpinBox->setSpecialValueText
        (QString("Auto (%1 ns)").arg(model_->offset()));

    if (INSTANCEOF(model_, CpuModel)) {
        CpuModel *cpuModel = dynamic_cast<CpuModel *>(model_);
        cpuIdSpinBox->setValue(cpuModel->cpuId());
        runtimeSpinBox->setRange(-1, INT_MAX);
        runtimeSpinBox->setSpecialValueText
            (QString("Auto (%1 ns)").arg(cpuModel->runtime()));
        runtimeSpinBox->setValue
            (cpuModel->autoRuntime() ? -1 : (int)cpuModel->runtime());
    }
    else if (model_->hasRuntime()) {
        runtimeSpinBox->setRange(0, INT_MAX);
        runtimeSpinBox->setSpecialValueText("");
        runtimeSpinBox->setValue(model_->runtime());
    }

    QValueList<PinModel *> pinList = model_->pins();
    for (QValueList<PinModel *>::iterator it = pinList.begin();
         it != pinList.end(); ++it) {

        PinModel *pin = (*it);
        switch (pin->type()) {
        case PinModel::INPUT:
            new PinListViewItem(inputRoot_, 0, pin->clone(), pin);
            break;
        case PinModel::OUTPUT:
            new PinListViewItem(outputRoot_, 0, pin->clone(), pin);
            break;
        case PinModel::EPISODIC:
            new PinListViewItem(episodicRoot_, 0, pin->clone(), pin);
            break;
        }
    }

    ioListView->setSorting(0);
    ioListView->sort();
    ioListView->setSorting(10);
}

void BlockConfDialog::updateModel()
{
    model_->setName(blockNameLineEdit->text());
    model_->setDescription(blockDescrLineEdit->text());
    model_->setClock(clockSpinBox->value());
    if (offsetSpinBox->value() != -1) {
        model_->setOffset(offsetSpinBox->value());
        model_->setAutoOffset(false);
    }
    else {
        model_->setAutoOffset(true);
    }

    if (INSTANCEOF(model_, CpuModel)) {
        CpuModel *cpuModel = (CpuModel *)model_;
        cpuModel->setCpuId(cpuIdSpinBox->value());
        if (runtimeSpinBox->value() != -1) {
            cpuModel->setRuntime(runtimeSpinBox->value());
            cpuModel->setAutoRuntime(false);
        }
        else {
            cpuModel->setAutoRuntime(true);
        }
    }
    else if (model_->hasRuntime()) {
        model_->setRuntime(runtimeSpinBox->value());
    }

    // updated pins
    for (QPtrListIterator<PinModel> uit(updatedPins_); uit != 0; ++uit) {
        PinModel *pin = *uit;
        PinModel *origPin = model_->findPinById(pin->id());
        origPin->setName(pin->name());
        origPin->setAddress(pin->address());
        origPin->setBits(pin->bits());
        origPin->setType(pin->type());
        origPin->setPosition(pin->position());
        origPin->updatePerformed();
    }
    updatedPins_.clear();

    // deleted pins
    for (QPtrListIterator<PinModel> dit(deletedPins_); dit != 0; ++dit) {
        model_->deletePin(*dit);
    }
    deletedPins_.clear();

    // new pins
    for (QPtrListIterator<PinModel> nit(newPins_); nit != 0; ++nit) {
        PinModel *pin = *nit;

        model_->addPin(pin->clone());
    }
    newPins_.clear();

    // clears list view
    ioListView->clear();

    // notify model about update to repaint views
    model_->updatePerformed();

    // sync again
    syncModel();
}

void BlockConfDialog::newIo()
{
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
}

void BlockConfDialog::updateIo()
{
    PinListViewItem *item =
        (PinListViewItem *)ioListView->selectedItem();

    if (item != 0 && !item->isRoot()) {
        PinModel *pin = item->data();
        if (pin != 0) {
            pin->setName(ioNameLineEdit->text());
            bool ok;
            pin->setAddress(addressLineEdit->text().toUInt(&ok, 16));
            // TODO: check ok and pop up an error dialog, if ok is false
            pin->setBits(bitsLineEdit->text().toUInt(&ok, 10));
            if (item->origData() != 0) {
                updatedPins_.append(pin);
            }
            updatePositions(item->type());
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

        PinModel *pin = item->data();
        PinModel *origPin = item->origData();
        if (origPin != 0) {
            if (origPin->connected() == 0) {
                // Save deleted pins, to clean up views
                // if the changes will be applied.
                if (updatedPins_.contains(origPin)) {
                    updatedPins_.remove(origPin);
                }
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
                        if (updatedPins_.contains(origPin)) {
                            updatedPins_.remove(origPin);
                        }
                        deletedPins_.append(origPin);
                        break;
                    case 1: // Cancel removal.
                        return;
                        break;
                }
            }
        }

        if (newPins_.contains(pin)) {
            newPins_.remove(pin);
        }
        root->takeItem(item);
        updatePositions(item->type());
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

    ioNameLineEdit->setEnabled(isChild);
    addressLineEdit->setEnabled(isChild && !isPeriodical);
    bitsLineEdit->setEnabled(isChild);

    ioNameLineEdit->setText(isChild ? item->text(1) : QString(""));
    bitsLineEdit->setText(isChild ? item->text(2) : QString(""));
    addressLineEdit->setText(isChild ? item->text(3) : QString(""));
}

void BlockConfDialog::cancel()
{
    reject();
}

void BlockConfDialog::apply()
{
    updateModel();
}

void BlockConfDialog::ok()
{
    updateModel();
    accept();
}

void BlockConfDialog::calcRuntime()
{
    if (INSTANCEOF(model_, CpuModel)) {
        CpuModel *model = (CpuModel *)model_;
        int runtime = RuntimeManager::calculateRuntime(model);
        if (runtime != -1) {
            model->setRuntime(runtime);
        }
    }

}

void BlockConfDialog::mouseButtonClicked(int button, QListViewItem *item,
                                         const QPoint &, int c)
{
    if (button == Qt::LeftButton && item != 0) {
        item->startRename(c);
    }
}

void BlockConfDialog::compile()
{
    if (INSTANCEOF(model_, CpuModel)) {
        CpuModel *model = (CpuModel *)model_;
        // Note: This additional slot is needed, because the
        // clicked() signal of QPushButton is not able to
        // emit the current <code>model_</code>.
        CodeManager *codeManager = CodeManager::instance();
        codeManager->compile(model);
    }
}

void BlockConfDialog::edit()
{
    if (INSTANCEOF(model_, CpuModel)) {
        CpuModel *model = (CpuModel *)model_;
        // Note: This additional slot is needed, because the
        // clicked() signal of QPushButton is not able to
        // emit the current <code>model_</code>.
        CodeManager *codeManager = CodeManager::instance();
        if (!codeManager->edit(model)) {
            QMessageBox::critical(this, "POA", "The code cannot be edited.\n\n"
                                 "This may depend on a bad configuration of the"
                                 " external editor or\n"
                                 "you have no read/write permission to the"
                                 " filesystem.", "&OK");
        }
    }
}

void BlockConfDialog::updatePositions(PinModel::PinType type)
{
    unsigned position = BlockModel::FIRST_PIN_POSISION;
    for (QListViewItemIterator it(ioListView); it.current(); ++it) {
        PinListViewItem *item = (PinListViewItem *)it.current();
        if (!item->isRoot()) {
            PinModel *pin = item->data();

            if (pin->type() == type) {
                pin->setPosition(position);
                PinModel *origPin = item->origData();
                if (origPin && origPin->position() != position) {
                    updatedPins_.append(pin);
                }
                position++;
            }
            item->update();
        }
    }
}
