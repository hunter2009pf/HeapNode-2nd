#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#define true 1
#define false 0
typedef int bool;
// This template is only a guide 
// You need to include additional fields, data structures and functions  

// data type for heap nodes
typedef struct HeapNode {
	// each node stores the priority (key), name, execution time,
	//  release time and deadline of one task
	int key; //key of this task
	int TaskName;  //task name 
	int Etime; //execution time of this task
	int Rtime; // release time of this task
	int Dline; // deadline of this task
	int degree;
	int startTime, finishTime, whichCore;
	struct HeapNode *child, *sibling, *parent;
} HeapNode;

typedef struct ListNode
{
	HeapNode *node;
	struct ListNode *Next;
} ListNode;

//data type for a priority queue (heap) 
typedef struct BinomialHeap { //this is heap header
	int  size;      // count of items in the heap
	ListNode *root; // add additional fields here
} BinomialHeap;

// create a new heap node to store an item (task) 
HeapNode *newHeapNode(int k, int n, int c, int r, int d)
{ // k:key, n:task name, c: execution time, r:release time, d:deadline
  // ... you need to define other parameters yourself) 	 
	HeapNode *hn;
	hn = (HeapNode*)malloc(sizeof(HeapNode));
	assert(hn != NULL);
	hn->key = k;
	hn->TaskName = n;
	hn->Etime = c;
	hn->Rtime = r;
	hn->Dline = d;
	hn->degree = 0;
	hn->startTime = hn->finishTime = -1;
	hn->whichCore = -1;
	hn->child = hn->sibling = hn->parent = NULL;
	return hn;
}

ListNode* newListNode(HeapNode *hn) {
	ListNode *ln;
	ln = (ListNode*)malloc(sizeof(ListNode));
	ln->node = hn;
	ln->Next = NULL;
	return ln;
}

// create a new empty heap-based priority queue
BinomialHeap *newHeap()
{ // this function creates an empty binomial heap-based priority queue
	BinomialHeap *T;
	T = (BinomialHeap*)malloc(sizeof(BinomialHeap));
	assert(T != NULL);
	T->size = 0;
	T->root = NULL;
	return T;
}

ListNode* deleteListNode(ListNode *head, HeapNode *removed) {
	ListNode *temp = head, *preNode = NULL;
	if (temp != NULL && temp->node == removed) {
		head = head->Next;
		free(temp);
		return head;
	}
	while (temp != NULL && temp->node != removed) {
		preNode = temp;
		temp = temp->Next;
	}
	if (temp == NULL) return NULL;
	preNode->Next = temp->Next;
	free(temp);
	return preNode->Next;
}

ListNode* push_front(ListNode *head, HeapNode* tree) {
	ListNode *temp = newListNode(tree);
	temp->Next = head;
	return temp;
}

ListNode* push_back(ListNode *head, HeapNode* tree) {
	ListNode *temp = newListNode(tree);
	if (head == NULL)
	{
		head = newListNode(tree);
		return head;
	}
	ListNode *ttemp = head;
	while (ttemp->Next)ttemp = ttemp->Next;
	ttemp->Next = temp;
	return head;
}

HeapNode* mergeBinomialTrees(HeapNode *t1, HeapNode *t2) {
	if (t1->key > t2->key) {
		HeapNode *temp = (HeapNode*)malloc(sizeof(HeapNode));
		(*temp) = (*t1);
		(*t1) = (*t2);
		(*t2) = (*temp);
		free(temp);
	}
	t2->parent = t1;
	t2->sibling = t1->child;
	t1->child = t2;
	t1->degree++;

	return t1;
}

ListNode* unionBinomialHeap(ListNode *l1, ListNode *l2) {
	ListNode *result = NULL;
	ListNode *it1 = l1;
	ListNode *it2 = l2;
	while (it1 != NULL && it2 != NULL) {
		if (it1->node->degree <= it2->node->degree) {
			result = push_back(result, it1->node);
			it1 = it1->Next;
		}
		else {
			result = push_back(result, it2->node);
			it2 = it2->Next;
		}
	}

	while (it1 != NULL) {
		result = push_back(result, it1->node);
		it1 = it1->Next;
	}

	while (it2 != NULL) {
		result = push_back(result, it2->node);
		it2 = it2->Next;
	}
	return result;
}

