#pragma once

#include <string>

namespace vm {

// Command types
enum class CommandType {
  Arithmetic,
  Push,
  Pop,
  Label,
  Goto,
  If,
  Function,
  Return,
  Call,
  Error
};

// Segment names (as they appear in .vm source)
inline constexpr const char* S_LOCAL = "local";
inline constexpr const char* S_ARGUMENT = "argument";
inline constexpr const char* S_THIS = "this";
inline constexpr const char* S_THAT = "that";
inline constexpr const char* S_POINTER = "pointer";
inline constexpr const char* S_TEMP = "temp";
inline constexpr const char* S_CONSTANT = "constant";
inline constexpr const char* S_STATIC = "static";

// Hack registers (numbers)
inline constexpr int R_SP = 0;
inline constexpr int R_LCL = 1;
inline constexpr int R_ARG = 2;
inline constexpr int R_THIS_PTR = 3; // THIS base / pointer 0
inline constexpr int R_THAT_PTR = 4; // THAT base / pointer 1
inline constexpr int R_TEMP_BASE = 5; // temp 0..7 => R5..R12
inline constexpr int R_FRAME = 13;
inline constexpr int R_RET = 14;
inline constexpr int R_COPY = 15;

inline std::string asmReg(int regNum) { return "R" + std::to_string(regNum); }

} // namespace vm

