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
 * $Id: colormanager.cpp,v 1.2 2004/01/22 21:20:22 vanto Exp $
 *
 *****************************************************************************/

#include "colormanager.h"
#include "settings.h"

ColorManager::ColorManager()
{
    palPosition_ = 0;
    palette_ = new Palette("Web-Colors");
    palette_->addColor(QColor("#FF0000"));
    palette_->addColor(QColor("#FFA500"));
    palette_->addColor(QColor("#FFFF00"));
    palette_->addColor(QColor("#008000"));
    palette_->addColor(QColor("#0000FF"));
    palette_->addColor(QColor("#800080"));
    palette_->addColor(QColor("#7FFFDF"));
    palette_->addColor(QColor("#8A2BE2"));
    palette_->addColor(QColor("#A52A2A"));
    palette_->addColor(QColor("#DEB887"));
    palette_->addColor(QColor("#5F9EA0"));
    palette_->addColor(QColor("#7FFF00"));
    palette_->addColor(QColor("#D2691E"));
    palette_->addColor(QColor("#FF7F50"));
    palette_->addColor(QColor("#DC143C"));
}

ColorManager::~ColorManager()
{
    delete palette_;
}

QColor ColorManager::color(const BlockModel *model, int luminance)
{
    if (model->clock() == 0) {
        return Settings::instance()->defaultColor();
    }

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

QColor ColorManager::activatedColor(const BlockModel *model, int luminance)
{
    if (model->clock() == 0) {
        return Settings::instance()->activatedColor();
    }
    return color(model, luminance + 50);
}

QColor ColorManager::selectedColor(const BlockModel *model, int luminance)
{
    if (model->clock() == 0) {
        return Settings::instance()->activatedColor();
    }
    return color(model, luminance + 50);
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
