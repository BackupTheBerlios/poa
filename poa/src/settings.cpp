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
 * $Id: settings.cpp,v 1.5 2003/08/22 16:50:51 squig Exp $
 *
 *****************************************************************************/
#include "settings.h"


/*****************************************************************************
 * A common prefix used for all setting keys.
 */
const char *Settings::prefix = "/POA/POA/";

/*****************************************************************************
 * The singleton instance.
 */
Settings* Settings::instance_ = 0;

Settings::Settings()
{
    QSettings settings;
    settings.insertSearchPath(QSettings::Windows, "/POA");

    setDefault(&settings, "Grid Size", "10");
    setDefault(&settings, "MainWindow/Height", "480");
    setDefault(&settings, "MainWindow/Width", "640");
}

Settings::~Settings()
{
}

Settings *Settings::instance()
{
    if (instance_ == 0) {
        instance_ = new Settings();
    }

    return instance_;
}

QString Settings::get(const QString &key)
{
    QSettings settings;
    return settings.readEntry(prefix + key);
}

bool Settings::getBool(const QString &key)
{
    QSettings settings;
    return settings.readBoolEntry(prefix + key);
}


int Settings::getNum(const QString &key)
{
    QSettings settings;
    return settings.readNumEntry(prefix + key);
}


bool Settings::set(const QString &key, const QString &value)
{
    QString oldValue = get(key);
    if (oldValue != value) {
        QSettings settings;
        settings.writeEntry(prefix + key, value);
        emit settingChanged(key);
        return TRUE;
    }
    return FALSE;
}

bool Settings::set(const QString &key, bool value)
{
    bool oldValue = getBool(key);
    if (oldValue != value) {
        QSettings settings;
        settings.writeEntry(prefix + key, value);
        emit settingChanged(key);
        return TRUE;
    }
    return FALSE;
}

bool Settings::set(const QString &key, int value)
{
    int oldValue = getNum(key);
    if (oldValue != value) {
        QSettings settings;
        settings.writeEntry(prefix + key, value);
        emit settingChanged(key);
        return TRUE;
    }
    return FALSE;
}

void Settings::setDefault(QSettings* settings, const QString &key,
                          const QString &value)
{
    bool success;
    settings->readEntry(prefix + key, QString::null, &success);
    if (!success) {
        // key does not exists
        settings->writeEntry(prefix + key, value);
    }
}

int Settings::gridSize()
{
    return getNum("Grid Size");
}

void Settings::setGridSize(int gridSize)
{
    if (set("Grid Size", gridSize)) {
        emit gridSizeChanged(gridSize);
    }
}

bool Settings::snapToGrid()
{
    return getBool("Snap To Grid");
}

void Settings::setSnapToGrid(bool snapToGrid)
{
    if (set("Snap To Grid", snapToGrid)) {
        emit snapToGridChanged(snapToGrid);
    }
}
