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
 * $Id: pathtablemodel.cpp,v 1.1 2004/01/26 00:40:58 keulsn Exp $
 *
 *****************************************************************************/

#include <pathtablemodel.h>


PathTableModel::SIZE_STEP = 10;


PathTableModel::PathTableModel(BlockGraph *graph)
{
    graph_ = graph;
    table_ = 0;
    beginningSize_ = SIZE_STEP;
    beginning_ = new TableEntry[beginningSize_];
    beginningCount_ = 0;
    endingSize_ = SIZE_STEP;
    ending_ = new TableEntry[endingSize_];
    endingCount_ = 0;
}


PathTableModel::~PathTableModel()
{
    delete [] beginning_;
    delete [] ending_;
}


QTable *PathTableModel::createTable(QWidget *parent, const char *name)
{
    QTable table = new QTable(tableHeight(), 3, parent, name);
    QHeader *topHeader = table->horizontalHeader();
    topHeader->setLabel(0, tr("Input Pin"));
    topHeader->setLabel(1, tr("Block"));
    topHeader->setLabel(2, tr("Output Pin"));
    table->setSorting(false);
    table->setSelectionMode(QTable::NoSelection);
    return table;
}


int PathTableModel::tableHeight() const
{
    return beginningCount_ + endingCount_ + 1;
}
