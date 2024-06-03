/*
 * @Description:/*
 * @Description: ������
 * @version: 1.0.0
 * @Author: yrp
 * @Date: 2022-08-21 19:18:59
 * @LastEditTime: 2022-10-06 10:50:24
 */

void *main_thread(void *arg);
void *count_thread(void *arg);
status addClause(ClauseList &clause, int data);
status recordValue(ClauseList clause, boolean value[]);
status deleteLiteral(ClauseList &clause, int var);
status deleteClause(ClauseList &clause);
status deleteVar(ClauseList &clause, int var);
status check(ClauseList clause, boolean value[]);
status drawFinalPanel(int data[], char str[]);
boolean isUnitClause(ClauseList clause);
boolean isEmptyClause(ClauseList clause);
boolean evaluateClause(ClauseList clause, boolean value[]);
ClauseList getUnitClause(ClauseList clause);
ClauseList copySingleClause(ClauseList clause);
ClauseList copyClauses(ClauseList origin);

/**
 * @brief: ��ӡ�����վ�
 * @param {int} data
 * @param {char} str
 * @return {*}
 */
status drawFinalPanel(int data[], char str[])
{
    printf("-------****** %s ******-------\n", str);
    printf("----------------------------------\n");
    // ��ӡ�ϰ벿��
    for (int y = 1; y <= 6; y++)
    {
        for (int x = 1 + (y - 1) * 9; x <= y * 9; x++)
        {
            printf("%d ", data[x] % 10);
            if (x % 3 == 0)
                printf(" ");
        }
        printf("\n");
        if (y % 3 == 0)
            printf("\n");
    }
    //��ӡ�м��ص�����
    for (int y = 7; y <= 9; y++)
    {
        for (int x = 55 + (y - 7) * 9; x <= y * 9 - 3; x++)
        {
            printf("%d ", data[x] % 10);
            if (x % 3 == 0)
                printf(" ");
        }
        for (int x = 82 + (y - 7) * 9; x <= 90 + (y - 7) * 9; x++)
        {
            printf("%d ", data[x] % 10);
            if (x % 3 == 0)
                printf(" ");
        }
        printf("\n");
        if (y % 3 == 0)
            printf("\n");
    }
    //��ӡ�°벿��
    for (int y = 4; y <= 9; y++)
    {
        printf("              ");
        for (int x = 109 + (y - 4) * 9; x <= (y + 9) * 9; x++)
        {
            printf("%d ", data[x] % 10);
            if (x % 3 == 0)
                printf(" ");
        }
        printf("\n");
        if (y % 3 == 0)
            printf("\n");
    }
    printf("----------------------------------\n");
    printf("--------****** End ******---------\n");
    return OK;
}

/**
 * @brief ��ԭ�Ӿ���������һ�����Ӿ�
 * @param {ClauseList} &clause
 * @param {int} data
 * @return {status}
 */
status addClause(ClauseList &clause, int data)
{
    ClauseList temp = (ClauseList)malloc(sizeof(ClauseNode));
    temp->head = (LiteralList)malloc(sizeof(LiteralNode));
    temp->head->next = (LiteralList)malloc(sizeof(LiteralNode));
    temp->head->next->next = NULL;

    temp->head->next->data = data;
    temp->nodeNum = 1;
    temp->next = clause->next;
    clause->next = temp;

    return OK;
}

/**
 * @brief ��¼���Ӿ��б�Ԫ����ֵ
 * @param {ClauseList} clause
 * @param {boolean} value
 * @return {status}
 */
status recordValue(ClauseList clause)
{
    LiteralList head = clause->head->next;

    if (head->data > 0)
        value[head->data - 1] = true;
    else
        value[abs(head->data) - 1] = false;

    return OK;
}

/**
 * @brief ɾ���Ӿ��е��ض�����
 * @param {ClauseList} &clause
 * @param {int} var
 * @return {*}
 */
boolean deleteLiteral(ClauseList &clause, int var)
{
    LiteralList literal = clause->head, temp;

    while (literal != NULL && literal->next != NULL)
    {
        if (literal->next->data == -var)
        {
            temp = literal->next;
            literal->next = literal->next->next;
            clause->nodeNum--;
            free(temp);
        }
        else if (literal->next->data == var)
            return false;
        literal = literal->next;
    }
    return true;
}

/**
 * @brief ɾ��������Ԫ��ͬ���Ӿ�
 * @param {ClauseList} &clause
 * @return {status}
 */
status deleteClause(ClauseList &clause)
{
    ClauseList temp = clause->next;
    LiteralList literal;
    do
    {
        literal = temp->head->next;
        temp->head->next = temp->head->next->next;
        free(literal);
    } while (temp->head->next != NULL); // ɾ���Ӿ�����������
    clause->next = clause->next->next;
    free(temp);
    return OK;
}

/**
 * @brief: ���Ӿ�����ɾ�����Ӿ�ı�Ԫ
 * @param {ClauseList} &clause
 * @param {int} var
 * @param {counts} count
 * @return {*}
 */
