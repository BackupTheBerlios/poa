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
 * $Id: blockgraph.cpp,v 1.1 2004/01/18 13:50:48 squig Exp $
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
    neighbours_.append(neighbour);
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

QPtrList<BlockNode> BlockNode::neighbours() const
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

PinNode::PinNode(PinModel *pin)
{
    pin_ = pin;
}

PinNode::~PinNode()
{
}

PinModel *PinNode::model()
{
    return pin_;
}

BlockGraph::BlockGraph(Project *project)
{
    QPtrList<BlockNode> inputBlocks;

    // First look for all input blocks
    for (QPtrListIterator<AbstractModel> it(*project->blocks()); it != 0;
         ++it) {

        if (INSTANCEOF(*it, BlockModel))  {
            BlockModel* bm = dynamic_cast<BlockModel*>(*it);
            if (!bm->hasInputPins() && !bm->hasEpisodicPins()) {
                BlockNode* node = new BlockNode(bm);
                inputBlocks.append(node);
            }
        }
    }


    for (QPtrListIterator<BlockNode> it2(inputBlocks); it2 != 0; ++it2) {
        add(*it2);
    }

}

// Recursively build tree
void BlockGraph::add(BlockNode *node)
{
    QValueList<PinModel*> pins = node->model()->pins();
//      for (QValueList<PinModel*>::Iterator pin = pins.begin(); pin != pins.end();
//           ++pin) {

//          if (((*pin)->type() == PinModel::OUTPUT) && (*pin)->connected()) {
//              BlockModel* target = (*pin)->connected()->parent();

//              // In case of Mux get list of connecting blocks from Mux
//              if (INSTANCEOF(target, MuxModel)) {
//                  QPtrList<PinModel> pins =
//                      ((MuxModel*)block)->connectionsForInputPin((*pin)->connected());
//                  // Check all output pins for this input pin
//                  for (QPtrListIterator<PinModel> muxit(pins); muxit != 0; ++muxit) {
//                      if (!(*muxit)->connected()) continue;
//                      block = (*muxit)->connected()->parent();
//                      // Only add model recursively if not already in tree

//                      if (blocksToTree_.contains(block)) {
//                          // add back reference only and stop recursion.
//                          bt->addBranch(block)->setBackReference(true);
//                      } else {
//                          BlockTree *lb = bt->addBranch(block);
//                          blocksToTree_.insert(block, lb);
//                          blocks_.append(lb);
//                          buildBranch(lb);
//                      }
//                  }
//              } else {    // No mux, straight connection

//                  if (blocksToTree_.contains(block)) {
//                      // add back reference only and stop recursion.
//                      bt->addBranch(block)->setBackReference(true);
//                  } else {
//                      BlockTree *lb = bt->addBranch(block);
//                      blocksToTree_.insert(block, lb);
//                      blocks_.append(lb);
//                      buildBranch(lb);
//                  }
//              }
//          }
//      }
}
