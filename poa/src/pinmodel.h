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
 * $Id: pinmodel.h,v 1.6 2003/08/29 17:59:38 vanto Exp $
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

    PinModel(BlockModel *parent, const QString &name);
    PinModel(BlockModel *parent, QDomElement pinElem);

    virtual ~PinModel();

    BlockModel *parent();

    void attach(ConnectorModel *connector);
    void detach();

    unsigned id();
    void setId(const unsigned id);

    QString name();
    void setName(QString &name);

    unsigned int address();
    void setAddress(unsigned int address);

    unsigned int bits();
    void setBits(unsigned int bits);

    PinView *createView(BlockView *block,
            PinView::DockPosition dockPosition);

    /*
     * Serializes this instance to a xml subtree
     * @param document the main QDomDocument instance. Needed to create elements
     */
    QDomElement serialize(QDomDocument *document);

    /*
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
