/***************************************************************************
 *   MP3 Diags - diagnosis, repairs and tag editing for MP3 files          *
 *                                                                         *
 *   Copyright (C) 2009 by Marian Ciobanu                                  *
 *   ciobi@inbox.com                                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License version 2 as     *
 *   published by the Free Software Foundation.                            *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include  <QFile>

#include  "AboutDlgImpl.h"

#include  "Helpers.h"

extern const char* APP_VER;

AboutDlgImpl::AboutDlgImpl(QWidget* pParent /*= 0*/) : QDialog(pParent, getDialogWndFlags()), Ui::AboutDlg()
{
    setupUi(this);

    QPalette pal (m_pMainTextM->palette());
    pal.setColor(QPalette::Base, pal.color(QPalette::Disabled, QPalette::Window));

    m_pMainTextM->setPalette(pal);
/*
<a href=\"DDDDDDDDDDDDD\">NNNNNNNNNNNNNN</a>
*/
    m_pMainTextM->setHtml(
        "Written by <a href=\"mailto:ciobi@inbox.com?subject=000 MP3 Diags\">Marian Ciobanu (Ciobi)</a>, 2008 - 2009<p/>"
        "Distributed under <a href=\"http://www.gnu.org/licenses/gpl-2.0.html#TOC1\">GPL V2</a><p/>"
        //"Using <a href=\"http://doc.trolltech.com/4/opensourceedition.html\">Qt 4 Open Source Edition</a><p/>"
        "Using <a href=\"http://www.qtsoftware.com/\">Qt Free Edition</a>, released under <a href=\"http://www.gnu.org/licenses/lgpl-2.1.html\">LGPL 2.1</a><p/>"
        "Using <a href=\"http://www.zlib.net/\">zlib</a>, released under the <a href=\"http://www.zlib.net/zlib_license.html\">zlib License</a><p/>"
        "Using <a href=\"http://www.boost.org/doc/libs/1_39_0/libs/serialization/doc/index.html\">Boost Serialization</a>, distributed under the <a href=\"http://www.boost.org/users/license.html\">Boost Software License</a><p/>"
        //"Most icons are either copies or modified versions of icons from the <a href=\"http://www.oxygen-icons.org/\">Oxygen Project</a> for <a href=\"http://www.kde.org/\">KDE 4</a>. They are distributed under <a href=\"http://www.gnu.org/licenses/lgpl.html\">LGPL V3</a><p/>"
        "Using original and modified icons from the <a href=\"http://www.oxygen-icons.org/\">Oxygen Project</a> for <a href=\"http://www.kde.org/\">KDE 4</a>, distributed under <a href=\"http://www.gnu.org/licenses/lgpl.html\">LGPL V3</a><p/>"
        "Using web services provided by <a href=\"http://www.discogs.com/\">Discogs</a> to retrieve album data<p/>"
        "Using web services provided by <a href=\"http://musicbrainz.org/\">MusicBrainz</a> to retrieve album data<p/><p/>"
        "Home page and documentation: <a href=\"http://mp3diags.sourceforge.net/\">http://mp3diags.sourceforge.net/</a><p/>"
        "Feedback and support: <a href=\"http://sourceforge.net/forum/forum.php?forum_id=947206\">Open Discussion Forum</a> or <a href=\"http://sourceforge.net/forum/forum.php?forum_id=947207\">Help Forum</a> at SourceForge<p/>"
        "Change log for the latest version: <a href=\"http://mp3diags.sourceforge.net/015_changelog.html\">http://mp3diags.sourceforge.net/015_changelog.html</a>"
        );

    m_pVersionL->setText(QString("MP3 Diags ") + APP_VER);

    initText(m_pGplV2M, ":/licences/gplv2.txt");
    initText(m_pGplV3M, ":/licences/gplv3.txt");
    initText(m_pLgplV3M, ":/licences/lgplv3.txt");
    initText(m_pLgplV21M, ":/licences/lgpl-2.1.txt");
    initText(m_pBoostM, ":/licences/boost.txt");
    initText(m_pZlibM, ":/licences/zlib.txt");

    m_pSysInfoM->setText(getSystemInfo());

    m_pMainTextM->setFocus();
    //{ QAction* p (new QAction(this)); p->setShortcut(QKeySequence("Ctrl+N")); connect(p, SIGNAL(triggered()), this, SLOT(accept())); addAction(p); }

    { QAction* p (new QAction(this)); p->setShortcut(QKeySequence("F1")); connect(p, SIGNAL(triggered()), this, SLOT(onHelp())); addAction(p); }
}



