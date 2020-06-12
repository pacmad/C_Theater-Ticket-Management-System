#ifndef ACCOUNT_PST_H
#define ACCOUNT_PST_H
#define _CRT_SECURE_NO_DEPRECATE


int Account_Perst_CheckAccFile(void);

int Account_Perst_Insert(account_t* data);

int Account_Perst_SelByName(char username[], account_t *buf);

int Account_Perst_Update(const account_t* data);


#endif