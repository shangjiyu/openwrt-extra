TOPSRCDIR = $(shell pwd)
DESTDIR = /usr/local/bin/

export TOPSRCDIR DESTDIR

#CC = gcc
WARN = -Wall
OPTIMIZE = -g
#CFLAGS = $(WARN) $(OPTIMIZE) 

BINS = drcom
OBJS = drcom.o main.o
SRCS = drcom.c md5.c main.c
INCS = drcom.h md5.h

.PHONY: all clean install

all: $(SRCS)
#	@for x in $(SUBDIRS); do (cd $$x && make all) || exit 1; done
	$(CC) $(CFLAGS) $(SRCS) -o $(BINS)

clean:
#	@for x in $(SUBDIRS); do (cd $$x && make clean) || exit 1; done
	@if [ -e /etc/drcom.conf ]; then \
	    rm -f /etc/drcom.conf; \
	fi;
	@if [ -e /usr/local/bin/drcom ]; then \
	    rm -f /usr/local/bin/drcom; \
	fi;

install:
#	@for x in $(SUBDIRS); do (cd $$x && make install) || exit 1; done
	@echo
	@echo
	@echo
	@if [ -f /etc/drcom.conf ]; then \
		echo "====================================" && \
		echo "" && \
		echo "/etc/drcom.conf exists.";\
		echo "" && \
		echo "You May Need to Edit /etc/drcom.conf" && \
		echo "" && \
		echo "====================================" \
		;\
	else\
		install -m 755 drcom.conf /etc/drcom.conf && \
		echo "====================================" && \
		echo "" && \
		echo "Do Not Forget To Edit /etc/drcom.conf" && \
		echo "" && \
		echo "====================================" \
		;\
	fi;
	@echo
	@install -m 755 drcom $(DESTDIR)

