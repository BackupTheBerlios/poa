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
 * $Id: muxmodel.cpp,v 1.18 2003/09/29 10:59:39 garbeam Exp $
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
    root.setAttribute("output-id", output_->id());
    root.setAttribute("begin", begin_);
    root.setAttribute("end", end_);
    return root;
}

MuxMapping *MuxMapping::clone(MuxPin *muxPin, PinModel *output) {
    return new MuxMapping(muxPin, output, begin_, end_);
}

///////////////////////////////////////////////////////////////////////////////

MuxPin::MuxPin(PinModel *model) {
    model_ = model;
}

MuxPin::~MuxPin() {
    for (unsigned i = 0; i < mappings_.count(); i++) {
        MuxMapping *mapping = mappings_.at(i);
        mappings_.remove(i);
        delete mapping;
    }
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
    for (unsigned i = 0; i < muxPins_.count(); i++) {
        MuxPin *pin = muxPins_.at(i);
        muxPins_.remove(i);
        delete pin;
    }

    emit deleted();
}

void MuxModel::addMuxPin(MuxPin *pin, bool suppressEmission)
{
    muxPins_.append(pin);
    QPtrList<MuxMapping> *mappings = pin->mappings();

    if (!suppressEmission) {
        if (mappings->count() > 0) {
            // emit pinAdd signals for each mapping if needed
            for (unsigned i = 0; i < mappings->count(); i++) {
                emit pinAdded(mappings->at(i)->output());
            }
        }
        emit pinAdded(pin->model());
    }
}

void MuxModel::removeMuxPin(MuxPin *pin)
{
    muxPins_.remove(pin);
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

    for (unsigned i = 0; i < muxPins_.count(); i++) {
        QDomElement pinElem = muxPins_.at(i)->serialize(document);
        pinElem.setAttribute("type","muxpin");
        root.appendChild(pinElem);
    }
    return root;
}

void MuxModel::deserialize(QDomElement element)
{
    AbstractModel::deserialize(element);
    type_ = (element.attribute("type", "Mux") == "Mux") ? MUX : DEMUX;

    // TODO: MuxMappings
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

    if (!outputPins_.containsRef(mapping->output())) {
        addOutput(mapping->output());
    }
}

void MuxModel::removeMuxMapping(MuxMapping *mapping) {

    MuxPin *pin = mapping->muxPin();
    QPtrList<MuxMapping> *mappings = pin->mappings();
    mappings->remove(mapping);

    PinModel *output = mapping->output();
    if (output->bits() > (mapping->end() - mapping->begin())) {
        output->setBits(output->bits() - (mapping->end() - mapping->begin()));
    }
    else {
        // it was the last MuxMapping related to output
        outputPins_.remove(output);
        delete output; // Notifies also the dedicated view
    }
}

PinModel *MuxModel::outputForName(QString name) {
    for (unsigned i = 0; i < outputPins_.count(); i++) {
        PinModel *model = outputPins_.at(i);
        if (model->name() == name) {
            return model;
        }
    }
    return 0;
}
