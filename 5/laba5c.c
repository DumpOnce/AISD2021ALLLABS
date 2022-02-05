#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <locale.h>
#include <vld.h>
#include <stdbool.h>
#define N 5


/*

Написать программу для работы  по запросам оператора с неориентированным взвешенным графом. Каждая вершина графа имеет уникальное имя и задается координатами на плоскости {x, y}. Граф задается списками смежностей. Вес ребра графа вводится пользователем, и не может быть отрицательным.
Предусмотреть следующие операции:

добавление новой вершины в граф;
добавление в граф нового ребра между двумя заданными вершинами, с необходимым вычислением веса ребра (в случае взвешенного графа);
удаление заданной вершины из графа; при этом удаляются и все ребра, связанные с удаляемой вершиной;
удаление заданного ребра из графа;
вывод графа;
поиск некоторой заданной вершины из другой заданной вершины методом поиска в ширину (если пути между вершинами нет, вернуть соответствующий код ошибки);
поиск кратчайшего пути между двумя заданными вершинами графа, используя алгоритм Беллмана — Форда (если граф не взвешенный, считать вес каждого ребра равным единице);
поиск минимального остовного дерева (для неориентированного графа) или кратчайшего пути ко всем вершинам из некоторой заданой вершины (для ориентированного графа; результат - дерево, представленное ациклическим графом, с указанной вершиной в качестве корня). Результатом операции должен являться новый граф.
    1. Программа  должна  содержать несколько функций;  функция main() должна выполнять: вывод меню, ввод и анализ  ответа,  вызов  на исполнение требуемой функции.
    2. Программа должна генерировать граф заданного размера случайным образом.
    3. Граф должен храниться в файле; в начале сеанса работы он загружается из файла; если в процессе работы граф был изменен, новое состояние должно быть сохранено в файле.
    4. В программе нужно предусмотреть проверку правильности ввода данных.
    5. Оценить сложность реализованных алгоритмов.
    6. Провести таймирование (или профилирование) алгоритмов.
    7. *(Дополнительно) Реализовать графический вывод графа при помощи внешней библиотеки или внешнего инструмента (например, graphviz).


*/

typedef struct Vertex
{
	double key;
	int index;
	int vertex;
	int color;//0-white, 1-grey,2-black
	char *name;
	int x, y;
	double d;
	struct Vertex* parant;
	struct Edge* next2;
	struct Vertex* next1;

}Vertex;


typedef struct Edge {
	Vertex* top;
	double weight;
	double key;
	struct Edge* next;
}Edge;


typedef struct Graph {

	Vertex* first;
	Vertex* last;
	Vertex** mas;
	int now;
	FILE* fd;

}Graph;

typedef struct Node 
{
	double weight;
	Vertex* v;
	struct Node* next;
}Node;

typedef struct List {

	Node* head;
	Node* tail;

}List;

typedef struct Item {

	Vertex* v;
	struct Item* next;

}Item;

typedef struct Queue {

	int n;
	Item* head;
	Item* tail;

}Queue;

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

int dialog(const char *msgs[], int M)
{
	char *errmsg = "";
	int rc;
	int i, n;
	do {
		puts(errmsg);
		errmsg = "You are wrong. Repeate, please!";

		for (i = 0; i < M; ++i)
			puts(msgs[i]);
		puts("Make your choice: --> ");
		n = getInt(&rc);
		if (n == 0)
			rc = 0;
	} while (rc < 0 || rc >= M);
	return rc;
}



char* get_str()
{
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
		else
		{
			scanf("%*c");
		}
	} while (n > 0);

	if (len > 0) {
		res[len] = '\0';
	}
	else
	{
		res = calloc(1, sizeof(char));
	}

	return res;
}

char* rando() 
{
	char* final = NULL,*arr = (char*)malloc(sizeof(char)),obj[] = "MNBVCXZLKHJFGDSAPQWERTYUIO1234567890mnbvcxzlkjhgfdsapoiuytrewq";
	int i, j, len = strlen(obj);
	for (i = 0; i < N - 1 && i < len; i++)
	{
		j = rand() % (len - i) + i;
		arr[i] = obj[j];
		obj[j] = obj[i];
	}
	arr[i] = '\0';
	final = realloc(final, sizeof(char)*N);
	memcpy(final, arr, N);
	return final;
}






