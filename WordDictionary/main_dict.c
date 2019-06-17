#include <visual_2013.h>
#include <stdio.h>
#include <conio.h>

#include <windows.h>

#include "menu.h"
#include "dict_func.h"


//------------- ������ -----------------------
char *txt_menu_g[]=
{
	  "�������� <������>\n--------------------",
		"����� �� ������ �����",
		"�������� �� ������ �� ������",
		"��������� ������� �� ������",
		"������� �� ���� � �������",
		"��������� �� �������",
    "�����/��������� �� ������",
    "��������/���������� �� ������",
		"����",
		NULL
};

char *txt_menu_1[]=
{
			"---������ �����---",
			"�� ����������",
			"�� �������� ����",
      "����� �� �����",
      NULL
};

char *txt_menu_2[]=
{
   "����� �� ���� ����������:",
		"�� ������� ���",
		"�� ���� �������� �� ������ � ������",
    "����� �� �����",
		NULL
};

char *txt_menu_3[]=
{
   "---����� �� �������---",
		"����� ��� ����",
		"��������� �� ����",
    "����� �� �����",
		NULL
};
char *txt_menu_4[]=
{
	"����� �� ����� �� ���������:",
	"����� ���� �� ��� ��� � ����� ��� ���������.",
	"��������� �� �������� � ������� ���� � ���������� \".cnt\".",
	"�����",
	NULL
};
char *txt_menu_5[]=
{
   "---���������---",
	"�������� �� �����",
	"���������� �� �����",
    "����� �� �����",
	NULL
};


void empty_function (int mode)
{
  printf("\n\n    ������ ������� !!! -  ������ ����� %d\n\n   �� ����������� - ���������� ������ !!!\n\n",mode);
  _getch();  
}
//--------------------------------------------

int main()
{
	int mode, submenu, submenu2, submenu3, submenu4, submenu5, err, i = 0;
	FILE *fp_in = NULL,        //--- ������ �������� �� ������ ���� ---
		*fp_dict = NULL;     //--- ������ �������� �� ������� ���� �� ������� ---
	char name_file[256], name_dict[256] = { 0 }, text[256] = { 0 };
	//------------------------------------------------------
	char **dict = NULL;        //---���������  ����� �� ��������� �� ����������� ������ �� ������� ---
	int *count_words = NULL;   //---��������� int ����� �� ���� �� ���������� �� ������ � ������� ----
	int size_dict = SIZE_DICT; //---����� ������ �� ������� --
	int index_dict = 0;        //---�������� ������ �� ����������� �� ���� ���� � ������� -----------
	int status;

	//---------------------------------------
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	//---------------------------------------

	do
	{
		mode = menu(txt_menu_g);

		switch (mode)
		{
		case 1:
		{
				  submenu = menu(txt_menu_1);
				  switch (submenu)
				  {
				  case 1:
					  fp_in = stdin;
					  printf("\n���� �� ������������ - �� �� ��������� ������ �������� '2' � �������� ����� �� ������������. �� ���� ��������� CTRL+Z\n\n");
					  break;
				  case 2:
					  do
					  {
						  printf("\n�������� ��� �� ����� �� ���������:");
						  err = scanf("%s", name_file);
					  } while (err != 1);
					  if ((fp_in = fopen(name_file, "rt")) == NULL)
					  {
						  printf("\n������ <%s> �� ���� �� �� ������ - error No % d !!!\n", name_file, errno);
						  printf("ERRROR No %d %s", errno, strerror(errno));
					  }
					  else
						  printf("\n������ <%s> � ������� ������� �� ��������� !\n", name_file);
					  break;
				  }
				  break;
		}
		case 2:
			if (fp_in == NULL)
				printf("\n������ ������ ���� !!!");
			else
			{
				DeleteDictionary(dict, count_words, index_dict);
				dict = NULL;
				size_dict = SIZE_DICT;
				count_words = 0;
				index_dict = 0;
				rewind(fp_in);
				dict = MakeDictionary(fp_in, dict, &size_dict, &index_dict, &count_words, &status);
			}
			break;

		case 3:
			PrintDictionary(dict, index_dict, count_words, status);
			break;

		case 4:
			printf("\n\n�������� ���� �� ������� � �������:");
			err = scanf("%s", text);
			if (err != 1)
				printf("\n\n������ ��� ����������� �� ���� �� �������!!!\n\n");
			else
			{
				cyr_lat_toupperCP1251(text);
				i = FindInToDictionary(dict, index_dict, text);
				if (i == -1)
					printf("\n\n������ <%s> �� �� ������� � ������� !!!\n\n", text);
				else
					printf("\n\n������ <%s> �� ������� � ������� �� ������� %d � ��\n����� ��� ����� <%s> %d ����\n\n",
					text, i, name_file, count_words[i]);
			}
			break;

		case 5:
			if (dict == NULL)
				printf("\n�� ��� ������� ������!\n");

			else
			{
				submenu2 = menu(txt_menu_2);
				SortDict(dict, index_dict, count_words, submenu2);
				printf("\n������!\n");
			}
			break;
		case 6:
			submenu3 = menu(txt_menu_3);
			switch (submenu3)
			{
			case 1:
				if (dict == NULL)
					printf("\n�� ��� ������� ������!\n");

				else
				{
					submenu4 = menu(txt_menu_4);
					SaveDictionary(dict, index_dict, count_words, name_file, name_dict, submenu4, submenu);
				}
				break;
			case 2:
				do
				{
					printf("\n�������� ��� �� �����, �� ����� ������ �� �������� ������:");
					err = scanf("%s", name_dict);
				} while (err != 1);
				if ((fp_dict = fopen(name_dict, "rt")) == NULL)
				{
					printf("\n������ <%s> �� ���� �� �� ������ - error No % d !!!\n", name_dict, errno);
					printf("ERRROR No %d %s", errno, strerror(errno));
				}
				else
				{
					printf("\n�������� � ������� ������� �� ����� %s \n!", name_dict);
					DeleteDictionary(dict, count_words, index_dict);
					dict = NULL;
					size_dict = SIZE_DICT;
					count_words = 0;
					index_dict = 0;
					rewind(fp_dict);
					dict = ReadDictionary(fp_dict, dict, &size_dict, &index_dict, &count_words, &status);
				}
				break;
			}
			break;

		case 7:
			submenu5 = menu(txt_menu_5);
				if (name_dict[1])
					SaveCodeFile(fp_in, dict, index_dict, name_file, name_dict, submenu5);
				else
					printf("\n�� ��� ������� ���� � �������\n");
			break;
		}
	} while (mode != 8);
}

