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
 * $Id: muxview.h,v 1.7 2003/11/24 16:37:41 squig Exp $
 *
 *****************************************************************************/

#ifndef POA_MUXVIEW_H
#define POA_MUXVIEW_H

#include "muxmodel.h"
#include "blockview.h"
#include "tooltipable.h"

/**
 * Definition of a mux view.
 */
class MuxView: public BlockView
{

public:

    /**
     * Creates a new MuxView on the given canvas.
     */
    MuxView(MuxModel *model, QCanvas *canvas) : BlockView(model, canvas){};

    /**
     * Returns the tooltip text.
     */
    virtual QString tip();

};

#endif // POA_MUXVIEW_H
