
// AutoSumDoc.h : CAutoSumDoc ��Ľӿ�
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


typedef struct tagLINEINFO
{
	tagLINEINFO()
	{
		nLineNo = -1;
		dbValue = 0;
	}

	int nLineNo;
	double dbValue;
}LineInfo,*pLineInfo;


class CAutoSumDoc : public CDocument
{
protected: // �������л�����
	CAutoSumDoc();
	DECLARE_DYNCREATE(CAutoSumDoc)

// ����
public:
	std::vector<std::string>   m_vectorFilePath;//ѡ���ļ���ַ
	std::vector<std::string>   m_vectorContent;//�ı�����
	std::vector<std::string>   m_vectorsentences;//�ı��д��ִ���Ϣ���о���
	std::vector<std::string>   m_vectorRawSentences;//ԭʼ�ı������о���
	std::vector<int>           m_vectorRank;//���Ӽ���
	int                        m_KSentence;//ժҪ�����
	integer_1d_array m_ArrayDeg;//��¼�ڵ�Ķ�����
// ����
public:
	UINT    AutoSummarization(void);
	UINT    PretreatComment(void);//�ı�Ԥ����
	std::vector<int>	ComputingLexRank(std::vector<std::string> &Sentences,double dCT);
	double  idf_modified_cosine(std::string Sentence1,std::string Sentence2);
	double  **PowerMethod(double **pCosineMatrix,int nCount,double dError);
	real_2d_array  PowerMethod(const real_2d_array &CosineMatrix,int nCount,double dError);
	int		GetTF(std::string &strWord,std::string Sentence);
	double  GetIDF(std::string &strWord);
	bool    WriteSummary(std::string strText);//д�ļ�
	void    InitAutoSum();
// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CAutoSumDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


	
// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnFileOpen();
	afx_msg void OnDoAutosum();
};