void list_add(List* list,double weight)
{
	if (!list->head)
	{
		Node* ptr = (Node*)malloc(sizeof(Node));
		ptr->weight = weight;
		ptr->next = NULL;
		list->head  =   ptr;
	}
	else
	{
		Node* ptr2 = (Node*)malloc(sizeof(Node));
		ptr2->weight = weight;
		ptr2->next = list->head;
		list->head = ptr2;
	//	list->tail->next = ptr2;
		//ptr2->next = NULL;
		//list->tail = ptr2;
		//ptr2->next = list->head;
		//list->head = ptr2;
	}
}

void list_clear(List* list)
{
	Node* ptr = list->head;
	while (ptr)
	{
		if (ptr->next) 
		{
			Node* ptr2 = ptr;
			ptr = ptr->next;
			free(ptr2);
			
			list->head = ptr;

		}
		else {
			free(ptr);
			list->head = NULL;
			list->tail = NULL;
			break;
		}
	}
}


double MinOfList(List* list)
{
	Node* ptr = list->head;
	double min = INFINITY;
	while (ptr)
	{
		if (ptr->weight < min) {
			min = ptr->weight;
		}


		ptr = ptr->next;
	}

	return min;

}

double searching_weight(Graph* graph,int j,int g) 
{
	Vertex* vert = graph->first;
	if (j == g)
		return 0.0;
	for (int i = 0; i < graph->now; i++)
	{
		if (i == j) 
		{
			Edge* ptr = vert->next2;
			while (ptr) 
			{
				/*if (ptr->top->index == j) 
				{
					return 0.0;
				}*/
				if (ptr->top->index == g) 
				{
					return ptr->weight;
				}
				ptr = ptr->next;
			}
			if (!ptr)
				return INFINITY;
		}
		vert = vert->next1;
	}
}


void Floyd(Graph* graph, int start)// алгоритм Флойда-Уоршелла
{
	int cap = graph->now;
	double* Lambda = (double*)malloc(sizeof(double)*cap);
	/*Array_List*/
	List* list = (List*)malloc(sizeof(List));
	list->head = NULL;
	for (int i = 0; i < cap; i++)
		Lambda[i] = INFINITY;
	Lambda[start - 1] = 0.0;
	for (int i = 1; i <= cap - 1; i++) 
	{
		for (int g = 0; g < cap; g++) 
		{
			for (int j = 0; j < cap; j++)
				list_add(list, Lambda[j]+searching_weight(graph,j,g));

			Lambda[g] = MinOfList(list);

			list_clear(list);

		}
	}
	for (int p = 0; p < cap; p++) 
		printf("%f ", Lambda[p]);
	
}

double fRand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}




void Enqueue(Vertex* v, Queue* queue)
{
	
	if (!queue->head) 
	{
		Item* v2 = (Item*)malloc(sizeof(Item));
		v2->next = NULL;;
		v2->v = v;
		queue->head = queue->tail = v2;

	}
	else
	{
		Item* v3 = (Item*)malloc(sizeof(Item));
		v3->next = NULL;
		v3->v = v;
		queue->tail->next = v3;
		queue->tail = v3;
	}
	
}

void relax(Graph* graph, Vertex* v1, Vertex*v2, double weight)
{
	if (v2->d > v1->d + weight)
		v2->d = weight + v1->d;
}


int B_Ford(Graph* graph)//Почти готовый
{
	
	printf("Введите индекс стартовой вершины:\n");
	int startt, finish;
	scanf("%d", &startt);
	printf("Введите индекс вершины, до которой необходимо найти кратчайший путь:\n");
	scanf("%d", &finish);
	Vertex* start = graph->mas[startt];
	Vertex* end = graph->mas[finish];
	for(int g = 0;g<graph->now;g++)
	{
		//ptr->color = 0;
		graph->mas[g]->d = INFINITY;
		
	}
	start->d = 0.0;


	for (int i = 1; i <= graph->now-1 ; i++)
	{
		Vertex* point = graph->mas[i-1];
		for(int h = 0;h<graph->now;h++) {
			Edge* ptr = point->next2;
			while (ptr)
			{
				relax(graph, point, ptr->top, ptr->weight);
				ptr = ptr->next;
			}
			//point = point->next1;
		}
		//start = start->next1;
	}
	
	printf("Кратчайшее расстояние от вершины '%s'до вершины '%s' равно: %f\n", start->name,end->name,end->d);
	
	return 1;
}










