status CreateBiTree(BiTree &T, TElemType definition[])
/*根据带空枝的二叉树先根遍历序列definition构造一棵二叉树，将根节点指针赋值给T并返回OK，
如果有相同的关键字，返回ERROR。此题允许通过增加其它函数辅助实现本关任务*/
{
    struct BiTNode *Stack[50], *p;       // 栈保存根节点
    int Key[2000] = {0}, sign[50] = {0}; // Key数组用来确保key有且仅出现一次，sign用于回溯时标记判断是否已经创建左子树
    int top = 0, pos = 0;                // pos表示当前读取数据的位置
    T = (BiTree)malloc(sizeof(BiTNode));
    sign[top] = 1;
    Stack[top++] = T;
    if (definition[pos].key != 0)
    { // 处理一开始根节点就为空的情况
        Key[definition[pos].key] = 1;
        T->data = definition[pos++];
    }
    else // 为空
    {
        T = NULL;
        return OK;
    }
    p = T; // p用于保存最初的根节点
    while (definition[pos].key != -1)
    { // 循环直到输入结束
        if (sign[top] == 0)
        { // 没有创建左子树的情况
            while (definition[pos].key != 0)
            {
                T->lchild = (BiTree)malloc(sizeof(BiTNode)); // 创建左子树
                T = T->lchild;
                if (Key[definition[pos].key] == 0)
                {                                 // 没有重复key的情况下
                    Key[definition[pos].key] = 1; // 标记已经使用过该key值
                    T->data = definition[pos++];  // 赋值
                    sign[top] = 1;                // 标记该节点已经创建左子树
                    Stack[top++] = T;             // 入栈
                }
                else
                    return ERROR; // key值重复返回报错
            }
            T->lchild = NULL; // 跳出while循环说明左子树出现null
            sign[top] = 0;    // 退栈的同时把sign对应位置的标记初始化
            T = Stack[--top]; // 退栈，回溯
            pos++;
        }
        if (definition[pos].key != 0)
        { // 左边处理完则开始创建右子树
            T->rchild = (BiTree)malloc(sizeof(BiTNode));
            T = T->rchild;
            if (Key[definition[pos].key] == 0)
            {
                Key[definition[pos].key] = 1;
                T->data = definition[pos++];
                Stack[top++] = T;
            }
            else
                return ERROR;
        }
        else
        {
            T->rchild = NULL;
            sign[top] = 0;
            T = Stack[--top];
            pos++;
        }
    }      // 右子树同理左子树的创建
    T = p; // 最后返回初始根节点，树创建完毕
    return OK;
}

status ClearBiTree(BiTree &T)
// 将二叉树设置成空，并删除所有结点，释放结点空间
{
    struct BiTNode *St[50], *pre;
    int flag, top = 0;
    if (T != NULL)
    {
        do
        {
            while (T != NULL)
            {
                St[top++] = T;
                T = T->lchild;
            }
            pre = NULL;
            flag = 1;
            while (top && flag)
            {
                T = St[top - 1];
                if (T->rchild == pre)
                {
                    pre = T;
                    free(T);
                    top--;
                }
                else
                {
                    T = T->rchild;
                    flag = 0;
                }
            }
        } while (top);
    }
    else
        return ERROR;

    T = NULL;
    return OK;
}

int BiTreeDepth(BiTree T)
// 求二叉树T的深度
{
    // 请在这里补充代码，完成本关任务
    /********** Begin *********/
    struct BiTNode *St[50], *pre;
    int flag, top = 0, max = 0;
    if (T != NULL)
    {
        do
        {
            while (T != NULL)
            {
                St[top++] = T;
                T = T->lchild;
            }
            if (top > max)
                max = top;
            pre = NULL;
            flag = 1;
            while (top && flag)
            {
                T = St[top - 1];
                if (T->rchild == pre)
                {
                    pre = T;
                    top--;
                }
                else
                {
                    T = T->rchild;
                    flag = 0;
                }
            }
        } while (top);
    }
    return max;
    /********** End **********/
}

