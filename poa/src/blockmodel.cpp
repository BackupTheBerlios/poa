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
 * $Id: blockmodel.cpp,v 1.11 2003/08/29 17:59:38 vanto Exp $
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

    clock_ = 0;
    offset_ = 0;
    autoOffset_ = true;
    currentPinId_ = 0;
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
    if (pin->id() == 0) {
        pin->setId(++currentPinId_);
    } else if (pin->id() > currentPinId_) {
        currentPinId_ = pin->id();
    }
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
    if (pin->id() == 0) {
        pin->setId(++currentPinId_);
    } else if (pin->id() > currentPinId_) {
        currentPinId_ = pin->id();
    }
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
    if (pin->id() == 0) {
        pin->setId(++currentPinId_);
    } else if (pin->id() > currentPinId_) {
        currentPinId_ = pin->id();
    }
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

QDomElement BlockModel::serialize(QDomDocument *document)
{
    QDomElement root = AbstractModel::serialize(document);
    root.setAttribute("auto-offset", autoOffset_? "true":"false");
    root.setAttribute("offset", (unsigned int)offset_);
    root.setAttribute("clock", (unsigned int)clock_);
    root.setAttribute("name", name());

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
    setAutoOffset( (element.attribute("auto-offset","true") == "true") );
    setOffset( (unsigned int)element.attribute("offset","0").toUInt() );
    setClock( (unsigned int)element.attribute("clock","0").toUInt() );
    setName( element.attribute("name","unnamed") );
    // pins
    inputPins_->clear();
    outputPins_->clear();
    episodicPins_->clear();

    QDomNode node = element.firstChild();
    while ( !node.isNull() ) {
        if (node.isElement() && node.nodeName() == "pin" ) {
            QDomElement pin = node.toElement();
            //PinModel *pinModel = new PinModel(this, pin.attribute("name","unknown"));
                        //pinModel->setAddress((unsigned int)pin.attribute("address","0").toUInt());
                        //            pinModel->setBits((unsigned int)pin.attribute("bits","0").toUInt());
            PinModel *pinModel = new PinModel(this, pin);
            if (pin.attribute("type", "") == "input") {
                addInputPin(pinModel);
            } else if (pin.attribute("type","") == "output") {
                addOutputPin(pinModel);
            } else if (pin.attribute("type","") == "episodic") {
                addEpisodicPin(pinModel);
            }
        }
        node = node.nextSibling();
     }

}
