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
 * $Id: project.cpp,v 1.47 2004/01/22 00:05:06 squig Exp $
 *
 *****************************************************************************/
#include "blockview.h"
#include "canvasview.h"
#include "connectorviewlist.h"
#include "connectorviewsegment.h"
#include "cpumodel.h"
#include "gridcanvas.h"
#include "modelfactory.h"
#include "pinmodel.h"
#include "poa.h"
#include "project.h"
#include "settings.h"

#include <qdir.h>
#include <qdom.h>
#include <qfileinfo.h>

#include "poaexception.h"

Project::Project(QString path)
    : filename_(QString::null)
{
    currentBlockId_ = 0;

    modified_ = false;

    setPath(path);
}

Project::~Project()
{
    AbstractModel *model;
    for ( model = blocks_.first(); model; model = blocks_.next() ) {
        delete model;
    }

    GridCanvas *canvas;
    for (canvas = canvasList_.first(); canvas; canvas = canvasList_.next()) {
        delete canvas;
    }
}

void Project::open()
{
    currentBlockId_ = 0;
    blocks_.clear();
    QDir dir(path_);
    QFileInfo fi(dir, QString("project.xml"));
    QFile file(fi.absFilePath());
    if (file.open(IO_ReadOnly)) {
        QDomDocument doc;
        if (doc.setContent(&file)) {
            //            try {
                deserialize(&doc);
                //            } catch (std::runtime_error) {
                //                throw;
                //            }
            Settings::instance()->addToRecentProjects(path_);
            //            file.close();
            setModified(false);
        } else {
            throw PoaException(tr("Could not parse project file in %1").arg(path_));
        }
        file.close();
    } else {
        throw PoaException(tr("Could not open project file in %1.").arg(path_));
    }
}

void Project::save()
{
    //FIX: Fehlerbehandlung
    QDir dir(path_);
    QFileInfo fi(dir, QString("project.xml"));
    QFile file(fi.absFilePath());
    if (file.open(IO_WriteOnly)) {
        QTextStream ts(&file);
        serialize().save(ts, 2);
        file.close();
        setModified(false);
    } else {
        throw PoaException(tr("Could not save project %1.").arg(name_));
    }
}

void Project::saveAs(QString path)
{
    setPath(path);
    save();

    Settings::instance()->addToRecentProjects(path_);
}

QString Project::projectPath()
{
    return path_;
}

void Project::addBlock(AbstractModel *item)
{
    if (item->id() == 0) {
        item->setId(++currentBlockId_);
    }
    else if (item->id() > currentBlockId_) {
        currentBlockId_ = item->id();
    }
    item->setProject(this);

    blocks_.append(item);
    setModified(true);
}

void Project::removeBlock(AbstractModel *item)
{
    blocks_.remove(item);
    delete item;
    setModified(true);
}

