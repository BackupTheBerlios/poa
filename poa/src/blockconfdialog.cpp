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
 * $Id: blockconfdialog.cpp,v 1.24 2003/09/17 15:03:36 garbeam Exp $
 *
 *****************************************************************************/

#include "blockconfdialog.h"
#include "canvasview.h"
#include "codemanager.h"
#include "cpumodel.h"
#include "coremodel.h"
#include "inputmodel.h"
#include "mainwindow.h"
#include "outputmodel.h"
#include "pinvector.h"
#include "poa.h"

#include <qvariant.h>
#include <qbuttongroup.h>
#include <qgroupbox.h>
#include <qheader.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlistview.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qlayout.h>

PinListViewItem::PinListViewItem(QListView *parent,
                                 QListViewItem *after,
                                 PinModel::PinType type)
    : QListViewItem(parent, after)
{
    setOpen(true);
    type_ = type;
    clone_ = origin_ = 0;
}

PinListViewItem::PinListViewItem(QListViewItem *parent,
                                 PinModel *clone, PinModel *origin)
    : QListViewItem(parent)
{
    setOpen(false);
    if (clone != 0) {
        type_ = clone->type();
    }
    clone_ = clone;
    origin_ = origin;

    update();
}

void PinListViewItem::update() {

    if (clone_ != 0) {
        setText(0, QString::number(clone_->id(), 10));
        setText(1, clone_->name());
        if (clone_->type() != PinModel::EPISODIC) {
            setText(2, QString::number(clone_->address(), 16));
        }
        setText(3, QString::number(clone_->bits(), 10));
    }
}

PinListViewItem::~PinListViewItem()
{
    delete clone_;
}

PinModel *PinListViewItem::data() const
{
    return clone_;
}

PinModel *PinListViewItem::origData() const
{
    return origin_;
}

PinModel::PinType PinListViewItem::type() {
    return type_;
}

bool PinListViewItem::isRoot() {
    return isOpen();
}

///////////////////////////////////////////////////////////////////////////////

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
    deletedPins_ = new PinVector();

    initLayout();

    // disable widgets, if neccessary
    syncModel();
    ioSelectionChanged();
    toggleManualOffset();
    toggleManualRuntime();
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

    if (!(INSTANCEOF(model_, InputModel) || INSTANCEOF(model_, OutputModel))) {
        if (INSTANCEOF(model_, CpuModel)) {
            initOffsetWidget();
        }
        initRuntimeWidget();
    }

    if (INSTANCEOF(model_, CpuModel)) {
        initCompileEditButtonWidget();
    }

    rightLayout->addItem(new QSpacerItem(1, 1, QSizePolicy::Maximum,
                                          QSizePolicy::Minimum));
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
    QGroupBox *blockGroupBox = new QGroupBox(rightWidget, "blockGroupBox");
    blockGroupBox->setTitle(tr(model_->type()));

    QGridLayout *blockLayout =
        new QGridLayout(blockGroupBox, 4, 3, 3 * WIDGET_SPACING);

    if (INSTANCEOF(model_, CpuModel)) {
        cpuIdSpinBox = new QSpinBox(blockGroupBox, "cpuIdSpinBox");
        blockLayout->addWidget(new QLabel(tr("id"), blockGroupBox), 0, 0);
        blockLayout->addWidget(cpuIdSpinBox, 0, 1);
    }

    blockNameLineEdit = new QLineEdit(blockGroupBox, "blockNameLineEdit" );
    blockDescrLineEdit = new QLineEdit(blockGroupBox, "blockDescrLineEdit");
    blockClockSpinBox = new QSpinBox(blockGroupBox, "blockClockSpinBox");

    blockLayout->addWidget(new QLabel(tr("name"), blockGroupBox), 1, 0);
    blockLayout->addWidget(blockNameLineEdit, 1, 1);
    blockLayout->addWidget(new QLabel(tr("description"), blockGroupBox), 2, 0);
    blockLayout->addWidget(blockDescrLineEdit, 2, 1);
    blockLayout->addWidget(new QLabel(tr("clock"), blockGroupBox), 3, 0);
    blockLayout->addWidget(blockClockSpinBox, 3, 1);
    blockLayout->addWidget(new QLabel(tr("ms"), blockGroupBox), 3, 2);

    rightLayout->addWidget(blockGroupBox);
}

