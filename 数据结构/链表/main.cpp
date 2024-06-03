#include "def.h"
#include "f.h"

int main() {
    LinkList L;
	LISTS Lists;
	Lists.length = 0;
	Lists.listsize = LISTINCREMENT;
    L = NULL;
	char file_name[40],list_name[20];
    int op = 1;
    int value, num, i;  //���溯������ֵ 
    while(op) {
        system("cls");	printf("\n\n");
        printf("      Menu for Linear Table On Sequence Structure \n");
        printf("-------------------------------------------------\n" );
        printf("    	  1. InitList       12.ListTraverse\n"       );
        printf("    	  2. DestroyList    13.SaveList\n"           );
        printf("    	  3. ClearList      14.LoadList\n"           );
        printf("    	  4. ListEmpty      15.reverseList\n"        );
        printf("    	  5. ListLength     16.RemoveNthFromEnd\n"   );
        printf("    	  6. GetElem        17.sortList\n"           );
        printf("          7. LocateElem     18.AddList\n"            );
        printf("          8. PriorElem      19.RemoveList\n"         );
        printf("          9. NextElem       20.LocateList\n"         );
        printf("          10.ListInsert     21.ListPrint\n"          );
        printf("          11.ListDelete     22.MutlListInsert\n"     );
		printf("          0. Exit           23.Delete\n"             );
        printf("-------------------------------------------------\n" );
        printf("    ��ѡ����Ĳ���[0~23]: "						      );

        scanf("%d",&op);
        switch(op) {
            case 1:
		value = InitList(L);
		if(value == OK) printf("�������ɹ�!\n");
		else printf("������ʧ��!\n");
		getchar();  getchar();
		break;
	   case 2:
		value = DestroyList(L);
		if(value == OK) printf("�������ٳɹ�!\n");
		else printf("��������ʧ��!\n");
		getchar(); getchar();
		break;
	   case 3:
	   	value = ClearList(L);
		if(value == OK) printf("��������Ԫ����ɾ��!\n");
		else printf("�������ʧ��!\n");
		getchar();getchar();
		break;
	   case 4:
	   	value = ListEmpty(L);
		if(value == OK) printf("����Ϊ��!\n");
		else if(value == INFEASIBLE) printf("��������!\n");
		else printf("����Ϊ��!\n");
		getchar();getchar();
		break;
	   case 5:
	   	value = ListLength(L);
		if(value == INFEASIBLE) printf("��������!\n");
		else printf("������Ϊ%d!\n",value);
		getchar();getchar();
		break;
	   case 6:
	   	 if(ListLength(L) != INFEASIBLE && ListLength(L) != 0)
	   	 printf("������Ҫ��ѯԪ�ص����!(1-%d)\n",ListLength(L)+1);
		 else {printf("�������ڻ�������Ϊ��!\n"); getchar();getchar(); break;}
		 scanf("%d",&i);
		 value = GetElem(L,i,num);
		 if(value == OK) printf("�Ѳ�ѯ����ӦԪ�أ�ֵΪ:%d\n",num);
		 else printf("��ȡԪ��ʧ��!\n");
		 getchar();getchar();
		 break;
	   case 7:
		 printf("��������Ҫ��ѯ������:");
		 scanf("%d",&num);
		 value = LocateElem(L,num);
		 if(value == INFEASIBLE) printf("��������!\n");
		 else if(value == ERROR) printf("δ���ҵ���ӦԪ��!\n");
		 else printf("%d�������е�λ��Ϊ%d!\n",num,value);
		 getchar();getchar();
		 break;
	   case 8:
		 printf("��������Ҫ��ѯ������!\n");
		 scanf("%d",&num);
		 value = PriorElem(L,num,i);
		 if(value == INFEASIBLE) printf("��������!\n");
		 else if(value == ERROR) printf("ǰ��Ԫ�ز�����!\n");
		 else printf("�Ѳ��ҵ�ǰ��Ԫ�� %d\n",i);
		 getchar();getchar();
		 break;
	   case 9:
		 printf("��������Ҫ��ѯ������!\n");
		 scanf("%d",&num);
		 value = NextElem(L,num,i);
		 if(value == INFEASIBLE) printf("��������!\n");
		 else if(value == ERROR) printf("����Ԫ�ز�����!\n");
		 else printf("�Ѳ��ҵ�����Ԫ�� %d\n",i); 
		 getchar();getchar();
		 break;
	   case 10:
		 printf("������Ҫ�����Ԫ��!\n");
		 scanf("%d",&num);
		 printf("������Ҫ�����λ��!\n");
		 scanf("%d",&i);
		 value = ListInsert(L,i,num);
		 if(value == INFEASIBLE) printf("��������!\n");
		 else if(value == ERROR) printf("����Ԫ��ʧ��!\n");
		 else printf("Ԫ��%d���뵽�����%d��Ԫ��֮ǰ!\n",num,i);    
		 getchar();getchar();
		 break;
	   case 11:
		 printf("������Ҫɾ����λ��!\n");
		 scanf("%d",&i);
		 value = ListDelete(L,i,num);
		 if(value == INFEASIBLE) printf("��������!\n");
		 else if(value == ERROR) printf("ɾ��Ԫ��ʧ��!\n");
		 else printf("��ɾ�������%d��Ԫ�� %d!\n",i,num);
		 getchar();getchar();
		 break;
	   case 12:  
		 if(ListTraverse(L) == INFEASIBLE) printf("��������!\n");
		 getchar();getchar();
		 break;
	   case 13:
	   	 printf("������Ҫ������ļ������ּ������·��!\n");
		 scanf("%s",file_name);
		 value = SaveList(L,file_name);
		 if(value == INFEASIBLE) printf("���Ա�����!\n");
		 else if(value == OK) printf("����ɹ�!\n");
		 getchar();getchar();
		 break;
	   case 14:
		 printf("������Ҫ��ȡ�ļ������ּ������·��!\n");
		 scanf("%s",file_name);
		 value = LoadList(L,file_name);
		 if(value == INFEASIBLE) printf("���Ա�����!\n");
		 else if(value == OK) printf("��ȡ�ɹ�!\n");
		 getchar();getchar();
		 break;
	   case 15:
	   	 value = reverseList(L);
		 if(value == INFEASIBLE) printf("��������!\n");
		 else printf("��ת�ɹ�!\n");
		 getchar();getchar();
		 break;
	   case 16:
	     printf("������Ҫɾ���Ľ���λ��,����Ҫ����λ��!\n");
		 scanf("%d",&i);
		 value = RemoveNthFromEnd(L,i);
		 if(value == INFEASIBLE) printf("��������!\n");
		 else if(value == ERROR) printf("��������Χ!\n");
		 else printf("ɾ���ɹ�!\n");
		 getchar();getchar();
		 break;
	   case 17:
	   	 value = sortList(L);
		 if(value == INFEASIBLE) printf("��������!\n");
		 else printf("��ת�ɹ�!\n");
		 getchar();getchar();
		 break;
	   case 18:
	     printf("�������������������!\n");
		 scanf("%s",list_name);
		 value = AddList(Lists,list_name);
		 if(value == OK) printf("���������ɳɹ�!\n");
		 else printf("����ʧ��!\n");
		 getchar();getchar();
		 break;
	   case 19:
	     printf("������Ҫɾ�����������!\n");
		 scanf("%s",list_name);
		 value = RemoveList(Lists,list_name);
		 if(value == OK) printf("ɾ������ɹ�!\n");
		 else printf("ɾ������ʧ��!\n");
		 getchar();getchar();
		 break;
	   case 20:
	     printf("������Ҫ�������������!\n");
		 scanf("%s",list_name);
		 value = LocateList(Lists,list_name);
		 if(value == ERROR) printf("û�в��ҵ�������!\n");
		 else printf("�������ڵ�%dλ!\n",value);
		 getchar();getchar();
		 break;
	   case 21:
	     printf("������Ҫ��ӡ��˳��������!\n");
		 scanf("%s",list_name);
		 if(ListPrint(Lists,list_name) == FALSE) printf("û�в��ҵ��ñ�!\n");
		 getchar();getchar();
		 break;
       case 22:
	     printf("������Ҫ��ӵ����������!\n");
		 scanf("%s",list_name);
		 printf("������Ҫ�����Ԫ��!\n");
		 scanf("%d",&num);
		 printf("������Ҫ�����λ��!\n");
		 scanf("%d",&i);
		 value = MutlListInsert(Lists,list_name,i,num);
		 if(value == INFEASIBLE) printf("��������!\n");
		 else if(value == ERROR) printf("����Ԫ��ʧ��!\n");
		 else printf("Ԫ��%d���뵽�����%d��Ԫ��֮ǰ!\n",num,i);    
		 getchar();getchar();
		 break;
	   case 23:
	     printf("������Ҫɾ�������������!\n");
	     scanf("%s",list_name);
	     printf("������Ҫɾ���������Ԫ�ص�λ��!\n");
	     scanf("%d",&i);
	   	 value = Delete(Lists,list_name,i,num);
		 if(value == INFEASIBLE) printf("��������!\n");
		 else if(value == ERROR) printf("ɾ��Ԫ��ʧ��!\n");
		 else printf("��ɾ�������%d��Ԫ�� %d!\n",i,num);
		   getchar();getchar();
		 break;
        }
	}
	printf("��ӭ�´���ʹ�ñ�ϵͳ��\n");
    return 0;
}
