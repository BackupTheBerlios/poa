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
 * $Id: pinmodel.cpp,v 1.3 2003/08/26 23:27:11 vanto Exp $
 *
 *****************************************************************************/


#include "pinmodel.h"

#include <qstring.h>
#include <qdom.h>

#include "blockview.h"
#include "connectormodel.h"


PinModel::PinModel(const QString &name)
{
    name_ = name;
    connector_ = 0;
}


PinModel::~PinModel()
{
    detach();
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


QString PinModel::name()
{
    return name_;
}


void PinModel::setName(QString &name)
{
    name_ = name;
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
    root.setAttribute("name", name_);
    root.setAttribute("address", (unsigned int)address_);
    root.setAttribute("bits", (unsigned int)bits_);
    return root;
}

