#include "table.h"



int createTable(char * ptitle, char * pdata, table * ptab)
{
  int retVal;
	int i = 0;
	if ((0 == strlen(ptitle)) || (0 == strlen(pdata)) || (0 == ptab))
	{
		return (-1);
	}
	retVal = countLinesCols(pdata, &(ptab->lines), &(ptab->cols));
	if (EOK != retVal)
	{
		printf("error on colomns count\n");
		return (-1);
	}
	ptab->pdata = malloc((ptab->lines + 2) * NB_LINE_CHAR);
	while ((ptab->lines + 2) * NB_LINE_CHAR > i)
	{
		ptab->pdata[i] = 0;
		++i;
	}
	strcat(ptab->pdata, END);
	setTitle(ptitle, ptab);
	retVal = findMaxLenDataPerColumns(pdata, ptab);
	insertData(pdata, ptab);
	strcat(ptab->pdata, END);
	return 0;
}

int destroyTable(table tab)
{
	int retVal = 0;
	
	free(tab.pdata);
	return retVal;	
}

int addLine()
{
	int retVal = 0;
	return retVal;
}

int countLinesCols(char* pdata, int * lines, int * cols)
{
	int i = 0;
	int tmpCols = 1;
	
	*lines = 1;
	*cols = 1;
	while('\0' != pdata[i])
	{
		if ((':' == pdata[i]) && (92 != pdata[i - 1]))
		{
			++tmpCols;
			if (1 == *lines)
			{
				++(*cols);
			}
		} 
		if ((';' == pdata[i]) && (92 != pdata[i - 1]) && ('\0' != pdata[i + 1]))
		{
			if ((1 != *lines) && (tmpCols != *cols))
			{
				return (-1);
			}
			++(*lines);
			tmpCols = 1;
		}
		++i;
	}
	return 0;
}

void setTitle(char * ptitle, table * ptab)
{
	int i = 0;
	int j = (NB_LINE_CHAR / 2 - strlen(ptitle)/2);
	
	while ('\0' != ptitle[i])
	{
		ptab->pdata[j] = ptitle[i];
		++i;
		++j;
	}
}

int insertData(char * pdata, table * ptab)
{
	int i = 0, j = 0, pos = 0, dataLen = 0, cellLength;
	int prvSpc, sucSpc;
	char tmpStr[NB_LINE_CHAR];

	dataLen = processLineLen(ptab);
	while (i < ptab->lines)
	{
		j = 0;
		while (j < ptab->cols)
		{
			if (j == 0)
			{
				prvSpc = 0;
				sucSpc = (NB_LINE_CHAR - dataLen) - cellLen(&pdata[pos]);
			}
			else
			{
				cellLength = cellLen(&pdata[pos]);
				prvSpc  = ((ptab->maxLen[j] - cellLength) / 2) + 1;;
				if (0 == (ptab->maxLen[j] - cellLength) % 2)
				{
					sucSpc  = ((ptab->maxLen[j] - cellLength) / 2) + 1;
				}
				else 
				{
					sucSpc  = ((ptab->maxLen[j] - cellLength) / 2) + 2;
				}
			}
			pos += copyCellData(&tmpStr[0], &pdata[pos], prvSpc, sucSpc);
			strcat(ptab->pdata, &tmpStr[0]);
			++pos;
			++j;
		}
		strcat(ptab->pdata,"\n");
		++i;
	}
	return i;
}


int findMaxLenDataPerColumns(char * pdata, table * ptab)
{
	int i, col = 0, colLen = 0;
	
	ptab->maxLen = malloc(ptab->cols * sizeof (int));
	for(i = 0; ptab->cols > i; ++i)
	{
		ptab->maxLen[i] = 0;
	}
	i = 0;
	while ('\0' != pdata[i])
	{
		if ((':' == pdata[i]) || (';' == pdata[i] ))
		{
			-- colLen;
			if (ptab->maxLen[col] < colLen)
			{
				ptab->maxLen[col] = colLen;
			}
			++col;
			colLen = 0;
		}
		if (';' == pdata[i])
		{
			col = 0;
			colLen = 0;
		}
		++colLen;
		++i;
	}
	return 0;
}

int copyCellData(char * pcell, char * pdata, int nbPrevSpc, int nbScdgSpc)
{
	int i = 0, j = 0, pos = 0;
	
	while (0 < nbPrevSpc)
	{
		pcell[i] = ' ';
		--nbPrevSpc;
		++i;
	}
	while (((':' != pdata[j]) && (';' != pdata[j]) && ('\0' != pdata[j])) && (92 != pdata[j - 1]))
	{
		pcell[i] = pdata[j];
		++i;
		++j;
		++pos;
	}
	while (0 < nbScdgSpc)
	{
		pcell[i] = ' ';
		--nbScdgSpc;
		++i;
	}
	pcell[i] = '|';
	++i;
	pcell[i] = '\0';
	return pos;
}

int cellLen(char * pdata)
{
	int i = 0;
	
	while ((':' != pdata[i]) && (';' != pdata[i]) && ('\0' != pdata[i]))
	{
		++i;
	}
	return i;
}

int processLineLen(table * ptab)
{
	int i, dataLen = 0;
	
	for (i = 1; i < ptab->cols; ++i)
	{
		dataLen += ptab->maxLen[i] + 3;
	}
	dataLen += 2;
	return dataLen;
}

void prepareData(char ** ppoutData, char * pinData, ...)
{
	int i = 0, j = 0;
	char strTmp[50];
	char tmpchar;
	va_list ap;
	
	va_start(ap, pinData);
	while ('\0' != pinData[i])
	{
		if ('%' == pinData[i])
		{
			initstring(strTmp, 50);
			++i;
			switch (pinData[i])
			{
				case 'c':	sprintf(&strTmp[0], "%c", (char)va_arg(ap, int));
				            j += (int)strlen(&strTmp[0]);
				            *ppoutData = realloc(*ppoutData, j);
							(*ppoutData)[j - (int)strlen(&strTmp[0])] = '\0';
				            strcat(*ppoutData, strTmp);
					break;
				case 'd':	sprintf(&strTmp[0], "%d", va_arg(ap, int));
							j += (int)strlen(&strTmp[0]);
							*ppoutData = realloc(*ppoutData, j);
							(*ppoutData)[j - (int)strlen(&strTmp[0])] = '\0';
							strcat(*ppoutData, strTmp);
					break;
				case 's':	sprintf(&strTmp[0], "%s", va_arg(ap, char*));
				            j += (int)strlen(&strTmp[0]);
				            *ppoutData = realloc(*ppoutData, j);
							(*ppoutData)[j - (int)strlen(&strTmp[0])] = '\0';
				            strcat(*ppoutData, strTmp);
					break;
				case 'f':	sprintf(&strTmp[0], "%.3f", va_arg(ap, double));
				            j += (int)strlen(&strTmp[0]);
				            *ppoutData = realloc(*ppoutData, j);
							(*ppoutData)[j - (int)strlen(&strTmp[0])] = '\0';
							strcat(*ppoutData, strTmp);
					break;
				default:	*ppoutData = realloc(*ppoutData, ++j);
							(*ppoutData)[j] = pinData[i];
					break;
			}
		}
		else 
		{
			*ppoutData = realloc(*ppoutData, ++j);
			(*ppoutData)[j - 1] = pinData[i];
		}
		++i;
	}
	*ppoutData = realloc(*ppoutData, ++j);
	(*ppoutData)[j - 1] = 0;
	va_end(ap);
}


void initstring(char * data, int const size)
{
	int i = 0;
	
	while (size > i)
	{
		data[i] = '\0';
		++i;
	}
}


