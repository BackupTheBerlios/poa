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
 * $Id: muxmodel.h,v 1.7 2003/09/22 10:43:33 garbeam Exp $
 *
 *****************************************************************************/


#ifndef POA_MUXMODEL_H
#define POA_MUXMODEL_H

#include "abstractmodel.h"
#include "pinvector.h"
#include "pinmodel.h"

/**
 * Provides range mappings from an input PinModel to an output
 * PinModel, e.g. map pins 2-8 of input pin to output pin.
 */
class MuxMapping : public QObject
{

    Q_OBJECT

public:

    /**
     * Basic constructor.
     * @input the input PinModel.
     * @output the output (output) PinModel.
     * @begin the begin of bit range (0 for absolute begin).
     * @end the end of bit range (<code>input->bits() - 1</code>
     * for absolute end).
     */
    MuxMapping(PinModel *input, PinModel *output,
               unsigned begin, unsigned end);

    virtual ~MuxMapping();

    /**
     * Returns the input PinModel.
     */
    PinModel *input();

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

private:

    PinModel *input_;
    PinModel *output_;
    unsigned begin_;
    unsigned end_;

public slots:
    /**
     * Deletes this mapping.
     */
    void deleteMapping();
};

/**
 * A block that has both: inputs and outputs. Outputs are directly dependent
 * on inputs and the production of outputs takes no time.
 */
class MuxModel: public AbstractModel
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
     * Adds a new MuxMapping.
     * @param output has to be valid not <code>NULL</code>. 
     * @param setOutputBits if <code>false</code> the bits in output
     * PinModel have to be set manually, otherwise <code>(end - begin)</code>
     * will be added to the current bits value of output PinModel
     * by this method.
     */
    void addMuxMapping(PinModel *input, PinModel *output,
                       unsigned begin, unsigned end, bool setOutputBits = true);

    /**
     * Overloaded for convenience.
     * Adds a new MuxMapping. Creates a new output PinModel and sets
     * it bits value to <code>(end - begin)</code>.
     */
    void addMuxMapping(PinModel *input, unsigned begin, unsigned end);

    /**
     * Adds a new pin to the dedicated PinVector (given by <code>type</code>)
     * and returns it.
     */
    PinModel *addPin(PinModel::PinType type);

    /**
     * Removes alle MuxMappings which are related to <code>input</code>
     * pin. If the involved output pin has more bits than the specific
     * mapping, it will be shrinked, otherwise it will be removed.
     */
    void removeMuxMappings(PinModel *input);

    /**
     * Removes the given pin from this MuxModel and any related
     * Mappings(!). 
     */
    void removePin(PinModel *pin);

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

private:

    /** Contains all I/O mappings, see {@link MuxMapping} for detail */
    QPtrList<MuxMapping> mappings_;

    /** input and output pins */
    PinVector *inputPins_;
    PinVector *outputPins_;
};


#endif // POA_MUXMODEL_H
