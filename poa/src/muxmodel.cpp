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
 * $Id: muxmodel.cpp,v 1.29 2003/12/10 16:13:21 garbeam Exp $
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

MuxMapping::MuxMapping(MuxModel *parent, QDomElement mapElement)
{
    if (!mapElement.isNull()) {
        deserialize(parent, mapElement);
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
    root.setAttribute("input-id", input_->id());
    root.setAttribute("output-id", output_->id());
    root.setAttribute("first-input-bit", firstInputBit_);
    root.setAttribute("last-input-bit", lastInputBit_);
    root.setAttribute("first-output-bit", firstOutputBit_);
    root.setAttribute("last-output-bit", lastOutputBit_);
    return root;
}

void MuxMapping::deserialize(MuxModel *parent, QDomElement element) {
    unsigned id = element.attribute("input-id", 0).toUInt();
    input_ = parent->findPinById(id);
    id = element.attribute("output-id", 0).toUInt();
    output_ = parent->findPinById(id);
    firstInputBit_ = element.attribute("first-input-bit", 0).toUInt();
    lastInputBit_ = element.attribute("last-input-bit", 0).toUInt();
    firstOutputBit_ = element.attribute("first-output-bit", 0).toUInt();
    lastOutputBit_ = element.attribute("last-output-bit", 0).toUInt();
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
    QDomElement root = BlockModel::serialize(document);
    if (type_ == MUX) {
        root.setAttribute("block-type", "mux");
    }
    else {
        root.setAttribute("block-type", "demux");
    }

    // serialize muxmappings
    for (QPtrListIterator<MuxMapping> it(mappings_); it != 0; ++it) {
        QDomElement mappingElem = it.current()->serialize(document);
        root.appendChild(mappingElem);
    }

    return root;
}

void MuxModel::deserialize(QDomElement element)
{
    BlockModel::deserialize(element);
    type_ = (element.attribute("block-type", "mux") == "mux") ? MUX : DEMUX;

    // deserialize muxmappings
    QDomNode node = element.firstChild();
    while ( !node.isNull() ) {
        if (node.isElement() && node.nodeName() == "muxmapping") {
            QDomElement mappingElem = node.toElement();
            addMuxMapping(new MuxMapping(this, mappingElem));
        }
        node = node.nextSibling();
    }
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


QPtrList<PinModel> MuxModel::connectionsForInputPin(PinModel *input) {

    QPtrList<PinModel> connectedPins;
    for (QPtrListIterator<MuxMapping> it(mappings_); it != 0; ++it) {
        MuxMapping *mapping = it.current();
        if (mapping->input() == input) {
            connectedPins.append(mapping->output());
        }
    }

    return connectedPins;
}
