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
 * $Id: mdiwindow.h,v 1.12 2003/08/22 12:31:14 garbeam Exp $
 *
 *****************************************************************************/

#ifndef POA_MDIWINDOW_H
#define POA_MIDWINDOW_H

#include <qmainwindow.h>
#include <qcanvas.h>

class QCanvasView;

/*****************************************************************************
 * Defines MDI windows.
 * @author garbeam
 */
class MdiWindow : public QMainWindow
{
    Q_OBJECT

public:
    MdiWindow(QCanvas* canvas, QWidget* parent = 0, const char* name = 0, 
              WFlags f = WType_TopLevel);
    ~MdiWindow();

    QCanvas *canvas();
    void setCanvas(QCanvas *);

    void load( const QString& fn );
    void save();
    void saveAs();
    double zoomLevel();
    void resizeCanvas();

public slots:
    void setZoomLevel(double zoomLevel);

protected:
    void closeEvent( QCloseEvent * );
    virtual void resizeEvent(QResizeEvent *e);

private:
    QCanvasView* view_;
    double zoomLevel_;

signals:
    void message(const QString&, int );

};

#endif // POA_MDIWINDOW_H
