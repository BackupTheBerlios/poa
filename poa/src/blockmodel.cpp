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
 * $Id: blockmodel.cpp,v 1.5 2003/08/26 16:53:09 keulsn Exp $
 *
 *****************************************************************************/


#include "blockmodel.h"

#include <qcanvas.h>
#include <qstring.h>

#include "blockview.h"
#include "pinvector.h"


BlockModel::BlockModel(QString name, QString description)
    : AbstractModel(name, description)
{
    episodicPins_ = new PinVector();
    inputPins_ = new PinVector();
    outputPins_ = new PinVector();
}


BlockModel::~BlockModel()
{
    delete episodicPins_;
    delete inputPins_;
    delete outputPins_;
}


PinVector *BlockModel::episodicPins()
{
    return episodicPins_;
}


PinVector *BlockModel::inputPins()
{
    return inputPins_;
}


PinVector *BlockModel::outputPins()
{
    return outputPins_;
}



void BlockModel::setClock(unsigned long clock)
{
    clock_ = clock;
}

void BlockModel::setOffset(unsigned long offset)
{
    offset_ = offset;
}


void BlockModel::setAutoOffset(bool autoOffset)
{
    autoOffset_ = autoOffset;
}


void BlockModel::addInputPin(PinModel *pin, PinModel *successor)
{
    inputPins_->addBefore(pin, successor);
    // FIX: update views
}

void BlockModel::removeInputPin(PinModel *pin)
{
    inputPins_->remove(pin);
    // FIX: update views
}

void BlockModel::addOutputPin(PinModel *pin, PinModel *successor)
{
    outputPins_->addBefore(pin, successor);
    // FIX: update views
}

void BlockModel::removeOutputPin(PinModel *pin)
{
    outputPins_->remove(pin);
    // FIX: opdate views
}

void BlockModel::addEpisodicPin(PinModel *pin, PinModel *successor)
{
    episodicPins_->addBefore(pin, successor);
    // FIX: update views
}


void BlockModel::removeEpisodicPin(PinModel *pin)
{
    episodicPins_->remove(pin);
    // FIX: update views
}

QCanvasItemList BlockModel::createView(QCanvas *canvas)
{
    QCanvasItemList list;
    BlockView *view = new BlockView (this, canvas);
    list.append(view);
    view->addPinViewsTo(list);
    return list;
}
