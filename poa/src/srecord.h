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
 * $Id: srecord.h,v 1.4 2004/02/01 20:28:57 squig Exp $
 *
 *****************************************************************************/

#ifndef POA_SRECORD_H
#define POA_SRECORD_H

#include <qobject.h>

/**
 * Represents a single line in an srec file. See
 * http://www.amelek.gda.pl/avr/uisp/srecord.htm for information on
 * the srec format.
 */
class SRecord : public QObject
{
    Q_OBJECT

public:

    /**
     * So far, only S1 record types are supported.
     */
    enum Record_Type { S1 };

    /**
     * Construct an srecord object parsing line reading up to length
     * characters. The checksum is not validated!

     * @throw PoaException if the format of line is not correct
     */
    SRecord(const char *line, const unsigned int length);

    ~SRecord();

    /**
     * Returns the address.
     */
    unsigned int address() const;

    /**
     * Returns an array of characters.
     *
     * @see #dataSize()
     */
    const unsigned char *data() const;

    /**
     * Returns the number of data characters.
     */
    unsigned int dataSize() const;

    /**
     * Returns the minimum number of bytes for a valid record.
     */
    static unsigned int minLength();

    /**
     * Returns the record type.
     */
    Record_Type type() const;

    /**
     * Returns the checksum.
     */
    unsigned int checksum() const;

private:

    Record_Type type_;
    unsigned int address_;
    unsigned char *data_;
    unsigned int checksum_;
    unsigned int count_;
};

#endif // POA_SRECORD_H
