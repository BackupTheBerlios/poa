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
 * $Id: muxmodel.cpp,v 1.40 2004/03/24 14:56:21 garbeam Exp $
 *
 *****************************************************************************/

#include "muxmodel.h"

#include "pinmodel.h"

MuxMapping::MuxMapping(PinModel *input, PinModel *output,
        unsigned firstInputBit, unsigned lastInputBit,
        unsigned firstOutputBit, unsigned lastOutputBit)
{
    Q_ASSERT(firstInputBit <= lastInputBit);
    Q_ASSERT(firstOutputBit <= lastOutputBit);
    id_ = 0;
    input_ = input;
    output_ = output;
    firstInputBit_ = firstInputBit;
    firstOutputBit_ = firstOutputBit;
    lastInputBit_ = lastInputBit;
    lastOutputBit_ = lastOutputBit;
}

MuxMapping::MuxMapping(MuxModel *parent, QDomElement mapElement)
{
    Q_ASSERT(!mapElement.isNull());
    deserialize(parent, mapElement);
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
    root.setAttribute("id", id_);
    root.setAttribute("input-id", input_->id());
    root.setAttribute("output-id", output_->id());
    root.setAttribute("first-input-bit", firstInputBit_);
    root.setAttribute("last-input-bit", lastInputBit_);
    root.setAttribute("first-output-bit", firstOutputBit_);
    root.setAttribute("last-output-bit", lastOutputBit_);
    return root;
}

void MuxMapping::deserialize(MuxModel *parent, QDomElement element) {
    unsigned id = element.attribute("id", 0).toUInt();
    id_ = id;
    id = element.attribute("input-id", 0).toUInt();
    input_ = parent->findPinById(id);
    id = element.attribute("output-id", 0).toUInt();
    output_ = parent->findPinById(id);
    firstInputBit_ = element.attribute("first-input-bit", "0").toUInt();
    lastInputBit_ = element.attribute("last-input-bit", "1").toUInt();
    firstOutputBit_ = element.attribute("first-output-bit", "0").toUInt();
    lastOutputBit_ = element.attribute("last-output-bit", "1").toUInt();
}

unsigned MuxMapping::id() {
    return id_;
}

void MuxMapping::setId(unsigned id) {
    id_ = id;
}

///////////////////////////////////////////////////////////////////////////////

MuxModel::MuxModel(QString type, QString description)
    : BlockModel(type, description)
{
    setHasEpisodicPins(false);
    setHasRuntime(false);

    initIdCounter();
}

MuxModel::MuxModel(QDomElement coreElement)
    : BlockModel(QString::null, QString::null)
{
    setHasEpisodicPins(false);
    setHasRuntime(false);

    initIdCounter();
    deserialize(coreElement);
}

MuxModel::~MuxModel()
{
    for (QPtrListIterator<MuxMapping> it(mappings_); it != 0; ++it) {
        MuxMapping *mapping = it.current();
        delete mapping;
    }
    mappings_.clear();
}

QDomElement MuxModel::serialize(QDomDocument *document)
{
    QDomElement root = BlockModel::serialize(document);
    root.setAttribute("block-type", "mux");

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

    // deserialize muxmappings
    QDomNode node = element.firstChild();
    while (!node.isNull()) {
        if (node.isElement() && node.nodeName() == "muxmapping") {
            QDomElement mappingElem = node.toElement();
            addMuxMapping(new MuxMapping(this, mappingElem));
        }
        // else don't care
        node = node.nextSibling();
    }
}

MuxMapping *MuxModel::findMuxMappingById(unsigned id)
{
    for (QPtrListIterator<MuxMapping> it(mappings_); it != 0; ++it) {

        MuxMapping *mapping = *it;
        if (mapping->id() == id) {
            return mapping;
        }
    }

    // not found
    return 0;
}

void MuxModel::addMuxMapping(MuxMapping *mapping) {

    idCounter_++;
    mapping->setId(idCounter_);
    mappings_.append(mapping);
    Q_ASSERT(mapping->input() != 0);
    Q_ASSERT(mapping->output() != 0);
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


void MuxModel::initIdCounter() {

    if (mappings_.count() < 1) {
        idCounter_ = 0;
        return;
    }

    for (QPtrListIterator<MuxMapping> it(mappings_); it != 0; ++it) {

        MuxMapping *mapping = *it;
        if (mapping->id() >= idCounter_) {
            idCounter_ = mapping->id() + 1;
        }
    }
}
