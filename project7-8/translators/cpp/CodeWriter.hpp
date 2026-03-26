#pragma once

#include "VMConstants.hpp"

#include <fstream>
#include <string>

namespace vm {

class CodeWriter {
public:
  explicit CodeWriter(const std::string& outAsmPath);

  void close();

  void setFileName(const std::string& filenameOnly); // e.g. "Main.vm" or "Main"
  void writeInit();

  void writeArithmetic(const std::string& command);
  void writePushPop(CommandType command, const std::string& segment, int index);

  void writeLabel(const std::string& label);
  void writeGoto(const std::string& label);
  void writeIf(const std::string& label);

  void writeFunction(const std::string& functionName, int numLocals);
  void writeCall(const std::string& functionName, int numArgs);
  void writeReturn();

private:
  // helpers
  std::string newLabel(const std::string& prefix = "LABEL");
  std::string staticName(int index) const;
  std::string scopedLabel(const std::string& label) const;

  void a(const std::string& address);
  void c(const std::string& dest, const std::string& comp, const std::string& jump = "");
  void l(const std::string& label);

  void incSP();
  void decSP();
  void loadSPToA();

  void compToReg(int reg, const std::string& comp);
  void regToDest(const std::string& dest, int reg);
  void regToReg(int dest, int src);

  void stackToDest(const std::string& dest);
  void compToStack(const std::string& comp);

  void valToStack(const std::string& val);

  bool isMemSeg(const std::string& seg) const;
  bool isConstSeg(const std::string& seg) const;
  bool isStaticSeg(const std::string& seg) const;
  bool isRegSeg(const std::string& seg) const;

  int regBase(const std::string& seg) const;
  int regNum(const std::string& seg, int index) const;

  std::string asmMemSeg(const std::string& seg) const;

  void loadSeg(const std::string& seg, int index, bool indirect = true);
  void loadSegNoIndex(const std::string& seg, bool indirect);
  void loadSegIndex(const std::string& seg, int index, bool indirect);
  void indir(const std::string& dest = "A");

  void push(const std::string& seg, int index);
  void pop(const std::string& seg, int index);
  void popToDest(const std::string& dest);

  void unary(const std::string& comp);
  void binary(const std::string& comp);
  void compare(const std::string& jump);
  std::string jumpToNewLabel(const std::string& comp, const std::string& jump);

  void loadSpOffset(int offset);
  void prevFrameToReg(int reg);

  std::ofstream out_;
  std::string vmFileBase_;
  std::string currentFunction_;
  long long labelNum_ = 0;
  long long callNum_ = 0;
};

} // namespace vm

