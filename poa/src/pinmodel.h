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
 * $Id: pinmodel.h,v 1.5 2003/08/27 12:28:24 vanto Exp $
 *
 *****************************************************************************/


#ifndef POA_PINMODEL_H
#define POA_PINMODEL_H

#include <qcanvas.h>
#include <qdom.h>

class BlockView;
class ConnectorModel;
class QDomDocument;
class QDomElement;

#include "pinview.h"


class PinModel
{

public:

    PinModel(const QString &name);

    virtual ~PinModel();

    void attach(ConnectorModel *connector);
    void detach();

    QString name();
    void setName(QString &name);

    unsigned int address();
    void setAddress(unsigned int address);

    unsigned int bits();
    void setBits(unsigned int bits);

    PinView *createView(BlockView *block,
            PinView::DockPosition dockPosition);

    /*************************************************************************
     * Returns an XML representation of this instance.
     */
    virtual QDomElement serialize(QDomDocument *document);

private:

    QString name_;
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
