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
 * $Id: pinmodel.cpp,v 1.34 2003/12/03 18:26:12 vanto Exp $
 *
 *****************************************************************************/


#include "pinmodel.h"

#include <qstring.h>
#include <qdom.h>

#include "blockview.h"

PinModel::PinModel(BlockModel *parent, const QString &name,
                   unsigned address, unsigned bits, PinType type)
{
    Q_ASSERT(name != 0);
    name_ = name;
    parent_ = parent;
    address_ = address;
    bits_ = bits;
    type_ = type;
    id_ = 0;
    position_ = 0;
    connected_ = 0;
    view_ = 0;
}

PinModel::PinModel(BlockModel *parent, QDomElement pinElem)
{
    parent_ = parent;
    position_ = 0;
    connected_ = 0;
    view_ = 0;

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

    if (view_) {
        emit(deleted());
    }
}

BlockModel *PinModel::parent()
{
    return parent_;
}

void PinModel::setParent(BlockModel *parent) {
    parent_ = parent;
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
        emit detached();
    }
}

PinModel *PinModel::connected()
{
    return connected_;
}

bool PinModel::isConnectable(PinModel *toPin)
{
    return (type() == EPISODIC
            || toPin->type() == EPISODIC
            || type() != toPin->type());
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

unsigned PinModel::position()
{
    return position_;
}

void PinModel::setPosition(unsigned position)
{
    position_ = position;
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
    view_ = new PinView(this, block, dockPosition);
    view_->show();
    return view_;
}

QDomElement PinModel::serialize(QDomDocument *document)
{
    QDomElement root = document->createElement("pin");
    root.setAttribute("id", id_);
    root.setAttribute("name", name());
    root.setAttribute("address", (unsigned int)address_);
    root.setAttribute("bits", (unsigned int)bits_);
    root.setAttribute("position", (unsigned int)position_);
    switch (type()) {
    case PinModel::INPUT:
        root.setAttribute("type", "input");
        break;
    case PinModel::OUTPUT:
        root.setAttribute("type", "output");
        break;
    case PinModel::EPISODIC:
        root.setAttribute("type", "episodic");
        break;
    }
    return root;
}

void PinModel::deserialize(QDomElement element)
{
    name_ =element.attribute("name","unknown");
    id_ = element.attribute("id","0").toUInt();
    address_ = element.attribute("address","0").toUInt();
    bits_ = element.attribute("bits", "0").toUInt();
    position_ = element.attribute("position","0").toUInt();
    if (element.attribute("type", "") == "input") {
        setType(PinModel::INPUT);
    }
    else if (element.attribute("type","") == "output") {
        setType(PinModel::OUTPUT);
    }
    else if (element.attribute("type","") == "episodic") {
        setType(PinModel::EPISODIC);
    }
}

PinModel *PinModel::clone()
{
    Q_ASSERT(parent_ != 0);

    PinModel *pin = new PinModel(parent_, name_, address_, bits_, type_);
    pin->setId(id_);
    pin->setPosition(position_);

    return pin;
}