status deleteVar(ClauseList &clause, int var)
{
    ClauseList temp = clause->next, pre = clause;
    int value;
    while (temp) // ѭ����ȡÿһ���Ӿ�
    {
        value = deleteLiteral(temp, var); // ɾ���Ӿ���ĸ���Ԫ���������������Ԫ�򷵻�false
        if (value == false)               // ����������Ԫ
        {
            deleteClause(pre); // ������Ԫ��ͬ��ɾ���Ӿ�
            temp = pre->next;
            continue;
        }
        if (temp == NULL)
            break;
        pre = temp; // ��һ���Ӿ�
        temp = temp->next;
    }
    return OK;
}

/**
 * @brief �ж��Ƿ�Ϊ���Ӿ�
 * @param {ClauseList} clause
 * @return {boolean}
 */
boolean isUnitClause(ClauseList clause)
{
    if (clause->nodeNum == 1)
        return true;
    else
        return false;
}

/**
 * @brief ��ȡ���Ӿ䣬���Ҳ����򷵻�NULL
 * @param {ClauseList} clause
 * @return {ClauseList}
 */
ClauseList getUnitClause(ClauseList clause)
{
    while (clause != NULL)
    {
        if (isUnitClause(clause))
            return clause;
        clause = clause->next;
    }
    return NULL;
}

/**
 * @brief �ж��Ӿ��Ƿ�Ϊ��
 * @param {ClauseList} clause
 * @return {boolean}
 */
boolean isEmptyClause(ClauseList clause)
{
    while (clause != NULL)
    {
        if (clause->nodeNum == 0)
            return true;
        clause = clause->next;
    }
    return false;
}

/**
 * @brief �ж��Ӿ��������
 * @param {ClauseList} clause
 * @param {int} value
 * @return {int}
 */
boolean evaluateClause(ClauseList clause, boolean value[])
{
    LiteralList head = clause->head->next;
    while (head)
    {
        if (head->data > 0)
            if (value[head->data - 1] == true)
                return true;
            else
                ;
        else if (value[abs(head->data) - 1] == false)
            return true;
        head = head->next;
    }
    return false;
}

/**
 * @brief ���Ƶ����Ӿ�
 * @param {ClauseList} clause
 * @return {*}
 */
ClauseList copySingleClause(ClauseList clause)
{
    ClauseList temp = (ClauseList)malloc(sizeof(ClauseNode));
    temp->nodeNum = 0;
    temp->head = (LiteralList)malloc(sizeof(LiteralNode));
    temp->next = NULL;
    temp->head->next = NULL;
    LiteralList literal = clause->head->next, headp = temp->head;
    while (literal)
    {
        headp->next = (LiteralList)malloc(sizeof(LiteralNode));
        headp->next->data = literal->data;
        literal = literal->next;
        headp = headp->next;
        headp->next = NULL;
        temp->nodeNum++;
    }
    return temp;
}

/**
 * @brief �����Ӿ���
 * @param {ClauseList} origin
 * @return {*}
 */
ClauseList copyClauses(ClauseList origin)
{
    ClauseList clause = (ClauseList)malloc(sizeof(ClauseNode));
    clause->next = NULL;
    ClauseList headp = clause;
    while (origin)
    {
        clause->next = copySingleClause(origin);
        clause = clause->next;
        origin = origin->next;
    }
    return headp;
}

/**
 * @brief DPLL������
 * @param {ClauseList} &clause
 * @param {int} value
 * @return {*}
 */
boolean DPLL_1(ClauseList clause)
{
    ClauseList headp = clause->next;
    pthread_testcancel();
    //���Ӿ����
    ClauseList unitClause = getUnitClause(headp);
    while (unitClause != NULL)
    {
        recordValue(unitClause);                         //��¼�õ��Ӿ��������ֵ
        deleteVar(clause, unitClause->head->next->data); //�����е��־���ɾ������ͬ����

        if (clause->next == NULL) // SΪ��
            return true;
        else if (isEmptyClause(clause->next)) // clause�д��ڿ��Ӿ�
            return false;

        headp = clause->next;
        unitClause = getUnitClause(headp);
    }
    //����
    int var = headp->head->next->data; // ѡ��Ԫ
    ClauseList copy = copyClauses(headp);
    addClause(copy, var);
    if (DPLL_1(copy))
        return true;
    addClause(clause, -var);
    return DPLL_1(clause);
}

boolean DPLL_2(ClauseList clause)
{
    ClauseList stack[100000];
    int saveValue[100000] = {0};
    int top = 0, val, flag;
    stack[++top] = clause;
    while (top != -1)
    {
        pthread_testcancel();
        flag = 0;
        ClauseList c = stack[top];
        ClauseList headp = c->next;
        ClauseList unitClause = getUnitClause(headp);
        srand(time(NULL));
        while (unitClause != NULL)
        {
            recordValue(unitClause);                    //��¼�õ��Ӿ��������ֵ
            deleteVar(c, unitClause->head->next->data); //�����е��־���ɾ������ͬ����

            if (c->next == NULL) // SΪ��
                return true;
            else if (isEmptyClause(c->next)) // clause�д��ڿ��Ӿ�
            {
                top--;
                if (top != 0)
                {
                    addClause(stack[top], -saveValue[top + 1]);
                    flag = 1;
                    break;
                }
                return false;
            }

            headp = c->next;
            unitClause = getUnitClause(headp);
        }
        //���Ѳ���
        if (flag != 1)
        {
            val = abs(headp->head->next->data);
            // if (counts[val].plus <= counts[val].minus)
            //     if (rand() % 3 < 2)
            //         val = -val;
            ClauseList copy = copyClauses(headp);
            addClause(copy, val);
            stack[++top] = copy;
            saveValue[top] = val;
        }
    }
    return false;
}

