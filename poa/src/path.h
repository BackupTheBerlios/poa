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
 * $Id: path.h,v 1.1 2004/02/18 03:42:19 keulsn Exp $
 *
 *****************************************************************************/

#ifndef POA_PATH_H
#define POA_PATH_H

#include <qmap.h>

#include "blockgraph.h"
#include "blockmodel.h"
#include "genericpriorityqueue.h"


class DepthFirstNode;
typedef QMap<BlockNode*, DepthFirstNode*> BlockMap;
typedef QValueList<DepthFirstNode*> DepthFirstNodeList;

class Path;
typedef GenericPriorityQueue<Path> PathQueue;


class Path : public PriorityItem
{
public:
    Path(BlockNode *target);
    Path(const Path &other);
    virtual ~Path();

    void prepend(BlockNode *node);
    void removeFirst();
    void setNodeFlag(bool state);

    BlockNode *node();

    QString getText() const;

    unsigned length() const;
    const BlockNode *front(int add) const;
    const BlockNode *end(int add) const;

    void optimize();

    virtual bool higherPriority(const PriorityItem *other) const;

    static void allPaths(PathQueue &paths,
			 BlockNode *from,
			 BlockNode *to);
    
private:

    static const int infinity;

    static void recursiveTarjan(DepthFirstNode &current,
				int &time,
				BlockMap &blockMap,
				DepthFirstNodeList &cycleStack);

    static void extractPaths(PathQueue &paths,
			     const DepthFirstNode &latest,
			     Path &current);
	
    QValueList<BlockNode*> nodes_;
    unsigned int runtime_;
};


#endif // POA_PATH_H
