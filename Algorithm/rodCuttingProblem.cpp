#include<stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)

int main() {
	
	int p[100];

	int n, val, i, j, q;

	val = scanf("%d", &n);

	for (i = 1; i <= n; i++)
		val = scanf("%d", &p[i]);


	int r[100];
	int s[100];

	r[0] = 0;

	for (j = 1; j <= n; j++) {
		q = -100;
		for (i = 1; i <= j; i++) {
			if (q < p[i] + r[j - i]) {
					q = p[i] + r[j - i];
					s[j] = i;
			}
		}
		r[j] = q;
	}

	printf("%d\n", r[n]);
	
	while (n > 0) {
		printf("%d\n", s[n]);
		n = n - s[n];
	}
	
	system("pause");

	return 0;
}