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
 * $Id: blockview.cpp,v 1.8 2003/08/25 17:08:29 keulsn Exp $
 *
 *****************************************************************************/


#include "blockview.h"

#include <qbrush.h>
#include <qcanvas.h>
#include <qpainter.h>

#include "blockmodel.h"
#include "pinvector.h"
#include "pinview.h"


/*unsigned max(unsigned left, unsigned right)
{
    if (left > right) return left;
    else return right;
}*/


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
        unsigned numberOfPins = max (model->inputPins()->size(),
				     model->outputPins()->size());
        numberOfPins += model->episodicPins()->size();
        height += numberOfPins * BlockView::DEFAULT_FONT_HEIGHT;

	// create pin models
	PinVector &leftPinModels = *(model->inputPins());
	PinVector &rightPinModels = *(model->outputPins());
	PinVector &bottomPinModels = *(model->episodicPins());
	for (unsigned i = 0; i < leftPinModels.size(); ++i) {
	    PinView *pinView = leftPinModels[i]->
		createView(this, PinView::PIN_LEFT);
	    leftPins_[i] = pinView;
	}
	for (unsigned i = 0; i < rightPinModels.size(); ++i) {
	    PinView *pinView = rightPinModels[i]->
		createView(this, PinView::PIN_RIGHT);
	    rightPins_[i] = pinView;
	}
	for (unsigned i = 0; i < bottomPinModels.size(); ++i) {
	    PinView *pinView = bottomPinModels[i]->
		createView(this, PinView::PIN_BOTTOM);
	    bottomPins_[i] = pinView;
	}
    }

    setSize(BlockView::DEFAULT_WIDTH, height);
    arrangeVerticalPins();
    // FIX: arrangeHorizontalPins
}


BlockView::~BlockView()
{
}


BlockModel *BlockView::model()
{
    return model_;
}


void BlockView::setModel(BlockModel *model)
{
    model_ = model;
    // FIX: update views
}


void BlockView::moveBy(double dx, double dy)
{
    QCanvasRectangle::moveBy(dx, dy);
    QValueVector<PinView*>::iterator current = leftPins_.begin();
    while (current != leftPins_.end()) {
	(*current)->moveBy(dx, dy);
	++current;
    }
    current = rightPins_.begin();
    while (current != rightPins_.end()) {
	(*current)->moveBy(dx, dy);
	++current;
    }
    current = bottomPins_.begin();
    while (current != bottomPins_.end()) {
	(*current)->moveBy(dx, dy);
	++current;
    }
}


int BlockView::rtti() const
{
    return BlockView::RTTI;
}



void BlockView::drawShape(QPainter &p)
{
    QCanvasRectangle::drawShape(p);
	
    int left = (int) x();
    int right = left + width() - 1;
    int currentY = (int) y() + BlockView::DEFAULT_TOP_SPACING;
    QRect textArea(left,
		   currentY,
		   width(),
		   BlockView::DEFAULT_FONT_HEIGHT);
    if (!(model_->name().isNull())) {
	p.drawText(textArea, Qt::AlignHCenter, model_->name());
    }
    currentY += textArea.height() + BlockView::DEFAULT_HEADER_SPACING;
    p.drawLine(left, currentY, right, currentY);
    currentY += BlockView::DEFAULT_HEADER_SPACING;

    textArea = QRect(left, 
		     currentY,
		     width(),
		     BlockView::DEFAULT_FONT_HEIGHT);

    PinVector *leftPinModels = model_->inputPins();
    unsigned leftSize = leftPinModels->size();
    PinVector *rightPinModels = model_->outputPins();
    unsigned rightSize = rightPinModels->size();
    unsigned slotCount = max(leftSize, rightSize);

    if (slotCount > 0) {
	unsigned pinHeight = height() / slotCount;

	for (unsigned i = 0; i < slotCount; ++i) {
	    
	    if (i < leftSize) {
		p.drawText(textArea,
			   Qt::AlignLeft,
			   leftPinModels->at(i)->name());
	    }
	    textArea.moveBy(0, pinHeight);
	    
	    if (i < rightSize) {
		p.drawText(textArea,
			   Qt::AlignRight,
			   rightPinModels->at(i)->name());
	    }
	    textArea.moveBy(0, pinHeight);
	}
    } else {
	Q_ASSERT(slotCount == 0);
    }
}


void BlockView::arrangeVerticalPins()
{
    int top = ((int) y()) + BlockView::DEFAULT_TOP_SPACING +
	BlockView::DEFAULT_FONT_HEIGHT +
	BlockView::DEFAULT_HEADER_SPACING;
    int bottom = ((int) y()) + height() - BlockView::DEFAULT_BOTTOM_SPACING;
	
    unsigned leftSize = leftPins_.size();
    unsigned rightSize = rightPins_.size();
    unsigned slotCount = max(leftSize, rightSize);

    if (slotCount > 0 && bottom > top) {
	unsigned height = (bottom - top + 1) / slotCount;
	top += BlockView::DEFAULT_FONT_HEIGHT / 2;

	for (unsigned i = 0; i < slotCount; ++i) {
	    
	    if (i < leftSize) {
		QRect pinBounds = leftPins_[i]->boundingRect();
		leftPins_[i]->setX(x() - pinBounds.width() - 1);
		leftPins_[i]->setY(top - pinBounds.height() / 2);
	    }
	    top += height;
	    
	    if (i < rightSize) {
		QRect pinBounds = rightPins_[i]->boundingRect();
		rightPins_[i]->setX(x() + pinBounds.width());
		rightPins_[i]->setY(top - pinBounds.height() / 2);
	    }
	    top += height;
	}
    } else {
	Q_ASSERT(slotCount == 0);
    }
}
