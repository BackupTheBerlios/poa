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
 * $Id: gridcanvas.h,v 1.9 2003/08/28 15:31:10 vanto Exp $
 *
 *****************************************************************************/

#ifndef POA_GRIDCANVAS_H
#define POA_GRIDCANVAS_H

#include <qcanvas.h>
#include <qpixmap.h>
class Project;
class QString;
class AbstractModel;
/*****************************************************************************
 * Defines the grid canvas.
 */
class GridCanvas : public QCanvas {

    Q_OBJECT

public:
    GridCanvas(QString name);
    void addView(AbstractModel *item, int x, int y);
    QString name();

public slots:
    void setGridSize(int gridSize);

private:
    /**
     * Counter for z-layer. Everytime a an item is added to the canvas,
     * all canvas items that make up the view for that item are assigned
     * an equal z value
     */
    unsigned currentZ_;
    QString name_;
};

#endif // POA_GRIDCANVAS_H
