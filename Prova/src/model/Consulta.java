 sc.package model;

import java.time.LocalDateTime;

public class Consulta {

    private int codigo;
    private String nomePaciente;
    private Especialidade especialidade;
    private LocalDateTime dataConsulta;
    private double valorConsulta;

    public Consulta(int codigo, String nomePaciente,
                    Especialidade especialidade,
                    LocalDateTime dataConsulta,
                    double valorConsulta) {
        this.codigo = codigo;
        this.nomePaciente = nomePaciente;
        this.especialidade = especialidade;
        this.dataConsulta = dataConsulta;
        this.valorConsulta = valorConsulta;
    }

    public int getCodigo() {
        return codigo;
    }

    public String getNomePaciente() {
        return nomePaciente;
    }

    public Especialidade getEspecialidade() {
        return especialidade;
    }

    public LocalDateTime getDataConsulta() {
        return dataConsulta;
    }

    public double getValorConsulta() {
        return valorConsulta;
    }

    public void setDataConsulta(LocalDateTime dataConsulta) {
        this.dataConsulta = dataConsulta;
    }

    @Override
    public String toString() {
        return "Consulta{" +
                "codigo=" + codigo +
                ", paciente='" + nomePaciente + '\'' +
                ", especialidade=" + especialidade +
                ", data=" + dataConsulta +
                ", valor=" + valorConsulta +
                '}';
    }
}
