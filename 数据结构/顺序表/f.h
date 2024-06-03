status InitList(SqList& L)
// ���Ա�L�����ڣ�����һ���յ����Ա�����OK�����򷵻�INFEASIBLE��
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
// ������Ա�L���ڣ��������Ա�L���ͷ�����Ԫ�صĿռ䣬����OK�����򷵻�INFEASIBLE��
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
// ������Ա�L���ڣ�ɾ�����Ա�L�е�����Ԫ�أ�����OK�����򷵻�INFEASIBLE��
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
// ������Ա�L���ڣ��ж����Ա�L�Ƿ�Ϊ�գ��վͷ���TRUE�����򷵻�FALSE��������Ա�L�����ڣ�����INFEASIBLE��
{
    if(L.elem == NULL)
    return INFEASIBLE;
    else if(L.length == 0) 
    return TRUE;
    else
    return FALSE;
}

status ListLength(SqList L)
// ������Ա�L���ڣ��������Ա�L�ĳ��ȣ����򷵻�INFEASIBLE��
{
    if(L.elem == NULL)
    return INFEASIBLE;
    else
    return L.length;
}

status GetElem(SqList L,int i,ElemType &e)
// ������Ա�L���ڣ���ȡ���Ա�L�ĵ�i��Ԫ�أ�������e�У�����OK�����i���Ϸ�������ERROR��������Ա�L�����ڣ�����INFEASIBLE��
{
    if(L.elem == NULL)
    return INFEASIBLE;
    else
        if(i > L.length || i < 1)  //i�������ȷ�Χ����Ϊ����֮�������򱨴�
        return ERROR;
        else
        {
            e = L.elem[i-1];
            return OK;
        }
}

int LocateElem(SqList L,ElemType e)
// ������Ա�L���ڣ�����Ԫ��e�����Ա�L�е�λ����Ų����ظ���ţ����e�����ڣ�����0�������Ա�L������ʱ������INFEASIBLE����-1����
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
// ������Ա�L���ڣ���ȡ���Ա�L��Ԫ��e��ǰ����������pre�У�����OK�����û��ǰ��������ERROR��������Ա�L�����ڣ�����INFEASIBLE��
{
    if(L.elem == NULL)
        return INFEASIBLE;
    else
        for(int i=0;i<L.length;i++)
        {
            if(e == L.elem[i] && i != 0)  //����Ϊ��Ԫ��
            {
                pre = L.elem[i-1];
                return OK;
            }
        }
        return ERROR;
}

status NextElem(SqList L,ElemType e,ElemType &next)
// ������Ա�L���ڣ���ȡ���Ա�LԪ��e�ĺ�̣�������next�У�����OK�����û�к�̣�����ERROR��������Ա�L�����ڣ�����INFEASIBLE��
{
    if(L.elem == NULL)
    return INFEASIBLE;
    else
    {
        for(int i=0;i<L.length;i++)
        {
            if(e == L.elem[i] && i != L.length-1)  //����ΪβԪ��
            {
                next = L.elem[i+1];
                return OK;
            }
        }
        return ERROR;
    }
}

status ListInsert(SqList &L,int i,ElemType e)
// ������Ա�L���ڣ���Ԫ��e���뵽���Ա�L�ĵ�i��Ԫ��֮ǰ������OK��������λ�ò���ȷʱ������ERROR��������Ա�L�����ڣ�����INFEASIBLE��
{
    i = i-1;
    if(L.elem == NULL)
    return INFEASIBLE;
    else
        {
            if(L.length <= L.listsize && i<=L.length && i >=0)
            {
                if(L.length == L.listsize)
                    L.elem = (ElemType *)realloc(L.elem,sizeof(ElemType)*(L.listsize+1));  //���·���ռ�
                if(L.length == 0 && i == 0)  //�ձ�Ͷ������⴦��
                {
                    L.elem[0] = e;
                    L.length++;
                    return OK;
                }
                for(int j=L.length-1;j>=i;j--)   //��λ
                    L.elem[j+1] = L.elem[j];
                L.elem[i] = e;
                L.length++;
                return OK;
            }
            return ERROR;
        }
}

