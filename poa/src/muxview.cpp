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
 * $Id: muxview.cpp,v 1.1 2003/09/23 09:53:07 garbeam Exp $
 *
 *****************************************************************************/

#include "muxview.h"
#include "muxmodel.h"

MuxView::MuxView(MuxModel *model, QCanvas *canvas)
    : BlockView(
{
    model_ = model;

}

QString CpuView::tip()
{
    CodeManager *codeManager = CodeManager::instance();
    CpuModel *m = (CpuModel*)model();
    return QString("<b>CPU</b><br><u>%1</u> (%2)<br><i>%3</i><hr>" \
                   "<b>Id on CPLD:</b> %4<br>" \
                   "<b>Clock:</b> %5 ms<br>" \
                   "<b>Offset:</b> %6<br>" \
                   "<b>Execution time:</b> %7<br>" \
                   "<b>Source:</b> %8")
        .arg(m->name())
        .arg(m->type())
        .arg(m->description())
        //        .arg((m->cpuId()==-1)?"not defined":QString::number(m->cpuId()))
        .arg((m->cpuId()==-1)?"not defined":QString::number(m->cpuId()))
        .arg(m->clock())
        .arg((m->autoOffset())?"auto":QString::number(m->offset())+" ms")
        .arg((m->autoExecTime())?"auto":QString::number(m->execTime())+" ms")
        .arg(codeManager->sourceFilePath(m));
}
