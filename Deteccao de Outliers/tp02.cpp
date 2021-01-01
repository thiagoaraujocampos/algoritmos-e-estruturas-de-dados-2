// Nome: Thiago Araujo Campos 
// Engenharia de Computaçãp
// RA: 769850

// -Wall -pedantic -O2 -Wno-unused-result

#include <stdio.h>
#include <stdlib.h>

#define MAX 100

void merge(double v[], int l, int m, int r)
{
    int n1 = m - l + 1;
    int n2 = r - m;
 
    double L[n1], R[n2];
 
    for (int i = 0; i < n1; i++)
        L[i] = v[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = v[m + 1 + j];
 
    int i = 0;
    int j = 0;
    int k = l;
 
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            v[k] = L[i];
            i++;
        }
        else {
            v[k] = R[j];
            j++;
        }
        k++;
    }
 
    while (i < n1) {
        v[k] = L[i];
        i++;
        k++;
    }
 
    while (j < n2) {
        v[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(double v[], int l, int r){
    if(l>=r){
        return;
    }
    int m = (l+r-1)/2;
    mergeSort(v,l,m);
    mergeSort(v,m+1,r);
    merge(v,l,m,r);
}

int outliers(double** v, double limiar, int n, int k) {
    int contador = 0;
    for (int i = 0; i < n; i++) {
        mergeSort(&(**(v+i)), 0, n-1);
        if (v[i][k+1] > limiar)
            contador++;
    }
    return contador;
}

int main(int argc, char *argv[])
{
    char file_name[MAX];
    double **v, limiar;
    int n, k, i, j;
    FILE *entrada;

    scanf("%s", file_name);
    entrada = fopen(file_name, "r");
    if (entrada == NULL)
    {
        printf("\nNão encontrei o arquivo!\n");
        exit(EXIT_FAILURE);
    }
    fscanf(entrada, "%d %d %lf", &n, &k, &limiar);
    v = (double **)malloc(n * sizeof(double *));
    for (i = 0; i < n; i++)
    {
        v[i] = (double *)malloc(n * sizeof(double));
        for (j = 0; j < n; j++)
            fscanf(entrada, "%lf", &v[i][j]);
    }
    fclose(entrada);

    printf("%d", outliers(v, limiar, n, k));

    for (i = 0; i < n; i++)
        free(v[i]);
    free(v);

    return 0;
}
