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
 * $Id: muxmodel.cpp,v 1.24 2003/10/01 14:50:43 garbeam Exp $
 *
 *****************************************************************************/

#include "muxmodel.h"

#include "muxview.h"

MuxMapping::MuxMapping(MuxPin *muxPin, PinModel *output,
                       unsigned begin, unsigned end)
{
    Q_ASSERT(begin <= end);
    muxPin_ = muxPin;
    output_ = output;
    begin_ = begin;
    end_ = end;
}

MuxMapping::MuxMapping(MuxPin *parent, PinModel *output,
                       QDomElement mapElement)
{
    muxPin_ = parent;
    output_ = output;
    if (!mapElement.isNull()) {
        deserialize(mapElement);
    }

}

MuxMapping::~MuxMapping()
{
}

MuxPin *MuxMapping::muxPin()
{
    return muxPin_;
}

PinModel *MuxMapping::output()
{
    return output_;
}

void MuxMapping::setOutput(PinModel *output) {
    output_ = output;
}

unsigned MuxMapping::begin()
{
    return begin_;
}

void MuxMapping::setBegin(unsigned begin)
{
    Q_ASSERT(begin <= end_);
    begin_ = begin;
}

unsigned MuxMapping::end()
{
    return end_;
}

void MuxMapping::setEnd(unsigned end)
{
    Q_ASSERT(begin_ <= end);
    end_ = end;
}

QDomElement MuxMapping::serialize(QDomDocument *document)
{
    QDomElement root = document->createElement("muxmapping");
    root.setAttribute("output-name", output_->name());
    root.setAttribute("output-id", output_->id());
    root.setAttribute("begin", begin_);
    root.setAttribute("end", end_);
    return root;
}

void MuxMapping::deserialize(QDomElement element) {

    // 0 for fallback
    begin_ = element.attribute("begin", "0").toUInt();
    end_ = element.attribute("end", "0").toUInt();
}

MuxMapping *MuxMapping::clone(MuxPin *muxPin, PinModel *output) {
    return new MuxMapping(muxPin, output, begin_, end_);
}

///////////////////////////////////////////////////////////////////////////////

MuxPin::MuxPin(PinModel *model) {
    model_ = model;
}

MuxPin::MuxPin(MuxModel *parent, QDomElement muxPinElement)
{
    if (!muxPinElement.isNull()) {
        deserialize(parent, muxPinElement);
    }
}

MuxPin::~MuxPin() {
    for (unsigned i = 0; i < mappings_.count(); i++) {
        MuxMapping *mapping = mappings_.at(i);
        mappings_.remove(i);
        delete mapping;
    }
    delete model_;
}

QPtrList<MuxMapping> *MuxPin::mappings() {
    return &mappings_;
}

PinModel *MuxPin::model() {
    return model_;
}

QDomElement MuxPin::serialize(QDomDocument *document)
{
    QDomElement root = document->createElement("muxpin");
    QDomElement pinElem = model_->serialize(document);
    root.appendChild(pinElem);

    for (unsigned i = 0; i < mappings_.count(); i++) {
        QDomElement mappingElem = mappings_.at(i)->serialize(document);
        mappingElem.setAttribute("type", "muxmapping");
        root.appendChild(mappingElem);
    }

    return root;
}

void MuxPin::deserialize(MuxModel *parent, QDomElement element) {

    QDomNode node = element.firstChild();
    while (!node.isNull()) {
        if (node.isElement() && node.nodeName() == "pin") {
            QDomElement pinElem = node.toElement();
            PinModel *input = new PinModel(parent, pinElem);
            input->setType(PinModel::INPUT);
            model_ = input;
        } else if (node.isElement() && node.nodeName() == "muxmapping") {
            QDomElement mapElem = node.toElement();
            QString outputName = mapElem.attribute("output-name", "");
            PinModel *output = parent->outputForName(outputName);
            Q_ASSERT(output != 0); // outputs must have been added already
            MuxMapping *mapping = new MuxMapping(this, output, mapElem);
            mappings_.append(mapping);
        }
        node = node.nextSibling();
    }

}

MuxPin *MuxPin::clone() {
    Q_ASSERT(model_ != 0);

    MuxPin *clonePin = new MuxPin(model_->clone());

    QPtrList<MuxMapping> *mappings = clonePin->mappings();
    for (unsigned i = 0; i < mappings_.count(); i++) {
        MuxMapping *mapping = mappings_.at(i);
        mappings->append(mapping->clone(clonePin, mapping->output()->clone()));
    }

    return clonePin;
}

