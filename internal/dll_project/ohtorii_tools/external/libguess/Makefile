PREFIX = /usr/local

MAJOR = 0
MINOR = 2
REVISION = 0
VER = ${MAJOR}.${MINOR}.${REVISION}

CC = gcc
OBJS = guess.o
SRCS = ${OBJS:.o=.c} guess_tab.c libguess.h test.c
LIBS = libguess.so libguess.a
CFLAGS += -fPIC
SONAME = libguess.so.${MAJOR}


all: $(LIBS)

libguess.so: ${OBJS}
	${CC} -o libguess.so -shared -Wl,-soname,${SONAME} ${OBJS}

libguess.a: ${OBJS}
	ar rc libguess.a ${OBJS}
	ranlib libguess.a

$(OBJS) : $(SRCS)

guess_tab.c : guess.scm
	gosh guess.scm guess_tab.c

test: test.c guess.c
	gcc -g -o test test.c guess.c

install:
	install -m644 libguess.h ${PREFIX}/include
	rm -f ${PREFIX}/lib/libguess.*
	install -m755 libguess.so ${PREFIX}/lib/libguess.so.${VER}
	install -m644 libguess.a ${PREFIX}/lib
	ln -sf ${PREFIX}/lib/libguess.so.${VER} ${PREFIX}/lib/libguess.so.${MAJOR}
	ln -sf ${PREFIX}/lib/libguess.so.${MAJOR} ${PREFIX}/lib/libguess.so

clean:
	rm -f $(LIBS) $(OBJS) test

distclean: clean
	rm -f *~ core*
