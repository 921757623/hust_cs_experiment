status InitList(LinkList &L)
// ���Ա�L�����ڣ�����һ���յ����Ա�����OK�����򷵻�INFEASIBLE��
{
    if(L != NULL)
    return INFEASIBLE;
    else
    {
        L = (LinkList)malloc(sizeof(LNode));
        L->next = NULL;
        return OK;
    }
}

status DestroyList(LinkList &L)
// ������Ա�L���ڣ��������Ա�L���ͷ�����Ԫ�صĿռ䣬����OK�����򷵻�INFEASIBLE��
{
    if(L == NULL)
    return INFEASIBLE;
    else {
        LNode *head,*tail;
        head = L;
        while(head != NULL) {
            tail = head->next;
            free(head);
            head = tail;
        }
        L = NULL;
        return OK;
    }
}

status ClearList(LinkList &L)
// ������Ա�L���ڣ�ɾ�����Ա�L�е�����Ԫ�أ�����OK�����򷵻�INFEASIBLE��
{
    if(L == NULL)
    return INFEASIBLE;
    else {
        LNode *head,*tail;
        head = L->next;
        while(head != NULL) {
            tail = head->next;
            free(head);
            head = tail;
        }
        L->next = NULL;
        return OK;
    }
}

status ListEmpty(LinkList L)
// ������Ա�L���ڣ��ж����Ա�L�Ƿ�Ϊ�գ��վͷ���TRUE�����򷵻�FALSE��������Ա�L�����ڣ�����INFEASIBLE��
{
    if(L == NULL)
    return INFEASIBLE;
    else if(L->next == NULL)
    return TRUE;
    else
    return FALSE;
}

int ListLength(LinkList L)
// ������Ա�L���ڣ��������Ա�L�ĳ��ȣ����򷵻�INFEASIBLE��
{
    if(L == NULL)
    return INFEASIBLE;
    else {
        int length = 0;
        LNode *phead = L->next;
        while(phead != NULL) {
            length++;
            phead = phead->next;
        }
        return length;
    }
}

status GetElem(LinkList L,int i,ElemType &e)
// ������Ա�L���ڣ���ȡ���Ա�L�ĵ�i��Ԫ�أ�������e�У�����OK�����i���Ϸ�������ERROR��������Ա�L�����ڣ�����INFEASIBLE��
{
    if(L == NULL)
    return INFEASIBLE;
    else {
        LNode *head = L->next;
        int count = 1;
        if(i < 1)
        return ERROR;
        while(count != i && head != NULL) {
            count++;
            head = head->next;
        }
        if(head == NULL)
        return ERROR;
        e = head->data;
        return OK;
    }
}

status LocateElem(LinkList L,ElemType e)
// ������Ա�L���ڣ�����Ԫ��e�����Ա�L�е�λ����ţ����e�����ڣ�����ERROR�������Ա�L������ʱ������INFEASIBLE��
{
    if(L == NULL)
    return INFEASIBLE;
    else {
        LNode *head = L->next;
        int position = 1;
        while(head != NULL) {
            if(head->data == e)
            return position;
            position++;
            head =head->next;
        }
        return ERROR;
    }
}

status PriorElem(LinkList L,ElemType e,ElemType &pre)
// ������Ա�L���ڣ���ȡ���Ա�L��Ԫ��e��ǰ����������pre�У�����OK�����û��ǰ��������ERROR��������Ա�L�����ڣ�����INFEASIBLE��
{
    if(L == NULL)
    return INFEASIBLE;
    else {
        if(L->next == NULL)
        return ERROR;
        LNode *fast = L->next->next , *slow = L->next;
        if(slow->data == e)
        return ERROR;
        while(fast != NULL) {
            if(fast->data == e) {
                pre = slow->data;
                return OK;
            }
            fast = fast->next;
            slow = slow->next;
        }
        return ERROR;
    }
}

status NextElem(LinkList L,ElemType e,ElemType &next)
// ������Ա�L���ڣ���ȡ���Ա�LԪ��e�ĺ�̣�������next�У�����OK�����û�к�̣�����ERROR��������Ա�L�����ڣ�����INFEASIBLE��
{
    if(L == NULL)
    return INFEASIBLE;
    else {
        if(L->next == NULL)
        return ERROR;
        LNode *head = L->next;
        while(head->next != NULL) {
            if (head->data == e) {
                next = head->next->data;
                return OK;
            }
            head = head->next;
        }
        return ERROR;
    }
}

