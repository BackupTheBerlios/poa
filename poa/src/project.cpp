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
 * $Id: project.cpp,v 1.9 2003/08/28 16:16:58 vanto Exp $
 *
 *****************************************************************************/
#include "blockview.h"
#include "project.h"
#include "gridcanvas.h"
#include "modelfactory.h"
#include "canvasview.h"

Project::Project(QString name)
{
    name_ = name;
}

Project::Project(QString name, QDomDocument *document)
{
    name_ = name;
    deserialize(document);
}

Project::~Project()
{
}

void Project::add(AbstractModel *item)
{
    items_.append(item);
}

QString Project::name()
{
    return name_;
}

GridCanvas *Project::newCanvas(const QString name)
{
    GridCanvas *gc = new GridCanvas(name);
    canvasList_.append(gc);
    return gc;
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
        vElem.setAttribute("name", canvas->name());
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
    QDomNodeList vList = document->elementsByTagName("view");
    for (unsigned int i = 0; i < mList.count(); i++) {
        QDomElement mEl = mList.item(i).toElement();
        AbstractModel *model = ModelFactory::generateSingle(mEl);
        idMap[mEl.attribute("id","0")] = model;
        add(model);
    }

    for (unsigned int i = 0; i < vList.count(); i++) {
        QDomElement vEl = vList.item(i).toElement();
        QDomNodeList viList = vEl.elementsByTagName("view-item");

        GridCanvas *canvas = newCanvas(vEl.attribute("name","name"));

        for (uint j = 0; j < viList.count(); j++) {
            QDomElement viEl = viList.item(j).toElement();
            if (viEl.attribute("model-id","no") != "no") {
                 canvas->addView(idMap[viEl.attribute("model-id","0")],
                    viEl.attribute("x","0").toUInt(),
                    viEl.attribute("y","0").toUInt());
            }
        }
    }
}

