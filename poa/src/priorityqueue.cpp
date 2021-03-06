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
 * $Id: priorityqueue.cpp,v 1.11 2004/02/05 14:00:23 papier Exp $
 *
 *****************************************************************************/


#include <qapplication.h>
#include <utility>
#include <qvaluelist.h>
#include <qstring.h>

#include "poa.h"
#include "priorityqueue.h"



PriorityQueue::PriorityQueue()
{
    heap_ = 0;
}

PriorityQueue::~PriorityQueue()
{
    clear();
}

int PriorityQueue::checkEqualDistribution()
{
    findHead();
    QValueList<PriorityItem*> *currentLayer = new QValueList<PriorityItem*>;
    QValueList<PriorityItem*> *nextLayer = new QValueList<PriorityItem*>;
    int layer = 1;
    currentLayer->push_front(heap_);
    int difference = 0;

    while (!currentLayer->isEmpty()) {
        int leftSize = 0;
        int rightSize = 0;
        PriorityItem *current = currentLayer->front();
        currentLayer->pop_front();

        PriorityItem *leftItem = current->left();
        if (leftItem != 0) {
            leftSize = leftItem->size();
            nextLayer->push_front(leftItem);
        }
        PriorityItem *rightItem = current->right();
        if (rightItem != 0) {
            rightSize = rightItem->size();
            nextLayer->push_front(rightItem);
        }
        int currentDiff = rightSize - leftSize;
        if (QABS(currentDiff) > QABS(difference)) {
            difference = currentDiff;
        }

        if (currentLayer->isEmpty()) {
            layer++;
            QValueList<PriorityItem*> *temp = currentLayer;
            currentLayer = nextLayer;
            nextLayer = temp;
        }
    }
    delete currentLayer;
    delete nextLayer;
    return difference;
}

void PriorityQueue::separateSmallerLarger(PriorityItem *left,
                                          PriorityItem *right,
                                          PriorityItem **smaller,
                                          PriorityItem **larger)
{
    if (left == 0) {
        *smaller = 0;
        *larger = right;
    }
    else if (right == 0) {
        *smaller = 0;
        *larger = left;
    }
    else if (right->size() < left->size()) {
        *smaller = right;
        *larger = left;
    }
    else {
        *smaller = left;
        *larger = right;
    }
}

void PriorityQueue::insert(PriorityItem *item)
{
    Q_ASSERT(item != 0 && !item->isInQueue());
    Q_ASSERT(item->left() == 0 && item->right() == 0 && item->parent_ == 0);
    findHead();

    PriorityItem *oldLeft;
    PriorityItem *oldRight;
    PriorityItem *oldParent = 0;
    PriorityItem *smaller = heap_;
    PriorityItem *larger = 0;

    // sink into smaller subtree until item has higher priority than smaller
    // subtree's root
    while (smaller != 0 && smaller->higherPriority(item)) {
        oldParent = smaller;
        oldLeft = smaller->left();
        oldRight = smaller->right();
        separateSmallerLarger(oldLeft, oldRight, &smaller, &larger);
    }
    
    if (smaller == 0) {
        if (oldParent == 0) {
            // heap was empty before, set new root
            // omit assignment "heap_ == item", since this is done after
            // updateSizeUpward()
        }
        else {
            // oldParent != 0
            // item becomes leaf
            if (oldLeft == 0) {
                oldParent->setLeft(item);
            }
            else {
                Q_ASSERT(oldRight == 0);
                oldParent->setRight(item);
            }
        }
    }
    else {
        // insert under oldParent, above smaller, as larger's sibling
        // NOTE that oldParent may be 0 --> item becomes new root
        if (oldParent == 0) {
            // must install item as root and insert smaller under it
            oldParent = item;
            oldLeft = smaller->left();
            oldRight = smaller->right();
            item = smaller;
            separateSmallerLarger(oldLeft, oldRight, &smaller, &larger);
        }
        
        while (true) {
            oldParent->setLeft(item);
            oldParent->setRight(larger);
            if (smaller == 0) {
                item->setLeft(0);
                item->setRight(0);
                break;
            }
            Q_ASSERT(smaller != 0);
            oldParent = item;
            item = smaller;
            oldLeft = smaller->left();
            oldRight = smaller->right();
            separateSmallerLarger(oldLeft, oldRight, &smaller, &larger);
        }
    }
    // must update size, item is assured to be a leaf
    Q_ASSERT(item->left() == 0 && item->right() == 0);
    heap_ = item->updateSizeUpward();
}


void PriorityQueue::remove(PriorityItem *item)
{
    Q_ASSERT(item != 0 && item->isInQueue());
    heap_ = item->removeFromQueue();
}

