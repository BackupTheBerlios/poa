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
 * $Id: problemmanager.cpp,v 1.5 2004/01/21 10:28:10 kilgus Exp $
 *
 *****************************************************************************/

#include "blockgraph.h"
#include "blockmodel.h"
#include "pinmodel.h"
#include "problemmanager.h"

#include <qmessagebox.h>
#include <qpushbutton.h>

//--- ProblemReportItem ---

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

    if (status == "Warning") {
        setPixmap(0, QMessageBox::standardIcon(QMessageBox::Warning));
    }
    else if (status == "Critical") {
        setPixmap(0, QMessageBox::standardIcon(QMessageBox::Critical));
    }
    else {
        setPixmap(0, QMessageBox::standardIcon(QMessageBox::Information));
    }
}

QString ProblemReportItem::shortDescription() const
{
    return text(0);
}

QString ProblemReportItem::status() const
{
    return text(1);
}

//--- DisconnectedPinReport ---

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

//--- DifferentWidthReport ---

DifferentWidthReport::DifferentWidthReport(QListViewItem *parent,
                                           PinModel *source, PinModel *target)
    : ProblemReportItem(parent, "Critical")
{
    source_ = source;
    target_ = target;

    setShortDescription(QString("%1 has a different width than %2")
                        .arg(source->absName()).arg(target->absName()));
    setLongDescription("Connected blocks need to have the same number of bits. "
                       "You can use a Mux in order to connect blocks with different widhts.");
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
    source_->updatePerformed();
    setFixed(true);
}

void DifferentWidthReport::adjustTarget()
{
    target_->setBits(source_->bits());
    target_->updatePerformed();
    setFixed(true);
}

//--- DifferentClockReport ---

DifferentClockReport::DifferentClockReport(QListViewItem *parent,
                                           BlockModel *source, BlockModel *target)
    : ProblemReportItem(parent, "Warning")
{
    // FIX: connect to deleted signals
    source_ = source;
    target_ = target;

    setShortDescription(QString("%1 has a different clock than %2")
                        .arg(source->name()).arg(target->name()));
    setLongDescription("Connected blocks should have the same clock.");
}

void DifferentClockReport::addWidgets(QWidget *widget)
{
    QPushButton *adjustSourceButton = new QPushButton(widget);
    adjustSourceButton->setText
        (QString("Set %1 clock to %2").arg(source_->name()).arg(target_->clock()));
    connect(adjustSourceButton, SIGNAL(clicked()), this, SLOT(adjustSource()));

    QPushButton *adjustTargetButton = new QPushButton(widget);
    adjustTargetButton->setText
        (QString("Set %1 clock to %2").arg(target_->name()).arg(source_->clock()));
    connect(adjustTargetButton, SIGNAL(clicked()), this, SLOT(adjustTarget()));
}

void DifferentClockReport::adjustSource()
{
    source_->setClock(target_->clock());
    source_->updatePerformed();
    setFixed(true);
}

void DifferentClockReport::adjustTarget()
{
    target_->setClock(source_->clock());
    target_->updatePerformed();
    setFixed(true);
}

//--- ProblemManager ---

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
    // iterate through all blocks
    for (QPtrListIterator<AbstractModel> it0(*project_->blocks()); it0 != 0;
         ++it0) {

        BlockModel *block = dynamic_cast<BlockModel*>(*it0);
        if (block != 0) {
            checkBlock(block);
        }
    }

    // iterate through reachable blocks
    BlockGraph graph(project_);
    QValueList<BlockNode*> blocks = graph.blocks();
    for (QValueList<BlockNode*>::Iterator it1 = blocks.begin();
         it1 != blocks.end(); ++it1) {

        QPtrList<BlockNode> neighbours = (*it1)->neighbours();
        for (QPtrListIterator<BlockNode> it2(neighbours); it2 != 0;
             ++it2) {

            if ((*it1)->clock() != (*it2)->clock()) {
                new DifferentClockReport(blockRoot_, (*it1)->model(),
                                         (*it2)->model());
            }
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

        // convenience access
        PinModel *pin = *it;

        // check if pin is connected at all
        if (pin->connected() == 0) {
            new DisconnectedPinReport(blockRoot_, pin);
        }
        else {
            // check if pin is connected to pin with the same width
            // the type is considered to make sure that connections are
            // only checked once
            if (pin->type() == PinModel::OUTPUT
                || (pin->type() == PinModel::EPISODIC
                    && pin->connected()->type() != PinModel::OUTPUT)) {

                if (pin->bits() != pin->connected()->bits()) {
                    new DifferentWidthReport(connectionRoot_, pin,
                                             pin->connected());
                }
            }
        }
    }
}

void ProblemManager::updateRoot(QListViewItem *item)
{
    item->setText(1,
                  (item->childCount() > 0)
                  ? QString("%1 Reports").arg(item->childCount())
                  : QString("OK"));
}