void BlockConfDialog::initOffsetWidget()
{

    // offset widget
    QButtonGroup *offsetButtonGroup =
        new QButtonGroup(rightWidget, "offsetButtonGroup" );
    offsetButtonGroup->setTitle(tr("offset"));

    QGridLayout *offsetLayout = new QGridLayout(offsetButtonGroup, 2, 3,
                                                3 * WIDGET_SPACING);

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

    rightLayout->addWidget(offsetButtonGroup);
}

void BlockConfDialog::initRuntimeWidget()
{

    if (INSTANCEOF(model_, CpuModel)) {
        // runtime widget
        QButtonGroup *runtimeButtonGroup =
            new QButtonGroup(rightWidget, "runtimeButtonGroup");
        runtimeButtonGroup->setTitle(tr("runtime"));

        runtimeSpinBox = new QSpinBox(runtimeButtonGroup, "runtimeSpinBox");

        QGridLayout *runtimeLayout = new QGridLayout(runtimeButtonGroup, 2, 3,
                                                     3 * WIDGET_SPACING);
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
        runtimeLayout->addWidget(runtimeAutoCalcRadioButton, 0, 0);
        runtimeLayout->addWidget(runtimeRadioButton, 1, 0);
        runtimeLayout->addWidget(runtimeSpinBox, 1, 1);
        runtimeLayout->addWidget(new QLabel(tr("ms"), runtimeButtonGroup), 1, 2);
        rightLayout->addWidget(runtimeButtonGroup);
    }
    else {
        // runtime widget for non cpu blocks (redundancy because of
        // aestetic reasons)
        QGroupBox *runtimeGroupBox =
            new QGroupBox(rightWidget, "runtimeGroupBox");
        runtimeGroupBox->setTitle(tr("runtime"));

        runtimeSpinBox = new QSpinBox(runtimeGroupBox, "runtimeSpinBox");

        // Note: For some Qt specific reasons, a spacing of 5px within
        // QBoxLayout produces overlapping labels. So WIDGET_SPACING
        // isn't used here.
        QBoxLayout *runtimeLayout = new QHBoxLayout(runtimeGroupBox, 15);
        runtimeLayout->addWidget(new QLabel(tr("runtime"), runtimeGroupBox));
        runtimeLayout->addWidget(runtimeSpinBox);
        runtimeLayout->addWidget(new QLabel(tr("ms"), runtimeGroupBox));
        rightLayout->addWidget(runtimeGroupBox);
    }
}

void BlockConfDialog::initCompileEditButtonWidget()
{

    // edit/compile button widget
    QWidget *compileEditButtonsWidget = new QWidget(rightWidget);

    QBoxLayout *compileEditButtonsLayout =
        new QHBoxLayout(compileEditButtonsWidget, WIDGET_SPACING);

    editCodePushButton =
        new QPushButton(compileEditButtonsWidget, "editCodePushButton" );
    editCodePushButton->setText(tr("&Edit code"));

    compilePushButton =
        new QPushButton(compileEditButtonsWidget, "compilePushButton");
    compilePushButton->setText(tr("Co&mpile"));
    connect(compilePushButton, SIGNAL(clicked()), this, SLOT(compile()));


    compileEditButtonsLayout->addWidget(editCodePushButton);
    compileEditButtonsLayout->addWidget(compilePushButton);

    rightLayout->addWidget(compileEditButtonsWidget);

}

