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
 * $Id: settings.cpp,v 1.26 2004/03/19 15:41:51 squig Exp $
 *
 *****************************************************************************/

#include <qdir.h>

#include "settings.h"

#include "poa.h"

/**
 * The singleton instance.
 */
Settings* Settings::instance_ = 0;

Settings::Settings(QString prefix)
{
    this->prefix = prefix;
    this->confPath_ = QDir::homeDirPath() + "/.poa";

    // create confpah if it does not exist
    QDir confDir(confPath());
    if (!confDir.exists()) {
        confDir.mkdir(confPath(), TRUE);
    }

    settingsInstance_ = new QSettings();
    initialize(settingsInstance_);
}

Settings::~Settings()
{
    delete settingsInstance_;
}

Settings *Settings::instance()
{
    if (instance_ == 0) {
        instance_ = new Settings("/POA/POA/");
    }

    return instance_;
}

void Settings::save()
{
    delete settingsInstance_;

    settingsInstance_ = new QSettings();
    initialize(settingsInstance_);
}

QSettings *Settings::initialize(QSettings *settings) const
{
    // does not properly work with qt3 under Linux
    // the properties are read from a different directory than written to
    //settings->insertSearchPath(QSettings::Unix, confPath());

    return settings;
}

QString Settings::confPath() const
{
    return confPath_;
}

QString Settings::get(const QString &key, QString defaultValue)
{
    QSettings settings;
    initialize(&settings);
    return settingsInstance_->readEntry(prefix + key, defaultValue);
}

bool Settings::getBool(const QString &key, bool defaultValue)
{
    return settingsInstance_->readBoolEntry(prefix + key, defaultValue);
}


QFont Settings::getFont(const QString &key, QFont defaultValue)
{
    QFont font;
    font.fromString(get(prefix + key, defaultValue.toString()));
    return font;
}

int Settings::getNum(const QString &key, int defaultValue)
{
    return settingsInstance_->readNumEntry(prefix + key, defaultValue);
}

QStringList Settings::getStrings(const QString &key, bool *ok)
{
    return settingsInstance_->readListEntry(prefix + key, ok);
}

bool Settings::set(const QString &key, const char *value)
{
    return set(key, QString(value));
}

bool Settings::set(const QString &key, const QString &value)
{
    QString oldValue = get(key);
    if (oldValue != value) {
        settingsInstance_->writeEntry(prefix + key, value);
        save();
        emit settingChanged(key);
        return TRUE;
    }
    return FALSE;
}

bool Settings::set(const QString &key, bool value)
{
    bool oldValue = getBool(key);
    if (oldValue != value) {
        settingsInstance_->writeEntry(prefix + key, value);
        save();
        emit settingChanged(key);
        return TRUE;
    }
    return FALSE;
}

bool Settings::set(const QString &key, int value)
{
    int oldValue = getNum(key);
    if (oldValue != value) {
        settingsInstance_->writeEntry(prefix + key, value);
        save();
        emit settingChanged(key);
        return TRUE;
    }
    return FALSE;
}

bool Settings::set(const QString &key, const QStringList &value)
{
    QStringList oldValue = getStrings(key);
    if (oldValue != value) {
        settingsInstance_->writeEntry(prefix + key, value);
        save();
        emit settingChanged(key);
        return TRUE;
    }
    return FALSE;
}

//  void Settings::setDefault(QSettings* settings, const QString &key,
//                            const QString &value)
//  {
//      bool success;
//      settings->readEntry(prefix + key, QString::null, &success);
//      if (!success) {
//          // key does not exists
//          settings->writeEntry(prefix + key, value);
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
    return Qt::darkGray;
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
    return get("Template Path", "templates/cpu_template.c");
}

QColor Settings::warningColor()
{
    return Qt::red;
}

QString Settings::compilerCmd()
{
    return get("Compiler", "nb");
}

QString Settings::editorCmd()
{
    return get("Editor");
}

QString Settings::serialPort()
{
  return get("Serial Port");
}

QString Settings::language()
{
  return get("Language", "english");
}
