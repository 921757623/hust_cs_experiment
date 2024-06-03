void visit(VertexType v)
{
    printf(" %d %s",v.key,v.others);
}

status searchPos(ALGraph G, KeyType VR[][2],int Key,int value)
{
    int position;
    if(Key == 0)
        for (position = 0 ; position < G.vexnum ; position++) {
                if(G.vertices[position].data.key == VR[value][0])
                    return position;
            }
    else if(Key == 1)
        for (position = 0 ; position < G.vexnum ; position++) {
                if(G.vertices[position].data.key == VR[value][1])
                    return position;
            }
    return -1;
}

status CreateCraph(ALGraph &G,VertexType V[],KeyType VR[][2])
/*根据V和VR构造图T并返回OK，如果V和VR不正确，返回ERROR
如果有相同的关键字，返回ERROR。此题允许通过增加其它函数辅助实现本关任务*/
{
    ArcNode *V1 ,*V2;
    int position = 0;
    G.vexnum = 0; G.arcnum = 0;
    G.kind = UDG;
    if(V[0].key == -1)
    return ERROR;
    while(V[position].key != -1) {      //所有根结点入G
        G.vertices[position].firstarc = NULL;
        G.vertices[position].data = V[position];
        G.vexnum++;
        position++;
    }
    if(position > MAX_VERTEX_NUM)
    return ERROR;
    position = 0;
    while(VR[position][0] != -1) {
        int NodePosLeft = searchPos(G,VR,0,position);
        int NodePosRight = searchPos(G,VR,1,position);
        if(NodePosLeft == -1 || NodePosRight == -1)
            return ERROR;
        V1 = (ArcNode *)malloc(sizeof(ArcNode));
        V1->adjvex = NodePosRight;
        V1->nextarc = G.vertices[NodePosLeft].firstarc;
        G.vertices[NodePosLeft].firstarc = V1;

        V2 = (ArcNode *)malloc(sizeof(ArcNode));
        V2->adjvex = NodePosLeft;
        V2->nextarc = G.vertices[NodePosRight].firstarc;
        G.vertices[NodePosRight].firstarc = V2;

        G.arcnum++;
        position++;
    }
    return OK;
}

status DestroyGraph(ALGraph &G)
/*销毁无向图G,删除G的全部顶点和边*/
{
    ArcNode *temp ,*p;
    for (int i = 0 ; i < G.vexnum ; i++) {
        p = G.vertices[i].firstarc;
        while(p != NULL) {
            temp = p;
            p = p->nextarc;
            free(temp);
        }
        G.vertices[i].firstarc = NULL;
    }
    G.arcnum = 0; G.vexnum = 0; 
    return OK;
}

int LocateVex(ALGraph G,KeyType u)
//根据u在图G中查找顶点，查找成功返回位序，否则返回-1；
{
    for(int i = 0 ; i < G.vexnum ; i++) {
        if(G.vertices[i].data.key == u)
            return i;
    }
    return -1;
}

status PutVex(ALGraph &G,KeyType u,VertexType value)
//根据u在图G中查找顶点，查找成功将该顶点值修改成value，返回OK；
//如果查找失败或关键字不唯一，返回ERROR
{
    int Key[1000] = {0}, key = -1;
    for(int i = 0 ; i < G.vexnum ; i++) {
        if(G.vertices[i].data.key == u) {
            key = i;
        }
        Key[G.vertices[i].data.key] = 1;
    }
    if(Key[value.key] == 0 && key != -1) {
        G.vertices[key].data = value;
        return OK;
    }
    return ERROR;
}

int FirstAdjVex(ALGraph G,KeyType u)
//根据u在图G中查找顶点，查找成功返回顶点u的第一邻接顶点位序，否则返回-1；
{
    for(int i = 0 ; i < G.vexnum ; i++) {
        if(G.vertices[i].data.key == u) {
            return G.vertices[i].firstarc->adjvex;
        }
    }
    return -1;
}

