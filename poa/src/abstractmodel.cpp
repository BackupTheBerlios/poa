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
 * $Id: abstractmodel.cpp,v 1.2 2003/08/26 15:59:23 vanto Exp $
 *
 *****************************************************************************/
#include "abstractmodel.h"

AbstractModel::AbstractModel(QString name, QString description)
    : name_(name), description_(description)
{
}

/*************************************************************************
 * Returns a description of the item.
 */
QString AbstractModel::description() const
{
    return description_;
}

/*************************************************************************
 * Returns the name of the item.
 */
QString AbstractModel::name() const
{
    return name_;
}

/**
 * Produces the XML representation of this instance like:
 *
 * <cpu name="cpu_XY" id="nn" autotime="true"/>
 */
QDomElement AbstractModel::serialize(QDomDocument *document)
{
    QDomElement root = document->createElement("model-item");
    root.setAttribute("name", name());
    root.setAttribute("desc", description());
    return root;
}

void AbstractModel::setDescription(const QString &description)
{
    description_ = description;
}

void AbstractModel::setName(const QString &name)
{
    name_ = name;
}
