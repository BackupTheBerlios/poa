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
 * $Id: deployprojectwizard.cpp,v 1.1 2003/09/25 17:25:19 papier Exp $
 *
 *****************************************************************************/

#include "deployprojectwizard.h"

#include <qvariant.h>
#include <qgroupbox.h>
#include <qheader.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qprogressbar.h>
#include <qtextedit.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>


/* 
 *  Constructs the DeployProjectWizard.
 *
 *  The wizard will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal wizard.
 */
DeployProjectWizard::DeployProjectWizard( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QWizard( parent, name, modal, fl )
{
    if ( !name )
	setName( "deployProjectWizard" );
    resize( 600, 480 ); 
    setCaption( trUtf8( "Deploy Project" ) );

    page = new QWidget( this, "page" );

    ErrorsGroupBox = new QGroupBox( page, "ErrorsGroupBox" );
    ErrorsGroupBox->setGeometry( QRect( 0, 271, 580, 130 ) ); 
    ErrorsGroupBox->setTitle( trUtf8( "Errors:" ) );

    ErrorsTextEdit = new QTextEdit( ErrorsGroupBox, "ErrorsTextEdit" );
    ErrorsTextEdit->setGeometry( QRect( 10, 20, 560, 104 ) ); 
    ErrorsTextEdit->setText( trUtf8( "pin xy is not connected" ) );

    CheckupListView = new QListView( page, "CheckupListView" );
    CheckupListView->addColumn( trUtf8( "Checkup" ) );
    CheckupListView->addColumn( trUtf8( "Correct?" ) );
    QListViewItem * item = new QListViewItem( CheckupListView, 0 );
    item->setText( 0, trUtf8( "pin connections" ) );
    item->setText( 1, trUtf8( "all pins connected" ) );

    item = new QListViewItem( CheckupListView, item );
    item->setText( 0, trUtf8( "other checks" ) );

    CheckupListView->setGeometry( QRect( 0, 0, 580, 270 ) ); 
    addPage( page, trUtf8( "Plausibility checkup" ) );

    page_2 = new QWidget( this, "page_2" );

    CompileListView = new QListView( page_2, "CompileListView" );
    CompileListView->addColumn( trUtf8( "CPU" ) );
    CompileListView->addColumn( trUtf8( "Compile" ) );
    item = new QListViewItem( CompileListView, 0 );
    item->setText( 0, trUtf8( "CPU 1" ) );
    item->setText( 1, trUtf8( "done" ) );

    item = new QListViewItem( CompileListView, item );
    item->setText( 0, trUtf8( "CPU 2" ) );
    item->setText( 1, trUtf8( "in progress" ) );

    item = new QListViewItem( CompileListView, item );
    item->setText( 0, trUtf8( "CPU 3" ) );

    item = new QListViewItem( CompileListView, item );
    item->setText( 0, trUtf8( "CPU 4" ) );

    item = new QListViewItem( CompileListView, item );
    item->setText( 0, trUtf8( "CPU 5" ) );
    item->setText( 1, trUtf8( "done" ) );

    item = new QListViewItem( CompileListView, item );
    item->setText( 0, trUtf8( "CPU 6" ) );
    item->setText( 1, trUtf8( "done" ) );

    item = new QListViewItem( CompileListView, item );
    item->setText( 0, trUtf8( "CPU x" ) );

    CompileListView->setGeometry( QRect( 0, 0, 580, 410 ) ); 
    addPage( page_2, trUtf8( "Compile" ) );

    page_3 = new QWidget( this, "page_3" );

    SchedulingListView = new QListView( page_3, "SchedulingListView" );
    SchedulingListView->addColumn( trUtf8( "Block" ) );
    SchedulingListView->addColumn( trUtf8( "Runtime" ) );
    SchedulingListView->addColumn( trUtf8( "Offset" ) );
    item = new QListViewItem( SchedulingListView, 0 );
    item->setText( 0, trUtf8( "New Item" ) );

    SchedulingListView->setGeometry( QRect( 0, 0, 580, 410 ) ); 
    addPage( page_3, trUtf8( "Scheduling" ) );

    page_4 = new QWidget( this, "page_4" );

    CompileTextLabel = new QLabel( page_4, "CompileTextLabel" );
    CompileTextLabel->setGeometry( QRect( 140, 70, 161, 31 ) ); 
    CompileTextLabel->setText( trUtf8( "Compiling..." ) );

    DownloadProgressBar = new QProgressBar( page_4, "DownloadProgressBar" );
    DownloadProgressBar->setGeometry( QRect( 140, 280, 280, 31 ) ); 

    DownloadTextLabel = new QLabel( page_4, "DownloadTextLabel" );
    DownloadTextLabel->setGeometry( QRect( 140, 230, 161, 31 ) ); 
    DownloadTextLabel->setText( trUtf8( "Downloading..." ) );

    CompileProgressBar = new QProgressBar( page_4, "CompileProgressBar" );
    CompileProgressBar->setGeometry( QRect( 140, 120, 280, 31 ) ); 
    addPage( page_4, trUtf8( "Compile and download Project" ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
DeployProjectWizard::~DeployProjectWizard()
{
    // no need to delete child widgets, Qt does it all for us
}

