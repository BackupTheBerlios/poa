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
 * $Id: pixmapbutton.cpp,v 1.1 2004/01/19 21:14:30 squig Exp $
 *
 *****************************************************************************/

#include "pixmapbutton.h"

PixmapButton::PixmapButton(const QIconSet & icon, QWidget *parent,
                           const char *name)
    : QPushButton(icon, "", parent, name)
{
}

QSize PixmapButton::sizeHint() const
{
    if (iconSet() && !iconSet()->isNull() && text().isEmpty()) {
        int w = iconSet()->pixmap(QIconSet::Small, QIconSet::Normal).width() + 4;
        int h = iconSet()->pixmap(QIconSet::Small, QIconSet::Normal).height();
        return QSize(w + 10, h + 10);
    }
    else {
        return QPushButton::sizeHint();
    }
}