void Project::createConnectorViews(PinView *source, PinView *target)
{
    GridCanvas *canvas;
    for (QPtrListIterator<GridCanvas> it(canvasList_);
         (canvas = it.current()) != 0; ++it) {
        ConnectorViewList *viewList
            = new ConnectorViewList(source, target, canvas);
        canvas->addConnectorView(viewList);
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

GridCanvas *Project::newCanvas(const QString name)
{
    GridCanvas *gc = new GridCanvas(name);
    canvasList_.append(gc);
    return gc;
}

const QPtrList<GridCanvas> *Project::canvasList() const {
    return &canvasList_;
}

QPtrList<AbstractModel> *Project::blocks()
{
    return &blocks_;
}

void Project::setModified(bool mod)
{
    modified_ = mod;
    emit modified(mod);
}

void Project::setPath(QString path)
{
    QDir dir(path);
    name_ = dir.path();
    path_ = dir.canonicalPath();
}

bool Project::isModified()
{
    return modified_;
}

QDomDocument Project::serialize()
{
    typedef QMap<ConnectorViewList *, bool> ConSerMap;
    ConSerMap serialized;
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
            ConnectorViewSegment *seg = (dynamic_cast<ConnectorViewSegment *> (*it));
            if (seg != 0 && !serialized[seg->viewList()]) {
                QDomElement conElem = seg->viewList()->serialize(&doc);
                vElem.appendChild(conElem);
                serialized[seg->viewList()] = true;
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
        throw PoaException(tr("Project could not be loaded. %1 does not contain a valid project file.").arg(name_));
    }

    QDomNodeList bList = mList.item(0).toElement().elementsByTagName("blocks");
    if (bList.count() != 1) {
        throw PoaException(tr("Project could not be loaded. %1 does not contain a valid project file.").arg(name_));
    }

    // create model instances
    QDomElement mEl = bList.item(0).toElement();
    QValueList<AbstractModel *> l = ModelFactory::generate(mEl);
    for (QValueList<AbstractModel *>::Iterator it = l.begin();
         it != l.end(); ++it) {
        addBlock(*it);
        idMap[(*it)->id()] = *it;
    }

    // create canvases
    QDomNodeList vList = document->elementsByTagName("view");
    for (unsigned int i = 0; i < vList.count(); i++) {
                uint j;
        QDomElement vEl = vList.item(i).toElement();
        QDomNodeList viList = vEl.elementsByTagName("view-item");
        QDomNodeList conList = vEl.elementsByTagName("connector-view");

        GridCanvas *canvas = newCanvas(vEl.attribute("name","name"));
        // create view items
        for (j = 0; j < viList.count(); j++) {
            QDomElement viEl = viList.item(j).toElement();
            if (viEl.attribute("model-id","no") != "no") {
                if (idMap[viEl.attribute("model-id","0").toUInt()] == 0) {
                    qDebug(tr("ModelViewMapping failed: View with id %1 has no model").arg(viEl.attribute("model-id","0")));
                    throw PoaException(tr("The project file contains inconsistent data. Project %1 could not be loaded.").arg(name_));
                }
                canvas->addView(idMap[viEl.attribute("model-id","0").toUInt()],
                    viEl.attribute("x","0").toUInt(),
                    viEl.attribute("y","0").toUInt());
            }
        }

        // create connector views
        for (j = 0; j < conList.count(); j++) {
            QDomElement conEl = conList.item(j).toElement();

            AbstractModel *sb = dynamic_cast<AbstractModel*>(
                idMap[conEl.attribute("source-block","0").toUInt()]);
            AbstractModel *tb = dynamic_cast<AbstractModel*>(
                idMap[conEl.attribute("target-block","0").toUInt()]);

            if (sb == 0 && tb == 0) {
                throw PoaException(tr("Could not load the project. %1 does not contain a valid project file.").arg(name_));
            }

            uint sid = conEl.attribute("source-pin","0").toUInt();
            uint tid = conEl.attribute("target-pin","0").toUInt();

            PinView *spv = 0;
            PinView *tpv = 0;

            QCanvasItemList canvasItems = canvas->allItems();
            QCanvasItemList::iterator it;

            for (it = canvasItems.begin(); it != canvasItems.end(); ++it) {
                PinView *pv = dynamic_cast<PinView*>(*it);
                if (pv != 0 && pv->parent()->model() == sb
                    && pv->model()->id() == sid) {

                    spv = pv;
                }
                if (pv != 0 && pv->parent()->model() == tb
                    && pv->model()->id() == tid) {

                    tpv = pv;
                }
            }

            //Q_ASSERT(spv != 0 && tpv!=0);

            if (spv != 0 && tpv != 0) {
                spv->model()->attach(tpv->model());
                ConnectorViewList *vl = new ConnectorViewList(spv, tpv, canvas);
                canvas->addConnectorView(vl);
                vl->deserialize(conEl);
            } else {
                qDebug(tr("Could not create connector view because one of the pinmodels is invalid."));
            }
        }
    }
}