MuxMapping *MuxPin::findEqual(MuxMapping *mapping) {

    for (unsigned i = 0; i < mappings_.count(); i++) {
        MuxMapping *m = mappings_.at(i);

        if ((m->begin() == mapping->begin()) &&
            (m->end() == mapping->end()) &&
            (m->output()->name() == mapping->output()->name()) &&
            (m->muxPin()->model()->name() ==
                 mapping->muxPin()->model()->name()))
        {
            // found equal mapping
            return m;
        }
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////

MuxModel::MuxModel(QString type, QString description)
    : AbstractModel(type, description)
{
    setName(QString("new %1").arg(type));
    type_ = (type == "Mux") ? MUX : DEMUX;
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
    QPtrListIterator<MuxPin> it(muxPins_);
    MuxPin *pin;
    while ((pin = it.current()) != 0) {
        ++it;
        delete pin;
    }
    muxPins_.clear();

    QPtrListIterator<PinModel> itOut(outputPins_);
    PinModel *outPin;
    while ((outPin = itOut.current()) != 0) {
        ++itOut;
        delete outPin;
    }
    emit deleted();
}

void MuxModel::updateOutputs() {

    // reset all bit values to 0
    QPtrListIterator<PinModel> outIt(outputPins_);
    PinModel *pin;
    while ((pin = outIt.current()) != 0) {
        ++outIt;
        pin->setBits(0);
    }

    // set all bit values to the current valid value
    QPtrListIterator<MuxPin> muxIt(muxPins_);
    MuxPin *muxPin;
    while ((muxPin = muxIt.current()) != 0) {
        ++muxIt;
        MuxMapping *mapping;
        QPtrListIterator<MuxMapping> mapIt(*muxPin->mappings());
        while ((mapping = mapIt.current()) != 0) {
            ++mapIt;
            PinModel *output = mapping->output();
            unsigned bits = mapping->end() - mapping->begin() + 1;
            output->setBits(output->bits() + bits);
        }
    }

    // seek for outputs which have bit range 0 and delete them
    QPtrListIterator<PinModel> it(outputPins_);
    while ((pin = it.current()) != 0) {
        ++it;
        if (pin->bits() == 0) { // still 0
            outputPins_.remove(pin);
            delete pin;
        }
    }
}

void MuxModel::addMuxPin(MuxPin *pin, bool suppressEmission)
{
    muxPins_.append(pin);

    // Reset output pin information for all mappings of this pin
    QPtrListIterator<MuxMapping> it(*pin->mappings());
    MuxMapping *mapping;
    while ((mapping = it.current()) != 0) {
        ++it;
        PinModel *output = mapping->output();
        mapping->setOutput(outputForName(output->name()));
    }

    if (!suppressEmission) {
        emit pinAdded(pin->model());
    }
}

void MuxModel::removeMuxPin(MuxPin *pin)
{
    muxPins_.remove(pin);
    delete pin;
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
    root.setAttribute("type", type());
    if (type_ == MUX) {
        root.setAttribute("block-type", "mux");
    }
    else {
        root.setAttribute("block-type", "demux");
    }

    // serialize output pins
    QPtrListIterator<PinModel> itOut(outputPins_);
    while (itOut != 0) {
        QDomElement pinElem = itOut.current()->serialize(document);
        pinElem.setAttribute("type", "muxoutput");
        root.appendChild(pinElem);
        ++itOut;
    }

    // serialize MuxPins - will also serailize MuxMappings
    QPtrListIterator<MuxPin> it(muxPins_);
    while (it != 0) {
        QDomElement pinElem = it.current()->serialize(document);
        pinElem.setAttribute("type","muxpin");
        root.appendChild(pinElem);
        ++it;
    }
    return root;
}

void MuxModel::deserialize(QDomElement element)
{
    AbstractModel::deserialize(element);
    type_ = (element.attribute("block-type", "mux") == "mux") ? MUX : DEMUX;

    QDomNode node = element.firstChild();
    while ( !node.isNull() ) {
        if (node.isElement() && node.nodeName() == "pin") {
            QDomElement pinElem = node.toElement();
            if (pinElem.attribute("type","") == "muxoutput") {
                PinModel *output = new PinModel(this, pinElem);
                output->setType(PinModel::OUTPUT);
                addOutput(output);
            }
        } else if (node.isElement() && node.nodeName() == "muxpin") {
            QDomElement muxPinElem = node.toElement();
            MuxPin *muxPin = new MuxPin(this, muxPinElem);
            addMuxPin(muxPin);
        }
        node = node.nextSibling();
     }
}

MuxModel::MuxType MuxModel::muxType() {
    return type_;
}

QPtrList<MuxPin> *MuxModel::muxPins() {
    return &muxPins_;
}

QPtrList<PinModel> *MuxModel::outputPins() {
    return &outputPins_;
}

void MuxModel::addOutput(PinModel *pin) {
    outputPins_.append(pin);
    emit pinAdded(pin);
}

void MuxModel::addMuxMapping(MuxMapping *mapping) {

    MuxPin *pin = mapping->muxPin();
    QPtrList<MuxMapping> *mappings = pin->mappings();
    mappings->append(mapping);

    PinModel *output = mapping->output();
    if (!outputPins_.containsRef(output)) {
        addOutput(output);
    }
}

PinModel *MuxModel::outputForName(QString name) {
    QPtrListIterator<PinModel> it(outputPins_);
    PinModel *pin;
    while ((pin = it.current()) != 0) {
        ++it;
        if (pin->name() == name) {
            return pin;
        }
    }
    return 0;
}
