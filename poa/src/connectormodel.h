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
 * $Id: connectormodel.h,v 1.3 2003/08/29 17:59:38 vanto Exp $
 *
 *****************************************************************************/


#ifndef POA_CONNECTORMODEL_H
#define POA_CONNECTORMODEL_H


#include "abstractmodel.h"
class PinModel;


/*****************************************************************************
 *
 */
class ConnectorModel: public AbstractModel
{
public:

    /*************************************************************************
     * Creates a new connector between two pins
     * @param source Source pin for this connector
     * @param target Target pin for this connector
     */
    ConnectorModel(PinModel *source,
           PinModel *target);

    /*************************************************************************
     * Default destructor
     */
    virtual ~ConnectorModel();

    /*************************************************************************
     * Creates the views.
     */
    virtual QCanvasItemList createView(QCanvas *canvas);

    /*
     * Returns the source of this connector
     */
    PinModel *source();

    /*
     * Returns the target of this connector
     */
    PinModel *target();

    /*
     * Returns the width of this connector
     */
    unsigned width();

    /*
     * Sets the width of this connector
     */
    void setWidth(const unsigned width);

    /*
     * Serializes this instance to a xml subtree
     * @param document the main QDomDocument instance. Needed to create elements
     */
    QDomElement serialize(QDomDocument *document);

    /*
     * Deserializes an xml subtree and sets this' properties
     */
    //void deserialize(QDomElement element);

private:

    /** Source pin. must not be 0 */
    PinModel *source_;
    /** Target pin. must not be 0  */
    PinModel *target_;

    /**
     * Number of bits. Must be equal to width of <code>source_</code> and
     * of <code>target_</code>
     */
    unsigned width_;
};


#endif  // POA_CONNECTORMODEL_H
