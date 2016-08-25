#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/raw_os_ostream.h"
#include <iostream>


using namespace llvm;

static cl::opt<std::string>
FileName(cl::Positional, cl::desc("Bitcode file"), cl::Required);

int main(int argc, char **argv) {
	cl::ParseCommandLineOptions(argc, argv, "LLVM Hello World\n");
	LLVMContext context;
	ErrorOr<std::unique_ptr<MemoryBuffer> > mb = MemoryBuffer::getFile(FileName);
	ErrorOr<Module *> m = parseBitcodeFile(mb.get()->getMemBufferRef(), context);

	if (m.getError()) {
		std::cerr << "Error reading bitcode " << m.getError() << "\n";
		return -1;
	}

	raw_os_ostream O(std::cout);
	for (Module::const_iterator i = m.get()->getFunctionList().begin(),
					e = m.get()->getFunctionList().end(); i != e; ++i) {
		if (!i->isDeclaration()) {
			O << i->getName() << " has " << i->size() << "basic block(s).\n";
		}
	}

	return 0;

}
