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
 * $Id: muxmodel.cpp,v 1.25 2003/11/26 15:52:45 garbeam Exp $
 *
 *****************************************************************************/

#include "muxmodel.h"

#include "muxview.h"

#include "pinmodel.h"

MuxMapping::MuxMapping(PinModel *input, PinModel *output,
        unsigned firstInputBit, unsigned lastInputBit,
        unsigned firstOutputBit, unsigned lastOutputBit)
{
    Q_ASSERT(firstInputBit <= lastInputBit);
    Q_ASSERT(firstOutputBit <= lastOutputBit);
    input_ = input;
    output_ = output;
    firstInputBit_ = firstInputBit;
    firstOutputBit_ = firstOutputBit;
    lastInputBit_ = lastInputBit;
    lastOutputBit_ = lastOutputBit;
}

MuxMapping::MuxMapping(PinModel *input, PinModel *output,
                       QDomElement mapElement)
{
    input_ = input;
    output_ = output;
    if (!mapElement.isNull()) {
        deserialize(mapElement);
    }
}

MuxMapping::~MuxMapping()
{
}

PinModel *MuxMapping::input() const {
    return input_;
}

PinModel *MuxMapping::output() const {
    return output_;
}

void MuxMapping::setOutput(PinModel *output) {
    output_ = output;
}

void MuxMapping::setInput(PinModel *input) {
    input_ = input;
}

unsigned MuxMapping::firstInputBit() const {
    return firstInputBit_;
}

unsigned MuxMapping::firstOutputBit() const {
    return firstOutputBit_;
}

unsigned MuxMapping::lastInputBit() const {
    return lastInputBit_;
}

unsigned MuxMapping::lastOutputBit() const {
    return lastOutputBit_;
}

void MuxMapping::setFirstInputBit(unsigned firstInputBit) {
    firstInputBit_ = firstInputBit;
}

void MuxMapping::setFirstOutputBit(unsigned firstOutputBit) {
    firstOutputBit_ = firstOutputBit;
}

void MuxMapping::setLastInputBit(unsigned lastInputBit) {
    lastInputBit_ = lastInputBit;
}

void MuxMapping::setLastOutputBit(unsigned lastOutputBit) {
    lastOutputBit_ = lastOutputBit;
}

QDomElement MuxMapping::serialize(QDomDocument *document)
{
    QDomElement root = document->createElement("muxmapping");
#if 0
    root.setAttribute("output-name", output_->name());
    root.setAttribute("output-id", output_->id());
    root.setAttribute("begin", begin_);
    root.setAttribute("end", end_);
#endif
    return root;
}

void MuxMapping::deserialize(QDomElement element) {
#if 0
    // 0 for fallback
    begin_ = element.attribute("begin", "0").toUInt();
    end_ = element.attribute("end", "0").toUInt();
#endif
}

MuxMapping *MuxMapping::clone(PinModel *input, PinModel *output) {
    return new MuxMapping(input, output, firstInputBit_, lastInputBit_,
                          firstOutputBit_, lastOutputBit_);
}

///////////////////////////////////////////////////////////////////////////////

MuxModel::MuxModel(QString type, QString description)
    : BlockModel(type, description)
{
    setName(QString("new %1").arg(type));
    type_ = (type == "Mux") ? MUX : DEMUX;
    setDescription(description);
}

MuxModel::MuxModel(QDomElement coreElement)
    : BlockModel(QString::null, QString::null)
{
    if (!coreElement.isNull()) {
        deserialize(coreElement);
    }
}

MuxModel::~MuxModel()
{
    for (QPtrListIterator<MuxMapping> it(mappings_); it != 0; ++it) {
        MuxMapping *mapping = it.current();
        delete mapping;
    }
    mappings_.clear();

    emit deleted();
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
#if 0
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
#endif
    return root;
}

void MuxModel::deserialize(QDomElement element)
{
#if 0
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
#endif
}

MuxModel::MuxType MuxModel::muxType() {
    return type_;
}

void MuxModel::addMuxMapping(MuxMapping *mapping) {

    mappings_.append(mapping);
}

void MuxModel::removeMuxMapping(MuxMapping *mapping) {

    mappings_.remove(mapping);
}

QPtrList<MuxMapping> *MuxModel::mappings() {
    return &mappings_;
}
