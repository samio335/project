#include<stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)

#define MAX_SIZE 100
#define SWAP(x,y,t) ((t) = (x), (x) = (y), (y) = (t))

void recursive_activity_selector(int s[], int f[], int k, int n, int a[], int a1);
void quickSort(int a[], int b[], int left, int right);

int main() {

	int s[MAX_SIZE];
	int f[MAX_SIZE];
	int a[MAX_SIZE];
	int p[MAX_SIZE];

	f[0] = 0;
	a[0] = 0;

	int n, val, i, random, temp;
	int s1 = 1;
	int f1 = 1;
	int j = 1;

	val = scanf("%d", &n);

	for (i = 1; i <= n * 2; i++)
		val = scanf("%d", &p[i]);


	for (i = 1; i <= n * 2; i++) {
		if (i % 2 == 1) {
			s[s1] = p[i];
			s1++;
		}
		else {
			f[f1] = p[i];
			f1++;
		}
	}

	random = rand() % n + 1;
	SWAP(f[1], f[random], temp);
	SWAP(s[1], s[random], temp);
	quickSort(f, s, 1, n);




	recursive_activity_selector(s, f, 0, n, a, 1);


	printf("%d\n", a[0]);
	for (i = 1; i <= a[0]; i++) {
		printf("%d\n", s[a[j]]);
		printf("%d\n", f[a[j]]);

		j++;
	}
	system("pause");

	return 0;
}

void recursive_activity_selector(int s[], int f[], int k, int n, int a[], int a1) {

	int m;
	m = k + 1;

	while (m <= n && s[m] < f[k])
		m = m + 1;

	if (m <= n) {
		a[0] = a[0] + 1;
		a[a1] = m;
		recursive_activity_selector(s, f, m, n, a, a1 + 1);
	}
}

void quickSort(int a[], int b[], int left, int right) {

	int pivot, i, j;
	int temp;

	if (left < right) {

		i = left;
		j = right + 1;


		pivot = a[left];

		do {

			do i++; while (a[i] < pivot);
			do j--; while (a[j] > pivot);
			if (i < j) {
				SWAP(a[i], a[j], temp);
				SWAP(b[i], b[j], temp);
			}

		} while (i < j);

		SWAP(a[left], a[j], temp);
		SWAP(b[left], b[j], temp);
		quickSort(a, b, left, j - 1);
		quickSort(a, b, j + 1, right);

	}
}