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
 * $Id: project.cpp,v 1.12 2003/08/29 17:59:38 vanto Exp $
 *
 *****************************************************************************/
#include "blockview.h"
#include "project.h"
#include "gridcanvas.h"
#include "modelfactory.h"
#include "canvasview.h"
#include <qdom.h>

Project::Project(QString name)
{
    currentBlockId_ = 0;
    name_ = name;
}

Project::Project(QString name, QDomDocument *document)
{
    name_ = name;
    currentBlockId_ = 0;
    currentConnectorId_ = 0;
    deserialize(document);
}

Project::~Project()
{
}

void Project::addBlock(AbstractModel *item)
{
    if (item->id() == 0) {
        item->setId(++currentBlockId_);
    } else if (item->id() > currentBlockId_) {
        currentBlockId_ = item->id();
    }

    blocks_.append(item);
}

void Project::addConnector(ConnectorModel *item)
{
    if (item->id() == 0) {
        item->setId(++currentConnectorId_);
    } else if (item->id() > currentConnectorId_) {
        currentConnectorId_ = item->id();
    }
    connectors_.append(item);
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
    QDomDocument doc;
    QDomElement proj = doc.createElement("project");
    doc.appendChild(proj);
    QDomElement mlist = doc.createElement("model");
    proj.appendChild(mlist);
    QDomElement vlist = doc.createElement("views");
    proj.appendChild(vlist);

    // create model list
    AbstractModel *model;
    for (model = blocks_.first(); model; model = blocks_.next()) {
        QDomElement mElem = model->serialize(&doc);
        mlist.appendChild(mElem);
    }

    // create view list
    GridCanvas *canvas;
    for (canvas = canvasList_.first(); canvas; canvas = canvasList_.next()) {
        QDomElement vElem = doc.createElement("view");
        vElem.setAttribute("name", canvas->name());
        QCanvasItemList canvasItems = canvas->allItems();
        QCanvasItemList::iterator it;
        // create views
        for (it = canvasItems.begin(); it != canvasItems.end(); ++it) {
            BlockView *bv = (dynamic_cast<BlockView *> (*it));
            if (bv != 0) {
                QDomElement viElem = bv->serialize(&doc);
                viElem.setAttribute("model-id", bv->model()->id());
                vElem.appendChild(viElem);
            }
        }
        vlist.appendChild(vElem);
    }
    return doc;
}

void Project::deserialize(QDomDocument *document) {
    typedef QMap<uint, AbstractModel*> IdMap;
    IdMap idMap;

    // create model instances
    QDomNodeList mList = document->elementsByTagName("model");
    if (mList.count() != 1) {
        qWarning("no valid project file");
        return;
    }

    QDomElement mEl = mList.item(0).toElement();
    QValueList<AbstractModel *> l = ModelFactory::generate(mEl);
    for (QValueList<AbstractModel *>::Iterator it = l.begin();
         it != l.end(); ++it) {
        addBlock(*it);
        // TODO: Tammo review!!!
        idMap[(*it)->id()] = *it;
    }

    // create canvases
    QDomNodeList vList = document->elementsByTagName("view");
    for (unsigned int i = 0; i < vList.count(); i++) {
        QDomElement vEl = vList.item(i).toElement();
        QDomNodeList viList = vEl.elementsByTagName("view-item");

        GridCanvas *canvas = newCanvas(vEl.attribute("name","name"));
        // create view items
        for (uint j = 0; j < viList.count(); j++) {
            QDomElement viEl = viList.item(j).toElement();
            if (viEl.attribute("model-id","no") != "no") {
                canvas->addViewAt(idMap[viEl.attribute("model-id","0").toUInt()],
                    viEl.attribute("x","0").toUInt(),
                    viEl.attribute("y","0").toUInt());
            }
        }
    }
}

