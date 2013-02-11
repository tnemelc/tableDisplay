#ifndef _TABLE_H_
#define _TABLE_H_

#define NB_LINE_CHAR  		81
#define EOK						0
#define END						"--------------------------------------------------------------------------------\n"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

typedef struct table_t
{
	char * pdata;
	int lines;
	int cols;
	int *maxLen;
} table;
/*	*
	* create a table
	* [in] ptitle : string containing the table title
	* [in] pdata : table cells
	* [in] ptab : table pointer
	* return : 0 if table has been created, -1 if not
	* */
int createTable(char* ptitle, char * pdata, table * ptab);

/*	*
	* destroy table data
	* */
int destroyTable(table tab);

/*	*
	* (not implemented yet) 
	* add a line to table
	* */
int addLine();

/* 	*
	* count lines and cols that table shall
	* store
	* [in] pdata : data that will be stored in table
	* [out] lines : lines of table
	* [out] cols : colomns of table
	* return : 0 if count has been correctly effected
	* */
int countLinesCols(char * pdata, int * lines, int * cols);

/* 	*
	* Set title of the Table
	* [in] ptitle : string containing the table title
	* [inout] : pointer on table 
	* */
void setTitle(char * ptitle, table * ptab);

/*	*
	* insert data in table. The data will be
	* prepared to be displayed
	* */
int insertData(char * pdata, table * ptab);

/*	* 
	* find the data that is the longest in each colomn
	* [in]
	* */
int findMaxLenDataPerColumns(char * pdata, table * ptab);

/*	* 
	* copy one cell of the data. nbPrevSpc spaces
	* are added before cell data,  nbScdgSpc spaces
	* are added after cell data.
	* [in]
	* */
int copyCellData(char * pcell, char * pdata, int nbPrevSpc, int nbScdgSpc);

/*	* 
	* return nb caracters before ':', ';' 
	* or end of string value
	* [in] pdata : cell data
	* return : len
	* */	
int cellLen(char * pdata);

/*	*
	* process and return 
	* length of a line of data minus
	* first cell length
	* */
int processLineLen(table * ptab);

/*	*
	*
	*
	* */
void prepareData(char ** outData, char * inData, ...);

void initstring(char * data, int const size);
#endif /*_TABLE_H_*/	

