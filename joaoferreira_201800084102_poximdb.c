#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
    false,
    true
} bool;

typedef struct
{
    int qntd;
    char chave[33];
    char nome[30];
} Chave;

typedef struct Node
{
    int n_chaves;
    Chave *chaves;
    struct Node **ptr_node;
    bool folha;
} Node;

Node *criaNode(Node *no, int ordem)
{
    no = malloc(sizeof(Node));
    no->folha = true;
    no->n_chaves = 0;
    no->chaves = (Chave *)malloc((ordem - 1) * sizeof(Chave));
    no->ptr_node = (Node **)malloc(ordem * sizeof(struct Node *));
    return no;
}

void insertionSort(Chave *array, int tamanho)
{
    int i, j;
    Chave item;
    for (i = 1; i < tamanho; i++)
    {
        item = array[i];
        j = i - 1;
        while (j >= 0 && strcmp(array[j].chave, item.chave) > 0)
        {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = item;
    }
}

void copiaNode(Node *sourc, Node *dest, int ordem, int m)
{
    int i, j = 0;
    for (i = m; i < ordem - 1; i++)
    {
        dest->chaves[j] = sourc->chaves[i];
        dest->ptr_node[j + 1] = sourc->ptr_node[i + 1];
        dest->n_chaves++;
        sourc->n_chaves--;
        j++;
    }
}

int ordenaNode(Node *no, Chave chave)
{
    int i = no->n_chaves;
    while (i > 0 && (strcmp(no->chaves[i - 1].chave, chave.chave) > 0))
    {
        no->chaves[i] = no->chaves[i - 1];
        no->ptr_node[i + 1] = no->ptr_node[i];
        i--;
    }
    no->chaves[i] = chave;
    no->n_chaves++;
    return i;
}

int posDescida(Chave *array, int pos, Chave chave)
{
    while (pos > 0 && (strcmp(array[pos - 1].chave, chave.chave) > 0))
    {
        pos--;
    }
    return pos;
}

void divide(Node *no, Chave chave, Node *direita, Node *novo, Chave *chave_promovida, int ordem)
{
    if (no->folha == false)
    {
        novo->folha = false;
    }
    int m, pos;
    m = (int)(ordem - 1) / 2;
    if (strcmp(chave.chave, no->chaves[m].chave) < 0)
    {
        copiaNode(no, novo, ordem, m);
        no->chaves[no->n_chaves] = chave;
        no->n_chaves++;
        insertionSort(no->chaves, no->n_chaves);
        pos = posDescida(no->chaves, no->n_chaves, chave);
        novo->ptr_node[0] = no->ptr_node[pos];
        no->ptr_node[pos] = direita;
    }
    else
    {
        copiaNode(no, novo, ordem, m + 1);
        novo->chaves[novo->n_chaves] = chave;
        novo->n_chaves++;
        insertionSort(novo->chaves, novo->n_chaves);
        pos = posDescida(novo->chaves, novo->n_chaves, chave);
        novo->ptr_node[0] = no->ptr_node[no->n_chaves];
        novo->ptr_node[pos] = direita;
    }
    *chave_promovida = no->chaves[(no->n_chaves) - 1];

    no->n_chaves--;
}

bool buscaInsere(Node *atual, Chave chave, bool *promocao, Chave *chave_promovida, Node **novo, int ordem)
{
    int i;
    bool jaExiste = false;
    for (i = 0; i < atual->n_chaves; i++)
    {
        if (strcmp(chave.chave, atual->chaves[i].chave) == 0)
        {
            jaExiste = true;
        }
    }
    if (jaExiste)
    {
        return false;
    }
    else
    {
        if (atual->folha)
        {
            if (atual->n_chaves < ordem - 1)
            {
                atual->chaves[(atual->n_chaves)] = chave;
                atual->n_chaves++;
                atual->ptr_node[(atual->n_chaves) + 1] = NULL;
                *promocao = false;
                insertionSort(atual->chaves, atual->n_chaves);
            }
            else
            {
                divide(atual, chave, NULL, *novo, chave_promovida, ordem);
                *promocao = true;
            }
            return true;
        }
        else
        {
            int pos = posDescida(atual->chaves, atual->n_chaves, chave);
            bool status = buscaInsere(atual->ptr_node[pos], chave, promocao, chave_promovida, novo, ordem);
            if (status == true && *promocao == true)
            {
                if (atual->n_chaves < ordem - 1)
                {
                    pos = ordenaNode(atual, *chave_promovida);
                    atual->ptr_node[pos + 1] = *novo;
                    *promocao = false;
                }
                else
                {
                    Node *filho = criaNode(filho, ordem);
                    divide(atual, *chave_promovida, *novo, filho, chave_promovida, ordem);
                    *novo = filho;
                }
            }
            return status;
        }
    }
}

bool insere(Node **raiz, Chave chave, int ordem)
{
    if (*raiz == NULL)
    {
        *raiz = criaNode(*raiz, ordem);
        (*raiz)->chaves[0] = chave;
        (*raiz)->n_chaves++;
        return true;
    }
    else
    {
        bool status, promocao;
        Chave chave_promovida;
        Node *novo = criaNode(novo, ordem);
        status = buscaInsere(*raiz, chave, &promocao, &chave_promovida, &novo, ordem);
        if (promocao == true)
        {
            Node *nova_raiz = criaNode(nova_raiz, ordem);
            nova_raiz->folha = false;
            nova_raiz->chaves[0] = chave_promovida;
            nova_raiz->n_chaves++;
            nova_raiz->ptr_node[0] = *raiz;
            nova_raiz->ptr_node[1] = novo;
            *raiz = nova_raiz;
        }
        return status;
    }
}

int buscar(Node **a, char chave[33], int ordem, FILE **output)
{
    int i = 0;
    while ((i < (*a)->n_chaves) && (strcmp(chave, (*a)->chaves[i].chave) > 0))
    {
        i = i + 1;
    }
    if (strcmp(chave, (*a)->chaves[i].chave) == 0)
    {
        for (int x = 0; x < ordem - 1; x++)
        {
            fprintf(*output, "%s:size=%i,hash=%s\n", (*a)->chaves[x].nome, (*a)->chaves[x].qntd, (*a)->chaves[x].chave);
        }
        return i;
    }
    else if ((*a)->folha)
    {
        fprintf(*output, "-\n");
        return 0;
    }
    else
    {
        if ((*a)->folha == false)
        {
            return buscar((&(*a)->ptr_node[i]), chave, ordem, output);
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    printf("1\n");
    int ordem, range, tmnh;
    printf("iniciado ordem, range e tmnh\n");
    char name[30], cdgchave[33], funcao[7];
    printf("iniciado name, cdgchave, funcao\n");

    FILE *input = fopen(argv[1], "r");
    printf("abriu o file input\n");
    FILE *output = fopen(argv[2], "w");
    printf("abriu o file output\n");

    fscanf(input, "%i", &ordem);
    printf("ordem = %i\n", ordem);
    Node *arvoreb = NULL;
    printf("criei a arvoreb\n");
    fscanf(input, "%i", &range);
    printf("range = %i\n", range);

    for (int i = 0; i < range; i++)
    {
        printf("primeiro for\n");
        fscanf(input, "%s %i %s", name, &tmnh, cdgchave);
        printf("name = %s, tmnh = %i, cdgchave = %s\n", name, tmnh, cdgchave);
        Chave new;
        printf("criou nova chave\n");
        new.qntd = tmnh;
        printf("new.qntd = %i\n", new.qntd);
        strcpy(new.chave, cdgchave);
        printf("new.chave = %s\n", new.chave);
        strcpy(new.nome, name);
        printf("new.nome = %s\n", new.nome);
        insere(&arvoreb, new, ordem);
        printf("inseriu\n");
    }
    printf("saiu do for\n");

    fscanf(input, "%i", &range);
    printf("novo range = %i\n", range);

    for (int x = 0; x < range; x++)
    {
        printf("segundo for\n");
        fscanf(input, "%s", funcao);
        printf("funcao = %s\n", funcao);
        if (strcmp(funcao, "INSERT") == 0)
        {
            printf("se a funcao eh insert\n");
            fscanf(input, "%s %i %s", name, &tmnh, cdgchave);
            printf("name = %s, tmnh = %i, cdgchave = %s\n", name, tmnh, cdgchave);
            Chave new;
            printf("criou nova chave\n");
            new.qntd = tmnh;
            printf("new.qntd = %i\n", new.qntd);
            strcpy(new.chave, cdgchave);
            printf("new.chave = %s\n", new.chave);
            strcpy(new.nome, name);
            printf("new.nome = %s\n", new.nome);
            insere(&arvoreb, new, ordem);
            printf("inseriu\n");
        }
        else if (strcmp(funcao, "SELECT") == 0)
        {
            printf("funcao eh select\n");
            fscanf(input, "%s", cdgchave);
            printf("chave = %s\n", cdgchave);
            fprintf(output, "[%s]\n", cdgchave);
            printf("escreveu\n");
            buscar(&arvoreb, cdgchave, ordem, &output);
            printf("buscou\n");
        }
    }
    printf("2\n");
    fclose(input);
    printf("3\n");
    fclose(output);
    printf("4\n");
    
    return 0;
}