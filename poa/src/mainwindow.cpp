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
 * $Id: mainwindow.cpp,v 1.2 2003/08/19 20:00:49 garbeam Exp $
 *
 *****************************************************************************/

#include "mainwindow.h"

#include <qvariant.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qaction.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qtoolbar.h>
#include <qimage.h>
#include <qpixmap.h>

static const char* const image0_data[] = { 
"22 22 7 1",
". c None",
"# c #000000",
"b c #2e2e2e",
"c c #5c5c5c",
"d c #878787",
"e c #c2c2c2",
"a c #ffffff",
"......................",
"....##########........",
"....#aaaaaaa#b#.......",
"....#aaaaaaa#cb#......",
"....#aaaaaaa#dcb#.....",
"....#aaaaaaa#edcb#....",
"....#aaaaaaa#aedcb#...",
"....#aaaaaaa#######...",
"....#aaaaaaaaaaaaa#...",
"....#aaaaaaaaaaaaa#...",
"....#aaaaaaaaaaaaa#...",
"....#aaaaaaaaaaaaa#...",
"....#aaaaaaaaaaaaa#...",
"....#aaaaaaaaaaaaa#...",
"....#aaaaaaaaaaaaa#...",
"....#aaaaaaaaaaaaa#...",
"....#aaaaaaaaaaaaa#...",
"....#aaaaaaaaaaaaa#...",
"....#aaaaaaaaaaaaa#...",
"....###############...",
"......................",
"......................"};

static const char* const image1_data[] = { 
"22 22 5 1",
". c None",
"# c #000000",
"c c #848200",
"a c #ffff00",
"b c #ffffff",
"......................",
"......................",
"......................",
"............####....#.",
"...........#....##.##.",
"..................###.",
".................####.",
".####...........#####.",
"#abab##########.......",
"#babababababab#.......",
"#ababababababa#.......",
"#babababababab#.......",
"#ababab###############",
"#babab##cccccccccccc##",
"#abab##cccccccccccc##.",
"#bab##cccccccccccc##..",
"#ab##cccccccccccc##...",
"#b##cccccccccccc##....",
"###cccccccccccc##.....",
"##cccccccccccc##......",
"###############.......",
"......................"};

static const char* const image2_data[] = { 
"22 22 5 1",
". c None",
"# c #000000",
"a c #848200",
"b c #c1c1c1",
"c c #cab5d1",
"......................",
".####################.",
".#aa#bbbbbbbbbbbb#bb#.",
".#aa#bbbbbbbbbbbb#bb#.",
".#aa#bbbbbbbbbcbb####.",
".#aa#bbbccbbbbbbb#aa#.",
".#aa#bbbccbbbbbbb#aa#.",
".#aa#bbbbbbbbbbbb#aa#.",
".#aa#bbbbbbbbbbbb#aa#.",
".#aa#bbbbbbbbbbbb#aa#.",
".#aa#bbbbbbbbbbbb#aa#.",
".#aaa############aaa#.",
".#aaaaaaaaaaaaaaaaaa#.",
".#aaaaaaaaaaaaaaaaaa#.",
".#aaa#############aa#.",
".#aaa#########bbb#aa#.",
".#aaa#########bbb#aa#.",
".#aaa#########bbb#aa#.",
".#aaa#########bbb#aa#.",
".#aaa#########bbb#aa#.",
"..##################..",
"......................"};

