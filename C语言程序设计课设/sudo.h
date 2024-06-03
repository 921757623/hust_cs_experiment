/*
 * @Description: 数独读取及求解模块
 * @version: 1.0.0
 * @Author: yrp
 * @Date: 2022-09-03 09:44:50
 * @LastEditTime: 2022-09-08 15:37:13
 */
status generateData(int ans[])
{
    data[0] = 58;
    int i = 1, count = 1;
    while (count != 59)
    {
        if (ans[i] != 0)
            data[count++] = ans[i];
        i++;
    }
    return OK;
}

int convertData(int x, int y, int i) // x表示第x行，y表示第y列,k表示填入k，i表示第i个数独格局
{
    if (i == 1)
        return (x - 1) * 9 + y;
    else if (i == 2)
        return (x - 1) * 9 + y + 81;
}

status generateRandPanel(int ans[])
{
    int count = 0;
    srand(time(NULL));
    while (1)
    {
        int n1 = rand() % 9 + 1;
        int n2 = rand() % 9 + 1;
        if (ans[convertData(n1, n2, 1)] != 0)
        {
            ans[convertData(n1, n2, 1)] = 0;
            count++;
        }
        if (count == 50)
            break;
    }
    count = 0;
    while (1)
    {
        int n1 = rand() % 9 + 1;
        int n2 = rand() % 9 + 1;
        if (ans[convertData(n1, n2, 2)] != 0)
        {
            ans[convertData(n1, n2, 2)] = 0;
            count++;
        }
        if (count == 45)
            break;
    }
    return OK;
}

status intiGame()
{
    ClauseList clause;
    int literalNum;
    char fileName[2000] = "baseCnf.cnf";
    srand(time(NULL));
    int randomNum = rand() % 11;
    generateCnfFile(datasource[randomNum]);
    loadCnfFile(clause, literalNum, fileName);
    DPLL_1(clause);
    int count = 1, j = 1;
    int ans[200];
    for (int i = 1; i <= BASE_LITERAL_NUM; i++)
    {
        if (value[number[i] - 1] == true)
            ans[j++] = number[i];
    }
    generateRandPanel(ans);
    drawFinalPanel(ans, "Origin");
    generateData(ans);
    return OK;
}