Item* Dequeue(Queue* queue)
{
	if (!queue->head->next)
	{
		Item* ptr2 = queue->head;
		queue->head = queue->tail = NULL;
		return ptr2;
	}
	else {
		Item* ptr = queue->head;
		queue->head = queue->head->next;
		return ptr;
	}
}

void insert(Queue* pqueue, Vertex* v1)
{

	if (!pqueue->head)
	{
		Item* ptr = (Item*)malloc(sizeof(Item));
		ptr->v = v1;
		ptr->next = NULL;
		pqueue->head = pqueue->tail = ptr;
	}
	else
	{
		Item* ptr2 =(Item*)malloc(sizeof(Item));
		Item* pointer = pqueue->head;
		Item* pointer2 = pointer;
		while (pointer)
		{
			if (pointer == pqueue->head) 
			{
				if (!pointer->next) 
				{
					if (v1->key <= pointer->v->key) {
						ptr2->v = v1;
						ptr2->next = pointer;
						pqueue->head = ptr2;
						break;
					}
					else 
					{
						ptr2->v = v1;
						ptr2->next = NULL;
						pointer->next = ptr2;

						break;


					}
				}
				
			}

			if (pointer->next)
			{
				if (v1->key <= pointer->v->key) {

					ptr2->next = pointer;
					ptr2->v = v1;
					break;

				}
				if (v1->key <= pointer->next->v->key)
				{
					ptr2->v = v1;
					ptr2->next = pointer->next;
					pointer->next = ptr2;
					break;
				}
				else {
					pointer = pointer->next;

				}
				
				}
			if (!pointer->next) {
					ptr2->next = NULL;
					ptr2->v = v1;
					pointer->next = ptr2;
					break;
			}
			
		}



	}

}

void clear(Graph* graph)
{

	for (int i = 0; i < graph->now; i++) {
		Edge* ptr = graph->mas[i]->next2;
		while (ptr) {
			Edge* h = ptr;
			ptr = ptr->next;
			free(h);
		}
		free(graph->mas[i]->name);
		free(graph->mas[i]);
		graph->mas[i] = NULL;
	}
	free(graph->mas);
	free(graph);

}


void add2(Graph* graph, Vertex* v1, Vertex* v2, double weight)
{
	if (!v1->next2)
	{
		Edge* ptr = (Edge*)malloc(sizeof(Edge));
		ptr->weight = weight;
		ptr->next = NULL;
		ptr->top = v2;
		v1->next2 = ptr;
	}
	else {
		Edge* ptr2 = (Edge*)malloc(sizeof(Edge));
		ptr2->weight = weight;
		ptr2->top = v2;
		Edge* helper = v1->next2;
		while (helper)
		{
			if (helper->top == v2) {
				free(ptr2);
				return;
			}
			helper = helper->next;
		}
		ptr2->next = v1->next2;
		v1->next2 = ptr2;
	}
}

void add1(Graph* graph)//Добавлние ребра в граф между двумя вершинами
{
	printf("Введите индекс вершины 1:\n");
	int index1;
	scanf("%d", &index1);
	printf("Введите индекс вершины 1:\n");
	int index2;
	scanf("%d", &index2);
	Vertex* fst = graph->mas[index1];
	Vertex * snd = graph->mas[index2];
	printf("Введите вес ребра:\n");
	double weight;
	scanf("%lf", &weight);
	add2(graph, fst, snd, weight);
	add2(graph, snd, fst, weight);
}





Item* Extract_Min(Queue* pqueue)
{

	Item* ptr = pqueue->head;
	pqueue->head = pqueue->head->next;
	return ptr;

}


