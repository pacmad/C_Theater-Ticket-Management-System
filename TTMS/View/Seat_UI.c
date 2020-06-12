/*
* Copyright(C), 2007-2008, XUPT Univ.	 
* ������ţ�TTMS_UC_02
* File name: Seat_UI.c			  
* Description : ������λ���������	
* Author:   XUPT  		 
* Version:  v.1 	 
* Date: 	2015��4��22��	
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
��ʶ����TTMS_SCU_Seat_UI_S2C 
�������ܣ�������λ״̬��ȡ������ʾ���š�
����˵����statusΪseat_status_t���ͣ���ʾ��λ״̬��
�� �� ֵ���ַ��ͣ���ʾ��λ�Ľ�����ʾ���š�
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
��ʶ����TTMS_SCU_Seat_UI_C2S
�������ܣ�����������Ż�ȡ��λ״̬��
����˵����statusCharΪ�ַ��ͣ���ʾ������λ��������š�
�� �� ֵ��seat_status_t���ͣ���ʾ��λ��״̬��
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
��ʶ����TTMS_SCU_Seat_UI_MgtEnt
�������ܣ�����������λ����ں�������ʾ��ǰ����λ���ݣ����ṩ��λ������ӡ��޸ġ�ɾ�����ܲ�������ڡ�
����˵����roomIDΪ���ͣ�����Ҫ������λ���ݳ���ID��
�� �� ֵ���ޡ�
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

	//��������
	if (Seat_Srv_FetchByRoomID(head, roomID) == 0){
		studio.seatsCount = Seat_Srv_RoomInit(head, roomID, studio.rowsCount, studio.colsCount);		//��ʼ����ΪΪ����λ
		Studio_Srv_Modify(&studio);		//�����ݳ�����λ����Ϣ
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
						printf("    ");		// ���ݳ�����ĳ��������û����λ��������������겻����λ����ļ������
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
ʶ����TTMS_SCU_Seat_UI_Add
�������ܣ��������һ���µ���λ���ݡ�
����˵������һ������listΪseat_list_t����ָ�룬ָ����λ����ͷָ�룬
         �ڶ�������rowsCountΪ���ͣ���ʾ��λ�����кţ�����������colsCountΪ���ͣ���ʾ��λ�����кš�
�� �� ֵ�����ͣ���ʾ�Ƿ�ɹ��������λ�ı�־��
*/
int Seat_UI_Add(seat_list_t list, int roomID, int row, int column) {  //����һ����λ
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
��ʶ����TTMS_SCU_Seat_UI_Mod 
�������ܣ������޸�һ����λ���ݡ�
����˵������һ������listΪseat_list_t����ָ�룬ָ����λ����ͷָ�룬�ڶ�������rowsCountΪ���ͣ���ʾ��λ�����кţ�����������colsCountΪ���ͣ���ʾ��λ�����кš�
�� �� ֵ�����ͣ���ʾ�Ƿ�ɹ��޸�����λ�ı�־��
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
��ʶ����TTMS_SCU_Seat_UI_Del
�������ܣ�����ɾ��һ����λ�����ݡ�
����˵������һ������listΪseat_list_t����ָ�룬ָ����λ����ͷָ�룬�ڶ�������rowsCountΪ���ͣ���ʾ��λ�����кţ�����������colsCountΪ���ͣ���ʾ��λ�����кš�
�� �� ֵ�����ͣ���ʾ�Ƿ�ɹ�ɾ������λ�ı�־��
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

