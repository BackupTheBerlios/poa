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
 * $Id: colormanager.h,v 1.3 2004/01/22 22:10:27 vanto Exp $
 *
 *****************************************************************************/

#ifndef POA_COLORMANAGER_H
#define POA_COLORMANAGER_H

#include "abstractmodel.h"
#include "blockmodel.h"

#include <qcolor.h>
#include <qmap.h>
#include <qptrlist.h>

class Palette;

class ColorManager
{
    //    Q_OBJECT
 public:

    /**
     * Instanciates a color manager.
     */
    ColorManager();

    /**
     * Default destructor.
     */
    ~ColorManager();

    /**
     * Returns the
     */
    QColor color_(const BlockModel *model, int luminance = 100);
    QColor color(AbstractModel *model, int luminance = 100);
    QColor activatedColor(AbstractModel *model, int luminance = 100);
    QColor selectedColor(AbstractModel *model, int luminance = 100);

 private:
    Palette *palette_;
    QMap<int, int> nsToPalIndex_;
    int palPosition_;

};

class Palette
{

 public:
    Palette(QString name);
    QString name();
    QColor color(int index) const;
    int size();
    void addColor(const QColor color);

 private:
    QString name_;
    QValueList<QColor> colorList_;
};

#endif // POA_COLORMANAGER_H
