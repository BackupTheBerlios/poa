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
 * $Id: mainwindow.cpp,v 1.44 2003/09/16 16:03:32 squig Exp $
 *
 *****************************************************************************/

#include "mainwindow.h"

#include "aboutdialog.h"
#include "cpuview.h"
#include "blockview.h"
#include "canvasview.h"
#include "project.h"
#include "gridcanvas.h"
#include "librarywindow.h"
#include "modelfactory.h"
#include "blockconfdialog.h"
#include "poa.h"
#include "project.h"
#include "settings.h"
#include "settingsdialog.h"

#include <qaction.h>
#include <qapplication.h>
#include <qcanvas.h>
#include <qclipboard.h>
#include <qcombobox.h>
#include <qdragobject.h>
#include <qdom.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qimage.h>
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


/**
 * Constructs the main window.
 *
 * The main window is only instanciated once.
 */
MainWindow::MainWindow(QWidget *parent, const char *name, WFlags fl)
    : QMainWindow(parent, name, fl), project_(0)
{
    // load toolbar icons/items


    // initialize main window
    if (!name) {
        setName("MainWindow");
    }
    setCaption(tr("POA"));
    setIcon(QPixmap(ICON_PATH + "poa.png"));

    // restore window settings
    Settings *s = Settings::instance();
    move(s->getNum("MainWindow/X"), s->getNum("MainWindow/Y"));
    resize(s->getNum("MainWindow/Width"),s->getNum("MainWindow/Height"));

    // initialize status bar implicitly
    (void)statusBar();

    // initialize library dock window
    LibraryWindow* lw = new LibraryWindow(QDockWindow::InDock, this);
    moveDockWindow(lw, Qt::DockLeft);

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
    QPixmap image_configure(ICON_PATH + "configure.png");
    QPixmap image_contents(ICON_PATH + "contents.png");
    QPixmap image_download(ICON_PATH + "dowload.png");
    QPixmap image_editcopy(ICON_PATH + "editcopy.png");
    QPixmap image_editcut(ICON_PATH + "editcut.png");
    QPixmap image_editdelete(ICON_PATH + "editdelete.png");
    QPixmap image_editpaste(ICON_PATH + "editpaste.png");
    QPixmap image_filesave(ICON_PATH + "filesave.png");
    QPixmap image_help(ICON_PATH + "help.png");
    QPixmap image_line(ICON_PATH + "line.xpm");
    QPixmap image_zoomin(ICON_PATH + "zoomin.png");
    QPixmap image_zoomnormal(ICON_PATH + "zoomnormal.png");
    QPixmap image_zoomout(ICON_PATH + "zoomout.png");

    fileNewAction = new QAction
        (tr("New"), QPixmap(ICON_PATH + "filenew.png"),
         tr("&New"), QKeySequence("Ctrl+N"), this, "fileNewAction");
    fileOpenAction = new QAction
        (tr("Open..."), QPixmap(ICON_PATH + "fileopen.png"),
         tr("&Open..."), QKeySequence("Ctrl+O"), this,
         "fileOpenAction");
    fileSaveAction =
        new QAction("Save", image_filesave, "&Save",
                    QKeySequence("Ctrl+S"), this, "fileSaveAction");
    fileSaveAsAction =
        new QAction("Save As...", "Save &As...", 0, this, "fileSaveAsAction");
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
    openBlockConfAction =
        new QAction("Block configuration", image_configure,
                    "&Block configuration", 0, this,
                    "openBlockConfAction");
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
        new QAction("Compile", QPixmap(ICON_PATH + "compile.png"), "&Compile",
                    QKeySequence("F8"), this, "invokeCompilerAction");
    invokeDownloadAction =
        new QAction("Download", image_download, "&Download",
                    QKeySequence("F7"), this, "invokeDownloadAction");
    openSettingsAction =
        new QAction("Configure POA", "Configure &POA...", 0,
                    this, "openSettingsAction");
    tileHorizontalAction =
        new QAction("Tile Horizontal", "Tile &Horizontal", 0,
                    this, "tileHorizontalAction");
    tileAction =
        new QAction("Tile", "&Tile", 0, this, "tileAction");
    cascadeAction =
        new QAction("Cascade", "&Cascade", 0, this, "cascadeAction");
    settingsShowGridAction_ =
        new QAction(tr("Show Grid"), tr("&Show Grid"),
                    QKeySequence(tr("Ctrl+G")), this, "editSettingsShowGrid",
                    true);
    settingsShowGridAction_->setOn(Settings::instance()->showGrid());
}

