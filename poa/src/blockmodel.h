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
 * $Id: blockmodel.h,v 1.7 2003/08/25 17:08:29 keulsn Exp $
 *
 *****************************************************************************/


#ifndef POA_BLOCKMODEL_H
#define POA_BLOCKMODEL_H

class QString;

#include "abstractmodel.h"
class PinVector;
class PinModel;

/*****************************************************************************
 * Base class for blocks.
 * A block can be either<br>
 * <ul>
 *   <li>Contained in the module library or</li>
 *   <li>Contained in a certain project</li>
 * </ul>
 * @author keulsn
 */
class BlockModel: public AbstractModel
{
public:

    BlockModel(QString name = QString::null,
               QString description = QString::null);
    virtual ~BlockModel();

    /*************************************************************************
     * Returns <code>true</code> if this cannot itself be contained in a
     * project, but produce instances. Returns <code>false</code> if this
     * can be contained in a project.
     */
//    virtual bool isProducer() const;

    /*************************************************************************
     * Creates an instance that can be inserted into a project.
     *
     * Precondition:
     *   this->isProducer()
     */
//   virtual BlockModel *getInstance ();


    /*************************************************************************
     * Returns a list of all episodic pins of this.
     */
    PinVector *episodicPins();
    PinVector *inputPins();
    PinVector *outputPins();

    void setClock(unsigned long clock);
    void setOffset(unsigned long offset);
    void setAutoOffset(bool autoOffset);

    void addEpisodicPin(PinModel *pin, PinModel *successor = 0);
    void removeEpisodicPin(PinModel *pin);

    QCanvasItemList createView(QCanvas *canvas);

private:

    PinVector *episodicPins_;
    PinVector *outputPins_;
    PinVector *inputPins_;
    unsigned long clock_;
    unsigned long offset_;
    bool autoOffset_;

};


#endif // POA_BLOCKMODEL_H
