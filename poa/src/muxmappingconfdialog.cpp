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
 * $Id: muxmappingconfdialog.cpp,v 1.10 2004/02/04 12:55:31 garbeam Exp $
 *
 *****************************************************************************/

#include <qgroupbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qstring.h>
#include <qwidget.h>

#include "muxmappingconfdialog.h"
#include "muxconfdialog.h"
#include "muxmodel.h"
#include "pinlistviewitem.h"
#include "pinmodel.h"
#include "poa.h"

PinItemComboBox::PinItemComboBox(QWidget *parent) 
    : QComboBox(parent)
{
}

PinItemComboBox::~PinItemComboBox() {
}

void PinItemComboBox::insertItem(PinListViewItem *item) {
    if (items_.size() <= items_.count()) {
        items_.resize(items_.count() + 1);
    }
    unsigned pos = items_.count();
    QComboBox::insertItem(item->text(1), pos);
    items_.insert(pos, item);
}

PinListViewItem *PinItemComboBox::selectedItem() {
    return items_.at(currentItem());
}

//////////////////////////////////////////////////////////////////////////////

MuxMappingConfDialog::MuxMappingConfDialog(
        QListViewItemIterator iter,
        MuxMappingListViewItem *item,
        QWidget* parent,
        const char* name,
        bool modal, WFlags fl)
    : QDialog(parent, name, modal, fl)
{
    itemIter_ = iter;
    muxMappingItem_ = item;

    if (!name) {
        setName( "MuxMappingConfDialog" );
    }
    setCaption(tr("Mux mapping configuration"));

    initLayout();
    sync();
    inputSelectionChanged(0);
    outputSelectionChanged(0);
}

MuxMappingConfDialog::~MuxMappingConfDialog()
{
}

void MuxMappingConfDialog::initLayout() {

    QBoxLayout *baseLayout
        = new QVBoxLayout(this, WIDGET_SPACING);

    // I/Os
    QWidget *ioWidget = new QWidget(this);
    QBoxLayout *ioLayout
        = new QHBoxLayout(ioWidget, WIDGET_SPACING);

    QGroupBox *inputGroupBox = new QGroupBox(tr("Input"), ioWidget);
    QBoxLayout *inputGroupBoxLayout
        = new QVBoxLayout(inputGroupBox, 3 * WIDGET_SPACING);

    QWidget *inputWidget = new QWidget(inputGroupBox);
    QBoxLayout *inputLayout
        = new QVBoxLayout(inputWidget, WIDGET_SPACING);

    QGroupBox *outputGroupBox = new QGroupBox(tr("Output"), ioWidget);
    QBoxLayout *outputGroupBoxLayout
        = new QVBoxLayout(outputGroupBox, 3 * WIDGET_SPACING);

    QWidget *outputWidget = new QWidget(outputGroupBox);
    QBoxLayout *outputLayout
        = new QVBoxLayout(outputWidget, WIDGET_SPACING);

    inputComboBox_ = new PinItemComboBox(inputWidget);
    outputComboBox_ = new PinItemComboBox(outputWidget);
    inputComboBox_->setDuplicatesEnabled(false);
    outputComboBox_->setDuplicatesEnabled(false);
    connect(inputComboBox_, SIGNAL(activated(int)),
            this, SLOT(inputSelectionChanged(int)));

    // inputs range widget
    QWidget *inputRangeWidget = new QWidget(inputWidget);
    QBoxLayout *inputRangeLayout
        = new QHBoxLayout(inputRangeWidget, WIDGET_SPACING);

    firstInputBitSpinBox_ = new QSpinBox(inputRangeWidget);
    lastInputBitSpinBox_ = new QSpinBox(inputRangeWidget);

    inputRangeLayout->addWidget(
        new QLabel(tr("first bit: "), inputRangeWidget));
    inputRangeLayout->addWidget(firstInputBitSpinBox_);
    inputRangeLayout->addWidget(
        new QLabel(tr("last bit: "), inputRangeWidget));
    inputRangeLayout->addWidget(lastInputBitSpinBox_);

    // outputs range widget
    QWidget *outputRangeWidget = new QWidget(outputWidget);
    QBoxLayout *outputRangeLayout
        = new QHBoxLayout(outputRangeWidget, WIDGET_SPACING);

    firstOutputBitSpinBox_ = new QSpinBox(outputRangeWidget);
    lastOutputBitSpinBox_ = new QSpinBox(outputRangeWidget);

    outputRangeLayout->addWidget(
        new QLabel(tr("first bit: "), outputRangeWidget));
    outputRangeLayout->addWidget(firstOutputBitSpinBox_);
    outputRangeLayout->addWidget(
        new QLabel(tr("last bit: "), outputRangeWidget));
    outputRangeLayout->addWidget(lastOutputBitSpinBox_);

    // layouts
    inputLayout->addWidget(inputComboBox_);
    inputLayout->addWidget(inputRangeWidget);
    outputLayout->addWidget(outputComboBox_);
    outputLayout->addWidget(outputRangeWidget);

    inputGroupBoxLayout->addWidget(inputWidget);
    outputGroupBoxLayout->addWidget(outputWidget);

    ioLayout->addWidget(inputGroupBox);
    ioLayout->addWidget(outputGroupBox);

    // buttons
    QWidget *buttonWidget = new QWidget(this);
    QBoxLayout *buttonLayout
        = new QHBoxLayout(buttonWidget, WIDGET_SPACING);

    QPushButton *okButton = new QPushButton(buttonWidget);
    okButton->setText(tr("&Ok"));
    okButton->setDefault(TRUE);
    connect(okButton, SIGNAL(clicked()), this, SLOT(ok()));

    QPushButton *cancelButton = new QPushButton(buttonWidget);
    cancelButton->setText(tr("&Cancel"));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    buttonLayout->addStretch(1);
    buttonLayout->add(okButton);
    buttonLayout->add(cancelButton);
    buttonLayout->addStretch(1);

    // base layout
    baseLayout->add(ioWidget);
    baseLayout->add(buttonWidget);
}

