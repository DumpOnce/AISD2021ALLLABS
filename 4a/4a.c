#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <locale.h>
//#include <vld.h>

const char *MSGS[] = { "0.Выйти","1.Добавить элемент в таблицу","2.Удалить элемент из таблицы","3.Вывести таблицу","4.Найти минимум","5.Вывести все элементы с заданным порядком числового ключа" };//{ "0. Выйти", "1.Добавить элемент", "2. Удалить элемент с составным ключом ", "3.Очистить таблицу ", "4. Вывести таблицу на экран ","5.Поиск в таблице всех версий элемента 1 пространства, заданного ключом  результатом поиска должна быть новая таблица, содержащая найденные элементы.","6.Удалить элемент из первого пространства ключей","7.Поиск в таблице всех версий элемента 2 пространства, заданного ключом  результатом поиска должна быть новая таблица, содержащая найденные элементы.","8.Удалить элемент из второго пространства ключей" };

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

int tabs = 0;
typedef struct Tree {
	int key;
	struct Tree* left;
	struct Tree* right;
	char* info;
}Tree;

typedef struct Table {
	Tree* tree;
	FILE* fin;
}Table;

void m_function(Tree* tree, int p) {

	if (tree) {

		m_function(tree->left, p);
		m_function(tree->right, p);
		if (abs(tree->key / (p * 10)) > 1&& abs(tree->key / (p * 10))<10)
			printf("Ключ:'%d',Информация:'%s'\n", tree->key, tree->info);

	}


}


int any_key(Table* table) {

	int poryadok;
	printf("Введите необходимый разряд ключа:\n");
	scanf("%d", &poryadok);
	m_function(table->tree,poryadok);
	return 1;

}




Tree* search_min(Table* table)
{
	Tree* ptr = table->tree;
	while (ptr->left != NULL)
	{
		ptr = ptr->left;
	}
	printf("Element has found!\n");
	return ptr;
}

int minimum(Table* table)
{
	printf("Элемент с минимальным ключом:информация:'%s',ключ;'%d'\n", search_min(table)->info, search_min(table)->key);
	return 1;
}

void pr(Tree* tree, int level) 
{
	if (tree) {

		pr(tree->right, level + 1);
		for (int i = 0; i < level; i++) {
			printf("	");
		}
		printf("%d\n", tree->key);
		pr(tree->left, level + 1);
	}
}




Tree* func(Tree* tree, int key, char* info)
{
	if (!tree)
	{
		Tree* t = (Tree*)malloc(sizeof(Tree));
		t->key = key;
		t->info = info;
		t->left = t->right = NULL;
	
		return t;
	}
	if (key > tree->key)
		tree->right = func(tree->right, key, info);
	if (key < tree->key)
		tree->left = func(tree->left, key, info);
	//balance(tree);
	return tree;
}

Tree* funct(Table* table)
{

	int key;

	char* info;
	printf("Enter key:\n");
	scanf("%d", &key);
	scanf("%*c");
	printf("Enter info:\n");
	info = get_str();
	Tree* help = func(table->tree, key, info);


	return help;
	
}

void treeprint(Tree* tree)
{
	if (tree != NULL)
	{

		treeprint(tree->left);
		printf("info:'%s',key:'%d'  ", tree->info, tree->key);
		treeprint(tree->right);
	}
}
void  function_print(Table* table)
{
	treeprint(table->tree);
}


void tell(Tree* tree) {

	if (tree) {
		tell(tree->right);
		printf("%d  ", tree->key);
		tell(tree->left);


	}


}

void FreeTree(Tree* node)
{
	if (!node)
		return;
	FreeTree(node->left);
	FreeTree(node->right);
	free(node->info);
	node->key = 0;
	free(node);
	return;
}

int printik(Table* table) {
	pr(table->tree, 1);
	printf("\n\n");
	tell(table->tree);
	return 1;
}



void print_Tree(Tree * p, int level)
{
	if (p)
	{
		print_Tree(p->left, level + 1);
		for (int i = 0; i < level; i++) printf("   ");
		printf("%d", p->key);
		print_Tree(p->right, level + 1);
	}
}


int algo(Table* table)
{
	int key;
	printf("Enter key of searching element:\n");
	scanf("%d", &key);
	Tree* ptr = table->tree;
	Tree* point = ptr;
	while (ptr) 
	{
		if (ptr->key == key)
		{
			if (ptr->left)
			{
				Tree* ptr2 = ptr->left;
				if (ptr2->right)
				{
					while (ptr2->right->right)
					{
						ptr2 = ptr2->right;
					}
					ptr->key = ptr2->right->key;
					free(ptr->info);
					ptr->info = ptr2->right->info;
					Tree* pp = ptr2->right;
					ptr2->right = ptr2->right->left;
					free(pp);
					return 1;
				}
				else {
					if (ptr2->left)
					{
						ptr->key = ptr2->key;
						free(ptr->info);
						ptr->info = ptr2->info;
						ptr->left = ptr2->left;
						free(ptr2);
						return;
					}
					else {
						ptr->key = ptr2->key;
						free(ptr->info);
						ptr->info = ptr2->info;
						free(ptr2);
						ptr->left = NULL;
						return 1;
					}
				}
			}
			if (ptr->right)
			{
				Tree* ptr3 = ptr->right;
				if (ptr3->left)
				{
					while (ptr3->left->left)
					{
						ptr3 = ptr3->left;
					}
					ptr->key = ptr3->left->key;
					free(ptr->info);
					ptr->info = ptr3->left->info;
					Tree* p = ptr3->left;
					ptr3->left = ptr3->left->right;
					free(p);
					return 1;
				}
				else
				{
					if (ptr3->right)
					{
						ptr->key = ptr3->key;
						free(ptr->info);
						ptr->info = ptr3->info;
						ptr->right = ptr3->right;
						free(ptr3);
						return 1;
					}
					else
					{
						ptr->key = ptr3->key;
						free(ptr->info);
						ptr->info = ptr3->info;
						free(ptr3);
						ptr->right = NULL;
						return 1;
					}
				}
			}
			if (!ptr->right && !ptr->left) {
				if (point == ptr) {
					free(ptr->info);
					free(ptr);
					table->tree = NULL;
					return 1;
				}
				if (point->left == ptr)
				{
					free(ptr->info);
					free(ptr);
					point->left = NULL;
					return 1;
				}
				else {
					free(ptr->info);
					free(ptr);
					point->right = NULL;
					return;
				}
			}
		}
		else {
			if (key > ptr->key)
			{
				ptr = ptr->right;
				if (ptr)
					if (ptr->key != key)
						point = point->right;
			}
			else {
				ptr = ptr->left;
				if (ptr)
					if (ptr->key != key)
						point = point->left;
			}
		}
	}
	return 1;
}



Table* initable()
{
	Table* table = (Table*)malloc(sizeof(Table));
	table->tree = NULL;
	return table;
}


int insert(Table* table)
{
		table->tree = funct(table);	
		return 1;
}






int(*fptr[])(Table*) = { NULL,insert,algo,printik,minimum,any_key };// { NULL,add_item, onekey_del,clear,print,func1ks1,ks1func2,ks2function1,ks2func2 };




int main()
{
	setlocale(LC_ALL, "");
	Table* table = initable();
	int rc;
	while (rc = dialog(MSGS, MSGS_SIZE))
		if (!fptr[rc](table))
			break;
	FreeTree(table->tree);
	free(table);
	return 0;
}