#include "Account.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Common/common.h"
#include "../Common/List.h"
#include "../Persistence/Account_Pst.h"

// 初次登录系统进行管理账户创建
void Account_Srv_InitSys(){
	// 若初次登陆的初始化已进行
	if (Account_Perst_CheckAccFile() == 1){
		return;
	}
	account_t init_admin_info;
	unsigned char passwd[MAX_LEN];
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
	printf("There's no account available!\nPlease Sign Up for Admin.\n");
	END_banner;
	printf("User Name:");
	scanf("%s", init_admin_info.username);
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

	strcpy(init_admin_info.passwd, base64_encode(passwd));	//base64 "加密字符串"

	printf("Security Code for Fogetting Password:");
	scanf("%s", init_admin_info.secret_ans);
	
	init_admin_info.type = USER_ADMIN;

	Account_Srv_Add(&init_admin_info);

}

int Account_Srv_Verify(char username[], unsigned char passwd[]){
	account_t user;
	if (Account_Perst_SelByName(username, &user)){
		strcpy(user.passwd, base64_decode(user.passwd));
		if (!strcmp(user.passwd, passwd)){
			gl_CurUser = user;			// 全局
			return 1;
		}
		else{
			return 0;
		}
	}
	return 0;
}

account_node_t* Account_Srv_FindByUsrName(account_list_t list, char userName[]) {
	account_node_t* tmp;
	List_ForEach(list, tmp){
		if (strcmp(tmp->data.username, userName) == 0){
			return tmp;
		}
	}
	return NULL;
}



//添加一个用户账号，通过调用Account_Perst_Insert(data)函数实现
int Account_Srv_Add(const account_t *data){
	return Account_Perst_Insert(data);
}

//修改一个用户账号，通过调用Account_Perst_Update(data)函数实现
int Account_Srv_Modify(const account_t *data){
	return Account_Perst_Update(data);
}

//删除一个用户账号，通过调用Account_Perst_DeleteByID(usrID)函数实现
int Account_Srv_DeleteByID(int usrID){
	return Account_Perst_DeleteByID(usrID);

}

//提取usrID对应的用户账号信息，通过调用Account_Perst_SelectByID(usrID, buf)函数实现
int Account_Srv_FetchByID(int usrID, account_t *buf){
	return Account_Perst_SelectByID(usrID, buf);

}

//提取usrName对应的用户账号信息，通过调用Account_Perst_SelByName(usrName, buf)函数实现
int Account_Srv_FetchByName(char usrName[], account_t *buf){
	return Account_Perst_SelByName(usrName, buf);

}

//提取所有用户账号信息，保存到list链表中，通过调用Account_Perst_SelectAll(list)函数实现
int Account_Srv_FetchAll(account_list_t list){
	return Account_Perst_SelectAll(list);
}