status ListDelete(SqList &L,int i,ElemType &e)
// ������Ա�L���ڣ�ɾ�����Ա�L�ĵ�i��Ԫ�أ���������e�У�����OK����ɾ��λ�ò���ȷʱ������ERROR��������Ա�L�����ڣ�����INFEASIBLE��
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
// ������Ա�L���ڣ�������ʾ���Ա��е�Ԫ�أ�ÿ��Ԫ�ؼ��һ�񣬷���OK��������Ա�L�����ڣ�����INFEASIBLE��
{
    if(L.elem == NULL)
    return INFEASIBLE;
    else
    {
        if(L.length == 0)
        printf("���Ա�Ϊ��!\n");
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
// ������Ա�L���ڣ������Ա�L�ĵ�Ԫ��д��FileName�ļ��У�����OK�����򷵻�INFEASIBLE��
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
            fprintf(fp,"%c",' ');  //ÿ��Ԫ���Կո�ֿ�
        }
        fclose(fp);
        return OK;
    }
}

status  LoadList(SqList &L,char FileName[])
// ������Ա�L�����ڣ���FileName�ļ��е����ݶ��뵽���Ա�L�У�����OK�����򷵻�INFEASIBLE��
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
        while(fscanf(fp,"%d",&L.elem[i]) != EOF)  //������ȡ���ݲ��������Ա�
        {
            i++;
            L.length++;
        }
        fclose(fp);
        return OK;
    }
}

status AddList(LISTS &Lists,char ListName[])
// ֻ��Ҫ��Lists������һ������ΪListName�Ŀ����Ա����Ա������ֺ�̨���Գ�����롣
{
    int i = 0;
    if(Lists.length < LISTINCREMENT-1)
    {
        while(ListName[i] != '\0')
        {
            Lists.elem[Lists.length].name[i] = ListName[i];
            i++;
        }
        Lists.elem[Lists.length].name[i] = '\0';     //�Դ���Ϊ������־
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
// Lists��ɾ��һ������ΪListName�����Ա�
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
// ��Lists�в���һ������ΪListName�����Ա��ɹ������߼���ţ����򷵻�0
{
    for(int i=0;i < Lists.length;i++)
        if(strcmp(Lists.elem[i].name,ListName) == 0)
            return i+1;
    return 0;
}

int reverseList(SqList L)
// ������Ա�L���ڣ����ν�����βԪ�أ�����OK��������Ա�L�����ڣ�����INFEASIBLE��
{
    int temp;
    if(L.elem == NULL)
    return INFEASIBLE;
    else
    {
        for(int i=0,j=L.length-1;i<=j;i++,j--)  //��β����
        {
            temp = L.elem[i];
            L.elem[i] = L.elem[j];
            L.elem[j] = temp;
        }
        return OK;
    }
}

int sortList(SqList &L)
// ������Ա�L���ڣ�������Ա����򣬷���OK��������Ա�L�����ڣ�����INFEASIBLE��
{
    int temp;
    if(L.elem == NULL)
    return INFEASIBLE;
    else
    {
        for(int i = 0;i < L.length;i++)  //ð������
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
                    Lists.elem[loc].L.elem = (ElemType *)realloc(Lists.elem[loc].L.elem,sizeof(ElemType)*(Lists.elem[loc].L.listsize+1));  //���·���ռ�
                if(Lists.elem[loc].L.length == 0 && i == 0)  //�ձ�Ͷ������⴦��
                {
                    Lists.elem[loc].L.elem[0] = e;
                    Lists.elem[loc].L.length++;
                    return OK;
                }
                for(int j=Lists.elem[loc].L.length-1;j>=i;j--)   //��λ
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
        printf("���Ա�Ϊ��!\n");
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