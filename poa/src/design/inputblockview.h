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
 * $Id: inputblockview.h,v 1.2 2003/07/14 14:21:48 garbeam Exp $
 *
 *****************************************************************************/

#ifndef POA_INPUTBLOCKVIEW_H
#define POA_INPUTBLOCKVIEW_H

#include <qcanvasitem.h>
#include "inputblockmodel.h"

/*****************************************************************************
 * Definition of a input block view.
 * @author garbeam
 */
class InputBlockView: public QCanvasItem
{

private:

    InputBlockModel *inputBlockModel_;

public:

    InputBlockModel *getModel();
    void setModel(InputBlockModel *cpuModel);

};

#endif // POA_CPUVIEW_H
