import java.util.List;

import exception.ConsultaNaoEncontradaException;
import exception.HorarioIndisponivelException;
import model.Consulta;
import repository.Repositorio;

public class ConsultaService {

    private Repositorio<Consulta> repositorio =
            new Repositorio<>();

    public void cadastrar(Consulta consulta)
            throws HorarioIndisponivelException {

        for (Consulta c : repositorio.listar()) {

            if (c.getDataConsulta()
                    .equals(consulta.getDataConsulta())) {

                throw new HorarioIndisponivelException(
                        "Horário indisponível.");
            }
        }

        repositorio.adicionar(consulta);
    }

    public List<Consulta> listar() {
        return repositorio.listar();
    }

    public Consulta buscarPorCodigo(int codigo)
            throws ConsultaNaoEncontradaException {

        for (Consulta consulta : repositorio.listar()) {

            if (consulta.getCodigo() == codigo) {
                return consulta;
            }
        }

        throw new ConsultaNaoEncontradaException(
                "Consulta não encontrada.");
    }

    public void removerPorCodigo(int codigo)
            throws ConsultaNaoEncontradaException {

        Consulta consulta = buscarPorCodigo(codigo);

        repositorio.remover(consulta);
    }
}