/*
* Copyright(C), 2007-2008, XUPT Univ.
* 用例编号：TTMS_UC_02	 
* File name: Seat.h	  
* Description : 设置座位用例持久化层	
* Author:   XUPT  		 
* Version:  v.1 	 
* Date: 	2015年4月22日	
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

//添加对象主键标识名称
static const char SEAT_KEY_NAME[] = "Seat";

/*
函数功能：用于向文件中添加一个新座位数据。
参数说明：data为seat_t类型指针，表示需要添加的座位数据结点。
返 回 值：整型，表示是否成功添加了座位的标志。
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
标识符：TTMS_SCU_Seat_Perst_InsertBatch
函数功能：用于向文件中添加一批座位数据。
参数说明：list为seat_list_t类型，表示需要添加的一批座位的链表头指针。
返 回 值：整型，表示成功添加一批座位的个数。
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
标识符：TTMS_SCU_Seat_Perst_Update
函数功能：用于在文件中更新一个座位数据。
参数说明：data为seat_t类型指针，表示需要更新的座位数据结点。
返 回 值：整型，表示是否成功更新了座位的标志。
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
识符：TTMS_SCU_Seat_Perst_DelByID
函数功能：用于从文件中删除一个座位的数据。
参数说明：参数ID为整型，表示需要删除的座位ID。 
返 回 值：整型，表示是否成功删除了座位的标志。
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
标识符：TTMS_SCU_Seat_Perst_DelAllByID
函数功能：根据编号用于从文件中删除座位数据。
参数说明：参数roomID为整型，表示演出厅ID。 
返 回 值：整型，表示是否成功删除了座位的标志。
*/ 
int Seat_Perst_DeleteAllByRoomID(int roomID) {
	
	return 0;
}

/*
标识符：TTMS_SCU_Studio_Perst_SelByID
函数功能：用于从文件中载入一个座位的数据。
参数说明：第一个参数ID为整型，表示需要载入数据的座位ID；第二个参数buf为seat_t指针，指向载入座位数据的指针。
返 回 值：整型，表示是否成功载入了座位的标志。
*/
int Seat_Perst_SelectByID(int ID, seat_t *buf) {
	
	return 0;
}

/*
标识符：TTMS_SCU_Seat_Perst_SelAll
函数功能：用于从文件中载入所有座位数据。
参数说明：list为seat_list_t类型，表示将要载入的座位链表头指针。
返 回 值：整型，成功载入座位的个数。
*/
int Seat_Perst_SelectAll(seat_list_t list) {
	
	return 0;
}

/*
标识符：TTMS_SCU_Seat_Perst_SelByRoomID
函数功能：用于在文件中根据演出厅ID载入所有座位数据。
参数说明：第一个参数list为seat_list_t类型，表示将要载入的座位链表头指针，第二个参数roomID为整型，表示演出厅ID。
返 回 值：整型，表示成功载入了演出厅座位的个数。
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
