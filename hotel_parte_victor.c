#include "hotel.h"

void limpar_buffer() { int c; while ((c=getchar())!='\n' && c!=EOF); }

// Parte dos quartos
int cadastrar_quarto(Quarto *qnovo) {
    FILE *f = fopen(ARQ_QUARTOS, "ab+");
    if (!f) return 0;

    Quarto q;
    rewind(f);
    while (fread(&q, sizeof(Quarto), 1, f)) {
        if (q.numero == qnovo->numero) { fclose(f); return 0; }
    }

    fwrite(qnovo, sizeof(Quarto), 1, f);
    fclose(f);
    return 1;
}

Quarto* buscar_quarto_por_numero(int numero) {
    FILE *f = fopen(ARQ_QUARTOS, "rb");
    if (!f) return NULL;

    Quarto *q = malloc(sizeof(Quarto));
    while (fread(q, sizeof(Quarto), 1, f)) {
        if (q->numero == numero) { fclose(f); return q; }
    }

    fclose(f);
    free(q);
    return NULL;
}

int atualizar_status_quarto(int numero, StatusQuarto status) {
    FILE *f = fopen(ARQ_QUARTOS, "r+b");
    if (!f) return 0;

    Quarto q;
    while (fread(&q, sizeof(Quarto), 1, f)) {
        if (q.numero == numero) {
            q.status = status;
            fseek(f, -sizeof(Quarto), SEEK_CUR);
            fwrite(&q, sizeof(Quarto), 1, f);
            fclose(f);
            return 1;
        }
    }

    fclose(f);
    return 0;
}

void listar_quartos() {
    FILE *f = fopen(ARQ_QUARTOS, "rb");
    if (!f) { printf("Nenhum quarto cadastrado.\n"); return; }

    Quarto q;
    printf("\n--- LISTA DE QUARTOS ---\n");
    while (fread(&q, sizeof(Quarto), 1, f)) {
        printf("Quarto: %d - Capacidade: %d - Diaria: R$ %.2f - Status: %s\n",
               q.numero, q.capacidade, q.valor_diaria, q.status == OCUPADO ? "OCUPADO" : "DESOCUPADO");
    }
    printf("------------------------\n");
    fclose(f);
}

//Parte das estadias

static int proximo_codigo_estadia() {
    FILE *f = fopen(ARQ_ESTADIAS, "rb");
    int max = 0;
    Estadia e;

    if (!f) return 1;

    while (fread(&e, sizeof(Estadia), 1, f))
        if (e.codigo_estadia > max) max = e.codigo_estadia;

    fclose(f);
    return max + 1;
}

static int periodo_intersecta(Data in1, Data out1, Data in2, Data out2) {
    long a1 = data_para_jdn(in1);
    long b1 = data_para_jdn(out1);
    long a2 = data_para_jdn(in2);
    long b2 = data_para_jdn(out2);
    return (a1 < b2 && a2 < b1);
}

static int quarto_disponivel_para_periodo(int numero, Data e, Data s) {
    FILE *f = fopen(ARQ_ESTADIAS, "rb");
    if (!f) return 1;

	//Verificação para a estadia não haver conflitos.
    Estadia est;
    while (fread(&est, sizeof(Estadia), 1, f)) {
        if (est.codigo_estadia != -1 && est.numero_quarto == numero) {
            if (periodo_intersecta(est.data_entrada, est.data_saida, e, s)) {
                fclose(f);
                return 0; 
            }
        }
    }

    fclose(f);
    return 1;
}

int cadastrar_estadia_by_input(Data entrada, Data saida, int cod_cli, int qtd) {

    FILE *fq = fopen(ARQ_QUARTOS, "r+b");
    if (!fq) return 0;

    Quarto q;
    long pos = -1;
    int achou = 0;

    while (fread(&q, sizeof(Quarto), 1, fq)) {
        if (q.capacidade >= qtd && q.status == DESOCUPADO) {
            if (quarto_disponivel_para_periodo(q.numero, entrada, saida)) {
                pos = ftell(fq) - sizeof(Quarto);
                achou = 1;
                break;
            }
        }
    }

    if (!achou) { fclose(fq); return 0; }

    Estadia e;
    e.codigo_estadia = proximo_codigo_estadia();
    e.data_entrada = entrada;
    e.data_saida = saida;
    e.qtd_diarias = diff_diarias(entrada, saida);
    e.codigo_cliente = cod_cli;
    e.numero_quarto = q.numero;

    FILE *fe = fopen(ARQ_ESTADIAS, "ab");
    if (!fe) { fclose(fq); return 0; }
    fwrite(&e, sizeof(Estadia), 1, fe);
    fclose(fe);

    q.status = OCUPADO;
    fseek(fq, pos, SEEK_SET);
    fwrite(&q, sizeof(Quarto), 1, fq);
    fclose(fq);

    return 1;
}

