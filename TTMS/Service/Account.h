#ifndef ACCOUNT_H
#define ACCOUNT_H


#define _CRT_SECURE_NO_DEPRECATE
#define MAX_LEN 30

// ȫ�ֱ���
account_t gl_CurUser;

//����ö������account_type_t�������û��Ľ�ɫ
typedef enum {
	USER_CLERK = 0, //USER_CLERK  ��ƱԱ
	USER_MANG = 1, //USER_MANG  ����
	USER_ADMIN = 2 //USER_ADMIN  ϵͳ����Ա
} account_type_t;



// ����ϵͳ�û��˺��������ͣ������û��˺���Ϣ
typedef struct {
	int  id;		//�û�id
	account_type_t type;		//�û�����
	char username[30];		//�û���
	unsigned char passwd[30];	//�û�����
	char secret_ans[30];		//�ܱ�
} account_t;

// ����ϵͳ�û��˺�˫������
typedef struct account_node {
	account_t data;
	struct account_node *next, *prev;
} account_node_t, *account_list_t;




int Account_Srv_Verify(char usrName[], unsigned char pwd[]);

int Account_Srv_Add(const account_t *data);

int Account_Srv_Modify(const account_t *data);

int Account_Srv_DeleteByID(int usrID);

int Account_Srv_FetchByID(int usrID, account_t *buf);

int Account_Srv_FetchByName(char usrName[], account_t *buf);

//��ȡ����ϵͳ�û�
int Account_Srv_FetchAll(account_list_t list);

void Account_Srv_InitSys(void);

account_node_t* Account_Srv_FindByUsrName(account_list_t list, char userName[]);



#endif