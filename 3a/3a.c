#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#include <vld.h>
#include <locale.h>
#define M 20

const char *MSGS[] = { "0. Выйти", "1.Добавить элемент", "2. Удалить элемент с составным ключом ", "3.Очистить таблицу ", "4. Вывести таблицу на экран ","5.Поиск в таблице всех версий элемента 1 пространства, заданного ключом  результатом поиска должна быть новая таблица, содержащая найденные элементы.","6.Удалить элемент из первого пространства ключей","7.Поиск в таблице всех версий элемента 2 пространства, заданного ключом  результатом поиска должна быть новая таблица, содержащая найденные элементы.","8.Удалить элемент из второго пространства ключей" };

const int MSGS_SIZE = sizeof(MSGS) / sizeof(MSGS[0]);
int getInt(int *a)
{
	int n;
	do {
		n = scanf("%d", a, sizeof(int));
		scanf("%*c");
		if (n < 0)
			return 0;
		if (n == 0) {
			printf("%s\n", "Error! Repeat input");
			scanf("%*c", 0);
		}
	} while (n == 0);
	return 1;
}
int dialog(const char *msgs[], int N)
{


	char *errmsg = "";
	int rc;
	int i, n;
	do {
		puts(errmsg);
		errmsg = "You are wrong. Repeate, please!";

		for (i = 0; i < N; ++i)
			puts(msgs[i]);
		puts("Make your choice: --> ");
		n = getInt(&rc);
		if (n == 0)
			rc = 0;
	} while (rc < 0 || rc >= N);
	return rc;
}
char* get_str() {
	char buf[81] = { 0 };
	char* res = NULL;
	int len = 0;
	int n = 0;
	do {
		n = scanf("%80[^\n]", buf);
		if (n < 0) {
			if (!res) {
				return NULL;
			}
		}
		else if (n > 0) {
			int chunk_len = strlen(buf);
			int str_len = len + chunk_len;
			res = realloc(res, str_len + 1);
			memcpy(res + len, buf, chunk_len);
			len = str_len;
		}
		else {
			scanf("%*c");
		}
	} while (n > 0);

	if (len > 0) {
		res[len] = '\0';
	}
	else {
		res = calloc(1, sizeof(char));
	}

	return res;
}

char* get_str2() {
	char buf[81] = { 0 };
	char* res = NULL;
	int len = 0;
	int n = 0;
	do {
		n = scanf("%80[^\n]", buf);
		if (n < 0) {
			if (!res) {
				return NULL;
			}
		}
		else if (n > 0) {
			int chunk_len = strlen(buf);
			int str_len = len + chunk_len;
			res = realloc(res, str_len + 1);
			memcpy(res + len, buf, chunk_len);
			len = str_len;
		}
		else {
			scanf("%*c");
		}
	} while (n > 0);
	if (len > M)
		return NULL;
	if (len > 0) {
		res[len] = '\0';
	}
	else {
		res = calloc(1, sizeof(char));
		return NULL;
	}
	return res;
}
typedef struct Item {

	char *info;
	int release;
	char* key1;
	int key2;
	struct Item* next;

} Item;

typedef struct KeySpace2 {

	int key;
	Item* info;
	int release;
	struct KeySpace2* next;

} KeySpace2;

typedef struct KeySpace1 {

	char* key;
	int release;
	Item* info;
	struct KeySpace1* next;

} KeySpace1;

typedef struct ks2htpointer {

	KeySpace2* head;
	KeySpace2* tail;

} ks2htpointer;

typedef struct itemhtpointer {

	Item* head;
	Item* tail;

} itemhtpointer;

typedef struct ks1htpointer {

	KeySpace1* head;
	KeySpace1* tail;

} ks1htpointer;

typedef struct Table {

	int msize2;
	ks2htpointer **mas;
	KeySpace2**massiv;
	ks1htpointer* ks1;
	itemhtpointer* ptr;

} Table;

