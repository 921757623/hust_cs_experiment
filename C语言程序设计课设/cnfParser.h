/*
 * @Description: ����cnf�ļ�
 * @version: 1.0.0
 * @Author: yrp
 * @Date: 2022-08-30 10:17:28
 * @LastEditTime: 2022-10-06 10:50:43
 */

/**
 * @brief: ���������Ӿ�
 * @param {ClauseList} &clause
 * @param {FILE} *fp
 * @return {*}
 */
status createClause(ClauseList &clause, FILE *fp)
{
    int literal;
    char buffer[10];
    //��ʼ��
    clause->next = (ClauseList)malloc(sizeof(ClauseNode));
    clause = clause->next;
    clause->nodeNum = 0;
    clause->head = (LiteralList)malloc(sizeof(LiteralNode));
    clause->head->next = (LiteralList)malloc(sizeof(LiteralNode));
    LiteralList literalHead = clause->head;
    literalHead->next = NULL;

    while (2233)
    {
        //��ȡ
        fscanf(fp, "%s", buffer);
        literal = atoi(buffer);
        //��ֵ
        if (literal == 0)
            break;
        //������
        literalHead->next = (LiteralList)malloc(sizeof(LiteralNode));
        literalHead = literalHead->next;
        literalHead->data = literal;
        if (literal > 0)
            counts[literal].plus++;
        else
            counts[-literal].minus++;
        clause->nodeNum++;
    }
    literalHead->next = NULL;
    return OK;
}

/**
 * @brief: ��ȡcnf�ļ�
 * @param {ClauseList} &clause
 * @param {int} &literalNum
 * @param {char} fileName
 * @return {*}
 */
status fileInput(ClauseList &clause, int &literalNum, char fileName[])
{
    FILE *fp = NULL;
    clause = (ClauseList)malloc(sizeof(ClauseNode));
    ClauseList headp = clause;

    char buffer[500];
    int clauseNum;

    printf("������Ҫ�򿪵��ļ���: ");
    scanf("%s", fileName);
    if ((fp = fopen(fileName, "r")) == NULL)
    {
        printf("���ļ�ʧ��!\n");
        return ERROR;
    }

    do
    {
        fscanf(fp, "%s", buffer);
    } while (strcmp(buffer, "cnf"));
    fscanf(fp, "%s", buffer);
    literalNum = atoi(buffer); //��ȡ������Ԫ��
    fscanf(fp, "%s", buffer);
    clauseNum = atoi(buffer); //��ȡ�Ӿ�����

    for (int i = 0; i < clauseNum; i++) //д�����ݵ�����
    {
        createClause(clause, fp);
    }
    clause->next = NULL;

    clause = headp;
    fclose(fp);
    return OK;
}

/**
 * @brief: ����Ҫ������������cnf�ļ�
 * @return {*}
 */
