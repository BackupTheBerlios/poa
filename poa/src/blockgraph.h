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
 * $Id: blockgraph.h,v 1.1 2004/01/18 13:50:48 squig Exp $
 *
 *****************************************************************************/

#ifndef POA_BLOCKGRAPH_H
#define POA_BLOCKGRAPH_H

#include <qmap.h>
#include <qptrlist.h>

class AbstractModel;
class BlockModel;
class PinModel;
class Project;

class BlockNode
{
public:

    BlockNode(BlockModel* bm);
    ~BlockNode();


    void addNeighbour(BlockNode *node);

    /**
     * Returns true, if the automatic offset calculation should be used.
     */
    bool autoOffset() const;

    /**
     * Returns the clock of the block.
     */
    unsigned int clock() const;

    /**
     * Commits all data (runtime, clock, offset) to the corresponding
     * BlockModel.
     */
    void commit();

    /**
     * Returns the model.
     */
    BlockModel *model() const;

    /**
     * Returns a list of neighbours.
     */
    QPtrList<BlockNode> neighbours() const;

    /**
     * Returns the starting offset of the block.
     */
    unsigned int offset() const;

    /**
     * Returns the runtime of the block.
     */
    unsigned int runtime() const;

    /**
     * Sets the automatic offset calculation.
     */
    void setAutoOffset(const bool autoOffset);

    /**
     * Sets the clock of the block.
     */
    void setClock(const unsigned int clock);

    /**
     * Sets the starting offset of the block.
     */
    void setOffset(const unsigned int offset);

    /**
     * Sets the runtime of the block.
     */
    void setRuntime(const unsigned int time);

private:
    BlockModel *block_;
    QPtrList<BlockNode> neighbours_;

    bool autoOffset_;
    int clock_;
    int offset_;
    int runtime_;
};

class PinNode
{
public:

    PinNode(PinModel *pin);

    ~PinNode();

    PinModel *model();

private:

    PinModel *pin_;
};

class BlockGraph
{
public:

    BlockGraph(Project *project);

private:

    void add(BlockNode *node);

    QMap<AbstractModel*, BlockNode*> modelByNode_;
    QPtrList<BlockNode> blocks_;
};

#endif // POA_BLOCKGRAPH_H
