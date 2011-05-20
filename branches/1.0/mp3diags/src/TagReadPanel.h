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


#include  <vector>

#include  <QFrame>

#include  "CommonTypes.h"


struct TagReader;
class QToolButton;
class QLabel;


class ImageInfoPanel : public QFrame
{
    Q_OBJECT

    ImageInfo m_imageInfo;
    QToolButton* m_pBtn;
    QLabel* m_pInfoLabel;

    void createButton(int nSize);
public:
    ImageInfoPanel(QWidget* pParent, const ImageInfo& imageInfo);
    void resize(int nSize); // removes the label and makes the button smaller

protected slots:
    void onShowFull();
};


class TagReadPanel : public QFrame
{
    Q_OBJECT

    QWidget* m_pImgWidget;
    std::vector<ImageInfoPanel*> m_vpImgPanels;
public:
    TagReadPanel(QWidget* pParent, TagReader* pTagReader);

protected slots:
    void onCheckSize();
};