int dar_baixa_estadia(int cod, double *valor, int *pontos) {

    FILE *fe = fopen(ARQ_ESTADIAS, "r+b");
    if (!fe) return 0;

    Estadia e;
    long pos = -1;
    int achou = 0;

    while (fread(&e, sizeof(Estadia), 1, fe)) {
        if (e.codigo_estadia == cod && e.codigo_estadia != -1) {
            pos = ftell(fe) - sizeof(Estadia);
            achou = 1;
            break;
        }
    }

    if (!achou) { fclose(fe); return 0; }

    FILE *fq = fopen(ARQ_QUARTOS, "r+b");
    if (!fq) { fclose(fe); return 0; }

    Quarto q;
    long posq = -1;
    while (fread(&q, sizeof(Quarto), 1, fq)) {
        if (q.numero == e.numero_quarto) {
            posq = ftell(fq) - sizeof(Quarto);
            break;
        }
    }

    if (posq == -1) { fclose(fe); fclose(fq); return 0; } 

    *valor = q.valor_diaria * e.qtd_diarias;
    *pontos = e.qtd_diarias * 10;

    e.codigo_estadia = -1;
    fseek(fe, pos, SEEK_SET);
    fwrite(&e, sizeof(Estadia), 1, fe);
    fclose(fe);

    q.status = DESOCUPADO;
    fseek(fq, posq, SEEK_SET);
    fwrite(&q, sizeof(Quarto), 1, fq);
    fclose(fq);

    return 1;
}

void listar_estadias_por_cliente_codigo(int cod) {
    FILE *fe = fopen(ARQ_ESTADIAS, "rb");
    if (!fe) { printf("Nenhuma estadia cadastrada.\n"); return; }

    Estadia e;
    printf("\n--- ESTADIAS DO CLIENTE %d ---\n", cod);
    while (fread(&e, sizeof(Estadia), 1, fe)) {
        if (e.codigo_cliente == cod && e.codigo_estadia != -1) {
            printf("Estadia %d - Quarto %d - Entrada: %d/%d/%d - Saida: %d/%d/%d - Diarias: %d\n",
                   e.codigo_estadia, e.numero_quarto, e.data_entrada.dia, e.data_entrada.mes, e.data_entrada.ano,
                   e.data_saida.dia, e.data_saida.mes, e.data_saida.ano, e.qtd_diarias);
        }
    }
    printf("------------------------------\n");
    fclose(fe);
}

int calcular_pontos_fidelidade(int cod) {
    FILE *fe = fopen(ARQ_ESTADIAS, "rb");
    if (!fe) return 0;

    Estadia e;
    int total = 0;

    while (fread(&e, sizeof(Estadia), 1, fe))
        if (e.codigo_cliente == cod && e.codigo_estadia != -1)
            total += e.qtd_diarias * 10;

    fclose(fe);
    return total;
}

// Menu