void MuxMappingConfDialog::sync() {

    inputComboBox_->clear();
    outputComboBox_->clear();
    QListViewItemIterator it = itemIter_;
    for (; it.current(); ++it) {
        PinListViewItem *item = (PinListViewItem *)it.current();
        if (item->type() == PinModel::INPUT) {
            inputComboBox_->insertItem(item);
        }
        else if (item->type() == PinModel::OUTPUT) {
            outputComboBox_->insertItem(item);
        }
    }
    // HACK to eliminate empty entries of QComboBoxes
    inputComboBox_->removeItem(0);
    outputComboBox_->removeItem(0);
    if (muxMappingItem_->text(0) != "") {
        inputComboBox_->setCurrentText(muxMappingItem_->text(0));
    }
    else {
        inputComboBox_->setCurrentItem(0);
    }
    if (muxMappingItem_->text(2) != "") {
        outputComboBox_->setCurrentText(muxMappingItem_->text(2));
    }
    else {
        outputComboBox_->setCurrentItem(0);
    }

    firstInputBitSpinBox_->setValue(muxMappingItem_->firstInputBit());
    firstOutputBitSpinBox_->setValue(muxMappingItem_->firstOutputBit());
    lastOutputBitSpinBox_->setValue(muxMappingItem_->lastOutputBit());
    lastInputBitSpinBox_->setValue(muxMappingItem_->lastInputBit());
}

void MuxMappingConfDialog::commit() {

    muxMappingItem_->setInputPinListViewItem(inputComboBox_->selectedItem());
    muxMappingItem_->setOutputPinListViewItem(outputComboBox_->selectedItem());
    muxMappingItem_->setFirstInputBit(firstInputBitSpinBox_->value());
    muxMappingItem_->setFirstOutputBit(firstOutputBitSpinBox_->value());
    muxMappingItem_->setLastInputBit(lastInputBitSpinBox_->value());
    muxMappingItem_->setLastOutputBit(lastOutputBitSpinBox_->value());
}

void MuxMappingConfDialog::ok() {
    commit();
    accept();
}

void MuxMappingConfDialog::inputSelectionChanged(int index) {

    index = 0;
    PinListViewItem *item = inputComboBox_->selectedItem();
    unsigned value = item->text(2).toUInt();
    firstInputBitSpinBox_->setMaxValue(value);
    lastInputBitSpinBox_->setMaxValue(value);
}

void MuxMappingConfDialog::outputSelectionChanged(int index) {

    index = 0;
    PinListViewItem *item = outputComboBox_->selectedItem();
    unsigned value = item->text(2).toUInt();
    firstOutputBitSpinBox_->setMaxValue(value);
    lastOutputBitSpinBox_->setMaxValue(value);
}
