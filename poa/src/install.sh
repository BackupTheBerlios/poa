#!/bin/sh
POADIR=poa
PREFIX=$1
BINARY=poa
ICONS=icons
TEMPLATES=templates

usage()
{
    echo "usage: install.sh [-p <install-path prefix, default is /usr>]"
    echo
    exit 0
}

while [ -n "$1" ]; do
case $1 in
    -p) PREFIX=$2;break;;
    -*) echo "error: no such option $1.";usage;exit 1;break;;
     *)  break;;
esac
done

if [ "$PREFIX" = "" ]; then
    PREFIX="/usr"
fi
if [ ! -x "$BINARY" ]; then
    echo "error: you've to build POA first before installing it."
    echo "       Invoke make within this directory first."
    exit 1
fi
if [ ! -d "$ICONS" ]; then
    echo "error: the icons resources of POA does not exist."
    echo "       Make sure that '$ICONS' exists."
    exit 1
fi
if [ ! -d "$TEMPLATES" ]; then
    echo "error: the templates directory does not exist."
    echo "       Make sure that '$TEMPLATES' exists."
    exit 1;
fi
echo "POA installation (Linux)"
echo "======================================================"
echo "Note: On many systems you have to be superuser running"
echo "      this script."
echo
echo "Installation path: $PREFIX/$POADIR"
mkdir $PREFIX/$POADIR || exit 1;
echo "Installing '$BINARY' into '$PREFIX/$POADIR'"
cp $BINARY $PREFIX/$POADIR || exit 1;
echo "Installing '$ICONS' into '$PREFIX/$POADIR'"
cp -r $ICONS $PREFIX/$POADIR || exit 1;
echo "Installing '$TEMPLATES' into '$PREFIX/$POADIR'"
cp -r $TEMPLATES $PREFIX/$POADIR || exit 1;
echo "Finished."
echo "To run POA, just execute it from '$PREFIX/$POADIR'"
echo "Have fun. Your POA Team."