int BFS(Graph* graph) 
{
	printf("Введите индекс стартовой вершины:\n");
	int startt, finish;
	scanf("%d", &startt);
	printf("Введите индекс вершины, до которой необходимо найти кратчайший путь:\n");
	scanf("%d", &finish);
	Vertex* start = graph->mas[startt];
	Vertex* end = graph->mas[finish];
	
	for(int i = 0;i<graph->now;i++)
	{
		graph->mas[i]->color = 0;
		graph->mas[i]->d = INFINITY;
		
	}
	start->d = 0.0;
	start->color = 1;
	Queue* queue = (Queue*)malloc(sizeof(Queue));
	queue->head = queue->tail = NULL;
	Enqueue(start,queue);
	while (queue->head) 
	{
		Item* f = Dequeue(queue);
		Edge* ptr2 = f->v->next2;
		while (ptr2) 
		{
			if(ptr2->top->color == 0)
			{
				ptr2->top->color = 1;
				ptr2->top->d = f->v->d + ptr2->weight;
				Enqueue(ptr2->top,queue );
			}
			ptr2 = ptr2->next;
		}
		f->v->color = 2;
		free(f);

	}
	printf("Кратчайшее расстояние от вершины '%s'до вершины '%s' равно: %f\n", start->name, end->name, end->d);

	free(queue);
	return 1;
}

bool exists(Queue* pqueue, Vertex* v1)
{
	Item* ptr = pqueue->head;
	while (ptr) 
	{
		if (ptr -> v == v1) 
		{
			return true;
		}
		ptr = ptr->next;
	}
	return false;
}


void decrease_key(Queue* pqueue)
{	

	Item * ptr1 = pqueue->head;
	if (!ptr1)
		return;
	if (!ptr1->next) 
		return;
	int i = 0;
	Item* ptr2 = ptr1->next;
	while (ptr2)
	{
		if (ptr2->v->key < ptr1->v->key) {
			Vertex* p = ptr2->v;
			ptr2->v = ptr1->v;
			ptr1->v = p;
			i++;
			continue;
		}
		ptr2 = ptr2->next;
		ptr1 = ptr1->next;
	}
	while (i > 0)
	{
		i = 0;
		ptr1 = pqueue->head;
		ptr2 = ptr1->next;
		while (ptr2)
		{
			if (ptr2->v->key < ptr1->v->key) {
				Vertex* p = ptr2->v;
				ptr2->v = ptr1->v;
				ptr1->v = p;
				i++;
				continue;
			}
			ptr2 = ptr2->next;
			ptr1 = ptr1->next;
		}
	}
}


void clear2(Graph* graph)
{

	for (int i = 0; i < graph->now; i++) {
		Edge* ptr = graph->mas[i]->next2;
		while (ptr) {
			Edge* h = ptr;
			ptr = ptr->next;
			free(h);
		}
		//free(graph->mas[i]->name);
		free(graph->mas[i]);
		graph->mas[i] = NULL;
	}
	free(graph->mas);
	free(graph);
}

int searching(Graph* graph, char* name)
{
	int num = 0;
	for (num; num < graph->now; num++)
	{
		if (strcmp(name, graph->mas[num]->name) == 0)
			return num;
	}

	return  num;
}


int Algorythm_Prima(Graph* graph)
{
	int index;
	printf("Введите индекс стартовой вершины:\n");
	scanf("%d", &index);
	Vertex* start = graph->mas[index];

	
	for(int c = 0;c<graph->now;c++)
	{
		graph->mas[c]->parant = NULL;
		graph->mas[c]->key = INFINITY;
		
	}
	//start->parant = NULL;
	start->key = 0;
	Queue* pqueue =(Queue*) malloc(sizeof(Queue));
	pqueue->head = pqueue->tail = NULL;
	
	for(int i = 0;i<graph->now;i++)
	{
		insert(pqueue, graph->mas[i]);
	}
	Item* p = pqueue->head;

	while (pqueue->head)
	{

		Item* u = Extract_Min(pqueue);
		Vertex* ptr2 = u->v;
		Edge* e = ptr2->next2;
		while (e)
		{
			if (exists(pqueue,e->top) == true && (e->weight < e->top->key)) 
			{
				printf("Here\n");                      
				e->top->key = e->weight;
				decrease_key(pqueue);

			}

			e = e->next;

		}
		free(u);
		
	}
	for (int i = 0; i < graph->now; i++)
		printf("%f ", graph->mas[i]->key);
	Graph* graph2 = (Graph*)malloc(sizeof(Graph));
	graph2->first = graph2->last = NULL;
	graph2->now = 0;
	graph2->mas = NULL;
	for (int i = 0; i < graph->now; i++) 
	{

		add_vertexX(graph2, graph->mas[i]->x, graph->mas[i]->y, graph->mas[i]->name);
	}
	for (int g = 0; g < graph->now; g++) 
	{

		Edge* ptr = graph->mas[g]->next2;
		while (ptr)
		{
			if (ptr->weight == graph->mas[g]->key)
			{
	
				printf("Hello ");
				add2(graph2, graph2->mas[searching(graph2,ptr->top->name)], graph2->mas[g], ptr->weight);
				add2(graph2, graph2->mas[g], graph2->mas[searching(graph2, ptr->top->name)], ptr->weight);
			}
	
			ptr = ptr->next;
		}
	}

	printik(graph2);

	free(pqueue);
	return 1;
}


