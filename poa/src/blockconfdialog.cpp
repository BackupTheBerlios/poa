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
 * $Id: blockconfdialog.cpp,v 1.60 2004/01/28 16:35:51 squig Exp $
 *
 *****************************************************************************/

#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlistview.h>
#include <qmessagebox.h>
#include <qptrlist.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qlayout.h>
#include <qvgroupbox.h>

#include "blockconfdialog.h"
#include "blockconfwidget.h"
#include "blockmodel.h"
#include "canvasview.h"
#include "blockmodel.h"
#include "codemanager.h"
#include "cpumodel.h"
#include "mainwindow.h"
#include "pinlistviewitem.h"
#include "pinmodel.h"
#include "poa.h"
#include "poaexception.h"
#include "runtimemanager.h"

///////////////////////////////////////////////////////////////////////////////

BlockConfDialog::BlockConfDialog(BlockModel *model, Project *project,
                                 QWidget* parent, const char* name, bool modal,
                                 WFlags fl)
    : QDialog(parent, name, modal, fl)
{
    if (!name) {
        setName("BlockConfDialog");
    }
    resize(600, 400);
    setCaption(tr("Block configuration"));

    model_ = model;
    project_ = project;

    initLayout();

    // disable widgets, if neccessary
    sync();
}

BlockConfDialog::~BlockConfDialog()
{
    delete blockConfWidget_;
}

void BlockConfDialog::initLayout()
{
    dialogLayout = new QVBoxLayout(this, WIDGET_SPACING);
    topWidget = new QWidget(this);
    blockConfWidget_ = new BlockConfWidget(model_, topWidget);
    topLayout = new QHBoxLayout(topWidget, WIDGET_SPACING);
    rightWidget = new QWidget(topWidget);
    rightLayout = new QVBoxLayout(rightWidget, WIDGET_SPACING);
    bottomWidget = new QWidget(this);

    initBlockWidget();

    if (INSTANCEOF(model_, CpuModel)) {
        initCompileEditButtonWidget();
    }

    rightLayout->addStretch(1);
    topLayout->addWidget(blockConfWidget_);
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

void BlockConfDialog::sync() {

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

    // sync block conf widget first
    blockConfWidget_->sync();
}

bool BlockConfDialog::commit()
{
    bool success = true;

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

        success &= saveSource(cpuModel);
    }
    else if (model_->hasRuntime()) {
        model_->setRuntime(runtimeSpinBox->value());
    }

    // commits all changes to blockConfWidget_
    blockConfWidget_->commit();

    // notify model about update to repaint views
    if (blockConfWidget_->newPinSortOrder()) {
        model_->updatePerformed();
    }


    // sync again
    sync();

    return success;
}

void BlockConfDialog::cancel()
{
    reject();
}

void BlockConfDialog::apply()
{
    commit();
}

void BlockConfDialog::ok()
{
    if (commit()) {
        accept();
    }
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

void BlockConfDialog::compile()
{
    CpuModel *cpuModel = dynamic_cast<CpuModel *>(model_);
    if (cpuModel != 0 && saveSource(cpuModel)) {
        CodeManager codeManager(project_, cpuModel);
        codeManager.compile();
    }
}

void BlockConfDialog::edit()
{
    CpuModel *cpuModel = dynamic_cast<CpuModel *>(model_);
    if (cpuModel != 0 && saveSource(cpuModel)) {
        CodeManager codeManager(project_, cpuModel);
        if (!codeManager.edit()) {
            QMessageBox::critical
                (this, "POA", "The code cannot be edited.\n\n"
                 "This may depend on a bad configuration of the"
                 " external editor or\n"
                 "you have no read/write permission to the"
                 " filesystem.", "&OK");
        }
    }
}

bool BlockConfDialog::saveSource(CpuModel *cpuModel)
{
    try {
        CodeManager codeManager(project_, cpuModel);
        codeManager.createDirectories();

        QFile sourceFile(codeManager.sourceFilePath());
        if (!sourceFile.exists()) {
            if (cpuModel->source().isNull()) {
                codeManager.copyTemplate();
            }
            else {
                codeManager.saveSource();
            }
        }
        else {
            if (!cpuModel->source().isNull()) {
                QMessageBox mb
                    ("POA",
                     "The block contains uncommited source code, "
                     "but there is a file on disk already.\n"
                     "Overwrite existing file?",
                     QMessageBox::Information,
                     QMessageBox::Yes | QMessageBox::Default,
                     QMessageBox::No,
                     QMessageBox::Cancel | QMessageBox::Escape );
                switch(mb.exec()) {
                case QMessageBox::Yes:
                    codeManager.saveSource();
                    break;
                case QMessageBox::No:
                    break;
                case QMessageBox::Cancel:
                    return false;
                    break;
                }
            }
        }
    }
    catch (const PoaException e) {
        QMessageBox::warning(this, tr("POA Error"), e.message());
    }

    return true;
}