void menu_cliente() {
    int op, cod;
    char nome[STR_MAX];
    Cliente c, *res;

    do {
        printf("\n--- MENU CLIENTE ---\n");
        printf("1-Cadastrar 2-Buscar por Codigo 3-Buscar por Nome 4-Listar Todos 0-Voltar\nOpcao: ");
        scanf("%d", &op);
        limpar_buffer();

        switch (op) {
            case 1:
                printf("Nome: "); fgets(c.nome, STR_MAX, stdin); c.nome[strcspn(c.nome, "\n")] = 0;
                printf("Endereco: "); fgets(c.endereco, STR_MAX, stdin); c.endereco[strcspn(c.endereco, "\n")] = 0;
                printf("Telefone: "); fgets(c.telefone, 30, stdin); c.telefone[strcspn(c.telefone, "\n")] = 0;
                c.codigo = 0;
                if (cadastrar_cliente(&c)) printf("Cliente cadastrado com sucesso! Codigo: %d\n", c.codigo);
                else printf("Erro ao cadastrar cliente.\n");
                break;
            case 2:
                printf("Codigo do Cliente: "); scanf("%d", &cod); limpar_buffer();
                res = buscar_cliente_por_codigo(cod);
                if (res) {
                    printf("Codigo: %d - Nome: %s - Endereco: %s - Tel: %s\n", res->codigo, res->nome, res->endereco, res->telefone);
                    free(res);
                } else printf("Cliente nao encontrado.\n");
                break;
            case 3:
                printf("Nome ou parte do Nome: "); fgets(nome, STR_MAX, stdin); nome[strcspn(nome, "\n")] = 0;
                res = buscar_cliente_por_nome(nome);
                if (res) {
                    printf("Codigo: %d - Nome: %s - Endereco: %s - Tel: %s\n", res->codigo, res->nome, res->endereco, res->telefone);
                    free(res);
                } else printf("Cliente nao encontrado.\n");
                break;
            case 4:
                listar_clientes();
                break;
            case 0:
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (op != 0);
}

void menu_funcionario() {
    int op, cod;
    char nome[STR_MAX];
    Funcionario f, *res;

    do {
        printf("\n--- MENU FUNCIONARIOS ---\n");
        printf("1-Cadastrar 2-Buscar por Codigo 3-Buscar por Nome 4-Listar Todos 0-Voltar\nOpcao: ");
        scanf("%d", &op);
        limpar_buffer();

        switch (op) {
            case 1:
                printf("Nome: "); fgets(f.nome, STR_MAX, stdin); f.nome[strcspn(f.nome, "\n")] = 0;
                printf("Telefone: "); fgets(f.telefone, 30, stdin); f.telefone[strcspn(f.telefone, "\n")] = 0;
                printf("Cargo: "); fgets(f.cargo, 40, stdin); f.cargo[strcspn(f.cargo, "\n")] = 0;
                printf("Salario: "); scanf("%lf", &f.salario); limpar_buffer();
                f.codigo = 0;
                if (cadastrar_funcionario(&f)) printf("Funcionario cadastrado com sucesso! Codigo: %d\n", f.codigo);
                else printf("Erro ao cadastrar funcionario.\n");
                break;
            case 2:
                printf("Codigo do Funcionario: "); scanf("%d", &cod); limpar_buffer();
                res = buscar_funcionario_por_codigo(cod);
                if (res) {
                    printf("Codigo: %d - Nome: %s - Tel: %s - Cargo: %s - Salario: R$ %.2f\n", res->codigo, res->nome, res->telefone, res->cargo, res->salario);
                    free(res);
                } else printf("Funcionario nao encontrado.\n");
                break;
            case 3:
                printf("Nome ou parte do Nome: "); fgets(nome, STR_MAX, stdin); nome[strcspn(nome, "\n")] = 0;
                res = buscar_funcionario_por_nome(nome);
                if (res) {
                    printf("Codigo: %d - Nome: %s - Tel: %s - Cargo: %s - Salario: R$ %.2f\n", res->codigo, res->nome, res->telefone, res->cargo, res->salario);
                    free(res);
                } else printf("Funcionario nao encontrado.\n");
                break;
            case 4:
                listar_funcionarios();
                break;
            case 0:
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (op != 0);
}

void menu_quarto() {
    int op, num;
    Quarto q, *res;

    do {
        printf("\n--- MENU QUARTO ---\n");
        printf("1-Cadastrar 2-Buscar por Numero 3-Listar Todos 0-Voltar\nOpcao: ");
        scanf("%d", &op);
        limpar_buffer();

        switch (op) {
            case 1:
                printf("Numero do Quarto: "); scanf("%d", &q.numero);
                printf("Capacidade (Hospedes): "); scanf("%d", &q.capacidade);
                printf("Valor da Diaria: "); scanf("%lf", &q.valor_diaria);
                limpar_buffer();
                q.status = DESOCUPADO;
                if (cadastrar_quarto(&q)) printf("Quarto %d cadastrado com sucesso!\n", q.numero);
                else printf("Erro ao cadastrar quarto. Numero ja¡ existe.\n");
                break;
            case 2:
                printf("Numero do Quarto: "); scanf("%d", &num); limpar_buffer();
                res = buscar_quarto_por_numero(num);
                if (res) {
                    printf("Quarto: %d - Capacidade: %d - Diaria: R$ %.2f - Status: %s\n",
                           res->numero, res->capacidade, res->valor_diaria, res->status == OCUPADO ? "OCUPADO" : "DESOCUPADO");
                    free(res);
                } else printf("Quarto nao encontrado.\n");
                break;
            case 3:
                listar_quartos();
                break;
            case 0:
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (op != 0);
}

void menu_estadia() {
    int op, cod_cli, qtd_hospedes, cod_estadia, pontos;
    Data entrada, saida;
    double valor;
    Cliente *c;

    do {
        printf("\n--- MENU ESTADIA ---\n");
        printf("1-Cadastrar 2-Dar Baixa 3-Listar por Cliente 4-Calcular Pontos 0-Voltar\nOpcao: ");
        scanf("%d", &op);
        limpar_buffer();

        switch (op) {
            case 1:
                printf("Codigo do Cliente: "); scanf("%d", &cod_cli); limpar_buffer();
                c = buscar_cliente_por_codigo(cod_cli);
                if (!c) { printf("Cliente nao encontrado. Cadastre o cliente primeiro.\n"); break; }
                free(c);

                printf("Quantidade de Hospedes: "); scanf("%d", &qtd_hospedes);
                printf("Data de Entrada (dd mm aaaa): "); scanf("%d %d %d", &entrada.dia, &entrada.mes, &entrada.ano);
                printf("Data de Saida (dd mm aaaa): "); scanf("%d %d %d", &saida.dia, &saida.mes, &saida.ano);
                limpar_buffer();

                if (diff_diarias(entrada, saida) <= 0) {
                    printf("Data de saida deve ser posterior a data de entrada.\n");
                    break;
                }

                if (cadastrar_estadia_by_input(entrada, saida, cod_cli, qtd_hospedes)) {
                    printf("Estadia cadastrada com sucesso!\n");
                } else {
                    printf("Erro ao cadastrar estadia. Verifique se ha¡ quartos disponiveis com a capacidade e periodo desejados.\n");
                }
                break;
            case 2:
                printf("Codigo da Estadia para Baixa: "); scanf("%d", &cod_estadia); limpar_buffer();
                if (dar_baixa_estadia(cod_estadia, &valor, &pontos)) {
                    printf("Baixa da estadia %d realizada com sucesso!\n", cod_estadia);
                    printf("Valor total a pagar: R$ %.2f\n", valor);
                    printf("Pontos de fidelidade ganhos: %d\n", pontos);
                } else {
                    printf("Erro ao dar baixa. Estadia nao encontrada ou ja¡ finalizada.\n");
                }
                break;
            case 3:
                printf("Codigo do Cliente: "); scanf("%d", &cod_cli); limpar_buffer();
                listar_estadias_por_cliente_codigo(cod_cli);
                break;
            case 4:
                printf("Codigo do Cliente: "); scanf("%d", &cod_cli); limpar_buffer();
                pontos = calcular_pontos_fidelidade(cod_cli);
                printf("O cliente %d possui %d pontos de fidelidade.\n", cod_cli, pontos);
                break;
            case 0:
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (op != 0);
}

// MAIN!
int main() {
    int op;
    do {
        printf("\n--- HOTEL DESCANSO GARANTIDO ---\n");
        printf("1-Cliente 2-Funcionario 3-Quarto 4-Estadia 0-Sair\nOpcao: ");
        scanf("%d", &op);
        limpar_buffer();

        switch (op) {
            case 1: menu_cliente(); break;
            case 2: menu_funcionario(); break;
            case 3: menu_quarto(); break;
            case 4: menu_estadia(); break;
            case 0: printf("Saindo do sistema.\n"); break;
            default: printf("Opcao invalida.\n");
        }
    } while (op != 0);
    return 0;
}
