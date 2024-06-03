/*
 * @Description: 主函数
 * @version: 1.0.0
 * @Author: yrp
 * @Date: 2022-09-07 23:14:34
 * @LastEditTime: 2022-10-03 21:43:55
 */

#include "def.h"
#include "cnfParser.h"
#include "DPLL.h"
#include "sudo.h"

int main()
{
    void *response; //接受线程返回值
    int op = 1, literalNum, val = 1;
    char fileName[200];
    char string[5];
    ClauseList clause;
    pthread_t main_work, count;
    parameter params;
    clock_t start;
    float end;

    while (op)
    {
        system("cls");
        printf("\n\n");
        printf("         基于SAT的二进制数独游戏求解程序\n");
        printf("-------------------------------------------------\n");
        printf("     1. SAT算例求解       2. 数独\n");
        printf("     0. Exit\n");
        printf("-------------------------------------------------\n");
        printf("    请选择你的操作[0-2]: ");
        scanf("%d", &op);
        memset(value, false, 4000);
        memset(data, 0, 100);
        memset(counts, 0, 4000);
        switch (op)
        {
        case 1:
            printf("开始文件读取...\n");
            val = fileInput(clause, literalNum, fileName);
            if (val == OK)
                printf("文件读取成功！\n");
            else if (val == ERROR)
            {
                printf("文件读取失败！\n");
                getchar();
                getchar();
                break;
            }

            params.clause = clause;
            params.literalNum = literalNum;
            params.thread = &count;
            strcpy(params.fileName, fileName);

            // DPLL求解线程，规定时间内求解完毕会结束全部线程
            pthread_create(&main_work, NULL, main_thread_1, (void *)&params);
            // 计时线程，超时会结束全部线程
            pthread_create(&count, NULL, count_thread, (void *)&main_work);
            pthread_join(count, &response);
            if (response != (void *)0xdeadbeef)
            {
                val = atoi((char *)response);
                if (val == OK)
                {
                    pthread_cancel(main_work);
                    printf("已终止DPLL线程！\n");
                }
                printf("是否输出结果？[Yes/No]\n");
                scanf("%s", string);
                if (strcmp(string, "Yes") == 0 || strcmp(string, "yes") == 0 || strcmp(string, "y") == 0)
                    if (generateResFile(params.literalNum, params.fileName, -1, MAX_EXECUTE_TIME) == OK)
                        printf("文件生成成功！\n");
                    else
                        printf("文件生成失败！\n");
            }
            pthread_join(main_work, &response);
            getchar();
            getchar();
            break;
        case 2:
            strcpy(fileName, "baseCnf.cnf");
            printf("开始生成数独...\n");
            start = clock();
            intiGame();
            end = ((float)(clock() - start));
            printf("数独生成成功！\n");
            printf("用时：%.1f ms\n", end);

            generateBaseCnfFile();
            val = loadCnfFile(clause, literalNum, fileName);
            if (val == OK)
                printf("文件读取成功！\n");
            else if (val == ERROR)
            {
                printf("文件读取失败！\n");
                getchar();
                getchar();
                break;
            }

            params.clause = clause;
            params.literalNum = literalNum;
            params.thread = &count;

            // DPLL求解线程，规定时间内求解完毕会结束全部线程
            pthread_create(&main_work, NULL, main_thread_2, (void *)&params);
            // 计时线程，超时会结束全部线程
            pthread_create(&count, NULL, count_thread, (void *)&main_work);
            pthread_join(count, &response);
            if (response != (void *)0xdeadbeef)
            {
                val = atoi((char *)response);
                if (val == OK)
                {
                    pthread_cancel(main_work);
                    printf("已终止DPLL线程！\n");
                }
            }
            pthread_join(main_work, &response);
            getchar();
            getchar();
            break;
        }
    }
    printf("欢迎使用本系统！\n");
    return 0;
}
