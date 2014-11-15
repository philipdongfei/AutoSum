
// AutoSumDoc.h : CAutoSumDoc 类的接口
//
#include <vector>

#include <math.h>
#include "alglibinternal.h"
#include "alglibmisc.h"
//#include "linalg.h"
#include "statistics.h"
#include "dataanalysis.h"
#include "specialfunctions.h"
#include "solvers.h"
#include "optimization.h"
#include "diffequations.h"
#include "fasttransforms.h"
#include "integration.h"
#include "interpolation.h"

using namespace alglib;



#pragma once


class CAutoSumDoc : public CDocument
{
protected: // 仅从序列化创建
	CAutoSumDoc();
	DECLARE_DYNCREATE(CAutoSumDoc)

// 特性
public:
	std::vector<std::string>   m_vectorFilePath;//选中文件地址
	std::vector<std::string>   m_vectorContent;//文本内容
	std::vector<std::string>   m_vectorsentences;//文本中所有句子
// 操作
public:
	UINT    AutoSummarization(void);
	UINT    PretreatComment(void);//文本预处理
	real_2d_array	ComputingLexRank(std::vector<std::string> &Sentences,double dCT);
	double  idf_modified_cosine(std::string Sentence1,std::string Sentence2);
	double  **PowerMethod(double **pCosineMatrix,int nCount,double dError);
	real_2d_array  PowerMethod(const real_2d_array &CosineMatrix,int nCount,double dError);
	int		GetTF(std::string &strWord,std::string Sentence);
	double  GetIDF(std::string &strWord);
	bool    WriteSummary(std::string strText);//写文件
// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CAutoSumDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	integer_1d_array m_ArrayDeg;//记录节点的度数组
// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnFileOpen();
	afx_msg void OnDoAutosum();
};
