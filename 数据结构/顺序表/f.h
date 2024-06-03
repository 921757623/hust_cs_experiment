status InitList(SqList& L)
// 线性表L不存在，构造一个空的线性表，返回OK，否则返回INFEASIBLE。
{
    if(L.elem == NULL)
    {
    L.elem = (ElemType *) malloc(sizeof(ElemType)*100);
    L.length = 0;
    L.listsize = LIST_INIT_SIZE;
    return OK;
    }
    else
        return INFEASIBLE;
}

status DestroyList(SqList& L)
// 如果线性表L存在，销毁线性表L，释放数据元素的空间，返回OK，否则返回INFEASIBLE。
{
    if(L.elem == NULL)
    return INFEASIBLE;
    else
    {
        free(L.elem);
        L.elem = NULL;
        return OK;
    }
}

status ClearList(SqList& L)
// 如果线性表L存在，删除线性表L中的所有元素，返回OK，否则返回INFEASIBLE。
{
    if(L.elem == NULL)
    return INFEASIBLE;
    else
    {
        free(L.elem);
        L.elem = NULL;
        InitList(L);
        L.length = 0;
        return OK;
    }
}

status ListEmpty(SqList L)
// 如果线性表L存在，判断线性表L是否为空，空就返回TRUE，否则返回FALSE；如果线性表L不存在，返回INFEASIBLE。
{
    if(L.elem == NULL)
    return INFEASIBLE;
    else if(L.length == 0) 
    return TRUE;
    else
    return FALSE;
}

status ListLength(SqList L)
// 如果线性表L存在，返回线性表L的长度，否则返回INFEASIBLE。
{
    if(L.elem == NULL)
    return INFEASIBLE;
    else
    return L.length;
}

status GetElem(SqList L,int i,ElemType &e)
// 如果线性表L存在，获取线性表L的第i个元素，保存在e中，返回OK；如果i不合法，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
    if(L.elem == NULL)
    return INFEASIBLE;
    else
        if(i > L.length || i < 1)  //i超出长度范围或者为负数之类的情况则报错
        return ERROR;
        else
        {
            e = L.elem[i-1];
            return OK;
        }
}

int LocateElem(SqList L,ElemType e)
// 如果线性表L存在，查找元素e在线性表L中的位置序号并返回该序号；如果e不存在，返回0；当线性表L不存在时，返回INFEASIBLE（即-1）。
{
    if(L.elem == NULL)
    return INFEASIBLE;
    else
        for(int i=0;i<L.length;i++)
        {
            if(e == L.elem[i])
            return i+1;
        }
        return ERROR;
}

status PriorElem(SqList L,ElemType e,ElemType &pre)
// 如果线性表L存在，获取线性表L中元素e的前驱，保存在pre中，返回OK；如果没有前驱，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
    if(L.elem == NULL)
        return INFEASIBLE;
    else
        for(int i=0;i<L.length;i++)
        {
            if(e == L.elem[i] && i != 0)  //不能为首元素
            {
                pre = L.elem[i-1];
                return OK;
            }
        }
        return ERROR;
}

status NextElem(SqList L,ElemType e,ElemType &next)
// 如果线性表L存在，获取线性表L元素e的后继，保存在next中，返回OK；如果没有后继，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
    if(L.elem == NULL)
    return INFEASIBLE;
    else
    {
        for(int i=0;i<L.length;i++)
        {
            if(e == L.elem[i] && i != L.length-1)  //不能为尾元素
            {
                next = L.elem[i+1];
                return OK;
            }
        }
        return ERROR;
    }
}

status ListInsert(SqList &L,int i,ElemType e)
// 如果线性表L存在，将元素e插入到线性表L的第i个元素之前，返回OK；当插入位置不正确时，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
    i = i-1;
    if(L.elem == NULL)
    return INFEASIBLE;
    else
        {
            if(L.length <= L.listsize && i<=L.length && i >=0)
            {
                if(L.length == L.listsize)
                    L.elem = (ElemType *)realloc(L.elem,sizeof(ElemType)*(L.listsize+1));  //重新分配空间
                if(L.length == 0 && i == 0)  //空表和队首特殊处理
                {
                    L.elem[0] = e;
                    L.length++;
                    return OK;
                }
                for(int j=L.length-1;j>=i;j--)   //移位
                    L.elem[j+1] = L.elem[j];
                L.elem[i] = e;
                L.length++;
                return OK;
            }
            return ERROR;
        }
}

status ListDelete(SqList &L,int i,ElemType &e)
// 如果线性表L存在，删除线性表L的第i个元素，并保存在e中，返回OK；当删除位置不正确时，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
{
    i = i-1;
    if(L.elem == NULL)
    return INFEASIBLE;
    else
        if(i >=0 && i<L.length)
        {
            e = L.elem[i];
            for(int j = i;j<L.length;j++)
                L.elem[j] = L.elem[j+1];
            L.length--;
            return OK;
        }
        return ERROR;
}

status ListTraverse(SqList L)
// 如果线性表L存在，依次显示线性表中的元素，每个元素间空一格，返回OK；如果线性表L不存在，返回INFEASIBLE。
{
    if(L.elem == NULL)
    return INFEASIBLE;
    else
    {
        if(L.length == 0)
        printf("线性表为空!\n");
        else
        for(int i=0;i<L.length;i++)
        {
            printf("%d",L.elem[i]);
            if(i != L.length-1)
            printf(" ");
        }
        return OK;
    }
}

status  SaveList(SqList L,char FileName[])
// 如果线性表L存在，将线性表L的的元素写到FileName文件中，返回OK，否则返回INFEASIBLE。
{
    if(L.elem == NULL)
    return INFEASIBLE;
    else
    {
        FILE *fp = NULL;
        if((fp = fopen(FileName,"w")) != NULL)
        for(int i=0; i<L.length; i++)
        {
            fprintf(fp,"%d",L.elem[i]);
            if(i != L.length-1)
            fprintf(fp,"%c",' ');  //每个元素以空格分开
        }
        fclose(fp);
        return OK;
    }
}

status  LoadList(SqList &L,char FileName[])
// 如果线性表L不存在，将FileName文件中的数据读入到线性表L中，返回OK，否则返回INFEASIBLE。
{
    if(L.elem != NULL)
    return INFEASIBLE;
    else
    {
        L.elem = (ElemType *) malloc(sizeof(ElemType)*LIST_INIT_SIZE);
        L.length = 0;
        L.listsize = LIST_INIT_SIZE;
        int num[100],len,i=0;
        FILE *fp = NULL;
        if((fp = fopen(FileName,"r")) != NULL)
        while(fscanf(fp,"%d",&L.elem[i]) != EOF)  //按个读取数据并填入线性表
        {
            i++;
            L.length++;
        }
        fclose(fp);
        return OK;
    }
}

status AddList(LISTS &Lists,char ListName[])
// 只需要在Lists中增加一个名称为ListName的空线性表，线性表数据又后台测试程序插入。
{
    int i = 0;
    if(Lists.length < LISTINCREMENT-1)
    {
        while(ListName[i] != '\0')
        {
            Lists.elem[Lists.length].name[i] = ListName[i];
            i++;
        }
        Lists.elem[Lists.length].name[i] = '\0';     //以此作为结束标志
    Lists.elem[Lists.length].L.elem = NULL;
    if(InitList(Lists.elem[Lists.length].L) == OK)
    {
    Lists.length++;
    return OK;
    }
    }
    return ERROR;
}

status RemoveList(LISTS &Lists,char ListName[])
// Lists中删除一个名称为ListName的线性表
{
    for(int i=0;i < Lists.length;i++)
        if(strcmp(Lists.elem[i].name,ListName) == 0)
        {
            for(int j=i;j<Lists.length;j++)
            Lists.elem[i] = Lists.elem[i+1];
            Lists.length--;
            return OK;
        }
    return ERROR;
}

int LocateList(LISTS Lists,char ListName[])
// 在Lists中查找一个名称为ListName的线性表，成功返回逻辑序号，否则返回0
{
    for(int i=0;i < Lists.length;i++)
        if(strcmp(Lists.elem[i].name,ListName) == 0)
            return i+1;
    return 0;
}

