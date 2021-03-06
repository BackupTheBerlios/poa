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
 * $Id: mainwindow.cpp,v 1.111 2004/02/15 03:56:24 kilgus Exp $
 *
 *****************************************************************************/

#include "mainwindow.h"

#include "aboutdialog.h"
#include "blockconfdialog.h"
#include "blockview.h"
#include "canvasview.h"
#include "cpumodel.h"
#include "connectorrouter.h"
#include "connectorviewlist.h"
#include "connectorviewsegment.h"
#include "copyable.h"
#include "deployprojectwizard.h"
#include "dijkstrarouter.h"
#include "directrouter.h"
#include "gridcanvas.h"
#include "librarywindow.h"
#include "mdiwindow.h"
#include "modelfactory.h"
#include "moveable.h"
#include "muxconfdialog.h"
#include "muxmodel.h"
#include "poa.h"
#include "printmanager.h"
#include "project.h"
#include "project.h"
#include "removeable.h"
#include "scheduledialog.h"
#include "settings.h"
#include "settingsdialog.h"
#include "util.h"

#include <qaction.h>
#include <qapplication.h>
#include <qcanvas.h>
#include <qclipboard.h>
#include <qcombobox.h>
#include <qdragobject.h>
#include <qdom.h>
#include <qdir.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qimage.h>
#include <qinputdialog.h>
#include <qlayout.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qpixmap.h>
#include <qpopupmenu.h>
#include <qtoolbar.h>
#include <qtooltip.h>
#include <qvariant.h>
#include <qvbox.h>
#include <qwhatsthis.h>

#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>

#include "poaexception.h"

const uint MainWindow::DEFAULT_ZOOM_LEVEL = 4;


/**
 * Constructs the main window.
 *
 * The main window is only instanciated once.
 */
MainWindow::MainWindow(QWidget *parent, const char *name, WFlags fl)
    : QMainWindow(parent, name, fl)
{
    // initialize main window
    setCaption(tr("POA"));
    setIcon(QPixmap(Util::findIcon("poa.png")));
    setDockMenuEnabled(false);

    // restore window settings
    Settings *s = Settings::instance();

    move(s->getNum("MainWindow/X", 0), s->getNum("MainWindow/Y", 0));
    resize(s->getNum("MainWindow/Width", 640),
           s->getNum("MainWindow/Height", 480));

    // initialize status bar implicitly
    (void)statusBar();

    // initialize library dock window
    libraryWindow_ = new LibraryWindow(QDockWindow::InDock, this);
    libraryWindow_->setFixedExtentWidth(s->getNum("LibraryWindow/Width", 100));
    moveDockWindow(libraryWindow_, Qt::DockLeft);

    // initialize mdi workspace
    QVBox* vb = new QVBox(this);
    vb->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    ws = new QWorkspace(vb);
    ws->setScrollBarsEnabled(TRUE);
    setCentralWidget(vb);

    initializeActions();
    initializeToolbars();
    initializeMenu();

    connectActions();
    connect(Settings::instance(), SIGNAL(recentProjectsChanged()),
            this, SLOT(updateRecentProjectsMenu()));

    // disable most actions
    windowActivated(0);
}

