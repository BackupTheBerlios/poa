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
 * $Id: pinview.h,v 1.16 2003/09/19 15:16:22 vanto Exp $
 *
 *****************************************************************************/

#ifndef POA_PINVIEW_H
#define POA_PINVIEW_H

#include <qcanvas.h>
#include <qvaluevector.h>

#include "abstractview.h"
#include "poa.h"
#include "tooltipable.h"

class BlockView;
class CanvasView;
class PinModel;

/**
 * Definition of a Pin view.
 */
class PinView: public AbstractView, public QCanvasRectangle,
               public Tooltipable
{

    Q_OBJECT

public:

    /**
     * Defines the side on that a pin docks to a block
     */
    enum DockPosition {PIN_TOP, PIN_BOTTOM, PIN_LEFT, PIN_RIGHT};

    /**
     * Creates a new pin view for the given pin and block model at the given
     * position.
     */
    PinView(PinModel *model,
            BlockView *block,
            PinView::DockPosition dockPosition);

    /**
     * Default destructor
     */
    virtual ~PinView();

    /**
     * Returns the point, where a connector view must dock onto this
     * pin view.
     */
    QPoint connectorPoint();

    /**
     * Returns the direction that the docking line of a connector must have
     */
    LineDirection connectorDirection();

    /**
     * Returns the side on that <code>this</code> docks onto a blockview
     */
    DockPosition dockPosition();

    /**
     * Returns the corresponding model of this view.
     */
    virtual AbstractModel *model();

    /**
     * Returns the corresponding model of this view as a PinModel.
     *
     * @see #model()
     */
    PinModel *pinModel();

    /**
     * If <code>yes</code>, marks the pin green
     */
    virtual void setActive(bool yes);

    /**
     * Sets the brush to red if Pin is selected, otherwise black
     */
    virtual void setSelected(bool yes);

    /**
     * Return the tooltip text
     */
    QString tip();

private:
    /**
     * Sets the pen and brush according to the active and selection state.
     */
    void updateProperties();

    DockPosition dockPosition_;
    PinModel *model_;
    BlockView *block_;


public slots:
    void deleteView();

signals:
    void deleted(PinView *);

};

#endif // POA_PINVIEW_H
