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
 * $Id: connectorviewsegment.h,v 1.8 2004/01/28 01:49:37 kilgus Exp $
 *
 *****************************************************************************/


#ifndef POA_CONNECTORVIEWSEGMENT_H
#define POA_CONNECTORVIEWSEGMENT_H

#include <qcanvas.h>

#include "tooltipable.h"
#include "pinview.h"
#include "poa.h"
#include "removeable.h"
#include "settings.h"
#include "moveable.h"

class ConnectorViewList;
class Project;

/**
 * Definition of a segment of a connector view.
 *
 * Each segment represents either a horizontal or a vertical line.
 * An entire list connects the source pin to the target pin.
 */
class ConnectorViewSegment: public QObject,
                            public QCanvasLine,
                            public Removeable,
                            public Tooltipable
{
    Q_OBJECT

public:
    /**
     * Creates a connector view segment on the given <code>canvas</code>
     * to draw a straight line from <code>from</code> point to <code>to</code>
     * point.
     */
    ConnectorViewSegment(QPoint from,
                         QPoint to,
                         QCanvas *canvas,
                         ConnectorViewList *viewList);

    /**
     * Default destructor.
     */
    virtual ~ConnectorViewSegment();

    /**
     * Invoked when the item is removed by the user.
     */
    virtual void remove(Project *project);

    /**
     * Returns the tooltip text.
     */
    QString tip();

    /*
     * Invoked by ConnectorViewList.
     */
    void select(bool);

    /*
     * Selects this connector.
     */
    void setSelected(bool);

    /**
     * Sets the pen according to the selection and hover state.
     */
    void updateProperties();

    /**
     * Returns a list containing all segments belonging to one connector
     * view.
     */
    ConnectorViewList *viewList();

private:
    /** List of all segments */
    ConnectorViewList *viewList_;

};


#endif // POA_CONNECTORVIEWSEGMENT_H
