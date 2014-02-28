
// AutoSumDoc.cpp : CAutoSumDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "AutoSum.h"
#endif

#include "AutoSumDoc.h"

#include <propkey.h>

#ifndef OS_LINUX
#include <Windows.h>
#pragma comment(lib, "ICTCLAS50.lib") //ICTCLAS50.lib库加入到工程中
#endif

#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#include <iostream>
#include  <string>
#include "ICTCLAS50.h"
#include <objbase.h>
#include <atlbase.h>
#include <io.h>
#include <math.h>

#include "Graph.h"
//#include "Matrix.h"




//#include <msxml6.h>  
//#include <comutil.h>  
//#pragma comment(lib, "comsuppwd.lib")  

//#import "msxml6.dll" //引入类型库 
//using namespace MSXML2;
//using namespace ATL; 


using namespace std;



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UINT    StarSummary(LPVOID pParam);



// CAutoSumDoc

IMPLEMENT_DYNCREATE(CAutoSumDoc, CDocument)

BEGIN_MESSAGE_MAP(CAutoSumDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, &CAutoSumDoc::OnFileOpen)
	ON_COMMAND(ID_DO_AUTOSUM, &CAutoSumDoc::OnDoAutosum)
END_MESSAGE_MAP()


// CAutoSumDoc 构造/析构

CAutoSumDoc::CAutoSumDoc()
{
	// TODO: 在此添加一次性构造代码

}

CAutoSumDoc::~CAutoSumDoc()
{
}

BOOL CAutoSumDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CAutoSumDoc 序列化

void CAutoSumDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CAutoSumDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CAutoSumDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CAutoSumDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CAutoSumDoc 诊断

#ifdef _DEBUG
void CAutoSumDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAutoSumDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CAutoSumDoc 命令


void CAutoSumDoc::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	    CFileDialog mFileDlg(TRUE, NULL,NULL, 
                         OFN_ALLOWMULTISELECT,
                         	_T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"),
                         AfxGetMainWnd()); 
	int NAMEBUF = 1024*1024;
	mFileDlg.m_ofn.lpstrFile=new TCHAR[NAMEBUF];   // 重新定义 lpstrFile 缓冲大小
	memset(mFileDlg.m_ofn.lpstrFile,0,NAMEBUF);  // 初始化定义的缓冲 
	mFileDlg.m_ofn.nMaxFile = NAMEBUF;           // 重定义 nMaxFile 
    CString pathName,FileName;
	std::string t_string;
//	GetCurrentDirectory(MAX_PATH,CurrentDir);
	char CurrentDir[256];
	memset(CurrentDir,0,256);
	
    GetCurrentDirectory(255,CurrentDir);
   
    if(mFileDlg.DoModal ()==IDOK)
    { 
		SetCurrentDirectory(CurrentDir);
	
        POSITION mPos=mFileDlg.GetStartPosition(); 
        while(mPos!=NULL) 
        { 
             pathName=mFileDlg.GetNextPathName(mPos);
			 t_string = pathName;
			 m_vectorFilePath.push_back(t_string);
		
		} 
	
    }
	delete [] mFileDlg.m_ofn.lpstrFile;             // 切记使用完后释放资源  
}


void CAutoSumDoc::OnDoAutosum()
{
	// TODO: 在此添加命令处理程序代码
	if (m_vectorFilePath.size() > 0)
	{
		CWinThread *pThread = ::AfxBeginThread(StarSummary,this);
	}

}

