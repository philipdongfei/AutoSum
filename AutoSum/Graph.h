#include "stdafx.h"
#include <iostream>
#include <Stack>
using namespace std;

template<class VertexType,class EdgeType> class Graph;
template<class VertexType,class EdgeType> class Vertex;

template<class VertexType,class EdgeType> class Edge {
	friend class Graph<VertexType,EdgeType>;
public:
	Edge(Vertex<VertexType,EdgeType>* d, EdgeType c, Edge<VertexType,EdgeType> *q):
	  dest(d),cost(c),link(q){ }
private:
	Vertex<VertexType,EdgeType> *dest;
	EdgeType cost;
	Edge<VertexType,EdgeType> *link;
};

template<class VertexType,class EdgeType> class Vertex {
	friend class Edge<VertexType,EdgeType>;
	friend class Graph<VertexType,EdgeType>;
public:
	Vertex(VertexType d,Vertex<VertexType,EdgeType> *p,Edge<VertexType,EdgeType> *q):
	  data(d),next(p),adj(q),ve(0),vl(0),tag(0) {}
private:
	int tag;
	VertexType data;
	EdgeType ve;
	EdgeType vl;
	Vertex<VertexType,EdgeType> *next;
	Edge<VertexType,EdgeType> *adj;
};

template<class VertexType,class EdgeType> class Graph {
private:
	Vertex<VertexType,EdgeType> *head;
	int NumVertex;
	int NumEdge;
	const VertexType Vertex_finish_flag;
	const EdgeType Edge_finish_flag;
public:
	Graph(VertexType flag,EdgeType tag);
	Graph(double **pMatrix,int &nNumVertex);//
	~Graph();
	int GraphEmpty() const { return NumVertex == 0;}
	int NumofVertex() const {return NumVertex;}
	int NumofEdge() const { return NumEdge;}
	VertexType Getvalue(Vertex<VertexType,EdgeType> *p)
	{return p?p->data:0;}
	void InsertVertex(const VertexType &vertex);
	void InsertEdge(Vertex<VertexType,EdgeType> *vl,
		Vertex<VertexType,EdgeType> *v2,EdgeType weight);
	void RemoveVertex(const VertexType &vertex);
	void RemoveEdge(Vertex<VertexType,EdgeType> *v1,
		Vertex<VertexType,EdgeType> *v2);
	EdgeType GetWeight(Vertex<VertexType,EdgeType> *v1,
		Vertex<VertexType,EdgeType> *v2);
	Vertex<VertexType,EdgeType> *GetFirstNeighbor(const Vertex<VertexType,EdgeType> *v);
	Vertex<VertexType,EdgeType> *GetNextNeighbor(Vertex<VertexType,EdgeType> *v1,Vertex<VertexType,EdgeType> *v2);
	Vertex<VertexType,EdgeType> *GetVertexPos(const VertexType &vertex);
	int DFS_Traveling(const VertexType &start,char *pBuf);
};

template <class VertexType,class EdgeType>
Graph<VertexType,EdgeType>::Graph(VertexType flag,EdgeType tag):
Vertex_finish_flag(flag),Edge_finish_flag(tag){
	NumVertex = 0;
	NumEdge = 0;

	VertexType v,u;
	EdgeType e;
	Vertex<VertexType,EdgeType> *p,*q;
	cout <<"Input vertex data one by one!" << endl;
	head = NULL;
	while (cin >>v,v != Vertex_finish_flag) {
		head = new Vertex<VertexType,EdgeType>(v,head,NULL);
		if (!head)
		{
			cerr<<"Head is NULL!"<<endl;
			break;
		}
//		Exception(!head,"Head is NULL!");
		NumVertex++;
	}

	cout <<"Input edges data one by one!" << endl;
	while (cin >> v,cin >> u,cin>>e,e!= Edge_finish_flag) {
//		Exception(!(p = GetVertexPos(v)),"It is NULL!");
		cout <<"e="<< e << " Edge_finish_flag="<<Edge_finish_flag << endl;
		if (!(p = GetVertexPos(v)))
		{
			cerr<<"It is NULL!"<<endl;
			break;
		}
//		Exception(!(q=GetVertexPos(u)),"It is NULL!");

		if (!(q=GetVertexPos(u)))
		{
			cerr <<"It is NULL!"<<endl;
			break;
		}
		p->adj = new Edge<VertexType,EdgeType>(q,e,p->adj);
//		Exception(!p->adj,"Fail on memory!");
		if (!p->adj)
		{
			cerr <<"Fail on memory!"<<endl;
			break;
		}
		NumEdge++;
	}
	cout <<"Input edges data end!" << endl;
}

