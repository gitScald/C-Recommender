OBJDIR = obj
EXEDIR = exe

TAR1 = googler
TAR2 = indexing
TAR3 = summarizer
OBJ1 = $(TAR1)/$(OBJDIR)/%.o
OBJ2 = $(TAR2)/$(OBJDIR)/%.o
OBJ3 = $(TAR3)/$(OBJDIR)/%.o
EXE1 = $(TAR1)/$(EXEDIR)/$(TAR1)
EXE2 = $(TAR2)/$(EXEDIR)/$(TAR2)
EXE3 = $(TAR3)/$(EXEDIR)/$(TAR3)

all:
	+$(MAKE) -C googler
	+$(MAKE) -C indexing
	+$(MAKE) -C summarizer

.PHONY: clean
clean:
	rm -f $(TAR1)/$(OBJDIR)/*o
	rm -f $(TAR2)/$(OBJDIR)/*o
	rm -f $(TAR3)/$(OBJDIR)/*o