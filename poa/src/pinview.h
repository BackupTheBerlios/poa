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
 * $Id: pinview.h,v 1.21 2003/11/24 20:11:59 squig Exp $
 *
 *****************************************************************************/

#ifndef POA_PINVIEW_H
#define POA_PINVIEW_H

#include <qcanvas.h>
#include <qvaluevector.h>

#include "poa.h"
#include "tooltipable.h"

class BlockView;
class CanvasView;
class PinModel;

/**
 * Definition of a Pin view.
 */
class PinView: public QObject, public QCanvasRectangle,
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
     * Moves the canvas item relative to its current position by (dx, dy).
     * Emits the signal 'moved'
     */
    virtual void moveBy(double dx, double dy);

    /**
     * Returns the point, where a connector view must dock onto this
     * pin view.
     */
    QPoint connectorPoint();

    /**
     * Returns the direction that the docking line of a connector must have,
     * if <code>this</code> is the source of the connector view.
     */
    LineDirection connectorSourceDir();

    /**
     * Returns the direction that the docking line of a connector must have,
     * if <code>this</code> is the target of the connector view.
     */
    LineDirection connectorTargetDir();

    /**
     * Returns the side on that <code>this</code> docks onto a blockview
     */
    DockPosition dockPosition();

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


    /**
     * Returns the parent BlockView.
     */
    BlockView *parent();

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
    /**
     * Emitted everytime <code>this</code> is moved on the canvas.
     */
    void moved(PinView*);

};

#endif // POA_PINVIEW_H
