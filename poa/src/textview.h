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
 * $Id: textview.h,v 1.1 2004/01/22 12:07:42 squig Exp $
 *
 *****************************************************************************/

#ifndef POA_TEXTVIEW_H
#define POA_TEXTVIEW_H

#include <qcanvas.h>
#include <qdom.h>

#include "moveable.h"
#include "removeable.h"
#include "serializable.h"

/**
 * Provides a movable, editable text item for comments.
 */
class TextView: public QCanvasText,
                public Moveable,
                public Removeable,
                public Serializable
{
public:

    /**
     */
    TextView(const QString &text, QCanvas *canvas);

    TextView(QDomElement element, QCanvas *canvas);

    /**
     * Default destructor
     */
    virtual ~TextView();

    /**
     * Returns <code>x()</code>. Needed for {@link Moveable}.
     */
    virtual double currentX();

    /**
     * Returns <code>y()</code>. Needed for {@link Moveable}.
     */
    virtual double currentY();

    /**
     * Deserializes an xml subtree and sets this' properties
     */
    void deserialize(QDomElement element);

    /**
     * Invokes moveBy(double,double).
     */
    virtual QSize dragBy(double dx, double dy);

    virtual void remove(Project *project);

    /**
     * Return own type information
     */
    virtual int rtti() const;
    static const int RTTI;

    /**
     * Serializes this instance to a xml subtree
     * @param document the main QDomDocument instance. Needed to create
     *        elements
     */
    QDomElement serialize(QDomDocument *document);

private:

};

#endif // POA_TEXTVIEW_H
