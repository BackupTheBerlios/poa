/*
    Copyright (C) 1992-2003 Trolltech AS. All Rights Reserved.

    This file ("Example Code"), is part of an example program for Qt,
    and is licensed under the Qt Commercial License Agreement,
    Agreement version 2.4 and higher as well as under the GPL.

    The Example Code is licensed to you "as is" without warranty of
    any kind. To the maximum extent permitted by applicable law,
    Trolltech on behalf of itself and its suppliers, disclaims all
    warranties express or implied, including, but not limited to,
    implied warranties of merchantability and fitness for a particular
    purpose with regard to the Example Code. Trolltech does not
    warrant that the Example Code will satisfy your requirements or
    that it is without defect or error or that the operation thereof
    will be uninterrupted. All use of and reliance on the Example Code
    is at the sole risk of and responsibility of you and your
    customers.

    If you are a holder of a Qt Commercial License Agreement, you can
    use the code under that agreement or you can use it under GPL. If
    you are not a holder of a Qt Commercial License Agreement, you can
    use the code under the GPL. Regardless of the applicable license
    agreement, the Example Code may be used, distributed and modified
    without limitation.
*/


#include <qfile.h>

#include "xmlwriter.h"

void writeProperty( XmlWriter& xw, const QString& name, const QString& type,
		    const QString& value )
{
    xw.writeOpenTag( "property", AttrMap("name", name) );
    xw.writeTaggedString( type, value );
    xw.writeCloseTag( "property" );
}

int main()
{
    QFile file;
    file.open( IO_WriteOnly, stdout );
    XmlWriter xw( &file );
    xw.setAutoNewLine( true );
    xw.writeRaw( "<!DOCTYPE UI><UI version=\"3.1\">" );
    xw.newLine();
    xw.writeTaggedString( "class", "Form1" );
    xw.writeOpenTag( "widget", AttrMap("class", "QDialog") );
    writeProperty( xw, "name", "cstring", "Form1" );
    writeProperty( xw, "caption", "string", "Form1" );
    xw.writeOpenTag( "vbox" );
    xw.writeOpenTag( "widget", AttrMap("class", "QLabel") );
    writeProperty( xw, "name", "cstring", "label" );
    writeProperty( xw, "text", "string", "Rock && Roll" );
    xw.writeCloseTag( "widget" );
    xw.writeCloseTag( "vbox" );
    xw.writeCloseTag( "widget" );
    AttrMap attrs;
    attrs.insert( "spacing", "6" );
    attrs.insert( "margin", "11" );
    xw.writeAtomTag( "layoutdefaults", attrs );
    xw.writeRaw( "</UI>" );
    return 0;
}