int NextAdjVex(ALGraph G,KeyType v,KeyType w)
//根据u在图G中查找顶点，查找成功返回顶点v的邻接顶点相对于w的下一邻接顶点的位序，查找失败返回-1；
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    ArcNode *p;
    for(int i = 0 ; i < G.vexnum ; i++) {
        if(G.vertices[i].data.key == v) {
            p = G.vertices[i].firstarc;
            for(int j = 0 ; p != NULL ; j++) {
                int pos = p->adjvex;
                if(G.vertices[pos].data.key == w && p->nextarc != NULL)
                    return p->nextarc->adjvex;
                p = p->nextarc;
            }
        }
    }
    return -1;
    /********** End **********/
}

status InsertVex(ALGraph &G,VertexType v)
//在图G中插入顶点v，成功返回OK,否则返回ERROR
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    int Key[10000] = {0};
    if(G.vexnum == 20)
    return ERROR;
    for(int i = 0 ; i < G.vexnum ; i++)
        Key[G.vertices[i].data.key] = 1;
    if(Key[v.key] == 0) {
        G.vertices[G.vexnum].data = v;
        G.vexnum++;
        return OK;
    }
    return ERROR;

    /********** End **********/
}

status DeleteVex(ALGraph &G,KeyType v)
//在图G中删除关键字v对应的顶点以及相关的弧，成功返回OK,否则返回ERROR
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    ArcNode *temp , *p;
    int pos = -1;
    for (int i = 0 ; i < G.vexnum ; i++)
        if(G.vertices[i].data.key == v) {   //先找出要删除的结点
            if(G.vexnum == 1)
                return ERROR;
            pos = i;                        //pos记录删除结点在数组中的位置
            p = G.vertices[i].firstarc;
            while(p != NULL) {             //清空该顶点
                temp = p;
                p = p->nextarc;
                free(temp);
                G.arcnum--;                //弧数对应减一
            }
            G.vertices[i].firstarc = NULL;      //删除完后初始化
            G.vertices[i].data.key = 0;
            memset(G.vertices[i].data.others,0,sizeof(G.vertices[i].data.others));
            for(int j = i ; j < G.vexnum - 1 ; j++)      //数组依次前移补空
                G.vertices[j] = G.vertices[j+1];
            G.vexnum--;                    //顶点数减一
            break;
        }
    for (int i = 0 ; i < G.vexnum ; i++) {      //剩余的顶点进行对应弧的删除以及位置的修改
        p = G.vertices[i].firstarc;
        while(p != NULL) {
            if(p->adjvex > pos)    //结点位置在删除位置的后面则位置信息减一
                p->adjvex--;
            else if(p->adjvex == pos) {   //如果相等则说明与删除顶点有关联，对应弧删除
                if(p == G.vertices[i].firstarc) {       //特殊处理头结点的情况
                    temp = G.vertices[i].firstarc->nextarc;
                    free(p);
                    G.vertices[i].firstarc = temp;
                    p = G.vertices[i].firstarc;
                }
                else {
                    temp->nextarc = p->nextarc;
                    free(p);
                    p = temp->nextarc;
                }
                continue;
            }
            temp = p;
            p = p->nextarc;
        }
    }
    if(pos != -1)
    return OK;
    else
    return ERROR;

    /********** End **********/
}

status InsertArc(ALGraph &G,KeyType v,KeyType w)
//在图G中增加弧<v,w>，成功返回OK,否则返回ERROR
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    ArcNode *V1,*V2,*p;
    int NodePosLeft = -1;
    int NodePosRight = -1;
    for(int i = 0 ; i < G.vexnum ; i++) 
        if(G.vertices[i].data.key == v) {
            NodePosLeft = i;
            break;
        }
    for(int i = 0 ; i < G.vexnum ; i++) 
        if(G.vertices[i].data.key == w) {
            NodePosRight = i;
            break;
        }
    if(NodePosLeft == -1 || NodePosRight == -1)
        return ERROR;
    p = G.vertices[NodePosLeft].firstarc;
    while(p != NULL) {
        if(p->adjvex == NodePosRight)
            return ERROR;
        p = p->nextarc;
    }
    V1 = (ArcNode *)malloc(sizeof(ArcNode));
    V1->adjvex = NodePosRight;
    V1->nextarc = G.vertices[NodePosLeft].firstarc;
    G.vertices[NodePosLeft].firstarc = V1;

    V2 = (ArcNode *)malloc(sizeof(ArcNode));
    V2->adjvex = NodePosLeft;
    V2->nextarc = G.vertices[NodePosRight].firstarc;
    G.vertices[NodePosRight].firstarc = V2;

    G.arcnum++;
    return OK;
    /********** End **********/
}

