NAME = TinyTerm
VERSION = 0.1

CC := $(CC) -std=c99

base_CFLAGS = -Wall -Wextra -pedantic -O2 -g -DPROGNAME=\"${NAME}\" -DVERSION=\"${VERSION}\"
base_LIBS = -lm

pkgs = vte
pkgs_CFLAGS = $(shell pkg-config --cflags $(pkgs))
pkgs_LIBS = $(shell pkg-config --libs $(pkgs))

CFLAGS := $(base_CFLAGS) $(pkgs_CFLAGS) $(CFLAGS)
LDLIBS := $(base_LIBS) $(pkgs_LIBS)

all: tinyterm

tinyterm: tinyterm.c config.h

clean:
	$(RM) tinyterm tinyterm.o

install: tinyterm
	install -Dm755 tinyterm $(DESTDIR)/usr/bin/tinyterm
