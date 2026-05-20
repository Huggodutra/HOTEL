// =========================
// IMPORTS
// =========================
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Scanner;

// =========================
// ENUM
// =========================
public enum StatusPedido {
    PENDENTE,
    PROCESSANDO,
    ENVIADO,
    ENTREGUE,
    CANCELADO
}

// =========================
// INTERFACE
// =========================
public interface Processavel {
    void processarPedido() throws PedidoInvalidoException;
    double calcularValorFinal();
}

public interface Entregavel {
    double calcularFrete();
}

// =========================
// EXCEÇÃO PERSONALIZADA
// =========================
public class PedidoInvalidoException extends Exception {
    public PedidoInvalidoException(String mensagem) {
        super(mensagem);
    }
}

public class ValorInvalidoException extends Exception {
    public ValorInvalidoException(String mensagem) {
        super(mensagem);
    }
}

// =========================
// CLASSE ABSTRATA
// =========================
public abstract class Pedido implements Processavel {
    protected int id;
    protected String nomeCliente;
    protected double valor;

    public Pedido(int id, String nomeCliente, double valor)
            throws ValorInvalidoException {

        if (valor <= 0) {
            throw new ValorInvalidoException("Valor inválido.");
        }

        this.id = id;
        this.nomeCliente = nomeCliente;
        this.valor = valor;
    }
}

// =========================
// HERANÇA
// =========================
public class PedidoEletronico extends Pedido implements Entregavel {

    public PedidoEletronico(int id, String nomeCliente, double valor)
            throws ValorInvalidoException {
        super(id, nomeCliente, valor);
    }

    @Override
    public void processarPedido() throws PedidoInvalidoException {
        System.out.println("Pedido processado.");
    }

    @Override
    public double calcularValorFinal() {
        return valor - 10;
    }

    @Override
    public double calcularFrete() {
        return 20;
    }
}

// =========================
// ARRAYLIST
// =========================
ArrayList<Pedido> pedidos = new ArrayList<>();

// =========================
// ADICIONAR
// =========================
pedidos.add(pedido);

// =========================
// FOR-EACH
// =========================
for (Pedido p : pedidos) {
    System.out.println(p);
}

// =========================
// ITERATOR
// =========================
Iterator<Pedido> it = pedidos.iterator();

while (it.hasNext()) {
    Pedido p = it.next();
    System.out.println(p);
}

// =========================
// ITERATOR REMOVENDO
// =========================
Iterator<Pedido> it2 = pedidos.iterator();

while (it2.hasNext()) {
    Pedido p = it2.next();

    if (p.getStatus() == StatusPedido.CANCELADO) {
        it2.remove();
    }
}

// =========================
// TRY CATCH
// =========================
try {
    // código
} catch (Exception e) {
    System.out.println(e.getMessage());
}

// =========================
// SCANNER
// =========================
Scanner sc = new Scanner(System.in);

int numero = sc.nextInt();
double valor = sc.nextDouble();
sc.nextLine();
String texto = sc.nextLine();

// =========================
// SWITCH
// =========================
switch (opcao) {
    case 1:
        break;
    case 2:
        break;
    default:
        System.out.println("Opção inválida.");
}

// =========================
// MENU
// =========================
int opcao;

do {
    System.out.println("1 - Cadastrar");
    System.out.println("2 - Listar");
    System.out.println("0 - Sair");

    opcao = sc.nextInt();

    switch (opcao) {
        case 1:
            break;
        case 2:
            break;
    }

} while (opcao != 0);

// =========================
// MAIN
// =========================
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        ArrayList<Pedido> pedidos = new ArrayList<>();
    }
}

// =========================
// COMANDOS TERMINAL
// =========================
// javac *.java
// java Main
// git status
// git add .
// git commit -m "Atualização"
// git push
// code .
// codium .