#include <stdio.h>
#include <stdlib.h>
#include <time.h>//for using rand()
#include <Windows.h>//for using gotoxy()
#include <math.h>//for using pow()

#define MAX 1000
#define TRUE 1
#define FALSE -1
#define Capacity MAX

typedef struct treeNode {
	int data;
	struct treeNode* leftchild;
	struct treeNode* rightchild;
}treeNode;

typedef struct circularQueue {
	treeNode* data[Capacity];
	int front;
	int rear;
}cQueue;

cQueue* createCircularQueue();
void enqueue(cQueue**, treeNode*);
void nullNodeenqueue(cQueue**);
treeNode* dequeue(cQueue**);
int isFull(cQueue*);
int isEmpty(cQueue*);

int generateRandomInteger();//between 1 and 100
void gotoxy(int, int);//coordinate (x, y)
int* storedTreeNodes(treeNode*, int);//save tree nodes

void randomInsertBinaryTree(treeNode**);
treeNode* createBinaryTree(int);
void printTree(treeNode*, int, int);
void insertBST(treeNode**, int);
treeNode* convertToBST(treeNode*, int);
int isBST(treeNode*, int);

int main() {

	srand((unsigned int)time(NULL));//using seed value 

	treeNode* binaryTree = NULL;
	treeNode* binarySearchTree = NULL;

	binaryTree = createBinaryTree(10);

	binarySearchTree = convertToBST(binaryTree, 10);
	
	printTree(binaryTree, 10, 0);
	printTree(binarySearchTree, 10, 1);

	if (isBST(binaryTree, 10) == TRUE) {
		gotoxy(0, 38);
		printf("First tree is binary search tree!!");
	}
	else {
		gotoxy(0, 38);
		printf("First tree is not binary search tree!!");

	}
	
	if (isBST(binarySearchTree, 10) == TRUE){
		gotoxy(0, 39);
		printf("second tree is binary search tree!!");
	}
	else {
		gotoxy(0, 39);
		printf("Second tree is not binary search tree!!");

	}

	gotoxy(0, 43);
	system("pause");
	return 0;
}

cQueue* createCircularQueue() {
	cQueue* pCQueue = NULL;

	pCQueue = (cQueue*)malloc(sizeof(cQueue));
	pCQueue->front = 0;
	pCQueue->rear = 0;

	return pCQueue;
}

void enqueue(cQueue** cQueue, treeNode* node) {
	if (isFull(*cQueue) == TRUE) {
		printf("Circular Queue is full!\n");
		return;
	}

	(*cQueue)->rear = ((*cQueue)->rear + 1) % Capacity;
	(*cQueue)->data[(*cQueue)->rear] = node;
}

void nullNodeenqueue(cQueue** cQueue) {
	if (isFull(*cQueue) == TRUE) {
		printf("Circular Queue is full!\n");
		return;
	}
	treeNode* nullNode;
	nullNode = (treeNode*)malloc(sizeof(treeNode));
	nullNode->data = 0;
	nullNode->leftchild = NULL;
	nullNode->rightchild = NULL;

	(*cQueue)->rear = ((*cQueue)->rear + 1) % Capacity;
	(*cQueue)->data[(*cQueue)->rear] = nullNode;
}

treeNode* dequeue(cQueue** cQueue) {
	if (isEmpty(*cQueue) == TRUE) {
		printf("Circular Queue is empty!\n");
		exit(1);
	}

	(*cQueue)->front = ((*cQueue)->front + 1) % Capacity;
	return (*cQueue)->data[(*cQueue)->front];
}


int isFull(cQueue* cQueue) {
	if ((cQueue->rear + 1) % Capacity == cQueue->front)
		return TRUE;
	else
		return FALSE;
}

int isEmpty(cQueue* cQueue) {
	if (cQueue->front == cQueue->rear)
		return TRUE;
	else
		return FALSE;
}

int generateRandomInteger() {
	int n;
	n = (rand() % 100) + 1;//1-100

	return n;
}

