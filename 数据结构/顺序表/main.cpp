//UTF-8
#include "def.h"
#include "f.h"

int main()
{

SqList L;
LISTS Lists;
Lists.length = 0;
Lists.listsize = LISTINCREMENT;
L.elem = NULL;
int op = 1,value,num,i;  //value储存函数返回值
char file_name[40],list_name[20];

while(op)
{
	system("cls");	printf("\n\n");
	printf("      Menu for Linear Table On Sequence Structure \n");
	printf("-------------------------------------------------\n" );
	printf("    	  1. InitList       7. LocateElem\n"         );
	printf("    	  2. DestroyList    8. PriorElem\n"          );
	printf("    	  3. ClearList      9. NextElem \n"          );
	printf("    	  4. ListEmpty      10.ListInsert\n"         );
	printf("    	  5. ListLength     11.ListDelete\n"         );
	printf("    	  6. GetElem        12.ListTraverse\n"       );
	printf("          13.SaveList       14.LoadList\n"           );
	printf("          15.AddList        16.RemoveList\n"         );
	printf("          17.LocateList     18.reverseList\n"        );
	printf("          19.sortList       20.MutlListInsert\n"     );
	printf("          21.ListPrint      22.MaxSubArray\n"        );
	printf("    	  0. Exit\n"								 );
	printf("-------------------------------------------------\n" );
	printf("    请选择你的操作[0~19]:"						      );
	
	scanf("%d",&op);
    switch(op)
	{
	   case 1:
		value = InitList(L);
		if(value == OK) printf("线性表创建成功!\n");
		else printf("线性表创建失败!\n");
		getchar();  getchar();
		break;
	   case 2:
		value = DestroyList(L);
		if(value == OK) printf("线性表销毁成功!\n");
		else printf("线性表销毁失败!\n");
		getchar(); getchar();
		break;
	   case 3:
	   	value = ClearList(L);
		if(value == OK) printf("线性表所有元素已删除!\n");
		else printf("清空线性表失败!\n");
		getchar();getchar();
		break;
	   case 4:
	   	value = ListEmpty(L);
		if(value == OK) printf("线性表为空!\n");
		else if(value == INFEASIBLE) printf("线性表不存在!\n");
		else printf("线性表不为空!\n");
		getchar();getchar();
		break;
	   case 5:
	   	value = ListLength(L);
		if(value == INFEASIBLE) printf("线性表不存在!\n");
		else printf("线性表长度为%d!\n",value);
		getchar();getchar();
		break;
	   case 6:
	   	 if(ListLength(L) != INFEASIBLE && ListLength(L) != 0)
	   	 printf("请输入要查询元素的序号!(1-%d)\n",ListLength(L)+1);
		 else {printf("线性表不存在或者线性表为空!\n"); getchar();getchar(); break;}
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
		 if(value == INFEASIBLE) printf("线性表不存在!\n");
		 else if(value == ERROR) printf("未查找到相应元素!\n");
		 else printf("%d在线性表中的位置为%d!\n",num,value);
		 getchar();getchar();
		 break;
	   case 8:
		 printf("请输入想要查询的内容!\n");
		 scanf("%d",&num);
		 value = PriorElem(L,num,i);
		 if(value == INFEASIBLE) printf("线性表不存在!\n");
		 else if(value == ERROR) printf("前驱元素不存在!\n");
		 else printf("已查找到前驱元素 %d\n",i);
		 getchar();getchar();
		 break;
	   case 9:
		 printf("请输入想要查询的内容!\n");
		 scanf("%d",&num);
		 value = NextElem(L,num,i);
		 if(value == INFEASIBLE) printf("线性表不存在!\n");
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
		 if(value == INFEASIBLE) printf("线性表不存在!\n");
		 else if(value == ERROR) printf("插入元素失败!\n");
		 else printf("元素%d插入到线性表第%d个元素之前!\n",num,i);    
		 getchar();getchar();
		 break;
	   case 11:
		 printf("请输入要删除的位置!\n");
		 scanf("%d",&i);
		 value = ListDelete(L,i,num);
		 if(value == INFEASIBLE) printf("线性表不存在!\n");
		 else if(value == ERROR) printf("删除元素失败!\n");
		 else printf("已删除线性表第%d个元素 %d!\n",i,num);
		 getchar();getchar();
		 break;
	   case 12:  
		 if(!ListTraverse(L)) printf("线性表是空表!\n");
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
	     printf("请输入新增线性表的名字!\n");
		 scanf("%s",list_name);
		 value = AddList(Lists,list_name);
		 if(value == OK) printf("新线性表生成成功!\n");
		 else printf("创建失败!\n");
		 getchar();getchar();
		 break;
	   case 16:
	     printf("请输入要删除线性表的名字!\n");
		 scanf("%s",list_name);
		 value = RemoveList(Lists,list_name);
		 if(value == OK) printf("删除线性表成功!\n");
		 else printf("删除线性表失败!\n");
		 getchar();getchar();
		 break;
	   case 17:
	     printf("请输入要查找线性表的名字!\n");
		 scanf("%s",list_name);
		 value = LocateList(Lists,list_name);
		 if(value == ERROR) printf("没有查找到该线性表!\n");
		 else printf("该线性表在第%d位!\n",value);
		 getchar();getchar();
		 break;
	   case 18:
	   	 value = reverseList(L);
		 if(value == INFEASIBLE) printf("线性表不存在!\n");
		 else printf("线性表翻转成功!\n");
		 getchar();getchar();
		 break;
	   case 19:
	   	 value = sortList(L);
		 if(value == INFEASIBLE) printf("线性表不存在!\n");
		 else printf("排序成功!\n");
		 getchar();getchar();
		 break;
	   case 20:
	     printf("请输入要添加的顺序表的名字!\n");
		 scanf("%s",list_name);
		 printf("请输入要插入的元素!\n");
		 scanf("%d",&num);
		 printf("请输入要插入的位置!\n");
		 scanf("%d",&i);
		 value = MutlListInsert(Lists,list_name,i,num);
		 if(value == INFEASIBLE) printf("线性表不存在!\n");
		 else if(value == ERROR) printf("插入元素失败!\n");
		 else printf("元素%d插入到线性表第%d个元素之前!\n",num,i);    
		 getchar();getchar();
		 break;
	   case 21:
	     printf("请输入要打印的顺序表的名字!\n");
		 scanf("%s",list_name);
		 if(!ListPrint(Lists,list_name)) printf("线性表是空表!\n");
		 getchar();getchar();
		 break;
	   case 22:
	   printf("请输入要删除的顺序表的名字!\n");
	   scanf("%s",list_name);
	   printf("请输入要删除的顺序表的元素的位置!\n");
	   scanf("%d",i);
	   	value = Delete(Lists,list_name,i);
		 if(value == INFEASIBLE) printf("线性表不存在!\n");
		 else if(value == ERROR) printf("删除元素失败!\n");
		 else printf("已删除线性表第%d个元素 %d!\n",i,num);
		   getchar();getchar();
		 break;
	   case 23:
	     value = MaxsubArray(L);
		 if(value == INFEASIBLE) 
		 printf("线性表不存在!\n");
		 else printf("%d\n",value);
		 getchar();getchar();
		 break;
	   case 0:
         break;
	}//end of switch
  }//end of while
printf("欢迎下次再使用本系统！\n");
return 0;
}
