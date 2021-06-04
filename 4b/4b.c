#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <locale.h>
#include <vld.h>

#define coef 0.5
static int num = 0;
const char *MSGS[] = { "0.Выйти","1.Добавить элемент в таблицу","2.Удалить элемент из таблицы","3.Вывести таблицу","4.Обратный обход","5.Поиск по ключу" };//,"4.Найти минимум","5.Вывести все элементы с заданным порядком числового ключа" };//,"6.Таймирование(вставка)" };//{ "0. Выйти", "1.Добавить элемент", "2. Удалить элемент с составным ключом ", "3.Очистить таблицу ", "4. Вывести таблицу на экран ","5.Поиск в таблице всех версий элемента 1 пространства, заданного ключом результатом поиска должна быть новая таблица, содержащая найденные элементы.","6.Удалить элемент из первого пространства ключей","7.Поиск в таблице всех версий элемента 2 пространства, заданного ключом результатом поиска должна быть новая таблица, содержащая найденные элементы.","8.Удалить элемент из второго пространства ключей" };

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
		puts("Make your choice: —> ");
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
#define N 10
typedef struct Info {

	struct InfoType* head;
	struct InfoType* tail;

}Info;



typedef struct InfoType
{

	char* info;
	int index;
	struct InfoType* next;

}InfoType;


typedef struct Tree
{
	int key;
	Info* inform;
	int hr, hl;
	int balance;
	struct Tree* left;
	struct Tree* right;
	struct Tree* next;
}Tree;

typedef struct Table {
	Tree* tree;
}Table;


typedef struct Stack
{
	struct Node* head;
}Stack;


typedef struct Node 
{

	Tree* tree;
	struct Node* next;

}Node;




void add_info(Tree* tree, char* info)
{


	if (tree->inform->head ==   NULL)
	{
		InfoType* i = (InfoType*)malloc(sizeof(InfoType));
		i->info = info;
		i->index = 1;
		i->next = NULL;
		tree->inform->head = tree->inform->tail = i;
	}
	else
	{
		InfoType* i2 =(InfoType*)malloc(sizeof(InfoType));
		i2->info = info;
		i2->next = NULL;
		i2->index = tree->inform->tail->index + 1;
		tree->inform->tail->next = i2;
		tree->inform->tail = tree->inform->tail->next;
	}
}


void del_info(Tree* tree) {


	if (tree->inform->head == NULL) {
		return;
	}

	if (tree->inform->head == tree->inform->tail)
	{
		free(tree->inform->tail->info);
		free(tree->inform->tail);
		tree->inform->head = tree->inform->tail = NULL;
		return;
	}
	else {

		InfoType* ptr = tree->inform->head;
		tree->inform->head = tree->inform->head->next;
		
		free(ptr->info);//////////////Обратить внимание на утечку тут <----------
		//ptr->home = NULL;
		free(ptr);
	}

}

static int f = 0;
static int n = 0;

Tree* bbfa(Tree **a, int i, int number)

{

	if (number <= 0)
		return NULL;
	int m = number / 2;

	a[i + m]->left = bbfa(a, i, m);

	a[i + m]->right =bbfa(a, i + m + 1, number - m - 1);

	return a[i + m];
}





void in_order(Tree* tree, Tree** mas)
{

	if (tree)
	{

		in_order(tree->left, mas);
		mas[num++] = tree;


		//printf("%d ", num);
		//index++;

		in_order(tree->right, mas);
		//index++;
		// index++;
		//???
	}


}




int balance(Tree* tree)
{

	if (!tree)
		return -1;
	int hl = balance(tree->left);
	hl++;
	tree->hl = hl;
	int hr = balance(tree->right);
	hr++;
	int hl2 = hl;
	int hr2 = hr;
	tree->hr = hr;
	tree->balance = hr + tree->hl + 1;
	hr = hl = 0;
	return hl2 + hr2;
}