static const char* const image3_data[] = { 
"22 22 88 2",
"Qt c None",
".2 c #000000",
".S c #08ff08",
"#v c #100810",
".U c #101010",
"#c c #101018",
".M c #181018",
"#e c #181818",
".A c #181821",
".L c #211821",
"#l c #212121",
".z c #212129",
"#m c #292129",
"#u c #292929",
"#n c #292931",
".R c #29ff29",
"#o c #312931",
".T c #313131",
"#p c #313139",
".Z c #31ff31",
"#q c #393139",
"#t c #393939",
".y c #393942",
"#s c #423942",
".o c #424242",
"#h c #4a4a52",
".n c #5a525a",
"#r c #5a5a63",
".I c #5ace5a",
"#b c #6b636b",
".p c #6b6b6b",
".x c #6b6b73",
".Y c #6bff63",
".l c #736b73",
".t c #7b737b",
".s c #7b7384",
".0 c #7bff7b",
".r c #847b84",
".u c #847b8c",
"#g c #84848c",
".v c #8c7b94",
"#i c #8c848c",
".w c #8c8494",
"#j c #8c8c8c",
".8 c #8c8c94",
".m c #948c94",
"#k c #948c9c",
"#f c #949494",
".q c #94949c",
".J c #94c694",
"#d c #9c949c",
"#a c #9c94a5",
".k c #9c9c9c",
".N c #9c9ca5",
".H c #9ccea5",
".K c #a59ca5",
"#. c #a59cad",
".i c #a5a5a5",
".3 c #a5a5ad",
"## c #ad9cad",
".V c #ada5ad",
".d c #adadad",
".j c #adadb5",
".9 c #b5adb5",
".# c #b5b5b5",
".a c #bdbdbd",
".7 c #bdd6bd",
".c c #c6c6c6",
".5 c #cec6ce",
".b c #cecece",
".4 c #ceced6",
".F c #d6ced6",
".G c #d6cede",
".h c #d6d6d6",
".E c #d6d6de",
".Q c #d6ffd6",
".B c #ded6de",
".1 c #ded6e7",
".g c #dedede",
".D c #dedee7",
".6 c #e7dee7",
".f c #e7e7e7",
".C c #e7e7ef",
".X c #e7ffe7",
".O c #efe7ef",
".e c #efefef",
".W c #f7f7f7",
".P c #ffffff",
"QtQtQtQtQtQt.#.a.b.b.b.b.c.c.a.a.d.aQtQtQtQt",
"QtQtQtQtQtQt.a.e.f.f.f.f.f.e.e.e.g.aQtQtQtQt",
"QtQtQtQtQtQt.a.c.c.c.b.b.c.c.c.c.a.cQtQtQtQt",
"QtQtQtQtQtQt.#.a.a.a.a.#.a.a.#.#.d.aQtQtQtQt",
"QtQtQtQtQt.c.d.c.a.c.c.c.a.a.a.c.#QtQtQtQtQt",
"QtQtQtQtQt.a.a.#.a.a.a.a.a.a.c.c.#QtQtQtQtQt",
"QtQtQtQtQt.a.#.c.a.a.a.a.a.c.a.c.dQtQtQtQtQt",
"QtQtQtQtQt.c.a.a.a.a.a.a.a.a.a.a.#QtQtQtQtQt",
"QtQtQtQtQt.d.b.f.g.g.g.g.g.g.h.g.i.i.jQtQtQt",
"QtQtQt.a.k.l.#.h.b.h.b.h.b.h.g.g.m.n.o.p.#Qt",
"QtQt.a.q.r.s.t.t.t.t.t.t.t.u.v.w.x.y.z.A.o.i",
"Qt.a.k.B.C.D.B.E.E.E.E.F.G.H.I.J.K.o.L.L.M.y",
".a.N.O.P.P.P.P.P.P.P.P.P.Q.R.S.R.b.v.T.A.U.L",
".V.W.P.P.P.P.P.P.P.P.P.P.X.Y.Z.0.P.1.t.A.2.L",
".3.E.4.5.4.h.E.E.g.6.D.B.D.E.7.F.4.5.8.M.2.A",
".m.9.j.V.3#..3.K#.#..i#..K#.###a.q.8#b#c.2.L",
".m.j.j#..3.K.K.K.N.K.N.N.N.N#a#d#d.w#b#c.2#e",
"#f#.#..K.N.K.N.N.N#a.k#a#d#d#d#a.m#g#b.M.2#h",
".m.3.K.K#a.k#a#d#a.k#a#d#a#d.q.m.8#i.x#c#e.d",
"#f#g#i.w#j.w#i.8.w#i.8.8.m.8.m#k.8.w#b#e#fQt",
".#.l.z.A#l.z#m#m#m#n#o#o#p#p#q#q#p#o#p#fQtQt",
"QtQt.d#r#s#s#t#p.T.T.T#u#u.z#e#e#v.o.kQtQtQt"};

