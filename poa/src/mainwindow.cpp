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
 * $Id: mainwindow.cpp,v 1.4 2003/08/20 10:33:16 squig Exp $
 *
 *****************************************************************************/

#include "mainwindow.h"
#include "aboutdialog.h"
#include "moduleconfdialog.h"

#include <qvariant.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qaction.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qtoolbar.h>
#include <qimage.h>
#include <qpixmap.h>

// contains all toolbar icons
#include "imagedata.h"

/* 
 *  Constructs a MainWindow which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 */
MainWindow::MainWindow( QWidget* parent,  const char* name, WFlags fl )
    : QMainWindow( parent, name, fl )
{
    (void)statusBar();
    QPixmap image0( ( const char** ) image0_data );
    QPixmap image1( ( const char** ) image1_data );
    QPixmap image2( ( const char** ) image2_data );
    QPixmap image3( ( const char** ) image3_data );
    QPixmap image4( ( const char** ) image4_data );
    QPixmap image5( ( const char** ) image5_data );
    QPixmap image6( ( const char** ) image6_data );
    QPixmap image7( ( const char** ) image7_data );
    QPixmap image8( ( const char** ) image8_data );
    QPixmap image9( ( const char** ) image9_data );
    if ( !name )
	setName( "MainWindow" );
    resize( 520, 480 ); 
    setCaption( trUtf8( "POA" ) );

    // actions
    fileNewAction = new QAction( this, "fileNewAction" );
    fileNewAction->setIconSet( QIconSet( image0 ) );
    fileNewAction->setText( trUtf8( "New" ) );
    fileNewAction->setMenuText( trUtf8( "&New" ) );
    fileNewAction->setAccel( 4194382 );
    fileOpenAction = new QAction( this, "fileOpenAction" );
    fileOpenAction->setIconSet( QIconSet( image1 ) );
    fileOpenAction->setText( trUtf8( "Open" ) );
    fileOpenAction->setMenuText( trUtf8( "&Open..." ) );
    fileOpenAction->setAccel( 4194383 );
    fileSaveAction = new QAction( this, "fileSaveAction" );
    fileSaveAction->setIconSet( QIconSet( image2 ) );
    fileSaveAction->setText( trUtf8( "Save" ) );
    fileSaveAction->setMenuText( trUtf8( "&Save" ) );
    fileSaveAction->setAccel( 4194387 );
    fileSaveAsAction = new QAction( this, "fileSaveAsAction" );
    fileSaveAsAction->setText( trUtf8( "Save As" ) );
    fileSaveAsAction->setMenuText( trUtf8( "Save &As..." ) );
    fileSaveAsAction->setAccel( 0 );
    filePrintAction = new QAction( this, "filePrintAction" );
    filePrintAction->setIconSet( QIconSet( image3 ) );
    filePrintAction->setText( trUtf8( "Print" ) );
    filePrintAction->setMenuText( trUtf8( "&Print..." ) );
    filePrintAction->setAccel( 4194384 );
    fileExitAction = new QAction( this, "fileExitAction" );
    fileExitAction->setText( trUtf8( "Exit" ) );
    fileExitAction->setMenuText( trUtf8( "E&xit" ) );
    fileExitAction->setAccel( 0 );
    editUndoAction = new QAction( this, "editUndoAction" );
    editUndoAction->setIconSet( QIconSet( image4 ) );
    editUndoAction->setText( trUtf8( "Undo" ) );
    editUndoAction->setMenuText( trUtf8( "&Undo" ) );
    editUndoAction->setAccel( 4194394 );
    editRedoAction = new QAction( this, "editRedoAction" );
    editRedoAction->setIconSet( QIconSet( image5 ) );
    editRedoAction->setText( trUtf8( "Redo" ) );
    editRedoAction->setMenuText( trUtf8( "&Redo" ) );
    editRedoAction->setAccel( 4194393 );
    editCutAction = new QAction( this, "editCutAction" );
    editCutAction->setIconSet( QIconSet( image6 ) );
    editCutAction->setText( trUtf8( "Cut" ) );
    editCutAction->setMenuText( trUtf8( "&Cut" ) );
    editCutAction->setAccel( 4194392 );
    editCopyAction = new QAction( this, "editCopyAction" );
    editCopyAction->setIconSet( QIconSet( image7 ) );
    editCopyAction->setText( trUtf8( "Copy" ) );
    editCopyAction->setMenuText( trUtf8( "C&opy" ) );
    editCopyAction->setAccel( 4194371 );
    editPasteAction = new QAction( this, "editPasteAction" );
    editPasteAction->setIconSet( QIconSet( image8 ) );
    editPasteAction->setText( trUtf8( "Paste" ) );
    editPasteAction->setMenuText( trUtf8( "&Paste" ) );
    editPasteAction->setAccel( 4194390 );
    editFindAction = new QAction( this, "editFindAction" );
    editFindAction->setIconSet( QIconSet( image9 ) );
    editFindAction->setText( trUtf8( "Find" ) );
    editFindAction->setMenuText( trUtf8( "&Find..." ) );
    editFindAction->setAccel( 4194374 );
    helpContentsAction = new QAction( this, "helpContentsAction" );
    helpContentsAction->setText( trUtf8( "Contents" ) );
    helpContentsAction->setMenuText( trUtf8( "&Contents..." ) );
    helpContentsAction->setAccel( 0 );
    helpIndexAction = new QAction( this, "helpIndexAction" );
    helpIndexAction->setText( trUtf8( "Index" ) );
    helpIndexAction->setMenuText( trUtf8( "&Index..." ) );
    helpIndexAction->setAccel( 0 );
    helpAboutAction = new QAction( this, "helpAboutAction" );
    helpAboutAction->setText( trUtf8( "About" ) );
    helpAboutAction->setMenuText( trUtf8( "&About..." ) );
    helpAboutAction->setAccel( 0 );

    openModuleConfDialogAction = new QAction( this, "openCpuConfDialogAction" );
    openModuleConfDialogAction->setText( trUtf8( "CPU configuration" ) );
    openModuleConfDialogAction->setMenuText( trUtf8( "&CPU configuration..." ) );
    openModuleConfDialogAction->setAccel( 0 );



    // toolbars
    commonToolBar = new QToolBar( "", this, DockTop ); 

    commonToolBar->setLabel( trUtf8( "common toolbar" ) );
    fileNewAction->addTo( commonToolBar );
    fileOpenAction->addTo( commonToolBar );
    fileSaveAction->addTo( commonToolBar );
    commonToolBar->addSeparator();
    editCutAction->addTo( commonToolBar );
    editCopyAction->addTo( commonToolBar );
    editPasteAction->addTo( commonToolBar );
    utilToolBar = new QToolBar( "", this, DockTop ); 

    utilToolBar->setLabel( trUtf8( "utility toolbar" ) );
    openModuleConfDialogAction->addTo(utilToolBar);
        
    drawToolBar = new QToolBar( "", this, DockTop ); 

    drawToolBar->setLabel( trUtf8( "draw toolbar" ) );


    // menubar
    menubar = new QMenuBar( this, "menubar" );

    fileMenu = new QPopupMenu( this ); 
    fileNewAction->addTo( fileMenu );
    fileOpenAction->addTo( fileMenu );
    fileSaveAction->addTo( fileMenu );
    fileSaveAsAction->addTo( fileMenu );
    fileMenu->insertSeparator();
    fileExitAction->addTo( fileMenu );
    menubar->insertItem( trUtf8( "&File" ), fileMenu );

    editMenu = new QPopupMenu( this ); 
    editCutAction->addTo( editMenu );
    editCopyAction->addTo( editMenu );
    editPasteAction->addTo( editMenu );
    editMenu->insertSeparator();
    menubar->insertItem(trUtf8("&Edit"), editMenu);

    toolsMenu = new QPopupMenu( this ); 
    menubar->insertItem(trUtf8("&Tools"), toolsMenu);
    openModuleConfDialogAction->addTo(toolsMenu);

    PopupMenu_3 = new QPopupMenu( this ); 
    menubar->insertItem( trUtf8( "&Draw" ), PopupMenu_3 );

    helpMenu = new QPopupMenu( this ); 
    helpContentsAction->addTo( helpMenu );
    helpMenu->insertSeparator();
    helpAboutAction->addTo( helpMenu );
    menubar->insertItem( trUtf8( "&Help" ), helpMenu );



    // signals and slots connections
    connect( fileNewAction, SIGNAL( activated() ), this, SLOT( fileNew() ) );
    connect( fileOpenAction, SIGNAL( activated() ), this, SLOT( fileOpen() ) );
    connect( fileSaveAction, SIGNAL( activated() ), this, SLOT( fileSave() ) );
    connect( fileSaveAsAction, SIGNAL( activated() ), this, SLOT( fileSaveAs() ) );
    connect( filePrintAction, SIGNAL( activated() ), this, SLOT( filePrint() ) );
    connect( fileExitAction, SIGNAL( activated() ), this, SLOT( fileExit() ) );
    connect( editUndoAction, SIGNAL( activated() ), this, SLOT( editUndo() ) );
    connect( editRedoAction, SIGNAL( activated() ), this, SLOT( editRedo() ) );
    connect( editCutAction, SIGNAL( activated() ), this, SLOT( editCut() ) );
    connect( editCopyAction, SIGNAL( activated() ), this, SLOT( editCopy() ) );
    connect( editPasteAction, SIGNAL( activated() ), this, SLOT( editPaste() ) );
    connect( editFindAction, SIGNAL( activated() ), this, SLOT( editFind() ) );
    connect( helpIndexAction, SIGNAL( activated() ), this, SLOT( helpIndex() ) );
    connect( helpContentsAction, SIGNAL( activated() ), this, SLOT( helpContents() ) );
    connect( helpAboutAction, SIGNAL( activated() ), this, SLOT( helpAbout() ) );

    connect( openModuleConfDialogAction, SIGNAL(activated()),
             this, SLOT(openModuleConfDialog()) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
MainWindow::~MainWindow()
{
    // no need to delete child widgets, Qt does it all for us
}

void MainWindow::fileNew()
{
    qWarning( "MainWindow::fileNew(): Not implemented yet!" );
}

void MainWindow::fileOpen()
{
    qWarning( "MainWindow::fileOpen(): Not implemented yet!" );
}

void MainWindow::fileSave()
{
    qWarning( "MainWindow::fileSave(): Not implemented yet!" );
}

void MainWindow::fileSaveAs()
{
    qWarning( "MainWindow::fileSaveAs(): Not implemented yet!" );
}

void MainWindow::filePrint()
{
    qWarning( "MainWindow::filePrint(): Not implemented yet!" );
}

void MainWindow::fileExit()
{
    qWarning( "MainWindow::fileExit(): Not implemented yet!" );
}

void MainWindow::editUndo()
{
    qWarning( "MainWindow::editUndo(): Not implemented yet!" );
}

void MainWindow::editRedo()
{
    qWarning( "MainWindow::editRedo(): Not implemented yet!" );
}

void MainWindow::editCut()
{
    qWarning( "MainWindow::editCut(): Not implemented yet!" );
}

void MainWindow::editCopy()
{
    qWarning( "MainWindow::editCopy(): Not implemented yet!" );
}

void MainWindow::editPaste()
{
    qWarning( "MainWindow::editPaste(): Not implemented yet!" );
}

void MainWindow::editFind()
{
    qWarning( "MainWindow::editFind(): Not implemented yet!" );
}

void MainWindow::helpIndex()
{
    qWarning( "MainWindow::helpIndex(): Not implemented yet!" );
}

void MainWindow::helpContents()
{
    qWarning( "MainWindow::helpContents(): Not implemented yet!" );
}

void MainWindow::helpAbout()
{
	AboutDialog *dialog = new AboutDialog();
	dialog->show();
}

void MainWindow::openModuleConfDialog()
{
    ModuleConfDialog *moduleConfDialog = new ModuleConfDialog();
    moduleConfDialog->show();


}
