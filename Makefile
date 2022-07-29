OS = Linux
VERSION = 0.0.1

CXXFLAGS = -Wall -g -O3
LDFLAGS = -lm

CXX = /usr/bin/g++
RM = rm -rfv
MKDIR = mkdir -p
ECHO = echo

SOURCEDIR = src
HEADERDIR = include
BUILDDIR = build
BINDIR = bin
COMPILE_FLAGS = compile_flags.txt

BINARY  := $(BINDIR)/raytracing
SOURCES := $(shell find $(SOURCEDIR) -name '*.cpp')
OBJECTS := $(addprefix $(BUILDDIR)/,$(SOURCES:%.cpp=%.o))

.PHONY: all clean setup

all: setup $(BINARY)

$(BINARY): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJECTS) -o $(BINARY)

$(BUILDDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -I$(HEADERDIR) -I$(dir $<) -c $< -o $@

$(COMPILE_FLAGS):
	$(ECHO) $(CXXFLAGS) > $(COMPILE_FLAGS)
	$(ECHO) -I$(HEADERDIR) >> $(COMPILE_FLAGS)

setup: $(COMPILE_FLAGS)
	@$(MKDIR) $(BUILDDIR)/$(SOURCEDIR) $(BINDIR)

clean:
	$(RM) $(BINDIR) $(BUILDDIR) $(COMPILE_FLAGS)

distclean: clean

help:
	@$(ECHO) "Targets:"
	@$(ECHO) "all   - build/compile all source files"
	@$(ECHO) "clean - cleanup build files"