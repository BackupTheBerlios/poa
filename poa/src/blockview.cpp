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
 * $Id: blockview.cpp,v 1.37 2003/09/24 09:09:11 garbeam Exp $
 *
 *****************************************************************************/


#include "blockview.h"

#include "blockconfdialog.h"
#include "blockmodel.h"
#include "canvasview.h"
#include "mainwindow.h"
#include "muxmodel.h"
#include "pinmodel.h"
#include "pinvector.h"
#include "pinview.h"
#include "poa.h"
#include "settings.h"
#include "util.h"

#include <qaction.h>
#include <qbrush.h>
#include <qcanvas.h>
#include <qpainter.h>


int BlockView::DEFAULT_FONT_HEIGHT = 12;


/*unsigned max(unsigned left, unsigned right)
  {
  if (left > right) return left;
  else return right;
  }*/


BlockView::BlockView(AbstractModel *model, QCanvas *canvas)
    : QCanvasRectangle(canvas)
{
    model_ = model;

    BlockView::DEFAULT_FONT_HEIGHT = QFontMetrics(QApplication::font()).height();

    setBrush(white);
    setPen(QPen(Settings::instance()->defaultColor(), 2));

    if (INSTANCEOF(model_, BlockModel)) {
        BlockModel *blockModel = (BlockModel *)model_;
        connect(blockModel, SIGNAL(pinAdded(PinModel *)),
                this, SLOT(addPin(PinModel *)));
        connect(blockModel, SIGNAL(updated()), this, SLOT(updateView()));
        connect(blockModel, SIGNAL(deleted()), this, SLOT(deleteView()));

        // create pin views
        addPins(*blockModel->inputPins());
        addPins(*blockModel->outputPins());
        addPins(*blockModel->episodicPins());
    }
    else if (INSTANCEOF(model_, MuxModel)) {
        MuxModel *muxModel = (MuxModel *)model_;
        connect(muxModel, SIGNAL(deleted()), this, SLOT(deleteView()));
    }

    arrangePins();
}


BlockView::~BlockView()
{
}


bool BlockView::isDraggable()
{
    return true;
}


QSize BlockView::dragBy(double dx, double dy)
{
    // must convert to be consistent with return value
    moveBy((double) (int) dx, (double) (int) dy);
    return QSize((int) dx, (int) dy);
}

void BlockView::addPin(PinModel *pin)
{
    PinView *pinView;
    switch (pin->type()) {
    case PinModel::INPUT:
        pinView = pin->createView(this, PinView::PIN_LEFT);
        break;
    case PinModel::OUTPUT:
        pinView = pin->createView(this, PinView::PIN_RIGHT);
        break;
    case PinModel::EPISODIC:
        pinView = pin->createView(this, PinView::PIN_BOTTOM);
        break;
    }
    pinList(pinView)->append(pinView);
}

void BlockView::addPins(const PinVector &pins)
{
    for (unsigned i = 0; i < pins.size(); ++i) {
        addPin(pins[i]);
    }
}

void BlockView::addPinViewsTo(QCanvasItemList &list)
{
    for (QValueList<PinView*>::iterator current = leftPins_.begin();
         current != leftPins_.end(); ++current) {

        list.prepend(*current);
    }
    for (QValueList<PinView*>::iterator current = rightPins_.begin();
         current != rightPins_.end(); ++current) {

        list.prepend(*current);
    }
    for (QValueList<PinView*>::iterator current = bottomPins_.begin();
         current != bottomPins_.end(); ++current) {

        list.prepend(*current);
    }
}