ListNode* adjustHeap(ListNode *ln) {  // notes:dont allocate memory to ListNode in case of arbitrary value assigned to struct members
	ListNode* temp = ln;
	ListNode* t1 = ln;
	ListNode* t2 = ln;
	ListNode* t3 = ln;
	int cnt = 0;
	if (ln == NULL)return NULL;
	while (temp != NULL) {
		cnt++;
		temp = temp->Next;
	}
	if (cnt <= 1) return ln;


	if (cnt == 2) {
		t2 = t2->Next;
		t3 = NULL;
	}
	else {
		t2 = t2->Next;
		t3 = t2->Next;
	}
	while (t1 != NULL) {
		if (t2 == NULL)t1 = t1->Next;
		else if (t1->node->degree < t2->node->degree) {
			t1 = t1->Next;
			t2 = t2->Next;
			if (t3 != NULL)t3 = t3->Next;
		}
		else if (t3 != NULL && t1->node->degree == t2->node->degree && t2->node->degree == t3->node->degree) {
			t1 = t1->Next;
			t2 = t2->Next;
			t3 = t3->Next;
		}
		else if (t1->node->degree == t2->node->degree) {
			t1->node = mergeBinomialTrees(t1->node, t2->node);
			t2 = deleteListNode(ln, t2->node);
			if (t3 != NULL)t3 = t3->Next;
		}
	}
	return ln;
}

ListNode* insertATreeInHeap(ListNode *ln, HeapNode *tree) {
	ListNode *new_ln = newListNode(tree);
	new_ln = unionBinomialHeap(ln, new_ln);

	return adjustHeap(new_ln);
}

ListNode* removeMinFromTreeReturnBHeap(HeapNode *tree) {
	HeapNode *temp = tree->child;
	HeapNode *leftSide;
	ListNode *ln = NULL;
	while (temp)
	{
		leftSide = temp;
		temp = temp->sibling;
		leftSide->sibling = NULL;
		ln = push_front(ln, leftSide);
	}
	return ln;
}

// put the time complexity analysis for Insert() here
// time complexity: O(logn)
// both unionBinomialHeap() and adjustHeap() functions are executed in logarithmic time according to the size of n
void Insert(BinomialHeap *T, int k, int n, int c, int r, int d)
{ // k: key, n: task name, c: execution time, r: release time, d:deadline 
  // You don't need to check if this task already exists in T 	 
  //put your code here
	HeapNode *temp = newHeapNode(k, n, c, r, d);
	T->root = insertATreeInHeap(T->root, temp);
	T->size++;
}

HeapNode* getMinNode(ListNode *ln) {
	ListNode *temp = ln;
	HeapNode *result = ln->node;
	while (temp != NULL) {
		if (temp->node->key < result->key)
			result = temp->node;
		temp = temp->Next;
	}
	return result;
}

// put your time complexity for RemoveMin() here
// time complexity: O(logn)
// removeMinFromTreeReturnBHeap() function has the time complexity of O(logn) and so does the other two, unionBinomialHeap() and adjustHeap()
HeapNode *RemoveMin(BinomialHeap *T)
{
	// put your code here
	ListNode *new_ln = NULL, *listTemp, *it;
	HeapNode *temp;

	temp = getMinNode(T->root);
	it = T->root;
	while (it != NULL) {
		if (it->node->TaskName != temp->TaskName) {
			new_ln = push_back(new_ln, it->node);
		}
		it = it->Next;
	}
	listTemp = removeMinFromTreeReturnBHeap(temp);
	new_ln = unionBinomialHeap(new_ln, listTemp);
	T->root = adjustHeap(new_ln);
	T->size--;
	return temp;
}

int Min(BinomialHeap *T)
{
	// put your code here
	ListNode *temp = T->root;
	int minVal = temp->node->key;
	while (temp) {
		if (temp->node->key < minVal)minVal = temp->node->key;
		temp = temp->Next;
	}
	return minVal;
}


