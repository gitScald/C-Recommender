CC := g++
CFLAGS := -c -std=c++11 -Wall

TAR := recommender
EXCL1 := gindexer.cpp
EXCL2 := googler.cpp
EXCL3 := summarizer.cpp

SRCDIR := ../src
OBJDIR := obj
EXEDIR := exe

SRC := $(filter-out $(SRCDIR)/$(EXCL1), $(wildcard $(SRCDIR)/*.cpp))
SRC := $(filter-out $(SRCDIR)/$(EXCL2), $(SRC))
SRC := $(filter-out $(SRCDIR)/$(EXCL3), $(SRC))
OBJ := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
EXE := $(EXEDIR)/$(TAR)

all: dir $(EXE)

dir:
	mkdir -p $(EXEDIR)
	mkdir -p $(OBJDIR)

$(EXE): $(OBJ)
	$(CC) $^ -o $@

$(OBJ): $(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*o $(EXE)