void BlockView::arrangePins()
{
    // calculate height
    unsigned height
        = BlockView::DEFAULT_TOP_SPACING
        + BlockView::DEFAULT_FONT_HEIGHT // name
        + BlockView::DEFAULT_HEADER_SPACING
        // separator
        + BlockView::DEFAULT_HEADER_SPACING
        // horizontal pins
        + BlockView::DEFAULT_HEADER_SPACING
        // vertical pins
        + BlockView::DEFAULT_BOTTOM_SPACING;
    // horizontal pins
    unsigned numberOfPins = QMAX(leftPins_.size(), rightPins_.size());
    height += numberOfPins * BlockView::DEFAULT_FONT_HEIGHT;
    // vertical pins
    if (bottomPins_.size() > 0) {
        height += DEFAULT_FONT_HEIGHT;
    }

    // calculate width
    int width
        = DEFAULT_LEFT_BORDER
        + bottomPins_.size() * (DEFAULT_LABEL_WIDTH + DEFAULT_LABEL_SPACING)
        + DEFAULT_RIGHT_BORDER;
    width = QMAX(BlockView::DEFAULT_WIDTH, width);

    // update view
    setSize(width, height);

    arrangeVerticalPins();
    arrangeHorizontalPins();
}

AbstractModel *BlockView::model()
{
    return model_;
}