status ListInsert(LinkList &L,int i,ElemType e)
// ������Ա�L���ڣ���Ԫ��e���뵽���Ա�L�ĵ�i��Ԫ��֮ǰ������OK��������λ�ò���ȷʱ������ERROR��������Ա�L�����ڣ�����INFEASIBLE��
{
    if(L == NULL)
    return INFEASIBLE;
    else {
        if(L->next == NULL && i == 1) {
            L->next = (LinkList)malloc(sizeof(LNode));
            L->next->data = e;
            L->next->next = NULL;
            return OK;
        }
        else if(L->next == NULL && i != 1)
        return ERROR;
        int position = 1;
        LNode *fast = L->next , *slow = L;
        while(fast != NULL) {
            if(position == i) {
                LNode *insert = (LinkList)malloc(sizeof(LNode));
                insert->data = e;
                insert->next = slow->next;
                slow->next = insert;
                return OK;
            }
            slow = fast;
            fast = fast->next;
            if(fast == NULL && position == i-1) {
                LNode *insert = (LinkList)malloc(sizeof(LNode));
                insert->data = e;
                insert->next = slow->next;
                slow->next = insert;
                return OK;
            }
            position++;
        }
        return ERROR;
    }
}

status ListDelete(LinkList &L,int i,ElemType &e)
// ������Ա�L���ڣ�ɾ�����Ա�L�ĵ�i��Ԫ�أ���������e�У�����OK����ɾ��λ�ò���ȷʱ������ERROR��������Ա�L�����ڣ�����INFEASIBLE��
{
    if(L == NULL)
    return INFEASIBLE;
    else {
        if(L->next == NULL)
        return ERROR;
        int position = 1;
        LNode *head = L->next , *front = L;
        while(head != NULL) {
            if(position == i) {
                e = head->data;
                front->next = head->next;
                free(head);
                return OK;
            }
            front = head;
            head = head->next;
            position++;
        }
        return ERROR;
    }
}

status ListTraverse(LinkList L)
// ������Ա�L���ڣ�������ʾ���Ա��е�Ԫ�أ�ÿ��Ԫ�ؼ��һ�񣬷���OK��������Ա�L�����ڣ�����INFEASIBLE��
{
    if(L == NULL)
    return INFEASIBLE;
    else if(L->next == NULL) {
        printf("����Ϊ��!");
        return OK;
    }
    else {
        LNode *head = L->next;
        while(head != NULL) {
            printf("%d",head->data);
            if(head->next != NULL)
            printf(" ");
            head = head->next;
        }
        return OK;
    }
}

status SaveList(LinkList L,char FileName[])
// ������Ա�L���ڣ������Ա�L�ĵ�Ԫ��д��FileName�ļ��У�����OK�����򷵻�INFEASIBLE��
{
    if(L == NULL)
    return INFEASIBLE;
    else {
        LNode *head = L->next;
        FILE *fp = NULL;
        if(head == NULL)
        return OK;
        if((fp = fopen(FileName,"w")) != NULL) {
            while(head != NULL) {
                fprintf(fp,"%d",head->data);
                if(head->next != NULL)
                fprintf(fp,"%c",' ');
                head = head->next;
            }
        }
        fclose(fp);
        return OK;
    }
}

status LoadList(LinkList &L,char FileName[])
// ������Ա�L�����ڣ���FileName�ļ��е����ݶ��뵽���Ա�L�У�����OK�����򷵻�INFEASIBLE��
{
    if(L != NULL)
    return INFEASIBLE;
    else {
        FILE *fp = NULL;
        L = (LinkList)malloc(sizeof(LNode));
        LNode *head = (LinkList)malloc(sizeof(LNode)) , *tail;
        L->next = head;
        if((fp = fopen(FileName,"r")) != NULL) {
            while(fscanf(fp,"%d",&head->data) != EOF) {
                tail = (LinkList)malloc(sizeof(LNode));
                head->next = tail;
                tail = head;
                head = head->next;
            }
        }
        free(head);
		tail->next = NULL;
        fclose(fp);
        return OK;
    }
}

status reverseList(LinkList &L)
// ������Ա�L���ڣ���ת��������OK��������Ա�L�����ڣ�����INFEASIBLE��
{
    if(L == NULL)
    return INFEASIBLE;
    if(L->next == NULL)
    return OK;
    else {
        LNode *head = L->next, *temp , *res;
        while(head->next != NULL) //tempΪ��ǰ���
            {
            temp = head->next;
            head->next = temp->next;
            temp->next = L->next;
            L->next = temp;          
        }
        return OK;
    }
}