status generateBaseCnfFile()
{
    FILE *fp = NULL;
    if ((fp = fopen("baseCnf.cnf", "w")) == NULL)
        return ERROR;
    fprintf(fp, "p cnf %d %d\n", BASE_LITERAL_NUM, BASE_CLAUSE_NUM + data[0]);
    //��ʾ��
    for (int i = 1; data[i] != 0; i++)
        fprintf(fp, "%d 0\n", data[i]);
    for (int t = 1; t <= 2; t++)
    {
        //��
        for (int i = 1; i <= 9; i++)
        {
            for (int j = 1; j <= 9; j++)
            {
                for (int k = 1; k <= 9; k++)
                    fprintf(fp, "%d%d%d%d ", t, i, k, j);
                fprintf(fp, "0\n");
            }
            for (int n = 1; n <= 9; n++)
                for (int j = 1; j <= 9; j++)
                    for (int k = 1 + j; k <= 9; k++)
                        fprintf(fp, "-%d%d%d%d -%d%d%d%d 0\n", t, i, j, n, t, i, k, n);
        }
        //��
        for (int i = 1; i <= 9; i++)
        {
            for (int j = 1; j <= 9; j++)
            {
                for (int k = 1; k <= 9; k++)
                    fprintf(fp, "%d%d%d%d ", t, k, i, j);
                fprintf(fp, "0\n");
            }
            for (int n = 1; n <= 9; n++)
                for (int j = 1; j <= 9; j++)
                    for (int k = 1 + j; k <= 9; k++)
                        fprintf(fp, "-%d%d%d%d -%d%d%d%d 0\n", t, j, i, n, t, k, i, n);
        }
        //�Ź���
        int a[10] = {0};
        for (int n = 0; n < 9; n++)
        {
            memset(a, 0, 10);
            for (int i = 1; i <= 9; i++)
            {
                int count = 1;
                for (int j = 1; j <= 3; j++)
                    for (int k = 1; k <= 3; k++)
                    {
                        fprintf(fp, "%d%d%d%d ", t, j + ((int)(n / 3) * 3), k + ((n % 3) * 3), i);
                        a[count++] = (j + ((int)(n / 3) * 3)) * 10 + k + ((n % 3) * 3);
                    }
                fprintf(fp, "0\n");
                for (int j = 2 + n; j <= 9; j++)
                    fprintf(fp, "-%d%d%d%d -%d%d%d%d 0\n", t, a[n + 1] / 10, a[n + 1] % 10, i, t, a[j] / 10, a[j] % 10, i);
            }
        }

        for (int i = 1; i <= 9; i++)
            for (int j = 1; j <= 9; j++)
                for (int n = 1; n <= 9; n++)
                    for (int g = 1 + n; g <= 9; g++)
                        fprintf(fp, "-%d%d%d%d -%d%d%d%d 0\n", t, i, j, n, t, i, j, g);
    }
    //�ص�����
    for (int j = 1; j <= 3; j++)
        for (int k = 1; k <= 3; k++)
            for (int t = 1; t <= 9; t++)
            {
                fprintf(fp, "-1%d%d%d 2%d%d%d 0\n", j + 6, k + 6, t, j, k, t);
                fprintf(fp, "1%d%d%d -2%d%d%d 0\n", j + 6, k + 6, t, j, k, t);
            }
    for (int n = 0; n < 9; n++)
        for (int i = 1; i <= 9; i++)
            for (int j = 1; j <= 3; j++)
                for (int k = 1; k <= 3; k++)
                    for (int g = 1 + i; g <= 9; g++)
                    {
                        fprintf(fp, "-%d%d%d%d -%d%d%d%d 0\n", 1, j + ((int)(n / 3) * 3), k + ((n % 3) * 3), i, 1, j + ((int)(n / 3) * 3), k + ((n % 3) * 3), g);
                    }
    for (int n = 1; n < 9; n++)
        for (int i = 1; i <= 9; i++)
            for (int j = 1; j <= 3; j++)
                for (int k = 1; k <= 3; k++)
                    for (int g = 1 + i; g <= 9; g++)
                    {
                        fprintf(fp, "-%d%d%d%d -%d%d%d%d 0\n", 2, j + ((int)(n / 3) * 3), k + ((n % 3) * 3), i, 2, j + ((int)(n / 3) * 3), k + ((n % 3) * 3), g);
                    }
    fclose(fp);
    return OK;
}

/**
 * @brief: ����Ҫ�ڶ��ĳ�ʼ����������cnf�ļ�����
 * @param {int} ans
 * @return {*}
 */