void MainWindow::initializeToolbars()
{
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
    openBlockConfAction->addTo(utilToolBar);
    utilToolBar->addSeparator();
    invokeCompilerAction->addTo(utilToolBar);
    invokeDownloadAction->addTo(utilToolBar);

    // view
    viewToolBar = new QToolBar(tr("view toolbar"), this, DockTop);
    zoomComboBox = new QComboBox(false, viewToolBar);
    zoomComboBox->insertItem("10 %", 0);
    zoomComboBox->insertItem("25 %", 1);
    zoomComboBox->insertItem("50 %", 2);
    zoomComboBox->insertItem("75 %", 3);
    zoomComboBox->insertItem("100 %", DEFAULT_ZOOM_LEVEL);
    zoomComboBox->insertItem("250 %", 5);
    zoomComboBox->insertItem("500 %", 6);
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
    QPopupMenu *windowMenu;
    QPopupMenu *helpMenu;

    fileMenu = new QPopupMenu(this);
    menuBar()->insertItem(trUtf8("&Project"), fileMenu);
    fileNewAction->addTo(fileMenu);
    fileOpenAction->addTo(fileMenu);
    fileMenu->insertSeparator();
    fileSaveAction->addTo(fileMenu);
    fileSaveAsAction->addTo(fileMenu);
    fileMenu->insertSeparator();
    recentProjectsMenu = new QPopupMenu(fileMenu);
    fileMenu->insertItem(tr("&Recent"), recentProjectsMenu);
    updateRecentProjectsMenu();
    fileMenu->insertSeparator();
    fileExitAction->addTo(fileMenu );

    // edit
    editMenu = new QPopupMenu(this);
    menuBar()->insertItem(trUtf8("&Edit"), editMenu);
    editCutAction->addTo(editMenu);
    editCopyAction->addTo(editMenu);
    editPasteAction->addTo(editMenu);

    // view
    viewMenu = new QPopupMenu(this);
    menuBar()->insertItem(trUtf8("&View"), viewMenu);
    zoomInAction->addTo(viewMenu);
    zoomNormalAction->addTo(viewMenu);
    zoomOutAction->addTo(viewMenu);

    // tools
    toolsMenu = new QPopupMenu(this);
    menuBar()->insertItem(tr("&Tools"), toolsMenu);
    openBlockConfAction->addTo(toolsMenu);
    toolsMenu->insertSeparator();
    invokeCompilerAction->addTo(toolsMenu);
    invokeDownloadAction->addTo(toolsMenu);

    // settings
    settingsMenu = new QPopupMenu(this);
    menuBar()->insertItem(tr("&Settings"), settingsMenu);
    settingsShowGridAction_->addTo(settingsMenu);
    settingsMenu->insertSeparator();
    openSettingsAction->addTo(settingsMenu);

    // window
    windowMenu = new QPopupMenu(this);
    //menuBar()->insertItem(tr("&Window"), windowMenu);
    tileAction->addTo(windowMenu);
    tileHorizontalAction->addTo(windowMenu);
    cascadeAction->addTo(windowMenu);

    // help
    helpMenu = new QPopupMenu(this);
    menuBar()->insertItem(trUtf8("&Help"), helpMenu );
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
    connect(fileExitAction, SIGNAL(activated()),
            qApp, SLOT(closeAllWindows()));
    connect(editCutAction, SIGNAL(activated()), this, SLOT(editCut()));
    connect(editCopyAction, SIGNAL(activated()), this, SLOT(editCopy()));
    connect(editPasteAction, SIGNAL(activated()), this, SLOT(editPaste()));
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
    connect(tileHorizontalAction, SIGNAL(activated()),
            this, SLOT(tileHorizontal()));
    connect(tileAction, SIGNAL(activated()), ws, SLOT(tile()));
    connect(cascadeAction, SIGNAL(activated()), ws, SLOT(cascade()));
    connect(ws, SIGNAL(windowActivated(QWidget*)),
            this, SLOT(windowActivated(QWidget*)));
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
    MdiWindow *m = (MdiWindow *)ws->activeWindow();
    return (m != 0) ? m->view() : 0;
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

    // save settings
    Settings *s = Settings::instance();
    s->set("MainWindow/X", x());
    s->set("MainWindow/Y", y());
    s->set("MainWindow/Width", width());
    s->set("MainWindow/Height", height());

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

QAction *MainWindow::blockConfAction()
{
    return openBlockConfAction;
}

void MainWindow::fileNew()
{
    // close current project
    if (!closeAll()) {
        return;
    }
    if (project_) {
        delete project_;
    }

    project_ = new Project(tr("Unnamed"));
    GridCanvas *canvas = project_->newCanvas("1");
    CanvasView *view = new CanvasView(project_, canvas);

    MdiWindow *w = new MdiWindow(view, ws, 0, WDestructiveClose);
    w->showMaximized();

    // connect to signals
    connect(view, SIGNAL(selectionChanged(bool)), editCutAction,
            SLOT(setEnabled(bool)));
    connect(view, SIGNAL(selectionChanged(bool)), editCopyAction,
            SLOT(setEnabled(bool)));

    // show the very first window in maximized mode
//      if (ws->windowList().isEmpty()) {
//          w->showMaximized();
//      }
//      else {
//          w->show();
//      }
}

void MainWindow::fileOpen()
{
    QString filename
        = QFileDialog::getOpenFileName(QString::null, QString::null, this);
    if (!filename.isEmpty()) {
        openProject(filename);
    }
}

void MainWindow::fileSave()
{
    if (project_) {
        if (project_->filename().isEmpty()) {
            fileSaveAs();
        }
        else {
            saveProject();
        }
    }
}

void MainWindow::fileSaveAs()
{
    QString filename
        = QFileDialog::getSaveFileName(QString::null, QString::null, this);
    if (!filename.isEmpty()) {
        // FIX: check if file already exists
        project_->setFilename(filename);
        saveProject();
        Settings::instance()->addToRecentProjects(filename);
    }
}

void MainWindow::fileExit()
{
    qWarning( "MainWindow::fileExit(): Not implemented yet!" );
}

void MainWindow::editCut()
{
    editCopy();
    CanvasView *view = activeView();
    if (view != 0) {
        QCanvasItemList items = view->selectedItems();
        // FIX: delete items
    }
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
                AbstractView *item = dynamic_cast<AbstractView *>(*current);
                if (item != 0 && item->model() != 0) {
                    root.appendChild(item->model()->serialize(&doc));
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
                    view->project()->addBlock(*it);
                    view->gridCanvas()->addView(*it);
                }
            }
        }
    }
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