//AutoSummarization函数：生成摘要
//参数： void
//返回值： UINT 摘要句子个数
UINT  CAutoSumDoc::AutoSummarization(void)
{
	int nCount(0);

	if((nCount =PretreatComment()) > 0)//分词和句子成功
	{
		TRACE("auto summarization\n");
		
		double dCT = 0.1;
		double **pLexRank = ComputingLexRank(m_vectorsentences,dCT);
		dCT = 0.2;
		ComputingLexRank(m_vectorsentences,dCT);
		dCT = 0.3;
		ComputingLexRank(m_vectorsentences,dCT);


		
		
	}

	return nCount;
}
//PretreatComment函数：文本预处理
//参数： void
//返回值： UINT  0:失败 n:返回原文本中句子个数
UINT  CAutoSumDoc::PretreatComment(void)
{
	UINT nRet(0);
	std::string  strSentence("");

	for (int nIndex1=0; nIndex1 < m_vectorFilePath.size(); nIndex1++)
	{

		
		std::string strPath = m_vectorFilePath[nIndex1];
		if (strPath.size() > 0)
		{
			CFile t_hFile;
			CFileException e;
			char *pBuf=NULL;
			int nSize;

			if (t_hFile.Open(strPath.c_str(),CFile::modeRead,&e))
			{
				nSize = t_hFile.GetLength();
				pBuf = new char[nSize+1];
				memset(pBuf,0,nSize+1);
				t_hFile.Read(pBuf,nSize);
				t_hFile.Close();
			}

			if(!ICTCLAS_Init()) //初始化分词组件。
			{
				printf("Init fails\n");  
				continue;
			}
			else
			{
				printf("Init ok\n");
			}

			CString strID(_T("")),strTitle(_T("")),strColumn(_T("")),strKeywords(_T(""));
			const char* pszDictBuffer="日圆@@nr;纽元@@nr"; //字符串形式导入用户词典
			unsigned int nItems=ICTCLAS_ImportUserDict (pszDictBuffer, strlen(pszDictBuffer),CODE_TYPE_UNKNOWN);
			//设置词性标注集(0 计算所二级标注集，1 计算所一级标注集，2 北大二级标注集，3 北大一级标注集)
			ICTCLAS_SetPOSmap(2);
			int nPos4(0);
			char Tempp[512];

			for(int i=0; i<4; i++)
			{
				memset(Tempp,0x0,sizeof(Tempp));
				char* pDest = strstr(pBuf+nPos4,"+++++");
				memcpy(Tempp,pBuf+nPos4,pDest-pBuf-nPos4);
				switch(i)
				{
				case 0:
					strID = Tempp;
					break;
				case 1:
					strTitle =Tempp;
					break;
				case 2:
					strColumn =Tempp;
					break;
				case 3:
					strKeywords =Tempp;
					break;
				}
				nPos4 = pDest-pBuf;
				nPos4 += 5;
			}
			int nIndex(0),nPos(0),nPos1(0),nPos2(0),nPos3(0);
			char* sSentence=pBuf+nPos4;
			std::string strContent = sSentence;
			m_vectorContent.push_back(strContent);

			char* sRst=0;   //用户自行分配空间，用于保存结果；
			sRst=(char *)malloc(nSize*6); //建议长度为字符串长度的倍。
			memset(sRst,0x0,nSize*6);
			int nRstLen=0; //分词结果的长度
			ICTCLAS_SetPOSmap(0);

			TRACE("%d:%s\n",nIndex,sSentence);
			nRstLen = ICTCLAS_ParagraphProcess(sSentence,nSize,sRst,CODE_TYPE_UNKNOWN,1);  //字符串处理

			int nSencNo(1),nWordNo(1),nLineNo(1),nTotal(0);
			CString szLine(_T("")),szRoot(_T("")),szTemp(_T("")),szWord(_T("")),szLine1(_T(""));

			char seps[] = "\r\n";
			char *token=NULL;
			CString szType(_T(""));
			token = strtok(sRst,seps);
			while(token != NULL)
			{

				szLine1 = token;
				nTotal += szLine1.GetLength();
				///////////////
				szTemp.Format("%d",nWordNo);

				/////////////////////解析每行
				//nPos2 = szLine.Find("/",0);

				nPos2 = 0;
				while((nPos1 = szLine1.Find(" ",nPos2)) >= 0)
				{
					szWord = szLine1.Mid(nPos2,nPos1-nPos2);
					nPos3 = szWord.Find("/",0);
					if (nPos3 > 0)
					{
						szType = szWord.Mid(nPos3+1);
					}
					else
					{
						nPos2 = nPos1 + 1;
						continue;
					}

				//	szWord = szWord.Left(nPos3);	
					strSentence += szWord;
					strSentence += '|';
				//	szTemp.Format("%c",szType.GetAt(0));
				//	szLine.Format(szWordFormat,nWordNo,nWordNo,szType.GetAt(0),szWord);
				//	t_file.Write(szLine,szLine.GetLength());
					if (szType.GetAt(0) == 'w')
					{
						if (szType.GetLength() > 1 && szType.GetAt(1)=='j')
						{
							++nSencNo;
							nWordNo = 0;
						
							m_vectorsentences.push_back(strSentence);
							strSentence.clear();
						
						}
					}
					++nWordNo;
					nPos2 = nPos1 + 1;
				}

				////////////////////////end
				++nLineNo;
				token = strtok(NULL,seps);
			}
			m_vectorsentences.push_back(strSentence);
			free(sRst);
			sRst = NULL;
			ICTCLAS_Exit();	//释放资源退出
			delete []pBuf;
			pBuf = NULL;


		}
	}
	nRet = m_vectorsentences.size();
	return nRet;
}

