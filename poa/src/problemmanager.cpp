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
 * $Id: problemmanager.cpp,v 1.2 2004/01/19 13:56:18 squig Exp $
 *
 *****************************************************************************/

#include "blockgraph.h"
#include "blockmodel.h"
#include "pinmodel.h"
#include "problemmanager.h"

#include <qpushbutton.h>

ProblemReportItem::ProblemReportItem(QListViewItem *parent, QString status,
                                     QString shortDescription)
    : QListViewItem(parent, shortDescription)
{
    setFixed(false);
    setStatus(status);
    setShortDescription(shortDescription);
    setLongDescription(shortDescription);
}

void ProblemReportItem::addWidgets(QWidget *)
{
}

bool ProblemReportItem::isFixed() const
{
    return fixed_;
}

QString ProblemReportItem::longDescription() const
{
    return longDescription_;
}

void ProblemReportItem::setFixed(const bool fixed)
{
    fixed_ = fixed;
    if (fixed) {
        setStatus("Fixed");
        emit updated();
    }
}

void ProblemReportItem::setLongDescription(const QString longDescription)
{
    longDescription_ = longDescription;
}

void ProblemReportItem::setShortDescription(const QString shortDescription)
{
    setText(0, shortDescription);
}

void ProblemReportItem::setStatus(const QString status)
{
    setText(1, status);
}

QString ProblemReportItem::shortDescription() const
{
    return text(0);
}

QString ProblemReportItem::status() const
{
    return text(1);
}

DisconnectedPinReport::DisconnectedPinReport(QListViewItem *parent,
                                             PinModel *pin)
    : ProblemReportItem(parent, "Warning")
{
    // FIX: connect to deleted signal
    pin_ = pin;

    setShortDescription(QString("%1 is not connected").arg(pin->absName()));
    setLongDescription("Disconnected pins are considered bad style.");
}

void DisconnectedPinReport::addWidgets(QWidget *widget)
{
    QPushButton *deletePinButton = new QPushButton(widget);
    deletePinButton->setText(tr("&Delete Pin"));
    connect(deletePinButton, SIGNAL(clicked()), this, SLOT(deletePin()));
}

void DisconnectedPinReport::deletePin()
{
    if (pin_ != 0) {
        pin_->parent()->deletePin(pin_);
        pin_ = 0;
        setFixed(true);
    }
}

DifferentWidthReport::DifferentWidthReport(QListViewItem *parent,
                                           PinModel *source, PinModel *target)
    : ProblemReportItem(parent, "Error")
{
    // FIX: connect to deleted signals
    source_ = source;
    target_ = target;

    setShortDescription(QString("%1 has a different width than %2")
                        .arg(source->absName()).arg(target->absName()));
    setLongDescription("Connected pins need to have the same number of bits. "
                       "You can use a Mux in order to connect pins with different widhts.");
}

void DifferentWidthReport::addWidgets(QWidget *widget)
{
    QPushButton *adjustSourceButton = new QPushButton(widget);
    adjustSourceButton->setText
        (QString("Set %1 bits to %2").arg(source_->name()).arg(target_->bits()));
    connect(adjustSourceButton, SIGNAL(clicked()), this, SLOT(adjustSource()));

    QPushButton *adjustTargetButton = new QPushButton(widget);
    adjustTargetButton->setText
        (QString("Set %1 bits to %2").arg(target_->name()).arg(source_->bits()));
    connect(adjustTargetButton, SIGNAL(clicked()), this, SLOT(adjustTarget()));
}

void DifferentWidthReport::adjustSource()
{
    source_->setBits(target_->bits());
    setFixed(true);
}

void DifferentWidthReport::adjustTarget()
{
    target_->setBits(source_->bits());
    setFixed(true);
}

ProblemManager::ProblemManager(Project *project, QListView *listView)
    : project_(project)
{
    QListViewItem *root = new QListViewItem(listView, "Problem Reports");
    root->setOpen(true);

    connectionRoot_ = new QListViewItem(root, "Connections");
    blockRoot_ = new QListViewItem(root, "Blocks");
}

ProblemManager::~ProblemManager()
{
}

void ProblemManager::report()
{
    for (QPtrListIterator<AbstractModel> it(*project_->blocks()); it != 0;
         ++it) {

        BlockModel *block = dynamic_cast<BlockModel*>(*it);
        if (block != 0) {
            checkBlock(block);
        }
    }

    updateRoot(connectionRoot_);
    updateRoot(blockRoot_);
}

void ProblemManager::checkBlock(BlockModel *block)
{
    QValueList<PinModel*> pins = block->pins();
    for (QValueList<PinModel*>::Iterator it = pins.begin(); it != pins.end();
         ++it) {

        checkConnected((*it));

        if ((*it)->type() == PinModel::OUTPUT
            || ((*it)->type() == PinModel::EPISODIC
                && (*it)->connected() != 0
                && (*it)->connected()->type() != PinModel::OUTPUT)) {

            checkConnectionBits((*it));
        }
    }
}

void ProblemManager::checkConnected(PinModel *pin)
{
    if (pin->connected() == 0) {
        new DisconnectedPinReport(blockRoot_, pin);
    }
}

void ProblemManager::checkConnectionBits(PinModel *pin)
{
    if (pin->connected() && pin->bits() != pin->connected()->bits()) {
        new DifferentWidthReport(connectionRoot_, pin, pin->connected());
    }

}

void ProblemManager::updateRoot(QListViewItem *item)
{
    item->setText(1,
                  (item->childCount() > 0)
                  ? QString("%1 Reports").arg(item->childCount())
                  : QString("OK"));
}
