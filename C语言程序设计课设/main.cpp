/*
 * @Description: ������
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
    void *response; //�����̷߳���ֵ
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
        printf("         ����SAT�Ķ�����������Ϸ������\n");
        printf("-------------------------------------------------\n");
        printf("     1. SAT�������       2. ����\n");
        printf("     0. Exit\n");
        printf("-------------------------------------------------\n");
        printf("    ��ѡ����Ĳ���[0-2]: ");
        scanf("%d", &op);
        memset(value, false, 4000);
        memset(data, 0, 100);
        memset(counts, 0, 4000);
        switch (op)
        {
        case 1:
            printf("��ʼ�ļ���ȡ...\n");
            val = fileInput(clause, literalNum, fileName);
            if (val == OK)
                printf("�ļ���ȡ�ɹ���\n");
            else if (val == ERROR)
            {
                printf("�ļ���ȡʧ�ܣ�\n");
                getchar();
                getchar();
                break;
            }

            params.clause = clause;
            params.literalNum = literalNum;
            params.thread = &count;
            strcpy(params.fileName, fileName);

            // DPLL����̣߳��涨ʱ���������ϻ����ȫ���߳�
            pthread_create(&main_work, NULL, main_thread_1, (void *)&params);
            // ��ʱ�̣߳���ʱ�����ȫ���߳�
            pthread_create(&count, NULL, count_thread, (void *)&main_work);
            pthread_join(count, &response);
            if (response != (void *)0xdeadbeef)
            {
                val = atoi((char *)response);
                if (val == OK)
                {
                    pthread_cancel(main_work);
                    printf("����ֹDPLL�̣߳�\n");
                }
                printf("�Ƿ���������[Yes/No]\n");
                scanf("%s", string);
                if (strcmp(string, "Yes") == 0 || strcmp(string, "yes") == 0 || strcmp(string, "y") == 0)
                    if (generateResFile(params.literalNum, params.fileName, -1, MAX_EXECUTE_TIME) == OK)
                        printf("�ļ����ɳɹ���\n");
                    else
                        printf("�ļ�����ʧ�ܣ�\n");
            }
            pthread_join(main_work, &response);
            getchar();
            getchar();
            break;
        case 2:
            strcpy(fileName, "baseCnf.cnf");
            printf("��ʼ��������...\n");
            start = clock();
            intiGame();
            end = ((float)(clock() - start));
            printf("�������ɳɹ���\n");
            printf("��ʱ��%.1f ms\n", end);

            generateBaseCnfFile();
            val = loadCnfFile(clause, literalNum, fileName);
            if (val == OK)
                printf("�ļ���ȡ�ɹ���\n");
            else if (val == ERROR)
            {
                printf("�ļ���ȡʧ�ܣ�\n");
                getchar();
                getchar();
                break;
            }

            params.clause = clause;
            params.literalNum = literalNum;
            params.thread = &count;

            // DPLL����̣߳��涨ʱ���������ϻ����ȫ���߳�
            pthread_create(&main_work, NULL, main_thread_2, (void *)&params);
            // ��ʱ�̣߳���ʱ�����ȫ���߳�
            pthread_create(&count, NULL, count_thread, (void *)&main_work);
            pthread_join(count, &response);
            if (response != (void *)0xdeadbeef)
            {
                val = atoi((char *)response);
                if (val == OK)
                {
                    pthread_cancel(main_work);
                    printf("����ֹDPLL�̣߳�\n");
                }
            }
            pthread_join(main_work, &response);
            getchar();
            getchar();
            break;
        }
    }
    printf("��ӭʹ�ñ�ϵͳ��\n");
    return 0;
}