int hash(int key, Table* table) {
	return key % table->msize2;
}
Table* initable(int msize2) {
	Table* table = (Table*)malloc(sizeof(Table));
	table->msize2 = msize2;
	table->ks1 = (ks1htpointer*)malloc(sizeof(ks1htpointer));
	table->ks1->head = table->ks1->tail = NULL;
	/*table->massiv = (KeySpace2**)malloc(sizeof(KeySpace2*)*table->msize2);
	for (int i = 0; i < table->msize2; i++) {
		table->massiv[i] = NULL;
	}*/
	table->ks1 = (ks1htpointer*)malloc(sizeof(ks1htpointer));
	table->ks1->head = NULL;
	table->ks1->tail = NULL;
	table->ptr = (itemhtpointer*)malloc(sizeof(itemhtpointer));
	table->ptr->head = NULL;
	table->ptr->tail = NULL;
	table->mas = (ks2htpointer**)malloc(sizeof(ks2htpointer*)*table->msize2);
	for (int i = 0; i < table->msize2; i++) {
		table->mas[i] = (ks2htpointer*)malloc(sizeof(ks2htpointer));
		table->mas[i]->head = NULL;
		table->mas[i]->tail = NULL;
	}
	return table;
}
int add_ks1(Table* table, Item* item, char* key1) 
{
	int release = 1;
	if (!table->ks1->head)
	{
		KeySpace1* ptr = (KeySpace1*)malloc(sizeof(KeySpace1));
		if (!ptr)
			return 0;
		ptr->key = key1;
		ptr->release = 1;
		ptr->info = item;
		ptr->next = NULL;
		table->ks1->head = ptr;
}
	else {
		KeySpace1* ptr = (KeySpace1*)malloc(sizeof(KeySpace1));
		if (!ptr)
			return 0;
		ptr->key = key1;
		KeySpace1* pointer = table->ks1->head;
		while (pointer) {
			if (strcmp(pointer->key ,key1) == 0)
				release++;
			pointer = pointer->next;
		}
		ptr->release = release;
		ptr->info = item;
		ptr->next = table->ks1->head;
		table->ks1->head = ptr;
	}
	return 1;
}
int add2(Table* table, Item* item, int key2) {
	int index = key2 % table->msize2;
	int release = 1;
	if (!table->mas[index]->head) 
	{
		KeySpace2* ptr = (KeySpace2*)malloc(sizeof(KeySpace2));
		if (!ptr)
			return 0;
		ptr->key = key2;
		ptr->release = release;
		ptr->info = item;
		ptr->next = NULL;
		//table->massiv[index] = ptr;
		table->mas[index]->head = ptr;
}
	else {
		KeySpace2* ptr = (KeySpace2*)malloc(sizeof(KeySpace2));
		if (!ptr)
			return 0;
		ptr->key = key2;
		KeySpace2* pointer = table->mas[index]->head;
		while (pointer) {
			if (pointer->key == key2)
				release++;
			pointer = pointer->next;
		}
		ptr->release = release;
		ptr->info = item;
		
		//table->massiv[index] = ptr;
		ptr->next = table->mas[index]->head;
		table->mas[index]->head = ptr;
	}
	return 1;
}
char *getStr()
{
	char buf[M + 1] = { '\0' };
	char *str = NULL;
	scanf("%10s", buf);
	str = buf;

	fseek(stdin, 0, SEEK_END);
	int num = ftell(stdin);
	fseek(stdin, 0, SEEK_SET);

	for (int i = 0; i < num; i++) {
		getchar();
	}

	return str;
}
int add_item(Table* table) {
	int key2;
	printf("Введите ключ 1:");
	char* key1 = get_str(); //getStr();= 
	printf("Введите ключ 2:");
	scanf("%d", &key2);
	scanf("%*c");
	printf("Введите информацию:");
	char* info = get_str();
	int index = hash(key2, table);
	Item* ptr = (Item*)malloc(sizeof(Item));
	int release = 1;
	if (!table->ptr->head) {
		if (!ptr)
			return 0;
		ptr->key1 = key1;
		ptr->key2 = key2;
		ptr->release = release;
		ptr->info = info;
		ptr->next = NULL;
		table->ptr->head = table->ptr->tail = ptr;
	}
	else {
		if (!ptr)
			return 0;
		ptr->key1 = key1;
		ptr->key2 = key2;
		Item* pointer = table->ptr->head;
		while (pointer) {
			if (strcmp(pointer->key1,key1) == 0 && pointer->key2 == key2)
				release++;
			pointer = pointer->next;
		}
		ptr->release = release;
		ptr->info = info;
		ptr->next = table->ptr->head;
		table->ptr->head = ptr;


	}
	add_ks1(table, ptr, key1);
	add2(table, ptr, key2);
	return 1;
}
int print(Table* table) 
{
	Item* ptr = table->ptr->head;
	while (ptr) {
		printf("Item: информация:'%s',релиз:'%d',ключ1:'%s',ключ2:'%d'\n", ptr->info, ptr->release, ptr->key1, ptr->key2);
		ptr = ptr->next;
	}
	printf("\n");
	KeySpace1* pointer1 = table->ks1->head;
	while (pointer1) {
			printf("ks1: информация:'%s',релиз:'%d',ключ1:'%s'\n",pointer1->info->info,  pointer1->release, pointer1->key);
		
		//printf("%d\n", pointer1->info);
		pointer1 = pointer1->next;
	
	}
	printf("\n");
	for(int i = 0;i<table->msize2;i++)
	{
		KeySpace2* p = table->mas[i]->head;
		if (!p) {
			printf("*\n");
			continue;
		}
		
		
		while (p) {
		
				printf("ks2: информация:'%s',релиз:'%d',ключ1:'%d'  ",p->info->info, p->release, p->key);
			
			p = p->next;
		}
		printf("\n");
		
	}
	//printf("%d , %d", table->massiv[m]->key, table->massiv[m]->release);
	return 1;
}

