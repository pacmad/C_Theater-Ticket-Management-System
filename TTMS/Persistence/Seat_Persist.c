/*
* Copyright(C), 2007-2008, XUPT Univ.
* ������ţ�TTMS_UC_02	 
* File name: Seat.h	  
* Description : ������λ�����־û���	
* Author:   XUPT  		 
* Version:  v.1 	 
* Date: 	2015��4��22��	
*/

#include "Seat_Persist.h"
#include "../Service/Seat.h"
#include "../Common/List.h"
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <assert.h>


static const char SEAT_DATA_FILE[] = "Seat.dat";
static const char SEAT_DATA_TEMP_FILE[] = "SeatTmp.dat";

//��Ӷ���������ʶ����
static const char SEAT_KEY_NAME[] = "Seat";

/*
�������ܣ��������ļ������һ������λ���ݡ�
����˵����dataΪseat_t����ָ�룬��ʾ��Ҫ��ӵ���λ���ݽ�㡣
�� �� ֵ�����ͣ���ʾ�Ƿ�ɹ��������λ�ı�־��
*/ 
int Seat_Perst_Insert(seat_t *data) {   
	assert(NULL!=data);
	int rtn = 0;
	FILE* fp = fopen(SEAT_DATA_FILE, "ab");
	if (fp == NULL){
		printf("Cannot set up file %s.\n", SEAT_DATA_FILE);
		return 0;
	}


	long key = EntKey_Perst_GetNewKeys(SEAT_KEY_NAME, 1); 
	if (key <= 0)
		return 0;
	data->id = key;

	rtn = fwrite(data, sizeof(seat_t), 1, fp);
	
	fclose(fp);

	return rtn;
}

/*
��ʶ����TTMS_SCU_Seat_Perst_InsertBatch
�������ܣ��������ļ������һ����λ���ݡ�
����˵����listΪseat_list_t���ͣ���ʾ��Ҫ��ӵ�һ����λ������ͷָ�롣
�� �� ֵ�����ͣ���ʾ�ɹ����һ����λ�ĸ�����
*/
int Seat_Perst_InsertBatch(seat_list_t list) {
	seat_node_t *p = list->next;
	assert(NULL!=list);
	FILE* fp = (SEAT_DATA_FILE, "ab");

	if (fp == NULL){
		printf("Cannot open %s.", SEAT_DATA_FILE);
		return 0;
	}

	int count = 0;

	while (p){
		long key = EntKey_Perst_GetNewKeys(SEAT_KEY_NAME, 1);
		if (key <= 0)			
			return 0;
		list->data.id = key;

		fwrite(p, sizeof(seat_t), 1, fp);
		p = p->next;
		count++;
	}

	return count;
}



/*
��ʶ����TTMS_SCU_Seat_Perst_Update
�������ܣ��������ļ��и���һ����λ���ݡ�
����˵����dataΪseat_t����ָ�룬��ʾ��Ҫ���µ���λ���ݽ�㡣
�� �� ֵ�����ͣ���ʾ�Ƿ�ɹ���������λ�ı�־��
*/
int Seat_Perst_Update(const seat_t *seatdata) {
	assert(NULL != seatdata);

	FILE *fp = fopen(SEAT_DATA_FILE, "rb+");
	if (NULL == fp) {
		printf("Cannot open %s.\n", SEAT_DATA_FILE);
		return 0;
	}

	seat_t buf;
	int rtn = 0;

	while (!feof(fp)) {
		if (fread(&buf, sizeof(seat_t), 1, fp)) {
			if (buf.id == seatdata->id) {
				fseek(fp, -((int)sizeof(seat_t)), SEEK_CUR);
				fwrite(seatdata, sizeof(seat_t), 1, fp);
				rtn = 1;
				break;
			}
		}
	}
	fclose(fp);

	return rtn;
}

