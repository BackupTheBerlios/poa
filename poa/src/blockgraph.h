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
 * $Id: blockgraph.h,v 1.7 2004/02/16 10:40:26 keulsn Exp $
 *
 *****************************************************************************/

#ifndef POA_BLOCKGRAPH_H
#define POA_BLOCKGRAPH_H

#include <qmap.h>
#include <qptrlist.h>

class AbstractModel;
class BlockGraphIterator;
class BlockModel;
class PinModel;
class PinNode;
class Project;

class BlockNode
{
public:

    BlockNode(BlockModel* bm);
    virtual ~BlockNode();


    void addNeighbour(BlockNode *node);

    //void addPin(PinNode *pin);

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
    const QPtrList<BlockNode> &neighbours() const;

    /**
     * Returns the starting offset of the block.
     */
    unsigned int offset() const;

    //QPtrList<PinNode> pins() const;

    /**
     * Returns the runtime of the block.
     */
    unsigned int runtime() const;

    /**
     * Returns the state of the flag
     */
    bool flag() const;

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

    /**
     * Sets the flag
     */
    void setFlag(bool flag);

private:
    BlockModel *block_;
    QPtrList<BlockNode> neighbours_;
    QPtrList<PinNode> pins_;

    bool autoOffset_;
    int clock_;
    int offset_;
    int runtime_;

    bool flag_;
};

class PinNode
{
public:

    PinNode(BlockNode *parent, PinModel *pin);
    virtual ~PinNode();

    void addNeighbour(PinNode *neighbour);
    PinModel *model();
    const QPtrList<PinNode> &neighbours() const;
    BlockNode *parent() const;

private:

    QPtrList<PinNode> neighbours_;
    BlockNode *parent_;
    PinModel *pin_;
};

class BlockGraph
{
public:

    BlockGraph(Project *project);
    ~BlockGraph();

    QValueList<BlockNode*> blocks() const;
    QValueList<PinNode*> pins() const;

private:

    void addInputBlock(BlockModel *model);
    PinNode *addInput(PinModel *pin);
    PinNode *addOutput(PinModel *pin);
    BlockNode *addBlock(BlockModel *block);
    void addBlockNeighbour(PinNode *source, PinNode *target,
                           QPtrList<PinNode> &seen);

    QMap<PinModel*, PinNode*> nodeByPin_;
    QMap<BlockModel*, BlockNode*> nodeByBlock_;
    QPtrList<BlockNode> inputBlocks_;
};

#endif // POA_BLOCKGRAPH_H