status DeleteArc(ALGraph &G,KeyType v,KeyType w)
//在图G中删除弧<v,w>，成功返回OK,否则返回ERROR
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    ArcNode *p,*temp;
    int sign = 0;
    int NodePosLeft = -1;
    int NodePosRight = -1;
    for(int i = 0 ; i < G.vexnum ; i++) 
        if(G.vertices[i].data.key == v) {
            NodePosLeft = i;
            break;
        }
    for(int i = 0 ; i < G.vexnum ; i++) 
        if(G.vertices[i].data.key == w) {
            NodePosRight = i;
            break;
        }
    if(NodePosLeft == -1 || NodePosRight == -1)
        return ERROR;
    p = G.vertices[NodePosLeft].firstarc;
    while(p != NULL) {
        if(p->adjvex == NodePosRight && p == G.vertices[NodePosLeft].firstarc) {
            temp = G.vertices[NodePosLeft].firstarc;
            p = p->nextarc;
            free(temp);
            G.vertices[NodePosLeft].firstarc = p;
            sign++;
            G.arcnum--;
            break;
        }
        else if(p->adjvex == NodePosRight) {
            temp->nextarc = p->nextarc;
            free(p);
            sign++;
            G.arcnum--;
            break;
        }
        temp = p;
        p = p->nextarc;
    }
    if(sign == 0) return ERROR;
    p = G.vertices[NodePosRight].firstarc;
    while(p != NULL) {
        if(p->adjvex == NodePosLeft && p == G.vertices[NodePosRight].firstarc) {
            temp = G.vertices[NodePosRight].firstarc;
            p = p->nextarc;
            free(temp);
            G.vertices[NodePosRight].firstarc = p;
            return OK;
        }
        else if(p->adjvex == NodePosLeft) {
            temp->nextarc = p->nextarc;
            free(p);
            return OK;
        }
        temp = p;
        p = p->nextarc;
    }
    return ERROR;
    /********** End **********/
}

status DFSTraverse(ALGraph &G,void (*visit)(VertexType))
//对图G进行深度优先搜索遍历，依次对图中的每一个顶点使用函数visit访问一次，且仅访问一次
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    VNode Vnode;
    ArcNode *p;
    AdjList stack;
    int sign[21] = {0};
    int top = 0 , now , record = 0;
    visit(G.vertices[0].data);
    stack[0] = G.vertices[0];
    sign[0] = 1;
    record++;
    while (top != -1 || record != G.vexnum) {
        if(top == -1) {
            int i;
            for (i = 0 ; sign[i] == 1 ; i++);
            visit(G.vertices[i].data);
            stack[++top] = G.vertices[i];
            sign[i] = 1;
            record++;
        }
        Vnode = stack[top];
        p = Vnode.firstarc;
        while (p != NULL) {
            while(p != NULL && sign[p->adjvex] == 1) {
                p = p->nextarc;
            }
            if(p == NULL)
                break;
            now = p->adjvex;
            stack[++top] = G.vertices[now];
            sign[now] = 1;
            visit(G.vertices[now].data);
            record++;
            p = G.vertices[now].firstarc;
        }
        --top;
    }
    return OK;
    /********** End **********/
}

status BFSTraverse(ALGraph &G,void (*visit)(VertexType))
//对图G进行广度优先搜索遍历，依次对图中的每一个顶点使用函数visit访问一次，且仅访问一次
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    VNode Vnode;
    ArcNode *p;
    AdjList stack;
    int sign[21] = {0};
    int top = 0 , now , record = 0;
    visit(G.vertices[0].data);
    stack[0] = G.vertices[0];
    sign[0] = 1;
    record++;
    while(top != -1 || record != G.vexnum) {
        if(top == -1) {
            int i;
            for (i = 0 ; sign[i] == 1 ; i++);
            visit(G.vertices[i].data);
            stack[++top] = G.vertices[i];
            sign[i] = 1;
            record++;
        }
        Vnode = stack[top];
        p = Vnode.firstarc;
        while(p != NULL) {
            while(p != NULL && sign[p->adjvex] == 1) {
                p = p->nextarc;
            }
            if(p == NULL)
            break;
            now = p->adjvex;
            visit(G.vertices[now].data);
            stack[++top] = G.vertices[now];
            sign[now] = 1;
            record++;
        }
        --top;
    }
    return OK;
    /********** End **********/
}

