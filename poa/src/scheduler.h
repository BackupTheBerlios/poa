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
 * $Id: scheduler.h,v 1.2 2004/02/13 17:07:57 keulsn Exp $
 *
 *****************************************************************************/

#ifndef POA_SCHEDULER
#define POA_SCHEDULER

#include <qmap.h>

#include "blockgraph.h"
#include "blockmodel.h"
#include "genericpriorityqueue.h"


class Path : public PriorityItem
{
public:
    Path(BlockNode *target);
    Path(const Path &other);
    virtual ~Path();

    void prepend(BlockNode *node);
    void removeFirst();

    BlockNode *node();

    QString getText() const;

    virtual bool higherPriority(const PriorityItem *other) const;

private:
    QValueList<BlockNode*> nodes_;
    unsigned int runtime_;
};

typedef GenericPriorityQueue<Path> PathQueue;

class DepthFirstNode;
typedef QMap<BlockNode*, DepthFirstNode*> BlockMap;
typedef QValueList<DepthFirstNode*> DepthFirstNodeList;



class Scheduler
{
public:
    Scheduler(BlockGraph *graph);
    virtual ~Scheduler();

    void allPaths(PathQueue &paths, BlockNode *from, BlockNode *to);
    
protected:

    void firstPass(DepthFirstNode &current,
		   int &time,
		   BlockMap &blockMap,
		   DepthFirstNodeList &cycleStack);

    void extractPaths(PathQueue &paths,
		      const DepthFirstNode &latest,
		      Path &current);
	
private:
    BlockGraph *graph_;
    static const int infinity;
};

#endif // POA_SCHEDULER
