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
 * $Id: coremodel.cpp,v 1.2 2003/08/29 14:34:41 vanto Exp $
 *
 *****************************************************************************/
#include "coremodel.h"
#include "abstractmodel.h"

#include <qdom.h> // used to provide serialization
#include <qtextstream.h>

#include "pinmodel.h"

CoreModel::CoreModel(QString type, QString description)
    : BlockModel(type, description)
{
    // FIX: remove
    PinModel *firstPin = new PinModel(QString("Input1"));
    addInputPin(firstPin);
    addInputPin(new PinModel("another Input"), firstPin);
    addOutputPin(new PinModel("Output"));
    // FIX: end remove
}

CoreModel::CoreModel(QDomElement coreElement)
    : BlockModel(QString::null, QString::null)
{
    if (!coreElement.isNull()) {
        BlockModel::deserialize(coreElement);
    }
}

/**
 * Produces the XML representation of this instance
 */
QDomElement CoreModel::serialize(QDomDocument *document)
{
    QDomElement root = BlockModel::serialize(document);
    root.setAttribute("block-type", "core");
    return root;
}