double** CAutoSumDoc::ComputingLexRank(std::vector<std::string> &Sentences,double dCT)
{
	int nCount = Sentences.size();
	double **pLexRank = NULL;//new double[nCount];
//	memset(pLexRank,0,nCount);
	int *pDegree = new int[nCount];
	memset(pDegree,0,sizeof(int)*nCount);
	double **pCosineMatrix = new double*[nCount];//cosinematrix 句子之间相似度
	pCosineMatrix[0] = new double[nCount*nCount];		
	for(int i=1; i<nCount; i++)
	{
		pCosineMatrix[i] = pCosineMatrix[i-1]+nCount;
	}

	real_2d_array  CosineMatrix;
	CosineMatrix.setlength(nCount,nCount);
	
	int nI,nJ;
	for( nI = 0; nI < nCount; nI++)
	{
		for( nJ = 0; nJ < nCount; nJ++)
		{
			//*((pCosineMatrix+nI*nCount*sizeof(double))[nJ])
			pCosineMatrix[nI][nJ] = 0.0;
			TRACE("%d:%s\n",nI,Sentences[nI].c_str());
			TRACE("%d:%s\n",nJ,Sentences[nJ].c_str());
			double dd;
			if (nI != nJ)
				dd = idf_modified_cosine(Sentences[nI],Sentences[nJ]);
			else 
				dd = 0;
			pCosineMatrix[nI][nJ] = dd;
			CosineMatrix[nI][nJ] = dd;
			if (pCosineMatrix[nI][nJ] > dCT)
			{
				pCosineMatrix[nI][nJ] = 1.0;
				CosineMatrix[nI][nJ] = 1.0;
				pDegree[nI]++;
			}
			else
			{
				pCosineMatrix[nI][nJ] = 0.0;
				CosineMatrix[nI][nJ] = 0.0;
			}
		}
	}



	////////////////////////find strongly subgraph

	Graph<int,double> m_graph(pCosineMatrix,nCount);   
	char *pBuf = new char[4*nCount];
	memset(pBuf,0,4*nCount);
	int nSubNum = m_graph.DFS_Traveling(0,pBuf);

	TRACE("SubNum=%d,Sub=%s\n",nSubNum,pBuf);

	string subgraphs(pBuf),subgroup(_T(""));

	string::size_type pos(0),pos1(0),pos2(0),pos3(0);
	int nSubCount(0),nIndex_I(0),nIndex_J(0);
	while ((pos1 = subgraphs.find_first_of("|", pos)) != string::npos) {
		subgroup = subgraphs.substr(pos,pos1-pos);
		string::size_type subpos(0);
		subpos = subgroup.find_last_of(",");
		string number = subgroup.substr(subpos+1);
		subgroup = subgroup.substr(0,subpos+1);
		nSubCount = atoi(number.c_str());
		if (nSubCount <= 1)
		{
			pos = pos1+1;
			continue;
		}
		real_2d_array  tCosineMatrix;
		tCosineMatrix.setlength(nSubCount,nSubCount);
	
		////////////get matrix
		char temp[8];
		int nDegree = 0;

		for( nI = 0; nI < nCount; nI++)
		{
			
			memset(temp,0,sizeof(temp));
			sprintf(temp,"%d,",nI);
			if ( (pos2 = subgroup.find(temp)) == string::npos)
			{
				continue;
			}
			else if (pos2 > 0)
			{
				if (subgroup.at(pos2-1) != ',')
						continue;
			}
			TRACE("\n %d:",nI);
		//	++nSubCount;
			nDegree = nIndex_J = 0;
			for( nJ = 0; nJ < nCount; nJ++)
			{
				memset(temp,0,sizeof(temp));
				sprintf(temp,"%d,",nJ);
				if ((pos3 = subgroup.find(temp)) == string::npos)
				{
					continue;
				}
				else if (pos3 > 0)
				{
					if (subgroup.at(pos3-1) != ',')
							continue;

				}
				if (pCosineMatrix[nI][nJ] == 1.0)
					++nDegree;
			//	double p =  pCosineMatrix[nI][nJ]/pDegree[nI];
			//	pCosineMatrix[nI][nJ] = pCosineMatrix[nI][nJ]/pDegree[nI];
			//	tCosineMatrix[nI][nJ] = CosineMatrix[nI][nJ]/pDegree[nI];
				tCosineMatrix[nIndex_I][nIndex_J] = pCosineMatrix[nI][nJ];///pDegree[nI];
				TRACE("%f ",pCosineMatrix[nI][nJ]);
				++nIndex_J;
				
			}
			TRACE("\n");
			for(nIndex_J=0;nIndex_J<nSubCount;nIndex_J++)
			{
				TRACE("%f ",tCosineMatrix[nIndex_I][nIndex_J]/(double)nDegree);
				if (nDegree > 0)
				tCosineMatrix[nIndex_I][nIndex_J] = tCosineMatrix[nIndex_I][nIndex_J]/nDegree;
				else
					tCosineMatrix[nIndex_I][nIndex_J] =1.0/(double)nSubCount;
			}
			++nIndex_I;
		}
		TRACE("\n%s\n",tCosineMatrix.tostring(5).c_str());
		double dError(0.0);
		real_2d_array  P = PowerMethod(tCosineMatrix,nSubCount,dError);
		//////////////
		
		pos = pos1;
		++pos;
	}
	if(pBuf)
	{
		delete []pBuf;
		pBuf = NULL;
	}
	/////////////////////////////////end
	TRACE("\nCM=%s\n",CosineMatrix.tostring(0).c_str());
	for( nI = 0; nI < nCount; nI++)
	{
		TRACE("\n %d:",nI);
		for( nJ = 0; nJ < nCount; nJ++)
		{
			if (pDegree[nI] > 1)
			{
				double p =  pCosineMatrix[nI][nJ]/(double)pDegree[nI];
				pCosineMatrix[nI][nJ] = pCosineMatrix[nI][nJ]/(double)pDegree[nI];
				CosineMatrix[nI][nJ] = CosineMatrix[nI][nJ]/(double)pDegree[nI];
			}
			else
			{
				pCosineMatrix[nI][nJ] = 1.0/(double)nCount;
				CosineMatrix[nI][nJ] = 1.0/(double)nCount;
			}
			TRACE("%f ",CosineMatrix[nI][nJ]);
		}

	}
	TRACE("\nCM=%s\n",CosineMatrix.tostring(5).c_str());
	
	double dError(0.0);
	real_2d_array  P = PowerMethod(CosineMatrix,nCount,dError);
//	pLexRank = PowerMethod(pCosineMatrix,nCount,dError);
	delete []pDegree;
	if(pCosineMatrix)
	{
		delete [](pCosineMatrix[0]);
		delete []pCosineMatrix;
		pCosineMatrix = NULL;
	}

	return pLexRank;
}