int deep(Tree* tree)
{
	return tree ? tree->balance : 0;
}

int BF(Tree* tree)
{
	return tree->hr - tree->hl;
	//return deep(tree->right) - deep(tree->left);
}



Tree* bal(Tree* p)
{
	balance(p);

	if (abs(BF(p)) == 2)
	{
		n = p->balance;

		Tree** mas = (Tree**)malloc(sizeof(Tree*)*n);

		in_order(p, mas);
	
		p = bbfa(mas, f, n);

		num = 0;
		balance(p);
		free(mas);

	}

	return p;

}


void push(Stack* stack, Tree* item)
{


	if (!stack->head)
	{
		Node* ptr = (Node*)malloc(sizeof(Node));
		ptr->tree = item;
		ptr->next = NULL;
		stack->head = ptr;
	}
	else
	{

		Node* ptr2 = (Node*)malloc(sizeof(Node));
		ptr2->next = stack->head;
		ptr2->tree = item;
		stack->head = ptr2;
	}

	//stack->cap++;

}

void pop(Stack* stack)
{

	if (!stack->head)
		return;

	Node* ptr = stack->head;
	stack->head->tree = bal(stack->head->tree);
	stack->head = stack->head->next;
	free(ptr);

	return;
}



/*void m_function(Tree* tree, int p)
{

	if (tree) {

		m_function(tree->left, p);
		m_function(tree->right, p);
		if (abs(tree->key / (p * 10)) > 1 && abs(tree->key / (p * 10)) < 10)
			printf("Ключ:'%d',Информация:'%s'\n", tree->key, tree->info);

	}


}
*/

int any_key(Table* table) {

	/*int poryadok;
	printf("Введите необходимый разряд ключа:\n");
	scanf("%d", &poryadok);
	m_function(table->tree, poryadok);*/
	return 1;

}

Tree* find(Tree* node, int key)
{
	if (!node )
	{
		return NULL;  
	}
	if (node->key == key)
	{
		return node; 
	}

	if (key <= node->key)
	{

		if (node->left != NULL)
			return find(node->left, key); 
		else
		{
			return NULL; 
		}
	}
	else
	{
		if (node->right)
			return find(node->right, key);
		else
		{
			return NULL;
		}
	}
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
	printf("Элемент с минимальным ключом:ключ;'%d'\n", search_min(table)->key);
	return 1;
}

void pr(Tree* tree, int level)
{
	if (tree)
	{
		pr(tree->right, level + 1);
		for (int i = 0; i < level; i++)
		{
			printf("	");
		}
		printf("%d\n", tree->key);
		pr(tree->left, level + 1);
	}
}


int finding(Table* table)
{
	printf("Введите ключ искомого элемента:\n");
	int i;
	scanf("%d", &i);
	Tree* ptr = find(table->tree, i);
	if (ptr) 
	{
		printf("Искомый элемент с ключом '%d' информация:'%s'\n",i,ptr->inform->tail->info);
	}
	else 
	{
		printf("Элемент не найден\n");
	}
	return 1;
}



Tree* func(Tree* tree, int key, char* info)
{
	if (!tree)
	{
		Tree* t = (Tree*)malloc(sizeof(Tree));
		t->key = key;
		//t->info = info;
		t->inform =(Info*) malloc(sizeof(Info));
		t->inform->head = t->inform->tail = NULL;
		add_info(t, info);
		t->left = t->right = NULL;
		t->hl = t->hr = 0;
		return t;
	}
	if (key > tree->key)
		tree->right = func(tree->right, key, info);
	if (key < tree->key)
		tree->left = func(tree->left, key, info);
	if (key == tree->key)
	{
		add_info(tree, info);
		//return bal(tree);
	}
	//balance(tree);
	return bal(tree);//tree;//
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
	//balance(help);

	return help;

}




static int count = 1;