void AboutDlgImpl::initText(QTextBrowser* p, const char* szFileName)
{
    QFile f (szFileName);
    //QFile::FileError err (f.error());
    //qDebug("file: %d", (int)err);
    //qDebug("size : %d", (int)f.size());
    //QByteArray b (f.readAll());
    f.open(QIODevice::ReadOnly);
    //qDebug("read size : %d", (int)b.size());
    p->setText(QString::fromUtf8(f.readAll()));
}


AboutDlgImpl::~AboutDlgImpl()
{
}



void AboutDlgImpl::onHelp()
{
    openHelp("index.html");
}




// exist: mp3 insight, mp3 doctor, mp3 butcher, mp3 toolbox, mp3 mechanic, mp3 workshop;
//ttt melt ? ice ? ? sorcerer ? exorcist ? healer ? ? MP3 Spy
//"mp3 workshop", "mp3 atelier"
//workshop synonyms:  foundry, laboratory, mill, plant, studio, works
// deep understanding




/*


//bjam --toolset=gcc
//PATH=D:\Qt\2009.02\mingw\bin;%PATH%
bjam serialization toolset=gcc


bjam toolset=gcc serialization threading=multi release

http://stackoverflow.com/questions/760323/why-does-my-qt4-5-app-open-a-console-window-under-windows
http://stackoverflow.com/questions/718447/adding-external-library-into-qt-creator-project
http://stackoverflow.com/questions/199092/compiling-a-qt-program-in-windows-xp-with-mingws-g
*/

/*
Finds problems in MP3 files and helps the user to fix many of them using included tools. Looks at both the audio part (VBR info, quality, normalization) and the tags containing track information (ID3.) Also includes a tag editor and a file renamer.
*/


//PATH=D:\Qt\2009.02\qt\bin;%PATH%



/*
ttt1
dpkg-deb: building package `mp3diags' in `../mp3diags_0.99.01.009_i386.deb'.
dpkg-genchanges >../mp3diags_0.99.01.009_i386.changes
parsechangelog/debian: warning: debian/changelog(l9): found change data where expected next heading or eof
LINE: * improved font handling
parsechangelog/debian: warning: debian/changelog(l15): found change data where expected next heading or eof
LINE: * initial version
parsechangelog/debian: warning: debian/changelog(l9): found change data where expected next heading or eof
LINE: * improved font handling
parsechangelog/debian: warning: debian/changelog(l15): found change data where expected next heading or eof
LINE: * initial version
dpkg-genchanges: warning: the current version (0.99.01.009) is smaller than the previous one (unknown1)

*/


//ttt0 doc: use case: compare images, id3v2 tags

//ttt2 perhaps "Scan images in the current folder", checked by default
//ttt2 perhaps something to remove image files after assigning them, or at least show them in a different color; it was suggested to add a "-" button to remove images, below the "v" for "assigning them", but not sure it's such great idea; perhaps some option to delete local images that were assigned (but perhaps the unassigned CD scan should go as well); // perhaps "-" works, though; should be enabled/visible only for local files

//ttt0 see about C malloc failure
//ttt0 show Lyrics
//ttt1 handle Lyrics tag, display, transform, download ...

//ttt0 doc: visible transf hdr can be dragged
