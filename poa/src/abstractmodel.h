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
 * $Id: abstractmodel.h,v 1.2 2003/08/22 17:39:04 squig Exp $
 *
 *****************************************************************************/


#ifndef ABSTRACTMODEL_H
#define ABSTRACTMODEL_H

#include <qobject.h>
class QCanvasItem;
class QDomDocument;
class QDomElement;
class QCanvas;

/*****************************************************************************
 * Base class for items in the module library.
 *
 * @author keulsn
 * @author Steffen Pingel
 */
class AbstractModel : public QObject
{

public:
    /*************************************************************************
     * Returns a description of the item.
     */
    virtual QCString description() = 0;

    /*************************************************************************
     * Returns the name of the item.
     */
    virtual QCString name() = 0;

    /*************************************************************************
     * Returns an XML representation of this instance.
     */
    virtual QDomElement serialize(QDomDocument *document) = 0;

    /*************************************************************************
     * Creates the corresponding view object.
     */
    virtual QCanvasItem *createView(QCanvas *canvas) = 0;

};

#endif
