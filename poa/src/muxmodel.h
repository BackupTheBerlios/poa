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
 * $Id: muxmodel.h,v 1.13 2003/09/25 11:02:48 garbeam Exp $
 *
 *****************************************************************************/


#ifndef POA_MUXMODEL_H
#define POA_MUXMODEL_H

#include "abstractmodel.h"
#include "pinmodel.h"

#include <qptrlist.h>

/**
 * Provides range mappings to an output
 * PinModel, e.g. map pins 2-8 of parent pin to output pin.
 */
class MuxMapping
{

public:

    /**
     * Basic constructor.
     * @output the output (output) PinModel.
     * @begin the begin of bit range (0 for absolute begin).
     * @end the end of bit range (<code>input->bits() - 1</code>
     * for absolute end).
     */
    MuxMapping(PinModel *output, unsigned begin, unsigned end);

    virtual ~MuxMapping();

    /**
     * Returns the output PinModel.
     */
    PinModel *output();

    /**
     * Returns the begin of range bits.
     */
    unsigned begin();

    /**
     * Sets the begin of range bits.
     */
    void setBegin(unsigned begin);

    /**
     * Returns the end of range bits (at least <code>begin() + 1</code>).
     */
    unsigned end();

    /**
     * Sets the end of range bits.
     */
    void setEnd(unsigned end);

    /**
     * Serializes the MuxMapping.
     */
    QDomElement serialize(QDomDocument *document);

private:

    PinModel *output_;
    unsigned begin_;
    unsigned end_;

};

//////////////////////////////////////////////////////////////////////////////

class MuxPin 
{

public:

    MuxPin(PinModel *model);
    ~MuxPin();

    /**
     * Adds the given mapping to this mux pin.
     */
    void addMapping(MuxMapping *mapping);

    /**
     * Removes the given mapping from this mux pin.
     */
    void removeMapping(MuxMapping *mapping);

    /**
     * Returns QPtrList of mux mappings.
     */
    QPtrList<MuxMapping> *mappings();

    /**
     * Returns the pin model.
     */
    PinModel *model();

    /**
     * Serializes this MuxPin.
     */
    QDomElement serialize(QDomDocument *document);

private:

    PinModel *model_;

    QPtrList<MuxMapping> mappings_;

};

///////////////////////////////////////////////////////////////////////////////

/**
 * A block that has both: inputs and outputs. Outputs are directly dependent
 * on inputs and the production of outputs takes no time.
 */
class MuxModel: public AbstractModel
{

    Q_OBJECT

public:

    /**
     * Defines the enum MuxType.
     */
    enum MuxType {MUX, DEMUX};

    /**
     * Creates a MuxModel instance for the library
     */
    MuxModel(QString type, QString description);

    /**
     * Default destructor.
     */
    virtual ~MuxModel();

    /**
     * Creates a MuxModel instance for the project out of an xml subtree
     */
    MuxModel(QDomElement coreElement);

    /**
     * Adds mux pin.
     */
    void addMuxPin(MuxPin *pin);

    /**
     * Removes the given pin from this MuxModel and any related
     * Mappings(!). 
     */
    void removeMuxPin(MuxPin *pin);

    /**
     * Creates the CanvasItems for this.
     */
    virtual QCanvasItemList createView(QCanvas *canvas);

    /**
     * Serializes this instance to a xml subtree
     * @param document the main QDomDocument instance. Needed to create elements
     */
    QDomElement serialize(QDomDocument *document);

    /**
     * Deserializes an xml subtree and sets this' properties
     */
    void deserialize(QDomElement element);


    /**
     * Returns the type of this muxmodel.
     */
    MuxType muxType();

    /**
     * Returns mux pin pointer list.
     */
    QPtrList<MuxPin> *muxPins();

private:

    /** Contains all I/O mappings */
    QPtrList<MuxPin> muxPins_;

    MuxType type_;

signals:

    /**
     * Emitted by <code>this</code> destructor.
     * Should be connected to View slots.
     */
    void deleted();
};


#endif // POA_MUXMODEL_H
