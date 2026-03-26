#pragma once

#include "VMConstants.hpp"

#include <optional>
#include <string>

namespace vm {

class Parser {
public:
  explicit Parser(const std::string& vmFilePath);

  bool hasMoreCommands() const;
  void advance(); // moves to next command (skips blanks/comments)

  CommandType commandType() const { return cmdType_; }
  const std::string& arg1() const { return arg1_; } // for Arithmetic returns the command itself
  int arg2() const { return arg2_; }                // only for push/pop/function/call

private:
  std::optional<std::string> nextCommandLine() const;
  static std::string stripComments(const std::string& line);
  static std::string trim(const std::string& s);
  static bool startsWith(const std::string& s, const std::string& prefix);

  void parseCurrentLine(const std::string& line);

  std::string filePath_;
  std::string fileText_;
  size_t cursor_ = 0;

  // Current command info
  CommandType cmdType_ = CommandType::Error;
  std::string arg1_;
  int arg2_ = 0;
};

} // namespace vm