int randd() {

	return (rand() % (100 - 1) + 1);
}

int add_vertex(Graph* graph)
{
	int x, y;
	printf("Введите название вершины:\n");

	char* name = get_str();

	printf("Введите координату X:\n");
	scanf("%d", &x);
	printf("Введите координату Y:\n");
	scanf("%d", &y);


	graph->mas = (Vertex*)realloc(graph->mas, (graph->now + 1) * sizeof(Vertex));

	if (!graph->first)
	{
		Vertex* ptr = (Vertex*)malloc(sizeof(Vertex));
		ptr->x = x;
		ptr->y = y;
		ptr->index = 0;
		ptr->name = name;
		ptr->next2 = NULL;
		ptr->next1 = NULL;
		ptr->color = 0;
		graph->mas[graph->now] = ptr;
		graph->now++;
		graph->first = graph->last = ptr;

	}
	else 
	{
		Vertex* ptr2 = (Vertex*)malloc(sizeof(Vertex));
		ptr2->x = x;
		ptr2->y = y;
		ptr2->color = 0;
		ptr2->name = name;
		ptr2->next1 = NULL;
		ptr2->next2 = NULL;
		graph->mas[graph->now] = ptr2;
		graph->last->next1 = ptr2;
		ptr2->index = graph->last->index+1;
		graph->last = ptr2;
		graph->now++;
	}
	return 1;
}


Graph* init(int number) 
{
	Graph* graph = (Graph*)malloc(sizeof(Graph));
	graph->first = graph->last = NULL;
	graph->now = 0;
	for (int i = 0; i < number; i++)
	{
		int x;
		int y;
		scanf("%d", &x);
		scanf("%d", &y);

		char* name = get_str();
		add_vertex(graph,x,y,name,i);
	}
	return graph;
}

Vertex*  search(Graph* graph, char* name) {
	Vertex* ptr = graph->first;
	while(ptr)
	{
		if (strcmp(ptr->name, name) == 0) {
			return ptr;
		}


	}

	return NULL;
}

int add_vertexX(Graph* graph,int x,int y,char* name)
{
	
	graph->mas = (Vertex*)realloc(graph->mas, (graph->now + 1) * sizeof(Vertex));

	if (!graph->first)
	{
		Vertex* ptr = (Vertex*)malloc(sizeof(Vertex));
		ptr->x = x;
		ptr->y = y;
		ptr->index = 0;
		ptr->name = name;
		ptr->next2 = NULL;
		ptr->next1 = NULL;
		ptr->color = 0;
		graph->mas[graph->now] = ptr;
		graph->now++;
		graph->first = graph->last = ptr;

	}
	else
	{
		Vertex* ptr2 = (Vertex*)malloc(sizeof(Vertex));
		ptr2->x = x;
		ptr2->y = y;
		ptr2->color = 0;
		ptr2->name = name;
		ptr2->next1 = NULL;
		ptr2->next2 = NULL;
		graph->mas[graph->now] = ptr2;
		graph->last->next1 = ptr2;
		ptr2->index = graph->last->index + 1;
		graph->last = ptr2;
		graph->now++;
	}
	return 1;
}






Vertex* rand_vert(Graph* graph)
{

	Vertex* ptr = graph->first;
	int f = graph->now-2;
	if (f == 0)
		return graph->first;
	int num = rand() % ((graph->now - 1) - 1) + 1;
	for (int i = 0; i < graph->now; i++)
	{
		if (i == num)
		{
			return ptr;
		}
		ptr = ptr->next1;
	}
}







