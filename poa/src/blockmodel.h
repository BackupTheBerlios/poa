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
 * $Id: blockmodel.h,v 1.16 2003/09/09 14:04:44 vanto Exp $
 *
 *****************************************************************************/


#ifndef POA_BLOCKMODEL_H
#define POA_BLOCKMODEL_H

class QString;

#include "abstractmodel.h"
class PinVector;
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
public:

    /**
     * Creates a BlockModel instance
     * @param name the name of this block instance
     * @param description the description of this block instance
     * @param type the name of the library item (type)
     */
    BlockModel(QString type, QString description);

    /**
     * Default destructor
     */
    virtual ~BlockModel();

    /**
     * Returns <code>true</code> if this cannot itself be contained in a
     * project, but produce instances. Returns <code>false</code> if this
     * can be contained in a project.
     */
//    virtual bool isProducer() const;

    /**
     * Creates an instance that can be inserted into a project.
     *
     * Precondition:
     *   this->isProducer()
     */
//   virtual BlockModel *getInstance ();


    /**
     * Returns a list of all episodic pins of this.
     */
    PinVector *episodicPins();

    /**
     * Returns a list of all input pins of this.
     */
    PinVector *inputPins();

    /**
     * Returns a list of all output pins of this.
     */
    PinVector *outputPins();


    /**
     * Sets the clock count in milliseconds of this.
     * @todo Move this to CpuModel?
     */
    void setClock(unsigned int clock);

    /**
     * Returns the clock count in milliseconds of this.
     * @todo Move this to CpuModel?
     */
    unsigned int clock();

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
     * Toggles the automatic offset calculation.
     */
    void setAutoOffset(bool autoOffset);

    /**
     * Indicates if the automatic offset calculation should be used.
     */
    bool autoOffset();


    /**
     * Adds an input pin to this
     */
    void addInputPin(PinModel *pin, PinModel *successor = 0);

    /**
     * Removes an input pin from this
     */
    void removeInputPin(PinModel *pin);

    /**
     * Adds an output pin to this
     */
    void addOutputPin(PinModel *pin, PinModel *successor = 0);

    /**
     * Removes an output pin from this
     */
    void removeOutputPin(PinModel *pin);

    /**
     * Adds an episodic pin to this
     */
    void addEpisodicPin(PinModel *pin, PinModel *successor = 0);

    /**
     * Removes an episodic pin from this
     */
    void removeEpisodicPin(PinModel *pin);

    /**
     * Finds a pin according to its id
     */
    PinModel *findPinById(const unsigned id);

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
     * Creates the CanvasItems for this.
     */
    virtual QCanvasItemList createView(QCanvas *canvas);

    /**
     * Returns tooltip text
     */
    virtual QString tip();

private:

    PinVector *episodicPins_;
    PinVector *outputPins_;
    PinVector *inputPins_;
    unsigned long clock_;
    unsigned long offset_;
    bool autoOffset_;
    unsigned currentPinId_;
};


#endif // POA_BLOCKMODEL_H
