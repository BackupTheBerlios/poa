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
 * $Id: abstractmodel.cpp,v 1.9 2003/09/23 09:53:07 garbeam Exp $
 *
 *****************************************************************************/
#include "abstractmodel.h"

AbstractModel::AbstractModel(QString type, QString description, uint id)
    : type_(type), description_(description), id_(id)
{
}

/**
 * Returns a description of the item.
 */
QString AbstractModel::description() const
{
    return description_;
}

/**
 * Returns the name of the item.
 */
QString AbstractModel::type() const
{
    return type_;
}

/**
 * Returns the project unique id
 */
uint AbstractModel::id() const
{
    return id_;
}

QString AbstractModel::name() const
{
    return name_;
}

void AbstractModel::setName(QString name)
{
    name_ = name;
}

/**
 * Produces the XML representation of this instance like:
 *
 * <cpu name="cpu_XY" id="nn" autotime="true"/>
 */
QDomElement AbstractModel::serialize(QDomDocument *document)
{
    QDomElement root = document->createElement("model-item");
    root.setAttribute("type", type());
    root.setAttribute("desc", description());
    root.setAttribute("id", id());
    return root;
}

void AbstractModel::deserialize(QDomElement element)
{
    setType(element.attribute("type", "unknown"));
    setDescription(element.attribute("desc","no description"));
    setId(element.attribute("id","0").toUInt());
}

void AbstractModel::setDescription(const QString &description)
{
    description_ = description;
}

void AbstractModel::setType(const QString &type)
{
    type_ = type;
}

void AbstractModel::setId(uint id)
{
    id_ = id;
}

void AbstractModel::updatePerformed()
{
    emit(updated());
}
