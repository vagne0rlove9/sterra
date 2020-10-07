// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <ctime>
#include <omp.h>
#include <string.h>
#include <pthread.h>
#include <mutex>

using namespace std;

struct Node
{
	int item;
	Node* prev;
	Node* next;
};
Node* head;

mutex mut;

void addItem(int value)
{
	Node* node = new (nothrow) Node;
	if (!node)
	{
		cout << "not enough memory";
	}
	else
	{
		node->item = value;
		if (head == NULL)
		{
			node->prev = NULL;
			node->next = NULL;
			head = node;
		}
		else
		{
			Node* p = head;
			while (p->next != NULL)
			{
				p = p->next;
			}
			p->next = node;
			node->prev = p;
			node->next = NULL;
		}
	}

}

void print()
{
	Node* p = head;
	while (p != NULL)
	{
		cout << p->item << " ";
		p = p->next;
	}
}

Node* delItem(Node* list)
{
	mut.lock();
	Node* prev, * next;
	prev = list->prev;
	next = list->next;
	if (prev != NULL)
	{
		prev->next = list->next;
		free(list);
		list = NULL;
		mut.unlock();
		return prev;
	}
	if (next != NULL)
	{
		next->prev = NULL;
		free(list);
		list = NULL;
		mut.unlock();
		return next;
	}
	free(list);
	list = NULL;
	mut.unlock();
	return NULL;
}

int check(unsigned int n, int parametr)
{
	int b;
	int c = 0;
	for (int i = 0; n > 0; i++)
	{
		b = fmod(n, 2);
		n /= 2;
		c += b * pow(10, i);
	}
	int k = 0;
	if ((c == 0) && (parametr == 0))
		k = 1;
	while (c != 0)
	{
		if (c % 10 == parametr)
		{
			k++;
		}
		c /= 10;
	}
	return k;
}

int k1 = 0;
int k2 = 0;

struct Args {
	int begin;
	int end;
	int id;
};

int countItems1 = 0;
int countItems2 = 0;

void* threadFunc(void* pargs)
{
	Node* tail = head;
	Args* args = (Args*)pargs;
	while (tail->next != NULL)
	{
		tail = tail->next;
	}
	Node* p = head;
	for (int i = args->begin; i <= args->end; i++)
	{
		if (args->id == 0)
		{
			k1 += check(tail->item, 1);
			countItems1++;
			tail = delItem(tail);
		}
		else
		{
			k2 += check(head->item, 0);
			countItems2++;
			head = delItem(head);
		}
	}

	return NULL;
}

int main()
{
	srand((unsigned int)time(NULL));
	int size = 5;
	for (int i = 0; i < size; i++)
	{
		addItem(rand() % 10 + 1);
	}
	print();
	int i;
	cout << endl;
	pthread_t threads[2];
	struct Args args[2];
	double block = (double)size / 2;
	for (i = 0; i < 2; i++)
	{
		int begin = (int)(block * i) + 1;
		int end = (int)(block * (i + 1));
		args[i].begin = begin;
		args[i].end = end;
		args[i].id = i;
	}
	for (i = 0; i < 2; i++) {
		if (pthread_create(&threads[i], NULL, threadFunc, &args[i])) {
			printf("Error: pthread_create failed!\n");
			return 1;
		}
	}
	for (i = 0; i < 2; i++) {
		int fac = 1;
		pthread_join(threads[i], NULL);
	}
	if (head != NULL)
		head = NULL;
	cout << endl << "0: " << k2 << "\n1: " << k1;
	cout << endl << "count items from tail: " << countItems1 << endl;
	cout << "count items from head: " << countItems2 << endl;
	if ((countItems1 + countItems2) == size)
	{
		cout << endl << "all items are checked!";
	}
	else cout << endl << countItems1 + countItems2;
	print();
	return 0;
}
