
// AutoSumView.cpp : CAutoSumView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "AutoSum.h"
#endif

#include "AutoSumDoc.h"
#include "AutoSumView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAutoSumView

IMPLEMENT_DYNCREATE(CAutoSumView, CView)

BEGIN_MESSAGE_MAP(CAutoSumView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CAutoSumView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CAutoSumView ����/����

CAutoSumView::CAutoSumView()
{
	// TODO: �ڴ˴���ӹ������

}

CAutoSumView::~CAutoSumView()
{
}

BOOL CAutoSumView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CAutoSumView ����

void CAutoSumView::OnDraw(CDC* /*pDC*/)
{
	CAutoSumDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CAutoSumView ��ӡ


void CAutoSumView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CAutoSumView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CAutoSumView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CAutoSumView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CAutoSumView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CAutoSumView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CAutoSumView ���

#ifdef _DEBUG
void CAutoSumView::AssertValid() const
{
	CView::AssertValid();
}

void CAutoSumView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAutoSumDoc* CAutoSumView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAutoSumDoc)));
	return (CAutoSumDoc*)m_pDocument;
}
#endif //_DEBUG


// CAutoSumView ��Ϣ�������
