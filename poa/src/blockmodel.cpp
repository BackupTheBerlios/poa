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
 * $Id: blockmodel.cpp,v 1.1 2003/08/22 17:21:22 keulsn Exp $
 *
 *****************************************************************************/


#include "blockmodel.h"


BlockModel::BlockModel()
{
    name_ = QString("");
    episodicPins_ = new PinVector();
    inputPins_ = new PinVector();
    outputPins_ = new PinVector();
}


BlockModel::BlockModel(QString &name)
{
    name_ = name;
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


PinVector *BlockModel::getEpisodicPins()
{
    return episodicPins_;
}


PinVector *BlockModel::getInputPins()
{
    return inputPins_;
}


PinVector *BlockModel::getOutputPins()
{
    return outputPins_;
}


void BlockModel::setName(QString &name)
{
    name_ = name;
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


void BlockModel::addEpisodicPin(PinModel *pin, PinModel *successor = 0)
{
    int size = episodicPins_->size();
    int i = 0;
    while (i < size && episodicPins_->at(i) != successor) {
	++i;
    }

    episodicPins_->insert(episodicPins_->begin() + i, pin);
    // FIX: update views
}


void BlockModel::removeEpisodicPin(PinModel *pin)
{
    int size = episodicPins_->size();
    int i = 0;
    bool found = episodicPins_->at(i) == pin;
    while (i < size && !found) {
	++i;
	found = episodicPins_->at(i) == pin;
    }
    if (found) {
	episodicPins_->erase(episodicPins_->begin() + i);
    }
    // FIX: update views
}


QString BlockModel::getName()
{
    return name_;
}
