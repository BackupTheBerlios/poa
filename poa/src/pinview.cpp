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
 * $Id: pinview.cpp,v 1.1 2003/08/26 00:19:03 keulsn Exp $
 *
 *****************************************************************************/


#include "pinview.h"

#include "blockview.h"
#include "pinmodel.h"


PinView::PinView(PinModel *model,
		 BlockView *block,
		 PinView::DockPosition dockPosition)
    : QCanvasRectangle(block->canvas())
{
    model_ = model;
}


PinView::~PinView()
{
}

void PinView::setModel(PinModel *model)
{
    model_ = model;
    // FIX: update views
}


PinModel *PinView::model()
{
    return model_;
}

