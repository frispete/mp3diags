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


#include  <QPainter>
#include  <QApplication>
#include  <QTableView>
#include  <QHeaderView>

#include  "FilesModel.h"

#include  "CommonData.h"
#include  "NotesModel.h"
#include  "StreamsModel.h"


using namespace std;


//=====================================================================================================================
//=====================================================================================================================
//=====================================================================================================================

FilesModel::FilesModel(CommonData* pCommonData) : QAbstractTableModel(pCommonData->m_pFilesG), m_nPrevCurrentRow(-1), m_pCommonData(pCommonData)
{
}


//static int RR (0);

//#define USE_10000 // not quite OK to use, but allows some quick testing

/*override*/ int FilesModel::rowCount(const QModelIndex&) const
{
#ifdef USE_10000
    return 10000;
#else
    return cSize(m_pCommonData->getViewHandlers());
#endif
}

/*override*/ int FilesModel::columnCount(const QModelIndex&) const
{
    return m_pCommonData->getUniqueNotes().getFltCount() + 1;
}


/*extern*/ int CELL_WIDTH (1); // these get calculated based on the current font; however, they are also used
/*extern*/ int CELL_HEIGHT (1);

/*override*/ QVariant FilesModel::data(const QModelIndex& index, int nRole) const
{
    if (!index.isValid()) { return QVariant(); }
    int j (index.column());
    if (nRole == Qt::SizeHintRole && j > 0) { return QSize(CELL_WIDTH - 1, CELL_HEIGHT - 1); }  // !!! "-1" so one pixel can be used to draw the grid
//qDebug("cw %d", CELL_WIDTH);
#ifdef USE_10000
    if (index.row() >= cSize(m_pCommonData->getFltHandlers())) { return QVariant(); }
#endif

    const Mp3Handler* pHndl (m_pCommonData->getViewHandlers()[index.row()]);
    if (nRole == Qt::ToolTipRole && 0 == j)
    {
        QString s (convStr(pHndl->getName()));

        //QFontMetrics fm (QApplication::fontMetrics());
        QFontMetrics fm (m_pCommonData->m_pFilesG->fontMetrics()); // !!! no need to use "QApplication::fontMetrics()"
        int nWidth (fm.width(s));

        if (nWidth + 10 < m_pCommonData->m_pFilesG->horizontalHeader()->sectionSize(0)) // ttt2 "10" is hard-coded
        {
            //return QVariant();
            return ""; // !!! with "return QVariant()" the previous tooltip remains until the cursor moves over another cell that has a tooltip
        }//*/

        return s;
    }

    if (nRole != Qt::DisplayRole) { return QVariant(); }

    if (0 == j)
    {
        return convStr(pHndl->getName());
    }
    const NoteColl& notes (pHndl->getNotes());

    for (int i = 0, n = cSize(notes.getList()); i < n; ++i) // ttt2 poor performance
    {
        const Note* pNote (notes.getList()[i]);
        if (j - 1 == m_pCommonData->findPos(pNote)) { return "x"; }
    }

    return "";
}


/*override*/ QVariant FilesModel::headerData(int nSection, Qt::Orientation eOrientation, int nRole /*= Qt::DisplayRole*/) const
{
#if 0
    // 2008.07.28: no longer used because resizeColumnsToContents() cannot be called (it takes too long)
    if (nRole == Qt::SizeHintRole)
    {
        /*QVariant v (QAbstractTableModel::headerData(nSection, eOrientation, nRole)); // !!! doesn't work because QAbstractTableModel::headerData always returns an invalid value
        [...] */

        if (eOrientation == Qt::Vertical)
        {
            /*QFontMetrics fm (m_pCommonData->m_pFilesG->fontMetrics());
            double d (1.01 + log10(double(m_pCommonData->getFltHandlers().size())));
            int n (d);
            QString s (n, QChar('9'));
            //QSize size (fm.boundingRect(r, Qt::AlignTop | Qt::TextWordWrap, s).size());
            int nWidth (fm.width(s));
            //return QSize(50, CELL_HEIGHT);
            return QSize(nWidth + 10, CELL_HEIGHT); //tttx perhaps use data cell height of 17 (fontMetrics().height()) (so header cell height is 18 ("+1" to account for the grid) //ttt2 hard-coded "10"*/
            getNumVertHdrSize ...
        }

        return QVariant();
    }
#endif

    if (nRole != Qt::DisplayRole) { return QVariant(); }
    if (Qt::Horizontal == eOrientation)
    {
        if (0 == nSection) { return "File name"; }
        const vector<const Note*>& v (m_pCommonData->getUniqueNotes().getFltVec());
        int nSize (cSize(v));
        if (nSection > nSize)
        {
            return ""; //ttt2 see why is this needed; it wasn't so until version 134 (handling of MainFormDlgImpl::resizeEvent() or of MainFormDlgImpl::on_m_pMainTabWidget_currentChanged() seem to have something to do with this)
        }
        return getNoteLabel(v[nSection - 1]);
    }

    return nSection + 1;
}


