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
 * $Id: genericpriorityqueue.h,v 1.1 2003/12/15 04:07:09 keulsn Exp $
 *
 *****************************************************************************/


#ifndef POA_GENERIC_PRIORITY_QUEUE_H
#define POA_GENERIC_PRIORITY_QUEUE_H

#include "priorityqueue.h"

template<class Item>
class GenericPriorityQueue
{
public:

  void insert(Item *item) { queue_.insert(item); }

  void remove(Item *item) { queue_.remove(item); }

  void clear() { queue_.clear(); }

  Item *head() { return static_cast<Item*>(queue_.head()); }

  Item *removeHead() { return static_cast<Item*>(queue_.removeHead()); }

  bool isEmpty() const { return queue_.isEmpty(); }

  unsigned size() const { return queue_.size(); }

private:
    PriorityQueue queue_;
};


#endif // POA_GENERIC_PRIORITY_QUEUE
