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
 * $Id: blockmodel.cpp,v 1.45 2004/01/24 16:20:10 squig Exp $
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
    currentPinId_ = 0;
    name_ = type;
    autoOffset_ = true;
    offset_ = 0;
    runtime_ = 0;

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

bool BlockModel::autoOffset() const
{
    return autoOffset_;
}

unsigned int BlockModel::clock() const
{
    return clock_;
}

QPtrList<PinModel> BlockModel::connectionsForInputPin(PinModel *)
{
    QValueList<PinModel*> pins = this->pins();
    QPtrList<PinModel> connectedPins;
    for (QValueList<PinModel*>::Iterator it = pins.begin(); it != pins.end();
         ++it) {

        if ((*it)->type() == PinModel::OUTPUT) {
            connectedPins.append(*it);
        }
    }
    return connectedPins;
}

QCanvasItemList BlockModel::createView(GridCanvas *canvas)
{
    QCanvasItemList list;
    BlockView *view = new BlockView(this, canvas);
    list.append(view);
    //view->addPinViewsTo(list);
    return list;
}

bool BlockModel::hasEpisodicPins() const
{
    return hasEpisodicPins_;
}

bool BlockModel::hasInputPins() const
{
    return hasInputPins_;
}

bool BlockModel::hasOutputPins() const
{
    return hasOutputPins_;
}

bool BlockModel::hasRuntime() const
{
    return hasRuntime_;
}

void BlockModel::addPin(PinModel *pin)
{
    if (pinById_.contains(pin->id())) {
        // find next free id (highest in Map +1)
        //currentPinId_ = pinById_.keys().last()+1;
        while (pinById_.contains(++currentPinId_));
        //qDebug("id: "+QString::number(pin->id())+" -> "+QString::number(currentPinId_)+" / "+QString::number(pinById_.keys().last()+1));
        pin->setId(currentPinId_);
    }
    pinById_[pin->id()] = pin;
    pin->setParent(this);

    emit pinAdded(pin);
}

void BlockModel::deletePin(PinModel *pin)
{
    pinById_.remove(pin->id());
    delete pin;
}

PinModel *BlockModel::findPinById(unsigned id)
{
    QMap<uint, PinModel *>::const_iterator it = pinById_.find(id);
    return (it != pinById_.end()) ? *it : 0;
}

unsigned int BlockModel::offset() const
{
    return offset_;
}

QValueList<PinModel*> BlockModel::pins() const
{
    return pinById_.values();
}

unsigned int BlockModel::runtime() const
{
    return runtime_;
}

QDomElement BlockModel::serialize(QDomDocument *document)
{
    QDomElement root = AbstractModel::serialize(document);

    root.setAttribute("auto-offset", autoOffset() ? "true" : "false");
    root.setAttribute("block-type", "block");
    root.setAttribute("name", name());
    root.setAttribute("clock", (unsigned int)clock());
    root.setAttribute("hasEpisodicPins", hasEpisodicPins() ? "true" : "false");
    root.setAttribute("hasInputPins", hasInputPins() ? "true" : "false");
    root.setAttribute("hasOutputPins", hasOutputPins() ? "true" : "false");
    root.setAttribute("hasRuntime", hasRuntime() ? "true" : "false");
    root.setAttribute("offset", (unsigned int)offset());
    root.setAttribute("runtime", (unsigned int)runtime());

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

    setAutoOffset((element.attribute("auto-offset", "true") == "true"));
    setClock(element.attribute("clock","0").toUInt());
    setHasEpisodicPins(element.attribute("hasEpisodicPins", "") == "true");
    setHasInputPins(element.attribute("hasInputPins", "") == "true");
    setHasOutputPins(element.attribute("hasOutputPins", "") == "true");
    setHasRuntime(element.attribute("hasRuntime", "") == "true");
    setOffset(element.attribute("offset","0").toUInt());
    setRuntime(element.attribute("runtime","0").toUInt());

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

void BlockModel::setAutoOffset(const bool autoOffset)
{
    autoOffset_ = autoOffset;
}

void BlockModel::setClock(const unsigned int clock)
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

void BlockModel::setOffset(const unsigned int offset)
{
    offset_ = offset;
}

void BlockModel::setRuntime(const unsigned int runtime)
{
    runtime_ = runtime;
}

QString BlockModel::tip()
{
    QString s = QString("<u>%2</u> (%3)<br>" \
                        "<i>%4</i><hr>"
                        "<b>Clock:</b> %5 ns<br>" \
                        "<b>Offset:</b> %6<br>")
        .arg(this->name())
        .arg(this->type())
        .arg(this->description())
        .arg(this->clock())
        .arg(toTip(this->autoOffset(), this->offset()));
    if (hasRuntime()) {
        s.append(QString("<b>Execution time:</b> %1 ns")
                 .arg(QString::number(this->runtime())));
    }

    return s;
}

QString BlockModel::toTip(bool autoValue, int value)
{
    return (autoValue)
        ? QString("Auto (%1 ns)").arg(QString::number(value))
        : QString("%1 ns").arg(QString::number(value));
}
