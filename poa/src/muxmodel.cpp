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
 * $Id: muxmodel.cpp,v 1.9 2003/09/24 11:11:19 garbeam Exp $
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

MuxMapping *MuxMapping::clone() {
    return new MuxMapping(output_, input_, begin_, end_);
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
    for (QPtrListIterator<MuxMapping> it(mappings_); it.current() != 0; ++it) {
        delete *it;
    }

    for (QPtrListIterator<PinModel> it(inputPins_); it.current() != 0; ++it) {
        delete *it;
    }

    for (QPtrListIterator<PinModel> it(outputPins_); it.current() != 0; ++it) {
        delete *it;
    }

    emit deleted();
}

PinModel *MuxModel::addPin(PinModel::PinType type)
{
    QPtrList<PinModel> *l;
    unsigned id;
    unsigned bits;

    Q_ASSERT(type != PinModel::EPISODIC);

    switch(type) {
    case PinModel::INPUT:
        bits = 32;
        id = inputPins_.count() + 1;
        l = &inputPins_;
        break;
    case PinModel::OUTPUT:
        bits = 0;
        id = outputPins_.count() + 1;
        l = &outputPins_;
        break;
    case PinModel::EPISODIC:
        // assertion true, so do nothing.
        break;
    }

    PinModel *pin = new PinModel(this, id, QString("Output %1").arg(id), 0, bits, type);
    l->append(pin);

    delete l; // l was only temporary used.

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
                outputPins_.remove(output);
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
        inputPins_.remove(pin);
        break;
    case PinModel::OUTPUT:
        outputPins_.remove(pin);
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
    if (type_ == MUX) {
        root.setAttribute("block-type", "mux");
    }
    else {
        root.setAttribute("block-type", "demux");
    }

    for (unsigned i = 0; i < inputPins_.count(); i++) {
        QDomElement pinElem = inputPins_.at(i)->serialize(document);
        pinElem.setAttribute("type","input");
        root.appendChild(pinElem);
    }
    for (unsigned i = 0; i < outputPins_.count(); i++) {
        QDomElement pinElem = outputPins_.at(i)->serialize(document);
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

    if (element.attribute("block-type", "") == "mux") {
        type_ = MUX;
        setType("mux");
    }
    else {
        type_ = DEMUX;
        setType("demux");
    }

    // TODO: MuxMappings
    QDomNode node = element.firstChild();
    while ( !node.isNull() ) {
        if (node.isElement() && node.nodeName() == "pin" ) {
            QDomElement pin = node.toElement();
            PinModel *pinModel = new PinModel(0, pin);
            if (pin.attribute("type", "") == "input") {
                pinModel->setType(PinModel::INPUT);
                inputPins_.append(pinModel);
            } else if (pin.attribute("type","") == "output") {
                pinModel->setType(PinModel::OUTPUT);
                outputPins_.append(pinModel);
            }
        }
        node = node.nextSibling();
     }
}

MuxModel::MuxType MuxModel::muxType() {
    return type_;
}

QPtrList<MuxMapping> *MuxModel::mappings() {
    return &mappings_;
}

QPtrList<PinModel> *MuxModel::outputs() {
    return &outputPins_;
}

QPtrList<PinModel> *MuxModel::inputs() {
    return &inputPins_;
}
