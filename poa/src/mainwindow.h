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
 * $Id: mainwindow.h,v 1.18 2003/09/11 12:43:11 garbeam Exp $
 *
 *****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mdiwindow.h"
class CanvasView;
class Project;

#include <qvariant.h>
#include <qmainwindow.h>
#include <qworkspace.h>
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QAction;
class QActionGroup;
class QComboBox;
class QToolBar;
class QPopupMenu;

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
     * Returns the copy action.
     */
    QAction *copyAction();

    /**
     * Returns the cut action.
     */
    QAction *cutAction();

    /**
     * Returns the paste action.
     */
    QAction *pasteAction();


    /**
     * Returns the block configure dialog.
     */
    QAction *blockConfAction();

public slots:
    virtual void checkClipboardContent();
    virtual void fileNew();
    virtual void fileOpen();
    virtual void fileSave();
    virtual void fileSaveAs();
    virtual void fileExit();
    virtual void editCut();
    virtual void editCopy();
    virtual void editPaste();
    virtual void helpContents();
    virtual void helpAbout();
    virtual void openBlockConf();
    virtual void openProject(QString filename);
    virtual void openRecentProject(int index);
    virtual void openSettings();
    virtual void saveProject();
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
    void zoomStepwise(int step);

    QWorkspace* ws;
    Project *project_;
    static const uint DEFAULT_ZOOM_LEVEL = 4;

    QPopupMenu *recentProjectsMenu;

    QComboBox *zoomComboBox;
    QToolBar *commonToolBar;
    QToolBar *utilToolBar;
    QToolBar *viewToolBar;
    QAction *fileNewAction;
    QAction *fileOpenAction;
    QAction *fileSaveAction;
    QAction *fileSaveAsAction;
    QAction *fileExitAction;
    QAction *editCutAction;
    QAction *editCopyAction;
    QAction *editPasteAction;
    QAction *helpContentsAction;
    QAction *helpAboutAction;

    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *zoomNormalAction;
    QAction *drawLineAction;
    QAction *openBlockConfAction;
    QAction *invokeCompilerAction;
    QAction *invokeDownloadAction;

    QAction *openSettingsAction;
    QAction *tileHorizontalAction;
    QAction *cascadeAction;
    QAction *tileAction;

private slots:
    void closeWindow();
    void tileHorizontal();
    void updateRecentProjectsMenu();

};

#endif // MAINWINDOW_H