BiTNode *LocateNode(BiTree T, KeyType e)
// 查找结点
{
    struct BiTNode *St[50], *pre, *aim;
    int flag, top = 0;
    if (T != NULL)
    {
        do
        {
            while (T != NULL)
            {
                St[top++] = T;
                T = T->lchild;
            }
            pre = NULL;
            flag = 1;
            while (top && flag)
            {
                T = St[top - 1];
                if (T->rchild == pre)
                {
                    if (T->data.key == e)
                        return T;
                    pre = T;
                    top--;
                }
                else
                {
                    T = T->rchild;
                    flag = 0;
                }
            }
        } while (top);
    }
    return NULL;
}

status Assign(BiTree &T, KeyType e, TElemType value)
// 实现结点赋值。此题允许通过增加其它函数辅助实现本关任务
{
    struct BiTNode *St[50], *pre, *aim, *phead, *record = NULL;
    int flag, top = 0, Key[1000] = {0};
    phead = T;
    if (T != NULL)
    {
        do
        {
            while (T != NULL)
            {
                St[top++] = T;
                T = T->lchild;
            }
            pre = NULL;
            flag = 1;
            while (top && flag)
            {
                T = St[top - 1];
                if (T->rchild == pre)
                {
                    if (T->data.key == e)
                        record = T;
                    Key[T->data.key] = 1;
                    pre = T;
                    top--;
                }
                else
                {
                    T = T->rchild;
                    flag = 0;
                }
            }
        } while (top);
        if (record != NULL && (Key[value.key] == 0 || value.key == record->data.key))
        {
            record->data = value;
            T = phead;
            return OK;
        }
    }
    return ERROR;
}

BiTNode *GetSibling(BiTree T, KeyType e)
// 实现获得兄弟结点
{
    struct BiTNode *St[50], *pre;
    int flag, top = 0;
    if (T != NULL)
    {
        do
        {
            while (T != NULL)
            {
                St[top++] = T;
                T = T->lchild;
            }
            pre = NULL;
            flag = 1;
            while (top && flag)
            {
                T = St[top - 1];
                if (T->rchild == pre)
                {
                    if (T->data.key == e)
                    {
                        T = St[top - 2];
                        if (T->lchild == NULL || T->rchild == NULL)
                            return NULL;
                        if (T->lchild->data.key == e)
                            return T->rchild;
                        if (T->rchild->data.key == e)
                            return T->lchild;
                    }
                    pre = T;
                    top--;
                }
                else
                {
                    T = T->rchild;
                    flag = 0;
                }
            }
        } while (top);
    }
    return NULL;
}

status InsertNode(BiTree &T, KeyType e, int LR, TElemType c)
// 插入结点。此题允许通过增加其它函数辅助实现本关任务
{
    struct BiTNode *St[50], *pre, *phead, *temp, *record = NULL;
    int flag, top = 0, Key[1000] = {0};
    phead = T;
    struct BiTNode *insert = (BiTree)malloc(sizeof(BiTNode));
    insert->data = c;
    insert->lchild = NULL;
    if (LR == -1)
    {
        insert->rchild = T;
        T = insert;
        return OK;
    }
    if (T != NULL)
    {
        do
        {
            while (T != NULL)
            {
                St[top++] = T;
                T = T->lchild;
            }
            pre = NULL;
            flag = 1;
            while (top && flag)
            {
                T = St[top - 1];
                if (T->rchild == pre)
                {
                    if (T->data.key == e)
                        record = T;
                    Key[T->data.key] = 1;
                    pre = T;
                    top--;
                }
                else
                {
                    T = T->rchild;
                    flag = 0;
                }
            }
        } while (top);
        if (record != NULL && Key[c.key] == 0)
        {
            if (LR == 0)
            {
                temp = record->lchild;
                record->lchild = insert;
                insert->rchild = temp;
            }
            else if (LR == 1)
            {
                temp = record->rchild;
                record->rchild = insert;
                insert->rchild = temp;
            }
            T = phead;
            return OK;
        }
        return ERROR;
    }
}

