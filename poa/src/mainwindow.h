/****************************************************************************
** Form interface generated from reading ui file 'mainwindow.ui'
**
** Created: Tue Aug 19 15:35:58 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
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
    QAction* openModuleConfDialogAction;

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
    virtual void openModuleConfDialog();

private slots:
    MdiWindow* newLayout();
    void closeWindow();
    void tileHorizontal();

};

#endif // MAINWINDOW_H