double  CAutoSumDoc::idf_modified_cosine(std::string Sentence1,std::string Sentence2)
{
	double dIdf(0.0);
	std::vector<std::string> vectorWords,vectorWord1,vectorWord2;
	std::string  t_word("");
	double Numerator2(0.0),Numerator1(0.0),Denominator1(0.0),Denominator2(0.0);
	int nPos1(0),nPos2(0);
	double tf1(0.0),tf2(0.0),idf1(0.0),idf2(0.0);

	while((nPos1 = Sentence1.find("|",nPos2)) > 0)
	{
		t_word = Sentence1.substr(nPos2,nPos1-nPos2);
		int pp = t_word.find("/w");
		if ( pp > 0)//过滤标点
		{
			nPos2 = nPos1+1;
			continue;
		}
		tf1 = GetTF(t_word,Sentence1);
		tf2 = GetTF(t_word,Sentence2);
		idf1 = GetIDF(t_word);
	//	if ((Sentence1.find(t_word)+t_word.size()) >= nPos1)
	//	if (tf1 > 0 && tf2 > 0)
		{
		//	TRACE("%s,tf1=%d,tf2=%d,idf=%f\n",t_word.c_str(),tf1,tf2,idf1);
		}
		pp = Sentence1.find(t_word)+t_word.size();
		if ((Sentence1.find(t_word)+t_word.size()) == nPos1)
		{
			if (tf1 > 0 && tf2 > 0)
	//		TRACE("%s,tf1=%f,tf2=%f,idf=%f\n",t_word.c_str(),tf1,tf2,idf1);
			Numerator1 += (tf1*tf2*pow(idf1,2.0));
		}
		Denominator1 += pow(tf1*idf1,2.0);
		
		nPos2 = nPos1+1;
	}
	nPos1=0,nPos2 = 0;
	while((nPos1 = Sentence2.find("|",nPos2)) > 0)
	{
		t_word = Sentence2.substr(nPos2,nPos1-nPos2);
		int pp =  t_word.find("/w");
		if (pp > 0)//过滤标点
		{
			nPos2 = nPos1+1;
			continue;
		}
		tf1 = GetTF(t_word,Sentence1);
		tf2 = GetTF(t_word,Sentence2);
		idf2 = GetIDF(t_word);
		int q = Sentence2.find(t_word)+t_word.size();
		if ((tf1 == 0) && ((Sentence2.find(t_word)+t_word.size()) == nPos1) )
			Numerator2 += (tf1*tf2*pow(idf2,2.0));
		Denominator2 += pow(tf2*idf2,2.0);

		nPos2 = nPos1+1;
	}

	Denominator1 = sqrt(Denominator1);
	Denominator2 = sqrt(Denominator2);

	dIdf = (Numerator1+Numerator2)/(Denominator1*Denominator2);

	return dIdf;
}