MainWindow *MainWindow::instance()
{
    return (MainWindow *)qApp->mainWidget();
}

void MainWindow::openBlockConf()
{

    CanvasView *view = activeView();
    if (view != 0) {
        QCanvasItemList l = view->selectedItems();
        QCanvasItem *topItem = l.isEmpty() ? 0 : l.first();

        if (INSTANCEOF(topItem, BlockView)) {
            AbstractModel *model = ((BlockView *)topItem)->model();
            if (INSTANCEOF(model, BlockModel)) {
                BlockConfDialog *dialog =
                    new BlockConfDialog((BlockModel *)model);
                dialog->show();


                // destray dialog
            }
        }
    }
    // future: use exec() instead of show and
    //         determine exit code
}

void MainWindow::openProject(QString filename)
{
    if (!closeAll()) {
        return;
    }

    if (project_) {
        delete project_;
    }

    // FIX: wo ist die Fehlerbehandlung?
    QFile file(filename);
    if (file.open(IO_ReadOnly)) {
        QDomDocument doc;
        if (doc.setContent(&file)) {
            project_ = new Project(filename, &doc);
            project_->setFilename(filename);
            //               GridCanvas *canvas = new GridCanvas(project_);
            //                project_->addCanvas(canvas);
            //                project_->deserialize(&doc);
            //                MdiWindow *w = new MdiWindow(project_, ws, 0, WDestructiveClose);
            GridCanvas *canvas = project_->canvasList()->getFirst();
            CanvasView *view = new CanvasView(project_, canvas);

            MdiWindow *w = new MdiWindow(view, ws, 0, WDestructiveClose);
            w->showMaximized();

            Settings::instance()->addToRecentProjects(filename);
        }
        else {
            QMessageBox::warning
                (this, tr("File error"),
                 tr("Cannot open project file: %1").arg(filename));
        }
        file.close();
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

void MainWindow::openSettings()
{
    SettingsDialog *dialog = new SettingsDialog();
    dialog->show();
}

QAction *MainWindow::pasteAction()
{
    return editPasteAction;
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

void MainWindow::saveProject()
{
    // FIX: wo ist die Fehlerbehandlung?
    QFile file(project_->filename());
    if (file.open(IO_WriteOnly)) {
        QTextStream ts(&file);
        project_->serialize().save(ts, 2);
        file.close();
    }
}


QAction *MainWindow::showGridAction()
{
    return settingsShowGridAction_;
}

void MainWindow::windowActivated(QWidget* w)
{
    if (w != 0) {
        MdiWindow *m = (MdiWindow *)ws->activeWindow();

        checkClipboardContent();
        zoomComboBox->setEnabled(true);
        zoomComboBox->setEditText
            (QString::number(m->zoomLevel() * 100.0) + "%");
    }
    else {
        editCutAction->setEnabled(false);
        editCopyAction->setEnabled(false);
        editPasteAction->setEnabled(false);
        zoomComboBox->setEnabled(false);
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
        MdiWindow *m = (MdiWindow *)ws->activeWindow();
        if (m != 0) {
            m->setZoomLevel(zoom);
            m->resizeCanvas();
        }
    }
}

