#include "Account_UI.h"


int SysLogin(){
	Account_Srv_InitSys();
	system("clear");
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
	char passwd[MAX_LEN];


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
//			RESET_Account_UI_Mgt();
		}
		else {
			return 0;
		}
	}

	return 1;
}