status DeleteNode(BiTree &T, KeyType e)
// 删除结点。此题允许通过增加其它函数辅助实现本关任务
{
    struct BiTNode *St[50], *pre, *phead = T, *temp, *p;
    int flag, top = 0;
    if (T != NULL)
    {
        do
        {
            while (T != NULL)
            {
                St[top++] = T;
                T = T->lchild;
            }
            pre = NULL;
            flag = 1;
            while (top && flag)
            {
                T = St[top - 1];
                if (T->rchild == pre)
                {
                    if (T->data.key == e)
                    {
                        if (T != phead)
                            if (T->lchild == NULL && T->rchild == NULL)
                            {
                                temp = St[top - 2];
                                free(T);
                                if (temp->lchild == T)
                                    temp->lchild = NULL;
                                else
                                    temp->rchild = NULL;
                                T = phead;
                                return OK;
                            }
                            else if (T->lchild == NULL || T->rchild == NULL)
                            {
                                temp = St[top - 2];
                                if (temp->lchild == T)
                                {
                                    if (T->lchild == NULL)
                                        temp->lchild = T->rchild;
                                    else
                                        temp->lchild = T->lchild;
                                }
                                else if (temp->rchild == T)
                                {
                                    if (T->lchild == NULL)
                                        temp->rchild = T->rchild;
                                    else
                                        temp->rchild = T->lchild;
                                }
                                free(T);
                                T = phead;
                                return OK;
                            }
                            else
                            {
                                temp = St[top - 2];
                                p = T;
                                if (temp->lchild == T)
                                    temp->lchild = T->lchild;
                                else if (temp->rchild == T)
                                    temp->rchild = T->lchild;
                                T = T->lchild;
                                while (T->rchild != NULL)
                                {
                                    T = T->rchild;
                                }
                                T->rchild = p->rchild;
                                free(p);
                                T = phead;
                                return OK;
                            }
                        else
                        {
                            if (T->lchild == NULL && T->rchild == NULL)
                            {
                                free(T);
                                T = NULL;
                                return OK;
                            }
                            else if (T->lchild == NULL || T->rchild == NULL)
                            {
                                temp = T;
                                if (T->lchild == NULL)
                                    T = T->rchild;
                                else
                                    T = T->lchild;
                                free(temp);
                                return OK;
                            }
                            else
                            {
                                temp = T;
                                T = T->lchild;
                                while (T->rchild != NULL)
                                {
                                    T = T->rchild;
                                }
                                T->rchild = temp->rchild;
                                T = temp->lchild;
                                free(temp);
                                return OK;
                            }
                        }
                    }
                    pre = T;
                    top--;
                }
                else
                {
                    T = T->rchild;
                    flag = 0;
                }
            }
        } while (top);
    }
    return ERROR;
}

void visit(BiTree T)
{
    printf(" %d,%s", T->data.key, T->data.others);
}

status PreOrderTraverse(BiTree T, void (*visit)(BiTree))
// 先序遍历二叉树T
{
    struct BiTNode *St[50], *p;
    int top = 0; // 置空栈
    if (T != NULL)
    {
        St[top++] = T;
        while (top)
        {
            p = St[--top];
            visit(p);
            if (p->rchild != NULL)
                St[top++] = p->rchild;
            if (p->lchild != NULL)
                St[top++] = p->lchild;
        }
        return OK;
    }
    return ERROR;
}

status InOrderTraverse(BiTree T, void (*visit)(BiTree))
// 中序遍历二叉树T
{
    struct BiTNode *st[50]; // 定义指针栈
    int top = 0;            // 置空栈
    if (T != NULL)
    {
        do
        {
            while (T)
            {                  // 根指针t表示的为非空二叉树
                st[top++] = T; // 根指针进栈
                T = T->lchild; // t移向左子树
            }                  // 循环结束表示以栈顶元素的指向为
              // 根结点的二叉树的左子树遍历结束
            if (top)
            {                  // 为非空栈
                T = st[--top]; // 弹出根指针
                visit(T);      // 访问根结点
                T = T->rchild; // 遍历右子树
            }
        } while (top || T);
        return OK;
    }
    return ERROR;
}

status PostOrderTraverse(BiTree T, void (*visit)(BiTree))
// 后序遍历二叉树T
{
    struct BiTNode *St[50], *pre;
    int flag, top = 0;
    if (T != NULL)
    {
        do
        {
            while (T != NULL)
            {
                St[top++] = T;
                T = T->lchild;
            }
            pre = NULL;
            flag = 1;
            while (top && flag)
            {
                T = St[top - 1];
                if (T->rchild == pre)
                {
                    visit(T);
                    top--;
                    pre = T;
                }
                else
                {
                    T = T->rchild;
                    flag = 0;
                }
            }
        } while (top);
        return OK;
    }
    return ERROR;
}