void MainWindow::initializeActions()
{
    QPixmap image_compile(Util::findIcon("compile.png"));
    QPixmap image_configure(Util::findIcon("configure.png"));
    QPixmap image_contents(Util::findIcon("contents.png"));
    QPixmap image_download(Util::findIcon("dowload.png"));
    QPixmap image_deploy(Util::findIcon("wizard.png"));
    QPixmap image_editcopy(Util::findIcon("editcopy.png"));
    QPixmap image_editcut(Util::findIcon("editcut.png"));
    QPixmap image_editdelete(Util::findIcon("editdelete.png"));
    QPixmap image_editpaste(Util::findIcon("editpaste.png"));
    QPixmap image_filesave(Util::findIcon("filesave.png"));
    QPixmap image_help(Util::findIcon("help.png"));
    QPixmap image_scheduling(Util::findIcon("scheduling.png"));
    QPixmap image_zoomin(Util::findIcon("zoomin.png"));
    QPixmap image_zoomnormal(Util::findIcon("zoomnormal.png"));
    QPixmap image_zoomout(Util::findIcon("zoomout.png"));

    defaultRouteAction_ = new QAction
        (tr("Default Router"), QPixmap(Util::findIcon("defaultroute.png")),
         tr("&Default Router"), 0, this, "defaultRouteAction");
    fileNewAction = new QAction
        (tr("New"), QPixmap(Util::findIcon("filenew.png")),
         tr("&New"), QKeySequence("Ctrl+N"), this, "fileNewAction");
    fileOpenAction = new QAction
        (tr("Open..."), QPixmap(Util::findIcon("fileopen.png")),
         tr("&Open..."), QKeySequence("Ctrl+O"), this,
         "fileOpenAction");
    fileSaveAction =
        new QAction(tr("Save"), image_filesave, tr("&Save"),
                    QKeySequence("Ctrl+S"), this, "fileSaveAction");
    fileSaveAsAction =
        new QAction(tr("Save As..."), tr("Save &As..."), 0, this, 
		    "fileSaveAsAction");
    filePrintAction = new QAction
        (tr("Print"), QPixmap(Util::findIcon("fileprint.png")),
         tr("&Print"), QKeySequence("Ctrl+P"), this, "filePrintAction");
    fileExitAction =
        new QAction(tr("Exit"), tr("E&xit"), 0, this, "fileExitAction");
    editCutAction =
        new QAction(tr("Cut"), image_editcut, tr("C&ut"),
                    QKeySequence("Ctrl+X"), this, "editCutAction");
    editCopyAction =
        new QAction(tr("Copy"), image_editcopy, tr("&Copy"),
                    QKeySequence("Ctrl+C"), this, "editCopyAction");
    editPasteAction =
        new QAction(tr("Paste"), image_editpaste, tr("&Paste"),
                    QKeySequence("Ctrl+V"), this, "editPasteAction" );
    editRemoveAction =
        new QAction(tr("Remove"), image_editdelete, tr("&Remove"),
                    QKeySequence("Del"), this, "removeAction" );

    editModeActionGroup_ = new QActionGroup(this, "editMode", true);
    editModeAnnotateAction_
        = new QAction(tr("Annotate"), QPixmap(Util::findIcon("editclear.png")),
                      tr("&Annotate"), 0, editModeActionGroup_, 
		      "editModeAnnotate", true);
    editModeDefaultAction_
        = new QAction(tr("Default"), 
		      QPixmap(Util::findIcon("editdefault.png")),
                      tr("&Default"),0, editModeActionGroup_, 
		      "editModeDefault", true);
    editModeDefaultAction_->setOn(true);
    helpContentsAction =
        new QAction(tr("Contents"), image_contents, tr("&Contents..."),
                    QKeySequence("F1"), this, "helpContentsAction");
    helpAboutAction =
        new QAction(tr("About"), image_help, tr("&About..."), 0,
                    this, "helpAboutAction" );
    openBlockConfAction =
        new QAction(tr("Configuration"), image_configure,
                    tr("&Configuration"), 0, this,
                    "openBlockConfAction");
    zoomInAction =
        new QAction(tr("Zoom in"), image_zoomin, tr("Zoom &in"),
                    QKeySequence("Ctrl++"), this, "zoomInAction");
    zoomOutAction =
        new QAction(tr("Zoom out"), image_zoomout, tr("Zoom &out"),
                    QKeySequence("Ctrl+-"), this, "zoomOutAction");
    zoomNormalAction =
        new QAction(tr("Zoom normal"), image_zoomnormal, tr("Zoom &normal"),
                    QKeySequence("Ctrl+="), this, "zoomNormalAction");
    invokeCompilerAction =
        new QAction(tr("Compile"), image_compile, tr("&Compile"),
                    QKeySequence("F8"), this, "invokeCompilerAction");
    invokeDownloadAction =
        new QAction(tr("Download"), image_download, tr("&Download"),
                    QKeySequence("F7"), this, "invokeDownloadAction");
    invokeSchedulingAction =
        new QAction(tr("Scheduling"), image_scheduling, tr("&Scheduling"),
            QKeySequence("F6"), this, "invokeSchedulingAction");
    invokeDeployAction =
        new QAction(tr("Deploy Project"), image_deploy, tr("Deploy &Project"),
            QKeySequence("F5"), this, "invokeDeployAction");
    openSettingsAction =
        new QAction(tr("Configure POA"), tr("Configure &POA..."), 0,
                    this, "openSettingsAction");
    tileHorizontalAction =
        new QAction(tr("Tile Horizontal"), tr("Tile &Horizontal"), 0,
                    this, "tileHorizontalAction");
    tileAction =
        new QAction(tr("Tile"), tr("&Tile"), 0, this, "tileAction");
    cascadeAction =
        new QAction(tr("Cascade"), tr("&Cascade"), 0, this, "cascadeAction");
    settingsShowGridAction_ =
        new QAction(tr("Show Grid"), tr("&Show Grid"),
                    QKeySequence("Ctrl+G"), this, "editSettingsShowGrid",
                    true);
    settingsShowGridAction_->setOn(Settings::instance()->showGrid());
    smartRouteAction_ = new QAction
        (tr("Smart Router"), QPixmap(Util::findIcon("smartroute.png")),
         tr("&Smart Router"), 0, this, "smartRouteAction");
    saveToLibraryAction_ =
        new QAction(tr("Save To Library"), image_filesave, 
		    tr("&Save To Library"),
                    QKeySequence("Ctrl+L"), this, "saveToLibraryAction");
}