void gotoxy(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int* storedTreeNodes(treeNode* root, int numberOfNodes) {
	cQueue* queue;
	queue = createCircularQueue();
	treeNode* temp;

	static int infoTreeElement[MAX];
	int index = 0;//array's index
	int currentNumber = 0;//counting number of tree nodes

	//initialize for -1 all elements
	for (int i = 0; i < MAX; i++)
		infoTreeElement[i] = -1;

	enqueue(&queue, root);
	while (currentNumber != numberOfNodes) {
		temp = dequeue(&queue);

		if (temp->data != 0)
			currentNumber++;

		infoTreeElement[index++] = temp->data;

		if (temp->leftchild)
			enqueue(&queue, temp->leftchild);

		if (!(temp->leftchild))
			nullNodeenqueue(&queue);

		if (temp->rightchild)
			enqueue(&queue, temp->rightchild);

		if (!(temp->rightchild))
			nullNodeenqueue(&queue);

	}
	return &infoTreeElement[0];
}

void randomInsertBinaryTree(treeNode** root) {
	int random;

	if ((*root) == NULL) {
		*root = (treeNode*)malloc(sizeof(treeNode));
		(*root)->data = generateRandomInteger();
		(*root)->leftchild = NULL;
		(*root)->rightchild = NULL;
	}
	//randomly selecting either left or right
	else if (random = generateRandomInteger() < 50) 
		randomInsertBinaryTree(&((*root)->leftchild));
	
	else 
		randomInsertBinaryTree(&((*root)->rightchild));
}

treeNode* createBinaryTree(int numberOfNodes) {
	treeNode* root = NULL;
	
	for (int i = 0; i < numberOfNodes; i++) 
		randomInsertBinaryTree(&root);
	
	return root;
}

void printTree(treeNode* root, int numberOfNodes, int position) {

	int* treeElements;
	int number = 0;//number of nodes in tree
	int level = 1;//tree level
	int n = 1;//number of node in each level 
	int x = 0;//position x
	int y = 0;//position y
	int cL = 1;//current level

	//for print two trees in same time
	if (position == 1)
		y = 20;

	treeElements = storedTreeNodes(root, numberOfNodes);

	while (treeElements[number] != -1)
		number++;

	for (;;) {
		if (number >= pow(2, (level - 1)) && number <= (pow(2, level) - 1))
			break;
		
		level++;
	}

	//root print
	x = pow(2, level - cL + 1) * n - pow(2, (level - cL));
	gotoxy(x, y);
	printf("%d", treeElements[0]);

	//set next cL
	y = y + 2;
	cL = 2;

	//start from second node
	for (int i = 1; i < number; i++) {
		if (treeElements[i] != 0) {
			x = pow(2, level - cL + 1) * n - pow(2, (level - cL));
			gotoxy(x, y);
			printf("%d", treeElements[i]);
			if (n % 2 == 1) {
				gotoxy(x, y - 1);
				printf("/");
			}
			else if (n % 2 == 0) {
				gotoxy(x, y - 1);
				printf("¡¬");
			}
		}
		n++;

		if (pow(2, (cL - 1)) == n - 1) {
			cL++;
			y = y + 2;
			n = 1;
		}
	}
}

void insertBST(treeNode** p, int value) {
	if ((*p) == NULL) {
		*p = (treeNode*)malloc(sizeof(treeNode));
		(*p)->data = value;
		(*p)->leftchild = NULL;
		(*p)->rightchild = NULL;
	}
	else if ((*p)->data >= value) {
		insertBST(&((*p)->leftchild), value);
	}
	else {
		insertBST(&((*p)->rightchild), value);
	}
}

treeNode* convertToBST(treeNode* root, int numberOfNodes) {
	treeNode* temp = NULL;
	int* treeElements;
	int number = 0;
	treeElements = storedTreeNodes(root, numberOfNodes);
	
	while (treeElements[number] != -1)
		number++;

	for (int i = 0; i < number; i++)
		if (treeElements[i] != 0)
			insertBST(&temp, treeElements[i]);

		return temp;
}

int isBST(treeNode* binarySearchTree, int numberOfNodes) {
	int* treeElements;
	int number = 0;
	
	treeElements = storedTreeNodes(binarySearchTree, numberOfNodes);

	while (treeElements[number] != -1)
		number++;
	
	//search first leftchild
	if (treeElements[1] != 0)
		if (treeElements[1] > treeElements[0])
			return FALSE;

	//search first rightchild
	if (treeElements[2] != 0)
		if (treeElements[2] <= treeElements[0])
			return FALSE;

	//search remaining child
	for (int i = 3; i < number; i++) {
		if (treeElements[i] != 0) {

			if (i % 2 == 1)
				if (treeElements[i] > treeElements[(i - 1) / 2])
					return FALSE;

				else if (i % 2 == 0)
					if (treeElements[i] <= treeElements[(i / 2) - 1])
						return FALSE;

		}
	}
	
	return TRUE;
}
















