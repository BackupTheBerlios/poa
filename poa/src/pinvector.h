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
 * $Id: pinvector.h,v 1.8 2003/09/12 08:58:42 garbeam Exp $
 *
 *****************************************************************************/

#ifndef POA_PINVECTOR_H
#define POA_PINVECTOR_H

#include <qvaluevector.h>

#include "pinmodel.h"

typedef QValueVector<PinModel*> PinPointerVector;

/**
 * Collection of pinmodels.
 */
class PinVector: public PinPointerVector
{

public:

    /**
     * Adds <code>pin</code> before <code>successor</code> to this vector
     */
    void addBefore(PinModel *pin, PinModel *successor = 0);

    /**
     * Removes the given pin from this vector
     */
    void remove(PinModel *pin);

    /**
     * Finds a pin according to it's id
     */
    PinModel *findById(const unsigned id);

    /**
     * Removes and deletes(!) all pins from this vector.
     */
    void removeAllPins();


};


#endif // POA_PINVECTOR_H
