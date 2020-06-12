#ifndef ACCOUNT_H
#define ACCOUNT_H


#define _CRT_SECURE_NO_DEPRECATE
#define MAX_LEN 30

// 全局变量
account_t gl_CurUser;

//定义枚举类型account_type_t，描述用户的角色
typedef enum {
	USER_CLERK = 0, //USER_CLERK  售票员
	USER_MANG = 1, //USER_MANG  经理
	USER_ADMIN = 2 //USER_ADMIN  系统管理员
} account_type_t;



// 定义系统用户账号数据类型，描述用户账号信息
typedef struct {
	int  id;		//用户id
	account_type_t type;		//用户类型
	char username[30];		//用户名
	unsigned char passwd[30];	//用户密码
	char secret_ans[30];		//密保
} account_t;

// 定义系统用户账号双向链表
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

//获取所有系统用户
int Account_Srv_FetchAll(account_list_t list);

void Account_Srv_InitSys(void);

account_node_t* Account_Srv_FindByUsrName(account_list_t list, char userName[]);



#endif