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
 * $Id: muxmodel.h,v 1.32 2004/02/20 19:15:38 garbeam Exp $
 *
 *****************************************************************************/


#ifndef POA_MUXMODEL_H
#define POA_MUXMODEL_H

#include "blockmodel.h"

#include <qptrlist.h>

class MuxModel;

/**
 * Provides range mappings between input and output
 * PinModels.
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
     * @param input the input PinModel.
     * @param output the output PinModel.
     * @param firstInputBit range start
     * @param lasstInputBit range end 
     * @param firstOutputBit range start
     * @param lastOutputBit range end
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

    /** Returns the id of this mapping. */
    unsigned id();

    /** Sets the id of this mapping. */
    void setId(unsigned id);

private:

    PinModel *input_;
    PinModel *output_;

    unsigned firstInputBit_;
    unsigned firstOutputBit_;
    unsigned lastInputBit_;
    unsigned lastOutputBit_;

    unsigned id_;
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
     * If this model will be detroyed, all MuxMappings
     * will be deleted automatically.
     * The input and output PinModel must be valid.
     */
    void addMuxMapping(MuxMapping *mapping);

    /**
     * Removes the given mux mapping.
     * The given MuxMapping won't be deleted.
     */
    void removeMuxMapping(MuxMapping *mapping);

    /**
     * Finds a mux mapping by id. Returns the MuxMapping
     * if it was fount, <code>0<\code> otherwise.
     */
    MuxMapping *findMuxMappingById(unsigned id);

    /**
     * Serializes this instance to a xml subtree
     * @param document the main QDomDocument instance.
     * Needed to create elements
     */
    QDomElement serialize(QDomDocument *document);

    /**
     * Deserializes an xml subtree and sets this' properties
     */
    void deserialize(QDomElement element);


    /** Returns MuxMapping list. */
    QPtrList<MuxMapping> *mappings();

    /**
     *  Returns pointer list of all PinModels which are connected
     * by the input.
     * */
    virtual QPtrList<PinModel> connectionsForInputPin(PinModel *input);


private:

    void initIdCounter();

    /** MuxMapping's */
    QPtrList<MuxMapping> mappings_;

    unsigned idCounter_;


// signals are inherited from BlockModel
};


#endif // POA_MUXMODEL_H
