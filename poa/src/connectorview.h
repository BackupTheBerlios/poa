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
 * $Id: connectorview.h,v 1.6 2003/08/30 18:37:33 vanto Exp $
 *
 *****************************************************************************/


#ifndef POA_CONNECTORVIEW_H
#define POA_CONNECTORVIEW_H

#include <qcanvas.h>

class ConnectorModel;
#include "pinview.h"


class ConnectorView;


/*****************************************************************************
 * Definition of a connector view. Connector views are doubly linked linear
 * lists of <code>ConnectorView</code>s. The first item in that list is
 * considered the view-object and listens to changes in the model.
 *
 * Each item in the list represents either a horizontal or a vertical line.
 * The entire list connects the source pin to the target pin.
 *
 * Each item in the list propagates changes to itself along the list. During
 * propagation some items may be removed or new items may be inserted into
 * the list.
 */
class ConnectorView: public QCanvasLine
{

public:
    /*****************************************************************************
     * Creates a connector view on the given <code>canvas</code> and draws a 
     * routed line from <code>from</code> pin to <code>to</code> pin for the
     * given <code>model</code>
     */
    ConnectorView(ConnectorModel *model,
          PinView *from,
          PinView *to,
          QCanvas *canvas);

    /*****************************************************************************
     * Default destructor
     */
    virtual ~ConnectorView();

    /*************************************************************************
     * Returns <code>this</code>'s model. Note that only the first
     * <code>ConnectorView</code> of a list has a model all others do not.
     * @return 0 or the model
     */
    ConnectorModel *model();

    /*************************************************************************
     * Returns a list including <code>this</code> and all succeeding view
     * objects. That is the list of all objects that make up a connector
     * view on a canvas.
     */
    QCanvasItemList allSegments();

    /**
     * Orientation of a <code>ConnectorView</code>.
     * <code>ConnectorView</code>s can only consist of  horizontal lines or
     * vertical lines.
     */
    enum LineOrientation {UNKNOWN, HORIZONTAL, VERTICAL};

    /*************************************************************************
     * Returns the orientation orthogonal to <code>orientation</code> or
     * <code>UNKNOWN</code> if <code>orientation == UNKNOWN</code>
     */
    static LineOrientation inflection(LineOrientation orientation);

    /*************************************************************************
     * Returns the orientation of this view.
     */
    LineOrientation orientation();

protected:
    ConnectorView(QPoint start,
          LineOrientation orientation,
          PinView *to,
          QCanvas *canvas);

    void setStartPoint(QPoint start);

    void setEndPoint(QPoint end);

    void setPrevConnector(ConnectorView *prev);

    void setNextConnector(ConnectorView *next);

    void setPrevPin(PinView *source);

    void setNextPin(PinView *target);

    void dockToSource(PinView *source);

    void dockToSource(ConnectorView *from);

    void dockToTarget(PinView *target);

    void dockToTarget(ConnectorView *to);
    
    void setOrientation(LineOrientation orientation);

    /*************************************************************************
     * Calculates the next point to be used in a multi line connector view.
     * Supposes a connector view exists until the point <code>start</code>
     * and the last segment has the orientation <code>orientation</code>.
     * The point <code>end</code> must be reached to connect to a pin with
     * direction <code>dock</code>.
     *
     * This function should be called until the value returned is equal
     * to <code>end</code>. Then <code>ConnectorView</code>-objects should
     * be created to connect the points returned.
     *
     * @param start The <code>ConnectorView</code> exists until this point
     * @param orientation Orientation of the line that reaches
     *        <code>start</code>
     * @param end The point to be reached (usually 
     *        {@link PinView#connectorPoint})
     * @param dock The direction in which the last line must dock onto
     *        <code>end</code>
     * @return The next point to be added to the polygon already drawn
     *         until <code>start</code>
     */
    static QPoint firstInflectionPoint(QPoint start,
                       LineOrientation orientation,
                       QPoint end,
                       PinView::DockPosition dock);

    /**
     * A <code>ConnectorView</code> can dock onto another connector view
     * or onto a pin.
     */
    union ConnectorDocking {
        ConnectorView *connector;
        PinView *pin;
    };


    static const unsigned DEFAULT_DOCK_LINE_LENGTH = 5;

private:
    /**
     * model for <code>this</code> if <code>this</code> is the first view
     * in the list, 0 else.
     */
    ConnectorModel *model_;

    /**
     * true if <code>this</code> has a pin as source or no source,
     * false else 
     */

    bool first_;
    /**
     * true if <code>this</code> has a pin as target or no target,
     * false else
     */

    bool last_;
    /**
     * <code>this</code>'s source item or 0 if <code>this</code> has no
     * source
     */
     
    ConnectorDocking prev_;
    /**
     * <code>this</code>'s target item or 0 if <code>this</code> has no target
     */
     
    ConnectorDocking next_;
    
    /** Orientation of this view */
    LineOrientation orientation_;

};


#endif // POA_CONNECTORMODELVIEW_H
