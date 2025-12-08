// Realizaremos uma biblioteca para facilitar nossas partes individuais.
#ifndef HOTEL_H
#define HOTEL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STR_MAX 100

// Structs
typedef struct {
    int dia, mes, ano;
} Data;

typedef struct {
    int codigo;
    char nome[STR_MAX];
    char endereco[STR_MAX];
    char telefone[30];
} Cliente;

typedef struct {
    int codigo;
    char nome[STR_MAX];
    char telefone[30];
    char cargo[40];
    double salario;
} Funcionario;

typedef enum { DESOCUPADO=0, OCUPADO=1 } StatusQuarto;

typedef struct {
    int numero;
    int capacidade;
    double valor_diaria;
    StatusQuarto status;
} Quarto;

typedef struct {
    int codigo_estadia;
    Data data_entrada;
    Data data_saida;
    int qtd_diarias;
    int codigo_cliente;
    int numero_quarto;
} Estadia;

// Arquivos
#define ARQ_CLIENTES "clientes.dat"
#define ARQ_FUNC "funcionarios.dat"
#define ARQ_QUARTOS "quartos.dat"
#define ARQ_ESTADIAS "estadias.dat"

// Funções para as datas
long data_para_jdn(Data d);
int diff_diarias(Data entrada, Data saida);

// Funções para os utilitários
char *strcasestr_local(const char *h, const char *n);
void limpar_buffer();

// Funções dos clientes
int cadastrar_cliente(Cliente *novo);
Cliente* buscar_cliente_por_codigo(int codigo);
Cliente* buscar_cliente_por_nome(const char *nome);
void listar_clientes();

// Funções dos funcionários
int cadastrar_funcionario(Funcionario *novo);
Funcionario* buscar_funcionario_por_codigo(int codigo);
Funcionario* buscar_funcionario_por_nome(const char *nome);
void listar_funcionarios();

// Funções dos quartos
int cadastrar_quarto(Quarto *qnovo);
Quarto* buscar_quarto_por_numero(int numero);
int atualizar_status_quarto(int numero, StatusQuarto status);
void listar_quartos();

// Funções da estadia
int cadastrar_estadia_by_input(Data entrada, Data saida, int cod_cli, int qtd);
int dar_baixa_estadia(int cod, double *valor, int *pontos);
void listar_estadias_por_cliente_codigo(int cod);
int calcular_pontos_fidelidade(int cod);

// Funções do menu
void imprimir_menu();
void menu_cliente();
void menu_funcionario();
void menu_quarto();
void menu_estadia();

#endif
