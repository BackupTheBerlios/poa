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
 * $Id: pinview.h,v 1.7 2003/08/30 18:37:33 vanto Exp $
 *
 *****************************************************************************/

#ifndef POA_PINVIEW_H
#define POA_PINVIEW_H

#include <qcanvas.h>
#include <qvaluevector.h>

class BlockView;
class PinModel;


/*****************************************************************************
 * Definition of a Pin view.
 */
class PinView: public QCanvasRectangle
{

public:

    /*************************************************************************
     * Defines the side on that a pin docks to a block
     */
    enum DockPosition {PIN_TOP, PIN_BOTTOM, PIN_LEFT, PIN_RIGHT};

    /*****************************************************************************
     * Creates a new pin view for the given pin and block model at the given 
     * position.
     */
    PinView(PinModel *model,
        BlockView *block,
        PinView::DockPosition dockPosition);

    /*****************************************************************************
     * Default destructor
     */
    virtual ~PinView();

    /*****************************************************************************
     * Returns the corresponding model of this view
     */
    PinModel *model();

    /*************************************************************************
     * Returns the side on that <code>this</code> docks onto a blockview
     */
    DockPosition dockPosition();

    /*************************************************************************
     * Returns the point, where a connector view must dock onto this
     * pin view.
     */
    QPoint connectorPoint();

    /**
     * Sets the brush to red if Pin is selected, otherwise black
     */
    virtual void setSelected(bool yes);

private:

    DockPosition dockPosition_;
    PinModel *model_;
};

#endif // POA_PINVIEW_H
