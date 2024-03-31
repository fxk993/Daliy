#include<iostream>
using namespace std;
//领接矩阵数据类型
#define MAXVEX 20       
#define MAXINT 32767  
typedef ArcType int;
typedef VexType char;
typedef struct {
    ArcType arcs[MAXVEX][MAXVEX];//边的信息，如果是有向网则为弧
    VExType vex[MAXVEX];//顶点数组，存储顶点的信息
    int vexnum,arcnum;//顶点的个数，边的个数
}AdjMatrix;//领接矩阵
//领接矩阵创造函数
void Create(AdjMatrix *G)
{
    int i=0;
    char vex1,vex2;
    cout<<"请输入顶点数和边数："<<endl;
    cin>>G->vexnum>>G->arcnum;
    for(i=0;i<G->vexnum;i++)
        for(j=0;j<G->vexnum;j++)
        {
            G->arcs[i][j]=MAXINT;
        }
    
    for(i=0;i<G->vexnum;i++)
    {
        cin>>G->vex[i];
    }
    for(int k=0;k<G->arcnum;K++)
    {
        cout<<"请输入边的信息"<<endl;
        cin>>vex1>>vex2;
        i=LocateVex(G,vex1);
        j=LocateVex(G,vex2);
        G->arcs[i][j]=1;
        G->arcs[j][i]=1;//如果是有向网则删掉这段代码
    }
    int LocateVex(AdjMatrix *G,char vex)//寻找顶点在顶点数组存储的下标，用简单的顺序查找
    {
        for(int  i=0;i<G->vexnum;i++)
        {
            if(vex==G->vex[i])
            return i;
            return 0;
        }
    }
}


