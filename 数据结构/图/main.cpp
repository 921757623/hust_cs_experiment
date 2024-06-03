#include "def.h"
#include "f.h"
int main(){
    int op=1,e,u,i; VertexType v;
    char FileName[20],ListName[20];
    VertexType V[21];
    KeyType VR[100][2];
    ALGraph G1; ALGraph* G=&G1;
    LISTS Lists;  Lists.length=0;
    while(op){
        system("cls"); printf("\n\n");
        printf("         Menu for Linear Table On Sequence Structure \n"      );
        printf("---------------------------------------------------------------\n");
        printf("    1. CreateCraph              2. DestroyGraph\n"            );
        printf("    3. LocateVex                4. PutVex\n"                  );
        printf("    5. FirstAdjVex              6. NextAdjVex\n"              );
        printf("    7. InsertVex                8. DeleteVex\n"               );
        printf("    9. InsertArc                10.DeleteArc\n"               );
        printf("    11.DFSTraverse              12.BFSTraverse\n"             );
        printf("    13.SaveGraph                14.LoadGraph\n"               );    
        printf("    15.AddGraph                 16.RemoveGraph\n"             );
        printf("    17.LocateGraph              18. VerticesSerLessThank\n"   );
        printf("    19. ShorttestPathLength     20. ConnectedComponentsNums\n");   
        printf("    21. Lists_Traverse          0. exit\n"                    );
        printf("---------------------------------------------------------------\n"); 
        printf("    请选择你的操作[0~21]: "				                        );

        scanf("%d",&op);
        switch(op){
            case 1: 
                i=0;
                printf("请输入数据:\n");
                do{
                    scanf("%d%s",&V[i].key,V[i].others);
                }while(V[i++].key!=-1);
                i=0;
                do{
                    scanf("%d%d",&VR[i][0],&VR[i][1]);
                }while(VR[i++][0]!=-1);
                i=CreateCraph(*G,V,VR);
                if(i==OK) printf("创建无向图成功！\n");
                else printf("无向图创建失败！\n");
                getchar(); getchar();
                break ;
            case 2:
                i=DestroyGraph(*G); 
                if(i==OK) printf("无向图销毁成功！\n");
                else printf("无向图销毁失败！\n");
                getchar(); getchar(); 
                break;
            case 3:
                printf("请输入查找的顶点的关键字：\n"); 
                scanf("%d",&e);
                i=LocateVex(*G,e);
                if(i==-1) printf("查找失败！\n");
                else printf("顶点的位序为：%d %s",G->vertices[i].data.key,G->vertices[i].data.others);
                getchar(); getchar();
                break; 
            case 4:
                printf("请输入需要赋值的顶点及所赋的值\n");
                scanf("%d%d%s",&u,&v.key,v.others);
                i=PutVex(*G,u,v);
                if(i==OK) printf("赋值成功！\n");
                else printf("复制失败！\n");
                getchar(); getchar();
                break;
            case 5:
                printf("请输入查找第一个邻接点的顶点的关键字\n");
                scanf("%d",&e);
                i=FirstAdjVex(*G,e);
                if(i==-1) printf("获取失败！\n");
                else printf("顶点的第一个邻接点为%d %s\n",G->vertices[i].data.key,G->vertices[i].data.others);
                getchar(); getchar();
                break;
            case 6:
                printf("请输入需要查找的顶点及其邻接点\n"); 
                scanf("%d%d",&u,&e);
                i=NextAdjVex(*G,u,e);
                if(i==-1) printf("查找失败！\n");
                else printf("下一个邻接点为%d %s\n",G->vertices[i].data.key,G->vertices[i].data.others);
                getchar(); getchar();
                break;
            case 7:
                printf("请输入需要插入的顶点\n");
                scanf("%d%s",&v.key,v.others);
                i=InsertVex(*G,v);
                if(i==OK) printf("插入成功！\n");
                else printf("插入失败！\n");
                getchar(); getchar();
                break;
            case 8:
                printf("请输入需要删除的顶点的关键字\n");
                scanf("%d",&e);
                i=DeleteVex(*G,e);
                if(i==OK) printf("删除成功！\n");
                else printf("删除失败！\n");
                getchar(); getchar();
                break;
            case 9:
                printf("请输入需要插入的弧的两个顶点\n");
                scanf("%d%d",&u,&e);
                i=InsertArc(*G,u,e);
                if(i==OK) printf("插入成功\n");
                else printf("插入失败\n");
                getchar(); getchar();
                break;
            case 10: 
                printf("请输入需要删除的弧\n");
                scanf("%d%d",&u,&e);
                i=DeleteArc(*G,u,e);
                if(i==OK) printf("删除成功！\n");
                else printf("删除失败！\n");
                getchar(); getchar();
                break;
            case 11:
                DFSTraverse(*G,visit);
                getchar(); getchar();
                break;
            case 12: 
                BFSTraverse(*G,visit);
                getchar(); getchar();
                break;
            case 13:
                printf("请输入文件地址\n");
                scanf("%s",FileName);
                i=SaveGraph(*G,FileName);
                if(i==OK) printf("保存成功！\n");
                else printf("保存失败！\n"); 
                getchar(); getchar();
                break;
            case 14: 
                printf("请输入文件地址\n");
                scanf("%s",FileName);
                i=LoadGraph(*G,FileName); 
                if(i==OK) printf("写入成功！\n");
                else printf("写入失败！\n");
                getchar(); getchar();
                break;
            case 15:
                printf("请输入需要添加的无向图的名称\n");
                scanf("%s",ListName);
                i=AddGraph(Lists,ListName);
                if(i==OK) printf("添加成功！\n");
                else printf("添加失败！\n");
                getchar(); getchar();
                break;
            case 16:
                printf("请输入需要删除的无向图的名称\n");
                scanf("%s",ListName);
                i=RemoveGraph(Lists,ListName);
                if(i==OK) printf("删除成功！\n");
                else printf("删除失败！\n");
                getchar(); getchar();
                break;
            case 17:
                printf("请输入需要查找的无向图的名称\n");
                scanf("%s",ListName);
                i=LocateGraph(Lists,ListName);
                if(i==0) printf("查找失败");
                else printf("无向图%s的位序为%d\n",ListName,i);
                G=&Lists.elem[i].G;
                getchar(); getchar();
                break;
            case 18:
                printf("请输入顶点与距离\n");
                scanf("%d%d",&u,&e); 
                i=VerticesSerLessThank(*G,u,e);
                getchar(); getchar();
                break;
            case 19:
                printf("请输入两个顶点的关键字\n");
                scanf("%d%d",&u,&e);
                i=ShorttestPathLength(*G,u,e);
                if(i==ERROR) printf("顶点%d与顶点%d不连通\n",u,e);
                else printf("最短路径为%d\n",i);
                getchar(); getchar();
                break;
            case 20:
                i=ConnectedComponentsNums(*G);
                printf("连通分量为%d\n",i); 
                getchar(); getchar(); 
                break;
            case 21:
                Lists_Traverse(Lists);
                getchar(); getchar();
                break;
            default:
                break;
        }
    }
}