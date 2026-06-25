import java.time.LocalDateTime;
import java.util.Scanner;

import exception.ConsultaNaoEncontradaException;
import exception.HorarioIndisponivelException;
import model.Consulta;
import model.Especialidade;
import service.ConsultaService;

public class Main {

    public static void main(String[] args) {

        Scanner sc = new Scanner(System.in);
        ConsultaService service = new ConsultaService();

        int opcao;

        do {

            System.out.println("\n===== CLÍNICA MÉDICA =====");
            System.out.println("1 - Cadastrar Consulta");
            System.out.println("2 - Listar Consultas");
            System.out.println("3 - Buscar Consulta");
            System.out.println("4 - Remover Consulta");
            System.out.println("0 - Sair");
            System.out.print("Escolha: ");

            opcao = sc.nextInt();
            sc.nextLine();

            try {

                switch (opcao) {

                    case 1:

                        System.out.print("Código: ");
                        int codigo = sc.nextInt();
                        sc.nextLine();

                        System.out.print("Nome do paciente: ");
                        String nome = sc.nextLine();

                        System.out.println("Especialidade:");
                        System.out.println("1 - CARDIOLOGIA");
                        System.out.println("2 - PEDIATRIA");
                        System.out.println("3 - ORTOPEDIA");
                        System.out.println("4 - DERMATOLOGIA");

                        int esp = sc.nextInt();

                        Especialidade especialidade;

                        switch (esp) {
                            case 1:
                                especialidade = Especialidade.CARDIOLOGIA;
                                break;
                            case 2:
                                especialidade = Especialidade.PEDIATRIA;
                                break;
                            case 3:
                                especialidade = Especialidade.ORTOPEDIA;
                                break;
                            default:
                                especialidade = Especialidade.DERMATOLOGIA;
                        }

                        System.out.print("Valor da consulta: ");
                        double valor = sc.nextDouble();

                        LocalDateTime data =
                                LocalDateTime.now().plusDays(1);

                        Consulta consulta = new Consulta(
                                codigo,
                                nome,
                                especialidade,
                                data,
                                valor
                        );

                        service.cadastrar(consulta);

                        System.out.println("Consulta cadastrada!");
                        break;

                    case 2:

                        System.out.println("\n=== CONSULTAS ===");

                        service.listar()
                                .forEach(System.out::println);

                        break;

                    case 3:

                        System.out.print("Código da consulta: ");
                        int codigoBusca = sc.nextInt();

                        Consulta encontrada =
                                service.buscarPorCodigo(codigoBusca);

                        System.out.println(encontrada);

                        break;

                    case 4:

                        System.out.print("Código da consulta: ");
                        int codigoRemover = sc.nextInt();

                        service.removerPorCodigo(codigoRemover);

                        System.out.println("Consulta removida!");

                        break;

                    case 0:

                        System.out.println("Encerrando sistema...");
                        break;

                    default:

                        System.out.println("Opção inválida.");
                }

            } catch (ConsultaNaoEncontradaException e) {

                System.out.println(e.getMessage());

            } catch (HorarioIndisponivelException e) {

                System.out.println(e.getMessage());

            }

        } while (opcao != 0);

        sc.close();
    }
}