int clear(Table* table) 
{
	if (!table->ptr->head) 
	{
		printf("The table has cleaned!!!");
		return 1;
	}
	KeySpace1* helper = table->ks1->head;
	while (helper) {
		if (helper->next) {
			table->ks1->head = helper->next;
			//free(p->info);
			free(helper);
			helper = table->ks1->head;
		}
		else {
			free(helper);
			helper = NULL;
			table->ks1->head = NULL;

		}
	}
	for (int i = 0; i < table->msize2; i++) {
		KeySpace2* hp = table->mas[i]->head;
		while (table->mas[i]->head) {
			if (hp->next)
			{
				table->mas[i]->head = hp->next;
				free(hp);
				hp = table->mas[i]->head;
			}
			else {
				free(hp);
				hp = NULL;
				table->mas[i]->head = NULL;

			}
		}
	}
	Item* p = table->ptr->head;
	while (p) {
		if (p->next) {
			table->ptr->head = p->next;
			free(p->info);
			free(p);
			p = table->ptr->head;
		}
		else {
			free(p->info);
			free(p);
			p = NULL;
			table->ptr->head = NULL;

		}

		//free(table->ptr->tail->info);
		
	}
	for (int i = 0; i < table->msize2; i++) {
		free(table->mas[i]);

	}
	free(table->mas);
	free(table->ks1->head);
	free(table->ks1);
	free(table);
	
	printf("Your table was cleaned!");
	return 1;
}

void print2(Table* table2) {
	KeySpace1* pointer1 = table2->ks1->head;
	while (pointer1) {
		
			printf("ks1:информация:'%s',релиз:'%d',ключ1:'%s'\n", pointer1->info->info, pointer1->release, pointer1->key);
		
		pointer1 = pointer1->next;
	}
}


int onekey_del(Table* table) {

	printf("Введите ключ 1:");
	char* key1 = get_str();
	printf("Введите ключ 2:");
	int key2;
	scanf("%d", &key2);
	Item* ptr = table->ptr->head;
	while (ptr)
	{

		if (strcmp(ptr->key1, key1) == 0 && ptr->key2 == key2)
		{
			KeySpace1* help = table->ks1->head;
			while (help)
			{
				if (help->info == ptr)
				{
					if (help == table->ks1->head)
					{
						//KeySpace1* help2 = help;
						help = help->next;
						free(table->ks1->head);
						table->ks1->head = help;
						break;
						//return 1;
					}

					else {
						KeySpace1* help5 = help;
						help = help->next;
						free(help5);
						break;
						//return 1;
					}
				}

				if (help->next) {
					if (help->next->info == ptr)
					{
						KeySpace1* help3 = help->next;
						help->next = help3->next;
						free(help3);
						break;
						//return 1;
					}
				}

				help = help->next;
			}
			int index = ptr->key2%table->msize2;
			KeySpace2* pointer2 = table->mas[index]->head;
			while (pointer2)
			{
				if (pointer2->info == ptr) {
					if (pointer2 == table->mas[index]->head)
					{
						//KeySpace2*pointer221 = pointer2;
						pointer2 = pointer2->next;
						free(table->mas[index]->head);
						table->mas[index]->head = pointer2;
						break;
						//return 1;
					}
					else
					{
						KeySpace2*pointer21 = pointer2;
						pointer2 = pointer2->next;
						free(pointer21);
						break;
						//return 1;
					}
				}
				if (pointer2->next->info == ptr) {
					KeySpace2* pointer22 = pointer2->next;
					pointer2->next = pointer22->next;
					free(pointer22);
					break;
					//return 1;
				}
				pointer2 = pointer2->next;
			}
			if (ptr == table->ptr->head) {

				ptr = ptr->next;
				free(table->ptr->head->info);
				free(table->ptr->head);
				table->ptr->head = ptr;
				continue;
				//return 1;

			}

			Item*h = ptr;
			ptr = h->next;
			free(h->info);
			free(h);
			continue;
			//return 1;
		}
		if (ptr->next)
		{
			Item* helper = ptr->next;
			if (strcmp(helper->key1, key1) == 0 && helper->key2 == key2)
			{
				KeySpace1* burn = table->ks1->head;
				while (burn)
				{
					if (burn->info == helper)
					{
						if (burn == table->ks1->head)
						{
				
							burn = burn->next;
							free(table->ks1->head);
							table->ks1->head = burn;
							break;
							//return 1;
						}

						else {
						
							KeySpace1* help5 = table->ks1->head;
							while (help5->next != burn)
								help5 = help5->next;
							burn = burn->next;
							free(help5->next);
							help5->next = burn;
							break;
		
						}
					}

					burn = burn->next;

				}
				int ind = helper->key2%table->msize2;

				KeySpace2* point = table->mas[ind]->head;
				while (point)
				{
					if (point->info == helper)
					{
						if (point == table->mas[ind]->head)
						{
			
							point = point->next;
							free(table->mas[ind]->head);
							table->mas[ind]->head = point;
							break;
				
						}
						else
						{

							KeySpace2*pointer21 = table->mas[ind]->head;
							while (pointer21->next->info!=helper)
								pointer21 = pointer21->next;
							point = point->next;
							free(pointer21->next);
							pointer21->next = point;
							break;
		
						}
					}
			
					point = point->next;
				}

				ptr->next = helper->next;
				free(helper->info);
				free(helper);
				continue;
				//return 1;
			}

		}

		ptr = ptr->next;

	}
	return 1;
}


