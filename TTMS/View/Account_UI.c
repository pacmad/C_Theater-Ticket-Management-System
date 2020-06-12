#include "Account_UI.h"

#define Account_PAGE_SIZE 5

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <conio.h>
#include <ctype.h>
#include "../Common/common.h"
#include "../Common/List.h"
#include "../Service/Account.h"

int SysLogin(){
	Account_Srv_InitSys();
	system("cls");
	printf("\n"
		"　　	   へ　　　　　／|\n"
		"　　	  /＼7　　　 ∠＿/\n"
		"　	  /　│　　 ／　／\n"
		"　	 │　Z ＿,＜　／　　 /`ヽ\n"
		"　	 │　　　　　ヽ　　 /　　〉\n"
		"　	  Y　　　　　`　 /　　/\n"
		"　	 ｲ●　､　●　　⊂⊃〈　　/\n"
		"　	 ()　 へ　　　　|　＼〈\n"
		"　　	  >ｰ ､_　 ィ　 │ ／／\n"
		"　	  / へ　　 /　ﾉ＜| ＼＼\n"
		"　	  ヽ_ﾉ　　(_／　 │／／\n"
		"　　	  7　　　　　　　|／\n"
		"　　	  ＞―r￣￣`ｰ―＿\n");
	HEAD_banner;
	printf("Welcome to TTMS.\n");
	int count = 0;
	char userName[MAX_LEN];
	unsigned char passwd[MAX_LEN];


	do{
		if (count++ > 0){
			printf("Invalidated Input. Try Again!\n");
			printf("You have %d chance(s) only.\n", 5 - count);
		}
		printf("User Name:");
		scanf("%s", userName);
		printf("Password:");
		fflush(stdin);

		int i = 0;
		while (i < MAX_LEN){
			passwd[i] = getch();
			if (passwd[i] == 8){
				printf("\b \b");
				i -= 2;
			}
			else if (passwd[i] == '\r' || passwd[i] == '\n'){
				break;
			}
			else{
				printf("*");
				i++;
			}

			if (i < 0){
				i = 0;
				passwd[i] = '\0';
			}
		}

	} while (count < 5 && Account_Srv_Verify(userName, passwd) == 0);


	if (count >= 5){
		char choice;
		do{
			printf("Forget Password? Wanna Reset?\n");
			printf("[Y/y]es.\n[N/n]o.\n");			
			scanf("%c", &choice);
		} while (choice != 'n' && choice != 'N' && choice != 'y' && choice != 'Y');
		if (tolower(choice) == 'y'){
			Find_Account_UI_Mgt();
		}
		else {
			return 0;
		}
	}

	return 1;
}

// 找回密码 -- 输入用户名 密保 输入新密码
void Find_Account_UI_Mgt(){
	system("cls");
	account_t user;
	char username[MAX_LEN];
	char security[MAX_LEN];
	int count = 0;
	do{
		if (count > 0){
			printf("There's no user like this, try again.  ('#' for return.\n)");
		}
		printf("User Name:");
		scanf("%s", username);
		if (strcmp(username, "#") == 0){
			return;
		}
	} while (!Account_Srv_FetchByName(username, &user));


	count = 0;

	do{
		if (count > 0){
			printf("Wrong security code, try again.  ('#' for return.\n)");
		}
		printf("Security Code:");
		scanf("%s", security);
		if (strcmp("#", security) == 0){
			return;
		}
	} while (strcmp(security, user.secret_ans));

	printf("New Password:");
	int i = 0;
	while (i < MAX_LEN){
		user.passwd[i] = getch();
		if (user.passwd[i] == 8){
			printf("\b \b");
			i -= 2;
		}
		else if (user.passwd[i] == '\r' || user.passwd[i] == '\n'){
			break;
		}
		else{
			printf("*");
			i++;
		}

		if (i < 0){
			i = 0;
			user.passwd[i] = '\0';
		}
	}

	strcpy(user.passwd, base64_encode(user.passwd));

	Account_Srv_Modify(&user);

}

char Account_UI_Status2Char(account_type_t status)
{
	if (status == 0)
		return 'C';
	else if (status == 1)
		return 'M';
	else if (status == 2)
		return 'A';
}


