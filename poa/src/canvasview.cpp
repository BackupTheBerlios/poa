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
 * $Id: canvasview.cpp,v 1.40 2003/09/23 17:10:59 garbeam Exp $
 *
 *****************************************************************************/

#include "canvasview.h"

#include "abstractmodel.h"
#include "abstractview.h"
#include "blockview.h"
#include "blockconfdialog.h"
#include "canvasviewaction.h"
#include "connectaction.h"
#include "connectorviewsegment.h"
#include "cpumodel.h"
#include "pinmodel.h"
#include "pinview.h"
#include "poa.h"
#include "project.h"
#include "mainwindow.h"
#include "modelfactory.h"
#include "moveaction.h"
#include "muxconfdialog.h"
#include "muxmodel.h"
#include "settings.h"

#include <qvariant.h>
#include <qaction.h>
#include <qapplication.h>
#include <qcursor.h>
#include <qdom.h>
#include <qpoint.h>
#include <qwmatrix.h>
#include <qpainter.h>
#include <qpopupmenu.h>
#include <qstatusbar.h>
#include <qtooltip.h>

const int AUTOSCROLL_MARGIN = 16;

CanvasView::CanvasView(Project *project, GridCanvas *canvas, QWidget *parent,
                       const char* name, WFlags fl)
    : QCanvasView(canvas, parent, name, fl), project_(project), action_(0)
{
    setAcceptDrops(true);
    setDragAutoScroll(true);
    tooltip_ = new CanvasToolTip(this);

    backgroundPopupMenu = new QPopupMenu(this);
    MainWindow::instance()->showGridAction()->addTo(backgroundPopupMenu);
    backgroundPopupMenu->insertSeparator();
    MainWindow::instance()->pasteAction()->addTo(backgroundPopupMenu);

    blockViewPopupMenu = new QPopupMenu(this);
    MainWindow::instance()->blockConfAction()->addTo(blockViewPopupMenu);
    blockViewPopupMenu->insertSeparator();
    MainWindow::instance()->cutAction()->addTo(blockViewPopupMenu);
    MainWindow::instance()->copyAction()->addTo(blockViewPopupMenu);
    MainWindow::instance()->pasteAction()->addTo(blockViewPopupMenu);
    blockViewPopupMenu->insertSeparator();
    MainWindow::instance()->removeAction()->addTo(blockViewPopupMenu);

    pinViewPopupMenu = new QPopupMenu(this);

    connectorViewPopupMenu = new QPopupMenu(this);
    MainWindow::instance()->removeAction()->addTo(connectorViewPopupMenu);
}

CanvasView::~CanvasView()
{
    delete tooltip_;
    // no need to delete child widgets, Qt does it all for us
}

GridCanvas *CanvasView::gridCanvas()
{
    return static_cast<GridCanvas *>(canvas());
}

Project *CanvasView::project()
{
    return project_;
}

void CanvasView::contentsMouseDoubleClickEvent(QMouseEvent *e)
{
    if (action_) {
        action_->mousePressEvent(e);
        return;
    }

    QPoint p = inverseWorldMatrix().map(e->pos());
    QCanvasItemList l = canvas()->collisions(p);
    QCanvasItem *topItem = l.isEmpty() ? 0 : l.first();

    if (topItem != 0) {
        selectItem(topItem);

        if (INSTANCEOF(topItem, BlockView)) {
            MainWindow::instance()->openBlockConf();
        }
    }
    else {
        // nirvana click
        deselectAll();
        canvas()->update();
    }

}

void CanvasView::contentsMousePressEvent(QMouseEvent *e)
{
    if (action_) {
        action_->mousePressEvent(e);
        return;
    }

    QPoint p = inverseWorldMatrix().map(e->pos());
    QCanvasItemList l = canvas()->collisions(p);
    if (e->button() == LeftButton) {
        if (!l.isEmpty()) {
            // first item is top item
            QCanvasItem *topItem = l.first();

            selectItem(topItem);

            // notify item that is has been clicked
            AbstractView *item = dynamic_cast<AbstractView*>(topItem);
            if (item != 0 && item->isDraggable()) {
                setAction(new MoveAction(this, e, topItem));
            }

            PinView *pinItem = dynamic_cast<PinView*>(topItem);
            if (pinItem != 0) {
                setAction(new ConnectAction(this, e, pinItem));
            }

            canvas()->update();
        }
        else {
            // nirvana click
            deselectAll();
            canvas()->update();
        }
    }
    else if (e->button() == RightButton) {
        if (!l.isEmpty()) {
            // first item is top item
            QCanvasItem *topItem = l.first();

            selectItem(topItem);
            canvas()->update();

            if (INSTANCEOF(topItem, BlockView)) {
                blockViewPopupMenu->exec
                    (contentsToViewport(mapToGlobal(e->pos())));
            }
            else if (INSTANCEOF(topItem, PinView)) {
//                 pinViewPopupMenu->exec
//                     (contentsToViewport(mapToGlobal(e->pos())));
            }
            else if (INSTANCEOF(topItem, ConnectorViewSegment)) {
                connectorViewPopupMenu->exec
                    (contentsToViewport(mapToGlobal(e->pos())));
            }
        }
        else {
            backgroundPopupMenu->exec
                (contentsToViewport(mapToGlobal(e->pos())));
        }
    }
}

