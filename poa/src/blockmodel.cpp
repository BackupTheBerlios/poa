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
 * $Id: blockmodel.cpp,v 1.49 2004/02/16 16:24:01 squig Exp $
 *
 *****************************************************************************/


#include "blockmodel.h"

#include <qcanvas.h>
#include <qdom.h>
#include <qstring.h>

#include "blockview.h"
#include "pinmodel.h"
#include "poa.h"

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
    autoOffset_ = true;
    offset_ = 0;
    runtime_ = 0;

    setName(type);
}

BlockModel::BlockModel(QDomElement element)
{
    deserialize(element);
}

BlockModel::~BlockModel()
{

    QValueList<PinModel*> pinList = pinById_.values();
    for (QValueList<PinModel *>::iterator it = pinList.begin();
         it != pinList.end(); ++it) {

        deletePin(*it);
    }

    emit deleted(this);
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
    // simply collect all output pins, we assume that logically each
    // input pin is connected to each output pin

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
    Q_ASSERT(pin->parent() == this);

    if (pinById_.contains(pin->id())) {
        // find free id
        while (pinById_.contains(currentPinId_)) {
            ++currentPinId_;
        }
        pin->setId(currentPinId_);
    }
    pinById_[pin->id()] = pin;

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
    QString s = QString(tr("<u>%2</u> (%3)<br>" \
                        "<i>%4</i><hr>"
                        "<b>Clock:</b> %5<br>" \
               "<b>Offset:</b> %6<br>"))
        .arg(this->name())
        .arg(this->type())
        .arg(this->description())
        .arg(formatTimeProperly(this->clock()))
        .arg(toTip(this->autoOffset(), this->offset()));
    if (hasRuntime()) {
      s.append(QString(tr("<b>Execution time:</b> %1"))
                 .arg(formatTimeProperly(this->runtime())));
    }

    return s;
}

QString BlockModel::toTip(bool autoValue, int value)
{
    return (autoValue)
        ? QString("Auto (%1)").arg(formatTimeProperly(value))
        : QString("%1").arg(formatTimeProperly(value));
}
