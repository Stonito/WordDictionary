#include <visual_2013.h>
#include <stdio.h>
#include <conio.h>

#include <windows.h>

#include "menu.h"
#include "dict_func.h"


//------------- менюта -----------------------
char *txt_menu_g[]=
{
	  "Програма <РЕЧНИК>\n--------------------",
		"Избор на входен текст",
		"Създване на речник на текста",
		"Извеждане речника на екрана",
		"Търсене на дума в речника",
		"Сортиране на речника",
    "Запис/зареждане на речник",
    "Кодиране/декодиране на текста",
		"КРАЙ",
		NULL
};

char *txt_menu_1[]=
{
			"---ВХОДЕН ТЕКСТ---",
			"От клавиатура",
			"От текстови файл",
      "ОТКАЗ от избор",
      NULL
};

char *txt_menu_2[]=
{
   "ИЗБОР НА ТИПА СОРТИРОВКА:",
		"По азбучен ред",
		"По брой срещания на думата в текста",
    "ОТКАЗ от избор",
		NULL
};

char *txt_menu_3[]=
{
   "---ОБМЕН на РЕЧНИКА---",
		"Запис във файл",
		"Зареждане от файл",
    "ОТКАЗ от избор",
		NULL
};
char *txt_menu_4[]=
{
	"Избор на начин за записване:",
	"Всяка дума на нов ред и срещу нея честотата.",
	"Записване на бройката в отделен файл с разширение \".cnt\".",
	"Назад",
	NULL
};
char *txt_menu_5[]=
{
   "---ОБРАБОТКА---",
	"Кодиране на текст",
	"Декодиране на текст",
    "ОТКАЗ от избор",
	NULL
};


void empty_function (int mode)
{
  printf("\n\n    Празна функция !!! -  избран режим %d\n\n   за продължение - произволен клавиш !!!\n\n",mode);
  _getch();  
}
//--------------------------------------------

int main()
{
	int mode, submenu, submenu2, submenu3, submenu4, submenu5, err, i = 0;
	FILE *fp_in = NULL,        //--- файлов указател за входен файл ---
		*fp_dict = NULL;     //--- файлов указател за изходен файл на речника ---
	char name_file[256], name_dict[256] = { 0 }, text[256] = { 0 };
	//------------------------------------------------------
	char **dict = NULL;        //---динамичен  масив от указатели за съхраняване думите на речника ---
	int *count_words = NULL;   //---динамичен int масив за броя на срещанията на думите в речника ----
	int size_dict = SIZE_DICT; //---текущ размер на речника --
	int index_dict = 0;        //---актуален индекс за съхраняване на нова дума в речника -----------
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
					  printf("\nВход от клавиатурата - за да създадете речник изберете '2' и въведете текст от клавиатурата. За край натиснете CTRL+Z\n\n");
					  break;
				  case 2:
					  do
					  {
						  printf("\nВъведете име на фаила за обработка:");
						  err = scanf("%s", name_file);
					  } while (err != 1);
					  if ((fp_in = fopen(name_file, "rt")) == NULL)
					  {
						  printf("\nФайлът <%s> не може да се отвори - error No % d !!!\n", name_file, errno);
						  printf("ERRROR No %d %s", errno, strerror(errno));
					  }
					  else
						  printf("\nФайлът <%s> е отворен успешно за обработка !\n", name_file);
					  break;
				  }
				  break;
		}
		case 2:
			if (fp_in == NULL)
				printf("\nЛипсва входен файл !!!");
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
			printf("\n\nВъведете дума за търсене в речника:");
			err = scanf("%s", text);
			if (err != 1)
				printf("\n\nГРЕШКА при въвеждането на дума за търсене!!!\n\n");
			else
			{
				cyr_lat_toupperCP1251(text);
				i = FindInToDictionary(dict, index_dict, text);
				if (i == -1)
					printf("\n\nДумата <%s> не се съдържа в речника !!!\n\n", text);
				else
					printf("\n\nДумата <%s> се съдържа в речника на позиция %d и се\nсреща във файла <%s> %d пъти\n\n",
					text, i, name_file, count_words[i]);
			}
			break;

		case 5:
			if (dict == NULL)
				printf("\nНе сте създали речник!\n");

			else
			{
				submenu2 = menu(txt_menu_2);
				SortDict(dict, index_dict, count_words, submenu2);
				printf("\nГотово!\n");
			}
			break;
		case 6:
			submenu3 = menu(txt_menu_3);
			switch (submenu3)
			{
			case 1:
				if (dict == NULL)
					printf("\nНе сте създали речник!\n");

				else
				{
					submenu4 = menu(txt_menu_4);
					SaveDictionary(dict, index_dict, count_words, name_file, name_dict, submenu4, submenu);
				}
				break;
			case 2:
				do
				{
					printf("\nВъведете име на фаила, от който искате да заредите речник:");
					err = scanf("%s", name_dict);
				} while (err != 1);
				if ((fp_dict = fopen(name_dict, "rt")) == NULL)
				{
					printf("\nФайлът <%s> не може да се отвори - error No % d !!!\n", name_dict, errno);
					printf("ERRROR No %d %s", errno, strerror(errno));
				}
				else
				{
					printf("\nРечникът е успешно зареден от файла %s \n!", name_dict);
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
					printf("\nНе сте създали файл с речника\n");
			break;
		}
	} while (mode != 8);
}

