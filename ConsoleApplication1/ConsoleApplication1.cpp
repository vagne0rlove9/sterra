// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <ctime>
#include <omp.h>
#include <string.h>
using namespace std;

struct Node
{
	int item;
	Node* prev;
	Node* next;
};
Node* head;

void addItem(int value)
{
	Node* node = new Node;
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
	Node* prev, * next;
	prev = list->prev; 
	next = list->next; 
	if (prev != NULL)
		prev->next = list->next; 
	if (next != NULL)
		next->prev = list->prev; 
	free(list);
	list = NULL;
	return prev;
}

Node* delHead()
{
	Node* temp;
	temp = head->next;
	if (temp != NULL)
		temp->prev = NULL;
	free(head);
	head = NULL;
	return temp;
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

int main()
{
	srand((unsigned int)time(NULL));
	int size = 7;
	for (int i = 0; i < size; i++)
	{
		addItem(rand()%10+1);
	}
	print();
	int i;
	cout << endl;
	Node* tail = head;
	while (tail->next != NULL)
	{
		tail = tail->next;
	}
	int k1 = 0, k2 = 0;
	int count1 = 0, count2 = 0; 
	Node* p = head;
#pragma omp parallel num_threads(2)
	{
#pragma omp for private(i)
		for (i = 0; i < size; i++)
		{
			if (omp_get_thread_num() == 0)
			{
				k1 += check(tail->item, 1);
				count1++;
				tail = delItem(tail);
			}
			else
			{
				k2 += check(head->item, 0);
				count2++;
				head = delHead();
			}

		}
#pragma omp barrier
		head = NULL;
	}
	cout << endl << "0: " << k2 << "\n1: " << k1;
	cout << endl << "count items from head: " << count2 << "\ncount items from tail: " << count1;
	cout << endl << "count items: " << size;
	return 0;
}