void BlockConfDialog::initBottomWidget()
{

    QBoxLayout *bottomLayout = new QHBoxLayout(bottomWidget, WIDGET_SPACING);

    // ok button
    okPushButton = new QPushButton(bottomWidget, "okPushButton");
    okPushButton->setText(tr("&OK"));
    okPushButton->setDefault(TRUE);
    connect(okPushButton, SIGNAL(clicked()), this, SLOT(ok()));

    // help button
    helpPushButton = new QPushButton(bottomWidget, "helpPushButton");
    helpPushButton->setText(tr("&Help"));

    // apply button
    applyPushButton = new QPushButton(bottomWidget, "applyPushButton");
    applyPushButton->setText(tr("&Apply"));
    connect(applyPushButton, SIGNAL(clicked()), this, SLOT(apply()));

    // cancel button
    cancelPushButton = new QPushButton(bottomWidget, "cancelPushButton" );
    cancelPushButton->setText(tr("&Cancel"));
    connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(cancel()));

    bottomLayout->addWidget(okPushButton);
    bottomLayout->addWidget(helpPushButton);
    bottomLayout->addWidget(applyPushButton);
    bottomLayout->addWidget(cancelPushButton);

}

void BlockConfDialog::initListView()
{
    // I/O list view
    ioListView = new QListView(leftWidget, "ioListView");
    ioListView->addColumn(tr("I/O"));
    ioListView->addColumn(tr("name"));
    ioListView->addColumn(tr("address"));
    ioListView->addColumn(tr("bits"));
    ioListView->setAllColumnsShowFocus(TRUE);
    ioListView->setMinimumWidth(300);
    connect(ioListView, SIGNAL(selectionChanged()),
            this, SLOT(ioSelectionChanged()));

    if (INSTANCEOF(model_, CpuModel) || INSTANCEOF(model_, CoreModel)
        || INSTANCEOF(model_, InputModel))
    {
        // inputs root
        inputRoot_ = new PinListViewItem(ioListView, 0, PinModel::INPUT);
        inputRoot_->setText(0, tr("inputs"));
    }

    if (INSTANCEOF(model_, CpuModel) || INSTANCEOF(model_, CoreModel)
        || INSTANCEOF(model_, OutputModel))
    {
        // outputs root
        outputRoot_ = new PinListViewItem(ioListView, inputRoot_,
                                          PinModel::OUTPUT);
        outputRoot_->setText(0, tr("outputs"));
    }

    if (!(INSTANCEOF(model_, InputModel) || INSTANCEOF(model_, OutputModel))) {
        // episodic root
        episodicRoot_ =
            new PinListViewItem(ioListView, outputRoot_, PinModel::EPISODIC);
        episodicRoot_->setText(0, tr("episodic inputs"));
    }

    leftLayout->addWidget(ioListView);

    // I/O list view manipulation widget
    QWidget *editIoWidget = new QWidget(leftWidget);
    QGridLayout *editIoLayout =
        new QGridLayout(editIoWidget, 3, 4, WIDGET_SPACING);

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

    leftLayout->addWidget(ioButtonsWidget);
    leftLayout->addWidget(editIoWidget);
}

void BlockConfDialog::addPins(PinVector pins, PinListViewItem *root) {

    for (unsigned i = 0; i < pins.size(); ++i) {
        PinListViewItem *child =
            new PinListViewItem((QListViewItem *)root, pins[i]->clone(),
            pins[i]);
        child->setVisible(true);
    }
}

void BlockConfDialog::syncModel() {

    if (model_ != 0) {
        blockNameLineEdit->setText(model_->name());
        blockDescrLineEdit->setText(model_->description());
        runtimeSpinBox->setValue(model_->execTime());
        blockClockSpinBox->setValue(model_->clock());

        if (INSTANCEOF(model_, CpuModel)) {
            CpuModel *cpuModel = (CpuModel *)model_;
            cpuIdSpinBox->setValue(cpuModel->cpuId());
            offsetAutoCalcRadioButton->setChecked(cpuModel->autoOffset());
            offsetRadioButton->setChecked(!cpuModel->autoOffset());
            runtimeAutoCalcRadioButton->setChecked(cpuModel->autoExecTime());
            runtimeRadioButton->setChecked(!cpuModel->autoExecTime());
            offsetSpinBox->setValue(cpuModel->offset());
        }

        addPins(*(model_->inputPins()), inputRoot_);
        addPins(*(model_->outputPins()), outputRoot_);
        addPins(*(model_->episodicPins()), episodicRoot_);
    }
}