void MainWindow::initializeToolbars()
{
    // common
    commonToolBar = new QToolBar(tr("common toolbar"), this, DockTop);
    fileNewAction->addTo(commonToolBar);
    fileOpenAction->addTo(commonToolBar);
    fileSaveAction->addTo(commonToolBar);
    filePrintAction->addTo(commonToolBar);
    commonToolBar->addSeparator();
    editCutAction->addTo(commonToolBar);
    editCopyAction->addTo(commonToolBar);
    editPasteAction->addTo(commonToolBar);
    commonToolBar->addSeparator();
    editRemoveAction->addTo(commonToolBar);

    // edit
    QToolBar *editToolBar = new QToolBar(tr("edit toolbar"), this, DockTop);
    editModeDefaultAction_->addTo(editToolBar);
    editModeAnnotateAction_->addTo(editToolBar);

    // utility
    utilToolBar = new QToolBar(tr("utility toolbar"), this, DockTop);
    openBlockConfAction->addTo(utilToolBar);
    utilToolBar->addSeparator();
    invokeSchedulingAction->addTo(utilToolBar);
    //invokeCompilerAction->addTo(utilToolBar);
    //invokeDownloadAction->addTo(utilToolBar);
    invokeDeployAction->addTo(utilToolBar);

    // view
    viewToolBar = new QToolBar(tr("view toolbar"), this, DockTop);
    zoomComboBox = new QComboBox(false, viewToolBar);
    zoomComboBox->insertItem("10%", 0);
    zoomComboBox->insertItem("25%", 1);
    zoomComboBox->insertItem("50%", 2);
    zoomComboBox->insertItem("75%", 3);
    zoomComboBox->insertItem("100%", DEFAULT_ZOOM_LEVEL);
    zoomComboBox->insertItem("250%", 5);
    zoomComboBox->insertItem("500%", 6);
    zoomComboBox->setCurrentItem(DEFAULT_ZOOM_LEVEL);
    zoomInAction->addTo(viewToolBar);
    zoomNormalAction->addTo(viewToolBar);
    zoomOutAction->addTo(viewToolBar);
}

void MainWindow::initializeMenu()
{
    QPopupMenu *fileMenu;
    QPopupMenu *editMenu;
    QPopupMenu *viewMenu;
    QPopupMenu *toolsMenu;
    QPopupMenu *settingsMenu;
    QPopupMenu *helpMenu;

    fileMenu = new QPopupMenu(this);
    menuBar()->insertItem(tr("&Project"), fileMenu);
    fileNewAction->addTo(fileMenu);
    fileOpenAction->addTo(fileMenu);
    fileMenu->insertSeparator();
    fileSaveAction->addTo(fileMenu);
    fileSaveAsAction->addTo(fileMenu);
    fileMenu->insertSeparator();
    filePrintAction->addTo(fileMenu);
    fileMenu->insertSeparator();
    recentProjectsMenu = new QPopupMenu(fileMenu);
    fileMenu->insertItem(tr("&Recent"), recentProjectsMenu);
    updateRecentProjectsMenu();
    fileMenu->insertSeparator();
    fileExitAction->addTo(fileMenu );

    // edit
    editMenu = new QPopupMenu(this);
    menuBar()->insertItem(tr("&Edit"), editMenu);
    editCutAction->addTo(editMenu);
    editCopyAction->addTo(editMenu);
    editPasteAction->addTo(editMenu);

    // view
    viewMenu = new QPopupMenu(this);
    menuBar()->insertItem(tr("&View"), viewMenu);
    zoomInAction->addTo(viewMenu);
    zoomNormalAction->addTo(viewMenu);
    zoomOutAction->addTo(viewMenu);

    // tools
    toolsMenu = new QPopupMenu(this);
    menuBar()->insertItem(tr("&Tools"), toolsMenu);
    openBlockConfAction->addTo(toolsMenu);
    QPopupMenu *routeMenu = new QPopupMenu(toolsMenu);
    toolsMenu->insertItem(tr("&Route"), routeMenu);
    defaultRouteAction_->addTo(routeMenu);
    smartRouteAction_->addTo(routeMenu);
    toolsMenu->insertSeparator();
    invokeSchedulingAction->addTo(toolsMenu);
    //invokeCompilerAction->addTo(toolsMenu);
    //invokeDownloadAction->addTo(toolsMenu);
    invokeDeployAction->addTo(toolsMenu);

    // settings
    settingsMenu = new QPopupMenu(this);
    menuBar()->insertItem(tr("&Settings"), settingsMenu);
    settingsShowGridAction_->addTo(settingsMenu);
    settingsMenu->insertSeparator();
    openSettingsAction->addTo(settingsMenu);

    // window
    windowsMenu_ = new QPopupMenu(this);
    menuBar()->insertItem(tr("&Windows"), windowsMenu_);
    connect(windowsMenu_, SIGNAL(aboutToShow()),
            this, SLOT(windowsMenuAboutToShow()));

    // help
    helpMenu = new QPopupMenu(this);
    menuBar()->insertItem(tr("&Help"), helpMenu );
    helpContentsAction->addTo(helpMenu);
    helpMenu->insertSeparator();
    helpAboutAction->addTo(helpMenu);
}

