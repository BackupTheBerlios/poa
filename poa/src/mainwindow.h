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
 * $Id: mainwindow.h,v 1.35 2004/01/21 23:38:21 squig Exp $
 *
 *****************************************************************************/

#ifndef POA_MAINWINDOW_H
#define POA_MAINWINDOW_H

#include "mdiwindow.h"
class CanvasView;
class ConnectorRouter;

#include <qvariant.h>
#include <qmainwindow.h>
#include <qworkspace.h>
class QCanvasItem;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QAction;
class QActionGroup;
class QComboBox;
class QToolBar;
class QPopupMenu;
class LibraryWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    /**
     * Default constructor
     */
    MainWindow(QWidget* parent = 0, const char* name = 0,
               WFlags fl = WType_TopLevel);

    /**
     * Default destructor
     */
    ~MainWindow();

    /**
     * Closes all mdi windows
     */
    bool closeAll();

    /**
     * Returns the content of the active window.
     * @return 0, if no window is active
     */
    CanvasView *activeView() const;

    /**
     * Returns a reference to the main window.
     */
    static MainWindow *instance();

    /**
     * Returns the block configure dialog.
     */
    QAction *blockConfAction();

    /**
     * Returns the copy action.
     */
    QAction *copyAction();

    /**
     * Returns the cut action.
     */
    QAction *cutAction();

    /**
     * Returns the default route action.
     */
    QAction *defaultRouteAction();

    /**
     * Returns the paste action.
     */
    QAction *pasteAction();

    /**
     * Returns the remove action.
     */
    QAction *removeAction();

    /**
     * Returns the save to library action.
     */
    QAction *saveToLibraryAction();

    /**
     * Returns the currently selected model.
     * @return 0, if no window is active or no model is selected
     */
    AbstractModel *selectedModel();

    /**
     * Returns the show grid action.
     */
    QAction *showGridAction();

    /**
     * Returns the smart route action.
     */
    QAction *smartRouteAction();

public slots:
    virtual void checkClipboardContent();
    virtual void defaultRoute();
    virtual void fileNew();
    virtual void fileOpen();
    virtual void filePrint();
    virtual void fileSave();
    virtual void fileSaveAs();
    virtual void fileExit();
    virtual void editCut();
    virtual void editCopy();
    virtual void editPaste();
    virtual void editRemove();
    virtual void helpContents();
    virtual void helpAbout();
    virtual void openBlockConf();
    virtual void openProject(QString filename);
    virtual void openRecentProject(int index);
    virtual void openScheduling();
    virtual void openSettings();
    virtual void openDeployWizard();
    //    virtual void saveProject();
    virtual void saveToLibrary();
    virtual void smartRoute();
    virtual void windowActivated(QWidget *w);
    virtual void zoomTo(const QString &level);
    virtual void zoomIn();
    virtual void zoomOut();
    virtual void zoomNormal();

protected:
    void closeEvent(QCloseEvent *);

private:
    void connectActions();
    void initializeActions();
    void initializeMenu();
    void initializeToolbars();
    void routeSelected(ConnectorRouter *router);
    void zoomStepwise(int step);

    /**
     * Creates a new project, stores it to the given location and
     * deletes it.
     */
    void createNewProject(QString path);

    QWorkspace* ws;
    static const uint DEFAULT_ZOOM_LEVEL;

    LibraryWindow *libraryWindow_;

    QPopupMenu *recentProjectsMenu;

    QComboBox *zoomComboBox;
    QToolBar *commonToolBar;
    QToolBar *utilToolBar;
    QToolBar *viewToolBar;
    QAction *fileNewAction;
    QAction *fileOpenAction;
    QAction *filePrintAction;
    QAction *fileSaveAction;
    QAction *fileSaveAsAction;
    QAction *fileExitAction;
    QAction *editCutAction;
    QAction *editCopyAction;
    QAction *editPasteAction;
    QAction *editRemoveAction;
    QAction *defaultRouteAction_;
    QAction *helpContentsAction;
    QAction *helpAboutAction;
    QAction *settingsShowGridAction_;
    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *zoomNormalAction;
    QAction *openBlockConfAction;
    QAction *invokeSchedulingAction;
    QAction *invokeCompilerAction;
    QAction *invokeDownloadAction;
    QAction *invokeDeployAction;
    QAction *openSettingsAction;
    QAction *smartRouteAction_;
    QAction *tileHorizontalAction;
    QAction *cascadeAction;
    QAction *saveToLibraryAction_;
    QAction *tileAction;

private slots:
    void closeWindow();
    void tileHorizontal();
    void updateRecentProjectsMenu();
    void selectionChanged(QCanvasItem *);
};

#endif // POA_MAINWINDOW_H
