LLVM_CONFIG?=llvm-config

ifndef VERBOSE
QUIET:=@
endif

SRC_DIR?=/usr/local/opt/llvm/include/ -I/usr/local/llvm36/include/
LDFLAGS+=$(shell $(LLVM_CONFIG) --ldflags)
LDFLAGS+=$(shell $(LLVM_CONFIG) --system-libs) 
COMMON_FLAGS=-Wall -Wextra
CXXFLAGS+=$(COMMON_FLAGS) $(shell $(LLVM_CONFIG) --cxxflags) 
CPPFLAGS+=$(shell $(LLVM_CONFIG) --cppflags) -I$(SRC_DIR) -fno-rtti

HELLO=hello
HELLO_OBJECTS=hello.o
default: $(HELLO)

%.o : $(SRC_DIR)/%.cpp
	@echo Compiling $*.cpp
	$(QUIET) $(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

$(HELLO) : $(HELLO_OBJECTS)
	@echo Linking $@
	$(QUIET)$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) $^ `$(LLVM_CONFIG) --libs bitreader core support`


clean::
	$(QUIET)rm -f $(HELLO) $(HELLO_OBJECTS)
