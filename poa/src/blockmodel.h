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
 * $Id: blockmodel.h,v 1.41 2004/01/25 15:53:31 vanto Exp $
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
     * Adds an pin to this block model.
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
     * Returns a list of all PinModels that are connected to input.
     */
    virtual QPtrList<PinModel> connectionsForInputPin(PinModel *input);

    /**
     * Creates the CanvasItems for the block.
     */
    virtual QCanvasItemList createView(GridCanvas *canvas);

    /**
     * Deletes an input pin from this block model.
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
     * Serializes this instance to a xml subtree
     * @param document the main QDomDocument instance. Needed to create elements
     */
    virtual QDomElement serialize(QDomDocument *document);

    /**
     * Returns the tooltip text.
     */
    virtual QString tip();

    /**
     * The position of the first pin. Usually either 0 or 1.
     */
    static const int FIRST_PIN_POSISION;

protected:

    /**
     * Deserializes an xml subtree and sets this' properties
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

    QMap<uint, PinModel *> pinById_;

signals:

    /**
     * Emitted when a pin is added.
     */
    void pinAdded(PinModel *pin);

    /**
     * Emitted when model gets deleted
     */
    void deleted(BlockModel *);

};


#endif // POA_BLOCKMODEL_H
