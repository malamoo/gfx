TARG=Graphics
OFILES=main.o\
	AppDelegate.o\
	RenderView.o

BIN=Graphics.app/Contents/MacOS

include makeone.mk
CC=clang
CFLAGS=-Wall\
	-Werror\
	-Wextra\
	-pedantic\
	-Wno-unused-parameter\
	-Wno-sign-compare\
	-std=c99\
	-g\
	-MJ $@.json
MFLAGS=-Wall\
	-Werror\
	-Wextra\
	-pedantic\
	-g\
	-MJ $@.json
LDADD=-framework Cocoa
CLEANFILES=*.o.json compile_commands.json

.PHONY: all
all: o.out compile_commands.json

compile_commands.json: $(OFILES)
	sed -e '1s/^/[\n/' -e '$$s/,$$/\n]/' *.o.json >compile_commands.json