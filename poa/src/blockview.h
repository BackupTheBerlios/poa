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
 * $Id: blockview.h,v 1.3 2003/08/22 13:31:37 keulsn Exp $
 *
 *****************************************************************************/

#ifndef POA_BLOCKVIEW_H
#define POA_BLOCKVIEW_H

#include <qbrush.h>
#include <qcanvas.h>
#include <qpainter.h>

#include "blockmodel.h"

/*****************************************************************************
 * Definition of a block view.
 * @author garbeam
 */
class BlockView: public QCanvasRectangle
{

 public:

    BlockView(BlockModel *model, QCanvas *canvas);
    virtual ~BlockView();

    BlockModel *getModel();
    void setModel(BlockModel *model);

    virtual int rtti() const;

 protected:

    virtual void drawShape (QPainter &p);

 private:

    BlockModel *blockModel_;

    static const int RTTI_ = 1000;

};

#endif // POA_BLOCKVIEW_H
