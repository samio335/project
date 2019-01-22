#include<stdio.h>
#include <stdlib.h>
#include <queue>
#include <functional>

#define MAX 10000

using namespace std;
priority_queue<int, vector<int>, greater<int> > minpq;
#pragma warning(disable:4996)

typedef struct treeNode {
	int data;
	int left;
	int right;

}treeNode;


int main() {

	int n, val, i, j, a, b, c, result;
	treeNode huffman[MAX];
	int p[MAX];
	int vlc[MAX];
	int parent[MAX];

	for (i = 0; i < MAX; i++)
		parent[i] = -1;

	val = scanf("%d", &n);


	for (i = 1; i <= n; i++) {
		val = scanf("%d", &p[i]);
		minpq.push(p[i]);
	}

	if (n >= 2)
		for (i = 1; i <= n - 1; i++) {

			a = minpq.top();
			minpq.pop();

			b = minpq.top();
			minpq.pop();

			c = a + b;
			result = c;
			huffman[c].data = c;
			huffman[c].left = a;
			huffman[c].right = b;

			parent[a] = c;
			parent[b] = c;

			minpq.push(c);
		}

	int test;
	int v = 0;
	
	if (n >= 2)
	for(int x = 1; x <= n; x++){

		test = p[x];

	while (parent[test] != -1) {
		
		if (huffman[parent[test]].left == test) 
			vlc[v++] = 0;

		if (huffman[parent[test]].right == test)
			vlc[v++] = 1;

		test = parent[test];
		
	}
	
	for (i = 1; i <= v; i++) 
		printf("%d", vlc[v-i]);

	printf("\n");
	v = 0;

	}
	
	if (n == 1)
		printf("%d\n", p[1]);
	
	system("pause");

	return 0;
}

