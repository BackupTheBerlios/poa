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
 * $Id: pinmodel.cpp,v 1.6 2003/09/09 14:04:44 vanto Exp $
 *
 *****************************************************************************/


#include "pinmodel.h"

#include <qstring.h>
#include <qdom.h>

#include "blockview.h"
#include "blockmodel.h"
#include "connectormodel.h"


PinModel::PinModel(BlockModel *parent, const QString &name)
    : AbstractModel("connector", "Connector")
{
    name_ = name;
    parent_ = parent;
    connector_ = 0;
    id_ = 0;
    address_ = 0;
    bits_ = 0;
}

PinModel::PinModel(BlockModel *parent, QDomElement pinElem)
{
    parent_ = parent;
    connector_ = 0;
    if (!pinElem.isNull()) {
        deserialize(pinElem);
    }
}

PinModel::~PinModel()
{
    detach();
}

BlockModel *PinModel::parent()
{
    return parent_;
}

void PinModel::attach(ConnectorModel *connector)
{
    connector_ = connector;
    // FIX: update views
}


void PinModel::detach()
{
    if (connector_ != 0) {
    connector_ = 0;
    // FIX: update views
    }
}


unsigned PinModel::id()
{
    return id_;
}


void PinModel::setId(unsigned id)
{
    id_ = id;
}

QString PinModel::name()
{
    return name_;
}


void PinModel::setName(QString &name)
{
    name_ = name;
}

void PinModel::setAddress(unsigned int address)
{
    address_ = address;
}

unsigned int PinModel::address()
{
    return address_;
}

void PinModel::setBits(unsigned int bits)
{
    bits_ = bits;
}

unsigned int PinModel::bits()
{
    return bits_;
}

QCanvasItemList PinModel::createView(QCanvas *canvas)
{
    // returns an empty list
    return QCanvasItemList();
}

PinView *PinModel::createView(BlockView *block,
                  PinView::DockPosition dockPosition)
{
    PinView *view = new PinView(this, block, dockPosition);
    return view;
}

QString PinModel::tip()
{
    return QString("<b>Pin %1<br>%2</b><table><tr><td>Address</td><td>0x%3" \
                   "</td></tr><tr><td>Width</td><td>%4 bits</td></tr></table>")
        .arg(id_).arg(name_).arg(address_).arg(bits_);
}

QDomElement PinModel::serialize(QDomDocument *document)
{
    QDomElement root = document->createElement("pin");
    root.setAttribute("id", id_);
    root.setAttribute("name", name());
    root.setAttribute("address", (unsigned int)address_);
    root.setAttribute("bits", (unsigned int)bits_);
    return root;
}

void PinModel::deserialize(QDomElement element)
{
    name_ =element.attribute("name","unknown");
    id_ = element.attribute("id","0").toUInt();
    address_ = element.attribute("address","0").toUInt();
    bits_ = element.attribute("bits", "0").toUInt();
}
