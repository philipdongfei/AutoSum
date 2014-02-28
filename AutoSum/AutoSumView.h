
// AutoSumView.h : CAutoSumView 类的接口
//

#pragma once


class CAutoSumView : public CView
{
protected: // 仅从序列化创建
	CAutoSumView();
	DECLARE_DYNCREATE(CAutoSumView)

// 特性
public:
	CAutoSumDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CAutoSumView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // AutoSumView.cpp 中的调试版本
inline CAutoSumDoc* CAutoSumView::GetDocument() const
   { return reinterpret_cast<CAutoSumDoc*>(m_pDocument); }
#endif