status LevelOrderTraverse(BiTree T, void (*visit)(BiTree))
// 按层遍历二叉树T
{
    struct BiTNode *St[50] = {NULL};
    St[0] = T;
    int top = 1, current = 0;
    if (T != NULL)
    {
        do
        {
            visit(St[current]);
            if (St[current]->lchild != NULL)
                St[top++] = St[current]->lchild;
            if (St[current]->rchild != NULL)
                St[top++] = St[current]->rchild;
            current++;
        } while (St[current] != NULL);
        return OK;
    }
    return ERROR;
}

status SaveBiTree(BiTree T, char FileName[])
// 将二叉树的结点数据写入到文件FileName中
{
    char str[5] = "null";
    char zero = '0';
    char stop[3] = "-1";
    if (T == NULL)
        return OK;
    FILE *fp = NULL;
    if ((fp = fopen(FileName, "w")) != NULL)
    {
        struct BiTNode *St[100], *p;
        int top = 0; // 置空栈
        if (T != NULL)
        {
            St[top++] = T;
            while (top)
            {
                do
                {
                    p = St[--top];
                    if (p == NULL)
                        fprintf(fp, "%c %s ", zero, str);
                } while (p == NULL && top != 0);
                if (p != NULL)
                {
                    fprintf(fp, "%d %s ", p->data.key, p->data.others);
                    if (p->rchild != NULL)
                        St[top++] = p->rchild;
                    else
                        St[top++] = NULL;
                    if (p->lchild != NULL)
                        St[top++] = p->lchild;
                    else
                        St[top++] = NULL;
                }
            }
        }
    }
    else
        return ERROR;
    fprintf(fp, "%s %s ", stop, str);
    fclose(fp);
    return OK;
}

status LoadBiTree(BiTree &T, char FileName[])
// 读入文件FileName的结点数据，创建二叉树
{
    FILE *fp = NULL;
    TElemType definition[50];
    int i = 0;
    if ((fp = fopen(FileName, "r")) != NULL)
    {
        while (fscanf(fp, "%d %s", &definition[i].key, &definition[i].others) != EOF)
        {
            i++;
        }
        struct BiTNode *Stack[50], *p;
        int sign[50] = {0}; // 该数组用来标记已经建立左子树的结点
        int top = 0, pos = 0;
        T = (BiTree)malloc(sizeof(BiTNode));
        sign[top] = 1;
        Stack[top++] = T;
        if (definition[pos].key != 0)
            T->data = definition[pos++];
        else
            T = NULL; // 此处处理空树的情况
        p = T;        // 保存根节点
        while (definition[pos].key != -1)
        {
            if (sign[top] == 0)
            { // 没建立左子树
                while (definition[pos].key != 0)
                {
                    T->lchild = (BiTree)malloc(sizeof(BiTNode));
                    T = T->lchild;
                    T->data = definition[pos++];
                    sign[top] = 1;
                    Stack[top++] = T;
                }
                T->lchild = NULL;
                sign[top] = 0; // 退栈回溯
                T = Stack[--top];
                pos++;
            }
            else if (definition[pos].key != 0)
            {
                T->rchild = (BiTree)malloc(sizeof(BiTNode));
                T = T->rchild;
                T->data = definition[pos++];
                Stack[top++] = T;
            }
            else
            {
                T->rchild = NULL;
                sign[top] = 0;
                T = Stack[--top];
                pos++;
            }
        }
        T = p;
    }
    else
        return ERROR;
    fclose(fp);
    return OK;
}

int max = 0, sum = 0;
void PreView(BiTree T)
{
    if (T == NULL)
        return;
    sum = sum + T->data.key;
    PreView(T->lchild);
    if (sum > max)
        max = sum;
    PreView(T->rchild);
    if (sum > max)
        max = sum;
    sum = sum - T->data.key;
}
status MaxPathSum(BiTree T)
// 初始条件是二叉树T存在；操作结果是返回根节点到叶子结点的最大路径和；
{
    PreView(T);
    return max;
}

