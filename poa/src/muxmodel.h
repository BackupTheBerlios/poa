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
 * $Id: muxmodel.h,v 1.17 2003/09/29 09:52:41 garbeam Exp $
 *
 *****************************************************************************/


#ifndef POA_MUXMODEL_H
#define POA_MUXMODEL_H

#include "abstractmodel.h"
#include "pinmodel.h"

#include <qptrlist.h>

// forward declaration for MuxMapping
class MuxPin;

/**
 * Provides range mappings to an output
 * PinModel, e.g. map pins 2-8 of parent pin to output pin.
 */
class MuxMapping
{

public:

    /**
     * Basic constructor.
     * @input the input PinModel.
     * @output the output (output) PinModel.
     * @begin the begin of bit range (0 for absolute begin).
     * @end the end of bit range (<code>input->bits() - 1</code>
     * for absolute end).
     */
    MuxMapping(MuxPin *muxPin, PinModel *output,
               unsigned begin, unsigned end);

    virtual ~MuxMapping();

    /**
     * Returns the input PinModel.
     */
    MuxPin *muxPin();

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

    /**
     * Returns clone of <code>this</code>. 
     * @param muxPin the muxPin which should be used for cloning.
     * Mostly, also the MuxPin has been cloned, so all the cloned
     * mappings should point to the cloned MuxPin instead of the
     * original one.
     * If you need for some reason the original MuxPin use this
     * method like this: <code>mapping->clone(mapping->muxPin());</code>
     * @param output equivalent to MuxPin, but this means the output
     * or also called "map to" pin.
     */
    MuxMapping *clone(MuxPin *muxPin, PinModel *output);

private:

    MuxPin *muxPin_;
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

    /**
     * Returns clone of <code>this</code>
     */
    MuxPin *clone();

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
     * Adds a new mux mapping.
     */
    void addMuxMapping(MuxMapping *mapping);

    /**
     * Removes the given mux mapping.
     */
    void removeMuxMapping(MuxMapping *mapping);

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
     * Returns pointer of mux pin pointer list.
     */
    QPtrList<MuxPin> *muxPins();

    /**
     * Returns pointer of output pinmodel list.
     */
    QPtrList<PinModel> *outputPins();

    /**
     * Returns pin model with name <code>name</code> if it exists,
     * otherwise <code>0</code>.
     */
    PinModel *outputForName(QString name);

private:

    /** Contains all I/O mappings */
    QPtrList<MuxPin> muxPins_;

    /** Contains all output pins */
    QPtrList<PinModel> outputPins_;

    MuxType type_;

signals:

    /**
     * Emitted when a mux pin or output pin was added.
     */
    void pinAdded(PinModel *pin);

    /**
     * Emitted by <code>this</code> destructor.
     * Should be connected to View slots.
     */
    void deleted();
};


#endif // POA_MUXMODEL_H