void MainWindow::connectActions()
{
    connect(fileNewAction, SIGNAL(activated()), this, SLOT(fileNew()));
    connect(fileOpenAction, SIGNAL(activated()), this, SLOT(fileOpen()));
    connect(fileSaveAction, SIGNAL(activated()), this, SLOT(fileSave()));
    connect(fileSaveAsAction, SIGNAL(activated()), this, SLOT(fileSaveAs()));
    connect(filePrintAction, SIGNAL(activated()), this, SLOT(filePrint()));
    connect(fileExitAction, SIGNAL(activated()),
            qApp, SLOT(closeAllWindows()));
    connect(defaultRouteAction_, SIGNAL(activated()),
            this, SLOT(defaultRoute()));
    connect(editCutAction, SIGNAL(activated()), this, SLOT(editCut()));
    connect(editCopyAction, SIGNAL(activated()), this, SLOT(editCopy()));
    connect(editPasteAction, SIGNAL(activated()), this, SLOT(editPaste()));
    connect(editRemoveAction, SIGNAL(activated()), this, SLOT(editRemove()));
    connect(editModeActionGroup_, SIGNAL(selected(QAction *)),
            this, SLOT(setEditMode(QAction *)));
    connect(QApplication::clipboard(), SIGNAL(dataChanged()),
            this, SLOT(checkClipboardContent()));
    connect(helpContentsAction, SIGNAL(activated()),
            this, SLOT(helpContents()));
    connect(helpAboutAction, SIGNAL(activated()), this, SLOT(helpAbout()));
    connect(openBlockConfAction, SIGNAL(activated()),
            this, SLOT(openBlockConf()));
    connect(openSettingsAction, SIGNAL(activated()),
            this, SLOT(openSettings()));
    connect(settingsShowGridAction_, SIGNAL(toggled(bool)),
            Settings::instance(), SLOT(setShowGrid(bool)));
    connect(Settings::instance(), SIGNAL(showGridChanged(bool)),
            settingsShowGridAction_, SLOT(setOn(bool)));
    connect(invokeSchedulingAction, SIGNAL(activated()), this,
            SLOT(openScheduling()));
    connect(invokeDeployAction, SIGNAL(activated()), this,
            SLOT(openDeployWizard()));
    connect(tileHorizontalAction, SIGNAL(activated()),
            this, SLOT(tileHorizontal()));
    connect(tileAction, SIGNAL(activated()), ws, SLOT(tile()));
    connect(cascadeAction, SIGNAL(activated()), ws, SLOT(cascade()));
    connect(ws, SIGNAL(windowActivated(QWidget*)),
            this, SLOT(windowActivated(QWidget*)));
    connect(saveToLibraryAction_, SIGNAL(activated()),
            this, SLOT(saveToLibrary()));
    connect(smartRouteAction_, SIGNAL(activated()), this, SLOT(smartRoute()));
    connect(zoomComboBox, SIGNAL(activated(const QString&)),
            this, SLOT(zoomTo(const QString&)));
    connect(zoomNormalAction, SIGNAL(activated()), this, SLOT(zoomNormal()));
    connect(zoomOutAction, SIGNAL(activated()), this, SLOT(zoomOut()));
    connect(zoomInAction, SIGNAL(activated()), this, SLOT(zoomIn()));
}

/**
 * Destroys the object and frees any allocated resources
 */
MainWindow::~MainWindow()
{
    // no need to delete child widgets, Qt does it all for us
}

CanvasView *MainWindow::activeView() const
{
    MdiWindow *m = dynamic_cast<MdiWindow *>(ws->activeWindow());
    return (m != 0) ? m->view() : 0;
}

MdiWindow *MainWindow::activeWindow() const
{
    MdiWindow *w = dynamic_cast<MdiWindow *>(ws->activeWindow());
    return (w != 0) ? w : 0;
}

