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
 * $Id: blockview.cpp,v 1.16 2003/08/29 14:34:41 vanto Exp $
 *
 *****************************************************************************/


#include "blockview.h"

#include <qbrush.h>
#include <qcanvas.h>
#include <qpopupmenu.h>
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

    unsigned height = BlockView::DEFAULT_TOP_SPACING +
    BlockView::DEFAULT_HEADER_SPACING +
    BlockView::DEFAULT_HEADER_SPACING;

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
        leftPins_.insert(leftPins_.end(), pinView);
    }
    for (unsigned i = 0; i < rightPinModels.size(); ++i) {
        PinView *pinView = rightPinModels[i]->
        createView(this, PinView::PIN_RIGHT);
        rightPins_.insert(rightPins_.end(), pinView);
    }
    for (unsigned i = 0; i < bottomPinModels.size(); ++i) {
        PinView *pinView = bottomPinModels[i]->
        createView(this, PinView::PIN_BOTTOM);
        bottomPins_.insert(bottomPins_.end(), pinView);
    }
    }

    height += BlockView::DEFAULT_BOTTOM_SPACING;
    setSize(BlockView::DEFAULT_WIDTH, height);
    arrangeVerticalPins();
    // FIX: arrangeHorizontalPins
}


BlockView::~BlockView()
{
}


void BlockView::addPinViewsTo(QCanvasItemList &list)
{
    for (QValueVector<PinView*>::iterator current = leftPins_.begin();
     current != leftPins_.end(); ++current) {

    list.prepend(*current);
    }
    for (QValueVector<PinView*>::iterator current = rightPins_.begin();
     current != rightPins_.end(); ++current) {

    list.prepend(*current);
    }
    for (QValueVector<PinView*>::iterator current = bottomPins_.begin();
     current != bottomPins_.end(); ++current) {

    list.prepend(*current);
    }
}


BlockModel *BlockView::model()
{
    return model_;
}


QPopupMenu *BlockView::popupMenu()
{
    QPopupMenu *pm = new QPopupMenu();
    pm->insertItem("Configure...");
    pm->insertSeparator();
    pm->insertItem("Edit Sourcecode...");
    return pm;
}
/*
void BlockView::setModel(BlockModel *model)
{
    model_ = model;
    // FIX: update views
}*/


void BlockView::moveBy(double dx, double dy)
{
    QCanvasRectangle::moveBy(dx, dy);
    arrangeVerticalPins();
    /*    QValueVector<PinView*>::iterator current = leftPins_.begin();
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
    }*/
}


int BlockView::rtti() const
{
    return BlockView::RTTI;
}


void BlockView::setSelected(bool yes)
{
    if (yes) {
        setBrush(red);
    } else {
        setBrush(white);
    }
    QCanvasRectangle::setSelected(yes);

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
    if (model_->name() != 0) {
    p.drawText(textArea, Qt::AlignHCenter, model_->name());
    }
    currentY += textArea.height() + BlockView::DEFAULT_HEADER_SPACING;
    p.drawLine(left, currentY, right, currentY);
    currentY += BlockView::DEFAULT_HEADER_SPACING;

    textArea = QRect(left + BlockView::DEFAULT_LEFT_BORDER,
             currentY,
             width() - BlockView::DEFAULT_LEFT_BORDER
                     - BlockView::DEFAULT_RIGHT_BORDER,
             BlockView::DEFAULT_FONT_HEIGHT);

    PinVector *leftPinModels = model_->inputPins();
    unsigned leftSize = leftPinModels->size();
    PinVector *rightPinModels = model_->outputPins();
    unsigned rightSize = rightPinModels->size();
    unsigned slotCount = max(leftSize, rightSize);

    if (slotCount > 0) {
    unsigned pinHeight = (height() - BlockView::DEFAULT_TOP_SPACING
                           - BlockView::DEFAULT_FONT_HEIGHT
                           - BlockView::DEFAULT_HEADER_SPACING
                           - BlockView::DEFAULT_HEADER_SPACING
                           - BlockView::DEFAULT_BOTTOM_SPACING)
        / slotCount;

    for (unsigned i = 0; i < slotCount; ++i) {

        if (i < leftSize) {
        p.drawText(textArea,
               Qt::AlignLeft,
               leftPinModels->at(i)->name());
        }

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
    BlockView::DEFAULT_HEADER_SPACING +
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
        QSize pinSize = leftPins_[i]->size();
        leftPins_[i]->move(x() - pinSize.width() + 1,
                   top - pinSize.height() / 2);
        }

        if (i < rightSize) {
        QSize pinSize = rightPins_[i]->size();
        rightPins_[i]->move(x() + width() - 1,
                    top - pinSize.height() / 2);
        }
        top += height;
    }
    } else {
    Q_ASSERT(slotCount == 0);
    }
}

QDomElement BlockView::serialize(QDomDocument *document)
{
    QDomElement root = document->createElement("view-item");
    root.setAttribute("x", x());
    root.setAttribute("y", y());
    return root;
}

void BlockView::deserialize(QDomElement element)
{
    setX(element.attribute("x","0").toDouble());
    setY(element.attribute("y","0").toDouble());
}
