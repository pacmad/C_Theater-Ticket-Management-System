#ifndef ACCOUNT_UI_H
#define ACCOUNT_UI_H
#define _CRT_SECURE_NO_DEPRECATE


#define MAX_LEN 30

// ��¼���� -- ��½�ɹ�����1 ��½ʧ�ܷ���0
int SysLogin(void);	// ���

void Find_Account_UI_Mgt(void);

void Account_UI_MgtEntry(void);

int Account_UI_Add(account_list_t list);

int Account_UI_Modify(account_list_t list, char usrName[]);

int Account_UI_Delete(account_list_t list, char usrName[]);

int Account_UI_Query(account_list_t list, char usrName[]);
#endif