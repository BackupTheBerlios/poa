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
 * $Id: srecord.h,v 1.2 2004/01/30 11:46:58 papier Exp $
 *
 *****************************************************************************/

#ifndef POA_SRECORD_H
#define POA_SRECORD_H

#include <qobject.h>

class SRecord : public QObject
{
  Q_OBJECT
  
 public:

  // more infos about the srec format can be fount at 
  // http://www.amelek.gda.pl/avr/uisp/srecord.htm

  SRecord( QString type, 
	   QString count,
	   QString address,
	   QString data,
	   QString checksum) ;

  QString type() const;
  
  QString count() const;

  QString address() const;

  QString data() const;

  QString checksum() const;

 private:
  QString type_;
  QString count_;
  QString address_;
  QString data_;
  QString checksum_;
};

#endif // POA_SRECORD_H
