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
 * $Id: project.cpp,v 1.3 2003/08/27 10:50:22 vanto Exp $
 *
 *****************************************************************************/
#include "project.h"

Project::Project()
{

}

Project::~Project()
{
}

void Project::add(AbstractModel *item, int x, int y)
{
    items_.append(item);
    emit modelAdded(item, x, y);
}

QDomDocument Project::serialize()
{
    QDomDocument doc;
    QDomElement proj = doc.createElement("project");
    doc.appendChild(proj);
    QDomElement mlist = doc.createElement("models");
    proj.appendChild(mlist);
    QDomElement vlist = doc.createElement("views");
    proj.appendChild(vlist);

    // create model list
    AbstractModel *model;
    for (model = items_.first(); model; model = items_.next()) {
        mlist.appendChild(model->serialize(&doc));
    }
    return doc;
    //TODO doc.appendChild(items[i
}