/*
ʶ����TTMS_SCU_Seat_Perst_DelByID
�������ܣ����ڴ��ļ���ɾ��һ����λ�����ݡ�
����˵��������IDΪ���ͣ���ʾ��Ҫɾ������λID�� 
�� �� ֵ�����ͣ���ʾ�Ƿ�ɹ�ɾ������λ�ı�־��
*/
int Seat_Perst_DeleteByID(int ID) {
	rename(SEAT_DATA_FILE, SEAT_DATA_TEMP_FILE);
	
	FILE* origin = fopen(SEAT_DATA_TEMP_FILE, "rb");
	FILE* target = fopen(SEAT_DATA_FILE, "wb");

	if (!origin || !target){
		printf("Fail to open file(s).");
		return 0;
	}

	seat_t seat;
	int flag = 0;

	while (!feof(origin)){
		if (fread(&seat, sizeof(seat_t), 1, origin)){
			if (seat.id == ID){
				flag = 1;
				continue;
			}
			fwrite(&seat, sizeof(seat_t), 1, target);
		}
	}

	fclose(SEAT_DATA_FILE);
	fclose(SEAT_DATA_TEMP_FILE);

	remove(SEAT_DATA_TEMP_FILE);
	return flag;
}

/*
��ʶ����TTMS_SCU_Seat_Perst_DelAllByID
�������ܣ����ݱ�����ڴ��ļ���ɾ����λ���ݡ�
����˵��������roomIDΪ���ͣ���ʾ�ݳ���ID�� 
�� �� ֵ�����ͣ���ʾ�Ƿ�ɹ�ɾ������λ�ı�־��
*/ 
int Seat_Perst_DeleteAllByRoomID(int roomID) {
	
	return 0;
}

/*
��ʶ����TTMS_SCU_Studio_Perst_SelByID
�������ܣ����ڴ��ļ�������һ����λ�����ݡ�
����˵������һ������IDΪ���ͣ���ʾ��Ҫ�������ݵ���λID���ڶ�������bufΪseat_tָ�룬ָ��������λ���ݵ�ָ�롣
�� �� ֵ�����ͣ���ʾ�Ƿ�ɹ���������λ�ı�־��
*/
int Seat_Perst_SelectByID(int ID, seat_t *buf) {
	
	return 0;
}

/*
��ʶ����TTMS_SCU_Seat_Perst_SelAll
�������ܣ����ڴ��ļ�������������λ���ݡ�
����˵����listΪseat_list_t���ͣ���ʾ��Ҫ�������λ����ͷָ�롣
�� �� ֵ�����ͣ��ɹ�������λ�ĸ�����
*/
int Seat_Perst_SelectAll(seat_list_t list) {
	
	return 0;
}

/*
��ʶ����TTMS_SCU_Seat_Perst_SelByRoomID
�������ܣ��������ļ��и����ݳ���ID����������λ���ݡ�
����˵������һ������listΪseat_list_t���ͣ���ʾ��Ҫ�������λ����ͷָ�룬�ڶ�������roomIDΪ���ͣ���ʾ�ݳ���ID��
�� �� ֵ�����ͣ���ʾ�ɹ��������ݳ�����λ�ĸ�����
*/
int Seat_Perst_SelectByRoomID(seat_list_t list, int roomID) {
	int rtn = 0;

	FILE* fp = fopen("SEAT_DATA_FILE", "rb");
	if (!fp){
		return 0;
	}

	seat_t seat;
	seat_node_t* pnew;

	while (!feof(fp)){
		if (fread(&seat, sizeof(seat_t), 1, fp)){
			if (roomID == seat.roomID){
				pnew = (seat_node_t*)malloc(sizeof(seat_node_t));
				if (!pnew) {
					printf(
						"Warning, Memory OverFlow!!!\n Cannot Load more Data into memory!!!\n");
					break;
				}
				pnew->data = seat;
				List_AddTail(list, pnew);
				rtn++;
			}
		}
	}

	fclose(fp);
	return rtn;
}
