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
 * $Id: project.cpp,v 1.20 2003/09/17 16:16:40 vanto Exp $
 *
 *****************************************************************************/
#include "blockview.h"
#include "cpumodel.h"
#include "pinmodel.h"
#include "poa.h"
#include "project.h"
#include "gridcanvas.h"
#include "modelfactory.h"
#include "canvasview.h"
#include "settings.h"

#include <qdom.h>
#include <qdir.h>
#include <qfileinfo.h>

Project::Project(QString path)
    : filename_(QString::null)
{
    currentBlockId_ = 0;

    QDir dir(path);
    name_ = dir.path();
    path_ = path;


    /*    if (document != 0) {
        deserialize(document);
        }*/
}

Project::~Project()
{
}

bool Project::open()
{
    currentBlockId_ = 0;
    blocks_.clear();
    QDir dir(path_);
    QFileInfo fi(dir, QString("project.xml"));
    QFile file(fi.absFilePath());
    if (file.open(IO_ReadOnly)) {
        QDomDocument doc;
        if (doc.setContent(&file)) {
            deserialize(&doc);
            Settings::instance()->addToRecentProjects(path_);
            file.close();
            return true;
        }
        else {
            file.close();
            return false;
        }
        file.close();
    }
    return false;
}

bool Project::save()
{
    //FIX: Fehlerbehandlung
    QDir dir(path_);
    QFileInfo fi(dir, QString("project.xml"));
    QFile file(fi.absFilePath());
    if (file.open(IO_WriteOnly)) {
        QTextStream ts(&file);
        serialize().save(ts, 2);
        file.close();

        return true;
    }
    return false;
}

void Project::addBlock(AbstractModel *item)
{
    if (item->id() == 0) {
        item->setId(++currentBlockId_);
    } else if (item->id() > currentBlockId_) {
        currentBlockId_ = item->id();
    }
    if (INSTANCEOF(item, CpuModel))
    {
        ((CpuModel *)item)->setProjectPath(&path_);
    }

    blocks_.append(item);
}

void Project::createConnectorViews(PinView *source, PinView *target)
{
    GridCanvas *canvas;
    for (QPtrListIterator<GridCanvas> it(canvasList_);
         (canvas = it.current()) != 0; ++it) {
        canvas->addConnectorView(source, target);
    }
}

void Project::createViews(AbstractModel *item, int x, int y)
{
    GridCanvas *canvas;
    for (QPtrListIterator<GridCanvas> it(canvasList_);
         (canvas = it.current()) != 0; ++it) {
        canvas->addView(item, x, y);
    }
}

QString Project::name()
{
    return name_;
}

QString Project::path()
{
    return path_;
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

    QDomElement blist = doc.createElement("blocks");
    mlist.appendChild(blist);

    // create block model list
    AbstractModel *model;
    for (model = blocks_.first(); model; model = blocks_.next()) {
        QDomElement mElem = model->serialize(&doc);
        blist.appendChild(mElem);
    }

    // serialize view list
    GridCanvas *canvas;
    for (canvas = canvasList_.first(); canvas; canvas = canvasList_.next()) {
        QDomElement vElem = doc.createElement("view");
        vElem.setAttribute("name", canvas->name());
        QCanvasItemList canvasItems = canvas->allItems();
        QCanvasItemList::iterator it;
        // serialize views
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

    QDomNodeList mList = document->elementsByTagName("model");
    if (mList.count() != 1) {
        qWarning("not a valid project file");
        return;
    }

    QDomNodeList bList = mList.item(0).toElement().elementsByTagName("blocks");
    if (bList.count() != 1) {
        qWarning("not a valid project file");
        return;
    }

    // create model instances
    QDomElement mEl = bList.item(0).toElement();
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
                canvas->addView(idMap[viEl.attribute("model-id","0").toUInt()],
                    viEl.attribute("x","0").toUInt(),
                    viEl.attribute("y","0").toUInt());
            }
        }
    }
}

