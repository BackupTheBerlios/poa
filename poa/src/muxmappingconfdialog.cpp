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
 * $Id: muxmappingconfdialog.cpp,v 1.4 2004/01/28 15:39:12 garbeam Exp $
 *
 *****************************************************************************/

#include <qcombobox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qwidget.h>

#include "muxmappingconfdialog.h"
#include "muxconfdialog.h"
#include "muxmodel.h"
#include "pinlistviewitem.h"
#include "pinmodel.h"
#include "poa.h"

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

    QWidget *comboInputWidget = new QWidget(ioWidget);
    QBoxLayout *comboInputLayout
        = new QHBoxLayout(comboInputWidget, WIDGET_SPACING);
    QWidget *comboOutputWidget = new QWidget(ioWidget);
    QBoxLayout *comboOutputLayout
        = new QHBoxLayout(comboOutputWidget, WIDGET_SPACING);

    inputComboBox_ = new QComboBox(comboInputWidget);
    outputComboBox_ = new QComboBox(comboOutputWidget);
    comboInputLayout->add(new QLabel(tr("Inputs "), comboInputWidget));
    comboInputLayout->add(inputComboBox_);
    comboOutputLayout->add(new QLabel(tr("Outputs"), comboOutputWidget));
    comboOutputLayout->add(outputComboBox_);

    ioLayout->addWidget(comboInputWidget);
    ioLayout->addWidget(comboOutputWidget);

    // range widget
    QWidget *rangeWidget = new QWidget(this);
    QBoxLayout *rangeLayout = new QHBoxLayout(rangeWidget, WIDGET_SPACING);

    firstInputBitSpinBox_ =
        new QSpinBox(rangeWidget, "firstInputBitSpinBox");
    lastInputBitSpinBox_ =
        new QSpinBox(rangeWidget, "lastInputBitSpinBox");

    rangeLayout->addWidget(
        new QLabel(tr("first input bit"), rangeWidget));
    rangeLayout->addWidget(firstInputBitSpinBox_);
    rangeLayout->addWidget(
        new QLabel(tr("last input bit"), rangeWidget));
    rangeLayout->addWidget(lastInputBitSpinBox_);

    firstOutputBitSpinBox_ =
        new QSpinBox(rangeWidget, "firstOutputBitSpinBox");
    lastOutputBitSpinBox_ =
        new QSpinBox(rangeWidget, "lastOutputBitSpinBox");

    rangeLayout->addWidget(
        new QLabel(tr("first output bit"), rangeWidget));
    rangeLayout->addWidget(firstOutputBitSpinBox_);
    rangeLayout->addWidget(
        new QLabel(tr("last output bit"), rangeWidget));
    rangeLayout->addWidget(lastOutputBitSpinBox_);


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
    baseLayout->add(rangeWidget);
    baseLayout->add(buttonWidget);
}

void MuxMappingConfDialog::sync() {

    inputComboBox_->clear();
    outputComboBox_->clear();
    int inputIdx = 1, inputCount = 1;
    int outputIdx = 1, outputCount = 1;
    QListViewItemIterator it = *itemIter_;
    for (; it.current(); ++it) {
        PinListViewItem *item = (PinListViewItem *)it.current();
        if (item->type() == PinModel::INPUT) {
            inputComboBox_->insertItem(item->text(1));
            if (item->text(1) == muxMappingItem_->text(1)) {
                inputIdx = inputCount;
            } 
            inputCount++;
        }
        else if (item->type() == PinModel::OUTPUT) {
            outputComboBox_->insertItem(item->text(1));
            if (item->text(1) == muxMappingItem_->text(1)) {
                outputIdx = outputCount;
            } 
            outputCount++;
        }
    }
    inputComboBox_->setCurrentItem(inputIdx);
    outputComboBox_->setCurrentItem(outputIdx);

    firstInputBitSpinBox_->setValue(muxMappingItem_->firstInputBit());
    firstOutputBitSpinBox_->setValue(muxMappingItem_->firstOutputBit());
    lastOutputBitSpinBox_->setValue(muxMappingItem_->lastOutputBit());
    lastInputBitSpinBox_->setValue(muxMappingItem_->lastInputBit());
}

void MuxMappingConfDialog::commit() {

    muxMappingItem_->setText(0, inputComboBox_->currentText());
    muxMappingItem_->setText(2, inputComboBox_->currentText());
    muxMappingItem_->setText(1,
         QString::number(firstInputBitSpinBox_->value()) + " - "
         + QString::number(lastInputBitSpinBox_->value()));
    muxMappingItem_->setText(3,
         QString::number(firstOutputBitSpinBox_->value()) + " - "
         + QString::number(lastOutputBitSpinBox_->value()));


    muxMappingItem_->setFirstInputBit(firstInputBitSpinBox_->value());
    muxMappingItem_->setFirstOutputBit(firstOutputBitSpinBox_->value());
    muxMappingItem_->setLastInputBit(lastInputBitSpinBox_->value());
    muxMappingItem_->setLastOutputBit(lastOutputBitSpinBox_->value());
}

void MuxMappingConfDialog::ok() {
    commit();
    accept();
}
