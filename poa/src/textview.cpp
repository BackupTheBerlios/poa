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
 * $Id: textview.cpp,v 1.2 2004/01/22 22:27:47 squig Exp $
 *
 *****************************************************************************/

#include <cmath>

#include "textview.h"

const int TextView::RTTI = 1002;


TextView::TextView(const QString &text, QCanvas *canvas)
    : QCanvasText(text, canvas)
{
}

TextView::TextView(QDomElement element, QCanvas *canvas)
    : QCanvasText(canvas)
{
    deserialize(element);
}

TextView::~TextView()
{
}

void TextView::deserialize(QDomElement element)
{
    setText(element.attribute("text", ""));
}

QPoint TextView::dragBy(int dx, int dy)
{
    // must convert to be consistent with return value
    moveBy(dx, dy);
    return QPoint(dx, dy);
}

QCanvasItem *TextView::item()
{
    return this;
}

void TextView::remove(Project *)
{
    delete this;
}

int TextView::rtti() const
{
    return TextView::RTTI;
}

QDomElement TextView::serialize(QDomDocument *document)
{
    QDomElement root = document->createElement("text-view");
    root.setAttribute("text", text());
    return root;
}
