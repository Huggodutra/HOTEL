#include "hotel.h"

char *strcasestr_local(const char *h, const char *n) {
    if (!*n) return (char*)h;
    for (; *h; h++) {
        const char *hh = h, *nn = n;
        while (*hh && *nn && tolower((unsigned char)*hh) == tolower((unsigned char)*nn)) { hh++; nn++; }
        if (!*nn) return (char*)h;
    }
    return NULL;
}

// Data
long data_para_jdn(Data d) {
    int a = (14 - d.mes) / 12;
    int y = d.ano + 4800 - a;
    int m = d.mes + 12*a - 3;
    return d.dia + (153*m + 2)/5 + 365L*y + y/4 - y/100 + y/400 - 32045;
}

int diff_diarias(Data entrada, Data saida) {
    long j1 = data_para_jdn(entrada);
    long j2 = data_para_jdn(saida);
    return (int)(j2 - j1);
}

// Parte dos clientes
static int proximo_codigo_cliente() {
    FILE *f = fopen(ARQ_CLIENTES, "rb");
    int max = 0;
    if (!f) return 1;
    Cliente c;
    while (fread(&c, sizeof(Cliente), 1, f)) if (c.codigo > max) max = c.codigo;
    fclose(f);
    return max + 1;
}

int cadastrar_cliente(Cliente *novo) {
    FILE *f = fopen(ARQ_CLIENTES, "ab");
    if (!f) return 0;
    if (novo->codigo == 0) novo->codigo = proximo_codigo_cliente();
    fwrite(novo, sizeof(Cliente), 1, f);
    fclose(f);
    return 1;
}

Cliente* buscar_cliente_por_codigo(int codigo) {
    FILE *f = fopen(ARQ_CLIENTES, "rb");
    if (!f) return NULL;
    Cliente *c = malloc(sizeof(Cliente));
    while (fread(c, sizeof(Cliente), 1, f)) {
        if (c->codigo == codigo) { fclose(f); return c; }
    }
    fclose(f); free(c); return NULL;
}

Cliente* buscar_cliente_por_nome(const char *nome) {
    FILE *f = fopen(ARQ_CLIENTES, "rb");
    if (!f) return NULL;
    Cliente *c = malloc(sizeof(Cliente));
    while (fread(c, sizeof(Cliente), 1, f)) {
        if (strcasestr_local(c->nome, nome)) { fclose(f); return c; }
    }
    fclose(f); free(c); return NULL;
}

void listar_clientes() {
    FILE *f = fopen(ARQ_CLIENTES, "rb");
    if (!f) { printf("Nenhum cliente cadastrado.\n"); return; }
    Cliente c;
    printf("\n--- LISTA DE CLIENTES ---\n");
    while (fread(&c, sizeof(Cliente), 1, f)) {
        printf("Codigo: %d - Nome: %s - Tel: %s\n", c.codigo, c.nome, c.telefone);
    }
    printf("-------------------------\n");
    fclose(f);
}

// Parte dos funcion�rios
static int proximo_codigo_func() {
    FILE *f = fopen(ARQ_FUNC, "rb");
    int max = 0;
    if (!f) return 1;
    Funcionario p;
    while (fread(&p, sizeof(Funcionario), 1, f)) if (p.codigo > max) max = p.codigo;
    fclose(f);
    return max + 1;
}

int cadastrar_funcionario(Funcionario *novo) {
    FILE *f = fopen(ARQ_FUNC, "ab");
    if (!f) return 0;
    if (novo->codigo == 0) novo->codigo = proximo_codigo_func();
    fwrite(novo, sizeof(Funcionario), 1, f);
    fclose(f);
    return 1;
}

Funcionario* buscar_funcionario_por_codigo(int codigo) {
    FILE *f = fopen(ARQ_FUNC, "rb");
    if (!f) return NULL;
    Funcionario *p = malloc(sizeof(Funcionario));
    while (fread(p, sizeof(Funcionario), 1, f)) {
        if (p->codigo == codigo) { fclose(f); return p; }
    }
    fclose(f); free(p); return NULL;
}

Funcionario* buscar_funcionario_por_nome(const char *nome) {
    FILE *f = fopen(ARQ_FUNC, "rb");
    if (!f) return NULL;
    Funcionario *p = malloc(sizeof(Funcionario));
    while (fread(p, sizeof(Funcionario), 1, f)) {
        if (strcasestr_local(p->nome, nome)) { fclose(f); return p; }
    }
    fclose(f); free(p); return NULL;
}

void listar_funcionarios() {
    FILE *f = fopen(ARQ_FUNC, "rb");
    if (!f) { printf("Nenhum funcionario cadastrado.\n"); return; }
    Funcionario p;
    printf("\n--- LISTA DE FUNCIONARIOS ---\n");
    while (fread(&p, sizeof(Funcionario), 1, f)) {
        printf("Codigo: %d - Nome: %s - Cargo: %s - Salario: R$ %.2f\n", p.codigo, p.nome, p.cargo, p.salario);
    }
    printf("-----------------------------\n");
    fclose(f);
}