void BlockView::moveBy(double dx, double dy)
{
    QCanvasRectangle::moveBy(dx, dy);
    arrangePins();

    /*    QValueList<PinView*>::iterator current = leftPins_.begin();
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

QValueList<PinView*> *BlockView::pinList(PinView *pin)
{
    switch (pin->pinModel()->type()) {
    case PinModel::INPUT:
        return &leftPins_;
    case PinModel::OUTPUT:
        return &rightPins_;
    case PinModel::EPISODIC:
        return &bottomPins_;
    default:
        return 0;
    }
}

int BlockView::rtti() const
{
    return BlockView::RTTI;
}


void BlockView::setSelected(bool yes)
{
    QCanvasRectangle::setSelected(yes);
    setZ(static_cast<GridCanvas *>(canvas())->incZ());
    updateProperties();
}

void BlockView::setZ(double z)
{
    QCanvasRectangle::setZ(z);

    QValueListIterator<PinView *> current = leftPins_.begin();
    while (current != leftPins_.end()) {
        (*current)->setZ(z);
        ++current;
    }
    current = rightPins_.begin();
    while (current != rightPins_.end()) {
        (*current)->setZ(z);
        ++current;
    }
    current = bottomPins_.begin();
    while (current != bottomPins_.end()) {
        (*current)->setZ(z);
        ++current;
    }
}

void BlockView::updateProperties()
{
    if (isActive()) {
        setBrush(QBrush(Settings::instance()->activatedColor()));
    }
    else if (isSelected()) {
        setBrush(QBrush(Settings::instance()->selectedColor()));
    }
    else {
        setBrush(QBrush(Settings::instance()->defaultBrushColor()));
    }
}

void BlockView::drawShape(QPainter &p)
{
    QCanvasRectangle::drawShape(p);

    // draw header
    int left = (int) x();
    int right = left + width() - 1;
    int currentY = (int) y() + BlockView::DEFAULT_TOP_SPACING;
    QRect textArea(left,
                   currentY,
                   width(),
                   BlockView::DEFAULT_FONT_HEIGHT);
    if (model_->name() != 0) {
        int w = width() - DEFAULT_LEFT_BORDER - DEFAULT_RIGHT_BORDER;
        QString label = Util::squeeze(model_->name(), w, p.font());
        p.drawText(textArea, Qt::AlignHCenter, label);
    }
    currentY += textArea.height() + BlockView::DEFAULT_HEADER_SPACING;
    p.drawLine(left, currentY, right, currentY);
    currentY += BlockView::DEFAULT_HEADER_SPACING;

    textArea = QRect(left + BlockView::DEFAULT_LEFT_BORDER,
                     currentY,
                     width() - BlockView::DEFAULT_LEFT_BORDER
                     - BlockView::DEFAULT_RIGHT_BORDER,
                     BlockView::DEFAULT_FONT_HEIGHT);

    unsigned slotCount = QMAX(leftPins_.size(), rightPins_.size());

    if (slotCount > 0) {
        unsigned pinHeight = (height() - BlockView::DEFAULT_TOP_SPACING
                              - BlockView::DEFAULT_FONT_HEIGHT
                              - BlockView::DEFAULT_HEADER_SPACING
                              - BlockView::DEFAULT_HEADER_SPACING
                              - BlockView::DEFAULT_HEADER_SPACING
                              - BlockView::DEFAULT_BOTTOM_SPACING
                              - ((bottomPins_.size() > 0)
                                 ? DEFAULT_FONT_HEIGHT
                                 : 0))
            / slotCount;

        int maxWidth = width() / 2 - DEFAULT_LABEL_SPACING;
        for (unsigned i = 0; i < slotCount; ++i) {
            if (i < leftPins_.size()) {
                QString label = Util::squeeze(leftPins_[i]->pinModel()->name(),
                                              maxWidth, p.font());
                p.drawText(textArea, Qt::AlignLeft, label);
            }

            if (i < rightPins_.size()) {
                QString label = Util::squeeze(rightPins_[i]->pinModel()->name(),
                                              maxWidth, p.font());
                p.drawText(textArea, Qt::AlignRight, label);
            }
            textArea.moveBy(0, pinHeight);
        }
    } else {
        Q_ASSERT(slotCount == 0);
    }

    if (bottomPins_.size() > 0) {
        unsigned width__
            = (width() - DEFAULT_LEFT_BORDER - DEFAULT_RIGHT_BORDER
               - bottomPins_.size() * DEFAULT_LABEL_SPACING)
            / bottomPins_.size();
        textArea = QRect(left + DEFAULT_LEFT_BORDER,
                         (int)y() + height() - DEFAULT_FONT_HEIGHT
                         - DEFAULT_BOTTOM_SPACING,
                         width__,
                         BlockView::DEFAULT_FONT_HEIGHT);
        for (unsigned i = 0; i < bottomPins_.size(); ++i) {
            QString label = Util::squeeze(bottomPins_[i]->pinModel()->name(),
                                          width__, p.font());
            p.drawText(textArea, Qt::AlignHCenter, label);
            textArea.moveBy(width__ + DEFAULT_LABEL_SPACING, 0);
        }
    }
}

void BlockView::arrangeHorizontalPins()
{
    if (bottomPins_.size() == 0) {
        return;
    }

    int x__ = (int)x() + DEFAULT_LEFT_BORDER;
    int y__ = (int)y() + height();
    unsigned width__
        = (width() - DEFAULT_LEFT_BORDER - DEFAULT_RIGHT_BORDER
           - bottomPins_.size() * DEFAULT_LABEL_SPACING)
        / bottomPins_.size();
    for (unsigned i = 0; i < bottomPins_.size(); ++i) {
        bottomPins_[i]->move(x__
                             + i * DEFAULT_LABEL_SPACING
                             + i * width__ + width__ / 2,
                             y__);
    }
}

void BlockView::arrangeVerticalPins()
{
    int top = ((int) y()) + BlockView::DEFAULT_TOP_SPACING +
        BlockView::DEFAULT_FONT_HEIGHT +
        BlockView::DEFAULT_HEADER_SPACING +
        BlockView::DEFAULT_HEADER_SPACING;
    int bottom
        = ((int) y()) + height()
        - BlockView::DEFAULT_HEADER_SPACING
        - BlockView::DEFAULT_BOTTOM_SPACING
        - ((bottomPins_.size() > 0)
           ? DEFAULT_FONT_HEIGHT
           : 0);

    unsigned leftSize = leftPins_.size();
    unsigned rightSize = rightPins_.size();
    unsigned slotCount = QMAX(leftSize, rightSize);

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

void BlockView::updateView()
{
    arrangePins();
    update();
    canvas()->update();
}

void BlockView::deletePinView(PinView *pin)
{
    pinList(pin)->remove(pin);
}

void BlockView::deleteView()
{
    delete this;
}