static const char* const image4_data[] = { 
"22 22 3 1",
". c None",
"# c #000084",
"a c #848284",
"......................",
"......................",
"......................",
"......................",
"......................",
"...........######a....",
"..#......##########...",
"..##...####......##a..",
"..###.###.........##..",
"..######..........##..",
"..#####...........##..",
"..######..........##..",
"..#######.........##..",
"..########.......##a..",
"...............a###...",
"...............###....",
"......................",
"......................",
"......................",
"......................",
"......................",
"......................"};

static const char* const image5_data[] = { 
"22 22 3 1",
". c None",
"a c #000084",
"# c #848284",
"......................",
"......................",
"......................",
"......................",
"......................",
"....#aaaaaa...........",
"...aaaaaaaaaa......a..",
"..#aa......aaaa...aa..",
"..aa.........aaa.aaa..",
"..aa..........aaaaaa..",
"..aa...........aaaaa..",
"..aa..........aaaaaa..",
"..aa.........aaaaaaa..",
"..#aa.......aaaaaaaa..",
"...aaa#...............",
"....aaa...............",
"......................",
"......................",
"......................",
"......................",
"......................",
"......................"};

static const char* const image6_data[] = { 
"22 22 3 1",
". c None",
"# c #000000",
"a c #000082",
"......................",
".......#.....#........",
".......#.....#........",
".......#.....#........",
".......#....##........",
".......##...#.........",
"........#...#.........",
"........##.##.........",
".........###..........",
".........###..........",
"..........#...........",
".........a#a..........",
"........aa.aaa........",
".......a.a.a..a.......",
"......a..a.a...a......",
".....a...a.a....a.....",
"....a....a.a....a.....",
"....a....a..a...a.....",
"....a....a..a..a......",
"....a...a....aa.......",
".....aaa..............",
"......................"};

static const char* const image7_data[] = { 
"22 22 6 1",
". c None",
"# c #000000",
"b c #000082",
"c c #3c3cfd",
"d c #8b8bfd",
"a c #ffffff",
"......................",
"......................",
"########..............",
"#aaaaaa##.............",
"#a####a#a#............",
"#aaaaaa#aa#...........",
"#a####a#bbbbbbbb......",
"#aaaaaa#baaaaaabb.....",
"#a#####aba####abcb....",
"#aaaaaaabaaaaaabdcb...",
"#a#####aba####abadcb..",
"#aaaaaaabaaaaaabbbbbb.",
"#a#####aba####aaaaaab.",
"#aaaaaaabaaaaaaaaaaab.",
"#a#####aba#########ab.",
"#aaaaaaabaaaaaaaaaaab.",
"########ba#########ab.",
"........baaaaaaaaaaab.",
"........ba#########ab.",
"........baaaaaaaaaaab.",
"........bbbbbbbbbbbbb.",
"......................"};

static const char* const image8_data[] = { 
"22 22 8 1",
". c None",
"# c #000000",
"e c #000084",
"c c #848200",
"b c #848284",
"d c #c6c3c6",
"a c #ffff00",
"f c #ffffff",
"......................",
".......#####..........",
"..######aaa######.....",
".######aaaaa######....",
"##bcb##a###a##bcb##...",
"#bcb#ddddddddd#bcb#...",
"#cbc#ddddddddd#cbc#...",
"#bcb###########bcb#...",
"#cbcbcbcbcbcbcbcbc#...",
"#bcbcbcbcbcbcbcbcb#...",
"#cbcbcbceeeeeeeeee#...",
"#bcbcbcbefffffffefe...",
"#cbcbcbcefeeeeefeffe..",
"#bcbcbcbefffffffefffe.",
"#cbcbcbcefeeeeefeffffe",
"#bcbcbcbefffffffeeeeee",
"#cbcbcbcefeeeeeffffffe",
"#bcbcbcbeffffffffffffe",
"#cbcbcbcefeeeeeeeeeefe",
".#######effffffffffffe",
"........eeeeeeeeeeeeee",
"......................"};

