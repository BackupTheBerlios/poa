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
 * $Id: project.cpp,v 1.6 2003/08/27 21:12:45 vanto Exp $
 *
 *****************************************************************************/
#include "blockview.h"
#include "project.h"
#include "gridcanvas.h"
#include "modelfactory.h"

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

void Project::addCanvas(GridCanvas *canvas)
{
    canvasList_.append(canvas);
}

const QPtrList<GridCanvas> *Project::canvasList() const {
    return &canvasList_;
}

QDomDocument Project::serialize()
{
    typedef QMap<AbstractModel *, unsigned int> IdMap;
    IdMap idMap;

    QDomDocument doc;
    QDomElement proj = doc.createElement("project");
    doc.appendChild(proj);
    QDomElement mlist = doc.createElement("model");
    proj.appendChild(mlist);
    QDomElement vlist = doc.createElement("views");
    proj.appendChild(vlist);

    // create model list
    AbstractModel *model;
    unsigned int i = 0;
    for (model = items_.first(); model; model = items_.next()) {
        i++;
        QDomElement mElem = model->serialize(&doc);
        mElem.setAttribute("id",i);
        mlist.appendChild(mElem);
        idMap[model] = i;
    }

    // create view list
    GridCanvas *canvas;
    i = 0;
    for (canvas = canvasList_.first(); canvas; canvas = canvasList_.next()) {
        QDomElement vElem = doc.createElement("view");
        vElem.setAttribute("id",++i);
        QCanvasItemList canvasItems = canvas->allItems();
        QCanvasItemList::iterator it;
        for (it = canvasItems.begin(); it != canvasItems.end(); ++it) {
            BlockView *bv = (dynamic_cast<BlockView *> (*it));
            if (bv != 0) {
                QDomElement viElem = bv->serialize(&doc);
                viElem.setAttribute("model-id", idMap[bv->model()]);
                vElem.appendChild(viElem);
            }
        }
        vlist.appendChild(vElem);
    }
    return doc;
}

void Project::deserialize(QDomDocument *document) {
    typedef QMap<QString, AbstractModel*> IdMap;
    IdMap idMap;

    //QDomElement root = document->documentElement();
    QDomNodeList mList = document->elementsByTagName("model-item");
    QDomNodeList vList = document->elementsByTagName("view-item");
    for (unsigned int i = 0; i < mList.count(); i++) {
        QDomElement mEl = mList.item(i).toElement();
        idMap[mEl.attribute("id","0")] = ModelFactory::generateSingle(mEl);
    }

    for (unsigned int i = 0; i < vList.count(); i++) {
        QDomElement vEl = vList.item(i).toElement();
        if (vEl.attribute("model-id","no") != "no") {
            add(idMap[vEl.attribute("model-id","0")],
                vEl.attribute("x","0").toUInt(),
                vEl.attribute("y","0").toUInt());
        }

    }
}
