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
 * $Id: blockmodel.h,v 1.36 2004/01/09 14:05:29 squig Exp $
 *
 *****************************************************************************/


#ifndef POA_BLOCKMODEL_H
#define POA_BLOCKMODEL_H

#include <qptrlist.h>

class QDomElement;
class QString;

#include "abstractmodel.h"
class PinModel;

/**
 * Base class for blocks.
 * A block can be either<br>
 * <ul>
 *   <li>Contained in the module library or</li>
 *   <li>Contained in a certain project</li>
 * </ul>
 */
class BlockModel: public AbstractModel
{
    Q_OBJECT

public:

    /**
     * Creates a BlockModel instance
     * @param name the name of this block instance
     * @param description the description of this block instance
     * @param type the name of the library item (type)
     */
    BlockModel(QString type, QString description);

    /**
     * Creates a CpuModel instance for the project out of an xml element.
     */
    BlockModel(QDomElement element);

    /**
     * Default destructor
     */
    virtual ~BlockModel();

    /**
     * Creates the CanvasItems for the block.
     */
    virtual QCanvasItemList createView(QCanvas *canvas);

    /**
     * Returns a list of all episodic pins of this.
     */
    //QPtrList<PinModel> *episodicPins();

    /**
     * Returns true, if the block has episodic pins.
     */
    virtual bool hasEpisodicPins();

    /**
     * Returns true, if the block has input pins.
     */
    virtual bool hasInputPins();

    /**
     * Returns true, if the block has output pins.
     */
    virtual bool hasOutputPins();

    /**
     * Returns true, if the block has a runtime property.
     */
    virtual bool hasRuntime();

    /**
     * Returns a list of all input pins of this.
     */
    //QPtrList<PinModel> *inputPins();

    /**
     * Returns a list of all output pins of this.
     */
    //QPtrList<PinModel> *outputPins();

    /**
     * Returns the runtime (in ms) of this block
     */
    unsigned int execTime();

    /**
     * Sets the runtime (in ms) of this block
     */
    void setExecTime(const unsigned int time);

    /**
     * Returns the clock (in ms) of this block
     */
    unsigned int clock();

    /**
     * Sets the clock (in ms) of this block
     */
    void setClock(const unsigned int clock);

    /**
     * Set to true, if block has episodic pins.
     */
    void setHasEpisodicPins(bool hasEpisodicPins);

    /**
     * Set to true, if block has input pins.
     */
    void setHasInputPins(bool hasInputPins);

    /**
     * Set to true, if block has output pins.
     */
    void setHasOutputPins(bool hasOutputPins);

    /**
     * Set to true, if block has a runtime.
     */
    void setHasRuntime(bool hasRuntime);

    /**
     * Adds an pin to this block model.
     */
    void addPin(PinModel *pin);

    /**
     * Deletes an input pin from this block model.
     */
    void deletePin(PinModel *pin);

    /**
     * Returns all pins
     */
    QValueList<PinModel*> pins();

    /**
     * Finds a pin according to its id
     */
    PinModel *findPinById(const unsigned id);

    /**
     * Sets the starting offset in milliseconds of this.
     * @todo Move this to CpuModel?
     */
    void setOffset(unsigned int offset);

    /**
     * Returns the starting offset in milliseconds of this.
     * @todo Move this to CpuModel?
     */
    unsigned int offset();

    /**
     * Serializes this instance to a xml subtree
     * @param document the main QDomDocument instance. Needed to create elements
     */
    virtual QDomElement serialize(QDomDocument *document);

    /**
     * Deserializes an xml subtree and sets this' properties
     */
    virtual void deserialize(QDomElement element);

    /**
     * Returns the tooltip text.
     */
    virtual QString tip();

protected:

    //    QPtrList<PinModel> episodicPins_;
    //    QPtrList<PinModel> outputPins_;
    //    QPtrList<PinModel> inputPins_;
    unsigned currentPinId_;
    unsigned int execTime_;
    unsigned int clock_;

 private:
    bool hasEpisodicPins_;
    bool hasInputPins_;
    bool hasOutputPins_;
    bool hasRuntime_;
    unsigned long offset_;

    QMap<uint, PinModel *> pinById_;

signals:
    /**
     * Emitted when a pin is added.
     */
    void pinAdded(PinModel *pin);

    /**
     * Emitted when model gets deleted
     */
    void deleted();

};


#endif // POA_BLOCKMODEL_H
