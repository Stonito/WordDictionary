#include <visual_2013.h>
#include <ctype.h>
#include "stdio.h"
#include "conio.h"
#include <stdlib.h>
#include <string.h>
#include "dict_func.h"



char delimiters_word[] = { ' ', ',', '.', '?', '!', '-','"', '=', ':', ';', '/', '\\', '*', '(', ')', '{', '}', '[', ']', '\n', '\t','0','1','2','3','4','5','6','7','8','9', 0, EOF };
char delimiters_digit[] = { ' ', ',', '.', '?', '!', '-', '"', '=', ':', ';', '/', '\\', '*', '(', ')', '{', '}', '[', ']', '\n', '\t', 0, EOF };



int IsDelimiter(char *delimiters, char c)
{
  int i=0;
  while(delimiters[i])
  {
    if (c==delimiters[i++])
      return 1; //--- символът с е разделител ---
  }
  return 0;
}

//-----------------------------------------------------------
void ComputeWord(	int c,        //--- текущ символ, които се анализира ---
						char *word,   //--- символен масив за отделяне на дума
						int *ind,     //--- текуща позиция в думата
						int *status   //--- текущ статус на автоматната функция
						)
{
	char ch=c;
	if(*status==2) 
	{
		*status=0;
		*ind=0;
	} 
	switch (*status)
  {
		case 0:
			if (!IsDelimiter(delimiters_word, ch)) //?? ako nqma isdigit
			{
				*status = 1;
				word[(*ind)++] = c;
				word[*ind] = 0;
			}
   break;

   case 1:
	   if (IsDelimiter(delimiters_word, ch))//? ako nqma isdigit
		   *status = 2;
	else
	{
		word[(*ind)++] = c;
		word[*ind] = 0;
	}
  }
} 


void ComputeCode(int c,        //--- текущ символ, които се анализира ---
	char *digit,   //--- символен масив за отделяне на дума
	int *ind,     //--- текуща позиция в думата
	int *status   //--- текущ статус на автоматната функция
	)
{
	char ch = c;
	if (*status == 2)
	{
		*status = 0;
		*ind = 0;
	}
	switch (*status)
	{
	case 0:
		if (!(IsDelimiter(delimiters_digit, ch)))
		{
			*status = 1;
			digit[(*ind)++] = c;
			digit[*ind] = 0;
		}
		break;

	case 1:
		if (IsDelimiter(delimiters_digit, ch))
			*status = 2;
		else
		{
			digit[(*ind)++] = c;
			digit[*ind] = 0;
		}
	}
}
//-------------------------------------------------------------------------------------------------
void DeleteDictionary(char **dict, int *count_words, int index_dict)
{
  int c;
  if (dict) 
  {
    for(c=0; c<index_dict; c++) free(dict[c]);
    free (dict);
    free (count_words);
  }
}
//-------------------------------------------------------------------------------------------------
void PrintDictionary(char **dict, int index_dict, int *count_words, int status)
{
  int i;

  if( index_dict) printf("\n\n    РЕЧНИК:");
  for (i=0; i<index_dict;i++)
  {
    printf("\n%6d %s",count_words[i],dict[i]);
    if (i>0 && !(i%20) )
    {
      printf("\n\n Натиснете произволен бутон за зареждане на още думи от речника\n\n ");
      _getch();
    }
  }
  if(!index_dict)
    printf("\n\nРечникът е празен!!!\n\n");
  else
  {
    printf("\n\n    Брой на думите в речника -> %d",index_dict);
    if(!status) printf(" - Речника е недовършен - липса на достатъчнo ПАМЕТ !!!\n\n"); else printf("\n\n");
  }
}

//-------------------------------------------------------------------------------------------------
char ** MakeDictionary(FILE *fp, char **dict, int *size_dict, int *index, int **count_words, int *status)
{
	int c, ind = 0;
	int sta = 0;
	char word[256];
	int(*fff)(FILE *fp);
	char **work;
	//----------------------------------------------
	if (fp == stdin)
	{
		printf("\nВъведете текст от клавиатурата:\n");
		fff = my_fgetc;
	}
	else fff = fgetc;
	//----------------------------------------------
	rewind(fp);
	while ((c = fff(fp)) != EOF)
	{
		ComputeWord(c, word, &ind, &sta);

		if (sta == 2)
		{
			cyr_lat_toupperCP1251(word);
			if ((work = AddToDictionary(dict, size_dict, index, count_words, word)) == NULL)
			{
				*status = 0; return dict;
			}
			else
			{
				*status = 1; dict = work;
			}
		}
	}
	if (sta == 1)
	{
		cyr_lat_toupperCP1251(word);
		if ((work = AddToDictionary(dict, size_dict, index, count_words, word)) == NULL)
		{
			*status = 0; return dict;
		}
		else
		{
			*status = 1; dict = work;
		}
	}
	printf("\nРечникът е създаден успешно!\n");
	return dict;
} 

