/*
* Copyright(C), 2007-2008, XUPT Univ.	 
* 用例编号：TTMS_UC_02
* File name: Seat_UI.c			  
* Description : 设置座位用例界面层	
* Author:   XUPT  		 
* Version:  v.1 	 
* Date: 	2015年4月22日	
*/

#include "Seat_UI.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "../Common/List.h"
#include "../Service/Seat.h"
#include "../Service/Studio.h"
#include "../Service/Account.h"


/*
表识符：TTMS_SCU_Seat_UI_S2C 
函数功能：根据座位状态获取界面显示符号。
参数说明：status为seat_status_t类型，表示座位状态。
返 回 值：字符型，表示座位的界面显示符号。
*/
char Seat_UI_Status2Char(seat_status_t status) {
	switch (status){
	case SEAT_GOOD:
		return '#';
	case SEAT_BROKEN:
		return 'X';
	case SEAT_NONE:
		return ' ';
	}
}

/*
标识符：TTMS_SCU_Seat_UI_C2S
函数功能：根据输入符号获取座位状态。
参数说明：statusChar为字符型，表示设置座位的输入符号。
返 回 值：seat_status_t类型，表示座位的状态。
*/
seat_status_t Seat_UI_Char2Status(char statusChar) {
	switch (statusChar){
	case '#':
		return SEAT_GOOD;
	case 'X':
		return SEAT_BROKEN;
	case ' ':
		return SEAT_NONE;
	}
}

/*
标识符：TTMS_SCU_Seat_UI_MgtEnt
函数功能：界面层管理座位的入口函数，显示当前的座位数据，并提供座位数据添加、修改、删除功能操作的入口。
参数说明：roomID为整型，是需要设置座位的演出厅ID。
返 回 值：无。
*/ 
void Seat_UI_MgtEntry(int roomID) {
	system("cls");
	if (gl_CurUser.type != USER_MANG){
		printf("you can't join in there!please input the [Enter]");
		getchar();
		return;
	}

	studio_t studio;
	seat_list_t head;
	seat_node_t* tmp;

	if (!Studio_Srv_FetchByID(roomID, &studio)) {
		printf("The room does not exist!\nPress [Enter] key to return!\n");
		getchar();
		return;
	}

	char choice;

	List_Init(head, studio_t);

	//载入数据
	if (Seat_Srv_FetchByRoomID(head, roomID) == 0){
		studio.seatsCount = Seat_Srv_RoomInit(head, roomID, studio.rowsCount, studio.colsCount);		//初始化作为为好座位
		Studio_Srv_Modify(&studio);		//更新演出厅座位数信息
	}
	
	do{
		HEAD_banner;
		printf("row:%10d colum:%10d seatcount:%10d\n", studio.rowsCount, studio.colsCount, studio.seatsCount);

		END_banner;
		int m = 0, n = 1;
		for (int i = 0; i < studio.rowsCount + 1; i++){
			for (int j = 0; j < studio.colsCount + 1; j++){
				if (i = 0){
					printf("%4d", m++);
				}
				else if (j = 0){
					printf("%4d", n++);
				}
				else{
					int flag = 1;
					List_ForEach(head, tmp){
						if (tmp->data.row == i && tmp->data.column == j){
							flag = 0;
							printf("%4c", tmp->data.status);
							break;
						}
					}
					if (flag){
						printf("    ");		// 在演出厅的某个坐标下没有座位，用来处理该坐标不在座位情况文件的情况
					}
				}
			}
			printf("\n");
		}
		END_banner;
		printf("[A]dd | [M]odify | [R]emove | Re[T]urn\n");
		END_banner;

		fflush(stdin);
		while (scanf("%c", &choice) && tolower(choice) != 'a' && tolower(choice) != 'm' && tolower(choice) != 'r' && tolower(choice) != 't'){
			printf("Please input proper letter:\n");
		}

		int row, col;
		fflush(stdin);
		switch (tolower(choice)){
		case 'a':
			printf("ROW of the seat:");
			scanf("%c", &row);
			printf("COLUMN of the seat:");
			scanf("%c", &col);
			if (!Seat_UI_Add(head, roomID, row, col)){
				printf("Fail to operate the seat.\n");
			}
			else{
				studio.seatsCount = studio.seatsCount + 1;
				Studio_Srv_Modify(&studio);
				Seat_Srv_FetchByRoomID(head, roomID);
				printf("Add successfully!\n");
			}
			break;
		case 'm':
			printf("ROW of the seat:");
			scanf("%c", &row);
			printf("COLUMN of the seat:");
			scanf("%c", &col);
			if (!Seat_UI_Modify(head, row, col)){
				printf("Fail to operate the seat.\n");
			}
			else{
				Seat_Srv_FetchByRoomID(head, roomID);
				printf("Modify Successfully\n");
			}
			break;
		case 'd':
			printf("ROW of the seat:");
			scanf("%c", &row);
			printf("COLUMN of the seat:");
			scanf("%c", &col);
			if (!Seat_UI_Delete(head, row, col)){
				printf("Fail to operate the seat.\n");
			}
			else{
				studio.seatsCount--;
				Studio_Srv_Modify(&studio);
				Seat_Srv_FetchByRoomID(head, roomID);
				printf("Remove Successful!\n");
			}
			break;
		}
	} while (tolower(choice) != 'r');
	Seat_Srv_FetchByRoomID(head, roomID);
}

