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
 * $Id: blockgraph.cpp,v 1.7 2004/01/21 10:28:10 kilgus Exp $
 *
 *****************************************************************************/

#include "abstractmodel.h"
#include "blockgraph.h"
#include "blockmodel.h"
#include "muxmodel.h"
#include "pinmodel.h"
#include "project.h"

BlockNode::BlockNode(BlockModel* block)
{
    block_ = block;

    autoOffset_ = block->autoOffset();
    clock_ = block->clock();
    offset_ = block->offset();
    runtime_ = block->runtime();
}

BlockNode::~BlockNode()
{
}

void BlockNode::addNeighbour(BlockNode* neighbour)
{
    // FIX: this is called frequently, better use a hashmap
    if (!neighbours_.contains(neighbour)) {
        neighbours_.append(neighbour);
    }
}


bool BlockNode::autoOffset() const
{
    return autoOffset_;
}

unsigned int BlockNode::clock() const
{
    return clock_;
}

void BlockNode::commit()
{
    // commit local properties
    block_->setAutoOffset(autoOffset_);
    block_->setClock(clock_);
    block_->setOffset(offset_);
    block_->setRuntime(runtime_);
}

BlockModel *BlockNode::model() const
{
    return block_;
}

const QPtrList<BlockNode> &BlockNode::neighbours() const
{
    return neighbours_;
}

unsigned int BlockNode::offset() const
{
    return offset_;
}

unsigned int BlockNode::runtime() const
{
    return runtime_;
}

void BlockNode::setAutoOffset(const bool autoOffset)
{
    autoOffset_ = autoOffset;
}

void BlockNode::setClock(const unsigned int clock)
{
    clock_ = clock;
}

void BlockNode::setOffset(const unsigned int offset)
{
    offset_ = offset;
}

void BlockNode::setRuntime(const unsigned int runtime)
{
    runtime_ = runtime;
}

PinNode::PinNode(BlockNode *parent, PinModel *pin)
{
    parent_ = parent;
    pin_ = pin;
}

PinNode::~PinNode()
{
}

void PinNode::addNeighbour(PinNode *neighbour)
{
    neighbours_.append(neighbour);
}

PinModel *PinNode::model()
{
    return pin_;
}

const QPtrList<PinNode> &PinNode::neighbours() const
{
    return neighbours_;
}

BlockNode *PinNode::parent() const
{
    return parent_;
}

BlockGraph::BlockGraph(Project *project)
{
    // First look for all input blocks
    QPtrList<BlockNode> inputBlocks;
    for (QPtrListIterator<AbstractModel> it(*project->blocks()); it != 0;
         ++it) {

        BlockModel* block = dynamic_cast<BlockModel*>(*it);
        if (block != 0)  {
            if (!block->hasInputPins() && !block->hasEpisodicPins()) {
                addInputBlock(block);
            }
        }
    }

    // iterate through all pins
    QValueList<PinNode*> pins = nodeByPin_.values();
    for (QValueList<PinNode*>::Iterator it2 = pins.begin(); it2 != pins.end();
         ++it2) {

        QPtrList<PinNode> neighbours = (*it2)->neighbours();
        for (QPtrListIterator<PinNode> it3(neighbours); it3 != 0;
             ++it3) {

            if ((*it2)->parent() != (*it3)->parent()) {
                QPtrList<PinNode> seen;
                addBlockNeighbour(*it2, *it3, seen);
            }
        }
    }
}

BlockGraph::~BlockGraph()
{
    QValueList<BlockNode*> blocks = nodeByBlock_.values();
    for (QValueList<BlockNode*>::Iterator it = blocks.begin();
         it != blocks.end(); ++it) {

        delete *it;
    }

    QValueList<PinNode*> pins = nodeByPin_.values();
    for (QValueList<PinNode*>::Iterator it2 = pins.begin(); it2 != pins.end();
         ++it2) {

        delete *it2;
    }
}

