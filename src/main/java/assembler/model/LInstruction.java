package assembler.model;

public class LInstruction implements Instruction {
    private final String symbol;

    public LInstruction(String symbol) {
        this.symbol = symbol;
    }

    public String getSymbol() {
        return symbol;
    }
}