void PriorityQueue::clear()
{
    findHead();
    PriorityItem *current = heap_;
    heap_ = 0;
    while (current != 0) {
        PriorityItem *left = current->left();
        PriorityItem *right = current->right();
        if (left != 0) {
            // descend into left subtree first
            current = left;
        }
        else if (right != 0) {
            // descend into right subtree second
            current = right;
        }
        else {
            // no children --> remove
            PriorityItem *parent = current->parent();
            if (parent != 0) {
                if (parent->left() == current) {
                    parent->setLeft(0);
                }
                else {
                    Q_ASSERT(parent->right() == current);
                    parent->setRight(0);
                }
                current->parent_ = 0;
            }
            current->size_ = 0;

            current = parent;
        }
    }
}

PriorityItem *PriorityQueue::head()
{
    findHead();
    return heap_;
}

PriorityItem *PriorityQueue::removeHead()
{
    Q_ASSERT(!isEmpty());
    PriorityItem *oldHead = head();
    if (oldHead != 0) {
        heap_ = oldHead->removeFromQueue();
    }
    return oldHead;
}

bool PriorityQueue::isEmpty() const
{
    return heap_ == 0;
}

unsigned PriorityQueue::size()
{
    if (heap_ == 0) {
        return 0;
    }
    else {
        findHead();
        return heap_->size();
    }
}

void PriorityQueue::findHead()
{
    PriorityItem *next = heap_;
    while (next != 0) {
        heap_ = next;
        next = heap_->parent();
    }
}

QString PriorityQueue::checkIntegrity()
{
    findHead();
    PriorityItem *head = heap_;
    QString defects = QString::null;
    if (head != 0) {
        if  (!head->isHead()) {
            defects += 
	      qApp->translate("priorityqueue",
			      "\nQueue's head is not heap's root item");
        }

        unsigned wrongPrio = 0;
        unsigned wrongSize = 0;
        unsigned wrongParent = 0;
        typedef std::pair<PriorityItem*, PriorityItem*> ParentChild;
        QValueList<ParentChild> list;
        list.push_front(ParentChild(0, head));

        while (!list.empty()) {
            ParentChild current = list.front();
            list.pop_front();
            PriorityItem *left = current.second->left();
            PriorityItem *right = current.second->right();
            PriorityItem *parent = current.second->parent();
            unsigned expectedItemSize = 1;

            if (left != 0) {
                list.push_front(ParentChild(current.second, left));
                expectedItemSize += left->size();
            }
            if (right != 0) {
                list.push_front(ParentChild(current.second, right));
                expectedItemSize += right->size();
            }

            if (current.first != 0 &&
                current.second->higherPriority(current.first)) {

                ++wrongPrio;
            }
            if (parent != current.first) {
                ++wrongParent;
            }
            if (current.second->size() != expectedItemSize) {
                ++wrongSize;
            }
        }
        if (wrongPrio != 0) {
            defects += qApp->translate("priorityqueue",
				       "\nHeap condition is violated ") +
                QString::number(wrongPrio) + qApp->translate("priorityqueue",
							     " times.");
        }
        if (wrongParent != 0) {
	  defects += "\n" + QString::number(wrongParent) 
	    + qApp->translate("priorityqueue"," item(s) have")
	    + qApp->translate("priorityqueue"," incorrect parent set.");
        }
        if (wrongSize != 0) {
	  defects += "\n" + QString::number(wrongSize) 
	    + qApp->translate("priorityqueue"," item(s) have")
	    + qApp->translate("priorityqueue"," incorrect size.");
        }

        if (defects != QString::null) {
            defects = QString(qApp->translate("priorityqueue",
					      "Queue's size = ")) +
                QString::number(head->size()) + defects + "\n";
        }
    }
    return defects;
}


PriorityItem::PriorityItem()
{
    parent_ = 0;
    left_ = 0;
    right_ = 0;
    size_ = 0;
}

bool PriorityItem::isInQueue()
{
    return size_ != 0;
}

PriorityItem *PriorityItem::maxPriority(PriorityItem *first,
                                        PriorityItem *second)
{
    if (first == 0 || (second != 0 && second->higherPriority(first))) {
        return second;
    }
    else {
        return first;
    }
}


void PriorityItem::updatePriority()
{
    updateDownward();
    updateUpward();
}

