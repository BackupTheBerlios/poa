/*****************************************************************************
 *
 *  POA - Programmierumgebung Offener Antrieb
 *
 *  Copyright (C) 2003 Necati Aydin, Anselm Garbe, Stefan Hauser,
 *  Steffen Keul, Marcel Kilgus, Steffen Pingel, Tammo van Lessen
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 *  $Id: blockmodel.h,v 1.3 2003/07/14 22:14:23 keulsn Exp $
 *
 *****************************************************************************/


#ifndef POA_BLOCKMODEL_H
#define POA_BLOCKMODEL_H


#include <qstring.h>

#include "modulelibraryitem.h"
#include "pinvector.h"


/*****************************************************************************
 *
 *  Base class for blocks. A block can be either
 *  - Contained in the module library or
 *  - Contained in a certain project
 *
 */
class BlockModel: public ModuleLibraryItem
{
  public:

    /*************************************************************************
     *
     *  Returns true if this cannot itself be contained in a project, but
     *  can produce instances. Returns false if this can be contained in a
     *  project.
     */
    virtual bool isProducer() const;

    /*************************************************************************
     *
     *  Creates an instance that can be inserted into a project.
     *
     *  Precondition:
     *    this->isProducer()
     */
    virtual BlockModel *getInstance ();


    void setName (QSting &name);
    void setId (unsigned long id);
    void setOffset (unsigned long offset);
    void setAutoOffset (bool autoOffset);

    void addEpisodicPin (PinModel *pin, PinModel *successor = 0);
    void removeEpisodicPin (PinModel *pin);

  private:

    QString *name_;
    PinVector *episodicPins_;
    unsigned long clock_;
    unsigned long offset_;
    bool autoOffset_;

};


#endif
