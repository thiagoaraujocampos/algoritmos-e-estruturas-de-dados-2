// Nome: Thiago Araujo Campos 
// Engenharia de Computaçãp
// RA: 769850

// -Wall -pedantic -O2 -Wno-unused-result

#include <stdio.h>
#include <stdlib.h>

#define PAI(i) (i - 1) / 2
#define FILHO_ESQ(i) (2 * i + 1)
#define FILHO_DIR(i) (2 * i + 2)

typedef struct noh Noh;
struct noh
{
    int rotulo;
    int custo;
    Noh *prox;
};

typedef struct grafo *Grafo;
struct grafo
{
    Noh **A;
    int n; // número de vértices
    int m; // número de arestas/arcos
};

typedef struct elem
{
    int chave;    // vamos guardar dist aqui
    int conteudo; // vamos guardar o vértice aqui
} Elem;

void troca(Elem *a, Elem *b)
{
    Elem aux;
    aux.chave = a->chave;
    aux.conteudo = a->conteudo;
    a->chave = b->chave;
    a->conteudo = b->conteudo;
    b->chave = aux.chave;
    b->conteudo = aux.conteudo;
}
void troca_pos(int *a, int *b)
{   
    int aux;
    aux = *a;
    *a = *b;
    *b = aux;

}

void sobeHeap(Elem v[], int pos_v[], int pos_elem_v)
{
    int f = pos_elem_v;
    while (f > 0 && v[PAI(f)].chave > v[f].chave)
    {
        troca(&v[f], &v[PAI(f)]);
        troca_pos(&pos_v[v[f].conteudo], &pos_v[v[PAI(f)].conteudo]);
        f = PAI(f);
    }
}
int insereHeap(Elem v[], int pos_v[], int m, Elem x)
{
    v[m] = x;
    pos_v[x.conteudo] = m;
    sobeHeap(v, pos_v, m);
    return m + 1;
}
// desce o elemento em v[pos_elem_v] até restaurar a prop. do heap
void desceHeap(Elem v[], int pos_v[], int m, int pos_elem_v)
{
    int p = pos_elem_v, f;
    while (FILHO_ESQ(p) < m && (v[FILHO_ESQ(p)].chave < v[p].chave || (FILHO_DIR(p) < m && v[FILHO_DIR(p)].chave < v[p].chave)))
    {
        f = FILHO_ESQ(p);
        if (FILHO_DIR(p) < m && v[FILHO_DIR(p)].chave < v[f].chave)
            f = FILHO_DIR(p);
        troca(&v[p], &v[f]);
        troca_pos(&pos_v[v[p].conteudo], &pos_v[v[f].conteudo]);
        p = f;
    }
}
int removeHeap(Elem v[], int pos_v[], int m, Elem *px)
{
    *px = v[0];
    troca(&v[0], &v[m - 1]);
    troca_pos(&pos_v[v[0].conteudo], &pos_v[v[m - 1].conteudo]);
    desceHeap(v, pos_v, m, 0);
    return m - 1;
}
void atualizaHeap(Elem v[], int pos_v[], Elem x)
{
    int pos_x_v = pos_v[x.conteudo]; // pega a posição de x em v
    v[pos_x_v].chave = x.chave;      // atualiza a chave de x em v
    sobeHeap(v, pos_v, pos_x_v);     // Quiz3: por que mando subir e não
                                     //descer ? Em que situação mandaria descer ?
}

/* Insere arco v-w no grafo G, supondo que v e w são inteiros
distintos entre 0 e n-1. Se grafo já tem arco v-w, não faz nada. */
void insereArcoGrafo(Grafo G, int v, int w, int custo)
{
    Noh *p;
    for (p = G->A[v]; p != NULL; p = p->prox)
        if (p->rotulo == w)
            return;
    p = (Noh *)malloc(sizeof(Noh));
    p->rotulo = w;
    p->custo = custo;
    p->prox = G->A[v];
    G->A[v] = p;
    G->m++;
}

Grafo inicializaGrafo(int n, int E)
{
    int i, A, B, W;

    Grafo G = malloc(sizeof *G);
    G->n = n;
    G->m = 0;
    G->A = malloc(n * sizeof(Noh *));
    for (i = 0; i < n; i++)
        G->A[i] = NULL;

    for (int i = 0; i < E; i++)
    {
        scanf("%d %d %d", &A, &B, &W);
        insereArcoGrafo(G, A, B, W);
    }

    return G;
}

// Libera a memória alocada para o grafo G e devolve NULL.
Grafo liberaGrafo(Grafo G)
{
    int i;
    Noh *p;
    for (i = 0; i < G->n; i++)
    {
        p = G->A[i];
        while (p != NULL)
        {
            G->A[i] = p;
            p = p->prox;
            free(G->A[i]);
        }
        G->A[i] = NULL;
    }
    free(G->A);
    G->A = NULL;
    free(G);
    return NULL;
}

int dijkstra(int origem, int tam, int E)
{
    Grafo G;
    
    G = inicializaGrafo(tam, E);

    int *dist = malloc(tam * sizeof(int));
    int *pred = malloc(tam * sizeof(int));

    int i, *pos_H, v, w, custo, tam_H;
    Elem *H, elem_aux;
    Noh *p;
    // inicializando distâncias e predecessores
    for (i = 0; i < G->n; i++)
    {
        dist[i] = __INT_MAX__;
        pred[i] = -1;
    }
    dist[origem] = 0;
    // criando um min heap em H com vetor de posições pos_H
    H = malloc(G->n * sizeof(Elem));
    pos_H = malloc(G->n * sizeof(int));
    for (i = 0; i < G->n; i++)
    {
        H[i].chave = dist[i]; // chave é a "distância" do vértice
        H[i].conteudo = i;    // conteúdo é o rótulo do vértice
        pos_H[i] = i;         // vértice i está na posição i do heap H
    }
    troca(&H[0], &H[origem]);             // coloca origem no início do heap H
    troca_pos(&pos_H[0], &pos_H[origem]); // atualiza posição
    tam_H = G->n;
    while (tam_H > 0)
    { // enquanto não visitar todo vértice
        // buscando vértice v que minimiza dist[v]
        tam_H = removeHeap(H, pos_H, tam_H, &elem_aux);
        v = elem_aux.conteudo;
        // percorrendo lista com vizinhos de v
        for (p = G->A[v]; p != NULL; p = p->prox)
        {
            w = p->rotulo;
            custo = p->custo;
            // e atualizando as distâncias e predecessores quando for o caso
            if (dist[w] > dist[v] + custo)
            {
                dist[w] = dist[v] + custo;
                pred[w] = v;
                elem_aux.chave = dist[w];
                elem_aux.conteudo = w;
                atualizaHeap(H, pos_H, elem_aux);
            }
        }
    }

    int sol = dist[tam - 1];

    free(H);
    free(pos_H);
    free(dist);
    free(pred);

    liberaGrafo(G);

    return sol;
}

int main()
{
    // V = representando os números de vértives
    // E = representando os números de arestas
    // A e B estão ligados por uma aresta de peso W

    int V, E;

    scanf("%d %d", &V, &E);

    printf("%d\n", dijkstra(0, V, E));

    return 0;
}