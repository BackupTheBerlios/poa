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
 * $Id: problemmanager.cpp,v 1.1 2004/01/19 11:23:07 squig Exp $
 *
 *****************************************************************************/

#include "blockgraph.h"
#include "pinmodel.h"
#include "problemmanager.h"

ProblemReportItem::ProblemReportItem(QListViewItem *parent,
                                     QString shortDescription)
    : QListViewItem(parent, shortDescription),
    longDescription_(shortDescription)
{
}

QString ProblemReportItem::longDescription()
{
    return longDescription_;
}

void ProblemReportItem::setLongDescription(QString longDescription)
{
    longDescription_ = longDescription;
}

QString ProblemReportItem::shortDescription()
{
    return text(0);
}

//  QValueList<QAction*> ProblemReportItem::actions()
//  {
//      return
//  }

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
    BlockGraph graph(project_);

    QValueList<PinNode*> pins = graph.pins();
    for (QValueList<PinNode*>::Iterator it = pins.begin(); it != pins.end();
         ++it) {

        if ((*it)->model()->type() == PinModel::OUTPUT) {
            checkConnectionBits((*it)->model());
            checkConnected((*it)->model());
        }
    }

    updateRoot(connectionRoot_);
    updateRoot(blockRoot_);
}

void ProblemManager::checkConnected(PinModel *pin)
{
    if (pin->connected() == 0) {
        ProblemReportItem *item = new ProblemReportItem
            (blockRoot_, QString("%1 is not connected").arg(pin->absName()));
    }
}

void ProblemManager::checkConnectionBits(PinModel *pin)
{
    if (pin->connected() && pin->bits() != pin->connected()->bits()) {
        ProblemReportItem *item = new ProblemReportItem
            (connectionRoot_, QString("%1 has a different width than %2")
             .arg(pin->absName()).arg(pin->connected()->absName()));
        item->setLongDescription("foo baz");
    }

}

void ProblemManager::updateRoot(QListViewItem *item)
{
    item->setText(1,
                  (item->childCount() > 0)
                  ? QString("%1 Errors").arg(item->childCount())
                  : QString("OK"));
}
