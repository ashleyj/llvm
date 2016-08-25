LLVM_CONFIG?=llvm-config

ifndef VERBOSE
QUIET:=@
endif

SRC_DIR?=/usr/local/opt/llvm/include/
LDFLAGS+=$(shell $(LLVM_CONFIG) --ldflags)
LDFLAGS+=$(shell $(LLVM_CONFIG) --system-libs)
COMMON_FLAGS=-Wall -Wextra
CXXFLAGS+=$(COMMON_FLAGS) $(shell $(LLVM_CONFIG) --cxxflags)
CPPFLAGS+=$(shell $(LLVM_CONFIG) --cppflags) -I$(SRC_DIR) -fno-rtti

CLANGLIBS = \
	-Wl \
	-lclang\
	-lclangFrontend\
	-lclangDriver\
	-lclangSerialization\
	-lclangParse\
	-lclangSema\
	-lclangAnalysis\
	-lclangEdit\
	-lclangAST\
	-lclangBasic\
	-Wl

LLVMLIBS=$(shell $(LLVM_CONFIG) --libs)

PROJECT=project
PROJECT_OBJECTS=project.o

default: $(PROJECT)
%.o : $(SRC_DIR)/%.cpp
	@echo Compiling $*.cpp
	$(QUIET)$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

$(PROJECT) : $(PROJECT_OBJECTS)
	@echo Linking $@
	$(QUIET)$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) $^ $(CLANGLIBS) $(LLVMLIBS)

clean::
	$(QUIET)rm -f $(PROJECT) $(PROJECT_OBJECTS)