void MainWindow::closeWindow()
{
    MdiWindow *m = activeWindow();
    if (m != 0) {
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

QAction *MainWindow::blockConfAction()
{
    return openBlockConfAction;
}

void MainWindow::checkClipboardContent()
{
    QMimeSource *data = QApplication::clipboard()->data();
    editPasteAction->setEnabled(data != 0 && data->provides("text/xml"));
}

bool MainWindow::closeAll()
{
    QWidgetList windows = ws->windowList();
    QWidgetListIt it(windows);
    QWidget *window;
    while ((window = it.current()) != 0 ) {
        ++it;
        if (!window->close()) {
            return false;
        }
    }
    return TRUE;
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if (!closeAll()) {
        e->ignore();
        return;
    }

    libraryWindow_->save();

    // save settings
    Settings *s = Settings::instance();
    s->set("MainWindow/X", x());
    s->set("MainWindow/Y", y());
    s->set("MainWindow/Width", width());
    s->set("MainWindow/Height", height());

    s->set("LibraryWindow/Width", libraryWindow_->width());

    e->accept();
}

QAction *MainWindow::copyAction()
{
    return editCopyAction;
}

QAction *MainWindow::cutAction()
{
    return editCutAction;
}

QAction *MainWindow::removeAction()
{
    return editRemoveAction;
}

void MainWindow::fileNew()
{
    QFileDialog* fd = new QFileDialog( this, tr("file dialog"), TRUE );
    //fd->setMode( QFileDialog::AnyFile );
    fd->setMode(QFileDialog::Directory);
    fd->setFilter(tr("POA project (project.xml)"));
    fd->setSelection("project.xml");
    fd->setCaption(tr("Select/Create project directory"));

    QString fileName;
    if ( fd->exec() == QDialog::Accepted ) {
        fileName = fd->selectedFile();
        delete fd;
    }
    else {
        delete fd;
        return;
    }

    QDir projDir(fileName);
    //projDir.cdUp();

    // if project is new, create empty project and save it
    if (QFileInfo(projDir, "project.xml").exists()) {
        // project already exists
        QMessageBox mb( "POA",
                        tr("Creating a new project at this location will overwrite an existing project.\n"
                        "Do you really want to continue?"),
                        QMessageBox::Information,
                        QMessageBox::Yes | QMessageBox::Default,
                        QMessageBox::No,
                        QMessageBox::Cancel | QMessageBox::Escape );
        mb.setButtonText( QMessageBox::Yes, tr("&Yes") );
        mb.setButtonText( QMessageBox::No, tr("&No, load existing project") );

        switch( mb.exec() ) {
        case QMessageBox::Yes:
            createNewProject(projDir.path());
            break;
        case QMessageBox::No:
            break;
        case QMessageBox::Cancel:
            return;
            break;
        }
    } else {
        // open new project
        createNewProject(projDir.path());
    }
    openProject(projDir.path());
}

void MainWindow::createNewProject(QString path)
{
    // create new project
    Project *prj = new Project(path);
    prj->newCanvas("1");
    prj->save();
    delete prj;
}

void MainWindow::defaultRoute()
{
    routeSelected(new DirectRouter());
}

QAction *MainWindow::defaultRouteAction()
{
    return defaultRouteAction_;
}

void MainWindow::editCut()
{
    editCopy();
    editRemove();
}

void MainWindow::editCopy()
{
    CanvasView *view = activeView();
    if (view != 0) {
        QCanvasItemList items = view->selectedItems();
        if (!items.isEmpty()) {
            QDomDocument doc;
            QDomElement root = doc.createElement("model");
            doc.appendChild(root);
            for (QCanvasItemList::iterator current = items.begin();
                 current != items.end(); ++current) {
                Copyable *item = dynamic_cast<Copyable *>(*current);
                if (item != 0) {
                    Q_ASSERT(item->model() != 0);
                    root.appendChild
                        (item->model()->serializeCopy(&doc, view->project()));
                }
            }

            if (root.hasChildNodes()) {
                QStoredDrag *dragItem = new QStoredDrag("text/xml", 0);
                dragItem->setEncodedData(doc.toCString());
                QApplication::clipboard()->setData(dragItem);
            }
        }
    }
}

void MainWindow::editPaste()
{
    CanvasView *view = activeView();
    QMimeSource *source = QApplication::clipboard()->data();
    if (source != 0 && source->provides("text/xml") && view != 0) {
        QByteArray data = source->encodedData("text/xml");
        if (data) {
            QDomDocument doc;
            if (doc.setContent(QString(data))) {
                QValueList<AbstractModel *> l = ModelFactory::generate(doc);
                for (QValueList<AbstractModel *>::Iterator it = l.begin();
                     it != l.end(); ++it) {

                    // virginize item
                    ModelFactory::clearProperties(*it);

                    view->project()->addBlock(*it);
                    view->gridCanvas()->addView(*it);
                }
            }
        }
    }
}

void MainWindow::editRemove()
{
    CanvasView *view = activeView();
    if (view != 0) {
        QCanvasItemList items = view->selectedItems();
        view->deselectAll();
        if (!items.isEmpty()) {
            for (QCanvasItemList::iterator current = items.begin();
                 current != items.end(); ++current) {

                Removeable *item = dynamic_cast<Removeable *>(*current);
                if (item != 0) {
                    item->remove(view->project());
                }
            }
        }
        view->canvas()->update();
    }
}

void MainWindow::fileOpen()
{
    QFileDialog* fd = new QFileDialog( this, tr("file dialog"), TRUE );
    fd->setMode( QFileDialog::ExistingFile );
    fd->setFilter(tr("POA project (project.xml)"));
    fd->setSelection("project.xml");
    fd->setCaption(tr("Select/Create project directory"));

    QString fileName;
    if ( fd->exec() == QDialog::Accepted ) {
        fileName = fd->selectedFile();
        delete fd;
    }
    else {
        delete fd;
        return;
    }

    QDir projDir(fileName);
    projDir.cdUp();

    openProject(projDir.path());
}

void MainWindow::filePrint()
{
    if (activeView() != 0) {
        PrintManager printer(activeView()->project()->name());
        printer.print(activeView()->canvas());
    }
}

void MainWindow::fileSave()
{
    CanvasView *view = activeView();
    if (view != 0) {
        try {
            view->project()->save();
        }
        catch (const PoaException e) {
            QMessageBox::warning(this, tr("POA Error"), e.message());
        }
    }
}

void MainWindow::fileSaveAs()
{
    MdiWindow *window = activeWindow();
    if (window != 0) {
        QFileDialog* fd = new QFileDialog( this, tr("file dialog"), TRUE );
        fd->setMode(QFileDialog::Directory);
        fd->setFilter(tr("POA project (project.xml)"));
        fd->setSelection("project.xml");
        fd->setCaption(tr("Save As - Select/Create project directory"));

        QString fileName;
        if ( fd->exec() == QDialog::Accepted ) {
            fileName = fd->selectedFile();
            delete fd;
        }
        else {
            delete fd;
            return;
        }

        QDir projDir(fileName);

        // if project dir already exists, ask.
        if ( QFileInfo(projDir, "project.xml").exists() &&
             QMessageBox::warning(
                                   this,
                                   tr("POA - Overwrite File?"),
                                   tr("A project directory called %1 already"
                                      " exists.\nDo you want to overwrite it?")
                                   .arg( projDir.path() ),
                                   tr("&Yes"), tr("&No"),
                                   QString::null, 0, 1 )
             ) {
            return;
        }

        // if project is new, create empty project and save it
        /*        if (QFileInfo(projDir, "project.xml").exists()) {
            QMessageBox mb( "POA",
                            "The choosen project directory already exists.\n" \
                            "Do you want to replace it?",
                            QMessageBox::Warning,
                            QMessageBox::Yes | QMessageBox::Default,
                            QMessageBox::No | QMessageBox::Escape);

            if (mb.exec() == QMessageBox::Yes) {
                return;
            }
            }*/
        try {
            window->view()->project()->saveAs(projDir.path());
        }
        catch (const PoaException e) {
            QMessageBox::warning(this,
                                 tr("File error"),
                                 e.message());
        }

        // update caption
        window->setModified(false);
    }

   /*    QString filename
        = QFileDialog::getSaveFileName(QString::null, QString::null, this);
    if (!filename.isEmpty()) {
        // FIX: check if file already exists
        project_->setFilename(filename);
        saveProject();
        Settings::instance()->addToRecentProjects(filename);
        }*/
}

void MainWindow::fileExit()
{
    qWarning( "MainWindow::fileExit(): Not implemented yet!" );
}

void MainWindow::helpContents()
{
    qWarning( "MainWindow::helpContents(): Not implemented yet!" );
}

void MainWindow::helpAbout()
{
    AboutDialog *dialog = new AboutDialog(this);
    dialog->exec();
    delete dialog;
}

MainWindow *MainWindow::instance()
{
    return dynamic_cast<MainWindow *>(qApp->mainWidget());
}

void MainWindow::openBlockConf()
{
    CanvasView *view = activeView();
    if (view != 0) {
        AbstractModel *model = selectedModel();
        if (model != 0) {
            if (INSTANCEOF(model, MuxModel)) {
                MuxConfDialog *dialog = new MuxConfDialog((MuxModel *)model);
                dialog->show();
                if (dialog->exec() == QDialog::Accepted) {
                    view->project()->setModified(true);
                }
                delete dialog;
            }
            else if (INSTANCEOF(model, BlockModel)) {
                BlockConfDialog *dialog
                    = new BlockConfDialog((BlockModel *)model, view->project());
                if (dialog->exec() == QDialog::Accepted) {
                    view->project()->setModified(true);
                }
                delete dialog;
            }
        }
    }
}

void MainWindow::openProject(QString path)
{
    Project *project_ = new Project(path);

    try {
        project_->open();

        GridCanvas *canvas = project_->canvasList()->getFirst();
        CanvasView *view = new CanvasView(project_, canvas);

        MdiWindow *w = new MdiWindow(view, ws, 0, WDestructiveClose);
        if (ws->windowList().count() == 0) {
            w->showMaximized();
        }
        else {
            w->show();
        }

        // connect to signals
        connect(view, SIGNAL(selectionChanged(QCanvasItem *)), this,
                SLOT(selectionChanged(QCanvasItem *)));

        connect(project_, SIGNAL(modified(bool)), w, SLOT(setModified(bool)));
        connect(project_, SIGNAL(modified(bool)),
                fileSaveAction, SLOT(setEnabled(bool)));
    }
    catch (const PoaException e){
        delete project_;
        project_ = 0;
        QMessageBox::warning
            (this, tr("File error"),
             e.message());
    }
}

void MainWindow::openRecentProject(int i)
{
    QStringList list = Settings::instance()->getStrings("RecentProjects");
    if (i < (int)list.count()) {
        openProject(list[i]);
    }
    else {
        qWarning("Index out of range.");
    }
}

void MainWindow::openScheduling()
{
    CanvasView *view = activeView();
    if (view != 0) {
        ScheduleDialog *dialog
            = new ScheduleDialog(activeView()->gridCanvas(), activeView()->project(), this);
        dialog->exec();
        delete dialog;
    }
}


void MainWindow::openSettings()
{
    SettingsDialog *dialog = new SettingsDialog(0, this);
    dialog->show();
}

void MainWindow::openDeployWizard()
{
    CanvasView *view = activeView();
    if (view != 0) {
        DeployProjectWizard *wizard
            = new DeployProjectWizard(view->project(), this);
        wizard->exec();
        delete wizard;
    }
}

QAction *MainWindow::pasteAction()
{
    return editPasteAction;
}

void MainWindow::raiseWindow(int id)
{
    QWidget* w = ws->windowList().at(id);
    if (w) {
        w->showNormal();
        w->setFocus();
    }
}

void MainWindow::routeSelected(ConnectorRouter *router)
{
    CanvasView *view = activeView();
    if (view != 0) {
        QCanvasItemList items = view->selectedItems();

        router->route(items);
        view->project()->setModified(true);
        view->canvas()->update();
    }
    delete router;
}

void MainWindow::updateRecentProjectsMenu()
{
    recentProjectsMenu->clear();

    QStringList list = Settings::instance()->getStrings("RecentProjects");
    QStringList::Iterator it = list.begin();
    uint i = 0;
    while(it != list.end() && i < MAX_RECENT_PROJECTS) {
        recentProjectsMenu->insertItem(*it, this, SLOT(openRecentProject(int)),
                                       0, i);
        ++i;
        ++it;
    }
    recentProjectsMenu->setEnabled(i > 0);
}

QAction *MainWindow::saveToLibraryAction()
{
    return saveToLibraryAction_;
}

void MainWindow::saveToLibrary()
{
    CanvasView *view = activeView();
    if (view != 0) {
        QCanvasItemList items = view->selectedItems();
        if (!items.isEmpty()) {
            QDomDocument doc;
            QDomElement root = doc.createElement("model");
            doc.appendChild(root);

            bool ok;
            QStringList typeNames = libraryWindow_->types();
            int index = (selectedModel() != 0)
                ? QMAX(0, typeNames.findIndex(selectedModel()->type()))
                : 0;
            QString type = QInputDialog::getItem
                ("POA", tr("Select a type"), typeNames, index, true, &ok, this);
            if (!ok) {
                return;
            }

            // serialize
            for (QCanvasItemList::iterator current = items.begin();
                 current != items.end(); ++current) {
                Copyable *item = dynamic_cast<Copyable *>(*current);
                if (item != 0) {
                    Q_ASSERT(item->model() != 0);
                    root.appendChild(item->model()->serializeCopy
                                     (&doc, view->project()));
                }
            }

            // deserialize
            QValueList<AbstractModel *> l = ModelFactory::generate(doc);
            for (QValueList<AbstractModel *>::Iterator it = l.begin();
                 it != l.end(); ++it) {

                (*it)->setType(type);
                libraryWindow_->add(*it);
            }
        }
    }
}

QCanvasItem *MainWindow::selectedItem()
{
    CanvasView *view = activeView();
    if (view != 0) {
        QCanvasItemList items = view->selectedItems();
        if (!items.isEmpty()) {
            return items.first();
        }
    }
    return 0;
}

AbstractModel *MainWindow::selectedModel()
{
    CanvasView *view = activeView();
    if (view != 0) {
        QCanvasItemList items = view->selectedItems();
        if (!items.isEmpty()) {
            for (QCanvasItemList::iterator current = items.begin();
                 current != items.end(); ++current) {

                Copyable *item = dynamic_cast<Copyable *>(*current);
                if (item != 0) {
                    return item->model();
                }
            }
        }
    }
    return 0;
}

QAction *MainWindow::showGridAction()
{
    return settingsShowGridAction_;
}

void MainWindow::smartRoute()
{
    routeSelected(new DijkstraRouter());
}

QAction *MainWindow::smartRouteAction()
{
    return smartRouteAction_;
}

void MainWindow::windowActivated(QWidget *window)
{
    MdiWindow *m = dynamic_cast<MdiWindow *>(window);
    if (m != 0) {
        checkClipboardContent();
        invokeDeployAction->setEnabled(true);
        fileSaveAction->setEnabled(m->view()->project()->isModified());
        invokeSchedulingAction->setEnabled(true);
        zoomComboBox->setEnabled(true);
        zoomComboBox->setCurrentText
            (QString::number(m->zoomLevel() * 100.0) + "%");
        invokeSchedulingAction->setEnabled(true);
        fileSaveAsAction->setEnabled(true);
        filePrintAction->setEnabled(true);
        editModeActionGroup_->setEnabled(true);
        CanvasView::EditMode mode = m->view()->editMode();
        editModeAnnotateAction_->setOn(mode == CanvasView::Annotate);
        editModeDefaultAction_->setOn(mode == CanvasView::Default);
    }
    else {
        editCutAction->setEnabled(false);
        editCopyAction->setEnabled(false);
        editPasteAction->setEnabled(false);
        editRemoveAction->setEnabled(false);
        filePrintAction->setEnabled(false);
        invokeDeployAction->setEnabled(false);
        invokeSchedulingAction->setEnabled(false);
        openBlockConfAction->setEnabled(false);
        fileSaveAction->setEnabled(false);
        zoomComboBox->setEnabled(false);
        fileSaveAsAction->setEnabled(false);
        editModeActionGroup_->setEnabled(false);
    }

    // update popup menu actions
    selectionChanged(selectedItem());
}

void MainWindow::selectionChanged(QCanvasItem *item)
{
    editCutAction->setEnabled(INSTANCEOF(item, Copyable));
    editCopyAction->setEnabled(INSTANCEOF(item, Copyable));
    openBlockConfAction->setEnabled(INSTANCEOF(item, BlockView));
    invokeCompilerAction->setEnabled(INSTANCEOF(item, BlockView));
    editRemoveAction->setEnabled(INSTANCEOF(item, Removeable));
    saveToLibraryAction_->setEnabled(INSTANCEOF(item, BlockView));
}

void MainWindow::setEditMode(QAction *action)
{
    CanvasView *view = activeView();
    if (view != 0) {
        if (action == editModeAnnotateAction_) {
            view->setEditMode(CanvasView::Annotate);
        }
        else {
            view->setEditMode(CanvasView::Default);
        }
    }
}

void MainWindow::windowsMenuAboutToShow()
{
    windowsMenu_->clear();
    tileAction->addTo(windowsMenu_);
    tileHorizontalAction->addTo(windowsMenu_);
    cascadeAction->addTo(windowsMenu_);

    QWidgetList windows = ws->windowList();
    tileAction->setEnabled(!windows.isEmpty());
    tileHorizontalAction->setEnabled(!windows.isEmpty());
    cascadeAction->setEnabled(!windows.isEmpty());

    if (!windows.isEmpty()) {
        windowsMenu_->insertSeparator();
        for (int i = 0; i < int(windows.count()); ++i ) {
            int id = windowsMenu_->insertItem
                (windows.at(i)->caption(), this, SLOT(raiseWindow(int)));
            windowsMenu_->setItemParameter(id, i);
            windowsMenu_->setItemChecked(id, ws->activeWindow() == windows.at(i));
        }
    }
}

void MainWindow::zoomIn()
{
    zoomStepwise(1);
}

void MainWindow::zoomNormal()
{
    zoomStepwise(0);
}

void MainWindow::zoomOut()
{
    zoomStepwise(-1);
}

/**
 * Supports toolbar/menu zooming.
 * If step equals 0, the default value will be set (100 %).
 */
void MainWindow::zoomStepwise(int step)
{
    if (step == 0) {
        zoomComboBox->setCurrentItem(DEFAULT_ZOOM_LEVEL);
    }
    else {
        int i = zoomComboBox->currentItem() + step;
        if (i < 0 || i >= zoomComboBox->count()) {
            // index out of bounds
            return;
        }
        zoomComboBox->setCurrentItem(i);
    }
    zoomTo(zoomComboBox->currentText());
}

void MainWindow::zoomTo(const QString& level)
{
    QString level_ = level.stripWhiteSpace();
    if (level_.endsWith("%")) {
        level_.truncate(level_.length() - 1);
    }

    bool success;
    double zoom = level_.toInt(&success);
    if (success) {
        zoom /= 100.0;
        MdiWindow *m = activeWindow();
        if (m != 0) {
            m->setZoomLevel(zoom);
            m->resizeCanvas();
        }
    }
}
