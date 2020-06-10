#include "Account_Pst.h"

#define ACCOUNT_DATA_FILE "../Data/acc_info.dat"



int Account_Perst_CheckAccFile() {
	if (_access(ACCOUNT_DATA_FILE, 0) == 0){
		return 1;
	}
	else{
		return 0;
	}
}

int Account_Perst_Insert(account_t* data){
	long key = EntKey_Perst_GetNewKeys(data->username, 1);
	if (key <= 0)			
		return 0;
	data->id = key;

	FILE *fp = fopen(ACCOUNT_DATA_FILE, "ab");
	int rtn = 0;
	if (fp == NULL) {
		printf("Can't open file %s!\n", ACCOUNT_DATA_FILE);
		return 0;
	}

	rtn = fwrite(data, sizeof(account_t), 1, fp);

	fclose(fp);
	return rtn;
}


int Account_Perst_SelByName(char username[], account_t *buf) {
	assert(NULL != buf);

	FILE *fp = fopen(ACCOUNT_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}

	account_t data;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&data, sizeof(account_t), 1, fp)) {
			if (!strcmp(username, data.username)) {
				*buf = data;
				found = 1;
				break;
			};
		}
	}
	fclose(fp);

	return found;
}

