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
 * $Id: settings.cpp,v 1.14 2003/10/01 15:55:17 squig Exp $
 *
 *****************************************************************************/
#include "settings.h"

#include "poa.h"

/**
 * A common prefix used for all setting keys.
 */
const char *Settings::PREFIX = "/POA/POA/";

/**
 * The singleton instance.
 */
Settings* Settings::instance_ = 0;

Settings::Settings()
{
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

QString Settings::get(const QString &key, QString defaultValue)
{
    QSettings settings;
    return settings.readEntry(PREFIX + key, defaultValue);
}

bool Settings::getBool(const QString &key, bool defaultValue)
{
    QSettings settings;
    return settings.readBoolEntry(PREFIX + key, defaultValue);
}


QFont Settings::getFont(const QString &key, QFont defaultValue)
{
    QFont font;
    font.fromString(get(PREFIX + key, defaultValue.toString()));
    return font;
}

int Settings::getNum(const QString &key, int defaultValue)
{
    QSettings settings;
    return settings.readNumEntry(PREFIX + key, defaultValue);
}

QStringList Settings::getStrings(const QString &key, bool *ok)
{
    QSettings settings;
    return settings.readListEntry(PREFIX + key, ok);
}



bool Settings::set(const QString &key, const QString &value)
{
    QString oldValue = get(key);
    if (oldValue != value) {
        QSettings settings;
        settings.writeEntry(PREFIX + key, value);
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
        settings.writeEntry(PREFIX + key, value);
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
        settings.writeEntry(PREFIX + key, value);
        emit settingChanged(key);
        return TRUE;
    }
    return FALSE;
}

bool Settings::set(const QString &key, const QStringList &value)
{
    QStringList oldValue = getStrings(key);
    if (oldValue != value) {
        QSettings settings;
        settings.writeEntry(PREFIX + key, value);
        emit settingChanged(key);
        return TRUE;
    }
    return FALSE;
}

//  void Settings::setDefault(QSettings* settings, const QString &key,
//                            const QString &value)
//  {
//      bool success;
//      settings->readEntry(PREFIX + key, QString::null, &success);
//      if (!success) {
//          // key does not exists
//          settings->writeEntry(PREFIX + key, value);
//      }
//  }

QColor Settings::activatedColor()
{
    return Qt::green;
}

void Settings::addToRecentProjects(QString filename)
{
    QStringList list = getStrings("RecentProjects");
    list.remove(filename);
    list.prepend(filename);
    while (list.size() > MAX_RECENT_PROJECTS) {
        list.pop_back();
    }
    if (set("RecentProjects", list)) {
        emit recentProjectsChanged();
    }
}

QColor Settings::defaultBrushColor()
{
    return Qt::white;
}

QColor Settings::defaultColor()
{
    return Qt::black;
}

int Settings::gridSize()
{
    return getNum("Grid Size", 10);
}

bool Settings::showGrid()
{
    return getBool("Show Grid", TRUE);
}

bool Settings::snapToGrid()
{
    return getBool("Snap To Grid", TRUE);
}

QColor Settings::selectedColor()
{
    return Qt::lightGray;
}

void Settings::setGridSize(int gridSize)
{
    if (set("Grid Size", gridSize)) {
        emit gridSizeChanged(gridSize);
    }
}

void Settings::setSnapToGrid(bool snapToGrid)
{
    set("Snap To Grid", snapToGrid);
}

void Settings::setShowGrid(bool showGrid)
{
    if (set("Show Grid", showGrid)) {
        emit showGridChanged(showGrid);
    }
}

QString Settings::templatePath()
{
    return get("Template Path", "templates/cpu_template.cpp");
}

QString Settings::compilerCmd()
{
    return get("Compiler", "gcc -v");
}

QString Settings::editorCmd()
{
    return get("Editor");
}
