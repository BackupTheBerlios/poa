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
 * $Id: mainwindow.cpp,v 1.12 2003/08/21 10:46:08 garbeam Exp $
 *
 *****************************************************************************/

#include "mainwindow.h"

#include "aboutdialog.h"
#include "cpuview.h"
#include "gridcanvas.h"
#include "layoutcanvas.h"
#include "moduleconfdialog.h"
#include "settingsdialog.h"

#include <qaction.h>
#include <qapplication.h>
#include <qcombobox.h>
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

    // load toolbar icons/items
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
    QPixmap image_zoomin(ICON_PATH + "zoomin.png");
    QPixmap image_zoomnormal(ICON_PATH + "zoomnormal.png");
    QPixmap image_zoomout(ICON_PATH + "zoomout.png");

    QComboBox *zoomComboBox;

    // initialize main window
    if (!name) {
        setName("MainWindow");
    }
    resize(520, 480); 
    setCaption(trUtf8("POA"));

    // initialize status bar implicitly
    (void)statusBar();

    // set up mdi workspace
    QVBox* vb = new QVBox(this);
    vb->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    ws = new QWorkspace(vb);
    ws->setScrollBarsEnabled(TRUE);
    setCentralWidget(vb);


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

    openModuleConfAction =
        new QAction("Module configuration", image_configure,
                    "&Module configuration", 0, this,
                    "openModuleConfAction");

    drawLineAction =
        new QAction("Draw line", image_line, "&Draw line",
                    QKeySequence("Ctrl+L"), this, "drawLineAction");

    zoomInAction =
        new QAction("Zoom in", image_zoomin, "Zoom &in",
                    QKeySequence("Ctrl++"), this, "zoomInAction");

    zoomOutAction =
        new QAction("Zoom out", image_zoomout, "Zoom &out",
                    QKeySequence("Ctrl+-"), this, "zoomOutAction");

    zoomNormalAction =
        new QAction("Zoom normal", image_zoomnormal, "Zoom &normal",
                    QKeySequence("Ctrl+="), this, "zoomNormalAction");

    invokeCompilerAction =
        new QAction("Compile", image_compile, "&Compile",
                    QKeySequence("F8"), this, "invokeCompilerAction");

    invokeDownloadAction =
        new QAction("Download", image_download, "&Download", 
                    QKeySequence("F7"), this, "invokeDownloadAction");

    openSettingsAction =
        new QAction("Configure POA", "Configure &POA...", 0,
                    this, "openSettingsAction");

    tileHorizontalAction = 
        new QAction("Tile horizontal", "Tile &horizontal", 0,
                    this, "tileHorizontalAction");

    /////////////////////////////////////////////////////////////////
    // toolbars
    //

    // common
    commonToolBar = new QToolBar(tr("common toolbar"), this, DockTop); 
    fileNewAction->addTo(commonToolBar);
    fileOpenAction->addTo(commonToolBar);
    fileSaveAction->addTo(commonToolBar);
    commonToolBar->addSeparator();
    editCutAction->addTo(commonToolBar);
    editCopyAction->addTo(commonToolBar);
    editPasteAction->addTo(commonToolBar);

    // utility
    utilToolBar = new QToolBar(tr("utility toolbar"), this, DockTop); 
    openModuleConfAction->addTo(utilToolBar);
    utilToolBar->addSeparator();
    invokeCompilerAction->addTo(utilToolBar);
    invokeDownloadAction->addTo(utilToolBar);

    // draw
    drawToolBar = new QToolBar(tr("draw toolbar"), this, DockTop); 
    drawLineAction->addTo(drawToolBar);
    drawToolBar->addSeparator();
    //TODO: QStrList for combo values
    zoomComboBox = new QComboBox(FALSE, drawToolBar);
    zoomComboBox->insertItem("10 %", 0);
    zoomComboBox->insertItem("25 %", 1);
    zoomComboBox->insertItem("50 %", 2);
    zoomComboBox->insertItem("75 %", 3);
    zoomComboBox->insertItem("100 %", 4);
    zoomComboBox->insertItem("250 %", 5);
    zoomComboBox->insertItem("500 %", 6);
    zoomComboBox->insertItem("1000 %", 7);
    zoomComboBox->setCurrentItem(4);
    zoomInAction->addTo(drawToolBar);
    zoomNormalAction->addTo(drawToolBar);
    zoomOutAction->addTo(drawToolBar);

    /////////////////////////////////////////////////////////////////
    // menus

    menubar = new QMenuBar(this, "menubar");
    fileMenu = new QPopupMenu(this); 
    fileNewAction->addTo(fileMenu);
    fileOpenAction->addTo(fileMenu);
    fileSaveAction->addTo(fileMenu);
    fileSaveAsAction->addTo(fileMenu);
    fileMenu->insertSeparator();
    fileExitAction->addTo(fileMenu );
    menubar->insertItem(trUtf8("&File"), fileMenu);

    // edit
    editMenu = new QPopupMenu(this); 
    editCutAction->addTo(editMenu);
    editCopyAction->addTo(editMenu);
    editPasteAction->addTo(editMenu);
    menubar->insertItem(trUtf8("&Edit"), editMenu);

    // tools
    toolsMenu = new QPopupMenu(this); 
    menubar->insertItem(trUtf8("&Tools"), toolsMenu);
    openModuleConfAction->addTo(toolsMenu);
    toolsMenu->insertSeparator();
    invokeCompilerAction->addTo(toolsMenu);
    invokeDownloadAction->addTo(toolsMenu);

    // draw
    drawMenu = new QPopupMenu(this); 
    menubar->insertItem(trUtf8("&Draw"), drawMenu);
    drawLineAction->addTo(drawMenu);
    drawMenu->insertSeparator();
    zoomInAction->addTo(drawMenu);
    zoomNormalAction->addTo(drawMenu);
    zoomOutAction->addTo(drawMenu);

    // settings
    settingsMenu = new QPopupMenu(this);
    menubar->insertItem(tr("&Settings"), settingsMenu);
    openSettingsAction->addTo(settingsMenu);

    // window
    windowMenu = new QPopupMenu(this);
    menubar->insertItem(tr("&Window"), windowMenu);
    tileHorizontalAction->addTo(windowMenu);

    // help
    helpMenu = new QPopupMenu(this); 
    helpContentsAction->addTo(helpMenu);
    helpMenu->insertSeparator();
    helpAboutAction->addTo(helpMenu);
    menubar->insertItem(trUtf8("&Help"), helpMenu );

    /////////////////////////////////////////////////////////////////
    // signals and slots connections
    connect(fileNewAction, SIGNAL(activated()), this, SLOT(newDoc()));
    connect(fileOpenAction, SIGNAL(activated()), this, SLOT(fileOpen()));
    connect(fileSaveAction, SIGNAL(activated()), this, SLOT(fileSave()));
    connect(fileSaveAsAction, SIGNAL(activated()), this, SLOT(fileSaveAs()));
    connect(fileExitAction, SIGNAL(activated()),
            qApp, SLOT(closeAllWindows()));
    connect(editCutAction, SIGNAL(activated()), this, SLOT(editCut()));
    connect(editCopyAction, SIGNAL(activated()), this, SLOT(editCopy()));
    connect(editPasteAction, SIGNAL(activated()), this, SLOT(editPaste()));
    connect(helpContentsAction, SIGNAL(activated()),
            this, SLOT(helpContents()));
    connect(helpAboutAction, SIGNAL(activated()), this, SLOT(helpAbout()));
    connect(openModuleConfAction, SIGNAL(activated()),
            this, SLOT(openModuleConf()));
    connect(openSettingsAction, SIGNAL(activated()),
            this, SLOT(openSettings()));
    connect(tileHorizontalAction, SIGNAL(activated()),
            this, SLOT(tileHorizontal()));
}

