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
 * $Id: colormanager.cpp,v 1.5 2004/01/22 23:45:50 vanto Exp $
 *
 *****************************************************************************/

#include "colormanager.h"
#include "settings.h"


ColorManager::ColorManager(Palette *palette)
    : palette_(palette)
{
    palPosition_ = 0;
}

ColorManager::~ColorManager()
{
    delete palette_;
}

QColor ColorManager::color(const BlockModel *model, int luminance)
{
    // rotate palette
    if (palPosition_ > palette_->size()) {
        palPosition_ = 0;
    }

    if (!nsToPalIndex_.contains(model->clock())) {
        nsToPalIndex_.insert(model->clock(), palPosition_);
        return palette_->color(palPosition_++);
    }
    else {
        return palette_->color(nsToPalIndex_[model->clock()]).light(luminance);
    }
}

QColor ColorManager::color(AbstractModel *model, int luminance)
{
    BlockModel *bm = dynamic_cast<BlockModel*>(model);
    if ((bm != 0) && (bm->clock() != 0)) {
        return color(bm, luminance);

    }

    return Settings::instance()->defaultBrushColor();
}

QColor ColorManager::activatedColor(AbstractModel*, int)
{
    return Settings::instance()->activatedColor();
}

QColor ColorManager::selectedColor(AbstractModel*, int)
{
    return Settings::instance()->selectedColor();
}



Palette::Palette(QString name)
    : name_(name)
{
}

QString Palette::name()
{
    return name_;
}

QColor Palette::color(int index) const
{
    return colorList_[index];
}

int Palette::size()
{
    return colorList_.count();
}

void Palette::addColor(const QColor color)
{
    colorList_.append(color);
}

Palette *Palette::strongPalette()
{
    Palette *palette = new Palette("Strong Palette");

    /*  stronger colors */
    palette->addColor(QColor("#FF0000"));
    palette->addColor(QColor("#FFA500"));
    palette->addColor(QColor("#FFFF00"));
    palette->addColor(QColor("#008000"));
    palette->addColor(QColor("#0000FF"));
    palette->addColor(QColor("#800080"));
    palette->addColor(QColor("#7FFFDF"));
    palette->addColor(QColor("#8A2BE2"));
    palette->addColor(QColor("#A52A2A"));
    palette->addColor(QColor("#DEB887"));
    palette->addColor(QColor("#5F9EA0"));
    palette->addColor(QColor("#7FFF00"));
    palette->addColor(QColor("#D2691E"));
    palette->addColor(QColor("#FF7F50"));
    palette->addColor(QColor("#DC143C"));

    return palette;
}

Palette *Palette::lightPalette()
{
    Palette *palette = new Palette("Light Palette");

    /* lighter colors */
    palette->addColor(QColor("#ADD8E6")); // light blue
    palette->addColor(QColor("#F08080")); // light coral
    palette->addColor(QColor("#E0FFFF")); // light cyan
    palette->addColor(QColor("#FAFAD2")); // light goldenrod yellow
    palette->addColor(QColor("#90EE90")); // light green
    palette->addColor(QColor("#FFb6C1")); // light pink
    palette->addColor(QColor("#FFA07A")); // light salmon
    palette->addColor(QColor("#20B2AA")); // light sea green
    palette->addColor(QColor("#87CEFA")); // light sky blue
    palette->addColor(QColor("#778899")); // light slate gray
    palette->addColor(QColor("#B0C4DE")); // light steel blue
    palette->addColor(QColor("#FFFFE0")); // light yellow
    palette->addColor(QColor("#F5DEB3")); // wheat

    return palette;
}