BiTree LowestCommonAncestor(BiTree T, KeyType bro1, KeyType bro2)
// 初始条件是二叉树T存在；操作结果是该二叉树中e1节点和e2节点的最近公共祖先；
{
    struct BiTNode *St[50] = {NULL}, *sign[50] = {NULL};
    St[0] = T;
    sign[0] = T;
    int top = 1, current = 0, num = 1, record_bro1 = 0, record_bro2 = 0, i;
    do
    {
        if (St[current]->data.key == bro1)
        {
            i = current;
            while (St[current] != sign[i++])
                ;
            record_bro1 = i;
        }
        if (St[current]->data.key == bro2)
        {
            i = current;
            while (St[current] != sign[i++])
                ;
            record_bro2 = i;
        }
        if (St[current]->lchild != NULL)
        {
            St[top++] = St[current]->lchild;
            sign[num++] = St[current]->lchild;
        }
        else
            num++;
        if (St[current]->rchild != NULL)
        {
            St[top++] = St[current]->rchild;
            sign[num++] = St[current]->rchild;
        }
        else
            num++;
        if (record_bro1 && record_bro2)
            break;
        current++;
    } while (St[current] != NULL);
    while (record_bro1 != record_bro2)
    {
        if (record_bro1 > record_bro2)
            record_bro1 /= 2;
        else
            record_bro2 /= 2;
    }
    return sign[record_bro1 - 1];
}

status InvertTree(BiTree &T)
// 初始条件是线性表L已存在；操作结果是将T翻转，使其所有节点的左右节点互换；
{
    struct BiTNode *St[50] = {NULL}, *temp;
    St[0] = T;
    int top = 1, current = 0;
    if (T != NULL)
    {
        do
        {
            temp = St[current]->lchild;
            St[current]->lchild = St[current]->rchild;
            St[current]->rchild = temp;
            if (St[current]->lchild != NULL)
                St[top++] = St[current]->lchild;
            if (St[current]->rchild != NULL)
                St[top++] = St[current]->rchild;
            current++;
        } while (St[current] != NULL);
        return OK;
    }
    return ERROR;
}

status AddList(LISTS &Lists, char ListName[])
{
    int i = 0;
    for (int j = 0; j < Lists.length; j++)
        if (strcmp(Lists.elem[j].name, ListName) == 0)
            return ERROR;
    if (Lists.length < LIST_INIT_SIZE - 1)
    {
        while (ListName[i] != '\0')
        {
            Lists.elem[Lists.length].name[i] = ListName[i];
            i++;
        }
        Lists.elem[Lists.length].name[i] = '\0'; // 以此作为结束标志
    }
    else
        return OVERFLOW;
    Lists.elem[Lists.length].T = NULL;
    Lists.length++;
    return OK;
}

// status RemoveGraph(LISTS &Lists,char ListName[]){ // Lists中删除一个名称为ListName的线性表
//     for(int i=0;i<Lists.length;i++){
//         int p=0;
//         while(ListName[p]!='\0' && ListName[p]==Lists.elem[i].name[p]) p++;
//         if(Lists.elem[i].name[p]=='\0'){
//             DestroyGraph(Lists.elem[i].G);
//             for(int j=i+1;j<Lists.length;j++)
//                 Lists.elem[j-1]=Lists.elem[j];
//             Lists.length=Lists.length-1;
//             return OK;
//         }
//     }
//     return ERROR;
// }

// int LocateGraph(LISTS Lists,char ListName[])
//  // 在Lists中查找一个名称为ListName的线性表，成功返回逻辑序号，否则返回0
// {
//     for(int i=0;i<Lists.length;i++){
//         int p=0;
//         while(ListName[p]!='\0' && Lists.elem[i].name[p]==ListName[p]) p++;
//         if(Lists.elem[i].name[p]=='\0') return (i+1);
//     }
//     return ERROR;
// }
BiTree TreeChange(LISTS Lists, char ListName[], int &record)
{
    for (int i = 0; i < Lists.length; i++)
        if (strcmp(Lists.elem[i].name, ListName) == 0)
        {
            record = i;
            return Lists.elem[i].T;
        }
    return NULL;
}

status BiTreeEmpty(BiTree T)
{
    if (T == NULL)
        return TRUE;
    else
        return FALSE;
}