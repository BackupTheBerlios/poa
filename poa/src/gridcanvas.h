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
 * $Id: gridcanvas.h,v 1.30 2004/01/28 18:00:56 squig Exp $
 *
 *****************************************************************************/

#ifndef POA_GRIDCANVAS_H
#define POA_GRIDCANVAS_H

#include "pinview.h"

#include <qcanvas.h>
#include <qpixmap.h>
class QString;

#include "colormanager.h"
#include "connectorviewlist.h"
class AbstractModel;
class ConnectorRouter;
class Project;

/**
 * Defines the grid canvas.
 */
class GridCanvas : public QCanvas {

    Q_OBJECT

public:

    /**
     * Creates a new grid canvas with the given name.
     * name will be shown in the mdiwindow's title and will be serialized to
     * xml, to identify this instance. name must be project-wide unique
     */
    GridCanvas(QString name);

    virtual ~GridCanvas();

    /**
     * Adds a view for the given <code>model</code> to the canvas at
     * (x,y)-coords This method creates the view instance (and its
     * children) by calling AbstractModel::createView(QCanvas *canvas)
     */
    void addView(AbstractModel *item, int x = 0, int y = 0);

    void addView(QCanvasItem *item, int x = 0, int y = 0);

    /**
     * Adds ConnectorViews between the two given PinViews. Uses an egoistic
     * router to route <code>viewList</code>
     */
    void addConnectorView(ConnectorViewList *viewList);

    /**
     * Expands the canvas to include item in case item lies outside
     * the canvas.
     */
    void ensureVisibility(QCanvasItem *item);

    /**
     * To be called whenever a set of <code>ConnectorViewList</code>s has
     * changed and needs to be rerouted. This member executes very efficiently
     * and can be called during moving operations. Thus the resulting routing
     * might be ugly.
     */
    void route(ConnectorViewList *connector);

    /**
     * Increases the z-layer and returns the increased value.
     */
    double incZ();

    /**
     * Returns grid coordiante that is closest to <code>p</code>.
     */
    QPoint toGrid(QPoint p);

    /**
     * Returns the color manager.
     */
    ColorManager *colorManager();

public slots:
    void updateAll();

protected:

    virtual void drawBackground(QPainter &painter, const QRect &clip);

private:

    /**
     * Egoistic router for adding new <code>ConnectorViewList</code>s.
     */
    ConnectorRouter *router_;

    /**
     * Colormanager
     */
    ColorManager *colormanager_;

    /**
     * Counter for z-layer. Everytime a an item is added to the canvas,
     * all canvas items that make up the view for that item are assigned
     * an equal z value
     */
    unsigned currentZ_;

};

#endif // POA_GRIDCANVAS_H
