#include <stdio.h>
#include <stdlib.h>
#define MAX 100

void copy_matrix(int m1[][MAX], int m2[][MAX], int N);
int arrsum(int a[], int N);
int count_routes(int m[][MAX], int i, int j, int si, int sj, int N);
void print_matrix(int **m, int N);
void fill_blanks_in_routes(int **routes);

int main()
{
	int j = 0, N = 4, **routes;
	int matrix[MAX][MAX] = {{1, 0, 0, 0},
		 					{0, 0, 0, 0},
		 					{0, 3, 4, 0},
		 					{0, 0, 0, 1}};
		 					
	routes = (int**) calloc(1000, sizeof(int*));
	for (j = 0; j < 1000; j++)
		routes[j] = (int*) calloc(1000, sizeof(int));
	
	printf("Number of possible routes: %d\n", count_routes(matrix, 0, 0, 0, 0, N));
	find_save_routes(matrix, 0, 0, 0, 0, 0, 1, routes, N);
	fill_blanks_in_routes(routes);
	print_matrix(routes, 1000);
	free(routes);

	return 0;	
}

void copy_matrix(int m1[][MAX], int m2[][MAX], int N)
{
	int i, j;
	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			m2[i][j] = m1[i][j];
}

int arrsum(int a[], int N) 
{
	int i, r = 0;
	for (i = 0; i < N; i++)
		r += a[i];
	return r;
}

int count_routes(int m[][MAX], int i, int j, int si, int sj, int N)
{
	int mc[MAX][MAX], r[4] = {0};
	
	if ((i != si || j != sj) && m[i][j] == 1)
		return 1;
		
	copy_matrix(m, mc, N);
	mc[i][j] = N+1;
	
	if (isBlocked(mc, i+1, j, N) == 0) 
		r[0] = count_routes(mc, i+1, j, si, sj, N);

	if (isBlocked(mc, i-1, j, N) == 0) 
		r[1] = count_routes(mc, i-1, j, si, sj, N);

	if (isBlocked(mc, i, j+1, N) == 0) 
		r[2] = count_routes(mc, i, j+1, si, sj, N);

	if (isBlocked(mc, i, j-1, N) == 0) 
		r[3] = count_routes(mc, i, j-1, si, sj, N);

	return arrsum(r, 4);
}

int isBlocked(int m[][MAX], int i, int j, int N)
{
	if (i < 0 || i >= N || j < 0 || j >= N) return 1;
	if (m[i][j] != 0 && m[i][j] != 1) return 1;
	return 0;
}

int find_save_routes(int m[][MAX], int i, int j, int si, int sj, int rc, int depth, int **routes, int N)
{
	int mc[MAX][MAX];
	
	routes[rc][depth] = i+1;
	routes[rc][depth+1] = j+1;
	
	if ((i != si || j != sj) && m[i][j] == 1) {
		routes[rc][0] = 1;
		routes[rc][depth+2] = 0;
		return rc+1;
	}
		
	copy_matrix(m, mc, N);
	mc[i][j] = N+1;
	
	if (isBlocked(mc, i+1, j, N) == 0) 
		rc = find_save_routes(mc, i+1, j, si, sj, rc, depth+2, routes, N);

	if (isBlocked(mc, i-1, j, N) == 0) 
		rc = find_save_routes(mc, i-1, j, si, sj, rc, depth+2, routes, N);

	if (isBlocked(mc, i, j+1, N) == 0) 
		rc = find_save_routes(mc, i, j+1, si, sj, rc, depth+2, routes, N);

	if (isBlocked(mc, i, j-1, N) == 0) 
		rc = find_save_routes(mc, i, j-1, si, sj, rc, depth+2, routes, N);

	return rc;
}

void print_matrix(int **m, int N)
{
	int i, j, c = 0, f;
	for (i = 0; i < N; i++) {
		if (m[i][0] == 1) {
			if (m[i][0] == 1) 
			{
				c++;
				printf("%3d: ", c);
				f = 0;
				for (j = 1; j < N; j+=2) {
					if (m[i][j] != 0) {
						printf("(%d %d) ", m[i][j], m[i][j+1]);
						f = 1;
					}
					else if (m[i][j] == 0 && f == 0) printf("      ");
					else break;
				}
				printf("\n");						
			}
		}
	}
}

// first element saves state, 1 indexed
void fill_blanks_in_routes(int **routes)
{
	int i = 1, j;
	
	while (routes[i][0] != 0) {
		j = 1;
		while (routes[i][j] == 0) {
			routes[i][j] = routes[i-1][j];
			j++;
		}
		i++;
	}
}