template <class VertexType,class EdgeType>
Graph<VertexType,EdgeType>::Graph(double **pMatrix,int &nNumVertex):
Vertex_finish_flag(0),Edge_finish_flag(0){
	NumVertex = nNumVertex;
	NumEdge = 0;

	VertexType v,u;
	EdgeType e;
	Vertex<VertexType,EdgeType> *p,*q;
	cout <<"Input vertex data one by one!" << endl;
	head = NULL;
	int i,j;
//	while (cin >>v,v != Vertex_finish_flag) {
	for(i=0; i<nNumVertex;i++) 
	{
		v = i;
		head = new Vertex<VertexType,EdgeType>(v,head,NULL);
		if (!head)
		{
			cerr<<"Head is NULL!"<<endl;
			break;
		}
//		Exception(!head,"Head is NULL!");
//		NumVertex++;
	}

	cout <<"Input edges data one by one!" << endl;
//	while (cin >> v,cin >> u,cin>>e,e!= Edge_finish_flag) 
	for(i=0; i<nNumVertex; i++)
	{
		v = i;
		for (j=0; j<nNumVertex; j++)
		{
			u = j;
			e = pMatrix[i][j];
			if ( e == 0 || i==j)
				continue;
			//		Exception(!(p = GetVertexPos(v)),"It is NULL!");
			//		cout <<"e="<< e << " Edge_finish_flag="<<Edge_finish_flag << endl;
			if (!(p = GetVertexPos(v)))
			{
				cerr<<"It is NULL!"<<endl;
				break;
			}
			//		Exception(!(q=GetVertexPos(u)),"It is NULL!");

			if (!(q=GetVertexPos(u)))
			{
				cerr <<"It is NULL!"<<endl;
				break;
			}
			p->adj = new Edge<VertexType,EdgeType>(q,e,p->adj);
			//		Exception(!p->adj,"Fail on memory!");
			if (!p->adj)
			{
				cerr <<"Fail on memory!"<<endl;
				break;
			}
			NumEdge++;
		}
	}
	cout <<"Input edges data end!" << endl;

}


template <class VertexType,class EdgeType>
Graph<VertexType,EdgeType>::~Graph() {
	Vertex<VertexType,EdgeType> *p,*m;
	Edge<VertexType,EdgeType> *q;
	p = head;
	while (p) {
		while (q=p->adj) { p->adj = q->link; delete q;}
		m = p; p = p->next; delete m;
	}
}

template <class VertexType, class EdgeType>
Vertex <VertexType,EdgeType> *Graph<VertexType,EdgeType>::GetVertexPos(const VertexType &vertex) {
	Vertex<VertexType,EdgeType> *p;
	p = head;
	while(p) {
		if (p->data == vertex)
			return p; 
		p = p->next;
	}
	return NULL;
}

template <class VertexType, class EdgeType>
Vertex<VertexType,EdgeType> *Graph<VertexType,EdgeType>::
	GetFirstNeighbor(const Vertex<VertexType,EdgeType> *v) {
		if (v) {
			Edge<VertexType,EdgeType *q = v->adj;
			if (q)
				return q->dest;
		}
		return NULL;
}

 template <class VertexType,class EdgeType>
 Vertex<VertexType,EdgeType> *Graph<VertexType,EdgeType>::GetNextNeighbor(
	 Vertex<VertexType,EdgeType> *v1,Vertex<VertexType,EdgeType> *v2) {
		 if (v1 && v2) {
			 p = v1->adj;
			 if (!p)
				 return NULL;
		 }
		 else
			 return NULL;
		 while(p) {
			 if (p->dest == v2 && p->link != NULL)
				 return p->link->dest;
			 p = p->link;
		 }
		 return NULL;
 }

 template <class VertexType,class EdgeType>
 EdgeType Graph<VertexType,EdgeType>::GetWeight(
	 Vertex<VertexType,EdgeType> *v1,Vertex<VertexType,EdgeType> *v2) {
	 Edge<VertexType,EdgeType> *p;
	 if(v1 && v2) {
		 p = v1->adj; 
		 if (!p)
			 return 0;
	 }
	 else
		 return 0;
	 while (p) {
		 if (p->dest == v2)
			 return p->cost;
		 p = p->link;
	 }
	 return 0;
 }

 template <class VertexType,class EdgeType>
 int Graph<VertexType,EdgeType>::DFS_Traveling(const VertexType & start,char *pBuf) {
	 stack<Edge<VertexType,EdgeType>*> s;//(NumEdge);
	 int nSubGraphs(0),nVerNum(0);
	 char temp[16];
	 memset(temp,0,sizeof(temp));
	 Vertex<VertexType,EdgeType> *p,*m;
	 Edge<VertexType,EdgeType> *q; 
	 p = head;
	 while(p)
	 { 
		 p->tag = 0; 
		 p = p->next;
	 }
	 p = GetVertexPos(start);//
	 if (!p) 
	 {
		 cout <<"Start vertex is Error!" << endl; 
		 return nSubGraphs;
	 }
	 int finished = 0;
	 while(p) {
		 if (p->tag == 0) {
			 nVerNum = 0;
			 p->tag = 1;
			 cout <<"visisted vertex data:" <<p->data <<endl;
			 TRACE("!!!!visisted vertex data:%d\n",p->data);
			// if (nSubGraphs > 0)
			//	  strcat(pBuf,"|");
			 memset(temp,0,sizeof(temp));
			 sprintf(temp,"%d",p->data);
			 strcat(pBuf,temp);
			 strcat(pBuf,",");
			 ++nSubGraphs;
			 ++nVerNum;
			 if (p->adj)
				 s.push(p->adj);
			 while(!(s.size()==0)) {
				 q = s.top(); s.pop();
				 if (q->link) s.push(q->link);
				 m = q->dest;
				 if (m->tag == 0) {
					 m->tag = 1;
					 cout <<"visited vertex data:"<<m->data<<endl;
					 TRACE("visited vertex data:%d\n",m->data);
					 memset(temp,0,sizeof(temp));
					 sprintf(temp,"%d",m->data);
					 strcat(pBuf,temp);
					 strcat(pBuf,",");
					 ++nVerNum;
					 if(m->adj)
						 s.push(m->adj);
				 }
			 }
			 memset(temp,0,sizeof(temp));
			 sprintf(temp,"%d",nVerNum);
			 strcat(pBuf,temp);
			 strcat(pBuf,"|");
		 }
		
		 p = p->next;
		 if (!p && !finished)
		 {
			 
			 p = head;
			 finished = 1;

		 }
	 }
//	 strcat(pBuf,"|");
	 return nSubGraphs;
 }

