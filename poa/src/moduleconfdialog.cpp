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
 * $Id: moduleconfdialog.cpp,v 1.2 2003/08/20 09:47:53 garbeam Exp $
 *
 *****************************************************************************/

/****************************************************************************
** Form implementation generated from reading ui file 'moduleconfdialog.ui'
**
** Created: Tue Aug 19 15:37:56 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
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


/* 
 *  Constructs a ModuleConfDialog which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ModuleConfDialog::ModuleConfDialog( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "ModuleConfDialog" );
    resize( 578, 485 ); 
    setCaption( trUtf8( "CPU <name>" ) );

    compilePushButton = new QPushButton( this, "compilePushButton" );
    compilePushButton->setGeometry( QRect( 470, 370, 90, 30 ) ); 
    compilePushButton->setText( trUtf8( "Co&mpile" ) );

    editCodePushButton = new QPushButton( this, "editCodePushButton" );
    editCodePushButton->setGeometry( QRect( 470, 330, 90, 30 ) ); 
    editCodePushButton->setText( trUtf8( "&Edit code" ) );

    helpPushButton = new QPushButton( this, "helpPushButton" );
    helpPushButton->setGeometry( QRect( 20, 440, 100, 30 ) ); 
    helpPushButton->setText( trUtf8( "&Help" ) );

    cancelPushButton = new QPushButton( this, "cancelPushButton" );
    cancelPushButton->setGeometry( QRect( 470, 440, 90, 30 ) ); 
    cancelPushButton->setText( trUtf8( "&Cancel" ) );

    connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(reject()));

    addIoPushButton = new QPushButton( this, "addIoPushButton" );
    addIoPushButton->setGeometry( QRect( 350, 330, 90, 30 ) ); 
    addIoPushButton->setText( trUtf8( "&Add I/O" ) );

    removeIoPushButton = new QPushButton( this, "removeIoPushButton" );
    removeIoPushButton->setGeometry( QRect( 350, 370, 90, 30 ) ); 
    removeIoPushButton->setText( trUtf8( "&Remove I/O" ) );

    okPushButton = new QPushButton( this, "okPushButton" );
    okPushButton->setGeometry( QRect( 350, 440, 90, 30 ) ); 
    okPushButton->setText( trUtf8( "&OK" ) );
    okPushButton->setToggleButton( FALSE );
    okPushButton->setDefault( TRUE );

    ioListView = new QListView( this, "ioListView" );
    ioListView->addColumn( trUtf8( "I/O" ) );
    ioListView->addColumn( trUtf8( "name" ) );
    ioListView->addColumn( trUtf8( "address" ) );
    ioListView->addColumn( trUtf8( "bits" ) );
    QListViewItem * item = new QListViewItem( ioListView, 0 );
    item->setText( 0, trUtf8( "input" ) );

    item = new QListViewItem( ioListView, item );
    item->setText( 0, trUtf8( "output" ) );

    item = new QListViewItem( ioListView, item );
    item->setText( 0, trUtf8( "periodical" ) );

    ioListView->setGeometry( QRect( 20, 20, 300, 390 ) ); 

    cpuGroupBox = new QGroupBox( this, "cpuGroupBox" );
    cpuGroupBox->setGeometry( QRect( 350, 20, 210, 80 ) ); 
    cpuGroupBox->setTitle( trUtf8( "CPU" ) );

    cpuNameTextLabel = new QLabel( cpuGroupBox, "cpuNameTextLabel" );
    cpuNameTextLabel->setGeometry( QRect( 10, 20, 34, 21 ) ); 
    cpuNameTextLabel->setText( trUtf8( "name" ) );

    cpuNameLineEdit = new QLineEdit( cpuGroupBox, "cpuNameLineEdit" );
    cpuNameLineEdit->setGeometry( QRect( 48, 20, 140, 22 ) ); 

    cpuClockTextLabel = new QLabel( cpuGroupBox, "cpuClockTextLabel" );
    cpuClockTextLabel->setGeometry( QRect( 10, 50, 65, 20 ) ); 
    cpuClockTextLabel->setText( trUtf8( "clock" ) );

    cpuClockSpinBox = new QSpinBox( cpuGroupBox, "cpuClockSpinBox" );
    cpuClockSpinBox->setGeometry( QRect( 110, 50, 55, 22 ) ); 

    clockMsTextLabel = new QLabel( cpuGroupBox, "clockMsTextLabel" );
    clockMsTextLabel->setGeometry( QRect( 170, 50, 20, 20 ) ); 
    clockMsTextLabel->setText( trUtf8( "ms" ) );

    offsetButtonGroup = new QButtonGroup( this, "offsetButtonGroup" );
    offsetButtonGroup->setGeometry( QRect( 350, 120, 210, 80 ) ); 
    offsetButtonGroup->setTitle( trUtf8( "offset" ) );

    offsetMsTextLabel = new QLabel( offsetButtonGroup, "offsetMsTextLabel" );
    offsetMsTextLabel->setGeometry( QRect( 180, 50, 20, 20 ) ); 
    offsetMsTextLabel->setText( trUtf8( "ms" ) );

    offsetAutoCalcRadioButton = new QRadioButton( offsetButtonGroup, "offsetAutoCalcRadioButton" );
    offsetAutoCalcRadioButton->setGeometry( QRect( 10, 20, 140, 20 ) ); 
    offsetAutoCalcRadioButton->setText( trUtf8( "automatic calculation" ) );

    offsetRadioButton = new QRadioButton( offsetButtonGroup, "offsetRadioButton" );
    offsetRadioButton->setGeometry( QRect( 10, 50, 60, 20 ) ); 
    offsetRadioButton->setText( trUtf8( "offset" ) );

    offsetSpinBox = new QSpinBox( offsetButtonGroup, "offsetSpinBox" );
    offsetSpinBox->setGeometry( QRect( 110, 50, 55, 22 ) ); 

    runtimeButtonGroup = new QButtonGroup( this, "runtimeButtonGroup" );
    runtimeButtonGroup->setGeometry( QRect( 350, 220, 210, 80 ) ); 
    runtimeButtonGroup->setTitle( trUtf8( "runtime" ) );

    runtimeMsTextLabel = new QLabel( runtimeButtonGroup, "runtimeMsTextLabel" );
    runtimeMsTextLabel->setGeometry( QRect( 170, 50, 20, 20 ) ); 
    runtimeMsTextLabel->setText( trUtf8( "ms" ) );

    runtimeAutoCalcRadioButton = new QRadioButton( runtimeButtonGroup, "runtimeAutoCalcRadioButton" );
    runtimeAutoCalcRadioButton->setGeometry( QRect( 10, 20, 140, 20 ) ); 
    runtimeAutoCalcRadioButton->setText( trUtf8( "automatic calculation" ) );

    runtimeSpinBox = new QSpinBox( runtimeButtonGroup, "runtimeSpinBox" );
    runtimeSpinBox->setGeometry( QRect( 110, 50, 55, 22 ) ); 

    runtimeRadioButton = new QRadioButton( runtimeButtonGroup, "runtimeRadioButton" );
    runtimeRadioButton->setGeometry( QRect( 10, 50, 90, 20 ) ); 
    runtimeRadioButton->setText( trUtf8( "runtime" ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
ModuleConfDialog::~ModuleConfDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

