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
 * $Id: outputmodel.h,v 1.2 2003/09/28 21:52:11 squig Exp $
 *
 *****************************************************************************/


#ifndef POA_OUTPUTMODEL_H
#define POA_OUTPUTMODEL_H


#include "coremodel.h"


/**
 * A block receiving outputs from the system and transmitting those outputs
 * to the environment.
 */
class OutputModel: public CoreModel
{
 public:
    OutputModel(QString type, QString description)
        : CoreModel(type, description) {}

    OutputModel(QDomElement element)
        : CoreModel(element) {}

    /**
     * Returns the XML representation of this instance
     */
    QDomElement serialize(QDomDocument *document)
        {
            QDomElement root = BlockModel::serialize(document);
            root.setAttribute("block-type", "output");
            return root;
        }

    virtual bool hasEpisodicPins() { return false; }

    virtual bool hasInputPins() { return false; }

    virtual bool hasRuntime() { return false; }

};

#endif
