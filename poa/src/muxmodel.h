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
 * $Id: muxmodel.h,v 1.25 2003/12/10 15:35:23 garbeam Exp $
 *
 *****************************************************************************/


#ifndef POA_MUXMODEL_H
#define POA_MUXMODEL_H

#include "blockmodel.h"

#include <qptrlist.h>

class MuxModel;

/**
 * Provides range mappings to an output
 * PinModel, e.g. map pins 2-8 of parent pin to output pin.
 */
class MuxMapping
{

public:

    /**
     * Dom constructor.
     */
    MuxMapping(MuxModel *parent, QDomElement mapElement);

    /**
     * Basic constructor.
     * @input the input PinModel.
     * @output the output (output) PinModel.
     * @begin the begin of bit range (0 for absolute begin).
     * @end the end of bit range (<code>input->bits() - 1</code>
     * for absolute end).
     */
    MuxMapping(PinModel *input, PinModel *output,
               unsigned firstInputBit, unsigned lastInputBit,
               unsigned firstOutputBit, unsigned lastOutputBit);

    virtual ~MuxMapping();

    /**
     * Returns the input PinModel.
     */
    PinModel *input() const;

    /**
     * Returns the output PinModel.
     */
    PinModel *output() const;

    /**
     * Sets input pin.
     */
    void setInput(PinModel *output);

    /**
     * Sets output pin.
     */
    void setOutput(PinModel *output);

    /** Returns the input range start bit. */
    unsigned firstInputBit() const;

    /** Returns the output range start bit. */
    unsigned firstOutputBit() const;

    /** Returns the input range end bit. */
    unsigned lastInputBit() const;

    /** Returns the output range end bit. */
    unsigned lastOutputBit() const;

    /** Sets the input range start bit. */
    void setFirstInputBit(unsigned firstInputBit);

    /** Sets the output range start bit. */
    void setFirstOutputBit(unsigned firstOutputBit);

    /** Sets the input range end bit. */
    void setLastInputBit(unsigned lastInputBit);

    /** Sets the output range end bit. */
    void setLastOutputBit(unsigned lastOutputBit);

    /**
     * Serializes the MuxMapping.
     */
    QDomElement serialize(QDomDocument *document);

    /**
     * Deserializes the MuxMapping.
     */
    void deserialize(MuxModel *parent, QDomElement element);

    /**
     * Returns clone of <code>this</code>. 
     * @param input the input PinModel which should be used for cloning.
     * If you need for some reason the original input pin use this
     * method like this: <code>mapping->clone(mapping->input());</code>
     */
    MuxMapping *clone(PinModel *input, PinModel *output);

private:

    PinModel *input_;
    PinModel *output_;

    unsigned firstInputBit_;
    unsigned firstOutputBit_;
    unsigned lastInputBit_;
    unsigned lastOutputBit_;

};

///////////////////////////////////////////////////////////////////////////////

/**
 * A block that has both: inputs and outputs. Outputs are directly dependent
 * on inputs and the production of outputs takes no time.
 */
class MuxModel: public BlockModel
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

    /** Returns MuxMapping list. */
    QPtrList<MuxMapping> *mappings();

    /** Returns list pointer of all PinModels which are connected by input. */
    QPtrList<PinModel> connectionsForInputPin(PinModel *input);

private:

    /** MuxMapping's */
    QPtrList<MuxMapping> mappings_;

    MuxType type_;

// signals are inherited from BlockModel
};


#endif // POA_MUXMODEL_H