status generateCnfFile(int ans[])
{
    FILE *fp = NULL;
    if ((fp = fopen("baseCnf.cnf", "w")) == NULL)
        return ERROR;
    fprintf(fp, "p cnf %d %d\n", BASE_LITERAL_NUM, BASE_CLAUSE_NUM + ans[0]);
    //��ʾ��
    for (int i = 1; ans[i] != 0; i++)
        fprintf(fp, "%d 0\n", ans[i]);
    for (int t = 1; t <= 2; t++)
    {
        //��
        for (int i = 1; i <= 9; i++)
        {
            for (int j = 1; j <= 9; j++)
            {
                for (int k = 1; k <= 9; k++)
                    fprintf(fp, "%d%d%d%d ", t, i, k, j);
                fprintf(fp, "0\n");
            }
            for (int n = 1; n <= 9; n++)
                for (int j = 1; j <= 9; j++)
                    for (int k = 1 + j; k <= 9; k++)
                        fprintf(fp, "-%d%d%d%d -%d%d%d%d 0\n", t, i, j, n, t, i, k, n);
        }
        //��
        for (int i = 1; i <= 9; i++)
        {
            for (int j = 1; j <= 9; j++)
            {
                for (int k = 1; k <= 9; k++)
                    fprintf(fp, "%d%d%d%d ", t, k, i, j);
                fprintf(fp, "0\n");
            }
            for (int n = 1; n <= 9; n++)
                for (int j = 1; j <= 9; j++)
                    for (int k = 1 + j; k <= 9; k++)
                        fprintf(fp, "-%d%d%d%d -%d%d%d%d 0\n", t, j, i, n, t, k, i, n);
        }
        //�Ź���
        int a[10] = {0};
        for (int n = 0; n < 9; n++)
        {
            memset(a, 0, 10);
            for (int i = 1; i <= 9; i++)
            {
                int count = 1;
                for (int j = 1; j <= 3; j++)
                    for (int k = 1; k <= 3; k++)
                    {
                        fprintf(fp, "%d%d%d%d ", t, j + ((int)(n / 3) * 3), k + ((n % 3) * 3), i);
                        a[count++] = (j + ((int)(n / 3) * 3)) * 10 + k + ((n % 3) * 3);
                    }
                fprintf(fp, "0\n");
                for (int j = 2 + n; j <= 9; j++)
                    fprintf(fp, "-%d%d%d%d -%d%d%d%d 0\n", t, a[n + 1] / 10, a[n + 1] % 10, i, t, a[j] / 10, a[j] % 10, i);
            }
        }

        for (int i = 1; i <= 9; i++)
            for (int j = 1; j <= 9; j++)
                for (int n = 1; n <= 9; n++)
                    for (int g = 1 + n; g <= 9; g++)
                        fprintf(fp, "-%d%d%d%d -%d%d%d%d 0\n", t, i, j, n, t, i, j, g);
    }
    //�ص�����
    for (int j = 1; j <= 3; j++)
        for (int k = 1; k <= 3; k++)
            for (int t = 1; t <= 9; t++)
            {
                fprintf(fp, "-1%d%d%d 2%d%d%d 0\n", j + 6, k + 6, t, j, k, t);
                fprintf(fp, "1%d%d%d -2%d%d%d 0\n", j + 6, k + 6, t, j, k, t);
            }
    for (int n = 0; n < 9; n++)
        for (int i = 1; i <= 9; i++)
            for (int j = 1; j <= 3; j++)
                for (int k = 1; k <= 3; k++)
                    for (int g = 1 + i; g <= 9; g++)
                    {
                        fprintf(fp, "-%d%d%d%d -%d%d%d%d 0\n", 1, j + ((int)(n / 3) * 3), k + ((n % 3) * 3), i, 1, j + ((int)(n / 3) * 3), k + ((n % 3) * 3), g);
                    }
    for (int n = 1; n < 9; n++)
        for (int i = 1; i <= 9; i++)
            for (int j = 1; j <= 3; j++)
                for (int k = 1; k <= 3; k++)
                    for (int g = 1 + i; g <= 9; g++)
                    {
                        fprintf(fp, "-%d%d%d%d -%d%d%d%d 0\n", 2, j + ((int)(n / 3) * 3), k + ((n % 3) * 3), i, 2, j + ((int)(n / 3) * 3), k + ((n % 3) * 3), g);
                    }
    fclose(fp);
    return OK;
}

/**
 * @brief: ��������cnf�ļ�
 * @param {ClauseList} &clause
 * @param {int} &literalNum
 * @param {char} fileName
 * @return {*}
 */
status loadCnfFile(ClauseList &clause, int &literalNum, char fileName[])
{
    FILE *fp = NULL;
    clause = (ClauseList)malloc(sizeof(ClauseNode));
    ClauseList headp = clause;

    char buffer[500];
    int clauseNum;

    if ((fp = fopen(fileName, "r")) == NULL)
    {
        printf("���ļ�ʧ��!\n");
        return ERROR;
    }

    do
    {
        fscanf(fp, "%s", buffer);
    } while (strcmp(buffer, "cnf"));
    fscanf(fp, "%s", buffer);
    literalNum = atoi(buffer); //��ȡ������Ԫ��
    fscanf(fp, "%s", buffer);
    clauseNum = atoi(buffer); //��ȡ�Ӿ�����

    for (int i = 0; i < clauseNum; i++) //д�����ݵ�����
    {
        createClause(clause, fp);
    }
    clause->next = NULL;

    clause = headp;
    fclose(fp);
    return OK;
}

/**
 * @brief: ���ɽ���ļ�
 * @param {int} literalNum
 * @param {char} fileName
 * @param {boolean} isSat
 * @param {float} runTime
 * @return {*}
 */
status generateResFile(int literalNum, char fileName[], boolean isSat, float runTime)
{
    FILE *fp = NULL;
    int i = -1;
    while (fileName[++i] != '.')
        ;
    fileName[i] = '\0';
    strcat(fileName, ".res");
    if ((fp = fopen(fileName, "w")) == NULL)
    {
        printf("�������ļ�ʧ�ܣ�\n");
        return ERROR;
    }
    fprintf(fp, "s %d\n", isSat);
    fprintf(fp, "v");
    for (int i = 1; i <= literalNum; i++)
    {
        if (value[i - 1] == true)
            fprintf(fp, "%5d ", i);
        else
            fprintf(fp, "%5d ", -i);
        // if (i % 8 == 0)
        //     fprintf(fp, "\n");
    }
    fprintf(fp, "\nt %g ms", runTime);
    fclose(fp);
    return OK;
}