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
 * $Id: pinview.h,v 1.1 2003/08/28 18:24:21 vanto Exp $
 *
 *****************************************************************************/

#ifndef POA_PINVIEW_H
#define POA_PINVIEW_H

#include <qcanvasitem.h>

#include "pinmodel.h"

/*****************************************************************************
 * Definition of a Pin view.
 * @author garbeam
 */
class PinView: public QCanvasItem
{

private:

    PinModel *pinModel_;

public:

    PinModel *getModel();
    void setModel(PinModel *cpuModel);

};

#endif // POA_PINVIEW_H
