#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <readline/readline.h>
#include "table.h"

int stoui(const char * src){
	for(int i = 0; i < (int)strlen(src); i++)
		if(!isdigit(src[i]))
			return -1;
	return atoi(src);
}

int main(){
	int size = -1, res, key, rel;
	KeySpace * elem;
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
				res = importfile(&table, input);
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
	while((input = readline("1 - Вставить\n2 - Удалить\n3 - Найти по ключу и версии\n"
				"4 - Найти все версии по ключу\n5 - Вывод\n6 - Экспорт в файл\n")) != NULL){
		switch(input[0] + input[1]){
			case 49:
				printf("Ведите ключ и информацию\n");
				if((res = scanf("%d %255[^\n]%*[^\n]%*c", &key, buffer)) == 2){
					if(key < 1)
						printf("Неверный формат\n");
					else{
						if((res = insert(table, key, buffer)) == 0)
							printf("OK\n");
						else
							printf("Таблица заполнена\n");
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
						if((res = delete(table, key)) == 0)
							printf("OK\n");
						else
							printf("Такого ключа нет\n");
					}
				}
				else
					printf("Неверный формат\n");
				break;
			case 51:
				printf("Введите ключ и версию\n");
				if((res = scanf("%d %d%*[^\n]%*c", &key, &rel)) == 2){
					if(key < 1 || rel < 1)
						printf("Неверный формат\n");
					else{
						if((res = find(table, key, rel, &elem)) == 0)
							printf("Ключ:%d Версия:%d Инфо:%s\n", key, rel, elem->info);
						else
							printf("Такой пары ключа и версии нет\n");
						}
				}
				else
					printf("Неверный формат\n");
				break;
			case 52:
				printf("Введите ключ\n");
				if((res = scanf("%d%*[^\n]%*c", &key)) == 1){
					if(key < 1)
						printf("Неверный формат\n");
					else{
						if((res = findall(table, key, &elem, &rel)) == 0){
							printf("Ключ:%d\n", key);
							for(int b = 0; b < rel; b++)
								printf("Версия:%d Инфо:%s\n", elem[b].rel, elem[b].info);
						}
						else
							printf("Такого ключа нет\n");		
					}
				}
				else
					printf("Неверный формат\n");
				break;
			case 53:
				printtab(table);
				break;
			case 54:
				printf("Введите имя файла\n");
				if((res = scanf("%255[^\n]%*[^\n]%*c", buffer)) == 1){
					if((res = exportfile(table, input)) == 0)
						printf("OK\n");
					else
						printf("\n");
				}
				else{
					printf("Неверный формат\n");
				}
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
		droptab(table);
	return 0;
}