void tell(Tree* tree)
{

	if (tree) {
		tell(tree->right);
		InfoType* ptr = tree->inform->head;
		printf(" (%d) <", tree->key);
		
		while (ptr) 
		{
			printf("[%d.'%s'] ", count, ptr->info);
			count++;
			ptr = ptr->next;
		}
		count = 1;
		printf(">\n");
		//printf("%d ,b:(%d,%d,%d) ", tree->key, tree->hl, tree->hr, tree->balance);
		tell(tree->left);
	}
}

Table* initable()
{
	Table* table = (Table*)malloc(sizeof(Table));

	table->tree = NULL;
	return table;
}

int obhod(Table* table) {
	tell(table->tree);
	return 1;
}



void insert2(Table* table, int key, char* info) {
	table->tree = func(table->tree, key, info);
}



void FreeTree(Tree* node)
{
	if (!node)
		return;
	FreeTree(node->left);
	FreeTree(node->right);
	InfoType*ptr = node->inform->head;
	while (ptr) 
	{
		del_info(node);
	}
	free(node->inform);
	//free(aBranch->info);
	node->key = 0;
	free(node);
	return;
}
/*Tree* find(Tree* tree,int key) 
{
	int i = 0;
	Tree* node = tree;
	while (node) 
	{
		printf("%d\n", i);
		if (node->key > key)
		{
			node = tree->left;
			i++;
			continue;
		}
		if (node->key < key) {
			node = tree->right;
			i++;
			continue;
		}
		if (node->key == key)
			return node;
		
	}
	
	return NULL;
}*/












int printik(Table* table) 
{
	pr(table->tree, 1);
	printf("\n\n");
	//tell(table->tree);
	//function_print(table);
	return 1;
}



void print_Tree(Tree * p, int level)
{
	if (p)
	{
		print_Tree(p->left, level + 1);
		for (int i = 0; i < level; i++) printf(" ");
		printf("%d", p->key);
		print_Tree(p->right, level + 1);
	}
}

Tree* look(Tree* tree) 
{
	if (tree) {
		tree->left = look(tree->left);
		tree->right = look(tree->right);
	}
	return bal(tree);
}

Tree* deleN(Tree* item, int val, Stack* stack) 
{
	if (item == NULL)
		return item;

	if (val == item->key) 
	{
		if (item->inform->head)
		{
			if (item->inform->head->next)
			{
				del_info(item);
				return item;
			}
			else
			{
				Tree* pointer;
				if (item->right == NULL)
					pointer = item->left;
				else {

					Tree* ptr = item->right;
					if (ptr->left == NULL) 
					{
						ptr->left = item->left;
						pointer = ptr;
					}
					else {

						Tree* pmin = ptr->left;
						while (pmin->left != NULL) 
						{
							ptr = pmin;
							pmin = ptr->left;
						}
						ptr->left =item->right;
						pmin->left = item->left;
						pmin->right = item->right;
						pointer = pmin;
					}
				}
				del_info(item);
				free(item->inform);
				free(item);
				return pointer;
			}
		}
		

		
		
		
	//	return tmp;
	}

	else if (val < item->key)
	{
		push(stack, item);
		item->left = deleN(item->left, val, stack);
	}
	else
	{
		push(stack, item);
		item->right = deleN(item->right, val, stack);
	}
	return bal(item);
}


int algo(Table* table)
{

	Stack* stack = (Stack*)malloc(sizeof(Stack));
	stack->head = NULL;

	int key;
	printf("Enter key of searching element:\n");
	scanf("%d", &key);

	table->tree = deleN(table->tree, key, stack);

	while (stack->head)
	{
		pop(stack);
	}
	free(stack);
	return 1;

}





int insert(Table* table)
{
	table->tree = funct(table);
	return 1;
}




int(*fptr[])(Table*) = { NULL,insert,algo,printik,obhod,finding };// { NULL,add_item, onekey_del,clear,print,func1ks1,ks1func2,ks2function1,ks2func2 };




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