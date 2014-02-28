
// AutoSumView.cpp : CAutoSumView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CAutoSumView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CAutoSumView 构造/析构

CAutoSumView::CAutoSumView()
{
	// TODO: 在此处添加构造代码

}

CAutoSumView::~CAutoSumView()
{
}

BOOL CAutoSumView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CAutoSumView 绘制

void CAutoSumView::OnDraw(CDC* /*pDC*/)
{
	CAutoSumDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CAutoSumView 打印


void CAutoSumView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CAutoSumView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CAutoSumView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CAutoSumView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
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


// CAutoSumView 诊断

#ifdef _DEBUG
void CAutoSumView::AssertValid() const
{
	CView::AssertValid();
}

void CAutoSumView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAutoSumDoc* CAutoSumView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAutoSumDoc)));
	return (CAutoSumDoc*)m_pDocument;
}
#endif //_DEBUG


// CAutoSumView 消息处理程序
