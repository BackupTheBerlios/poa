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
 * $Id: tag.h,v 1.1 2003/07/14 19:52:42 garbeam Exp $
 *
 *****************************************************************************/

#ifndef POA_TAG_H
#define POA_TAG_H

#include "xmlwriter.h"

/*****************************************************************************
 * Represents a xml tag.
 * @author garbeam
 */
class Tag {

private:
    QString name_;
    QString value_;
    QMap attributes_;
    XmlWriter writer_;

public:
    Tag(XmlWriter *writer);
    void setName(QString &name);
    void addAttribute(QString &key, QString &value);
    QString *getName();
    QString *getValue(QString &key);
    void write(); // inherits QTextStream?

};

#endif // POA_TAG_H