void print(Graph* graph)
{
	Vertex* ptr = graph->first;
	while (ptr)
	{
		printf("'%s' ---{%d,%d}--{%d} ---> ", ptr->name, ptr->x, ptr->y,ptr->index);
		if (ptr->next2)
		{
			Edge* ptr2 = ptr->next2;
			while (ptr2)
			{
				printf("'%s' -- {%d,%d} -- w:'%f' --> ", ptr2->top->name, ptr2->top->x, ptr2->top->y, ptr2->weight);

				ptr2 = ptr2->next;
			}
		}
		ptr = ptr->next1;
		printf("\n|\n");
	}
}

int  del_edge(Graph* graph)

{
	int index1, index2;
	printf("Введите индекс1:\n");//Возможны ошибки!!!
	scanf("%d", &index1);
	printf("Введите индекс2:\n");
	scanf("%d", &index2);
	Vertex* v1 = graph->mas[index1];
	Vertex* v2 = graph->mas[index2];
	scanf("%d", &index2);
	Edge* ptr = v1->next2;
	Edge* ptr2 = ptr;
	while (ptr)
	{
		if (ptr->top == v2) 
		{
			if (v1->next2 == ptr)
			{
				v1->next2 = ptr->next;
				free(ptr);
				break;
			}	
			else
			{
				ptr2->next = ptr->next;
				free(ptr);
				ptr = ptr2;
				break;
			}
		}
		else
		{
			if (ptr->next)
			{
				if (ptr->next->top != v2)
					ptr2 = ptr2->next;

				//ptr = ptr->next;
			}
			ptr = ptr->next;
		}
	}
	ptr = v2->next2;
	ptr2 = ptr;
	while (ptr) {
		if (ptr->top == v1) {
			if (v2->next2 == ptr) {
				v2->next2 = ptr->next;
				free(ptr);
				break;
			}
			else {
				ptr2->next = ptr->next;
				free(ptr);
				ptr = ptr2;
				break;
			}
		}
		else {
			if (ptr->next) {
				if (ptr->next->top != v1)
					ptr2 = ptr2->next;
				
			}
			ptr = ptr->next;
		}
	}
	return 1;
}




int dot(Graph* graph)
{
	printf("Введите путь к файлу:\n");
	char *A = get_str();
	FILE* fin = fopen(A, "r+");
	
	fprintf(fin, "digraph G{\n");
	
	for(int i = 0;i<graph->now;i++)
	{
		Edge* ptr = graph->mas[i]->next2;
		while (ptr) 
		{
			fprintf(fin, "%s->%s[label = \"%f\"] \n", graph->mas[i]->name, ptr->top->name, ptr->weight);
			ptr = ptr->next;
		}
		
	}
	fprintf(fin, "}");
	
	fclose(fin);
	graph->fd = NULL;
	free(A);
	return 1;
}






int delete_vertex(Graph* graph)
{
	int i;
	printf("Введите идекс удаляемой вершины:\n");
	scanf("%d", &i);
	if (i > graph->now-1||i<0)
		return 1;
	Vertex* v = graph->mas[i];
	Edge* ptr = v->next2;
	while (ptr)
	{
		Edge* ptr2 = ptr;
		ptr = ptr->next;
		Vertex* ptr3 = ptr2->top;
		Edge* pointer = ptr3->next2;
		Edge* help = pointer;
		while (pointer) 
		{
			if (pointer->top == v) 
			{
				if (pointer == ptr2->top->next2) 
				{
					Edge* h = pointer;
					ptr3->next2 = pointer->next;
					free(h);
					break;
				}
				else 
				{
					pointer = pointer->next;
					free(help->next);
					help->next = pointer;
					break;
				}

			}
			else 
			{
				if (pointer->next)
					if (pointer->next->top != v)
						help = help->next;
				pointer = pointer->next;
			}

		}
		free(ptr2);

	}
	free(v->name);

		graph->mas[i] = graph->mas[graph->now - 1];
		graph->mas[graph->now - 1] = NULL;
		graph->mas = (Vertex**)realloc(graph->mas, (graph->now - 1) * sizeof(Vertex*));
		free(v);
		graph->now--;
		printf("%d\n", graph->now);
	return 1;
}