static const char* const image9_data[] = { 
"22 22 106 2",
"Qt c None",
"#N c #212121",
"#J c #292929",
"#K c #393939",
"#M c #424242",
".m c #5a5a5a",
"#r c #5a736b",
"#A c #5a9c84",
"#B c #636363",
".N c #636b6b",
"#G c #63736b",
"#v c #63ad8c",
"#H c #6b6b6b",
".c c #6b7373",
"#D c #6b7b73",
".l c #6b7b7b",
".W c #6b847b",
".t c #6b8c7b",
"#x c #6b9484",
"#w c #6bad94",
".u c #737373",
".v c #737b73",
".g c #737b7b",
".D c #738c84",
".7 c #739484",
"#d c #73948c",
"#z c #73a59c",
"#p c #73b594",
"#o c #73bd9c",
".b c #7b7b7b",
".O c #7b8484",
"#a c #7bad94",
"#q c #7bad9c",
"#i c #7bb59c",
".5 c #7bbd9c",
"#b c #7bbda5",
"#h c #7bc69c",
"#y c #7bc6a5",
".8 c #848484",
".4 c #8484b5",
".h c #849c94",
"#n c #84a5ad",
".V c #84ada5",
"#E c #84b59c",
"#F c #84b5a5",
"#j c #84bda5",
"#t c #84c6a5",
"#l c #84cead",
".E c #8c8c8c",
"#g c #8c8cbd",
"## c #8c94b5",
"#m c #8cadbd",
"#c c #8cb5a5",
".L c #8cc6ad",
".T c #8ccead",
"#f c #8cceb5",
".a c #949494",
".n c #94a59c",
"#u c #94a5ce",
".k c #94bdad",
".M c #94c6ad",
".6 c #94c6b5",
".B c #94ceb5",
".1 c #94d6b5",
".C c #94d6bd",
".9 c #94debd",
".K c #9c94d6",
".d c #9c9c9c",
".w c #9cbdb5",
"#. c #9cbdd6",
"#s c #9ccebd",
".2 c #9ccece",
".0 c #9cdebd",
".S c #9ce7bd",
".Z c #9ce7c6",
".U c #a594de",
"#L c #a5a5a5",
".A c #a5b5de",
".3 c #a5b5ef",
".I c #a5cece",
"#k c #a5dec6",
".s c #a5e7c6",
".f c #adadad",
".J c #adadef",
".R c #adb5de",
".i c #adcec6",
".P c #add6c6",
".j c #addece",
"#e c #ade7ce",
".# c #b5b5b5",
".H c #b5bdde",
".Q c #b5dede",
".r c #b5efd6",
".e c #bdbdbd",
".z c #bdc6f7",
".y c #bdd6ff",
".X c #bde7d6",
".q c #bdefde",
".Y c #bdf7de",
"#I c #c6c6c6",
".x c #c6def7",
".p c #c6f7e7",
"#C c #ced6ce",
".G c #cedeff",
".o c #ceffef",
".F c #d6fff7",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt",
"QtQtQt.#.a.b.c.c.b.d.eQtQtQtQtQtQtQtQtQtQtQt",
"QtQt.f.g.h.i.j.j.k.l.b.#QtQtQtQtQtQtQtQtQtQt",
"Qt.f.m.n.o.o.p.q.r.s.t.u.eQtQtQtQtQtQtQtQtQt",
".e.v.w.q.p.x.y.z.A.B.C.D.EQtQtQtQtQtQtQtQtQt",
".d.h.F.q.G.H.I.I.J.K.L.M.N.#QtQtQtQtQtQtQtQt",
".O.P.o.Q.R.B.S.T.A.U.V.B.W.dQtQtQtQtQtQtQtQt",
".g.X.Y.Z.Z.0.1.2.3.4.5.6.7.8QtQtQtQtQtQtQtQt",
".g.j.r.0.9.1#..3###a#b#c#d.8QtQtQtQtQtQtQtQt",
".v.P#e.1.1#f.A#g.5#h#i#j.W.EQtQtQtQtQtQtQtQt",
".E#c#k#f.L#l#m#n#o#p#p#q#r.fQtQtQtQtQtQtQtQt",
".f.g#s#t#t#b#u.4#v#w#w#x.u.eQtQtQtQtQtQtQtQt",
"Qt.E.t#y#b.5#z#d#p#A#A#B.d#C.f.eQtQtQtQtQtQt",
"Qt.e.b#D#E#F#j#i#a.7.m.dQt.f#B.m.fQtQtQtQtQt",
"QtQt.e.E.N#D.W.W#G#H.fQtQt#I.b#J#K.E.eQtQtQt",
"QtQtQtQt.#.d.E.a#L#IQtQtQtQtQt#L#M#J#H.fQtQt",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt.e.u#N#K.aQt",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt.d#K#N#H",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt.##B.m",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt"};


/* 
 *  Constructs a MainWindow which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 */
MainWindow::MainWindow( QWidget* parent,  const char* name, WFlags fl )
    : QMainWindow( parent, name, fl )
{
    (void)statusBar();
    QPixmap image0( ( const char** ) image0_data );
    QPixmap image1( ( const char** ) image1_data );
    QPixmap image2( ( const char** ) image2_data );
    QPixmap image3( ( const char** ) image3_data );
    QPixmap image4( ( const char** ) image4_data );
    QPixmap image5( ( const char** ) image5_data );
    QPixmap image6( ( const char** ) image6_data );
    QPixmap image7( ( const char** ) image7_data );
    QPixmap image8( ( const char** ) image8_data );
    QPixmap image9( ( const char** ) image9_data );
    if ( !name )
	setName( "MainWindow" );
    resize( 520, 480 ); 
    setCaption( trUtf8( "POA" ) );

    // actions
    fileNewAction = new QAction( this, "fileNewAction" );
    fileNewAction->setIconSet( QIconSet( image0 ) );
    fileNewAction->setText( trUtf8( "New" ) );
    fileNewAction->setMenuText( trUtf8( "&New" ) );
    fileNewAction->setAccel( 4194382 );
    fileOpenAction = new QAction( this, "fileOpenAction" );
    fileOpenAction->setIconSet( QIconSet( image1 ) );
    fileOpenAction->setText( trUtf8( "Open" ) );
    fileOpenAction->setMenuText( trUtf8( "&Open..." ) );
    fileOpenAction->setAccel( 4194383 );
    fileSaveAction = new QAction( this, "fileSaveAction" );
    fileSaveAction->setIconSet( QIconSet( image2 ) );
    fileSaveAction->setText( trUtf8( "Save" ) );
    fileSaveAction->setMenuText( trUtf8( "&Save" ) );
    fileSaveAction->setAccel( 4194387 );
    fileSaveAsAction = new QAction( this, "fileSaveAsAction" );
    fileSaveAsAction->setText( trUtf8( "Save As" ) );
    fileSaveAsAction->setMenuText( trUtf8( "Save &As..." ) );
    fileSaveAsAction->setAccel( 0 );
    filePrintAction = new QAction( this, "filePrintAction" );
    filePrintAction->setIconSet( QIconSet( image3 ) );
    filePrintAction->setText( trUtf8( "Print" ) );
    filePrintAction->setMenuText( trUtf8( "&Print..." ) );
    filePrintAction->setAccel( 4194384 );
    fileExitAction = new QAction( this, "fileExitAction" );
    fileExitAction->setText( trUtf8( "Exit" ) );
    fileExitAction->setMenuText( trUtf8( "E&xit" ) );
    fileExitAction->setAccel( 0 );
    editUndoAction = new QAction( this, "editUndoAction" );
    editUndoAction->setIconSet( QIconSet( image4 ) );
    editUndoAction->setText( trUtf8( "Undo" ) );
    editUndoAction->setMenuText( trUtf8( "&Undo" ) );
    editUndoAction->setAccel( 4194394 );
    editRedoAction = new QAction( this, "editRedoAction" );
    editRedoAction->setIconSet( QIconSet( image5 ) );
    editRedoAction->setText( trUtf8( "Redo" ) );
    editRedoAction->setMenuText( trUtf8( "&Redo" ) );
    editRedoAction->setAccel( 4194393 );
    editCutAction = new QAction( this, "editCutAction" );
    editCutAction->setIconSet( QIconSet( image6 ) );
    editCutAction->setText( trUtf8( "Cut" ) );
    editCutAction->setMenuText( trUtf8( "&Cut" ) );
    editCutAction->setAccel( 4194392 );
    editCopyAction = new QAction( this, "editCopyAction" );
    editCopyAction->setIconSet( QIconSet( image7 ) );
    editCopyAction->setText( trUtf8( "Copy" ) );
    editCopyAction->setMenuText( trUtf8( "C&opy" ) );
    editCopyAction->setAccel( 4194371 );
    editPasteAction = new QAction( this, "editPasteAction" );
    editPasteAction->setIconSet( QIconSet( image8 ) );
    editPasteAction->setText( trUtf8( "Paste" ) );
    editPasteAction->setMenuText( trUtf8( "&Paste" ) );
    editPasteAction->setAccel( 4194390 );
    editFindAction = new QAction( this, "editFindAction" );
    editFindAction->setIconSet( QIconSet( image9 ) );
    editFindAction->setText( trUtf8( "Find" ) );
    editFindAction->setMenuText( trUtf8( "&Find..." ) );
    editFindAction->setAccel( 4194374 );
    helpContentsAction = new QAction( this, "helpContentsAction" );
    helpContentsAction->setText( trUtf8( "Contents" ) );
    helpContentsAction->setMenuText( trUtf8( "&Contents..." ) );
    helpContentsAction->setAccel( 0 );
    helpIndexAction = new QAction( this, "helpIndexAction" );
    helpIndexAction->setText( trUtf8( "Index" ) );
    helpIndexAction->setMenuText( trUtf8( "&Index..." ) );
    helpIndexAction->setAccel( 0 );
    helpAboutAction = new QAction( this, "helpAboutAction" );
    helpAboutAction->setText( trUtf8( "About" ) );
    helpAboutAction->setMenuText( trUtf8( "&About..." ) );
    helpAboutAction->setAccel( 0 );


    // toolbars
    commonToolBar = new QToolBar( "", this, DockTop ); 

    commonToolBar->setLabel( trUtf8( "common toolbar" ) );
    fileNewAction->addTo( commonToolBar );
    fileOpenAction->addTo( commonToolBar );
    fileSaveAction->addTo( commonToolBar );
    commonToolBar->addSeparator();
    editCutAction->addTo( commonToolBar );
    editCopyAction->addTo( commonToolBar );
    editPasteAction->addTo( commonToolBar );
    commonToolBar->addSeparator();
    utilToolBar = new QToolBar( "", this, DockTop ); 

    utilToolBar->setLabel( trUtf8( "utility toolbar" ) );
    drawToolBar = new QToolBar( "", this, DockTop ); 

    drawToolBar->setLabel( trUtf8( "draw toolbar" ) );


    // menubar
    menubar = new QMenuBar( this, "menubar" );

    fileMenu = new QPopupMenu( this ); 
    fileNewAction->addTo( fileMenu );
    fileOpenAction->addTo( fileMenu );
    fileSaveAction->addTo( fileMenu );
    fileSaveAsAction->addTo( fileMenu );
    fileMenu->insertSeparator();
    fileExitAction->addTo( fileMenu );
    menubar->insertItem( trUtf8( "&File" ), fileMenu );

    editMenu = new QPopupMenu( this ); 
    editCutAction->addTo( editMenu );
    editCopyAction->addTo( editMenu );
    editPasteAction->addTo( editMenu );
    editMenu->insertSeparator();
    menubar->insertItem( trUtf8( "&Edit" ), editMenu );

    PopupMenu = new QPopupMenu( this ); 
    menubar->insertItem( trUtf8( "&Tools" ), PopupMenu );

    PopupMenu_3 = new QPopupMenu( this ); 
    menubar->insertItem( trUtf8( "&Draw" ), PopupMenu_3 );

    helpMenu = new QPopupMenu( this ); 
    helpContentsAction->addTo( helpMenu );
    helpMenu->insertSeparator();
    helpAboutAction->addTo( helpMenu );
    menubar->insertItem( trUtf8( "&Help" ), helpMenu );



    // signals and slots connections
    connect( fileNewAction, SIGNAL( activated() ), this, SLOT( fileNew() ) );
    connect( fileOpenAction, SIGNAL( activated() ), this, SLOT( fileOpen() ) );
    connect( fileSaveAction, SIGNAL( activated() ), this, SLOT( fileSave() ) );
    connect( fileSaveAsAction, SIGNAL( activated() ), this, SLOT( fileSaveAs() ) );
    connect( filePrintAction, SIGNAL( activated() ), this, SLOT( filePrint() ) );
    connect( fileExitAction, SIGNAL( activated() ), this, SLOT( fileExit() ) );
    connect( editUndoAction, SIGNAL( activated() ), this, SLOT( editUndo() ) );
    connect( editRedoAction, SIGNAL( activated() ), this, SLOT( editRedo() ) );
    connect( editCutAction, SIGNAL( activated() ), this, SLOT( editCut() ) );
    connect( editCopyAction, SIGNAL( activated() ), this, SLOT( editCopy() ) );
    connect( editPasteAction, SIGNAL( activated() ), this, SLOT( editPaste() ) );
    connect( editFindAction, SIGNAL( activated() ), this, SLOT( editFind() ) );
    connect( helpIndexAction, SIGNAL( activated() ), this, SLOT( helpIndex() ) );
    connect( helpContentsAction, SIGNAL( activated() ), this, SLOT( helpContents() ) );
    connect( helpAboutAction, SIGNAL( activated() ), this, SLOT( helpAbout() ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
MainWindow::~MainWindow()
{
    // no need to delete child widgets, Qt does it all for us
}

void MainWindow::fileNew()
{
    qWarning( "MainWindow::fileNew(): Not implemented yet!" );
}

void MainWindow::fileOpen()
{
    qWarning( "MainWindow::fileOpen(): Not implemented yet!" );
}

void MainWindow::fileSave()
{
    qWarning( "MainWindow::fileSave(): Not implemented yet!" );
}

void MainWindow::fileSaveAs()
{
    qWarning( "MainWindow::fileSaveAs(): Not implemented yet!" );
}

void MainWindow::filePrint()
{
    qWarning( "MainWindow::filePrint(): Not implemented yet!" );
}

void MainWindow::fileExit()
{
    qWarning( "MainWindow::fileExit(): Not implemented yet!" );
}

void MainWindow::editUndo()
{
    qWarning( "MainWindow::editUndo(): Not implemented yet!" );
}

void MainWindow::editRedo()
{
    qWarning( "MainWindow::editRedo(): Not implemented yet!" );
}

void MainWindow::editCut()
{
    qWarning( "MainWindow::editCut(): Not implemented yet!" );
}

void MainWindow::editCopy()
{
    qWarning( "MainWindow::editCopy(): Not implemented yet!" );
}

void MainWindow::editPaste()
{
    qWarning( "MainWindow::editPaste(): Not implemented yet!" );
}

void MainWindow::editFind()
{
    qWarning( "MainWindow::editFind(): Not implemented yet!" );
}

void MainWindow::helpIndex()
{
    qWarning( "MainWindow::helpIndex(): Not implemented yet!" );
}

void MainWindow::helpContents()
{
    qWarning( "MainWindow::helpContents(): Not implemented yet!" );
}

void MainWindow::helpAbout()
{
    qWarning( "MainWindow::helpAbout(): Not implemented yet!" );
}

