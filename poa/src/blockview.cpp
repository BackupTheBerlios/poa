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
 * $Id: blockview.cpp,v 1.1 2003/08/22 13:58:45 keulsn Exp $
 *
 *****************************************************************************/


#include "blockview.h"


//static const int BlockView::RTTI_ = 1000;


BlockView::BlockView(BlockModel *model, QCanvas *canvas):
    QCanvasRectangle::QCanvasRectangle(QRect(0, 0, 15, 60), canvas)
{
    blockModel_ = model;
}


BlockView::~BlockView()
{
}


BlockModel *BlockView::getModel()
{
    return blockModel_;
}


void BlockView::setModel(BlockModel *model)
{
    blockModel_ = model;
}


int BlockView::rtti() const
{
    return RTTI_;
}


void BlockView::drawShape(QPainter &p)
{
    p.fillRect((int) x(), (int) y(), width(), height(), brush());
}
