#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <readline/readline.h>
#include "table.h"

// 0 - ok
// 1 - wrong key
// 2 - wrong par/child
// 3 - special

int stoui(const char * src){
	for(int i = 0; i < (int)strlen(src); i++)
		if(!isdigit(src[i]))
			return -1;
	return atoi(src);
}

int main(){
	int size = -1, res, key, par;
	char * input;
	char buffer[256];
	Table * table = NULL;
	start:
	if((input = readline("1 - Заполнение вручную 2 - Чтение из файла\n")) != NULL){
		if(input[0] == '1' && input[1] == '\0'){
			free(input);
			while(size == -1 && (input = readline("Введите размер таблицы (1 или больше), "
					"для отмены введите q\n")) != NULL){
				if(input[0] == 'q' && input[1] == '\0')
					goto start;
				size = stoui(input);
				free(input);
			}
			if(size == -1)
				goto end;
			table = mktab(size);
		}
		else if(input[0] == '2' && input[1] == '\0'){
			free(input);
			while((input = readline("Введите имя файла, для отмены введите q\n")) != NULL){
				if(input[0] == 'q' && input[1] == '\0')
					goto start;
				res = readfromfile(&table, input);
				if(res == 0)
					break;
				else if(res == -1)
					printf("Данные в файле некорректные\n");
				else
					printf("Файл не существует/поврежден/не текстовый\n");
				free(input);
			}
			if(res != 0)
				goto end;
		}
		else{
			free(input);
			goto start;
		}
	}
	else
		goto end;
	while((input = readline("1 - Вставить элемент\n2 - Удалить элемент по ключу\n3 - Найти элемент по ключу\n"
				"4 - Выбрать элементы по диапазону родителей\n5 - Вывести таблицу\n")) != NULL){
		switch(input[0] + input[1]){
			case 49:
				printf("Ведите ключ, ключ родителя и информацию через пробел\n");
				if((res = scanf("%d %d %255[^\n]%*[^\n]%*c", &key, &par, buffer)) == 3){
					if(key < 1 || par < 0)
						printf("Неверный формат\n");
					else{
						res = insert(table, key, par, buffer);
						switch(res){
							case 3:
								printf("Таблица заполнена\n");
								break;
							case 2:
								printf("Такого родителя не существует\n");
								break;
							case 1:	
								printf("Такой ключ уже сущеествует\n");
								break;
							default:
								printf("OK\n");
								break;
						}
					}
				}
				else
					printf("Неверный формат\n");
				break;
			case 50:
				printf("Введите ключ\n");
				if((res = scanf("%d%*[^\n]%*c", &key)) == 1){
					if(key < 1)
						printf("Неверный формат\n");
					else{
						res = safedel(table, key);
						switch(res){
							case 1:
								printf("Такого ключа не существует\n");
								break;
							case 2:
								printf("Ключ имеет потомков\n");
								break;
							default:
								printf("OK\n");
								break;
						}
					}
				}
				else
					printf("Неверный формат\n");
				break;
			case 51:
				printf("Введите ключ\n");
				if((res = scanf("%d%*[^\n]%*c", &key)) == 1){
					if(key < 1)
						printf("Неверный формат\n");
					else{
						res = findkey(table, key, &par, input);
						switch(res){
							case 1:
								printf("Такого ключа не существует\n");
								break;
							default:
								printf("Ключ - %d, Родитель - %d, Инфо - %s\n", key, par, input);
								break;
						}
					}
				}
				else
					printf("Неверный формат\n");
				break;
			case 52:
				printf("Введите нижнюю и верхнюю границы через пробел\n");
				if((res = scanf("%d %d%*[^\n]%*c", &key, &par)) == 2){
					if(key < 1 || par < 0)
						printf("Неверный формат\n");
					else{
						res = selbypar(table, key, par);
						switch(res){
							// TODO
						}
					}
				}
				else
					printf("Неверный формат\n");
				break;
			case 53:
				showtab(table);
				break;
			default:
				printf("Нет такого варианта\n");
				break;
		}
		printf("\n");
	}
	end:
	if(input)
		free(input);
	if(table)
		deltab(table);
	return 0;
}



