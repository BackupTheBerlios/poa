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
 * $Id: connectormodel.cpp,v 1.2 2003/08/29 17:53:19 keulsn Exp $
 *
 *****************************************************************************/


#include "connectormodel.h"

#include <qcanvas.h>

#include "abstractmodel.h"
#include "connectorview.h"
#include "pinmodel.h"


ConnectorModel::ConnectorModel(PinModel *source, PinModel *target)
    : AbstractModel(source->name() + "->" + target->name(), QString::null)
{
    source_ = source;
    target_ = target;
}

ConnectorModel::~ConnectorModel()
{
}

QCanvasItemList ConnectorModel::createView(QCanvas *canvas)
{
    QCanvasItemList allViews = canvas->allItems();

    PinView *sourceView = 0;
    PinView *targetView = 0;
    for (QCanvasItemList::iterator current = allViews.begin();
	 current != allViews.end(); ++current) {

	PinView *currentPinView = dynamic_cast<PinView*>(*current);
	if (currentPinView != 0) {
	    PinModel *currentPinModel = currentPinView->model();
	    if (currentPinModel == source_) {
		Q_ASSERT(sourceView == 0);
		sourceView = currentPinView;
	    } else if (currentPinModel == target_) {
		Q_ASSERT(targetView == 0);
		targetView = currentPinView;
	    }
	}
    }
    Q_ASSERT(sourceView != 0 && targetView != 0);

    ConnectorView *view = new ConnectorView(this,
					    sourceView,
					    targetView,
					    canvas);
    return view->allSegments();
}
