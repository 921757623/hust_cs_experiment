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
    int value, num, i;  //保存函数返回值 
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
        printf("    请选择你的操作[0~23]: "						      );

        scanf("%d",&op);
        switch(op) {
            case 1:
		value = InitList(L);
		if(value == OK) printf("链表创建成功!\n");
		else printf("链表创建失败!\n");
		getchar();  getchar();
		break;
	   case 2:
		value = DestroyList(L);
		if(value == OK) printf("链表销毁成功!\n");
		else printf("链表销毁失败!\n");
		getchar(); getchar();
		break;
	   case 3:
	   	value = ClearList(L);
		if(value == OK) printf("链表所有元素已删除!\n");
		else printf("清空链表失败!\n");
		getchar();getchar();
		break;
	   case 4:
	   	value = ListEmpty(L);
		if(value == OK) printf("链表为空!\n");
		else if(value == INFEASIBLE) printf("链表不存在!\n");
		else printf("链表不为空!\n");
		getchar();getchar();
		break;
	   case 5:
	   	value = ListLength(L);
		if(value == INFEASIBLE) printf("链表不存在!\n");
		else printf("链表长度为%d!\n",value);
		getchar();getchar();
		break;
	   case 6:
	   	 if(ListLength(L) != INFEASIBLE && ListLength(L) != 0)
	   	 printf("请输入要查询元素的序号!(1-%d)\n",ListLength(L)+1);
		 else {printf("链表不存在或者链表为空!\n"); getchar();getchar(); break;}
		 scanf("%d",&i);
		 value = GetElem(L,i,num);
		 if(value == OK) printf("已查询到相应元素，值为:%d\n",num);
		 else printf("获取元素失败!\n");
		 getchar();getchar();
		 break;
	   case 7:
		 printf("请输入想要查询的内容:");
		 scanf("%d",&num);
		 value = LocateElem(L,num);
		 if(value == INFEASIBLE) printf("链表不存在!\n");
		 else if(value == ERROR) printf("未查找到相应元素!\n");
		 else printf("%d在链表中的位置为%d!\n",num,value);
		 getchar();getchar();
		 break;
	   case 8:
		 printf("请输入想要查询的内容!\n");
		 scanf("%d",&num);
		 value = PriorElem(L,num,i);
		 if(value == INFEASIBLE) printf("链表不存在!\n");
		 else if(value == ERROR) printf("前驱元素不存在!\n");
		 else printf("已查找到前驱元素 %d\n",i);
		 getchar();getchar();
		 break;
	   case 9:
		 printf("请输入想要查询的内容!\n");
		 scanf("%d",&num);
		 value = NextElem(L,num,i);
		 if(value == INFEASIBLE) printf("链表不存在!\n");
		 else if(value == ERROR) printf("后驱元素不存在!\n");
		 else printf("已查找到后驱元素 %d\n",i); 
		 getchar();getchar();
		 break;
	   case 10:
		 printf("请输入要插入的元素!\n");
		 scanf("%d",&num);
		 printf("请输入要插入的位置!\n");
		 scanf("%d",&i);
		 value = ListInsert(L,i,num);
		 if(value == INFEASIBLE) printf("链表不存在!\n");
		 else if(value == ERROR) printf("插入元素失败!\n");
		 else printf("元素%d插入到链表第%d个元素之前!\n",num,i);    
		 getchar();getchar();
		 break;
	   case 11:
		 printf("请输入要删除的位置!\n");
		 scanf("%d",&i);
		 value = ListDelete(L,i,num);
		 if(value == INFEASIBLE) printf("链表不存在!\n");
		 else if(value == ERROR) printf("删除元素失败!\n");
		 else printf("已删除链表第%d个元素 %d!\n",i,num);
		 getchar();getchar();
		 break;
	   case 12:  
		 if(ListTraverse(L) == INFEASIBLE) printf("链表不存在!\n");
		 getchar();getchar();
		 break;
	   case 13:
	   	 printf("请输入要存入的文件的名字及其相对路径!\n");
		 scanf("%s",file_name);
		 value = SaveList(L,file_name);
		 if(value == INFEASIBLE) printf("线性表不存在!\n");
		 else if(value == OK) printf("储存成功!\n");
		 getchar();getchar();
		 break;
	   case 14:
		 printf("请输入要读取文件的名字及其相对路径!\n");
		 scanf("%s",file_name);
		 value = LoadList(L,file_name);
		 if(value == INFEASIBLE) printf("线性表不存在!\n");
		 else if(value == OK) printf("读取成功!\n");
		 getchar();getchar();
		 break;
	   case 15:
	   	 value = reverseList(L);
		 if(value == INFEASIBLE) printf("链表不存在!\n");
		 else printf("翻转成功!\n");
		 getchar();getchar();
		 break;
	   case 16:
	     printf("请输入要删除的结点的位置,必须要倒序位置!\n");
		 scanf("%d",&i);
		 value = RemoveNthFromEnd(L,i);
		 if(value == INFEASIBLE) printf("链表不存在!\n");
		 else if(value == ERROR) printf("超出链表范围!\n");
		 else printf("删除成功!\n");
		 getchar();getchar();
		 break;
	   case 17:
	   	 value = sortList(L);
		 if(value == INFEASIBLE) printf("链表不存在!\n");
		 else printf("翻转成功!\n");
		 getchar();getchar();
		 break;
	   case 18:
	     printf("请输入新增链表的名字!\n");
		 scanf("%s",list_name);
		 value = AddList(Lists,list_name);
		 if(value == OK) printf("新链表生成成功!\n");
		 else printf("创建失败!\n");
		 getchar();getchar();
		 break;
	   case 19:
	     printf("请输入要删除链表的名字!\n");
		 scanf("%s",list_name);
		 value = RemoveList(Lists,list_name);
		 if(value == OK) printf("删除链表成功!\n");
		 else printf("删除链表失败!\n");
		 getchar();getchar();
		 break;
	   case 20:
	     printf("请输入要查找链表的名字!\n");
		 scanf("%s",list_name);
		 value = LocateList(Lists,list_name);
		 if(value == ERROR) printf("没有查找到该链表!\n");
		 else printf("该链表在第%d位!\n",value);
		 getchar();getchar();
		 break;
	   case 21:
	     printf("请输入要打印的顺序表的名字!\n");
		 scanf("%s",list_name);
		 if(ListPrint(Lists,list_name) == FALSE) printf("没有查找到该表!\n");
		 getchar();getchar();
		 break;
       case 22:
	     printf("请输入要添加的链表的名字!\n");
		 scanf("%s",list_name);
		 printf("请输入要插入的元素!\n");
		 scanf("%d",&num);
		 printf("请输入要插入的位置!\n");
		 scanf("%d",&i);
		 value = MutlListInsert(Lists,list_name,i,num);
		 if(value == INFEASIBLE) printf("链表不存在!\n");
		 else if(value == ERROR) printf("插入元素失败!\n");
		 else printf("元素%d插入到链表第%d个元素之前!\n",num,i);    
		 getchar();getchar();
		 break;
	   case 23:
	     printf("请输入要删除的链表的名字!\n");
	     scanf("%s",list_name);
	     printf("请输入要删除的链表的元素的位置!\n");
	     scanf("%d",&i);
	   	 value = Delete(Lists,list_name,i,num);
		 if(value == INFEASIBLE) printf("链表不存在!\n");
		 else if(value == ERROR) printf("删除元素失败!\n");
		 else printf("已删除链表第%d个元素 %d!\n",i,num);
		   getchar();getchar();
		 break;
        }
	}
	printf("欢迎下次再使用本系统！\n");
    return 0;
}
