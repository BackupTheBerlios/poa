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
 * $Id: deployprojectwizard.cpp,v 1.3 2003/09/30 20:05:52 papier Exp $
 *
 *****************************************************************************/

#include "deployprojectwizard.h"
#include "project.h"
#include "abstractmodel.h"
#include "blockmodel.h"
#include "pinmodel.h"

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

    setupCheckPage();
    setupCompilePage();
    setupSchedulingPage();
    setupDownloadPage();
}

void DeployProjectWizard::setupCheckPage()
{
    CheckPage = new QWidget( this, "CheckPage" );

    CheckListView = new QListView( CheckPage, "CheckupListView" );
    CheckListView->addColumn( trUtf8( "Checkup" ) );
    CheckListView->addColumn( trUtf8( "Correct?" ) );
    QListViewItem * CheckListItem = new QListViewItem( CheckListView, 0 );
    CheckListItem->setText( 0, trUtf8( "pin connections" ) );
    CheckListItem->setText( 1, trUtf8( "all pins connected" ) );

    CheckListItem = new QListViewItem( CheckListView, CheckListItem );
    CheckListItem->setText( 0, trUtf8( "other checks" ) );

    CheckListView->setGeometry( QRect( 0, 0, 580, 270 ) ); 

    ErrorsGroupBox = new QGroupBox( CheckPage, "ErrorsGroupBox" );
    ErrorsGroupBox->setGeometry( QRect( 0, 271, 580, 130 ) ); 
    ErrorsGroupBox->setTitle( trUtf8( "Errors:" ) );

    ErrorsTextEdit = new QTextEdit( ErrorsGroupBox, "ErrorsTextEdit" );
    ErrorsTextEdit->setGeometry( QRect( 10, 20, 560, 104 ) ); 
    ErrorsTextEdit->setText( trUtf8( "pin xy is not connected" ) );

    addPage( CheckPage, trUtf8( "Plausibility checkup" ) );
}

void DeployProjectWizard::setupCompilePage()
{
    CompilePage = new QWidget( this, "CompilePage" );

    CompileListView = new QListView( CompilePage, "CompileListView" );
    CompileListView->addColumn( trUtf8( "CPU" ) );
    CompileListView->addColumn( trUtf8( "Compile" ) );
    QListViewItem * CompileListItem = new QListViewItem( CompileListView, 0 );
    CompileListItem->setText( 0, trUtf8( "CPU 1" ) );
    CompileListItem->setText( 1, trUtf8( "done" ) );

    CompileListItem = new QListViewItem( CompileListView, CompileListItem );
    CompileListItem->setText( 0, trUtf8( "CPU 2" ) );
    CompileListItem->setText( 1, trUtf8( "in progress" ) );

    CompileListItem = new QListViewItem( CompileListView, CompileListItem );
    CompileListItem->setText( 0, trUtf8( "CPU 3" ) );

    CompileListItem = new QListViewItem( CompileListView, CompileListItem );
    CompileListItem->setText( 0, trUtf8( "CPU 4" ) );

    CompileListItem = new QListViewItem( CompileListView, CompileListItem );
    CompileListItem->setText( 0, trUtf8( "CPU 5" ) );
    CompileListItem->setText( 1, trUtf8( "done" ) );

    CompileListItem = new QListViewItem( CompileListView, CompileListItem );
    CompileListItem->setText( 0, trUtf8( "CPU 6" ) );
    CompileListItem->setText( 1, trUtf8( "done" ) );

    CompileListItem = new QListViewItem( CompileListView,CompileListItem );
    CompileListItem->setText( 0, trUtf8( "CPU x" ) );

    CompileListView->setGeometry( QRect( 0, 0, 580, 410 ) ); 
    addPage( CompilePage, trUtf8( "Compile" ) );
}

void DeployProjectWizard::setupSchedulingPage()
{
    SchedulingPage = new QWidget( this, "SchedulingPage" );

    SchedulingListView = new QListView( SchedulingPage, "SchedulingListView" );
    SchedulingListView->addColumn( trUtf8( "Block" ) );
    SchedulingListView->addColumn( trUtf8( "Runtime" ) );
    SchedulingListView->addColumn( trUtf8( "Offset" ) );
    QListViewItem * SchedulingListItem = 
      new QListViewItem( SchedulingListView, 0 );
    SchedulingListItem->setText( 0, trUtf8( "New Item" ) );

    SchedulingListView->setGeometry( QRect( 0, 0, 580, 410 ) ); 
    addPage( SchedulingPage, trUtf8( "Scheduling" ) );
}

void DeployProjectWizard::setupDownloadPage()
{
    DownloadPage = new QWidget( this, "DownloadPage" );

    CompileTextLabel = new QLabel( DownloadPage, "CompileTextLabel" );
    CompileTextLabel->setGeometry( QRect( 140, 70, 161, 31 ) ); 
    CompileTextLabel->setText( trUtf8( "Compiling..." ) );

    DownloadProgressBar = new QProgressBar( DownloadPage, "DownloadProgressBar" );
    DownloadProgressBar->setGeometry( QRect( 140, 280, 280, 31 ) ); 

    DownloadTextLabel = new QLabel( DownloadPage, "DownloadTextLabel" );
    DownloadTextLabel->setGeometry( QRect( 140, 230, 161, 31 ) ); 
    DownloadTextLabel->setText( trUtf8( "Downloading..." ) );

    CompileProgressBar = new QProgressBar( DownloadPage, "CompileProgressBar" );
    CompileProgressBar->setGeometry( QRect( 140, 120, 280, 31 ) ); 
    addPage( DownloadPage, trUtf8( "Compile and download Project" ) );
    
}

void DeployProjectWizard::showPage(QWidget* page)
{
  QWizard::showPage(page);

  if (page == CheckPage){

  }
  else if (page == CompilePage){

  }
  else if (page == SchedulingPage){

  }
  else if (page == DownloadPage){

  }

}

bool DeployProjectWizard::allPinsConnected(QPtrList<AbstractModel>* blocks){
  /*  for (uint i=0; i < blocks.count(); ++i){
    if blocks.at(i).hasInputPins() {
      for (PinVector::iterator j = blocks.at(i).inputPins().begin();
	   j != blocks.at(i).inputPins().end();
	   ++j){
	if (*j).*connected() == null {
	  // pin is not connected!!
	}
      }
    }
    if blocks.take(i).hasOutputPins() {
    }
    //nessary?? Or are Episodic Pins always connected??
    if block.take(i).hasEpisodicPins() {
    }
  }
  */
  return true;
}



/*  
 *  Destroys the object and frees any allocated resources
 */
DeployProjectWizard::~DeployProjectWizard()
{
    // no need to delete child widgets, Qt does it all for us
}

