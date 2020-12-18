// Nome: Thiago Araujo Campos 
// Engenharia de Computaçãp
// RA: 769850

// -Wall -pedantic -O2 -Wno-unused-result

#include <iostream>
#include <string.h>
using namespace std;

int hashing(char* chave, int M) {
  int i, h = 0;
  int primo = 127;
  for (i = 0; chave[i] != '\0'; i++)
    h = (h * primo + chave[i]) % M;
  return h;
}

typedef struct celTS CelTS;

struct celTS {
  char* chave;
  char* valor;
  CelTS *prox;
};

static CelTS **tab = NULL;
static int nChaves = 0;
static int M;

void *mallocSafe(unsigned nbytes) {
  void *p;
  p = malloc(nbytes);
  if (p == NULL) {
    printf("Malloc devolveu NULL!\n");
    exit(EXIT_FAILURE);
  }
  return p;
}

char* copyString(char* str) {
  char* novoStr = NULL;
  int len = strlen(str) + 1;
  novoStr = (char*) mallocSafe(len * sizeof(char));
  strncpy(novoStr, str, len);
  return novoStr;
}

void stInit(int max) {
  int h;
  M = max;
  nChaves = 0;
  tab = (CelTS**) mallocSafe(M * sizeof(CelTS *));
  for (h = 0; h < M; h++) 
    tab[h] = NULL;
}

char* stSearch(char* chave) {
  CelTS *p;
  int h = hashing(chave, M);
  p = tab[h];
  while (p != NULL && strcmp(p->chave, chave) != 0)
    p = p->prox;
  if (p != NULL) {
    return p->valor;
  }
  return NULL;
} 

void stInsert(char* chave, char* valor) {
  CelTS *p;
  int h = hashing(chave, M);
  p = tab[h];
  while (p != NULL && strcmp(p->chave, chave))
    p = p-> prox;
  if (p == NULL) {
    p = (CelTS*) mallocSafe(sizeof(*p));
    p->chave = copyString(chave);
    nChaves += 1;
    p->prox = tab[h];
    tab[h] = p;
  }
  p->valor = copyString(valor);
}

void stDelete(char* chave) {
  CelTS *p, *aux;
  int h = hashing(chave, M);
  p = tab[h];
  if (p == NULL)
    return;
  if (strcmp(p->chave, chave) == 0) {
    tab[h] = p->prox;
    free(p->chave);
    free(p);
    nChaves--;
    return;
  }
  while (p->prox != NULL && strcmp((p->prox)->chave, chave) != 0)
    p = p->prox;
  if (p->prox != NULL) {
    aux = p->prox;
    p->prox = aux->prox;
    free(aux->chave);
    free(aux);
    nChaves--;
  }
}

void stFree() {
  CelTS *p = NULL, *q = NULL;
  int h;
  for (h = 0; h < M; h++) {
    p = tab[h];
    while (p != NULL) {
      q = p;
      p = p->prox;
      free(q->chave);
      free(q);
    }
  }
  free(tab);
  tab = NULL;
  nChaves = 0;
}

void agenda() {
  stInit(9009899);

  char op, nome[11], telefone[10];
  cin >> op;

  while(op != '0') {
    cin >> nome;

    switch (op) {
      case 'I':
        cin >> telefone;
        if (stSearch(&nome[0]) == NULL) {
          stInsert(&nome[0], &telefone[0]);
        } else {
          printf("Contatinho ja inserido\n");
        }
        break;
      case 'P':
        if (stSearch(&nome[0]) != NULL) {
          printf("Contatinho encontrado: telefone %s\n", stSearch(&nome[0]));
        } else {
          printf("Contatinho nao encontrado\n");
        }
        break;
      case 'R':
        if (stSearch(&nome[0]) != NULL) {
          stDelete(&nome[0]);
        } else {
          printf("Operacao invalida: contatinho nao encontrado\n");
        }
        break;
      case 'A':
        cin >> telefone;
        if (stSearch(&nome[0]) == NULL) {
          printf("Operacao invalida: contatinho nao encontrado\n");
        } else {
          stInsert(&nome[0], &telefone[0]);
        }
        break;
        
      default:
        break;
    }

    cin >> op;
  }

  stFree();
}

int main() {

  agenda();

  return 0;
}