/**
 * Destroys the object and frees any allocated resources
 */
MainWindow::~MainWindow()
{
    // no need to delete child widgets, Qt does it all for us
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
    qWarning( "MainWindow::helpContents(): Not implemented yet!" );
}

void MainWindow::helpAbout()
{
    AboutDialog *dialog = new AboutDialog();
    dialog->show();
}

void MainWindow::openModuleConf()
{
    ModuleConfDialog *dialog = new ModuleConfDialog();
    dialog->show();
    // future: use exec() instead of show and
    //         determine exit code
}

void MainWindow::openSettings()
{
    SettingsDialog *dialog = new SettingsDialog();
    dialog->show();
}

MdiWindow* MainWindow::newDoc()
{
//    LayoutCanvas *doc = new LayoutCanvas();
    GridCanvas *doc = new GridCanvas(100, 400);
//    doc->resize(400, 400);

    MdiWindow* w = new MdiWindow(doc, ws, 0, WDestructiveClose);
    w->setCaption("unnamed layout");
    w->setIcon(QPixmap(ICON_PATH + "document.xpm"));
    w->resize(w->sizeHint());
    // show the very first window in maximized mode
    if (ws->windowList().isEmpty()) {
        w->showMaximized();
    } else {
        w->show();
    }

    // FIX: remove: create dummy items
    QCanvasPolygonalItem *i
        = new QCanvasRectangle( rand()%doc->width(),rand()%doc->height(),
                                doc->width()/5,doc->width()/5,doc);
    int z = rand()%256;
    i->setBrush( QColor(z,z,z) );
    i->setPen( QPen(QColor(rand()%32*8,rand()%32*8,rand()%32*8), 6) );
    i->setZ(z);
    i->show();

    doc->update();

    return w;
}