// makes current and selects the specified row and emits a change signal regardless of the element that was selected before; makes current the default invalid index (-1,-1) if the table is empty;
void FilesModel::selectRow(int nRow, const vector<int>& vnSel /*= std::vector<int>()*/)
{
    QItemSelectionModel* pSelModel (m_pCommonData->m_pFilesG->selectionModel());
//m_pCommonData->printFilesCrt();

    {
        NonblockingGuard g (m_pCommonData->m_bChangeGuard);
        if (!g) { return; }
        pSelModel->clear(); // 2008.07.08: this doesn't work quite as expected: it does trigger SelectionChanged, but it changes the current item after emitting the signal, so whoever catches it, doesn't know what the current item is (it's probably in the second param of CurrentChanged, but that doesn't help SelectionChanged, which has QItemSelection params, which don't seem to offer any way to get at the "current" index)
    }

//m_pCommonData->printFilesCrt();

    emit layoutChanged();

    m_nPrevCurrentRow = -2; // to make sure that onFilesGSelChanged() updates the notes and streams regardless of whether there are any files in m_pFilesG or not
    if (!m_pCommonData->getViewHandlers().empty())
    {
        //pSelModel->select(index(0, 0), QItemSelectionModel::Current);
        //pSelModel->select(index(0, 0), QItemSelectionModel::Select);

        //m_pCommonData->printFilesCrt();
        m_pCommonData->m_pFilesG->setCurrentIndex(index(nRow, 0)); // this sometimes calls onFilesGSelChanged(), but not always;
        //m_pCommonData->printFilesCrt();
    }

    if (-2 == m_nPrevCurrentRow)
    {
        onFilesGSelChanged();
    }

    for (int i = 0, n = cSize(vnSel); i < n; ++i)
    {
        pSelModel->select(pSelModel->model()->index(vnSel[i], 0), QItemSelectionModel::Select);
    }
}


// this gets called more often than expected: besides the cases when it gets called recursively and exits immediately, it is called on MousePressed, MouseReleased and Clicked (which is sent at the later stage of MouseReleased processing)
void FilesModel::onFilesGSelChanged()
{
    NonblockingGuard g (m_pCommonData->m_bChangeGuard);
    if (!g) { return; }
//m_pCommonData->printFilesCrt();
    int nGridCrt (m_pCommonData->getFilesGCrtRow());

    if (m_nPrevCurrentRow != nGridCrt)
    {
        //m_pCommonData->m_pNotesModel->updateCurrentNotes();
        //m_pCommonData->m_pStreamsModel->updateCurrentStreams();
        emit currentFileChanged();
        m_nPrevCurrentRow = nGridCrt;
    }

    fixSelection();

    m_pCommonData->m_pNotesModel->matchSelToMain();
    m_pCommonData->m_pStreamsModel->matchSelToNotes();
}


void FilesModel::fixSelection() // deselects cells that are selected but are on a different row from the "current" cell and selects the file name
{
    // it works OK when getFltHandlers() is empty
    QItemSelectionModel* pSelModel (m_pCommonData->m_pFilesG->selectionModel());
    QModelIndexList lstSel (pSelModel->selection().indexes());
    QModelIndex crt (m_pCommonData->m_pFilesG->selectionModel()->currentIndex());
    int nCrtRow (crt.row());
    int nCrtCol (crt.column());

    if (0 == nCrtCol)
    {
        for (QModelIndexList::iterator it = lstSel.begin(), end = lstSel.end(); it != end; ++it)
        {
            if (0 != it->column())
            {
                pSelModel->select(*it, QItemSelectionModel::Deselect);
            }
        }
    }
    else
    {
        set<int> sSelectableColumns;
        sSelectableColumns.insert(0);
        for (int i = 0, n = cSize(m_pCommonData->getCrtNotes()); i < n; ++i) // ttt2 poor performance
        {
            const Note* pNote (m_pCommonData->getCrtNotes()[i]);
            sSelectableColumns.insert(m_pCommonData->findPos(pNote) + 1);
        }

        for (QModelIndexList::iterator it = lstSel.begin(), end = lstSel.end(); it != end; ++it)
        {
            if ((it->row() != nCrtRow && 0 != it->column()) || 0 == sSelectableColumns.count(it->column()))
            {
                pSelModel->select(*it, QItemSelectionModel::Deselect);
            }
        }

        if (nCrtRow >= 0)
        {
            pSelModel->select(index(nCrtRow, 0), QItemSelectionModel::Select);
        }
    }
}


