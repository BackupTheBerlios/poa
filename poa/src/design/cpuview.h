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
 * $Id: cpuview.h,v 1.4 2003/08/16 18:00:09 garbeam Exp $
 *
 *****************************************************************************/

#ifndef POA_CPUVIEW_H
#define POA_CPUVIEW_H

#include "cpumodel.h"
#include "inputblockview.h"
#include "outputblockview.h"

/*****************************************************************************
 * Definition of a cpu view.
 * @author garbeam
 */
class CpuView: public BlockView
{

private:

    CpuModel *cpuModel_;

public:

    CpuModel *getModel();
    void setModel(CpuModel *cpuModel);

};

#endif // POA_CPUVIEW_H
