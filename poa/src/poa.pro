###############################################################################
# qmake definition
#
TEMPLATE = app
INCLUDEPATH += .

# Input
# note: mdiwindow.h currently removed
HEADERS +=  blockmodel.h blockview.h codemanager.h connectormodel.h connectorview.h coremodel.h coreview.h cpumodel.h cpuview.h inputblockmodel.h inputblockview.h layoutcanvas.h layoutmanager.h mainwindow.h moduleconfdialog.h modulelibraryitem.h muxmodel.h muxview.h optiondialog.h outputblockmodel.h outputblockview.h pinmodel.h pinvector.h pinview.h viewmap.h
SOURCES += mainwindow.cpp moduleconfdialog.cpp optiondialog.cpp main.cpp

TARGET = poa