/*
识符：TTMS_SCU_Seat_UI_Add
函数功能：用于添加一个新的座位数据。
参数说明：第一个参数list为seat_list_t类型指针，指向座位链表头指针，
         第二个参数rowsCount为整型，表示座位所在行号，第三个参数colsCount为整型，表示座位所在列号。
返 回 值：整型，表示是否成功添加了座位的标志。
*/
int Seat_UI_Add(seat_list_t list, int roomID, int row, int column) {  //输入一个座位
	if (Seat_Srv_FindByRowCol(list, row, column) == NULL){
		seat_t tmp;
		tmp.row = row;
		tmp.column = column;
		tmp.status = SEAT_GOOD;
		tmp.roomID = roomID;
		if (Seat_Srv_Add(&tmp)){
			return 1;
		}
	}
	return 0;
}

/*
标识符：TTMS_SCU_Seat_UI_Mod 
函数功能：用于修改一个座位数据。
参数说明：第一个参数list为seat_list_t类型指针，指向座位链表头指针，第二个参数rowsCount为整型，表示座位所在行号，第三个参数colsCount为整型，表示座位所在列号。
返 回 值：整型，表示是否成功修改了座位的标志。
*/
int Seat_UI_Modify(seat_list_t list, int row, int column) {
	seat_node_t* tmp = Seat_Srv_FindByRowCol(list, row, column);
	if (tmp != NULL){
		END_banner;
		printf("[#]Well | [X]Broken | [ ]Remove\n");
		END_banner;
		char choice;
		fflush(stdin);
		scanf("%c", &choice);

		tmp->data.status = Seat_UI_Char2Status(choice);

		if (Seat_Srv_Modify(tmp)){
			return 1;
		}
	}

	return 0;
}

/*
标识符：TTMS_SCU_Seat_UI_Del
函数功能：用于删除一个座位的数据。
参数说明：第一个参数list为seat_list_t类型指针，指向座位链表头指针，第二个参数rowsCount为整型，表示座位所在行号，第三个参数colsCount为整型，表示座位所在列号。
返 回 值：整型，表示是否成功删除了座位的标志。
*/
int Seat_UI_Delete(seat_list_t list, int row, int column) {
	seat_list_t tmp = Seat_Srv_FindByRowCol(list, row, column);

	if (tmp != NULL){
		if (Seat_Srv_DeleteByID(tmp->data.id)){
			return 1;
		}
	}
	return 0;
}