int printik(Graph* graph)
{
	for (int i = 0; i < graph->now; i++) 
	{
		printf("'%s' ---{%d,%d}--{%d} ---> ", graph->mas[i]->name, graph->mas[i]->x, graph->mas[i]->y,i);
		if (graph->mas[i]->next2)
		{
			Edge* ptr2 = graph->mas[i] ->next2;
			while (ptr2)
			{
				printf("'%s' -- {%d,%d} -- w:'%f' --> ", ptr2->top->name, ptr2->top->x, ptr2->top->y, ptr2->weight);
				ptr2 = ptr2->next;
			}
		}	
		printf("\n|\n");
	}
	return 1;
}

int random_graph()
{

	Graph* graph = (Graph*)malloc(sizeof(Graph));
	graph->first = graph->last = NULL;
	graph->fd = NULL;
	int number;
	printf("Введите количество вершин в рандомном графе:\n");
	scanf("%d", &number);
	graph->mas = NULL;
	graph->now = 0;
	for (int i = 0; i < number; i++)
	{

		add_vertexX(graph, rand() % (100 - 1) + 1, rand() % (100 - 1) + 1, rando());
	}
	Vertex* ptr = graph->first;
	if (graph->now == 1)
	{
		return graph;
	}
	while (ptr)
	{
		int g = rand() % (10 - 1) + 1;

		for (int i = 1; i <= g; i++)
		{

			int k = 0;
			Vertex* ran = rand_vert(graph);
			double weight = fRand(1.0, 1000.0);
			double w = weight;
			if (graph->now != 2) {
				while (strcmp(ran->name, ptr->name) == 0)
				{

					ran = rand_vert(graph);
				}
			}


			Edge* pointer = ptr->next2;
			while (pointer) {
				if (strcmp(pointer->top->name, ran->name) == 0)
				{
					k++;
					break;
				}
				pointer = pointer->next;
			}
			if (k > 0)
				break;

			add2(graph, ptr, ran, w);
			int c = 0;

			Edge* pointer2 = ran->next2;
			while (pointer2)
			{
				if (strcmp(pointer2->top->name, ran->name) == 0)
				{
					c++;
					break;
				}
				pointer2 = pointer2->next;
			}
			if (c > 0)
				break;
			add2(graph, ran, ptr, w);


		}
		ptr = ptr->next1;
	}

	printik(graph);
	printf("\n\n\n");
	int choose;
	printf("Применить один из алгоритмов к данному графу?1 - да, другая цифра -нет\n");
	scanf("%d", &choose);
	
	if(choose == 1) 
	{
		int choose2;

		printf("1-Алгоритм Беллмана-Форда, 2-Поиск в ширину,3- поиск MST\n");
		scanf("%d", &choose2);
		
		switch (choose2) 
		{

		case 1:
			B_Ford(graph);

			break;

		case 2:
			BFS(graph);
			break;

			
		case 3:

			Algorythm_Prima(graph);
			break;

		}
	}
	scanf("%*c");

	dot(graph);
	
	 printf(".dot файл создан!\n");
	clear(graph);
	return 1;
}



int(*fptr[])(Graph*) = { NULL,add_vertex,delete_vertex,add1,del_edge,printik,B_Ford,BFS,Algorythm_Prima,random_graph,dot };



const char *MSGS[] = { "0. Выйти", "1.Добавить вершину", "2. Удалить вершину", "3.Добавить ребро между двумя вершинами ", "4. Удалить ребро из графа ","5.Распечать граф","6.Поиск кратчайшего расстояния между двумя вершинами (Алгоритм Беллмана-Форда)","7.Обход в ширину","8.Построение минимального остовного дерева","9.Формирование рандомного графа","10.Сохранить граф в .dot файл","11.Таймирование алгоритмов" };

const int MSGS_SIZE = sizeof(MSGS) / sizeof(MSGS[0]);


Graph* initialisation() {
	Graph* graph = (Graph*)malloc(sizeof(Graph));
	graph->now = 0;
	graph->mas = NULL;
	graph->first = graph->last = NULL;
	return graph;
}


int main()
{
	setlocale(LC_ALL, "");
	srand(time(NULL));
	
	Graph* graph = initialisation();
	int rc;
	while (rc = dialog(MSGS, MSGS_SIZE))
		if (!fptr[rc](graph))
			break;
	clear(graph);
	return 0;
}