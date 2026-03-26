#include "Parser.hpp"

#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace vm {

static std::vector<std::string> splitWs(const std::string& s) {
  std::vector<std::string> out;
  std::istringstream iss(s);
  for (std::string tok; iss >> tok;) out.push_back(tok);
  return out;
}

Parser::Parser(const std::string& vmFilePath) : filePath_(vmFilePath) {
  std::ifstream in(vmFilePath);
  if (!in) throw std::runtime_error("Failed to open input file: " + vmFilePath);
  std::ostringstream ss;
  ss << in.rdbuf();
  fileText_ = ss.str();
}

bool Parser::hasMoreCommands() const {
  return nextCommandLine().has_value();
}

void Parser::advance() {
  auto lineOpt = nextCommandLine();
  if (!lineOpt) {
    cmdType_ = CommandType::Error;
    arg1_.clear();
    arg2_ = 0;
    return;
  }

  // move cursor_ past this line
  const auto& line = *lineOpt;
  cursor_ = fileText_.find(line, cursor_);
  if (cursor_ == std::string::npos) {
    // Fallback: move to end
    cursor_ = fileText_.size();
  } else {
    cursor_ += line.size();
    // consume trailing newline(s)
    while (cursor_ < fileText_.size() && (fileText_[cursor_] == '\n' || fileText_[cursor_] == '\r'))
      cursor_++;
  }

  parseCurrentLine(line);
}

std::optional<std::string> Parser::nextCommandLine() const {
  size_t i = cursor_;
  while (i < fileText_.size()) {
    // read one line
    size_t lineEnd = fileText_.find('\n', i);
    std::string line = (lineEnd == std::string::npos) ? fileText_.substr(i) : fileText_.substr(i, lineEnd - i);
    // handle CRLF
    if (!line.empty() && line.back() == '\r') line.pop_back();

    line = trim(stripComments(line));
    if (!line.empty()) return line;

    if (lineEnd == std::string::npos) break;
    i = lineEnd + 1;
  }
  return std::nullopt;
}

std::string Parser::stripComments(const std::string& line) {
  auto pos = line.find("//");
  if (pos == std::string::npos) return line;
  return line.substr(0, pos);
}

std::string Parser::trim(const std::string& s) {
  size_t b = 0;
  while (b < s.size() && std::isspace(static_cast<unsigned char>(s[b]))) b++;
  size_t e = s.size();
  while (e > b && std::isspace(static_cast<unsigned char>(s[e - 1]))) e--;
  return s.substr(b, e - b);
}

bool Parser::startsWith(const std::string& s, const std::string& prefix) {
  return s.rfind(prefix, 0) == 0;
}

void Parser::parseCurrentLine(const std::string& line) {
  cmdType_ = CommandType::Error;
  arg1_.clear();
  arg2_ = 0;

  auto parts = splitWs(line);
  if (parts.empty()) return;

  const std::string& c = parts[0];
  auto isArithmetic = [&](const std::string& x) {
    return x == "add" || x == "sub" || x == "neg" || x == "eq" || x == "gt" || x == "lt" || x == "and" ||
           x == "or" || x == "not";
  };

  if (isArithmetic(c)) {
    cmdType_ = CommandType::Arithmetic;
    arg1_ = c;
    return;
  }

  if (c == "push") {
    cmdType_ = CommandType::Push;
    if (parts.size() >= 3) {
      arg1_ = parts[1];
      arg2_ = std::stoi(parts[2]);
    }
    return;
  }

  if (c == "pop") {
    cmdType_ = CommandType::Pop;
    if (parts.size() >= 3) {
      arg1_ = parts[1];
      arg2_ = std::stoi(parts[2]);
    }
    return;
  }

  if (c == "label") {
    cmdType_ = CommandType::Label;
    if (parts.size() >= 2) arg1_ = parts[1];
    return;
  }
  if (c == "goto") {
    cmdType_ = CommandType::Goto;
    if (parts.size() >= 2) arg1_ = parts[1];
    return;
  }
  if (c == "if-goto") {
    cmdType_ = CommandType::If;
    if (parts.size() >= 2) arg1_ = parts[1];
    return;
  }

  if (c == "function") {
    cmdType_ = CommandType::Function;
    if (parts.size() >= 3) {
      arg1_ = parts[1];
      arg2_ = std::stoi(parts[2]);
    }
    return;
  }
  if (c == "call") {
    cmdType_ = CommandType::Call;
    if (parts.size() >= 3) {
      arg1_ = parts[1];
      arg2_ = std::stoi(parts[2]);
    }
    return;
  }
  if (c == "return") {
    cmdType_ = CommandType::Return;
    return;
  }
}

} // namespace vm

