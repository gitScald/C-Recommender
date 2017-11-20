OBJDIR = obj
EXEDIR = exe

TAR1 = googler
TAR2 = indexing
TAR3 = summarizer
TAR4 = recommender
OBJ1 = $(TAR1)/$(OBJDIR)/%.o
OBJ2 = $(TAR2)/$(OBJDIR)/%.o
OBJ3 = $(TAR3)/$(OBJDIR)/%.o
OBJ4 = $(TAR4)/$(OBJDIR)/%.o
EXE1 = $(TAR1)/$(EXEDIR)/$(TAR1)
EXE2 = $(TAR2)/$(EXEDIR)/$(TAR2)
EXE3 = $(TAR3)/$(EXEDIR)/$(TAR3)
EXE4 = $(TAR4)/$(EXEDIR)/$(TAR4)

all:
	+$(MAKE) -C googler
	+$(MAKE) -C indexing
	+$(MAKE) -C summarizer
	+$(MAKE) -C recommender

.PHONY: clean
clean:
	rm -f $(TAR1)/$(OBJDIR)/*o
	rm -f $(TAR2)/$(OBJDIR)/*o
	rm -f $(TAR3)/$(OBJDIR)/*o
	rm -f $(TAR4)/$(OBJDIR)/*o