#include "CodeWriter.hpp"
#include "Parser.hpp"
#include "VMConstants.hpp"

#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

static void usage() {
  std::cerr
      << "Usage:\n"
      << "  VMtranslator [--bootstrap|--no-bootstrap] <file.vm|dir>\n"
      << "\n"
      << "Notes:\n"
      << "  - Project 7 and the first Project 8 tests use manual init scripts, so use --no-bootstrap.\n"
      << "  - The last Project 8 tests (FibonacciElement, StaticsTest) require bootstrap, so use --bootstrap.\n";
}

static std::vector<fs::path> collectVmFiles(const fs::path& fileOrDir) {
  std::vector<fs::path> out;
  if (fs::is_regular_file(fileOrDir) && fileOrDir.extension() == ".vm") {
    out.push_back(fileOrDir);
    return out;
  }
  if (!fs::is_directory(fileOrDir)) return out;

  for (auto& ent : fs::directory_iterator(fileOrDir)) {
    if (ent.is_regular_file() && ent.path().extension() == ".vm") out.push_back(ent.path());
  }
  // Stable ordering helps diffs / reproducibility
  std::sort(out.begin(), out.end());
  return out;
}

static fs::path outAsmPathFor(const fs::path& fileOrDir) {
  if (fs::is_regular_file(fileOrDir) && fileOrDir.extension() == ".vm") {
    fs::path out = fileOrDir;
    out.replace_extension(".asm");
    return out;
  }
  // For a directory: <dir>/<dirName>.asm
  fs::path dirName = fileOrDir.filename();
  return fileOrDir / (dirName.string() + ".asm");
}

int main(int argc, char** argv) {
  try {
    bool bootstrap = false; // default matches Project 7 + early Project 8 tests
    std::string input;

    if (argc == 2) {
      input = argv[1];
    } else if (argc == 3) {
      std::string flag = argv[1];
      input = argv[2];
      if (flag == "--bootstrap") bootstrap = true;
      else if (flag == "--no-bootstrap") bootstrap = false;
      else {
        usage();
        return 2;
      }
    } else {
      usage();
      return 2;
    }

    fs::path inPath(input);
    auto vmFiles = collectVmFiles(inPath);
    if (vmFiles.empty()) {
      std::cerr << "No .vm files found at: " << inPath << "\n";
      return 2;
    }

    fs::path outPath = outAsmPathFor(inPath);
    vm::CodeWriter w(outPath.string());

    if (bootstrap) w.writeInit();

    for (const auto& f : vmFiles) {
      vm::Parser p(f.string());
      w.setFileName(f.filename().string());
      while (p.hasMoreCommands()) {
        p.advance();
        switch (p.commandType()) {
          case vm::CommandType::Arithmetic: w.writeArithmetic(p.arg1()); break;
          case vm::CommandType::Push:
          case vm::CommandType::Pop: w.writePushPop(p.commandType(), p.arg1(), p.arg2()); break;
          case vm::CommandType::Label: w.writeLabel(p.arg1()); break;
          case vm::CommandType::Goto: w.writeGoto(p.arg1()); break;
          case vm::CommandType::If: w.writeIf(p.arg1()); break;
          case vm::CommandType::Function: w.writeFunction(p.arg1(), p.arg2()); break;
          case vm::CommandType::Return: w.writeReturn(); break;
          case vm::CommandType::Call: w.writeCall(p.arg1(), p.arg2()); break;
          default: break;
        }
      }
    }

    w.close();
    std::cout << "Wrote: " << outPath << "\n";
    return 0;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
}