// Recursively build tree
void BlockGraph::addInputBlock(BlockModel *model)
{
    inputBlocks_.append(addBlock(model));

    QValueList<PinModel*> pins = model->pins();
    for (QValueList<PinModel*>::Iterator it = pins.begin(); it != pins.end();
         ++it) {

        addOutput(*it);
    }
}

PinNode *BlockGraph::addInput(PinModel *pin)
{
    Q_ASSERT(pin->type() == PinModel::INPUT);

    QMap<PinModel*, PinNode*>::const_iterator nodeIt = nodeByPin_.find(pin);
    if (nodeIt != nodeByPin_.end()) {
        return *nodeIt;
    }

    PinNode *node = new PinNode(addBlock(pin->parent()), pin);
    nodeByPin_[pin] = node;

    // Check all output pins for this input pin
    QPtrList<PinModel> pins = pin->parent()->connectionsForInputPin(pin);
    for (QPtrListIterator<PinModel> it(pins); it != 0; ++it) {
        node->addNeighbour(addOutput(*it));
    }

    return node;
}

PinNode *BlockGraph::addOutput(PinModel *pin)
{
    Q_ASSERT(pin->type() == PinModel::OUTPUT);

    QMap<PinModel*, PinNode*>::const_iterator nodeIt = nodeByPin_.find(pin);
    if (nodeIt != nodeByPin_.end()) {
        return *nodeIt;
    }

    PinNode *node = new PinNode(addBlock(pin->parent()), pin);
    nodeByPin_[pin] = node;

    PinModel *target = pin->connected();
    if (target != 0 && target->type() == PinModel::INPUT) {
        node->addNeighbour(addInput(target));
    }

    return node;
}

BlockNode *BlockGraph::addBlock(BlockModel *block)
{
    if (nodeByBlock_.contains(block)) {
        return nodeByBlock_[block];
    }
    else {
        BlockNode *node = new BlockNode(block);
        nodeByBlock_[block] = node;
        return node;
    }
}

void BlockGraph::addBlockNeighbour(PinNode *source, PinNode *target,
                                   QPtrList<PinNode> &seen)
{
    if (INSTANCEOF(target->parent()->model(), MuxModel)) {
        // iterate through output pins
        QPtrList<PinNode> neighbours = target->neighbours();
        for (QPtrListIterator<PinNode> it(neighbours); it != 0;
             ++it) {

            Q_ASSERT(target->parent() == (*it)->parent());

            if (!seen.contains(*it)) {
                seen.append(*it);
                QPtrList<PinNode> neighbours2 = (*it)->neighbours();
                for (QPtrListIterator<PinNode> it2(neighbours2); it2 != 0;
                     ++it2) {

                    addBlockNeighbour(source, *it2, seen);
                }
            }
        }
    }
    else {
        qDebug(QString("added connection %1 -> %2")
               .arg(source->parent()->model()->name())
               .arg(target->parent()->model()->name()));
        source->parent()->addNeighbour(target->parent());
    }
}

QValueList<BlockNode*> BlockGraph::blocks() const
{
    return nodeByBlock_.values();
}

QValueList<PinNode*> BlockGraph::pins() const
{
    return nodeByPin_.values();
}


// Breitensuche
//
//      QPtrList<PinNode> seen;
//      QPtrList<PinNode> pending;

//      for (QPtrListIterator<BlockNode> it(inputBlocks_); it != 0; ++it) {
//          QValueList<PinModel*> pins = (*it)->model()->pins();
//          for (QValueList<PinModel*>::Iterator it2 = pins.begin();
//               it2 != pins.end(); ++it2) {
//              qDebug("Push: " + (*it2)->name());
//              pending.append(nodeByPin_[*it2]);
//          }
//      }


//      while (!pending.isEmpty()) {
//          PinNode *node = pending.first();
//          pending.removeFirst();
//          seen.append(node);

//          qDebug("Name: " + node->model()->name());

//          QPtrList<PinNode> pins = node->neighbours();
//          for (QPtrListIterator<PinNode> it(pins); it != 0; ++it) {
//              if (!seen.contains(*it)) {
//                  pending.append(*it);
//              }
//          }
//      }
