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
 * $Id: muxmodel.cpp,v 1.10 2003/09/24 15:44:28 garbeam Exp $
 *
 *****************************************************************************/

#include "muxmodel.h"

#include "muxview.h"

MuxMapping::MuxMapping(PinModel *output, unsigned begin, unsigned end)
{
    Q_ASSERT(begin < end);
    output_ = output;
    begin_ = begin;
    end_ = end;
}

MuxMapping::~MuxMapping()
{

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

QDomElement MuxMapping::serialize(QDomDocument *document)
{
    QDomElement root = document->createElement("muxmapping");
    root.setAttribute("output-id", output_->id());
    root.setAttribute("begin", begin_);
    root.setAttribute("end", end_);
    return root;
}

///////////////////////////////////////////////////////////////////////////////

MuxPin::MuxPin(PinModel *model) {
    model_ = model;
}

MuxPin::~MuxPin() {
    for (unsigned i = 0; i < mappings_.count(); i++) {
        MuxMapping *mapping = mappings_.at(i);
        mappings_.remove(mapping);
        delete mapping;
    }
}

void MuxPin::addMapping(MuxMapping *mapping) {
    mappings_.append(mapping);
}

void MuxPin::removeMapping(MuxMapping *mapping) {
    mappings_.remove(mapping);

    PinModel *output = mapping->output();
    if (output->bits() > (mapping->end() - mapping->begin())) {
        output->setBits(output->bits() - (mapping->end() - mapping->begin()));
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

///////////////////////////////////////////////////////////////////////////////

MuxModel::MuxModel(QString type, QString description)
    : AbstractModel(type, description)
{
    setName(QString("new %1").arg(type));
    type_ = (type == "mux") ? MUX : DEMUX;
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
    for (QPtrListIterator<MuxPin> it(muxPins_); it.current() != 0; ++it) {
        delete *it;
    }

    emit deleted();
}

MuxPin *MuxModel::addPin()
{
    unsigned id;
    unsigned bits;

    bits = 32;
    id = muxPins_.count() + 1;
    PinModel::PinType type = (type_ == MUX) ? PinModel::INPUT : PinModel::OUTPUT;

    MuxPin *pin = new MuxPin(new PinModel(
                    this, id, QString("Output %1").arg(id), 0, bits, type));
    muxPins_.append(pin);

    return pin;
}

void MuxModel::addMuxMapping(MuxPin *input, PinModel *output,
                             unsigned begin, unsigned end, bool setOutputBits)
{
    Q_ASSERT(output != 0);
    // add bits of cureent mapping to output
    if (setOutputBits) {
        output->setBits(output->bits() + (begin - end));
    }
    MuxMapping *mapping = new MuxMapping(output, begin, end);
    input->addMapping(mapping);
}

void MuxModel::removeMuxMapping(MuxPin *pin, MuxMapping *mapping)
{
    pin->removeMapping(mapping);
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
    type_ = (element.attribute("type", "mux") == "mux") ? MUX : DEMUX;

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
