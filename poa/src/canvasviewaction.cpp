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
 * $Id: canvasviewaction.cpp,v 1.1 2003/09/07 19:07:46 squig Exp $
 *
 *****************************************************************************/

#include "canvasviewaction.h"

#include "canvasview.h"

#include <qevent.h>

CanvasViewAction::CanvasViewAction(CanvasView *view)
    : view_(view)
{
}

CanvasViewAction::~CanvasViewAction()
{
}

CanvasView *CanvasViewAction::view()
{
    return view_;
}

void CanvasViewAction::cancel()
{
    delete this;
}

void CanvasViewAction::mouseMoveEvent(QMouseEvent *)
{
}

void CanvasViewAction::mousePressEvent(QMouseEvent *)
{
}

void CanvasViewAction::mouseReleaseEvent(QMouseEvent *)
{
}
