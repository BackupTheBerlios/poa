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
 * $Id: blocktree.h,v 1.3 2004/01/09 22:39:44 vanto Exp $
 *
 *****************************************************************************/

#ifndef POA_BLOCKTREE_H
#define POA_BLOCKTREE_H

#include <qptrlist.h>

class BlockModel;

class BlockTree
{
public:
    BlockTree(BlockModel* bm);
    ~BlockTree();

    /**
     * Add new branch to tree. Tree object has already been created
     */
    void addBranch(BlockTree* bt);

    /**
     * Add new branch to tree. Create tree object first
     */
    BlockTree* addBranch(BlockModel* bm);

    /**
     * Get corresponding blockmodel
     */
    BlockModel* getBlock();

    /**
     * Get list of all branches for this root
     */
    QPtrList<BlockTree>* getBranches();

    /**
     * Check whether previous branches in tree already contain this block
     */
    bool contains(BlockModel* bm);

    /**
     * Count all branches from this root on
     */
    int count();

    /**
     * Get runtime of block
     */
    int getRuntime();

    /*
     * Set runtime for this block
     */
    void setRuntime(int runtime);

    /**
     * Get clock of block
     */
    int getClock();

    /**
     * Set clock for this block
     */
    void setClock(int clock);

    /**
     * Get offset of block
     */
    int getOffset();

    /**
     * Set offset for this block
     */
    void setOffset(int ofs);

    /**
     * Set whether this branch is really just a back reference
     */
    void setBackReference(bool b);

    /**
     * Get whether this is just a back reference
     */
    bool getBackReference();

    /**
     * Commits all data (runtime, clock, offset) to the corresponding
     * BlockModel.
     */
    void commit();

private:
    BlockModel *block_;
    BlockTree *parent_;
    QPtrList<BlockTree> branches_;

    int runtime_;
    int clock_;
    int offset_;
    bool backReference_;
};

#endif // POA_BLOCKTREE_H
