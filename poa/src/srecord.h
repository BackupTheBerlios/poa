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
 * $Id: srecord.h,v 1.3 2004/02/01 17:18:48 squig Exp $
 *
 *****************************************************************************/

#ifndef POA_SRECORD_H
#define POA_SRECORD_H

#include <qobject.h>

class SRecord : public QObject
{
    Q_OBJECT

public:

    /**
     * So far, only S1 record types are supported.
     */
    enum Record_Type { S1 };

    /**
     * Construct an srecord object parsing line.
     *
     * More infos about the srec format can be fount at
     * http://www.amelek.gda.pl/avr/uisp/srecord.htm
     */
    SRecord(const char *line, const unsigned int length);

    unsigned int address() const;

    const char *data() const;

    unsigned int dataSize() const;

    static unsigned int minLength();

    Record_Type type() const;

    unsigned int checksum() const;

private:

    Record_Type type_;
    unsigned int address_;
    QString data_;
    unsigned int checksum_;
    unsigned int count_;
};

#endif // POA_SRECORD_H
