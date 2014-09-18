
CFLAGS=-DUNIX -lreadline -lcurses 
DEBUG=-g
SRCDIR=../src
#DEBUG=

all: shell


shell:	$(SRCDIR)/*
	g++ $(CFLAGS) $(DEBUG) $(SRCDIR)/*.cpp -o shell
clean:
	rm -f shell *~

