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
 * $Id: blockmodel.cpp,v 1.27 2003/09/28 21:52:11 squig Exp $
 *
 *****************************************************************************/


#include "blockmodel.h"

#include <qcanvas.h>
#include <qstring.h>

#include "blockview.h"
#include "pinvector.h"


BlockModel::BlockModel(QString type, QString description)
    : AbstractModel(type, description)
{
    episodicPins_ = new PinVector();
    inputPins_ = new PinVector();
    outputPins_ = new PinVector();

    execTime_ = 0;
    currentPinId_ = 0;
    name_ = type;
}


BlockModel::~BlockModel()
{

    PinVector::iterator it;

    for(it = episodicPins_->begin(); it != episodicPins_->end(); ++it ) {
        delete (*it);
    }
    delete episodicPins_;

    for(it = inputPins_->begin(); it != inputPins_->end(); ++it ) {
        delete (*it);
    }
    delete inputPins_;

    for(it = outputPins_->begin(); it != outputPins_->end(); ++it ) {
        delete (*it);
    }
    delete outputPins_;

    emit deleted();
}


PinVector *BlockModel::episodicPins()
{
    return episodicPins_;
}

bool BlockModel::hasEpisodicPins()
{
    return true;
}

bool BlockModel::hasInputPins()
{
    return true;
}

bool BlockModel::hasOutputPins()
{
    return true;
}

bool BlockModel::hasRuntime()
{
    return true;
}

PinVector *BlockModel::inputPins()
{
    return inputPins_;
}


PinVector *BlockModel::outputPins()
{
    return outputPins_;
}

void BlockModel::setExecTime(unsigned int time)
{
    execTime_ = time;
}

unsigned int BlockModel::execTime()
{
    return execTime_;
}

void BlockModel::setClock(unsigned int clock)
{
    clock_ = clock;
}

unsigned int BlockModel::clock()
{
    return clock_;
}


void BlockModel::addPin(PinModel *pin, PinModel *successor, bool emitSignal)
{

    if (pin->id() == 0) {
        pin->setId(++currentPinId_);
    }
    else if (pin->id() > currentPinId_) {
        currentPinId_ = pin->id();
    }

    switch (pin->type()) {
    case PinModel::INPUT:
        inputPins_->addBefore(pin, successor);
        break;
    case PinModel::OUTPUT:
        outputPins_->addBefore(pin, successor);
        break;
    case PinModel::EPISODIC:
        episodicPins_->addBefore(pin, successor);
        break;
    }

    if (emitSignal) {
        emit pinAdded(pin);
    }
}

void BlockModel::removePin(PinModel *pin)
{
    switch (pin->type()) {
    case PinModel::INPUT:
        inputPins_->remove(pin);
        break;
    case PinModel::OUTPUT:
        outputPins_->remove(pin);
        break;
    case PinModel::EPISODIC:
        episodicPins_->remove(pin);
        break;
    }
}


PinModel *BlockModel::findPinById(unsigned id)
{
    PinModel *pin = inputPins_->findById(id);
    if (pin != 0) return pin;
    pin = outputPins_->findById(id);
    if (pin != 0) return pin;
    pin = episodicPins_->findById(id);
    return pin;
}

QDomElement BlockModel::serialize(QDomDocument *document)
{
    QDomElement root = AbstractModel::serialize(document);
    root.setAttribute("name", name());
    root.setAttribute("exectime", (unsigned int)execTime_);

    for (unsigned i = 0; i < inputPins_->size(); i++) {
        QDomElement pinElem = inputPins_->at(i)->serialize(document);
        pinElem.setAttribute("type","input");
        root.appendChild(pinElem);
    }
    for (unsigned i = 0; i < outputPins_->size(); i++) {
        QDomElement pinElem = outputPins_->at(i)->serialize(document);
        pinElem.setAttribute("type","output");
        root.appendChild(pinElem);
    }
    for (unsigned i = 0; i < episodicPins_->size(); i++) {
        QDomElement pinElem = episodicPins_->at(i)->serialize(document);
        pinElem.setAttribute("type","episodic");
        root.appendChild(pinElem);
    }

    return root;
}

void BlockModel::deserialize(QDomElement element)
{
    AbstractModel::deserialize(element);

    setExecTime(element.attribute("exectime","0").toUInt());

    // pins
    inputPins_->clear();
    outputPins_->clear();
    episodicPins_->clear();

    QDomNode node = element.firstChild();
    while ( !node.isNull() ) {
        if (node.isElement() && node.nodeName() == "pin" ) {
            QDomElement pin = node.toElement();
            PinModel *pinModel = new PinModel(this, pin);
            if (pin.attribute("type", "") == "input") {
                pinModel->setType(PinModel::INPUT);
            } else if (pin.attribute("type","") == "output") {
                pinModel->setType(PinModel::OUTPUT);
            } else if (pin.attribute("type","") == "episodic") {
                pinModel->setType(PinModel::EPISODIC);
            }
            addPin(pinModel);
        }
        node = node.nextSibling();
     }

}

