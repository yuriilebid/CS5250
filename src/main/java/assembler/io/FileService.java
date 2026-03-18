package assembler.io;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.List;

public final class FileService {

    private FileService() {
    }

    public static List<String> readAllLines(Path p) throws IOException {
        return Files.readAllLines(p, StandardCharsets.UTF_8);
    }

    public static void writeAllLines(Path p, List<String> xs) throws IOException {
        Files.write(p, xs, StandardCharsets.UTF_8);
    }

    public static Path defaultOutputPath(Path in) {
        String n = in.getFileName().toString();
        int d = n.lastIndexOf('.');
        String b = (d == -1) ? n : n.substring(0, d);
        String out = b + ".hack";
        return in.resolveSibling(out);
    }
}