status RemoveNthFromEnd(LinkList &L,int n) 
// ������Ա�L�����ҷǿգ�ɾ����������n����㣬����OK��������Ա�L�����ڣ��������ڷ���INFEASIBLE����Ϊ�ջ򳬳���Χ����ERROR��
{
    if(L == NULL)
        return INFEASIBLE;
    if(L->next == NULL)
        return ERROR;
    int position = 1 ,length = 0;
    reverseList(L);
    LNode *head = L->next , *forhead = L;
    while(head != NULL && position <= n) {
        if(position == n) {
            forhead->next = head->next;
            free(head);
            reverseList(L);
            return OK;
        }
        position++;
        head = head->next;
        forhead = forhead->next;
    }
    return ERROR;
}

status sortList(LinkList &L)
// ������Ա�L���ڣ����������򣬷���OK��������Ա�L�����ڣ�����INFEASIBLE��
{
    if(L == NULL)
    return INFEASIBLE;
    else {
        int temp;
        LNode *head = L->next ,*tail = NULL;
        while(head != tail) {
            while(head->next != tail) {
                if(head->data > head->next->data) {
                    temp = head->data;
                    head->data = head->next->data;
                    head->next->data = temp;
                }
                head = head->next;
            }
            tail = head;
            head = L->next;
        }
        printf("ѡ����������������1,��������������0!\n");
        scanf("%d",&temp);
        if(temp == 1)
        return OK;
        else if(temp == 0) {
            reverseList(L);
            return OK;
        }
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
    Lists.elem[Lists.length].L = NULL;
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
            DestroyList(Lists.elem[i].L);
            for(int j=i;j<Lists.length;j++)
            Lists.elem[i] = Lists.elem[i+1];
            Lists.length--;
            return OK;
        }
    return ERROR;
}

status LocateList(LISTS Lists,char ListName[])
// ��Lists�в���һ������ΪListName�����Ա��ɹ������߼���ţ����򷵻�False
{
    for(int i=0;i < Lists.length;i++)
        if(strcmp(Lists.elem[i].name,ListName) == 0)
            return i+1;
    return FALSE;
}

status ListPrint(LISTS &Lists,char ListName[])
//��ӡ�����Ա��е�����
{
    int loc;
    loc = LocateList(Lists,ListName);
    if(loc == FALSE)
    return FALSE;
    loc = loc-1;
    if(Lists.elem[loc].L == NULL)
    return INFEASIBLE;
    else {
        ListTraverse(Lists.elem[loc].L);
        return OK;
    }
}

status MutlListInsert(LISTS &Lists,char ListName[],int i,ElemType e)
{
    int loc;
    loc = LocateList(Lists,ListName);
    if(loc == FALSE)
    return FALSE;
    loc = loc-1;
    if(Lists.elem[loc].L == NULL)
    return INFEASIBLE;
    else {
        if(Lists.elem[loc].L->next == NULL && i == 1) {
            Lists.elem[loc].L->next = (LinkList)malloc(sizeof(LNode));
            Lists.elem[loc].L->next->data = e;
            Lists.elem[loc].L->next->next = NULL;
            return OK;
        }
        else if(Lists.elem[loc].L->next == NULL && i != 1)
        return ERROR;
        int position = 1;
        LNode *fast = Lists.elem[loc].L->next , *slow = Lists.elem[loc].L;
        while(fast != NULL) {
            if(position == i) {
                LNode *insert = (LinkList)malloc(sizeof(LNode));
                insert->data = e;
                insert->next = slow->next;
                slow->next = insert;
                return OK;
            }
            slow =fast;
            fast = fast->next;
            if(fast == NULL && position == i-1) {
                LNode *insert = (LinkList)malloc(sizeof(LNode));
                insert->data = e;
                insert->next = slow->next;
                slow->next = insert;
                return OK;
            }
            position++;
        }
        return ERROR;
    }
}

status Delete(LISTS &Lists,char ListName[],int i,ElemType &e)
{
    int loc;
    loc = LocateList(Lists,ListName);
    if(loc == FALSE)
    return FALSE;
    loc = loc-1;
    if(Lists.elem[loc].L == NULL)
    return INFEASIBLE;
    else {
        if(Lists.elem[loc].L->next == NULL)
        return ERROR;
        int position = 1;
        LNode *head = Lists.elem[loc].L->next , *front = Lists.elem[loc].L;
        while(head != NULL) {
            if(position == i) {
                e = head->data;
                front->next = head->next;
                free(head);
                return OK;
            }
            front = head;
            head = head->next;
            position++;
        }
        return ERROR;
    }
}