void Account_UI_MgtEntry(){
	if (gl_CurUser.type != USER_ADMIN){
		printf("Permission Limit. Admin Only. Press any key to return.\n");
		getchar();
		return 0;
	}

	char choice;
	account_list_t head;
	account_node_t* tmp;
	Pagination_t paging;
	List_Init(head, account_node_t);
	paging.offset = 0;
	paging.pageSize = Account_PAGE_SIZE;

	paging.totalRecords = Account_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do{
		system("cls");
		HEAD_banner;
		printf("{A}dmin  |   {C}lerk    |   {M}anager\n");
		printf("=========================== Account Management ============================\n");
		printf("%4d %20s %20s %10s\n", "ID", "Username", "Plaintext Password", "User Type");
		END_banner;
		int i;
		Paging_ViewPage_ForEach(head, paging, account_node_t, tmp, i){
			printf("%4d %20s %20s %10s", tmp->data.id, tmp->data.username, tmp->data.passwd, Account_UI_Status2Char(tmp->data.type));
		}
		printf("------- Total Records:%2d ----------------------- Page %2d/%2d -----------\n",
			paging.totalRecords, Pageing_CurPage(paging),Pageing_TotalPages(paging));
		printf("[P]revPage | [N]extPage | [A]dd | [D]elete | [Q]uery | [M]odify | [R]eturn\n");
		END_banner;
		fflush(stdin);
		scanf("%c", &choice);

		switch (tolower(choice)){
		case 'p':
			system("cls");
			if (!Pageing_IsFirstPage(paging)){
				Paging_Locate_OffsetPage(head, paging, -1, account_node_t);
			}
			break;
		case 'n':
			if (!Pageing_IsLastPage(paging)){
				Paging_Locate_OffsetPage(head, paging, 1, account_node_t);
			}
			break;
		case 'a':
			if (!Account_UI_Add(head)){
				printf("Fail to add. Press any key to continue.\n");
				getchar();
			}
			else{
				printf("Add Successfully! Press any key to continue.\n");
				paging.totalRecords = Account_Srv_FetchAll(head);
				List_Paging(head, paging, account_node_t);
				getchar();
			}
			break;
		case 'd':
			char name[MAX_LEN];
			printf("User Name:");
			scanf("%s", name);
			if (!Account_UI_Delete(head, name)){
				printf("Fail to Delete. Press any key to continue.\n");
				getchar();
			}
			else{
				printf("Delete Successfully! Press any key to continue.\n");
				getchar();
			}
			break;
		case 'q':
			char name[MAX_LEN];
			printf("User Name:");
			scanf("%s", name);
			if (!Account_UI_Query(head, name)){
				printf("User does not exist. Press any key to continue.");
				getchar();
			}
			else{
				printf("Press any key to continue.");
				getchar();
			}
			break;
		case 'm':
			char name[MAX_LEN];
			printf("User Name:");
			scanf("%s", name);
			if (!Account_UI_Modify(head, name)){
				printf("Fail to Modify. Press any key to continue.\n");
				getchar();
			}
			else{
				paging.totalRecords = Account_Srv_FetchAll(head);
				List_Paging(head, paging, account_node_t);
				printf("Modify Successfully! Press any key to continue.\n");
				getchar();
			}
			break;
		}
	} while (tolower(choice) != 'r');
	List_Destroy(head, account_node_t);
}

int Account_UI_Add(account_list_t list){
	account_t newUser;

	while (1){
		system("cls");
		HEAD_banner;
		printf("=========================== Sign Up =================================");
		printf("User Name:");
		scanf("%s", newUser.username);
		if (Account_Srv_FindByUsrName(list, newUser.username)){
			printf("The username has already existed. [R]eturn or [T]ry again.\n");
			char choice;
			fflush(stdin);
			while (scanf("%c", &choice) && tolower(choice) != 'r' && tolower(choice) != 't'){
				fflush(stdin);
				printf("[R]eturn or [T]ry again\n");
			}
			if (tolower(choice) == 'r'){
				return 0;
			}
			else{
				continue;
			}
		}


		printf("User Password:");
		int i = 0;
		while (i < MAX_LEN){
			newUser.passwd[i] = getch();
			if (newUser.passwd[i] == 8){
				printf("\b \b");
				i -= 2;
			}
			else if (newUser.passwd[i] == '\r' || newUser.passwd[i] == '\n'){
				break;
			}
			else{
				printf("*");
				i++;
			}

			if (i < 0){
				i = 0;
				newUser.passwd[i] = '\0';
			}
		}
		strcpy(newUser.passwd, base64_encode(newUser.passwd));

		printf("User Type: [0]Clerk / [1]Manager / [2]Admin\n");
		fflush(stdin);
		while (scanf("%d", newUser.type) && newUser.type != 0 && newUser.type != 1 && newUser.type != 2){
			printf("[0]Clerk / [1]Manager / [2]Admin\n");
			fflush(stdin);
		}

		printf("Security Code:\n");
		fflush(stdin);
		scanf("%s", newUser.secret_ans);

		return Account_Srv_Add(&newUser);
	}
}

int Account_UI_Modify(account_list_t list, char userName[]){
	system("cls");
	account_node_t* tmp;
	tmp = Account_Srv_FindByUsrName(list, userName);
	if (tmp){
		printf("New Password:");
		int i = 0;
		while (i < MAX_LEN){
			tmp->data.passwd[i] = getch();
			if (tmp->data.passwd[i] == 8){
				printf("\b \b");
				i -= 2;
			}
			else if (tmp->data.passwd[i] == '\r' || tmp->data.passwd[i] == '\n'){
				break;
			}
			else{
				printf("*");
				i++;
			}

			if (i < 0){
				i = 0;
				tmp->data.passwd[i] = '\0';
			}
		}
		strcpy(tmp->data.passwd, base64_encode(tmp->data.passwd));
		return Account_Srv_Modify(tmp);
	}
	
	return 0;
}

int Account_UI_Delete(account_list_t list, char userName[]){
	system("cls");
	account_node_t* tmp;
	tmp = Account_Srv_FindByUsrName(list, userName);
	if (tmp){
		Account_Srv_DeleteByID(tmp->data.id);
		return 1;
	}
	return 0;
}

int Account_UI_Query(account_list_t list, char userName[]){
	system("cls");
	account_node_t* tmp;
	tmp = Account_Srv_FindByUsrName(list, userName);
	if (tmp){
		HEAD_banner;
		printf("User Name:%s\n", tmp->data.username);
		printf("User Ciphetext Password:%s\n", tmp->data.passwd);
		printf("User Plaintext Password:%s\n", bae64_decode(tmp->data.passwd));
		printf("User ID:%d\n", tmp->data.id);
		printf("User Type:%c", Account_UI_Status2Char(tmp->data.type));
		printf("User Type: [C]lerk / [M]anager / [A]dmin");
		return 1;
	}
	return 0;
}