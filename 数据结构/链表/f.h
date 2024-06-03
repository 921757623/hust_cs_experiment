status InitList(LinkList &L)
// 线性表L不存在，构造一个空的线性表，返回OK，否则返回INFEASIBLE。
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
// 如果线性表L存在，销毁线性表L，释放数据元素的空间，返回OK，否则返回INFEASIBLE。
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
// 如果线性表L存在，删除线性表L中的所有元素，返回OK，否则返回INFEASIBLE。
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
// 如果线性表L存在，判断线性表L是否为空，空就返回TRUE，否则返回FALSE；如果线性表L不存在，返回INFEASIBLE。
{
    if(L == NULL)
    return INFEASIBLE;
    else if(L->next == NULL)
    return TRUE;
    else
    return FALSE;
}

int ListLength(LinkList L)
// 如果线性表L存在，返回线性表L的长度，否则返回INFEASIBLE。
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
// 如果线性表L存在，获取线性表L的第i个元素，保存在e中，返回OK；如果i不合法，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
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
// 如果线性表L存在，查找元素e在线性表L中的位置序号；如果e不存在，返回ERROR；当线性表L不存在时，返回INFEASIBLE。
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
// 如果线性表L存在，获取线性表L中元素e的前驱，保存在pre中，返回OK；如果没有前驱，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
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
// 如果线性表L存在，获取线性表L元素e的后继，保存在next中，返回OK；如果没有后继，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
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
// 如果线性表L存在，将元素e插入到线性表L的第i个元素之前，返回OK；当插入位置不正确时，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
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
// 如果线性表L存在，删除线性表L的第i个元素，并保存在e中，返回OK；当删除位置不正确时，返回ERROR；如果线性表L不存在，返回INFEASIBLE。
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
// 如果线性表L存在，依次显示线性表中的元素，每个元素间空一格，返回OK；如果线性表L不存在，返回INFEASIBLE。
{
    if(L == NULL)
    return INFEASIBLE;
    else if(L->next == NULL) {
        printf("链表为空!");
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
// 如果线性表L存在，将线性表L的的元素写到FileName文件中，返回OK，否则返回INFEASIBLE。
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
// 如果线性表L不存在，将FileName文件中的数据读入到线性表L中，返回OK，否则返回INFEASIBLE。
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
// 如果线性表L存在，翻转链表，返回OK；如果线性表L不存在，返回INFEASIBLE。
{
    if(L == NULL)
    return INFEASIBLE;
    if(L->next == NULL)
    return OK;
    else {
        LNode *head = L->next, *temp , *res;
        while(head->next != NULL) //temp为待前差的
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
// 如果线性表L存在且非空，删除链表倒数第n个结点，返回OK；如果线性表L不存在，若不存在返回INFEASIBLE，若为空或超出范围返回ERROR。
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
// 如果线性表L存在，对链表排序，返回OK；如果线性表L不存在，返回INFEASIBLE。
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
        printf("选择正序排序请输入1,倒序排序请输入0!\n");
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
// Lists中删除一个名称为ListName的线性表
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
// 在Lists中查找一个名称为ListName的线性表，成功返回逻辑序号，否则返回False
{
    for(int i=0;i < Lists.length;i++)
        if(strcmp(Lists.elem[i].name,ListName) == 0)
            return i+1;
    return FALSE;
}

status ListPrint(LISTS &Lists,char ListName[])
//打印多线性表中的链表
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