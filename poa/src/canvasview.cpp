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
 * $Id: canvasview.cpp,v 1.19 2003/08/29 21:27:46 vanto Exp $
 *
 *****************************************************************************/


#include "canvasview.h"

#include "abstractmodel.h"
#include "abstractview.h"
#include "blockview.h"
#include "connectormodel.h"
#include "cpumodel.h"
#include "pinmodel.h"
#include "pinview.h"
#include "poa.h"
#include "project.h"
#include "mainwindow.h"
#include "modelfactory.h"

#include <qvariant.h>
#include <qapplication.h>
#include <qdom.h>
#include <qpoint.h>
#include <qwmatrix.h>
#include <qpainter.h>
#include <qpopupmenu.h>
#include <qstatusbar.h>
#include <qtooltip.h>


CanvasView::CanvasView(Project *project, QCanvas *canvas, QWidget *parent,
                       const char* name, WFlags fl)
    : QCanvasView(canvas, parent, name, fl), project_(project),
    movingItem_(0), selectedItem_(0)
{
    setAcceptDrops(TRUE);
}

CanvasView::~CanvasView()
{
    // no need to delete child widgets, Qt does it all for us
}

Project *CanvasView::project()
{
    return project_;
}

void CanvasView::contentsMousePressEvent(QMouseEvent *e)
{
    QPoint p = inverseWorldMatrix().map(e->pos());
    QCanvasItemList l = canvas()->collisions(p);
    if (e->button() == LeftButton) {
        if (!l.isEmpty()) {
            // first item is top item
            QCanvasItem *topItem = l.first();

            // item selection
            if (selectedItem_) {
                if (selectedItem_ != topItem) {
                    // deselect old item
                    selectedItem_->setSelected(false);
                    // select new item
                    selectedItem_ = topItem;
                    selectedItem_->setSelected(true);
                }
            } else {
                // select new item
                selectedItem_ = topItem;
                selectedItem_->setSelected(true);
            }
            canvas()->update();

            // move blocks
            if (INSTANCEOF(topItem, BlockView)) {
                movingItem_ = topItem;
                movingStartPoint_ = p;
            }
        } else {
            // nirvana click -> deselect
            if (selectedItem_) {
                selectedItem_->setSelected(false);
                canvas()->update();
                selectedItem_ = 0;
            }
        }
    }
    else if (e->button() == RightButton) {
        if (!l.isEmpty()) {
            QCanvasItem *topItem = l.first();
            if (INSTANCEOF(topItem, PinView)) {
                // item is a pinview
                if (INSTANCEOF(selectedItem_, PinView)) {
                    // create connector
                    PinView *source = dynamic_cast<PinView *>(selectedItem_);
                    PinView *target = dynamic_cast<PinView *>(topItem);

                    ConnectorModel *cm = new ConnectorModel(source->model(), target->model());
                    // add connector to project
                    project_->addConnector(cm);
                    ((GridCanvas *)canvas())->addView(cm);

                    qWarning("Connect: "+QString(source->model()->parent()->name())+":"+QString(source->model()->name())+"\n"
                             +"to "+ QString(target->model()->parent()->name())+":"+QString(target->model()->name()));
                    // deselect selected item
                    selectedItem_->setSelected(false);
                    selectedItem_ = 0;
                    canvas()->update();
                }
            } else {
                // item is not a pin view
                AbstractView *item = dynamic_cast<AbstractView *>(l.first());
                // item may be 0 if !INSTANCEOF(l.first(), AbstractView)
                if (item != 0) {
                    // item is a AbstractView -> show popup menu
                    QPopupMenu *menu = item->popupMenu();
                    if (menu) {
                        menu->exec(mapToGlobal(e->pos()));
                    }
                }
            }
        }
    }
}

void CanvasView::contentsMouseReleaseEvent(QMouseEvent *)
{
    movingItem_ = 0;
}

void CanvasView::contentsMouseMoveEvent(QMouseEvent *e)
{
    if (movingItem_ != 0) {
        QPoint p = inverseWorldMatrix().map(e->pos());
        movingItem_->moveBy(p.x() - movingStartPoint_.x(),
                            p.y() - movingStartPoint_.y());
        movingStartPoint_ = p;
        canvas()->update();
    }

    QPoint pos = toCanvas(e->pos());
    ((MainWindow *)qApp->mainWidget())->statusBar()->message
        (QString::number(pos.x()) + ":" + QString::number(pos.y()));
}

void CanvasView::dragEnterEvent(QDragEnterEvent *e)
{
    e->accept(e->provides("text/xml"));
}

void CanvasView::dropEvent(QDropEvent *e)
{
    QByteArray data = e->encodedData("text/xml");
    if (data) {
        QDomDocument doc;
        if (doc.setContent(QString(data))) {
            QPoint pos = toCanvas(e->pos());
            QValueList<AbstractModel *> l = ModelFactory::generate(doc);
            for (QValueList<AbstractModel *>::Iterator it = l.begin();
                 it != l.end(); ++it) {
                project_->addBlock(*it);
                ((GridCanvas *)canvas())->addViewAt(*it, pos.x(), pos.y());
            }
        }
    }
}


QPoint CanvasView::toCanvas(QPoint pos)
{
    return inverseWorldMatrix().map(viewportToContents(pos));
}
