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
 * $Id: blockmodel.cpp,v 1.41 2004/01/17 15:05:59 squig Exp $
 *
 *****************************************************************************/


#include "blockmodel.h"

#include <qcanvas.h>
#include <qdom.h>
#include <qstring.h>

#include "blockview.h"
#include "pinmodel.h"

const int BlockModel::FIRST_PIN_POSISION = 1;

BlockModel::BlockModel(QString type, QString description)
    : AbstractModel(type, description)
{

    setHasEpisodicPins(true);
    setHasInputPins(true);
    setHasOutputPins(true);
    setHasRuntime(true);

    clock_ = 0;
    execTime_ = 0;
    currentPinId_ = 0;
    name_ = type;
    offset_ = 0;

    pinById_ = QMap<uint, PinModel*>();
}

BlockModel::BlockModel(QDomElement element)
{
    deserialize(element);
}

BlockModel::~BlockModel()
{

    QValueList<PinModel*> pinList = pinById_.values();
    for (QValueList<PinModel *>::iterator it = pinList.begin();
         it != pinList.end(); ++it)
    {
        deletePin(*it);
    }

    emit deleted();
}

QCanvasItemList BlockModel::createView(QCanvas *canvas)
{
    QCanvasItemList list;
    BlockView *view = new BlockView(this, canvas);
    list.append(view);
    view->addPinViewsTo(list);
    return list;
}

bool BlockModel::hasEpisodicPins()
{
    return hasEpisodicPins_;
}

bool BlockModel::hasInputPins()
{
    return hasInputPins_;
}

bool BlockModel::hasOutputPins()
{
    return hasOutputPins_;
}

bool BlockModel::hasRuntime()
{
    return hasRuntime_;
}

/*QPtrList<PinModel> *BlockModel::inputPins()
{
    return &inputPins_;
}


QPtrList<PinModel> *BlockModel::outputPins()
{
    return &outputPins_;
}*/

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

void BlockModel::setHasEpisodicPins(bool hasEpisodicPins)
{
    this->hasEpisodicPins_ = hasEpisodicPins;
}

void BlockModel::setHasInputPins(bool hasInputPins)
{
    this->hasInputPins_ = hasInputPins;
}

void BlockModel::setHasOutputPins(bool hasOutputPins)
{
    this->hasOutputPins_ = hasOutputPins;
}

void BlockModel::setHasRuntime(bool hasRuntime)
{
    this->hasRuntime_ = hasRuntime;
}

unsigned int BlockModel::clock()
{
    return clock_;
}

void BlockModel::addPin(PinModel *pin)
{

    /*    if (pin->id() == 0) {
        pin->setId(++currentPinId_);
    }
    else if (pin->id() > currentPinId_) {
        currentPinId_ = pin->id();
        }*/

    if (pinById_.contains(pin->id())) {
        // find next free id (highest in Map +1)
        //currentPinId_ = pinById_.keys().last()+1;
        while (pinById_.contains(++currentPinId_));
        //qDebug("id: "+QString::number(pin->id())+" -> "+QString::number(currentPinId_)+" / "+QString::number(pinById_.keys().last()+1));
        pin->setId(currentPinId_);
    }
    pinById_[pin->id()] = pin;
    pin->setParent(this);

    /*
    switch (pin->type()) {
    case PinModel::INPUT:
        inputPins_.append(pin);
        break;
    case PinModel::OUTPUT:
        outputPins_.append(pin);
        break;
    case PinModel::EPISODIC:
        episodicPins_.append(pin);
        break;
        }*/

    emit pinAdded(pin);
}

void BlockModel::deletePin(PinModel *pin)
{
    /*    switch (pin->type()) {
    case PinModel::INPUT:
        inputPins_.remove(pin);
        break;
    case PinModel::OUTPUT:
        outputPins_.remove(pin);
        break;
    case PinModel::EPISODIC:
        episodicPins_.remove(pin);
        break;
        }*/
    pinById_.remove(pin->id());
    delete pin;
}


QValueList<PinModel*> BlockModel::pins()
{
    return pinById_.values();
}