int  CAutoSumDoc::GetTF(std::string &strWord,std::string Sentence)
{
	int nPos1(0),nPos2(0),nTF(0);
	while((nPos1 = Sentence.find(strWord,nPos2)) >= 0)
	{
		++nTF;
		nPos2 = nPos1+strWord.size()+1;
	}

	return nTF;
}

double CAutoSumDoc::GetIDF(std::string &strWord)
{
	int nCount(0),nN = m_vectorsentences.size();
	if (nN == 0)
		return -1.0;
	std::vector<std::string>::iterator   iterSentence;
	std::string  t_word ;
	int nPos = strWord.find("/");
	t_word = strWord.substr(0,nPos);
	for(iterSentence = m_vectorsentences.begin(); iterSentence != m_vectorsentences.end(); ++iterSentence)
	{
		int p = (*iterSentence).find(t_word,0);
		if (p >= 0)
		{
			++nCount;
		}
	}

	double dbNn = (double)(nN/nCount);
	double dIDF = log10(dbNn);
	return dIDF;
}

double  **CAutoSumDoc::PowerMethod(double **pCosineMatrix,int nCount,double dError)
{
	

	double **pLexRank = new double*[nCount];//cosinematrix 句子之间相似度
	pLexRank[0] = new double[nCount*nCount];

	for(int i=1; i<nCount; i++)
	{
		pLexRank[i] = pLexRank[i-1]+nCount;
	}
	
//	Matrix   P(5,1),M(5,5);
//	Matrix M = Matrix(nCount, nCount);
//	Matrix P = Matrix(nCount,nCount);
//	Matrix Pt =  Matrix(nCount,nCount);

	for(int r=1; r <= nCount; r++)
	{
		TRACE("\n %d:",r);
		for(int c=1; c <= nCount; c++)
		{
		//	M(r,c) = pCosineMatrix[r-1][c-1];
	
		//	P(r,c) = 1.0/(double)nCount;
		}
		
	}
//	M = Inv(M);
//	Matrix P_diff;
	int t(0);
	double value(0.0),threshold(0.0);
	do{
		++t;
	//	Pt = M*P;
	//	P_diff = Pt - P;
	//	value = Det(P_diff);
	//	P = Pt;
	}while(value < dError);



	for(int r=1; r <= nCount; r++)
	{
		for(int c=1; c <= nCount; c++)
		{
		//	pLexRank[r-1][c-1] = Pt(r,c);
			TRACE("%f ",pLexRank[r-1][c-1]);
			
		}
		TRACE("\n");
	}


	return pLexRank;
}

