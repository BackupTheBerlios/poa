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
 * $Id: coremodel.h,v 1.9 2003/09/09 23:21:22 vanto Exp $
 *
 *****************************************************************************/


#ifndef POA_COREMODEL_H
#define POA_COREMODEL_H


#include "blockmodel.h"
#include <qdom.h>

class QCanvas;
class QCanvasItemList;

/**
 * A block that calculates one specific function everytime it is triggered.
 */
class CoreModel: public BlockModel
{

 public:
    /**
     * Creates a CoreModel instance for the library
     */
    CoreModel(QString type, QString description);

    /**
     * Creates a CoreModel instance for the project out of an xml subtree
     */
    CoreModel(QDomElement coreElement);

    /**
     * Creates the CanvasItems for this.
     */
    virtual QCanvasItemList createView(QCanvas *canvas);

    /**
     * Serializes this instance to a xml subtree
     * @param document the main QDomDocument instance. Needed to create elements
     */
    QDomElement serialize(QDomDocument *document);

    /**
     * Deserializes an xml subtree and sets this' properties
     */
    //void deserialize(QDomElement element);
};


#endif
