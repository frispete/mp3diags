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


#ifndef MainFormDlgImplH
#define MainFormDlgImplH

#include  <memory>

#include  <QDialog>

#include  "ui_MainForm.h"

#include  "StructuralTransformation.h"
#include  "CommonData.h"
#include  "StoredSettings.h"

class ModifInfoToolButton;
class TagWriter;
class QScrollArea;
class QStackedLayout;

class ModifInfoToolButton;

class FileEnumerator;

/**
    @author ciobi
*/
class MainFormDlgImpl : public QDialog, private Ui::MainFormDlg
{
Q_OBJECT
public:
    MainFormDlgImpl(QWidget* pParent, const std::string& strSession);

    ~MainFormDlgImpl();

    /*override*/ void resizeEvent(QResizeEvent* pEvent);

    /*override*/ void keyPressEvent(QKeyEvent* pEvent);
    /*override*/ void keyReleaseEvent(QKeyEvent* pEvent);
    /*override*/ void closeEvent(QCloseEvent* pEvent);

    enum CloseOption { EXIT, OPEN_SESS_DLG };
    CloseOption run();

public slots:
    /*$PUBLIC_SLOTS$*/
    void on_m_pScanB_clicked();
    void on_m_pSessionsB_clicked();

    void on_m_pNoteFilterB_clicked();
    void on_m_pDirFilterB_clicked();
    void on_m_pConfigB_clicked();

    void on_m_pModeAllB_clicked();
    void on_m_pModeAlbumB_clicked();
    void on_m_pModeSongB_clicked();

    void on_m_pPrevB_clicked();
    void on_m_pNextB_clicked();

    void on_m_pTransformB_clicked();
    void on_m_pCustomTransform1B_clicked() { applyCustomTransf(0); }
    void on_m_pCustomTransform2B_clicked() { applyCustomTransf(1); }
    void on_m_pCustomTransform3B_clicked() { applyCustomTransf(2); }
    void on_m_pCustomTransform4B_clicked() { applyCustomTransf(3); }
    void on_m_pNormalizeB_clicked();
    void on_m_pReloadB_clicked();

    void on_m_pViewFileInfoB_clicked();
    void on_m_pViewAllNotesB_clicked();
    void on_m_pViewTagDetailsB_clicked();

    void on_m_pAboutB_clicked();

    void on_m_pTagEdtB_clicked();
    void on_m_pRenameFilesB_clicked();
    void on_m_pDebugB_clicked();

    void emptySlot() {} // needed to disable exiting on ESCAPE

    void onCrtFileChanged();

    void testSlot();

    void onShow();
    void onShowAssert();

private:
    void scan(FileEnumerator& fileEnum, bool bForce, std::deque<const Mp3Handler*> vpExisting, int nKeepWhenUpdate); // a subset of vpExisting gets copied to vpDel in the m_pCommonData->mergeHandlerChanges() call; so if vpExisting is empty, vpDel will be empty too; if bForce is true, thw whole vpExisting is copied to vpDel;

    void scan(bool bForce);
    enum { IGNORE_SEL = 0, USE_SEL = 1 };
    //enum { KEEP_FLT = 0, DISABLE_FLT = 1 };
    //enum ReloadSrc { SEL, ALL_KEEP_FLT, ALL_REMOVE_FLT };
    enum { DONT_FORCE = 0, FORCE = 1 };
    //void reload(ReloadSrc eReloadSrc, bool bForce);
    void reload(bool bSelOnly, bool bForce);

    SessionSettings m_settings;

    int m_nLastKey;

    friend void trace(const std::string& s);
    CommonData* m_pCommonData;

    QWidget* m_pTagDetailsW; // this gets erased and recreated each time the current file changes
    QHBoxLayout* m_pTagDetailsLayout;
    QStackedLayout* m_pLowerHalfLayout;

    TransfConfig m_transfConfig;

    void saveIgnored();
    void loadIgnored();

    void saveCustomTransf(int k);
    void loadCustomTransf(int k);

    enum { SELECTED, ALL };
    void transform(std::vector<Transformation*>& vpTransf, bool bAll);

    std::vector<ModifInfoToolButton*> m_vpTransfButtons;
    void setTransfTooltip(int k);
    void setTransfTooltips() { for (int i = 0; i < CUSTOM_TRANSF_CNT; ++i) { setTransfTooltip(i); } }
    void applyCustomTransf(int k);

    ModifInfoToolButton* m_pModifNormalizeB;
    ModifInfoToolButton* m_pModifReloadB;
    std::string m_strSession;

    void resizeIcons();
    void updateUi(const std::string& strCrt); // strCrt may be empty

    /*override*/ bool eventFilter(QObject* pObj, QEvent* pEvent);

    void initializeUi();
    bool m_bShowMaximized;
    int m_nScanWidth;

signals:
    void tagEditorClosed();
};

class AssertSender : public QObject
{
Q_OBJECT
    //MainFormDlgImpl* m_pDlg;
public:
    AssertSender(MainFormDlgImpl* pDlg)
    {
        connect(this, SIGNAL(showAssertMsg()), pDlg, SLOT(onShowAssert()));
        emit showAssertMsg();
    }
    //void emitAssrt() { emit assrt(); }
signals:
    void showAssertMsg();
};



#endif // #ifndef MainFormDlgImplH










