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
 * $Id: muxmodel.cpp,v 1.1 2003/09/19 20:21:04 garbeam Exp $
 *
 *****************************************************************************/

#include "muxmodel.h"


MuxModel::MuxModel(QString type, QString description)
{

}

MuxModel::MuxModel(QDomElement coreElement)
{

}

MuxModel::~MuxModel()
{

}

QCanvasItemList MuxModel::createView(QCanvas *canvas)
{

    QCanvasItemList l;
    return l;
}

QDomElement MuxModel::serialize(QDomDocument *document)
{

    QDomElement e;
    return e;
}