// put your time complexity analysis for MyTaskScheduler here
// time complexity: O(n*logn)
// Each insertion or removal of one task consumes logx time (x increases from 1 to n or decreases from n to 1 gradually,respectively) and there are n tasks
int TaskScheduler(char *f1, char *f2, int m)
{
	// put your code here
	BinomialHeap* taskHeap = newHeap();
	int val[4] = { -1 };

	FILE* fptr;
	fptr = fopen(f1, "r");//r represents reading mode
	if (fptr == NULL) {
		perror("file1 does not exist\n");
	}
	else {
		while (!feof(fptr)) {
			char str[100000];
			bool holdNum = false;
			fgets(str, 100000, fptr);
			if (str[0] == '\n')continue;
			int len = strlen(str), cnt = 0, temp = 0;
			if (len == 0)break;
			for (int it = 0; it < len; it++) {
				if (str[it] != 10 && str[it] != 13 && str[it] != ' ' && (str[it] < 48 || str[it]>57)) {
					printf("input error when reading the attribute of task %d\n", val[0]);
					return 0;
				}
				if (str[it] >= 48 && str[it] <= 57) {
					temp = temp * 10 + (int)(str[it] - 48);
					holdNum = true;
					continue;
				}
				else {
					if (holdNum) {
						val[cnt] = temp;
						temp = 0;
						if (cnt == 3) {
							if (val[2] + val[1] > val[3] || val[2] > val[3]) {
								printf("input error when reading the attribute of task %d\n", val[0]);
								return 0;
							}
							else Insert(taskHeap, val[3], val[0], val[1], val[2], val[3]);
						}
						cnt++;
						cnt %= 4;
						holdNum = false;
					}
				}
			}
		}
	}
	fclose(fptr);

	bool isFeasible = true;
	BinomialHeap* result = newHeap();
	int cores[15] = { 0 };
	for (int j = 1; j <= m; j++) {
		if (taskHeap->size <= 0)break;
		HeapNode* minNode = RemoveMin(taskHeap);
		if (minNode->Rtime > cores[j]) {
			minNode->startTime = minNode->Rtime;
			minNode->finishTime = minNode->Rtime + minNode->Etime;
			minNode->whichCore = j;
			cores[j] = minNode->finishTime;
		}
		else {
			minNode->startTime = cores[j];
			minNode->finishTime = cores[j] + minNode->Etime;
			minNode->whichCore = j;
			cores[j] = minNode->finishTime;
		}
		if (minNode->finishTime > minNode->Dline) {
			isFeasible = false;

		}
		Insert(result, minNode->startTime, minNode->TaskName, minNode->Etime, minNode->Rtime, minNode->whichCore);     //here input minNode->whichCore instead of minNode->Dline
	}

	while (taskHeap->size > 0) {
		int earliest = 99999999, whichOne = -1;
		for (int k = 1; k <= m; k++)
			if (cores[k] < earliest) {
				earliest = cores[k];
				whichOne = k;
			}

		HeapNode* minHn = RemoveMin(taskHeap);
		if (cores[whichOne] > minHn->Dline) {
			isFeasible = false;

		}
		else if (cores[whichOne] < minHn->Rtime) {
			minHn->startTime = minHn->Rtime;
			minHn->finishTime = minHn->Rtime + minHn->Etime;
			minHn->whichCore = whichOne;
			cores[whichOne] = minHn->finishTime;
		}
		else {
			minHn->startTime = cores[whichOne];
			minHn->finishTime = cores[whichOne] + minHn->Etime;
			minHn->whichCore = whichOne;
			cores[whichOne] = minHn->finishTime;
		}
		if (minHn->finishTime > minHn->Dline) {
			isFeasible = false;

		}
		Insert(result, minHn->startTime, minHn->TaskName, minHn->Etime, minHn->Rtime, minHn->whichCore);
	}

	FILE* fptr2 = fopen(f2, "w");
	int ccnt = 0;
	while (result->size > 0) {
		HeapNode* temp = RemoveMin(result);
		fprintf(fptr2, "%d Core%d %d", temp->TaskName, temp->Dline, temp->key);//here temp->key in the heap called 'result' represents startTime, temp->Dline represents the core
		ccnt++;
		if (ccnt % 5 == 0)fprintf(fptr2, "\n");
		else fprintf(fptr2, " ");
	}
	fclose(fptr2);

	if (isFeasible)return 1;
	return 0;
}

int main() {
	int i;
	i = TaskScheduler("samplefile1.txt", "feasibleschedule1.txt", 4);
	if (i == 0)printf("No feasible schedule!\n");
	i = TaskScheduler("samplefile1.txt", "feasibleschedule2.txt", 3);
	if (i == 0)printf("No feasible schedule!\n");
	i = TaskScheduler("samplefile2.txt", "feasibleschedule3.txt", 5);
	if (i == 0)printf("No feasible schedule!\n");
	i = TaskScheduler("samplefile2.txt", "feasibleschedule4.txt", 4);
	if (i == 0)printf("No feasible schedule!\n");
	i = TaskScheduler("samplefile3.txt", "feasibleschedule5.txt", 2);
	if (i == 0)printf("No feasible schedule!\n");
	i = TaskScheduler("samplefile4.txt", "feasibleschedule6.txt", 2);
	if (i == 0)printf("No feasible schedule!\n"); //here I can not find a feasible schedule only by EDF rule
//but actually there exists a feasible schedule for samplefile4 with 2 cores
//(task assignment) core1: 1 3 5 7 9 10 11 core2: 2 4 6 8 12
	return 0;
}