PriorityItem *PriorityItem::removeFromQueue()
{
    PriorityItem *oldParent = this->parent();
    PriorityItem *oldLeft = this->left();
    PriorityItem *oldRight = this->right();
    if (oldParent != 0) {
        if (oldParent->right() == this) {
	    // swap children, to make left subtree empty
	    oldParent->setRight(oldParent->left());
	    // oldParent->setLeft(this); need not be done here.
        }
	else {
	    Q_ASSERT(oldParent->left() == this);
	}
    }
    
    this->parent_ = 0;
    this->left_ = 0;
    this->right_ = 0;
    this->size_ = 0;

    PriorityItem *greater;
    do {
        // find greater subtree
        greater = PriorityItem::maxPriority(oldLeft, oldRight);

        // is there any subtree?
        if (greater != 0) {
            // install root of greater subtree under parent if parent exists
            PriorityItem *oldGrandLeft = greater->left();
            PriorityItem *oldGrandRight = greater->right();
            if (oldParent != 0) {
                oldParent->setLeft(greater);
            }
            else {
                greater->parent_ = 0;
            }
            // install greater child under the parent and prepare an empty
            // position for the greater grand child to be added in the
            // next iteration
            if (greater == oldLeft) {
                greater->setLeft(0);
                greater->setRight(oldRight);
            }
            else {
                Q_ASSERT(greater == oldRight);
                greater->setLeft(0);
                greater->setRight(oldLeft);
            }
            // perform next iteration on greater child's children, use
            // greater child as parent
            oldParent = greater;
            oldLeft = oldGrandLeft;
            oldRight = oldGrandRight;
        }
        else if (oldParent != 0) {
            // greater == 0, no greater subtree --> remove this
            oldParent->setLeft(0);
        }
    } while (greater != 0);

    if (oldParent != 0) {
        return oldParent->updateSizeUpward();
    }
    else {
        return 0;
    }
}

void PriorityItem::updateDownward()
{
    PriorityItem *greater;
    do {
        PriorityItem *oldLeft = this->left();
        PriorityItem *oldRight = this->right();

        // find greater child, if any
        greater = PriorityItem::maxPriority(oldLeft, oldRight);

        if (greater != 0 && !greater->higherPriority(this)) {
            // do not update, if there exists no child or if this has
            // equal or even higher priority
            greater = 0;
        }
        if (greater != 0) {
            // must move down
            PriorityItem *oldParent = this->parent();
            PriorityItem *oldGrandLeft = greater->left();
            PriorityItem *oldGrandRight = greater->right();
            // raise greater child under this's parent
            if (oldParent == 0) {
                greater->parent_ = 0;
            }
            else if (oldParent->left() == this) {
                oldParent->setLeft(greater);
            }
            else {
                Q_ASSERT(oldParent->right() == this);
                oldParent->setRight(greater);
            }
            // move this under raised greater child
            if (greater == oldLeft) {
                greater->setLeft(this);
                greater->setRight(oldRight);
            }
            else {
                Q_ASSERT(greater == oldRight);
                greater->setLeft(oldLeft);
                greater->setRight(this);
            }
            this->setLeft(oldGrandLeft);
            this->setRight(oldGrandRight);
            // adjust sizes
            this->updateSize();
            greater->updateSize();
        }
    } while (greater != 0);
}

void PriorityItem::updateUpward()
{
    // replace parent?
    while (!this->isHead() && this->higherPriority(parent())) {
        PriorityItem *oldParent = parent();
        PriorityItem *oldGrandParent = oldParent->parent();
        PriorityItem *oldLeft = left();
        PriorityItem *oldRight = right();

        // is this left or right child?
        if (this == oldParent->left()) {
            this->setLeft(oldParent);
            this->setRight(oldParent->right());
        }
        else {
            Q_ASSERT(oldParent->right() == this);
            this->setLeft(oldParent->left());
            this->setRight(oldParent);
        }
        oldParent->setLeft(oldLeft);
        oldParent->setRight(oldRight);

        if (oldGrandParent != 0) {
            if (oldGrandParent->left() == oldParent) {
                oldGrandParent->setLeft(this);
            }
            else {
                Q_ASSERT(oldGrandParent->right() == oldParent);
                oldGrandParent->setRight(this);
            }
        }
        else {
            this->parent_ = 0;
        }
        oldParent->updateSize();
        this->updateSize();
    }
}

PriorityItem *PriorityItem::updateSizeUpward()
{
    PriorityItem *current = this;
    while (true) {
        current->updateSize();
        PriorityItem *next = current->parent();
        if (next == 0) {
            return current;
        }
        current = next;
    }
}

PriorityItem *PriorityItem::parent() const
{
    return parent_;
}

void PriorityItem::setLeft(PriorityItem *left)
{
    left_ = left;
    if (left != 0) {
        left->parent_ = this;
    }
}

PriorityItem *PriorityItem::left() const
{
    return left_;
}

void PriorityItem::setRight(PriorityItem *right)
{
    right_ = right;
    if (right != 0) {
        right->parent_ = this;
    }
}

PriorityItem *PriorityItem::right() const
{
    return right_;
}

void PriorityItem::updateSize()
{
    size_ = 1;
    if (left_ != 0) {
        size_ += left_->size();
    }
    if (right_ != 0) {
        size_ += right_->size();
    }
}

unsigned PriorityItem::size() const
{
    return size_;
}


bool PriorityItem::isHead() const
{
    return parent_ == 0;
}
