#include "llvm/ADT/IntrusiveRefCntPtr.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Host.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Parse/Parser.h"
#include "clang/Parse/ParseAST.h"
#include <iostream>

using namespace llvm;
using namespace clang;

static cl::opt<std::string>
FileName(cl::Positional, cl::desc("Input File"), cl::Required);

int main(int argc, char **argv) {
	cl::ParseCommandLineOptions(argc, argv, "My Simple Front End\n");
	CompilerInstance CI;
	DiagnosticOptions diagnosticOptions;
	CI.createDiagnostics();

	TargetOptions PTO = TargetOptions();
	PTO.Triple = sys::getDefaultTargetTriple();
	TargetInfo *PTI = TargetInfo::CreateTargetInfo(CI.getDiagnostics(), std::make_shared<TargetOptions>(PTO));
	CI.setTarget(PTI);

	CI.createFileManager();
	
	CI.createSourceManager(CI.getFileManager());
	CI.createPreprocessor(TranslationUnitKind::TU_Complete); 
	CI.getPreprocessorOpts().UsePredefines = false;
	std::unique_ptr<ASTConsumer> astConsumer = CreateASTPrinter(NULL, "");
	CI.setASTConsumer(llvm::make_unique<ASTConsumer>(*astConsumer));

	CI.createASTContext();
	CI.createSema(TU_Complete, NULL);
	const FileEntry *pFile = CI.getFileManager().getFile(FileName);
	if (!pFile) {
		std::cerr << "File not found: " << FileName << std::endl;
		return 1;
	}

	MemoryBuffer *buff = CI.getSourceManager().getMemoryBufferForFile(pFile);
	FileID mainId = CI.getSourceManager().createFileID(MemoryBuffer::getMemBuffer(buff->getMemBufferRef()));
	CI.getSourceManager().setMainFileID(mainId);
	CI.getDiagnosticClient().BeginSourceFile(CI.getLangOpts(), 0);
	ParseAST(CI.getSema());
	CI.getASTContext().PrintStats();
	CI.getASTContext().Idents.PrintStats();
	
	return 0;

}


