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
 * $Id: moduleconfdialog.cpp,v 1.5 2003/09/01 19:53:05 garbeam Exp $
 *
 *****************************************************************************/

#include "moduleconfdialog.h"

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

#define INPUTS_TEXT "inputs"
#define OUTPUTS_TEXT "outputs"
#define PERIODICAL_TEXT "periodical inputs"


ModuleConfDialog::ModuleConfDialog( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
    setName( "ModuleConfDialog" );
    resize( 578, 485 ); 
    setCaption( tr( "CPU <name>" ) );


    QBoxLayout *topLayout = new QHBoxLayout(this, 5);
    QWidget *leftWidget = new QWidget(this);
//    QBoxLayout *leftLayout = new QVBoxLayout(leftWidget, 5);
    QWidget *rightWidget = new QWidget(this);
//    QBoxLayout *rightLayout = new QVBoxLayout(rightWidget, 5);

    topLayout->addWidget(leftWidget);
    topLayout->addWidget(rightWidget);

    ioListView = new QListView(leftWidget, "ioListView");
    ioListView->addColumn(tr("I/O"));
    ioListView->addColumn(tr("name"));
    ioListView->addColumn(tr("address"));
    ioListView->addColumn(tr("bits"));
    ioListView->setAllColumnsShowFocus(TRUE);


    compilePushButton = new QPushButton( this, "compilePushButton" );
    compilePushButton->setGeometry( QRect( 470, 370, 90, 30 ) ); 
    compilePushButton->setText( tr( "Co&mpile" ) );

    editCodePushButton = new QPushButton( this, "editCodePushButton" );
    editCodePushButton->setGeometry( QRect( 470, 330, 90, 30 ) ); 
    editCodePushButton->setText( tr( "&Edit code" ) );

    helpPushButton = new QPushButton( this, "helpPushButton" );
    helpPushButton->setGeometry( QRect( 20, 440, 100, 30 ) ); 
    helpPushButton->setText( tr( "&Help" ) );

    cancelPushButton = new QPushButton( this, "cancelPushButton" );
    cancelPushButton->setGeometry( QRect( 470, 440, 90, 30 ) ); 
    cancelPushButton->setText( tr( "&Cancel" ) );

    connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(reject()));

    addIoPushButton = new QPushButton( this, "addIoPushButton" );
    addIoPushButton->setGeometry( QRect( 350, 330, 90, 30 ) ); 
    addIoPushButton->setText( tr( "&Add I/O" ) );

    connect(addIoPushButton, SIGNAL(clicked()), this, SLOT(addIo()));

    removeIoPushButton = new QPushButton( this, "removeIoPushButton" );
    removeIoPushButton->setGeometry( QRect( 350, 370, 90, 30 ) ); 
    removeIoPushButton->setText( tr( "&Remove I/O" ) );

    connect(removeIoPushButton, SIGNAL(clicked()), this, SLOT(removeIo()));

    okPushButton = new QPushButton( this, "okPushButton" );
    okPushButton->setGeometry( QRect( 350, 440, 90, 30 ) ); 
    okPushButton->setText( tr( "&OK" ) );
    okPushButton->setToggleButton( FALSE );
    okPushButton->setDefault( TRUE );

    connect(okPushButton, SIGNAL(clicked()), this, SLOT(accept()));

    // inputs root
    inputRoot = new QListViewItem( ioListView, 0 );
    inputRoot->setText(0, tr(INPUTS_TEXT));
    inputRoot->setOpen(TRUE);

    // outputs root
    outputRoot = new QListViewItem(ioListView, inputRoot);
    outputRoot->setText(0, tr(OUTPUTS_TEXT));
    outputRoot->setOpen(TRUE);

    // periodical root
    periodicalRoot = new QListViewItem(ioListView, outputRoot);
    periodicalRoot->setText(0, tr(PERIODICAL_TEXT));
    periodicalRoot->setOpen(TRUE);

    ioListView->setGeometry( QRect( 20, 20, 300, 350 ) ); 


    ioNumberTextLabel = new QLabel(this, "ioNumberTextLabel");
    ioNumberTextLabel->setGeometry(QRect(20, 380, 40, 20));
    ioNumberTextLabel->setText(tr("I/O"));

    ioNumberLineEdit = new QLineEdit(this, "ioNumberLineEdit");
    ioNumberLineEdit->setGeometry(QRect(60, 380, 100, 20));
    

    cpuGroupBox = new QGroupBox( this, "cpuGroupBox" );
    cpuGroupBox->setGeometry( QRect( 350, 20, 210, 80 ) ); 
    cpuGroupBox->setTitle( tr( "CPU" ) );

    cpuNameTextLabel = new QLabel( cpuGroupBox, "cpuNameTextLabel" );
    cpuNameTextLabel->setGeometry( QRect( 10, 20, 34, 21 ) ); 
    cpuNameTextLabel->setText( tr( "name" ) );

    cpuNameLineEdit = new QLineEdit( cpuGroupBox, "cpuNameLineEdit" );
    cpuNameLineEdit->setGeometry( QRect( 48, 20, 140, 22 ) ); 

    cpuClockTextLabel = new QLabel( cpuGroupBox, "cpuClockTextLabel" );
    cpuClockTextLabel->setGeometry( QRect( 10, 50, 65, 20 ) ); 
    cpuClockTextLabel->setText( tr( "clock" ) );

    cpuClockSpinBox = new QSpinBox( cpuGroupBox, "cpuClockSpinBox" );
    cpuClockSpinBox->setGeometry( QRect( 110, 50, 55, 22 ) ); 

    clockMsTextLabel = new QLabel( cpuGroupBox, "clockMsTextLabel" );
    clockMsTextLabel->setGeometry( QRect( 170, 50, 20, 20 ) ); 
    clockMsTextLabel->setText( tr( "ms" ) );

    offsetButtonGroup = new QButtonGroup( this, "offsetButtonGroup" );
    offsetButtonGroup->setGeometry( QRect( 350, 120, 210, 80 ) ); 
    offsetButtonGroup->setTitle( tr( "offset" ) );

    offsetMsTextLabel = new QLabel( offsetButtonGroup, "offsetMsTextLabel" );
    offsetMsTextLabel->setGeometry( QRect( 180, 50, 20, 20 ) ); 
    offsetMsTextLabel->setText( tr( "ms" ) );

    offsetAutoCalcRadioButton = new QRadioButton( offsetButtonGroup, "offsetAutoCalcRadioButton" );
    offsetAutoCalcRadioButton->setGeometry( QRect( 10, 20, 140, 20 ) ); 
    offsetAutoCalcRadioButton->setText( tr( "automatic calculation" ) );

    offsetRadioButton = new QRadioButton( offsetButtonGroup, "offsetRadioButton" );
    offsetRadioButton->setGeometry( QRect( 10, 50, 60, 20 ) ); 
    offsetRadioButton->setText( tr( "offset" ) );

    offsetSpinBox = new QSpinBox( offsetButtonGroup, "offsetSpinBox" );
    offsetSpinBox->setGeometry( QRect( 110, 50, 55, 22 ) ); 

    runtimeButtonGroup = new QButtonGroup( this, "runtimeButtonGroup" );
    runtimeButtonGroup->setGeometry( QRect( 350, 220, 210, 80 ) ); 
    runtimeButtonGroup->setTitle( tr( "runtime" ) );

    runtimeMsTextLabel = new QLabel( runtimeButtonGroup, "runtimeMsTextLabel" );
    runtimeMsTextLabel->setGeometry( QRect( 170, 50, 20, 20 ) ); 
    runtimeMsTextLabel->setText( tr( "ms" ) );

    runtimeAutoCalcRadioButton = new QRadioButton( runtimeButtonGroup, "runtimeAutoCalcRadioButton" );
    runtimeAutoCalcRadioButton->setGeometry( QRect( 10, 20, 140, 20 ) ); 
    runtimeAutoCalcRadioButton->setText( tr( "automatic calculation" ) );

    runtimeSpinBox = new QSpinBox( runtimeButtonGroup, "runtimeSpinBox" );
    runtimeSpinBox->setGeometry( QRect( 110, 50, 55, 22 ) ); 

    runtimeRadioButton = new QRadioButton( runtimeButtonGroup, "runtimeRadioButton" );
    runtimeRadioButton->setGeometry( QRect( 10, 50, 90, 20 ) ); 
    runtimeRadioButton->setText( tr( "runtime" ) );
}

/**
 *  Destroys the object and frees any allocated resources
 */
ModuleConfDialog::~ModuleConfDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

void ModuleConfDialog::addIo()
{
    QListViewItem *root = ioListView->selectedItem();

    if (root != 0) {
        while (!root->isOpen()) {
            root = root->parent();
        }
        QListViewItem *child = new QListViewItem(root); 
        child->setText(0, QString::number(root->childCount(), 10));
        child->setText(1, "data" + child->text(0));
        child->setText(3, "32");
        if (root->text(0).compare(tr(PERIODICAL_TEXT)) == 0) {
           child->setText(2, "");
        } else {
           child->setText(2,
                          QString::number(root->childCount() * 100, 16));
        }
    }
}

void ModuleConfDialog::removeIo()
{
    QListViewItem *item = ioListView->selectedItem();
    QListViewItem *root = item;

    if (root != 0) {
        while (!root->isOpen()) {
            root = root->parent();
        }
        root->takeItem(item);
        delete item;
    }
}
