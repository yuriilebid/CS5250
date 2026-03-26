#include "CodeWriter.hpp"

#include <filesystem>
#include <stdexcept>

namespace vm {

CodeWriter::CodeWriter(const std::string& outAsmPath) : out_(outAsmPath) {
  if (!out_) throw std::runtime_error("Failed to open output file: " + outAsmPath);
}

void CodeWriter::close() {
  out_.close();
}

void CodeWriter::setFileName(const std::string& filenameOnly) {
  std::filesystem::path p(filenameOnly);
  vmFileBase_ = p.stem().string();
}

void CodeWriter::writeInit() {
  a("256");
  c("D", "A");
  compToReg(R_SP, "D"); // SP=256
  writeCall("Sys.init", 0);
}

void CodeWriter::writeArithmetic(const std::string& command) {
  if (command == "add") binary("D+A");
  else if (command == "sub") binary("A-D");
  else if (command == "neg") unary("-D");
  else if (command == "eq") compare("JEQ");
  else if (command == "gt") compare("JGT");
  else if (command == "lt") compare("JLT");
  else if (command == "and") binary("D&A");
  else if (command == "or") binary("D|A");
  else if (command == "not") unary("!D");
}

void CodeWriter::writePushPop(CommandType command, const std::string& segment, int index) {
  if (command == CommandType::Push) push(segment, index);
  else if (command == CommandType::Pop) pop(segment, index);
}

void CodeWriter::writeLabel(const std::string& label) { l(scopedLabel(label)); }

void CodeWriter::writeGoto(const std::string& label) {
  a(scopedLabel(label));
  c("", "0", "JMP");
}

void CodeWriter::writeIf(const std::string& label) {
  popToDest("D");
  a(scopedLabel(label));
  c("", "D", "JNE");
}

void CodeWriter::writeFunction(const std::string& functionName, int numLocals) {
  currentFunction_ = functionName;
  l(functionName);
  for (int i = 0; i < numLocals; i++) push(S_CONSTANT, 0);
}

void CodeWriter::writeCall(const std::string& functionName, int numArgs) {
  // Return labels should be globally unique
  std::string returnAddress = functionName + "$ret." + std::to_string(++callNum_);
  // push return-address
  a(returnAddress);
  c("D", "A");
  compToStack("D");
  incSP();

  push("reg", R_LCL);        // push LCL
  push("reg", R_ARG);        // push ARG
  push("reg", R_THIS_PTR);   // push THIS
  push("reg", R_THAT_PTR);   // push THAT

  loadSpOffset(-numArgs - 5);
  compToReg(R_ARG, "D");     // ARG=SP-n-5
  regToReg(R_LCL, R_SP);     // LCL=SP
  a(functionName);
  c("", "0", "JMP");
  l(returnAddress);
}

void CodeWriter::writeReturn() {
  regToReg(R_FRAME, R_LCL); // FRAME=LCL

  a("5");
  c("A", "D-A");            // A=FRAME-5 (D still holds FRAME from regToReg -> via D)
  c("D", "M");              // D=*(FRAME-5)
  compToReg(R_RET, "D");    // RET=*(FRAME-5)

  pop(S_ARGUMENT, 0);       // *ARG=return value
  regToDest("D", R_ARG);    // D=ARG
  compToReg(R_SP, "D+1");   // SP=ARG+1

  prevFrameToReg(R_THAT_PTR);
  prevFrameToReg(R_THIS_PTR);
  prevFrameToReg(R_ARG);
  prevFrameToReg(R_LCL);

  regToDest("A", R_RET);
  c("", "0", "JMP");
}

// ---- helpers ----

std::string CodeWriter::newLabel(const std::string& prefix) { return prefix + std::to_string(++labelNum_); }

std::string CodeWriter::staticName(int index) const { return vmFileBase_ + "." + std::to_string(index); }

std::string CodeWriter::scopedLabel(const std::string& label) const {
  if (currentFunction_.empty()) return label;
  return currentFunction_ + "$" + label;
}

void CodeWriter::a(const std::string& address) { out_ << "@" << address << "\n"; }

void CodeWriter::c(const std::string& dest, const std::string& comp, const std::string& jump) {
  if (!dest.empty()) out_ << dest << "=";
  out_ << comp;
  if (!jump.empty()) out_ << ";" << jump;
  out_ << "\n";
}

void CodeWriter::l(const std::string& label) { out_ << "(" << label << ")\n"; }

void CodeWriter::incSP() {
  a("SP");
  c("M", "M+1");
}

void CodeWriter::decSP() {
  a("SP");
  c("M", "M-1");
}

void CodeWriter::loadSPToA() {
  a("SP");
  c("A", "M");
}

void CodeWriter::compToReg(int reg, const std::string& comp) {
  a(asmReg(reg));
  c("M", comp);
}

void CodeWriter::regToDest(const std::string& dest, int reg) {
  a(asmReg(reg));
  c(dest, "M");
}

void CodeWriter::regToReg(int dest, int src) {
  regToDest("D", src);
  compToReg(dest, "D");
}

void CodeWriter::stackToDest(const std::string& dest) {
  loadSPToA();
  c(dest, "M");
}

void CodeWriter::compToStack(const std::string& comp) {
  loadSPToA();
  c("M", comp);
}

void CodeWriter::valToStack(const std::string& val) {
  a(val);
  c("D", "A");
  compToStack("D");
}

bool CodeWriter::isMemSeg(const std::string& seg) const {
  return seg == S_LOCAL || seg == S_ARGUMENT || seg == S_THIS || seg == S_THAT;
}
bool CodeWriter::isConstSeg(const std::string& seg) const { return seg == S_CONSTANT; }
bool CodeWriter::isStaticSeg(const std::string& seg) const { return seg == S_STATIC; }
bool CodeWriter::isRegSeg(const std::string& seg) const { return seg == "reg" || seg == S_POINTER || seg == S_TEMP; }

int CodeWriter::regBase(const std::string& seg) const {
  if (seg == "reg") return 0; // direct R#
  if (seg == S_POINTER) return R_THIS_PTR; // pointer 0->R3, 1->R4
  if (seg == S_TEMP) return R_TEMP_BASE;   // temp 0->R5
  return 0;
}

int CodeWriter::regNum(const std::string& seg, int index) const { return regBase(seg) + index; }

std::string CodeWriter::asmMemSeg(const std::string& seg) const {
  if (seg == S_LOCAL) return "LCL";
  if (seg == S_ARGUMENT) return "ARG";
  if (seg == S_THIS) return "THIS";
  if (seg == S_THAT) return "THAT";
  return seg;
}

void CodeWriter::indir(const std::string& dest) { c(dest, "M"); }

void CodeWriter::loadSegNoIndex(const std::string& seg, bool indirect) {
  a(seg);
  if (indirect) indir("AD"); // A=D=*A
}

void CodeWriter::loadSegIndex(const std::string& seg, int index, bool indirect) {
  std::string comp = "D+A";
  int idx = index;
  if (index < 0) {
    idx = -index;
    comp = "A-D";
  }
  a(std::to_string(idx));
  c("D", "A");
  a(seg);
  if (indirect) indir("A"); // A=*seg
  c("AD", comp);            // A=D=seg +/- index
}

void CodeWriter::loadSeg(const std::string& seg, int index, bool indirect) {
  if (index == 0) loadSegNoIndex(seg, indirect);
  else loadSegIndex(seg, index, indirect);
}

void CodeWriter::push(const std::string& seg, int index) {
  if (isConstSeg(seg)) {
    valToStack(std::to_string(index));
  } else if (isMemSeg(seg)) {
    loadSeg(asmMemSeg(seg), index, true);
    c("D", "M");
    compToStack("D");
  } else if (isRegSeg(seg)) {
    regToDest("D", regNum(seg, index));
    compToStack("D");
  } else if (isStaticSeg(seg)) {
    a(staticName(index));
    c("D", "M");
    compToStack("D");
  }
  incSP();
}

void CodeWriter::pop(const std::string& seg, int index) {
  decSP();
  if (isMemSeg(seg)) {
    loadSeg(asmMemSeg(seg), index, true);
    compToReg(R_COPY, "D"); // R_COPY = addr (in D from AD load)
    stackToDest("D");
    regToDest("A", R_COPY);
    c("M", "D");
  } else if (isRegSeg(seg)) {
    stackToDest("D");
    compToReg(regNum(seg, index), "D");
  } else if (isStaticSeg(seg)) {
    stackToDest("D");
    a(staticName(index));
    c("M", "D");
  }
}

void CodeWriter::popToDest(const std::string& dest) {
  decSP();
  stackToDest(dest);
}

void CodeWriter::unary(const std::string& comp) {
  decSP();
  stackToDest("D");
  c("D", comp);
  compToStack("D");
  incSP();
}

void CodeWriter::binary(const std::string& comp) {
  decSP();
  stackToDest("D");
  decSP();
  stackToDest("A");
  c("D", comp);
  compToStack("D");
  incSP();
}

std::string CodeWriter::jumpToNewLabel(const std::string& comp, const std::string& jump) {
  std::string label = newLabel();
  a(label);
  c("", comp, jump);
  return label;
}

void CodeWriter::compare(const std::string& jump) {
  decSP();
  stackToDest("D");
  decSP();
  stackToDest("A");
  c("D", "A-D");
  std::string labelTrue = jumpToNewLabel("D", jump);
  compToStack("0");
  std::string labelEnd = jumpToNewLabel("0", "JMP");
  l(labelTrue);
  compToStack("-1");
  l(labelEnd);
  incSP();
}

void CodeWriter::loadSpOffset(int offset) {
  // D and A become SP +/- offset
  loadSeg(asmReg(R_SP), offset, true);
}

void CodeWriter::prevFrameToReg(int reg) {
  regToDest("D", R_FRAME);   // D=FRAME
  c("D", "D-1");             // D=FRAME-1
  compToReg(R_FRAME, "D");   // FRAME=FRAME-1
  c("A", "D");               // A=FRAME-1
  c("D", "M");               // D=*(FRAME-1)
  compToReg(reg, "D");       // reg=D
}

} // namespace vm

