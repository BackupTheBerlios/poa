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
 * $Id: blockview.cpp,v 1.6 2003/08/22 22:47:49 squig Exp $
 *
 *****************************************************************************/
#include "blockview.h"

#include <qbrush.h>
#include <qpainter.h>

BlockView::BlockView(BlockModel *model, QCanvas *canvas)
	: QCanvasRectangle(canvas)
{
    model_ = model;

    setBrush(white);
    setPen(QPen(black, 2));

    unsigned height = 10;
    if (model != 0) {
        // name
        height += BlockView::DEFAULT_FONT_HEIGHT;

        // pins
        unsigned numberOfPins = 0;
		// FIX: where is max declared?
//          numberOfPins = max (model->getInputPins()->size(),
//                              model->getOutputPins()->size());
		numberOfPins = model->getOutputPins()->size();
        numberOfPins += model->getEpisodicPins()->size();
        height += numberOfPins * BlockView::DEFAULT_FONT_HEIGHT;
    }

    setSize(BlockView::DEFAULT_WIDTH, height);
}


BlockView::~BlockView()
{
}


BlockModel *BlockView::getModel()
{
    return model_;
}


void BlockView::setModel(BlockModel *model)
{
    model_ = model;
    // FIX: update views
}


int BlockView::rtti() const
{
    return BlockView::RTTI;
}


void BlockView::drawShape(QPainter &p)
{
    QCanvasRectangle::drawShape(p);
	
	if (model_->name() != QString::null) {
		QRect textArea((int) x(),
					   (int) y() + 3,
					   width(),
					   BlockView::DEFAULT_FONT_HEIGHT);
		p.drawText(textArea, Qt::AlignHCenter, model_->name());
	}
}

