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
 * $Id: blockmodel.h,v 1.42 2004/02/16 16:24:01 squig Exp $
 *
 *****************************************************************************/


#ifndef POA_BLOCKMODEL_H
#define POA_BLOCKMODEL_H

#include <qptrlist.h>

class QDomElement;
class QString;

#include "abstractmodel.h"
class GridCanvas;
class PinModel;

/**
 * Represents a block. A block can be either be stored in the module
 * library or in a project. The BlockModel class acts as the concrete
 * or super class for all types of blocks like input, output, core and
 * cpu blocks.
 *
 * The BlockModel class provides all common properties like clock,
 * offset and runtime. It also stores a list of PinModel objects. The
 * type of a block is determined by the kind of pins it has. Output
 * blocks for instance only have input pins, so hasInputPins() will
 * return true where as the other has... methods will return false.
 *
 * BlockModel objects are represented by BlockView objects on screen.
 *
 * @see BlockView
 */
class BlockModel: public AbstractModel
{
    Q_OBJECT

public:

    /**
     * Constructs a block.
     *
     * @param type the type of the library item, also used for the name property
     * @param description the description of this block instance
     */
    BlockModel(QString type, QString description);

    /**
     * Constructs a block, deserializing its properties from element.
     *
     * @param element stores the properties
     * @see #deserialize(QDomElement)
     */
    BlockModel(QDomElement element);

    /**
     * Deletes all pins.
     */
    virtual ~BlockModel();

    /**
     * Adds pin to this block. Pin will be deleted by the block. Emits
     * pinAdded(pin).
     */
    void addPin(PinModel *pin);

    /**
     * Returns true, if the automatic offset calculation should be used.
     */
    bool autoOffset() const;

    /**
     * Returns the clock of the block.
     */
    unsigned int clock() const;

    /**
     * Returns a list of all PinModel objects that are connected to input.
     */
    virtual QPtrList<PinModel> connectionsForInputPin(PinModel *input);

    /**
     * Creates the CanvasItems for the block.
     */
    virtual QCanvasItemList createView(GridCanvas *canvas);

    /**
     * Removes pin from the block model and deletes pin.
     */
    void deletePin(PinModel *pin);

    /**
     * Returns true, if the block has episodic pins.
     */
    virtual bool hasEpisodicPins() const;

    /**
     * Returns true, if the block has input pins.
     */
    virtual bool hasInputPins() const;

    /**
     * Returns true, if the block has output pins.
     */
    virtual bool hasOutputPins() const;

    /**
     * Returns true, if the block has a runtime property.
     */
    virtual bool hasRuntime() const;

    /**
     * Finds a pin according to its id
     */
    PinModel *findPinById(const unsigned id);

    /**
     * Returns the starting offset of the block.
     */
    unsigned int offset() const;

    /**
     * Returns all pins
     */
    QValueList<PinModel*> pins() const;

    /**
     * Returns the runtime of the block.
     */
    unsigned int runtime() const;

    /**
     * Sets the automatic offset calculation.
     */
    void setAutoOffset(const bool autoOffset);

    /**
     * Sets the clock of the block.
     */
    void setClock(const unsigned int clock);

    /**
     * Set to true, if block has episodic pins.
     */
    void setHasEpisodicPins(const bool hasEpisodicPins);

    /**
     * Set to true, if block has input pins.
     */
    void setHasInputPins(const bool hasInputPins);

    /**
     * Set to true, if block has output pins.
     */
    void setHasOutputPins(const bool hasOutputPins);

    /**
     * Set to true, if block has a runtime.
     */
    void setHasRuntime(const bool hasRuntime);

    /**
     * Sets the starting offset of the block.
     */
    void setOffset(const unsigned int offset);

    /**
     * Sets the runtime of the block.
     */
    void setRuntime(const unsigned int runtime);

    /**
     * Serializes the block to an xml subtree.
     *
     * @param document the parent document, needed to create elements.
     * @return the node, storing all persistent block properties
     */
    virtual QDomElement serialize(QDomDocument *document);

    /**
     * Returns the tooltip text.
     */
    virtual QString tip();

    /**
     * The position of the first pin. Usually either 0 or 1. Used by
     * the BlockConfDialog as the index of the first pin of each type.
     */
    static const int FIRST_PIN_POSISION;

protected:

    /**
     * Sets the properties of the block from element.
     *
     * @param element stores the properties
     */
    virtual void deserialize(QDomElement element);

    /**
     * Returns a tooltip string for auto values.
     */
    QString toTip(bool autoValue, int value);

private:

    bool autoOffset_;
    unsigned int clock_;
    unsigned currentPinId_;
    bool hasEpisodicPins_;
    bool hasInputPins_;
    bool hasOutputPins_;
    bool hasRuntime_;
    unsigned long offset_;
    unsigned int runtime_;

    /**
     * Stores the PinModel objects. Each PinModel has a block-unique id.
     */
    QMap<uint, PinModel *> pinById_;

signals:

    /**
     * Emitted after a pin has been added.
     */
    void pinAdded(PinModel *pin);

    /**
     * Emitted before the block is deleted.
     */
    void deleted(BlockModel *);

};


#endif // POA_BLOCKMODEL_H
