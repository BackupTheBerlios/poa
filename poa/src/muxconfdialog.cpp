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
 * $Id: muxconfdialog.cpp,v 1.1 2003/09/23 17:10:59 garbeam Exp $
 *
 *****************************************************************************/

#include "muxconfdialog.h"

#include "poa.h"

#include <qcombobox.h>
#include <qgroupbox.h>
#include <qheader.h>
#include <qimage.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qlistview.h>
#include <qpixmap.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qtooltip.h>
#include <qvariant.h>
#include <qwhatsthis.h>


MuxConfDialog::MuxConfDialog(QWidget* parent, const char* name, bool modal, WFlags fl)
    : QDialog(parent, name, modal, fl)
{
    if (!name) {
        setName( "MuxConfDialog" );
        resize(400, 500); 
    }
    setCaption(tr("Mux configuration"));

    initLayout();

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
    connect(okPushButton, SIGNAL(clicked()), this, SLOT(accept()));

    applyPushButton = new QPushButton(bottomWidget, "applyPushButton");
    applyPushButton->setText(tr("&Apply"));

    cancelPushButton = new QPushButton(bottomWidget, "cancelPushButton");
    cancelPushButton->setText(tr("&Cancel"));
    connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(reject()));

    bottomLayout->addWidget(helpPushButton);
    bottomLayout->addWidget(okPushButton);
    bottomLayout->addWidget(applyPushButton);
    bottomLayout->addWidget(cancelPushButton);

    // finish layout
    dialogLayout_->addWidget(bottomWidget);
}

/**
 *  Destroys the object and frees any allocated resources
 */
MuxConfDialog::~MuxConfDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