int func1ks1(Table* table)
{
	printf("Введите ключ 1:");
	char* key1 = get_str();
	Table* table2 = initable(0);
	KeySpace1* pointer = table->ks1->head;
	while (pointer) 
	{
		if (strcmp(pointer->key, key1) == 0)
		{
			add_ks1(table2, pointer->info, key1);
		}
		pointer = pointer->next;
	}
	printf("\n");
	print2(table2);
	printf("\n");
    clear(table2); 
	printf("\n");
	return 1;
}

int ks1func2(Table* table)
{
	printf("Введите ключ 1:\n");
	char* key1 = get_str();

	printf("\n");
	//printf("Waiting for program!\n");
	KeySpace1* help = table->ks1->head;
	while (help)
	{
		if (strcmp(help->key,key1) == 0)
		{
			if (help == table->ks1->head)
			{
				//KeySpace1* help2 = help;
				help = help->next;
				free(table->ks1->head);
				table->ks1->head = help;
				break;
				//return 1;
			}

			else {
				KeySpace1* help5 = help;
				help = help->next;
				free(help5);
				break;
				//return 1;
			}
		}

		if (help->next) {
			if (strcmp(help->next->key, key1) == 0)
			{
				KeySpace1* help3 = help->next;
				help->next = help3->next;
				free(help3);
				break;
				//return 1;
			}
		}

		help = help->next;
	}
	return 1;

}

int ks2func2(Table* table) {

	printf("Введите ключ 2:\n");
	int key2;
	scanf("%d", &key2);
	int ind = key2%table->msize2;

	KeySpace2* point = table->mas[ind]->head;
	while (point)
	{
		if (point->key == key2)
		{
			if (point == table->mas[ind]->head)
			{

				point = point->next;
				free(table->mas[ind]->head);
				table->mas[ind]->head = point;
				//break;
				continue;

			}
			else
			{

				KeySpace2*pointer21 = table->mas[ind]->head;
				while (pointer21->key == key2)
					pointer21 = pointer21->next;
				point = point->next;
				free(pointer21->next);
				pointer21->next = point;
				//break;
				continue;
			}
		}

		point = point->next;
	}

	return 1;
}


void print3(Table* table)
{
	for (int i = 0; i < table->msize2; i++)
	{
		KeySpace2* p = table->mas[i]->head;
		if (!p) {
			printf("*\n");
			continue;
		}


		while (p) {
		
				printf("ks2: информация:'%s',релиз:'%d',ключ1:'%d'  ", p->info->info, p->release, p->key);
			
			p = p->next;
		}
		printf("\n");

	}

}

int ks2function1(Table* table) 
{
	Table* tb = initable(table->msize2);
	printf("Введите ключ 2:\n");
	int key2;
	scanf("%d", &key2);
	//Item* ptr = table->ptr->head;
	for (int i = 0; i < table->msize2; i++) {

		KeySpace2* pointer = table->mas[i]->head;
		while (pointer) {
			if (pointer->key == key2) {
				add2(tb, pointer->info, key2);
			}
			pointer = pointer->next;
		}
		//printf("\n");

	}
	print3(tb);
	clear(tb);
	free(tb);
	return 1;
}

int(*fptr[])(Table*) = { NULL,add_item, onekey_del,clear,print,func1ks1,ks1func2,ks2function1,ks2func2 };

int main()
{
	setlocale(LC_ALL, "");
	Table* table = initable(10);
	int rc;
	while (rc = dialog(MSGS, MSGS_SIZE))
		if (!fptr[rc](table))
			break;
	printf("That's all. Bye!\n");
	clear(table);
	return 0;
}