//-------------------------------------------------------------------------------------------------
char ** AddToDictionary(char **dict, int *size_dict, int *index, int **count_words, char *word)
{ 
  int ind;
  char **work_dict;
  int *work_count_words;

  if (dict==NULL)
  {
    if( (work_dict=(char **)malloc(*size_dict*sizeof(char *))) == NULL)  return NULL; 
    dict=work_dict;
  }
  if (*count_words==NULL)
  {
     if((work_count_words=(int *) malloc(*size_dict*sizeof(int)))==NULL)return NULL;
    *count_words=work_count_words;
  }
  if ( (ind=FindInToDictionary(dict, *index, word)) != -1)
  {
    (*count_words)[ind]++;
	  return dict;
  }
  if ((dict[*index] = (char *) malloc(sizeof(char)*(strlen(word)+1)))== NULL) return NULL;
  strcpy(dict[*index],word);
  (*count_words)[*index]=1;
  (*index)++;
   if (*index==*size_dict)
   {
     *size_dict+=DELTA_SIZE;
      if ((work_dict = (char **)realloc(dict,*size_dict* sizeof(char *)))==NULL) return NULL;
      dict=work_dict;
      if ((work_count_words = (int *) realloc(*count_words, *size_dict*sizeof(int)))==NULL) return NULL;
      *count_words=work_count_words;
   }
  return dict;
}

//-------------------------------------------------------------------------------------------------
int FindInToDictionary(char **dict, int index_dict, char *word)
{
  int i;
  int flag=-1;
  for (i=0; i< index_dict; i++)
    if (strcmp(dict[i],word)==0)
		  return i;
	
	return -1;
}

//------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------
int my_fgetc(FILE *fp)
{
  int ch;
  ch=_getche();
  if (ch==26) return EOF;
  if (ch=='\r')
  {
    printf("\n");
    ch='\n';
  }
  return ch;
}

//-----------------------------------------------------------------------------
void cyr_lat_toupperCP1251(unsigned char *line)
{
   int i, len;
   len=strlen(line);
   for(i=0; i<len; i++)
   {
     if(line[i]>=0xE0)
       line[i]=line[i]-0x20;
     else
       line[i]=(unsigned char)toupper(line[i]);
   }
}
//-------------------------------------------------------------------------------
void cyr_lat_tolowerCP1251(unsigned char *line)
{
   int i, len;
   len=strlen(line);
   for(i=0; i<len; i++)
   {
     if(line[i]>=0xC0 && line[i]<=0xDF)
       line[i]=line[i]+0x20;
     else
       line[i]=(unsigned char)tolower(line[i]);
   }
}
//-------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------
void PrintErrors(int err, char *name_file, char *name_dict)
{
  switch(err)
  {
    case 0:
			printf("\n\nРечникът е зреден от файла <%s>, като е бил създаден от файла <%s> !\n ",name_dict, name_file);
    break;
    case 1:
  		printf("\n\nГРЕШКА - липсва името на изходния файл, от които е създаден речника !!!\n ");
    break;
    case 2:
								 printf("\n\nГРЕШКА при четене на текуща дума от речника");
    break;
    case 3:
		  printf("\n\nГРЕШКА - недостатъчна памет за възтановяване на речника !!!\n ");
    break;
  }
}
//------------------------------------------------------------------------------------------------
void SortDict(char **dict, int index, int *count_words,int mode)
{
	int i, j, k;
	char*tmp;

	switch (mode)
	{
	case 1:
		for (i = 0; i < index; i++)
		for (j = 0; j < index - 1; j++)
		if (strcmp(dict[j], dict[j + 1])>0)
		{
			tmp = dict[j]; dict[j] = dict[j + 1]; dict[j + 1] = tmp;
			k = count_words[j]; count_words[j] = count_words[j + 1]; count_words[j + 1] = k;
		}
		break;
	case 2:
		for (i = 0; i < index;i++)
		for (j = 0; j < index;j++)
		if (count_words[j] < count_words[j + 1])
		{
			tmp = dict[j]; dict[j] = dict[j + 1]; dict[j + 1] = tmp;
			k = count_words[j]; count_words[j] = count_words[j + 1]; count_words[j + 1] = k;
		}
		break;
	default:
		break;
	}
}

