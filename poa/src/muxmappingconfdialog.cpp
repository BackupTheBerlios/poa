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
 * $Id: muxmappingconfdialog.cpp,v 1.1 2004/01/28 02:20:40 garbeam Exp $
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
        QWidget* parent = 0,
        const char* name = 0,
        bool modal = FALSE, WFlags fl = 0)
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
    QWidget *comboWidget = new QWidget(this);
    QBoxLayout *comboLayout
        = new QHBoxLayout(comboWidget, WIDGET_SPACING);

    inputComboBox_ = new QComboBox(comboWidget);
    outputComboBox_ = new QComboBox(comboWidget);
    comboLayout->add(new QLabel(tr("Inputs "), comboWidget));
    comboLayout->add(inputComboBox_);
    comboLayout->add(new QLabel(tr("Outputs"), comboWidget));
    comboLayout->add(outputComboBox_);

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

    buttonLayout->add(okButton);
    buttonLayout->add(cancelButton);

    // base layout
    baseLayout->add(comboWidget);
    baseLayout->add(rangeWidget);
    baseLayout->add(buttonWidget);
}

void MuxMappingConfDialog::sync() {

    QListViewItemIterator it = *itemIter_;
    for (; it.current(); ++it) {
        PinListViewItem *item = (PinListViewItem *)it.current();
        if (item->type() == PinModel::INPUT) {
            inputComboBox_->insertItem(item->text(1));
        }
        else if (item->type() == PinModel::OUTPUT) {
            outputComboBox_->insertItem(item->text(1));
        }
    }
    inputComboBox_->setCurrentText(muxMappingItem_->text(0));
    outputComboBox_->setCurrentText(muxMappingItem_->text(2));

    MuxMapping *mapping = muxMappingItem_->mapping();
    if (mapping) {
        firstInputBitSpinBox_->setValue(mapping->firstInputBit());
        firstOutputBitSpinBox_->setValue(mapping->firstOutputBit());
        lastOutputBitSpinBox_->setValue(mapping->lastOutputBit());
        lastInputBitSpinBox_->setValue(mapping->lastInputBit());
    }
    else {
        // HACK, to prevent string parsing
        firstInputBitSpinBox_->setValue(0);
        firstOutputBitSpinBox_->setValue(0);
        lastInputBitSpinBox_->setValue(0);
        lastOutputBitSpinBox_->setValue(0);
    }
}

void MuxMappingConfDialog::commit() {

    // TODO

}

void MuxMappingConfDialog::cancel() {

}

void MuxMappingConfDialog::ok() {
    commit();
    accept();
}
