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
 * $Id: muxmodel.cpp,v 1.6 2003/09/23 10:53:39 garbeam Exp $
 *
 *****************************************************************************/

#include "muxmodel.h"

#include "muxview.h"

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

QDomElement MuxMapping::serialize(QDomDocument *document)
{
    QDomElement root = document->createElement("muxmapping");
    root.setAttribute("input-id", input_->id());
    root.setAttribute("output-id", output_->id());
    root.setAttribute("begin", begin_);
    root.setAttribute("end", end_);
    return root;
}

///////////////////////////////////////////////////////////////////////////////

MuxModel::MuxModel(QString type, QString description)
    : AbstractModel(type, description)
{
    inputPins_ = new PinVector();
    outputPins_ = new PinVector();
    setType(type);
    setName(QString("new %1").arg(type));
    setDescription(description);
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

PinModel *MuxModel::addPin(PinModel::PinType type)
{
    PinVector *pinVector;
    unsigned id;
    unsigned bits;

    Q_ASSERT(type != PinModel::EPISODIC);

    switch(type) {
    case PinModel::INPUT:
        bits = 32;
        id = inputPins_->size() + 1;
        pinVector = inputPins_;
        break;
    case PinModel::OUTPUT:
        bits = 0;
        id = outputPins_->size() + 1;
        pinVector = outputPins_;
        break;
    case PinModel::EPISODIC:
        // assertion true, so do nothing.
        break;
    }

    PinModel *pin = new PinModel(this, id, QString("Output %1").arg(id), 0, bits, type);
    pinVector->addBefore(pin);

    return pin;
}

void MuxModel::addMuxMapping(PinModel *input, unsigned begin, unsigned end)
{
    addMuxMapping(input, addPin(PinModel::OUTPUT), begin, end);
}

void MuxModel::addMuxMapping(PinModel *input, PinModel *output,
                             unsigned begin, unsigned end, bool setOutputBits)
{
    Q_ASSERT(output != 0);
    // add bits of cureent mapping to output
    if (setOutputBits) {
        output->setBits(output->bits() + (begin - end));
    }
    MuxMapping *mapping = new MuxMapping(input, output, begin, end);

    mappings_.append(mapping);
}

void MuxModel::removeMuxMappings(PinModel *pin)
{
    for (QPtrListIterator<MuxMapping> it(mappings_); it.current() != 0; ++it) {
        MuxMapping *mapping = (*it);

        if (mapping->input() == pin || mapping->output() == pin) {

            PinModel *output = mapping->output();
            unsigned bits = mapping->end() - mapping->begin();
            if (output->bits() > bits) {
                // there are other existing mappings, so reset bits
                // of the current output pin.
                output->setBits(output->bits() - bits);
            }
            else {
                outputPins_->remove(output);
                delete output;
            }
            mappings_.remove(mapping);
            delete mapping;
        }
    }
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
    QCanvasItemList l;
    MuxView *view = new MuxView(this, canvas);
    l.append(view);
    view->addPinViewsTo(l);

    return l;
}

QDomElement MuxModel::serialize(QDomDocument *document)
{
    QDomElement root = AbstractModel::serialize(document);
    root.setAttribute("name", name());
    root.setAttribute("mux-type", type());

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
    for (unsigned i = 0; i < mappings_.count(); i++) {
        QDomElement mappingElem = mappings_.at(i)->serialize(document);
        mappingElem.setAttribute("type", "muxmapping");
        root.appendChild(mappingElem);
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
