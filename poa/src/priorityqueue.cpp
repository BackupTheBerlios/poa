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
 * $Id: priorityqueue.cpp,v 1.4 2003/12/16 16:30:22 keulsn Exp $
 *
 *****************************************************************************/


#include "priorityqueue.h"

#include "poa.h"



PriorityQueue::PriorityQueue()
{
    head_ = 0;
}

PriorityQueue::~PriorityQueue()
{
    clear();
}

void PriorityQueue::insert(PriorityItem *item)
{
    Q_ASSERT(item != 0 && !item->isInQueue());

    PriorityItem *oldParent = 0;
    PriorityItem *oldLeft;
    PriorityItem *oldRight;
    PriorityItem *larger;
    PriorityItem *smaller;
    bool mustInsert = false;

    if (head_ == 0) {
	item->parent_ = 0;
	head_ = item;
    }
    else if (!head_->higherPriority(item)) {
	// insert directly before head
	oldLeft = head_->left();
	oldRight = head_->right();
	PriorityItem *newChild = head_;

	head_ = item;
	head_->parent_ = 0;
	
	// swap former head and item
	item = newChild;
	
	if (oldLeft == 0) {
	    head_->setRight(oldRight);
	    head_->setLeft(newChild);
	    newChild->setLeft(0);
	    newChild->setRight(0);
	}
	else if (oldRight == 0) {
	    head_->setLeft(oldLeft);
	    head_->setRight(newChild);
	    newChild->setLeft(0);
	    newChild->setRight(0);
	}
	else {
	    if (oldRight->size() > oldLeft->size()) {
		larger = oldRight;
		smaller = oldLeft;
	    }
	    else {
		larger = oldLeft;
		smaller = oldRight;
	    }
	    oldParent = head_;
	    mustInsert = true;
	}
    }
    else {
	// sink into heap until both children (oldLeft and oldRight) have
	// equal or less priority than item or until there are less than
	// two children
	oldParent = head_;
	while (true) {
	    oldLeft = oldParent->left();
	    oldRight = oldParent->right();
	    
	    if (oldLeft == 0) {
		oldParent->setLeft(item);
		break;
	    }
	    else if (oldRight == 0) {
		oldParent->setRight(item);
		break;
	    }
	    else {
		PriorityItem *smaller;
		PriorityItem *larger;
		if (oldRight->size() < oldLeft->size()) {
		    smaller = oldRight;
		    larger = oldLeft;
		}
		else {
		    smaller = oldLeft;
		    larger = oldRight;
		}
		if (smaller->higherPriority(item)) {
		    oldParent = smaller;
		}
		else if (larger->higherPriority(item)) {
		    oldParent = larger;
		}
		else {
		    item->setLeft(oldLeft);
		    item->setRight(oldRight);
		    mustInsert = true;
		}
	    }
	}
    }

    if (mustInsert) {
	// avoid empty subtrees, only executed if there was no empty subtree
	// into which the item could be inserted.
	oldParent->setRight(item);
	
	while (true) {
	    oldParent->setLeft(larger);
	    if (larger == oldLeft) {
		item->setLeft(0);
	    }
	    else {
		Q_ASSERT(larger == oldRight);
		item->setLeft(0);
		item->setRight(smaller);
	    }
	    oldParent = item;
	    item = smaller;
	    oldLeft = item->left();
	    oldRight = item->right();
	    if (oldRight == 0) {
		// item's right subtree is empty --> move left up and break
		item->setLeft(0);
		oldParent->setLeft(oldLeft);
		break;
	    }
	    else if (oldLeft == 0) {
		// item's left subtree is empty --> move right up and break
		item->setRight(0);
		oldParent->setLeft(oldRight);
		break;
	    }
	    else {
		if (oldRight->size() > oldLeft->size()) {
		    larger = oldRight;
		    smaller = oldLeft;
		}
		else {
		    larger = oldLeft;
		    smaller = oldRight;
		}
	    }
	}
    }

    PriorityItem *newHead = item->updateSizeUpward();
    Q_ASSERT(newHead == head_);
}

void PriorityQueue::remove(PriorityItem *item)
{
    Q_ASSERT(item != 0 && item->isInQueue());
    head_ = item->removeFromQueue();
}


void PriorityQueue::clear()
{
    PriorityItem *current = head_;
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
    return head_;
}

PriorityItem *PriorityQueue::removeHead()
{
    Q_ASSERT(head_ != 0);
    PriorityItem *oldHead = head_;
    if (head_ != 0) {
	head_ = head_->removeFromQueue();
    }
    return oldHead;
}

bool PriorityQueue::isEmpty() const
{
    return head_ == 0;
}

unsigned PriorityQueue::size() const
{
    Q_ASSERT(head_ != 0);
    return head_->size();
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
    if (first == 0) {
	return second;
    }
    else if (second == 0 || !second->higherPriority(first)) {
	return first;
    }
    else {
	return second;
    }
}


void PriorityItem::updatePriority()
{
    updateDownward();
    updateUpward();
}

PriorityItem *PriorityItem::removeFromQueue()
{
    void (PriorityItem::* installParent) (PriorityItem*) = 0;
    PriorityItem *oldParent = this->parent();
    PriorityItem *oldLeft = this->left();
    PriorityItem *oldRight = this->right();
    if (oldParent != 0) {
	if (oldParent->left() == this) {
	    installParent = &PriorityItem::setLeft;
	}
	else {
	    Q_ASSERT(oldParent->right() == this);
	    installParent = &PriorityItem::setRight;
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
		(oldParent->*installParent)(greater);
	    }
	    else {
		greater->parent_ = 0;
	    }
	    // install greater child under the parent and prepare an empty
	    // position for the greater grand child to be added in the
	    // next iteration
	    if (greater == oldLeft) {
		greater->setLeft(0);
		installParent = &PriorityItem::setLeft;
		greater->setRight(oldRight);
	    }
	    else {
		Q_ASSERT(greater == oldRight);
		greater->setLeft(oldLeft);
		greater->setRight(0);
		installParent = &PriorityItem::setRight;
	    }
	    // perform next iteration on greater child's children, use
	    // greater child as parent
	    oldParent = greater;
	    oldLeft = oldGrandLeft;
	    oldRight = oldGrandRight;
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