int reverseList(SqList L)
// 如果线性表L存在，依次交换首尾元素，返回OK；如果线性表L不存在，返回INFEASIBLE。
{
    int temp;
    if(L.elem == NULL)
    return INFEASIBLE;
    else
    {
        for(int i=0,j=L.length-1;i<=j;i++,j--)  //首尾交换
        {
            temp = L.elem[i];
            L.elem[i] = L.elem[j];
            L.elem[j] = temp;
        }
        return OK;
    }
}

int sortList(SqList &L)
// 如果线性表L存在，则对线性表排序，返回OK；如果线性表L不存在，返回INFEASIBLE。
{
    int temp;
    if(L.elem == NULL)
    return INFEASIBLE;
    else
    {
        for(int i = 0;i < L.length;i++)  //冒泡排序
            for(int j = 0;j < L.length-i-1;j++)
                if(L.elem[j] > L.elem[j+1])
                {
                    temp = L.elem[j];
                    L.elem[j] = L.elem[j+1];
                    L.elem[j+1] = temp;
                }
        return OK;
    }
}

int MutlListInsert(LISTS &Lists,char ListName[],int i,ElemType e)
{
    int loc;
    loc = LocateList(Lists,ListName);
    loc = loc-1;
    i = i-1;
    if(Lists.elem[loc].L.elem == NULL)
    return INFEASIBLE;
    else
        {
            if(Lists.elem[loc].L.length <= Lists.elem[loc].L.listsize && i<=Lists.elem[loc].L.length && i >=0)
            {
                if(Lists.elem[loc].L.length == Lists.elem[loc].L.listsize)
                    Lists.elem[loc].L.elem = (ElemType *)realloc(Lists.elem[loc].L.elem,sizeof(ElemType)*(Lists.elem[loc].L.listsize+1));  //重新分配空间
                if(Lists.elem[loc].L.length == 0 && i == 0)  //空表和队首特殊处理
                {
                    Lists.elem[loc].L.elem[0] = e;
                    Lists.elem[loc].L.length++;
                    return OK;
                }
                for(int j=Lists.elem[loc].L.length-1;j>=i;j--)   //移位
                    Lists.elem[loc].L.elem[j+1] = Lists.elem[loc].L.elem[j];
                Lists.elem[loc].L.elem[i] = e;
                Lists.elem[loc].L.length++;
                return OK;
            }
            return ERROR;
        }
}

int ListPrint(LISTS &Lists,char ListName[])
{
    int loc;
    loc = LocateList(Lists,ListName);
    loc = loc-1;
    if(Lists.elem[loc].L.elem == NULL)
    return INFEASIBLE;
    else
    {
        if(Lists.elem[loc].L.length == 0)
        printf("线性表为空!\n");
        else
        for(int i=0;i<Lists.elem[loc].L.length;i++)
        {
            printf("%d",Lists.elem[loc].L.elem[i]);
            if(i != Lists.elem[loc].L.length-1)
            printf(" ");
        }
        return OK;
    }
}

int Delete(LISTS &Lists,char ListName[],int i)
{
    int loc;
    loc = LocateList(Lists,ListName);
    loc = loc-1;
    if(Lists.elem[loc].L.elem == NULL)
    return INFEASIBLE;
    else
        if(i >=0 && i<Lists.elem[loc].L.length)
        {
            for(int j = i;j<Lists.elem[loc].L.length;j++)
                Lists.elem[loc].L.elem[j] = Lists.elem[loc].L.elem[j+1];
           Lists.elem[loc].L.length--;
            return OK;
        }
        return ERROR;
}

int MaxsubArray(SqList L) 
{
    if(L.elem == NULL)
    return INFEASIBLE;
    else {
        if(L.length == 0)
        return 0;
        int Maxsum = 0, sum = 0;
        for(int i = 0 ; i < L.length ; i++) {
            if(L.elem[i] > 0)
                sum = sum + L.elem[i];
            else {
                if(Maxsum < sum) 
                    Maxsum = sum;
                sum = 0;
            }
        }
        Maxsum = Maxsum > sum ? Maxsum : sum;
        if(Maxsum == 0) {
            Maxsum = L.elem[0];
            for(int i = 0; i < L.length; i++) {
                Maxsum = Maxsum > sum ? Maxsum : sum;
            }
        }
        return Maxsum;
    }
}