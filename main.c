
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

typedef struct nodedist {
	int dist;
	int i;
	int j;
	int ind;
}nodedist;

typedef struct heap {
	nodedist *arr[80*80+1];
	int siz;
}heap;


#define left(i) (2*(i))
#define right(i) (2*(i) + 1)
#define parent(i) ((i)>>1)
#define swap(h, i, j) aux = h->arr[i]; h->arr[i] = h->arr[j]; h->arr[j] = aux; h->arr[i]->ind = i; h->arr[j]->ind = j;
#define top(h) (h->arr[1])


nodedist* min_pop(heap *h);
heap* new_min_heap(int n);
void min_decrease(heap *h, int i, int64_t dist);


heap static_heap;
int matrix[80][80];
nodedist nodedistmat[80][80];
int visited[80][80];

void solve(int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			nodedistmat[i][j].i = i;
			nodedistmat[i][j].j = j;
			nodedistmat[i][j].dist = INT32_MAX;
		}
	}
	nodedistmat[0][0].i = 0;
	nodedistmat[0][0].j = 0;
	nodedistmat[0][0].dist = matrix[0][0];

	heap *h = new_min_heap(n);
	nodedist *v, *u;
	int ind;
	int64_t alt;
	while (h->siz)
	{
		v = min_pop(h);
		visited[v->i][v->j] = 1;

		if (v->i == n - 1 && v->j == n - 1) break;
        
		//up
		if (v->i > 0 && !visited[v->i - 1][v->j])
		{
			u = &nodedistmat[v->i - 1][v->j];
			alt = v->dist + matrix[v->i - 1][v->j];
			if (alt < u->dist)
				min_decrease(h, u->ind, alt);
		}

		//down
		if (v->i < n - 1 && !visited[v->i + 1][v->j])
		{
			u = &nodedistmat[v->i + 1][v->j];
			alt = v->dist + matrix[v->i + 1][v->j];
			if (alt < u->dist)
				min_decrease(h, u->ind, alt);
		}

		//right
		if (v->j < n - 1 && !visited[v->i][v->j + 1])
		{
			u = &nodedistmat[v->i][v->j + 1];
			alt = v->dist + matrix[v->i][v->j + 1];
			if (alt < u->dist)
				min_decrease(h, u->ind, alt);
		}

        
		//left
		if (v->j > 0 && !visited[v->i][v->j - 1])
		{
			u = &nodedistmat[v->i][v->j - 1];
			alt = v->dist + matrix[v->i][v->j - 1];
			if (alt < u->dist)
				min_decrease(h, u->ind, alt);
		}
        
	}

	int ans = nodedistmat[n - 1][n - 1].dist;
	printf("%d\n", ans);
}




int load_data();
int main()
{
    load_data();
    solve(80);

	return 0;
}



//min heap

void min_heapify(heap *h, int i) {
	if (i < 1 || i > h->siz) return;

	nodedist* aux;
	int l = left(i);
	int r = right(i);
	int min = i;
	if (r <= h->siz && h->arr[r]->dist < h->arr[min]->dist) {
		min = r;
	}
	if (l <= h->siz && h->arr[l]->dist < h->arr[min]->dist) {
		min = l;
	}

	if (min != i) {
		swap(h, i, min);
		min_heapify(h, min);
	}

}

heap* new_min_heap(int n) {
	heap *h = &static_heap;
	int len = n*n;
	h->siz = len;
	int k = 1;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			h->arr[k] = &nodedistmat[i][j];
			nodedistmat[i][j].ind = k;
			k++;
		}
	}

	for (int i = len / 2; i >= 1; i--)
		min_heapify(h, i);

	return h;
}

nodedist* min_pop(heap *h) {
	nodedist* aux;
	nodedist* ret = top(h);
	swap(h, 1, h->siz);
	h->siz--;
	min_heapify(h, 1);

	return ret;
}


void min_decrease(heap *h, int i, int64_t dist)
{
	nodedist *aux;
	h->arr[i]->dist = dist;
	int p = parent(i);
	while (i > 1 && h->arr[p]->dist > h->arr[i]->dist) {
		swap(h, p, i);
		i = p;
		p = parent(i);
	}

}

char linebuf[4096];
int load_data()
{
    FILE *pf = fopen("p083_matrix.txt", "r");
    if(!pf){
        fprintf(stderr, "Can't open file\n");
        return 1;
    }

    int i=0;
    while(fgets(linebuf, sizeof(linebuf), pf)){
        char *token = linebuf;

        for(int j = 0;;j++){
            matrix[i][j] = (int)strtol(token, &token, 10);
            if(*token != ',') break;
            token++;
        }
        i++;
    }

    fclose(pf);

    return 0;
}