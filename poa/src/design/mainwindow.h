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
 * $Id: mainwindow.h,v 1.1 2003/07/13 20:15:12 garbeam Exp $
 *
 *****************************************************************************/

#ifndef POA_MAINWINDOW_H
#define POA_MAINWINDOW_H


/*****************************************************************************
 * Defines the main application window.
 * @author garbeam
 */
class MainWindow: public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();

protected:
    void closeEvent( QCloseEvent * );

private slots:
    MdiWindow* newMdiWindow();
    void load();
    void save();
    void saveAs();
    void closeWindow();
    void tileHorizontal();

    void about();

    void windowsMenuAboutToShow();
    void windowsMenuActivated( int id );

private:
    QWorkspace* workspace_;
    QToolBar *toolbar_;
    QPopupMenu* windowMenu_;
};


#endif // POA_MAINWINDOW_H