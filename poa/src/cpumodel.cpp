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
 * $Id: cpumodel.cpp,v 1.2 2003/08/22 10:27:00 garbeam Exp $
 *
 *****************************************************************************/

#include "cpumodel.h"

#include <qdom.h> // used to provide serialization
#include <qtextstream.h>
#include <iostream.h>


CpuModel::CpuModel(QString *name, unsigned short id, bool autoExecTime)
{
    name_ = name;
    id_ = id;
    autoExecTime_ = autoExecTime;
}

/**
 * Produces the XML representation of this instance like:
 *
 * <cpu name="cpu_XY" id="nn" autotime="true"/>
 */
QByteArray *CpuModel::serialize()
{
    QDomDocument doc("cpu");
    QDomElement root = doc.createElement( "cpu" );
    doc.appendChild(root);

    QDomElement tag = doc.createElement("name");
    root.appendChild(tag);

    QDomText t = doc.createTextNode(*name_);
    tag.appendChild(t);

    QString xml = doc.toString();

    cout << xml;

    QByteArray *result = new QByteArray();
    QTextStream ts(*result, IO_WriteOnly);
    ts << xml;

    return result;
}
