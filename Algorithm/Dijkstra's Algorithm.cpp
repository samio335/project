#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> 

#define MAX 100
#define TRUE 1
#define FALSE 0

#pragma warning(disable:4996)

typedef struct node {
	int d;
	int pi;
	int f;

}node;

int extract_min_heap(node v[], int n);


int main() {

	char s1[MAX];
	char s3[MAX];
	char s2[] = ",";
	char *tok;
	char *c[MAX];
	char *temp[MAX];
	int w[MAX][MAX];
	int adj[MAX][MAX];
	node v[MAX];
	int val, n, t1;
	int c1 = 0;
	int w1, w2;
	int wt1 = 0;
	int wt2 = 0;


	
	for(int x = 0; x < MAX; x++)
		for (int y = 0; y < MAX; y++)
			adj[x][y] = FALSE;

	val = scanf("%s", s1);

	tok = strtok(s1, ",");
	while (tok != NULL) {
		c[c1++] = tok;
		tok = strtok(NULL, ",");
	}

	v[0].d = 0;
	v[0].pi = 0;
	v[0].f = FALSE;

	for (int i = 1; i < c1; i++) {
		v[i].d = INT_MAX;
		v[i].pi = FALSE;
		v[i].f = FALSE;
	}

	val = scanf("%d", &n);


	for (int i = 0; i < n; i++) {
		t1 = 0;
		
		val = scanf("%s", s3);

		tok = strtok(s3, ",");
		while (tok != NULL) {
			temp[t1++] = tok;
			tok = strtok(NULL, ",");
		}
		
		for (int j = 0; j < c1; j++) {
			if (strcmp(c[j], temp[0]) == 0)
				w1 = j;
				
			
			if (strcmp(c[j], temp[1]) == 0)
				w2 = j;
		
		}

		w[w1][w2] = atoi(temp[2]);
		adj[w1][w2] = TRUE;
		
	}
	int q = 0;

	while (q != -1) {
		
		q = extract_min_heap(v, c1);

			for (int j = 0; j < c1; j++)
				if (adj[q][j] == TRUE){
					if (v[j].d > v[q].d + w[q][j]){
							v[j].d = v[q].d + w[q][j];
					}

				}
			
	}

	for (int i = 0; i < c1; i++)
		printf("%d\n", v[i].d);


	system("pause");

	return 0;
}

int extract_min_heap(node v[], int n) {

	int min = INT_MAX;
	int index = -1;

	for (int i = 0; i < n; i++) {
		if (v[i].f == FALSE) {
			if (v[i].d < min) {
				min = v[i].d;
				index = i;
			}
		}
	}
	if (index == -1)
		return -1;

	v[index].f = TRUE;

	return index;
}