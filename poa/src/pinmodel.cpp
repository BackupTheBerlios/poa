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
 * $Id: pinmodel.cpp,v 1.14 2003/09/15 17:38:51 garbeam Exp $
 *
 *****************************************************************************/


#include "pinmodel.h"

#include <qstring.h>
#include <qdom.h>

#include "blockview.h"
#include "blockmodel.h"

PinModel::PinModel(BlockModel *parent, unsigned id, const QString &name,
                   unsigned address, unsigned bits, PinType type)
{
    name_ = name;
    parent_ = parent;
    id_ = id;
    connected_ = 0;
    address_ = address;
    bits_ = bits;
    type_ = type;
}


PinModel::PinModel(BlockModel *parent, const QString &name)
{
    name_ = name;
    parent_ = parent;
    connected_ = 0;
    id_ = 0;
    address_ = 0;
    bits_ = 0;
}

PinModel::PinModel(BlockModel *parent, QDomElement pinElem)
{
    parent_ = parent;
    connected_ = 0;
    if (!pinElem.isNull()) {
        deserialize(pinElem);
    }
}

PinModel::~PinModel()
{
    if (connected_ != 0) {
        connected_->detach();
    }
    detach();
    emit(deleted());
}

BlockModel *PinModel::parent()
{
    return parent_;
}

void PinModel::attach(PinModel *connectTo)
{
    connected_ = connectTo;
    // FIX: update views
}


void PinModel::detach()
{
    if (connected_ != 0) {
        connected_ = 0;
    // FIX: update views
    }
}

PinModel *PinModel::connected()
{
    return connected_;
}

PinModel::PinType PinModel::type()
{
    return type_;
}

void PinModel::setType(PinModel::PinType type)
{
    type_ = type;
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


void PinModel::setName(QString name)
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

PinView *PinModel::createView(BlockView *block,
                  PinView::DockPosition dockPosition)
{
    PinView *view = new PinView(this, block, dockPosition);
    return view;
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

PinModel *PinModel::clone()
{
    return new PinModel(parent_, id_, name_, address_, bits_, type_);
}
