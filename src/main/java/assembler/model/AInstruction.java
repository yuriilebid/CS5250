package assembler.model;

public class AInstruction implements Instruction {
    private final String symbol;

    public AInstruction(String symbol) {
        this.symbol = symbol;
    }

    public String getSymbol() {
        return symbol;
    }
}

