package repository;

import java.util.ArrayList;
import java.util.List;

public class Repositorio<T> {

    private List<T> lista = new ArrayList<>();

    public void adicionar(T objeto) {
        lista.add(objeto);
    }

    public void remover(T objeto) {
        lista.remove(objeto);
    }

    public List<T> listar() {
        return lista;
    }
}