status SaveGraph(ALGraph G, char FileName[])
//将图的数据写入到文件FileName中
{
    // 请在这里补充代码，完成本关任务
    /********** Begin 1 *********/
    FILE *fp = NULL; ArcNode *p = NULL;
    int end = -1; char ends[4] = "nil";
    int visit[30][30] = {0};
    if((fp = fopen(FileName,"w")) != NULL) {
        for(int i=0;i<G.vexnum;i++) 
            fprintf(fp,"%d %s ",G.vertices[i].data.key,G.vertices[i].data.others);
        fprintf(fp,"%d %s ",end , ends); 
        for(int i = 0; i < G.vexnum ; i++ ){
             p = G.vertices[i].firstarc;
            while(p) {
                visit[i][p->adjvex] = 1;
                p = p->nextarc;
            }
        }
        for(int i = 0 ; i < G.vexnum ; i++ ){
            if(i == 1) continue;
            for(int j = G.vexnum-1 ; j > i ; j-- )
                if(visit[i][j] == 1) 
                    fprintf(fp,"%d %d ",G.vertices[i].data.key,G.vertices[j].data.key);
        }
        for(int j = G.vexnum - 1 ; j > 1 ; j-- )
            if(visit[1][j]==1) 
                fprintf(fp,"%d %d ",G.vertices[1].data.key,G.vertices[j].data.key);
        fprintf(fp,"%d %d\n",end , end);
    }
    fclose(fp);
    return OK;
    /********** End 1 **********/
}
status LoadGraph(ALGraph &G, char FileName[])
//读入文件FileName的图数据，创建图的邻接表
{
    // 请在这里补充代码，完成本关任务
    /********** Begin 2 *********/
    FILE *fp = NULL;
    VertexType V[21];
    KeyType VR[100][2];
    int i=0;
    if((fp = fopen(FileName,"r")) != NULL) {
        do{
            fscanf(fp,"%d%s",&V[i].key,V[i].others);
        }while(V[i++].key != -1);
        i=0;
        do{
            fscanf(fp,"%d%d",&VR[i][0],&VR[i][1]);
        }while(VR[i++][0] != -1);
        ArcNode *V1 ,*V2;
        int position = 0;
        G.vexnum = 0; G.arcnum = 0;
        G.kind = UDG;
        if(V[0].key == -1)
        return ERROR;
        while(V[position].key != -1) {      //所有根结点入G
            G.vertices[position].firstarc = NULL;
            G.vertices[position].data = V[position];
            G.vexnum++;
            position++;
        }
        if(position > MAX_VERTEX_NUM)
        return ERROR;
        position = 0;
        while(VR[position][0] != -1) {
            int NodePosLeft = searchPos(G,VR,0,position);
            int NodePosRight = searchPos(G,VR,1,position);
            if(NodePosLeft == -1 || NodePosRight == -1)
                return ERROR;
            V1 = (ArcNode *)malloc(sizeof(ArcNode));
            V1->adjvex = NodePosRight;
            V1->nextarc = G.vertices[NodePosLeft].firstarc;
            G.vertices[NodePosLeft].firstarc = V1;

            V2 = (ArcNode *)malloc(sizeof(ArcNode));
            V2->adjvex = NodePosLeft;
            V2->nextarc = G.vertices[NodePosRight].firstarc;
            G.vertices[NodePosRight].firstarc = V2;

            G.arcnum++;
            position++;
        }
    }
    return OK;
    /********** End 2 **********/
}

