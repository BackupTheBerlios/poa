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
 * $Id: codemanager.cpp,v 1.1 2003/09/16 15:18:13 garbeam Exp $
 *
 *****************************************************************************/


#include "codemanager.h"
#include "cpumodel.h"
#include "settings.h"

CodeManager::CodeManager(AbstractModel *parent)
{
    model_ = parent;
}

CodeManager::~CodeManager()
{
}

QString CodeManager::sourcePath()
{
    CpuModel *model = (CpuModel *)model_;
    return QString("%1/CPU_%2_sdk").arg(*(model->projectPath()))
                                   .arg(model->id());
}

int CodeManager::compile()
{

    return 0;
}

void CodeManager::save()
{

}

void CodeManager::remove()
{

}