void FilesModel::matchSelToNotes()
{
    QItemSelectionModel* pNotesSelModel (m_pCommonData->m_pNotesG->selectionModel());
    QModelIndexList notesLstSel (pNotesSelModel->selection().indexes());

    set<int> sSel;
    for (QModelIndexList::iterator it = notesLstSel.begin(), end = notesLstSel.end(); it != end; ++it)
    {
//cout << "cell sel at " << it->row() << "x" << it->column() << endl;
        const Note* pNote (m_pCommonData->getCrtNotes()[it->row()]);
        int nPos (m_pCommonData->findPos(pNote));
        sSel.insert(nPos);
    }

    m_pCommonData->m_pFilesG->selectionModel()->clearSelection();

    for (set<int, int>::iterator it = sSel.begin(), end = sSel.end(); it != end; ++it)
    {
        m_pCommonData->m_pFilesG->selectionModel()->select(index(m_pCommonData->getFilesGCrtRow(), *it + 1), QItemSelectionModel::Select);
//cout << "selecting " << m_pCommonData->getFilesGCrtRow() << "x" << *it + 1 << endl;
    }

    m_pCommonData->m_pFilesG->selectionModel()->select(index(m_pCommonData->getFilesGCrtRow(), 0), QItemSelectionModel::Select);
}




//=====================================================================================================================
//=====================================================================================================================
//=====================================================================================================================

/*override*/ void FilesGDelegate::paint(QPainter* pPainter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{ // adapted from Trolltech's Pixelator (GPL V2 or V3) http://doc.trolltech.com/4.3/itemviews-pixelator.html
    int nCol (index.column());
    if (0 == nCol)
    {
        return QItemDelegate::paint(pPainter, option, index);
    }

    //int nRow (index.row());
    double r ((index.model()->data(index, Qt::DisplayRole).toString() == "" ? 0 : (CELL_HEIGHT - 0)/5));

    pPainter->save();
    pPainter->setRenderHint(QPainter::Antialiasing, true);
    pPainter->setPen(Qt::NoPen);

    const Note* pNote (m_pCommonData->getUniqueNotes().getFlt(nCol - 1));

    bool bSel (0 != (QStyle::State_Selected & option.state));
    bool bCrt (0 != (QStyle::State_HasFocus & option.state));
    bool bActive (0 != (QStyle::State_Active & option.state)); // "active" is true if the parent window has keyboard focus

    /*QColor colSev (getNoteColor(*pNote));  //ttt2 perhaps try to derive all these colors from the global pallette (e.g. option.palette.highlight(), option.palette.highlightedText(), ...)
    QColor colBkg (colSev);
    QColor colFg (option.palette.color(QPalette::Active, QPalette::Highlight)); //ttt3 not necessarily "Active"
    //QColor colFg (Qt::black); //ttt3 not necessarily "Active"
    if (colFg.green() >= 160 && colFg.red() >= 160)
    {
        colFg = QColor(0, 0, 0);
    }

    if (bSel)
    {
        QColor c (colBkg);
        colBkg = colFg;
        colFg = c;
    }*/

    QColor colSev (getNoteColor(*pNote));  //ttt2 perhaps try to derive all these colors from the global pallette (e.g. option.palette.highlight(), option.palette.highlightedText(), ...)
    QColor colSel (option.palette.color(QPalette::Active, QPalette::Highlight)); //ttt3 not necessarily "Active"

    QColor colFg, colBkg;

    colBkg = bSel ? colSel : colSev;
    if (colSel.green() >= 160 && colSel.red() >= 160)
    { // for better contrast we use something dark if the "highlight" color is light
        //colFg = QColor(0, 0, 0);
        colFg = option.palette.color(QPalette::Active, QPalette::HighlightedText);
    }
    else
    {
        colFg = bSel ? colSev : colSel;
    }

    pPainter->fillRect(option.rect, colBkg);
    if (0 != r)
    {
        pPainter->setBrush(QBrush(colFg));

        pPainter->drawEllipse(
                QRectF(option.rect.x() + option.rect.width()/2.0 - r,
                    option.rect.y() + option.rect.height()/2.0 - r,
                    2*r, 2*r));
    }

    if (bCrt && bActive)
    {
        pPainter->setRenderHint(QPainter::Antialiasing, false);
        const int ADJ (0);
        QRect r (option.rect);
        r.adjust(ADJ, ADJ, -ADJ - 1, -ADJ - 1);
        pPainter->setBrush(QBrush(Qt::NoBrush));

        QPen pen (pPainter->pen());

        pen.setStyle(Qt::DotLine);
        //pen.setColor(Qt::black);
        pen.setColor(colFg);
        pPainter->setPen(pen);
        pPainter->drawRect(r);
    }

    pPainter->restore();
}


/*override*/ QSize FilesGDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (!index.isValid()) { return QSize(); }
    if (index.column() > 0) { return QItemDelegate::sizeHint(option, index); }

    int nMargin (QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1);

    int j (index.column());
    int nColWidth (((const QTableView*)parent())->horizontalHeader()->sectionSize(j));
    QRect r (0, 0, nColWidth - 2*nMargin - 1, 10000);

    QSize res (option.fontMetrics.boundingRect(r, Qt::AlignTop | Qt::TextWordWrap, index.data(Qt::DisplayRole).toString()).size());

    res.setWidth(nColWidth);

    return res;
}