/**
 * @brief У������������ȷ�ԣ��Ƿ�����ȫ���Ӿ�
 * @param {ClauseList} clause
 * @param {boolean} value
 * @return {*}
 */
status check(ClauseList clause)
{
    int count = 1;
    clause = clause->next;
    while (clause)
    {
        if (!evaluateClause(clause, value))
        {
            printf("%d false!\n", count);
        }
        count++;
        clause = clause->next;
    }
    return OK;
}

/**
 * @brief: DPLL�����߳�
 * @param {void} *arg
 * @return {*}
 */
void *main_thread_1(void *arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    param params = (param)arg;
    clock_t start;
    float end;
    char string[5]; //����ָ��
    int choice, val;

    printf("DPLL�ݹ�������1,���ǵݹ�������2.\n");
    scanf("%d", &choice);
    printf("DPLL��ʼ����...\n��ʱ%d��\n", MAX_EXECUTE_TIME);
    start = clock();
    if (choice == 1)
        val = DPLL_1(params->clause); //�ݹ�ⷨ
    else if (choice == 2)
        val = DPLL_2(params->clause); //�ǵݹ�ⷨ
    end = ((float)(clock() - start));
    printf("DPLL�������!\n");
    printf("��ʱ��%.1f ms\n", end);

    int vals = pthread_cancel(*params->thread); //ȡ����ʱ�߳�
    if (vals != 0)
    {
        printf("��ֹ��ʱ�߳�ʧ�ܣ�\n");
        printf("������Ϊ��%d\n", val);
    }

    if (val == false)
    {
        printf("������������!\n");
    }
    else
    {
        printf("��ʼУ����...\n");
        check(params->clause);
        printf("У��ɹ���\n");
    }

    printf("�Ƿ���������[Yes/No]\n");
    scanf("%s", string);
    if (strcmp(string, "Yes") == 0 || strcmp(string, "yes") == 0 || strcmp(string, "y") == 0)
        if (generateResFile(params->literalNum, params->fileName, val, end) == OK)
            printf("�ļ����ɳɹ���\n");
        else
            printf("�ļ�����ʧ�ܣ�\n");
    return (void *)"2";
}

/**
 * @brief: ���������߳�
 * @param {void} *arg
 * @return {*}
 */
void *main_thread_2(void *arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    param params = (param)arg;
    clock_t start;
    float end;
    char string[5];
    int choice, val;
    int ans[165];

    printf("DPLL�ݹ�������1,���ǵݹ�������2.\n");
    scanf("%d", &choice);
    printf("DPLL��ʼ����...\n��ʱ%d��\n", MAX_EXECUTE_TIME);
    start = clock();
    if (choice == 1)
        val = DPLL_1(params->clause);
    else if (choice == 2)
        val = DPLL_2(params->clause);
    end = ((float)(clock() - start));
    printf("DPLL�������!\n");
    printf("��ʱ��%.1f ms\n", end);

    int vals = pthread_cancel(*params->thread);
    if (vals != 0)
    {
        printf("��ֹ��ʱ�߳�ʧ�ܣ�\n");
        printf("������Ϊ��%d\n", val);
    }
    if (val == false)
    {
        printf("������������!\n");
        return (void *)"2";
    }
    printf("��ʼУ����...\n");
    check(params->clause);
    printf("У��ɹ���\n");

    printf("�Ƿ���������[Yes/No]\n");
    scanf("%s", string);
    if (strcmp(string, "Yes") == 0 || strcmp(string, "yes") == 0 || strcmp(string, "y") == 0)
    {
        printf("���Ϊ��\n");
        int count = 1, j = 1;
        for (int i = 1; i <= params->literalNum; i++)
        {
            if (value[number[i] - 1] == true)
            {
                printf("%4d ", number[i]);
                ans[j++] = number[i];
                count++;
            }

            if (count % 10 == 0)
            {
                printf("\n");
                count = 1;
            }
        }
        drawFinalPanel(ans, "Answer");
    }
    return (void *)"2";
}

/**
 * @brief: ��ʱ�߳�
 * @param {void} *arg
 * @return {*}
 */
void *count_thread(void *arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    pthread_t thread = (pthread_t)arg;
    clock_t start, end;

    start = clock();
    while (end < MAX_EXECUTE_TIME)
    {
        pthread_testcancel();
        end = (clock() - start) / CLOCKS_PER_SEC;
    }
    return (void *)"1";
}
