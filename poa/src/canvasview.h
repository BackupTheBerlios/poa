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
 * $Id: canvasview.h,v 1.1 2003/08/22 10:14:19 squig Exp $
 *
 *****************************************************************************/
#ifndef CANVASVIEW_H
#define CANVASVIEW_H

#include <qvariant.h>
#include <qcanvas.h>
class QMouseEvent;
class QPoint;

/*****************************************************************************
 * Provides the view for the gridcanvas.
 *
 * @author Steffen Pingel
 */
class CanvasView : public QCanvasView
{
    Q_OBJECT

public:
    CanvasView(QCanvas *canvas, QWidget *parent = 0, const char* name = 0,
               WFlags fl = 0);
    ~CanvasView();

	void contentsMousePressEvent(QMouseEvent *e);
	void contentsMouseMoveEvent(QMouseEvent *e);

private :
    QCanvasItem* movingItem;
    QPoint movingStartPoint;

};

#endif // CANVASVIEW_H
