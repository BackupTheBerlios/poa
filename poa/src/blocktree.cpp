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
 * $Id: blocktree.cpp,v 1.6 2004/01/17 17:35:39 squig Exp $
 *
 *****************************************************************************/

#include "blocktree.h"
#include "blockmodel.h"

BlockTree::BlockTree(BlockModel* bm)
{
    block_ = bm;
    parent_ = NULL;
    runtime_ = bm->runtime();
    clock_ = bm->clock();
    offset_ = bm->offset();
    backReference_ = false;
}

BlockTree::~BlockTree()
{
    for (QPtrListIterator<BlockTree> it(branches_); it != 0; ++it) {
        delete it;
    }
    branches_.clear();
}

void BlockTree::addBranch(BlockTree* bt)
{
    bt->parent_ = this;
    branches_.append(bt);
}

BlockTree* BlockTree::addBranch(BlockModel* bm)
{
    BlockTree* bt = new BlockTree(bm);
    bt->parent_ = this;
    branches_.append(bt);
    return bt;
}

BlockModel* BlockTree::getBlock()
{
    return block_;
}

QPtrList<BlockTree>* BlockTree::getBranches()
{
    return &branches_;
}

bool BlockTree::contains(BlockModel* bm)
{
    BlockTree* bt = this;
    do {
        if (bt->block_ == bm) return true;
        bt = bt->parent_;
    } while (bt != NULL);

    return false;
}

int BlockTree::count()
{
    int cnt = 1;
    for (QPtrListIterator<BlockTree> it(branches_); it != 0; ++it) {
        cnt += (*it)->count();
    }
    return cnt;
}

int BlockTree::getRuntime() {
    return runtime_;
}

int BlockTree::getClock() {
    return clock_;
}

int BlockTree::getOffset() {
    return offset_;
}

void BlockTree::setRuntime(int runtime) {
    runtime_ = runtime;
}

void BlockTree::setClock(int clock) {
    clock_ = clock;
}

void BlockTree::setOffset(int ofs) {
    offset_ = ofs;
}

void BlockTree::setBackReference(bool b)
{
    backReference_ = b;
}

bool BlockTree::getBackReference()
{
    return backReference_;
}

void BlockTree::commit()
{
    // commit local properties
    block_->setRuntime(runtime_);
    block_->setClock(clock_);
    block_->setOffset(offset_);

    // commit leaves
    QPtrList<BlockTree> bt = *getBranches();
    for (QPtrListIterator<BlockTree> it(bt); it != 0; ++it) {
        (*it)->commit();
    }
}