void SaveDictionary(char **dict, int index, int *count_words, char *name_file, char *name_dict, int mode, int mode2)
{
	int i;
	char fcount[50];
	FILE *fp;
	FILE *fc;

	printf("\nВъведете името на файла за записване на речника: ");
	scanf("%s", name_dict);

	fp = fopen(name_dict, "w");

	if (mode2 == 1)
		fprintf(fp, "Речникът е създаден при въвеждане от клавиатурата\n");
	else
		fprintf(fp, "Речникът е създаден от файл \"%s\"\n", name_file);

	switch (mode)
	{
	case 1:
		for (i = 0; i < index; i++)
			fprintf(fp, "%s - %d\n", dict[i], count_words[i]);
		fclose(fp);
		break;
	case 2:
			
		strcpy(fcount, name_dict);
		strcat(fcount, ".cnt");
		fc = fopen(fcount, "w");
			for (i = 0; i < index; i++)
			{
				fprintf(fc, "%d\n", count_words[i]);
				fprintf(fp, "%s\n", dict[i]);
			}
			fclose(fc);
			fclose(fp);
	case 3:
		break;
	}
	printf("\nГотово!\n");
}

char ** ReadDictionary(FILE *fp, char **dict, int *size_dict, int *index, int **count_words, int *status)
{
	int c, ind = 0, flag = 0;
	int sta = 0;
	char word[256];
	int(*fff)(FILE *fp);
	char **work;
	fff = fgetc;
	rewind(fp);
	while ((c = fff(fp)) != EOF)
	{
		if (c == '\n')
			flag = 1;
		if (flag == 1)
		{
			ComputeWord(c, word, &ind, &sta);

			if (sta == 2)
			{
				cyr_lat_toupperCP1251(word);
				if ((work = AddToDictionary(dict, size_dict, index, count_words, word)) == NULL)
				{
					*status = 0; return dict;
				}
				else
				{
					*status = 1; dict = work;
				}
			}
		}
	}
	if (sta == 1)
	{
		cyr_lat_toupperCP1251(word);
		if ((work = AddToDictionary(dict, size_dict, index, count_words, word)) == NULL)
		{
			*status = 0; return dict;
		}
		else
		{
			*status = 1; dict = work;
		}
	}
	return dict;
}

void SaveCodeFile(FILE *fp, char **dict, int index, char *name_file, char *name_dict, int mode)
{
	int c, ind = 0, i, cmp, flag = 0, num;
	int sta = 0;
	char word[256], digits[256], fname[50], fname2[50], str[4] = "xxx";
	FILE *fp_code;
	FILE *fp_decode;
	int(*fff)(FILE *fp);
	switch (mode)
	{
	case 1:
		if (fp == NULL)
		{
			printf("\n Не сте въвели файл за обработка !!!");
			break;
		}
		printf("\nВъведете името на файла за записване на кодирания файл (думите, които липсват в речника биват кодирани с 'xxx'): ");
		scanf("%s", fname);
		fp_code = fopen(fname, "w");
		fprintf(fp_code, "%s\n", name_dict);
		fff = fgetc;
		rewind(fp);
		do
		{
			c = fff(fp);
			ComputeWord(c, word, &ind, &sta);
			if (sta == 2)
			{
				cyr_lat_toupperCP1251(word);
				for (i = 0; i < index; i++)
				{
					cmp = strcmp(word, dict[i]);
					if (cmp == 0)
					{
						fprintf(fp_code, "%d", i);
						flag = 1;
					}
					if (i == (index - 1) && flag == 0)
						fprintf(fp_code, "%s", str);
				}
				flag = 0;
			}
			if (IsDelimiter(delimiters_word, c))
				fprintf(fp_code, "%c", c);
		} while (c != EOF);
		fclose(fp_code);
		printf("\nГотово!\n");
		break;
	case 2:
		if (fp == NULL)
		{
			printf("\n Не сте въвели файл за обработка !!!\n");
			break;
		}
		printf("\nВъведете името на файла, в който искате да запишете декодирания файл: ");
		scanf("%s", fname2);
		fp_decode = fopen(fname2, "w");
		fff = fgetc;
		rewind(fp);
		do
		{
			c = fff(fp);
			if (c == '\n')
				flag = 1;
			if (flag == 1)
			{
				ComputeCode(c, digits, &ind, &sta);
				if (sta == 2)
				{
					num = atoi(digits);
					if (dict[num])
						fprintf(fp_decode, "%s", dict[num]);

				}
				if (IsDelimiter(delimiters_digit, c))
					fprintf(fp_decode, "%c", c);
			}
		} while (c != EOF);
		fclose(fp_decode);
		printf("\nГотово!\n");
		break;
	}
}

