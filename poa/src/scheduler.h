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
 * $Id: scheduler.h,v 1.1 2004/02/09 01:29:49 keulsn Exp $
 *
 *****************************************************************************/

#ifndef POA_SCHEDULER
#define POA_SCHEDULER

#include "blockgraph.h"

class Scheduler
{
public:
    Scheduler(BlockGraph *graph);
    virtual ~Scheduler();

    enum {OPTIMIZED = 0, CYCLIC, PARTITIONED} Result;

    Result optimizeBetween(PinNode *from, PinNode *to);

    bool isCyclic() const;

private:
    BlockGraph *graph_;
};

#endif // POA_SCHEDULER
