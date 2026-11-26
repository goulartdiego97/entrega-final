#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

typedef int chave_t;
typedef int dado_t;

#define DADO_NAO_EXISTE (-1)

typedef struct No {
    chave_t chave;
    dado_t dado;
    struct No *esq;
    struct No *dir;
    int altura;
    int nivel;
} No;

typedef No ABB;

static ABB *candidato_2vias = NULL;
static ABB *ultimo_2vias = NULL;

double tempo_segundos() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

void inicializa_vetor(int *vet, int n) {
    for (int i = 0; i < n; i++) {
        vet[i] = 2 * i;
    }
}

void embaralha_total(int *vet, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = vet[i];
        vet[i] = vet[j];
        vet[j] = tmp;
    }
}

void embaralha_parcial(int *vet, int n) {
    int num_pares = (n + 1) / 2;
    for (int i = num_pares - 1; i > 0; i--) {
        int idx_i = i * 2;
        int idx_j = (rand() % (i + 1)) * 2;
        int tmp = vet[idx_i];
        vet[idx_i] = vet[idx_j];
        vet[idx_j] = tmp;
    }
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

No* cria_no(chave_t chave, dado_t dado) {
    No *novo = (No*) malloc(sizeof(No));
    if (!novo) {
        perror("Erro de alocacao de memoria");
        exit(EXIT_FAILURE);
    }
    novo->chave = chave;
    novo->dado = dado;
    novo->esq = novo->dir = NULL;
    novo->altura = 1;
    novo->nivel = 1;
    return novo;
}

void abb_libera(ABB *a) {
    if (a != NULL) {
        abb_libera(a->esq);
        abb_libera(a->dir);
        free(a);
    }
}

ABB* abb_insere(ABB *a, chave_t chave, dado_t dado) {
    if (a == NULL) return cria_no(chave, dado);
    if (chave < a->chave) a->esq = abb_insere(a->esq, chave, dado);
    else if (chave > a->chave) a->dir = abb_insere(a->dir, chave, dado);
    return a;
}

dado_t* abb_busca3(ABB *a, chave_t chave) {
    if (a == NULL) return NULL;
    if (chave == a->chave) return &a->dado;
    if (chave < a->chave) return abb_busca3(a->esq, chave);
    return abb_busca3(a->dir, chave);
}

dado_t* abb_busca2(ABB *a, chave_t chave) {
    if (a != NULL) {
        if (chave < a->chave) {
            return abb_busca2(a->esq, chave);
        } else {
            candidato_2vias = a;
            return abb_busca2(a->dir, chave);
        }
    }
    dado_t *ret = NULL;
    if (candidato_2vias != NULL && chave == candidato_2vias->chave) {
        ret = &candidato_2vias->dado;
    }
    candidato_2vias = NULL;
    return ret;
}

ABB* abb_remove3(ABB *a, chave_t chave) {
    if (a == NULL) return NULL;
    if (chave < a->chave) a->esq = abb_remove3(a->esq, chave);
    else if (chave > a->chave) a->dir = abb_remove3(a->dir, chave);
    else {
        if (a->esq == NULL) {
            ABB *temp = a->dir;
            free(a);
            return temp;
        } else if (a->dir == NULL) {
            ABB *temp = a->esq;
            free(a);
            return temp;
        }
        ABB *p = a->dir;
        while (p->esq != NULL) p = p->esq;
        a->chave = p->chave;
        a->dado = p->dado;
        a->dir = abb_remove3(a->dir, p->chave);
    }
    return a;
}

ABB* abb_remove2(ABB *a, chave_t chave) {
    if (a == NULL) return a;
    ultimo_2vias = a;
    if (chave < a->chave) {
        a->esq = abb_remove2(a->esq, chave);
    } else {
        candidato_2vias = a;
        a->dir = abb_remove2(a->dir, chave);
    }
    if (a == ultimo_2vias) {
        if (candidato_2vias != NULL && chave == candidato_2vias->chave) {
            if (candidato_2vias != ultimo_2vias) {
                candidato_2vias->chave = ultimo_2vias->chave;
                candidato_2vias->dado = ultimo_2vias->dado;
                a = ultimo_2vias->dir;
            } else {
                a = ultimo_2vias->esq;
            }
            free(ultimo_2vias);
        }
        candidato_2vias = NULL;
    }
    return a;
}

int avl_altura(No *n) { return n ? n->altura : 0; }
int avl_fator_bal(No *n) { return n ? avl_altura(n->esq) - avl_altura(n->dir) : 0; }

No* rot_dir(No *y) {
    No *x = y->esq;
    No *T2 = x->dir;
    x->dir = y;
    y->esq = T2;
    y->altura = max(avl_altura(y->esq), avl_altura(y->dir)) + 1;
    x->altura = max(avl_altura(x->esq), avl_altura(x->dir)) + 1;
    return x;
}

No* rot_esq(No *x) {
    No *y = x->dir;
    No *T2 = y->esq;
    y->esq = x;
    x->dir = T2;
    x->altura = max(avl_altura(x->esq), avl_altura(x->dir)) + 1;
    y->altura = max(avl_altura(y->esq), avl_altura(y->dir)) + 1;
    return y;
}

ABB* avl_insere(ABB *a, chave_t chave, dado_t dado) {
    if (a == NULL) return cria_no(chave, dado);
    if (chave < a->chave) a->esq = avl_insere(a->esq, chave, dado);
    else if (chave > a->chave) a->dir = avl_insere(a->dir, chave, dado);
    else return a;

    a->altura = 1 + max(avl_altura(a->esq), avl_altura(a->dir));
    int fb = avl_fator_bal(a);

    if (fb > 1 && chave < a->esq->chave) return rot_dir(a);
    if (fb < -1 && chave > a->dir->chave) return rot_esq(a);
    if (fb > 1 && chave > a->esq->chave) {
        a->esq = rot_esq(a->esq);
        return rot_dir(a);
    }
    if (fb < -1 && chave < a->dir->chave) {
        a->dir = rot_dir(a->dir);
        return rot_esq(a);
    }
    return a;
}

ABB* avl_remove3(ABB *a, chave_t chave) {
    if (a == NULL) return NULL;
    if (chave < a->chave) a->esq = avl_remove3(a->esq, chave);
    else if (chave > a->chave) a->dir = avl_remove3(a->dir, chave);
    else {
        if ((a->esq == NULL) || (a->dir == NULL)) {
            ABB *temp = a->esq ? a->esq : a->dir;
            if (temp == NULL) {
                temp = a;
                a = NULL;
            } else *a = *temp;
            free(temp);
        } else {
            ABB *p = a->dir;
            while (p->esq != NULL) p = p->esq;
            a->chave = p->chave;
            a->dado = p->dado;
            a->dir = avl_remove3(a->dir, p->chave);
        }
    }
    if (a == NULL) return a;

    a->altura = 1 + max(avl_altura(a->esq), avl_altura(a->dir));
    int fb = avl_fator_bal(a);

    if (fb > 1 && avl_fator_bal(a->esq) >= 0) return rot_dir(a);
    if (fb > 1 && avl_fator_bal(a->esq) < 0) {
        a->esq = rot_esq(a->esq);
        return rot_dir(a);
    }
    if (fb < -1 && avl_fator_bal(a->dir) <= 0) return rot_esq(a);
    if (fb < -1 && avl_fator_bal(a->dir) > 0) {
        a->dir = rot_dir(a->dir);
        return rot_esq(a);
    }
    return a;
}

ABB* aa_skew(ABB *a) {
    if (a == NULL) return NULL;
    if (a->esq != NULL && a->esq->nivel == a->nivel) {
        ABB *l = a->esq;
        a->esq = l->dir;
        l->dir = a;
        return l;
    }
    return a;
}

ABB* aa_split(ABB *a) {
    if (a == NULL) return NULL;
    if (a->dir != NULL && a->dir->dir != NULL && a->dir->dir->nivel == a->nivel) {
        ABB *r = a->dir;
        a->dir = r->esq;
        r->esq = a;
        r->nivel++;
        return r;
    }
    return a;
}

ABB* aa_insere(ABB *a, chave_t chave, dado_t dado) {
    if (a == NULL) return cria_no(chave, dado);
    if (chave < a->chave) a->esq = aa_insere(a->esq, chave, dado);
    else if (chave > a->chave) a->dir = aa_insere(a->dir, chave, dado);
    else return a;

    a = aa_skew(a);
    a = aa_split(a);
    return a;
}

ABB* aa_remove3(ABB *a, chave_t chave) {
    if (a == NULL) return NULL;
    if (chave < a->chave) a->esq = aa_remove3(a->esq, chave);
    else if (chave > a->chave) a->dir = aa_remove3(a->dir, chave);
    else {
        if (a->esq == NULL && a->dir == NULL) {
            free(a);
            return NULL;
        } else if (a->esq == NULL) {
            ABB *l = a->dir;
            while (l->esq != NULL) l = l->esq;
            a->chave = l->chave;
            a->dado = l->dado;
            a->dir = aa_remove3(a->dir, l->chave);
        } else {
            ABB *l = a->esq;
            while (l->dir != NULL) l = l->dir;
            a->chave = l->chave;
            a->dado = l->dado;
            a->esq = aa_remove3(a->esq, l->chave);
        }
    }

    int n_esq = a->esq ? a->esq->nivel : 0;
    int n_dir = a->dir ? a->dir->nivel : 0;
    if (n_esq < a->nivel - 1 || n_dir < a->nivel - 1) {
        a->nivel--;
        if (n_dir > a->nivel) a->dir->nivel = a->nivel;
        a = aa_skew(a);
        a->dir = aa_skew(a->dir);
        if (a->dir) a->dir->dir = aa_skew(a->dir->dir);
        a = aa_split(a);
        a->dir = aa_split(a->dir);
    }
    return a;
}

void executa_experimento(const char* nome_arvore, int tipo_vias, int n, int modo_embaralha, int *vetor_base) {
    ABB *raiz = NULL;
    double t_inicio, t_fim;
    double t_inclusao, t_busca_sucesso, t_busca_insucesso, t_remocao;

    t_inicio = tempo_segundos();
    for (int i = 0; i < n; i++) {
        if (strcmp(nome_arvore, "ABB") == 0) raiz = abb_insere(raiz, vetor_base[i], vetor_base[i]);
        else if (strcmp(nome_arvore, "AVL") == 0) raiz = avl_insere(raiz, vetor_base[i], vetor_base[i]);
        else if (strcmp(nome_arvore, "AA") == 0) raiz = aa_insere(raiz, vetor_base[i], vetor_base[i]);
    }
    t_fim = tempo_segundos();
    t_inclusao = t_fim - t_inicio;

    t_inicio = tempo_segundos();
    for (int i = 0; i < n; i++) {
        if (tipo_vias == 3) abb_busca3(raiz, vetor_base[i]);
        else abb_busca2(raiz, vetor_base[i]);
    }
    t_fim = tempo_segundos();
    t_busca_sucesso = t_fim - t_inicio;

    t_inicio = tempo_segundos();
    for (int i = 0; i < n; i++) {
        int chave_impar = 2 * i + 1;
        if (tipo_vias == 3) abb_busca3(raiz, chave_impar);
        else abb_busca2(raiz, chave_impar);
    }
    t_fim = tempo_segundos();
    t_busca_insucesso = t_fim - t_inicio;

    t_inicio = tempo_segundos();
    for (int i = 0; i < n; i++) {
        if (tipo_vias == 3) {
            if (strcmp(nome_arvore, "ABB") == 0) raiz = abb_remove3(raiz, vetor_base[i]);
            else if (strcmp(nome_arvore, "AVL") == 0) raiz = avl_remove3(raiz, vetor_base[i]);
            else if (strcmp(nome_arvore, "AA") == 0) raiz = aa_remove3(raiz, vetor_base[i]);
        } else {
            raiz = abb_remove2(raiz, vetor_base[i]);
        }
    }
    t_fim = tempo_segundos();
    t_remocao = t_fim - t_inicio;

    abb_libera(raiz);

    printf("%s,%d_vias,%d,%s,%.6f,%.6f,%.6f,%.6f\n",
           nome_arvore, tipo_vias, n,
           (modo_embaralha == 0 ? "Total" : "Parcial"),
           t_inclusao, t_busca_sucesso, t_busca_insucesso, t_remocao);
}

int main(int argc, char *argv[]) {
    srand(42);

    int n_pequeno = 20000;
    int n_medio   = 40000;
    int n_grande  = 80000;

    int tamanhos[3] = {n_pequeno, n_medio, n_grande};
    const char *arvores[3] = {"ABB", "AVL", "AA"};

    printf("Arvore,Vias,N,Embaralhamento,Inclusao_s,BuscaSucesso_s,BuscaInsucesso_s,Remocao_s\n");

    for (int t = 0; t < 3; t++) {
        int n = tamanhos[t];
        int *vetor = (int*) malloc(n * sizeof(int));

        for (int modo = 0; modo < 2; modo++) {
            for (int a = 0; a < 3; a++) {
                for (int vias = 2; vias <= 3; vias++) {

                    inicializa_vetor(vetor, n);
                    if (modo == 0) embaralha_total(vetor, n);
                    else embaralha_parcial(vetor, n);

                    executa_experimento(arvores[a], vias, n, modo, vetor);
                }
            }
        }
        free(vetor);
    }

    return 0;
}