void BlockConfDialog::updateModel() {

    if (model_ != 0) {
        model_->setName(blockNameLineEdit->text());
        model_->setDescription(blockDescrLineEdit->text());
        model_->setExecTime(runtimeSpinBox->value());
        model_->setClock(blockClockSpinBox->value());

        if (INSTANCEOF(model_, CpuModel)) {
            CpuModel *cpuModel = (CpuModel *)model_;
            cpuModel->setCpuId(cpuIdSpinBox->value());
            cpuModel->setAutoOffset(offsetAutoCalcRadioButton->isChecked());
            cpuModel->setAutoExecTime(runtimeAutoCalcRadioButton->isChecked());
            cpuModel->setOffset(offsetSpinBox->value());
        }

        // clear and free deleted pins
        deletedPins_->removeAllPins(true);

        // clear models
        model_->inputPins()->removeAllPins(false);
        model_->outputPins()->removeAllPins(false);
        model_->episodicPins()->removeAllPins(false);

        QListViewItemIterator it(ioListView);
        for ( ; it.current(); ++it) {
            PinListViewItem *item = (PinListViewItem *)it.current();
            if (!item->isRoot()) {
                PinModel *source = item->data();
                PinModel *target = item->origData();
                if (target != 0) {
                    target->setId(source->id());
                    target->setName(source->name());
                    target->setAddress(source->address());
                    target->setBits(source->bits());
                    target->setType(source->type());
                    // use original pin
                    model_->addPin(target, 0, false);
                }
                else {
                    model_->addPin(source->clone(), 0, true);
                }
            }
        }
        // Notify model about update, so the view will be
        // repaint.
        ((AbstractModel *)model_)->updatePerformed();
    }
}

void BlockConfDialog::newIo()
{
    PinListViewItem *item = (PinListViewItem *)ioListView->selectedItem();

    if (item != 0) {
        while (!item->isRoot()) {
            item = (PinListViewItem *)item->parent();
        }
        int childCount = item->childCount() + 1;
        PinModel *pin = new PinModel(model_, childCount,
                "data" + QString::number(childCount),
                childCount * 100, 32, item->type());
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
            bool ok;
            pin->setId(ioNumberLineEdit->text().toUInt(&ok, 10));
            pin->setAddress(addressLineEdit->text().toUInt(&ok, 16));
            // TODO: check ok and pop up an error dialog, if ok is false
            pin->setBits(bitsLineEdit->text().toUInt(&ok, 10));
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
        PinModel *origin = item->origData();
        bool remove = true;
        if (origin != 0) {
            if(origin->connected() == 0) {
                // Save deleted pins, to clean up views
                // if the changes will be applied.
                // deletedPins_->append(item->origData()); // QT 3.1+
                // specific
                deletedPins_->addBefore(item->origData());
            }
            else {
                // there exists a connection between the selected pin
                // and another one
                PinModel *connected = origin->connected();
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
                        deletedPins_->addBefore(origin);
                        break;
                    case 1: // Cancel removal.
                        remove = false;
                        break;
                }
            }
        }

        if (remove) {
            root->takeItem(item);
            delete item;
        }
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
    if (INSTANCEOF(model_, CpuModel)) {
        offsetSpinBox->setEnabled(offsetRadioButton->isChecked());
    }
}

void BlockConfDialog::toggleManualRuntime() {
    if (INSTANCEOF(model_, CpuModel)) {
        runtimeSpinBox->setEnabled(runtimeRadioButton->isChecked());
    }
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
