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
 * $Id: muxmodel.cpp,v 1.3 2003/09/22 08:57:04 garbeam Exp $
 *
 *****************************************************************************/

#include "muxmodel.h"

MuxMapping::MuxMapping(PinModel *input, PinModel *output,
                       unsigned begin, unsigned end)
{
    Q_ASSERT(begin < end);
    input_ = input;
    connect(input_, SIGNAL(deleted()), this, SLOT(deleteMapping()));
    output_ = output;
    begin_ = begin;
    end_ = end;
}

MuxMapping::~MuxMapping()
{

}

PinModel *MuxMapping::input()
{
    return input_;
}

PinModel *MuxMapping::output()
{
    return output_;
}

unsigned MuxMapping::begin()
{
    return begin_;
}

void MuxMapping::setBegin(unsigned begin)
{
    Q_ASSERT(begin < end_);
    begin_ = begin;
}

unsigned MuxMapping::end()
{
    return end_;
}

void MuxMapping::setEnd(unsigned end)
{
    Q_ASSERT(begin_ < end);
    end_ = end;
}

void MuxMapping::deleteMapping()
{
    delete this;
}

MuxModel::MuxModel(QString type, QString description)
    : AbstractModel(type, description)
{
    inputPins_ = new PinVector();
    outputPins_ = new PinVector();
}

MuxModel::MuxModel(QDomElement coreElement)
    : AbstractModel(QString::null, QString::null)
{
    if (!coreElement.isNull()) {
        deserialize(coreElement);
    }
}

MuxModel::~MuxModel()
{

}

void MuxModel::addMuxMapping(PinModel *input, PinModel *output,
                             unsigned begin, unsigned end)
{
    MuxMapping *mapping = new MuxMapping(input, output, begin, end);


    mappings_.append(mapping);

}

void MuxModel::removePin(PinModel *pin)
{
    switch (pin->type()) {
    case PinModel::INPUT:
        inputPins_->remove(pin);
        break;
    case PinModel::OUTPUT:
        outputPins_->remove(pin);
        break;
    case PinModel::EPISODIC:
        qDebug("Warning: Cannot remove EPISODIC pin from MuxModel");
        break;
    }
}

QCanvasItemList MuxModel::createView(QCanvas *canvas)
{
    // TODO
    // only to prevent compiler warning until MuxView is implemented
    // on Monday or Sunday
    canvas->isWidgetType();
    
    QCanvasItemList l;
    return l;
}

QDomElement MuxModel::serialize(QDomDocument *document)
{
    QDomElement root = AbstractModel::serialize(document);

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

    return root;
}

void MuxModel::deserialize(QDomElement element)
{
    AbstractModel::deserialize(element);

    // pins
    inputPins_->clear();
    outputPins_->clear();

    // TODO
    QDomNode node = element.firstChild();
    while ( !node.isNull() ) {
        if (node.isElement() && node.nodeName() == "pin" ) {
            QDomElement pin = node.toElement();
            PinModel *pinModel = new PinModel(0, pin);
            if (pin.attribute("type", "") == "input") {
                pinModel->setType(PinModel::INPUT);
            } else if (pin.attribute("type","") == "output") {
                pinModel->setType(PinModel::OUTPUT);
            }
        }
        node = node.nextSibling();
     }
}