PinModel *BlockModel::findPinById(unsigned id)
{
    /*    // seek in input pins
    for (QPtrListIterator<PinModel> it(inputPins_); it != 0; ++it) {
        PinModel *pin = it.current();
        if (pin->id() == id) {
            return pin;
        }
    }

    // seek in output pins
    for (QPtrListIterator<PinModel> it(outputPins_); it != 0; ++it) {
        PinModel *pin = it.current();
        if (pin->id() == id) {
            return pin;
        }
    }

    // seek in episodic pins
    for (QPtrListIterator<PinModel> it(episodicPins_); it != 0; ++it) {
        PinModel *pin = it.current();
        if (pin->id() == id) {
            return pin;
        }
        }*/
    QMap<uint, PinModel *>::const_iterator it = pinById_.find(id);
    return (it != pinById_.end()) ? *it : 0;
}

QDomElement BlockModel::serialize(QDomDocument *document)
{
    QDomElement root = AbstractModel::serialize(document);
    root.setAttribute("hasEpisodicPins", hasEpisodicPins() ? "true" : "false");
    root.setAttribute("hasInputPins", hasInputPins() ? "true" : "false");
    root.setAttribute("hasOutputPins", hasOutputPins() ? "true" : "false");
    root.setAttribute("hasRuntime", hasRuntime() ? "true" : "false");

    root.setAttribute("block-type", "block");
    root.setAttribute("name", name());
    root.setAttribute("exectime", (unsigned int)execTime_);
    root.setAttribute("clock", (unsigned int)clock_);
    root.setAttribute("offset", (unsigned int)offset_);

    /*    for (QPtrListIterator<PinModel> it(inputPins_); it != 0; ++it) {
        PinModel *pin = it.current();
        QDomElement pinElem = pin->serialize(document);
        pinElem.setAttribute("type", "input");
        root.appendChild(pinElem);
    }
    for (QPtrListIterator<PinModel> it(inputPins_); it != 0; ++it) {
        PinModel *pin = it.current();
        QDomElement pinElem = pin->serialize(document);
        pinElem.setAttribute("type", "output");
        root.appendChild(pinElem);
    }
    for (QPtrListIterator<PinModel> it(inputPins_); it != 0; ++it) {
        PinModel *pin = it.current();
        QDomElement pinElem = pin->serialize(document);
        pinElem.setAttribute("type", "episodic");
        root.appendChild(pinElem);
        }*/

     QMap<uint, PinModel *>::Iterator it;
     for ( it = pinById_.begin(); it != pinById_.end(); ++it ) {
         QDomElement pinElem = (*it)->serialize(document);
         root.appendChild(pinElem);
     }

     return root;
}

void BlockModel::deserialize(QDomElement element)
{
    AbstractModel::deserialize(element);

    setExecTime(element.attribute("exectime","0").toUInt());
    setClock(element.attribute("clock","0").toUInt());
    setHasEpisodicPins(element.attribute("hasEpisodicPins", "") == "true");
    setHasInputPins(element.attribute("hasInputPins", "") == "true");
    setHasOutputPins(element.attribute("hasOutputPins", "") == "true");
    setHasRuntime(element.attribute("hasRuntime", "") == "true");
    setOffset((unsigned int)element.attribute("offset","0").toUInt());

    // pins
    /*    inputPins_.clear();
    outputPins_.clear();
    episodicPins_.clear();*/

    QDomNode node = element.firstChild();
    while ( !node.isNull() ) {
        if (node.isElement() && node.nodeName() == "pin" ) {
            QDomElement pin = node.toElement();
            PinModel *pinModel = new PinModel(this, pin);
            if (pin.attribute("type", "") == "input") {
                pinModel->setType(PinModel::INPUT);
            }
            else if (pin.attribute("type","") == "output") {
                pinModel->setType(PinModel::OUTPUT);
            }
            else if (pin.attribute("type","") == "episodic") {
                pinModel->setType(PinModel::EPISODIC);
            }
            addPin(pinModel);
        }
        node = node.nextSibling();
     }

}

void BlockModel::setOffset(unsigned int offset)
{
    offset_ = offset;
}

unsigned int BlockModel::offset()
{
    return offset_;
}

QString BlockModel::tip()
{
    QString s = QString("<b>Core</b><br>" \
                        "<u>%1</u> (%2)<br>" \
                        "<i>%3</i>")
        .arg(this->name())
        .arg(this->type())
        .arg(this->description());
    if (hasRuntime()) {
        s.append(QString("<hr><b>Execution time:</b> %4 ns")
                 .arg(QString::number(this->execTime())));
    }

    return s;
}
