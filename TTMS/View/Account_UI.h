#ifndef ACCOUNT_UI_H
#define ACCOUNT_UI_H
#define _CRT_SECURE_NO_DEPRECATE


#define MAX_LEN 30

// 登录函数 -- 登陆成功返回1 登陆失败返回0
int SysLogin(void);	// 完成

void Find_Account_UI_Mgt(void);

void Account_UI_MgtEntry(void);

int Account_UI_Add(account_list_t list);

int Account_UI_Modify(account_list_t list, char usrName[]);

int Account_UI_Delete(account_list_t list, char usrName[]);

int Account_UI_Query(account_list_t list, char usrName[]);
#endif