void CanvasView::contentsMouseReleaseEvent(QMouseEvent *e)
{
    if (action_) {
        action_->mouseReleaseEvent(e);
    }
}

void CanvasView::contentsMouseMoveEvent(QMouseEvent *e)
{
    QPoint pos = toCanvas(e->pos());
    ((MainWindow *)qApp->mainWidget())->statusBar()->message
        (QString::number(pos.x()) + ":" + QString::number(pos.y()));

    doAutoScroll();

    if (action_) {
        action_->mouseMoveEvent(e);
    }
}

void CanvasView::deselectAll()
{
    QCanvasItemList allItems = canvas()->allItems();
    for (QCanvasItemList::iterator current = allItems.begin();
         current != allItems.end(); ++current) {

        (*current)->setSelected(false);
    }
    emit(selectionChanged(false));
}

void CanvasView::doAutoScroll()
{
    QPoint p = mapFromGlobal(QCursor::pos());

    int dx = 0, dy = 0;
    if (p.y() < AUTOSCROLL_MARGIN) {
        dy = -1;
    }
    else if (p.y() > visibleHeight() - AUTOSCROLL_MARGIN) {
        dy = +1;
    }
    if ( p.x() < AUTOSCROLL_MARGIN ) {
        dx = -1;
    } else if ( p.x() > visibleWidth() - AUTOSCROLL_MARGIN) {
        dx = +1;
    }
    if ( dx || dy ) {
        scrollBy(dx,dy);
    }
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
                project()->addBlock(*it);
                gridCanvas()->addView(*it, pos.x(), pos.y());

                if (INSTANCEOF(*it, BlockModel)) {
                    BlockConfDialog *dialog =
                        new BlockConfDialog((BlockModel *)(*it));
                    dialog->exec();
                    delete dialog;
                }
                else if (INSTANCEOF(*it, MuxModel)) {
                    MuxConfDialog *dialog =
                        new MuxConfDialog();
                    dialog->exec();
                    delete dialog;
                }
            }
        }
    }
}

void CanvasView::selectItem(QCanvasItem *item)
{
    deselectAll();
    item->setSelected(true);
    emit(selectionChanged(item));
}

QCanvasItemList CanvasView::selectedItems()
{
    QCanvasItemList selectedItems;
    QCanvasItemList allItems = canvas()->allItems();
    for (QCanvasItemList::iterator current = allItems.begin();
         current != allItems.end(); ++current) {

        if ((*current)->isSelected()) {
            selectedItems.append(*current);
        }
    }
    return selectedItems;
}

void CanvasView::setAction(CanvasViewAction *action)
{
    if (action_) {
        action_->cancel();
    }
    action_ = action;

    // FIX: this call should go somewhere else
    canvas()->update();
}

QPoint CanvasView::toCanvas(QPoint pos)
{
    return inverseWorldMatrix().map(viewportToContents(pos));
}

CanvasToolTip::CanvasToolTip(QCanvasView *parent)
    : QToolTip(parent)
{
    // no explicit initialization needed
}

void CanvasToolTip::maybeTip(const QPoint &pos)
{
    QCanvasView *view = static_cast<QCanvasView *>(parentWidget());

    QPoint p = view->inverseWorldMatrix().map(view->viewportToContents(pos));
    QCanvasItemList l = view->canvas()->collisions(p);
    if (!l.isEmpty()) {
        // first item is top item
        QCanvasItem *topItem = l.first();
        Tooltipable *item = dynamic_cast<Tooltipable*>(topItem);
        if (item != 0) {
            // reverse coordinate conversion
            QRect r = view->worldMatrix().map(topItem->boundingRect());
            QRect target
                = QRect(view->contentsToViewport(r.topLeft()),
                        view->contentsToViewport(r.bottomRight()));
            tip(target, item->tip());
        }
    }
}
