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
 * $Id: connectorview.h,v 1.2 2003/08/25 11:58:29 keulsn Exp $
 *
 *****************************************************************************/


#ifndef POA_CONNECTORVIEW_H
#define POA_CONNECTORVIEW_H


//#include <qcanvasitem.h>
#include <qcanvas.h>
#include <qpointarray.h>

#include "pinmodel.h"


/*****************************************************************************
 * Definition of a connector view.
 * @author keulsn
 */
class ConnectorView: public QCanvas//Item // probably QCanvasPolygon??
{

private:

    ConnectorModel *connectorModel_;
    QPointArray points_;

public:

    QPoint getPoint(int index);
    void setPoint(QPoint point, uint index);
    void addPoint(QPoint point, uint after = 0);
    void removePoint (uint index);

    ConnectorModel *model();
    void setModel(ConnectorModel *connectorModel);

};


#endif // POA_CONNECTORMODELVIEW_H