status AddGraph(LISTS &Lists,char ListName[])
// 只需要在Lists中增加一个名称为ListName的空线性表
{
    int i = 0;
    for(int j = 0 ; j < Lists.length ; j++)
        if(strcmp(Lists.elem[j].name,ListName) == 0)
            return ERROR;
    if(Lists.length < LIST_INIT_SIZE-1)
    {
        while(ListName[i] != '\0')
        {
            Lists.elem[Lists.length].name[i] = ListName[i];
            i++;
        }
        Lists.elem[Lists.length].name[i] = '\0';     //以此作为结束标志
    }
    else
    return OVERFLOW;
    // Lists.elem[Lists.length].G.vexnum = 0;
    // Lists.elem[Lists.length].G.arcnum = 0;
    // Lists.elem[Lists.length].G.kind = UDG;
    Lists.length++;
    return OK; 
}

status RemoveGraph(LISTS &Lists,char ListName[])
 // Lists中删除一个名称为ListName的线性表
{
    for(int i=0;i<Lists.length;i++){
        int p=0;
        while(ListName[p]!='\0' && ListName[p]==Lists.elem[i].name[p]) p++;
        if(Lists.elem[i].name[p]=='\0'){
            DestroyGraph(Lists.elem[i].G);
            for(int j=i+1;j<Lists.length;j++)
                Lists.elem[j-1]=Lists.elem[j];
            Lists.length=Lists.length-1;
            return OK;
        }
    }
    return ERROR;
}

int LocateGraph(LISTS Lists,char ListName[])
 // 在Lists中查找一个名称为ListName的线性表，成功返回逻辑序号，否则返回0
{
    for(int i=0;i<Lists.length;i++){
        int p=0; 
        while(ListName[p]!='\0' && Lists.elem[i].name[p]==ListName[p]) p++;
        if(Lists.elem[i].name[p]=='\0') return (i+1);
    }
    return ERROR;
}
status VerticesSerLessThank(ALGraph G,KeyType u,int k){
    int q[20],head=0,tail=0,vis[20],s[20],pv;
    for(int i=0;i<G.vexnum;i++) vis[i]=0;
    pv=LocateVex(G,u); 
    q[tail++]=pv; vis[pv]=1; s[pv]=0;
    while(head<tail){
        for(ArcNode* p=G.vertices[q[head]].firstarc;p;p=p->nextarc){
            if(vis[p->adjvex]==0){
                vis[p->adjvex]=1;
                s[p->adjvex]=s[q[head]]+1;
                q[tail++]=p->adjvex;
                if(s[p->adjvex]>k) return OK;
                visit(G.vertices[p->adjvex].data);
            }   
        }
        head++;
    } 
    return ERROR;
}
status ShorttestPathLength(ALGraph G,KeyType u,KeyType v){
    int q[20],head=0,tail=0,vis[20],s[20],pv,pv2;
    for(int i=0;i<G.vexnum;i++) vis[i]=0;
    pv=LocateVex(G,u); pv2=LocateVex(G,v);  
    q[tail++]=pv; vis[pv]=1; s[pv]=0;
    while(head<tail){
        for(ArcNode* p=G.vertices[q[head]].firstarc;p;p=p->nextarc){
            if(vis[p->adjvex]==0){
                vis[p->adjvex]=1;
                s[p->adjvex]=s[q[head]]+1;
                q[tail++]=p->adjvex;
                if(p->adjvex==pv2) return s[p->adjvex];
            }   
        }
        head++;
    } 
    return ERROR;
} 
status ConnectedComponentsNums(ALGraph G){
    int s[100],vis[100],top=0; int res=0;
    for(int i=0;i<G.vexnum;i++) vis[i]=0;
    for(int i=0;i<G.vexnum;i++){
        if(vis[i]==1) continue;
        s[top++]=i; vis[i]=1; res++;
        while(top){
            ArcNode* p=G.vertices[s[top-1]].firstarc;
            while(p){
                if(vis[p->adjvex]==0){
                    vis[p->adjvex]=1;
                    s[top++]=p->adjvex;
                    break;
                }
                p=p->nextarc;
            }
            if(p==NULL) top--;
        }
    }
    return res;
}

status Lists_Traverse(LISTS Lists){
    for(int i=0;i<Lists.length;i++){
        printf("%s\n",Lists.elem[i].name);
        printf("\n");
    }
}