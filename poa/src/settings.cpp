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
 * $Id: settings.cpp,v 1.3 2003/08/21 15:59:57 squig Exp $
 *
 *****************************************************************************/
#include "settings.h"

Settings *Settings::instance_ = 0;
const char *Settings::prefix = "/POA/POA/";

Settings::Settings()
{
	QSettings settings;
	settings.insertSearchPath(QSettings::Windows, "/POA");
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

void Settings::set(const QString &key, const QString &value)
{
	QString oldValue = get(key);
	if (oldValue != value) {
		QSettings settings;
		settings.writeEntry(prefix + key, value);
		emit settingChanged(key);
	}
}
