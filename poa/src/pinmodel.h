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
 * $Id: pinmodel.h,v 1.7 2003/08/29 23:51:08 vanto Exp $
 *
 *****************************************************************************/


#ifndef POA_PINMODEL_H
#define POA_PINMODEL_H

#include <qcanvas.h>
#include <qdom.h>

class BlockView;
class BlockModel;
class ConnectorModel;
class QDomDocument;
class QDomElement;

#include "pinview.h"


class PinModel
{

public:

    /*************************************************************************
     * Creates a view for <code>PinModel</code> on <code>canvas</code>.
     * @param parent is the parent BlockModel
     */
    PinModel(BlockModel *parent, const QString &name);

    /*************************************************************************
     * Creates a view for <code>PinModel</code> on <code>canvas</code> and de-
     * serializes the given xml subtree to set this' properties.
     * @param parent is the parent BlockModel
     */
    PinModel(BlockModel *parent, QDomElement pinElem);

    /*************************************************************************
     * Default destructor
     */
    virtual ~PinModel();

    /*************************************************************************
     * Returns this' parent (BlockModel)
     */
    BlockModel *parent();


    /*************************************************************************
     * Attach to a ConnectorModel
     */
    void attach(ConnectorModel *connector);

    /*************************************************************************
     * Detach from the attached ConnectorModel
     */
    void detach();


    /*************************************************************************
     * Returns this' id. 
     * The id is set by the BlockModel which contains this pin.
     */
    unsigned id();

    /*************************************************************************
     * Sets this' id. 
     * Used by the BlockModel
     */
    void setId(const unsigned id);

    /*************************************************************************
     * Returns this' name
     */
    QString name();
    
    /*************************************************************************
     * Sets this' name
     */
    void setName(QString &name);

    /*************************************************************************
     * Returns this pin's address on the cpld
     */
    unsigned int address();

    /*************************************************************************
     * Sets this pin's address on the cpld
     */
    void setAddress(unsigned int address);

    /*************************************************************************
     * Returns this pin's bit width on the cpld
     */
    unsigned int bits();

    /*************************************************************************
     * Sets this pin's bit width on the cpld
     */
    void setBits(unsigned int bits);

    /*************************************************************************
     * Creates the CanvasItems for this.
     */
    PinView *createView(BlockView *block,
            PinView::DockPosition dockPosition);

    /*************************************************************************
     * Serializes this instance to a xml subtree
     * @param document the main QDomDocument instance. Needed to create 
     * elements
     */
    QDomElement serialize(QDomDocument *document);

    /*************************************************************************
     * Deserializes an xml subtree and sets this' properties
     */
    void deserialize(QDomElement element);

private:

    unsigned id_;
    QString name_;
    BlockModel *parent_;
    unsigned address_;
    unsigned bits_;   // data type to be used in C source and width of pin
                      // one pin can be wider than one bit.
    ConnectorModel *connector_;

//public slots:
//
//    void attach (Connector *connector);
//    void deattach (Connector *connector);
//
};

#endif // POA_PINMODEL_H