real_2d_array  CAutoSumDoc::PowerMethod(const real_2d_array &CosineMatrix,int nCount,double dError)
{
	real_2d_array   P,M,Pt,Pdiff,E;
	char chLimit[32],chValue[32];
	memset(chLimit,0,sizeof(chLimit));
	sprintf(chLimit,"%.5f",dError);
	std::string   strLimit;
	
	double dbStart = 1.0/(double)nCount,d=0.85;
	P.setlength(nCount,1);
	Pt.setlength(nCount,1);
	Pdiff.setlength(nCount,1);
	M.setlength(nCount,nCount);
	E.setlength(nCount,nCount);
	M = CosineMatrix;
	ae_int_t i,j;
	for (i=0; i<nCount; i++)
	{
		TRACE("\n%d:",i);
		P[i][0] = dbStart;
		Pt[i][0] = 0;
		Pdiff[i][0] = 0;
		for(j=0; j<nCount; j++)
		{
			
			E[i][j] = (1.0-d)/(double)nCount;
			M[i][j] *= d;

			TRACE("%f ",M[i][j]);
			M[i][j] += E[i][j];
		}
		TRACE("\n");
	}
	TRACE("\nP=%s\n",P.tostring(5).c_str());
	TRACE("\nPt=%s\n",Pt.tostring(1).c_str());
	TRACE("\nE=%s\n",E.tostring(5).c_str());
	TRACE("\nM=%s\n",M.tostring(5).c_str());
	
	double value(0.0),threshold(0.0);

	ae_int_t m = nCount;
	ae_int_t n = 1;
	ae_int_t k = nCount;
	double alpha = 1.0;
	ae_int_t ia = 0;
	ae_int_t ja = 0;
	ae_int_t optypea = 1;
	ae_int_t ib = 0;
	ae_int_t jb = 0;
	ae_int_t optypeb = 0;
	double beta = 0.0;
	ae_int_t ic = 0;
	ae_int_t jc = 0;


	do{
		value = 0.0;
		 rmatrixgemm(m, n, k, alpha, M, ia, ja, optypea, P, ib, jb, optypeb, beta, Pt, ic, jc);
		 TRACE("\nPt=%s\n",Pt.tostring(5).c_str());
		 for (i=0; i<nCount; i++)
		 {
			 
			 value += fabs(Pt[i][0] - P[i][0]);
			
		
			 TRACE("value=%.5f\n",value);
		 }
	     memset(chValue,0,sizeof(chValue));
		 sprintf(chValue,"%.5f",value);
		 P = Pt;
		 TRACE("\nP=%s\n",P.tostring(5).c_str());
	}while(strcmp(chValue,chLimit) > 0);

	return Pt;

}


UINT    StarSummary(LPVOID pParam)
{
	if (!pParam)
		return 1;
	CAutoSumDoc *pDocument = (CAutoSumDoc*)pParam;

	pDocument->AutoSummarization();


	return 0;

}