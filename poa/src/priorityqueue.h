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
 * $Id: priorityqueue.h,v 1.6 2004/01/11 10:44:15 squig Exp $
 *
 *****************************************************************************/


#ifndef POA_PRIORITY_QUEUE_H
#define POA_PRIORITY_QUEUE_H

#include <qstring.h>
class PriorityItem;


/**
 * Class to provide priority-ordered queues. Instances of subclasses of
 * {@link PriorityItem} can be inserted in such a queue and removed in
 * oder as specified through {@link PriorityItem#higherPriority}.
 *
 * Every <code>PriorityItem</code> can be contained in at most one
 * <code>PriorityQueue</code>-instance at any point of time.
 */
class PriorityQueue
{
public:

    /**
     * Constructs an empty priority queue.
     * Postcondition: no item is contained in this queue.
     */
    PriorityQueue();

    /**
     * Removes all items from this queue.
     * Postcondition: Any item that was contained in this queue will not
     * be contained in any queue.
     */
    virtual ~PriorityQueue();

    /**
     * Inserts an item into the priority queue.
     * Precondition: <code>item != 0</code> and <code>*item</code> must
     * not be contained in any queue (including this),
     * Postcondition: <code>*item</code> is contained in this queue.
     */
    void insert(PriorityItem *item);

    /**
     * Removes <code>*item</code> from the queue.
     * Precondition: <code>*item</code> must be contained in this queue.
     * <code>item</code> must not be 0,
     * Postcondition: <code>*item</code> is not contained in any queue.
     */
    void remove(PriorityItem *item);

    /**
     * Clears this queue.
     * Postcondition: Any item that was contained in this queue will not
     * be contained in any queue.
     */
    void clear();

    /**
     * Returns a pointer to one of the highest-priority items. If there is
     * more than one item having the highest priority, then any of those is
     * chosen nondeterministically.
     * Returns 0 if the queue is empty.
     */
    PriorityItem *head();

    /**
     * Chooses the item returned by {@link #head} and removes that item from
     * the queue.
     * Precondition: This queue must not be empty
     * @return See {@link #head}
     */
    PriorityItem *removeHead();

    /**
     * Returns true if there is no item contained in this queue. Returns false
     * otherwise.
     */
    bool isEmpty() const;

    /**
     * Returns the number of items currently contained in this queue.
     */
    unsigned size();

    QString checkIntegrity();

private:
    /**
     * Pointer to an item in the heap of <code>PriorityItem</code>-objects
     * or 0 if this queue is empty.
     */
    PriorityItem *heap_;

    /**
     * Sets {@link #heap_} to the root item of the heap.
     * Postcondition: <code>heap_ == 0 || heap_->isHead()</code>
     */
    void findHead();
};



/**
 * Abstract base class for all classes that are to be stored in priority
 * queues. Subclasses must redefine the abstract method
 * {@link PriorityItem#higherPriority}.
 *
 * Instances of a successor of this class can be contained in at most one
 * instance of {@link PriorityQueue} at any point of time.
 */
class PriorityItem
{
public:

    PriorityItem();

    /**
     * Abstract comparison for an irreflexive, transitive, asymmetric
     * (priority) order.
     * Precondition: <code>other != 0</code>
     * @return true if *this has a higher priority than <code>*other</code>,
     * false otherwise.
     */
    virtual bool higherPriority(const PriorityItem *other) const = 0;

    /**
     * Returns true if this is contained in any priority queue and false
     * otherwise. It is not possible to gain access to the
     * {@link PriorityQueue}-object <code>*this</code> is contained in, if any.
     */
    bool isInQueue();

    /**
     * If one of the arguments is 0 then returns the other, else returns
     * <code>first</code>, if <code>!second->higherPriority(first)</code>.
     * Returns <code>second</code>, if
     * </code>second->higherPriority(first)</code>.
     */
    static PriorityItem *maxPriority(PriorityItem *first,
                     PriorityItem *second);

protected:

    /**
     * MUST be called everytime after the priority of <code>*this</code>
     * defined by {@link #higherPriority} has changed. If this is contained
     * in a priority queue, then the structure of that queue will be updated.
     */
    void updatePriority();

private:

    /**
     * Removes <code>*this</code> from a priority queue if any.
     * @return The new head item of the priority queue or 0 if the queue is
     *         empty.
     */
    PriorityItem *removeFromQueue();

    /**
     * Ensures the heap-nature in cases where <code>this</code>'s priority
     * has increased.
     */
    void updateUpward();

    /**
     * Ensures the heap-nature in cases where <code>this</code>'s priority
     * has decreased.
     */
    void updateDownward();

    /**
     * Calls {@link #updateSize} on <code>*this</code> and all predecessors
     * of <code>*this</code>. A predecessor is every <code>PriorityItem</code>-
     * instance that can be reached by traversing along the {@link #parent}
     * links.
     * @return The instance for which {@link #parent} returns 0. That object
     * is the head of the heap.
     */
    PriorityItem *updateSizeUpward();

    PriorityItem *parent() const;

    /**
     * Sets <code>this</code>'s left child and adjusts that child's
     * parent-field. May be 0.
     */
    void setLeft(PriorityItem *left);

    /**
     * Returns the left child or 0 if there is none.
     */
    PriorityItem *left() const;

    /**
     * Sets <code>this</code>'s right child and adjusts that child's
     * parent-field to this. May be 0.
     */
    void setRight(PriorityItem *right);

    /**
     * Returns the right child or 0 if there is none.
     */
    PriorityItem *right() const;

    /**
     * Updates the size (number of items in <code>this</code>'s subtree). Must
     * be called when <code>this</code> is inserted into a heap.
     */
    void updateSize();

    /**
     * Returns the number of items in <code>*this</code>'s subtree.
     * {@link #updateSize} must be called after a change in the subtree and
     * before a call to size.
     */
    unsigned size() const;

    /**
     * Returns true if <code>*this</code> does not have a parent, false
     * otherwise
     */
    bool isHead() const;


    PriorityItem *parent_;
    PriorityItem *left_;
    PriorityItem *right_;
    unsigned size_;

    /**
     * Methods of {@link PriorityQueue} need access to the private members and
     * private fields.
     */
    friend class PriorityQueue;
};


#endif // POA_PRIORITY_QUEUE_H
