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
 * $Id: mainwindow.h,v 1.6 2003/08/20 15:33:30 garbeam Exp $
 *
 *****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mdiwindow.h"

#include <qvariant.h>
#include <qmainwindow.h>
#include <qworkspace.h>

class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QAction;
class QActionGroup;
class QToolBar;
class QPopupMenu;

class MainWindow : public QMainWindow
{ 
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0, const char* name = 0,
               WFlags fl = WType_TopLevel);
    ~MainWindow();

    QMenuBar *menubar;
    QPopupMenu *fileMenu;
    QPopupMenu *editMenu;
    QPopupMenu *toolsMenu;
    QPopupMenu *drawMenu;
    QPopupMenu *settingsMenu;
    QPopupMenu *helpMenu;
    QToolBar *commonToolBar;
    QToolBar *utilToolBar;
    QToolBar *drawToolBar;
    QAction* fileNewAction;
    QAction* fileOpenAction;
    QAction* fileSaveAction;
    QAction* fileSaveAsAction;
    QAction* fileExitAction;
    QAction* editCutAction;
    QAction* editCopyAction;
    QAction* editPasteAction;
    QAction* helpContentsAction;
    QAction* helpAboutAction;

    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *zoomNormalAction;
    QAction *drawLineAction;
    QAction *openModuleConfAction;
    QAction *invokeCompilerAction;
    QAction *invokeDownloadAction;

    QAction *openSettingsAction;

protected:
    void closeEvent(QCloseEvent *);

private:
    QWorkspace* ws;

public slots:
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
    virtual void openModuleConf();
    virtual void openSettings();

private slots:
    MdiWindow* newLayout();
    void closeWindow();
    void tileHorizontal();

};

#endif // MAINWINDOW_H
