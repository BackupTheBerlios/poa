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
 * $Id: mainwindow.cpp,v 1.7 2003/08/20 14:06:02 squig Exp $
 *
 *****************************************************************************/

#include "aboutdialog.h"
#include "imagedata.h" // contains toolbar icons
#include "mainwindow.h"
#include "moduleconfdialog.h"
#include "settingsdialog.h"

#include <qaction.h>
#include <qapplication.h>
#include <qimage.h>
#include <qlayout.h>
#include <qmenubar.h>
#include <qpixmap.h>
#include <qpopupmenu.h>
#include <qtoolbar.h>
#include <qtooltip.h>
#include <qvariant.h>
#include <qvbox.h>
#include <qwhatsthis.h>


// Note: should this be configurable?
#define ICON_PATH QString("icons/")

/** 
 * Constructs a MainWindow which is a child of 'parent', with the 
 * name 'name' and widget flags set to 'f'.
 */
MainWindow::MainWindow( QWidget* parent,  const char* name, WFlags fl )
    : QMainWindow( parent, name, fl )
{

    // initialize status bar implicitly
    (void)statusBar();

    QPixmap image_compile(ICON_PATH + "compile.png");
    QPixmap image_configure(ICON_PATH + "configure.png");
    QPixmap image_contents(ICON_PATH + "contents.png");
    QPixmap image_download(ICON_PATH + "dowload.png");
    QPixmap image_editcopy(ICON_PATH + "editcopy.png");
    QPixmap image_editcut(ICON_PATH + "editcut.png");
    QPixmap image_editdelete(ICON_PATH + "editdelete.png");
    QPixmap image_editpaste(ICON_PATH + "editpaste.png");
    QPixmap image_filenew(ICON_PATH + "filenew.png");
    QPixmap image_fileopen(ICON_PATH + "fileopen.png");
    QPixmap image_filesave(ICON_PATH + "filesave.png");
    QPixmap image_help(ICON_PATH + "help.png");
    QPixmap image_line(ICON_PATH + "line.xpm");
    QPixmap image_viewgrow(ICON_PATH + "viewgrow.png");
    QPixmap image_viewnormal(ICON_PATH + "viewnormal.png");
    QPixmap image_viewshrink(ICON_PATH + "viewshrink.png");
    
    if (!name) {
        setName("MainWindow");
    }
    resize(520, 480); 
    setCaption(trUtf8("POA"));

    /////////////////////////////////////////////////////////////////
    // actions
    fileNewAction =
        new QAction("New layout", image_filenew, "&New layout",
                    QKeySequence("Ctrl+N"), this, "fileNewAction");

    fileOpenAction =
        new QAction("Open layout...", image_fileopen, "&Open layout",
                    QKeySequence("Ctrl+O"), this, "fileOpenAction");

    fileSaveAction =
        new QAction("Save", image_filesave, "&Save",
                    QKeySequence("Ctrl+S"), this, "fileSaveAction");

    fileSaveAsAction =
        new QAction("Save As", "Save &As", 0, this, "fileSaveAsAction");

    fileExitAction =
        new QAction("Exit", "E&xit", 0, this, "fileExitAction");

    editCutAction =
        new QAction("Cut", image_editcut, "C&ut",
                    QKeySequence("Ctrl+X"), this, "editCutAction");

    editCopyAction =
        new QAction("Copy", image_editcopy, "&Copy",
                    QKeySequence("Ctrl+C"), this, "editCopyAction");

    editPasteAction =
        new QAction("Paste", image_editpaste, "&Paste",
                    QKeySequence("Ctrl+V"), this, "editPasteAction" );

    helpContentsAction =
        new QAction("Contents", image_contents, "&Contents...",
                    QKeySequence("F1"), this, "helpContentsAction");

    helpAboutAction =
        new QAction("About", image_help, "&About...", 0,
                    this, "helpAboutAction" );

    openModuleConfDialogAction =
        new QAction("Module configuration", image_configure,
                    "&Module configuration", 0, this,
                    "openModuleConfDialogAction");


    /////////////////////////////////////////////////////////////////
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
    utilToolBar->addSeparator();

        
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
    connect( fileNewAction, SIGNAL( activated() ), this, SLOT( newLayout() ) );
    connect( fileOpenAction, SIGNAL( activated() ), this, SLOT( fileOpen() ) );
    connect( fileSaveAction, SIGNAL( activated() ), this, SLOT( fileSave() ) );
    connect( fileSaveAsAction, SIGNAL( activated() ), this, SLOT( fileSaveAs() ) );
    connect( fileExitAction, SIGNAL( activated() ),
             qApp, SLOT(closeAllWindows()));
    connect( editCutAction, SIGNAL( activated() ), this, SLOT( editCut() ) );
    connect( editCopyAction, SIGNAL( activated() ), this, SLOT( editCopy() ) );
    connect( editPasteAction, SIGNAL( activated() ), this, SLOT( editPaste() ) );
    connect( helpContentsAction, SIGNAL( activated() ), this, SLOT( helpContents() ) );
    connect( helpAboutAction, SIGNAL( activated() ), this, SLOT( helpAbout() ) );

    connect(openModuleConfDialogAction, SIGNAL(activated()),
            this, SLOT(openModuleConfDialog()) );

    // set up mdi workspace
    QVBox* vb = new QVBox(this);
    vb->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    ws = new QWorkspace(vb);
    ws->setScrollBarsEnabled(TRUE);
    setCentralWidget(vb);
    
}

void MainWindow::closeWindow()
{
    MdiWindow *m = (MdiWindow *)ws->activeWindow();
    if (m) {
        m->close();
    }
}

void MainWindow::tileHorizontal()
{        
    // primitive horizontal tiling
    QWidgetList windows = ws->windowList();
    if ( !windows.count() )
        return;
    int heightForEach = ws->height() / windows.count();
    int y = 0;
    for ( int i = 0; i < int(windows.count()); ++i ) {
        QWidget *window = windows.at(i);
        if ( window->testWState( WState_Maximized) ) {
        // prevent flicker
        window->hide();
        window->showNormal();
    }
    int preferredHeight = window->minimumHeight() +
                          window->parentWidget()->baseSize().height();

    int actHeight = QMAX(heightForEach, preferredHeight);
        window->parentWidget()->setGeometry( 0, y, ws->width(), actHeight );
        y += actHeight;
    }
}

void MainWindow::closeEvent( QCloseEvent *e )
{
    QWidgetList windows = ws->windowList();
    if ( windows.count() ) {
        for ( int i = 0; i < int(windows.count()); ++i ) {
            QWidget *window = windows.at( i );
            if ( !window->close() ) {
                e->ignore();
                return;
            }
        }
    }

    QMainWindow::closeEvent( e );
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

void MainWindow::fileExit()
{
    qWarning( "MainWindow::fileExit(): Not implemented yet!" );
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

void MainWindow::helpContents()
{
	SettingsDialog *dialog = new SettingsDialog();
	dialog->show();
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
    // future: use exec() instead of show and
    //         determine exit code
}


MdiWindow* MainWindow::newLayout()
{
    MdiWindow* w = new MdiWindow(ws, 0, WDestructiveClose);
//    connect(w, SIGNAL(message(const QString&, int)),
//            statusBar(), SLOT(message(const QString&, int)));
    w->setCaption("unnamed layout");
    // show the very first window in maximized mode
    if (ws->windowList().isEmpty()) {
        w->showMaximized();
    } else {
        w->show();
    }
    return w;
}
