TARGET=Graphics
OFILES=main.o cocoa.o ray.o vec.o
NIB=MainMenu.nib

BIN=.

include makeapp.mk
CC=clang
CFLAGS=-Wall\
	-Werror\
	-Wextra\
	-pedantic\
	-ansi\
	-Wno-unused-parameter\
	-Wno-sign-compare\
	-Wno-unused-function\
	-g\
	-MJ $*.o.json\
	-MMD
MFLAGS=-Wall\
	-Werror\
	-Wextra\
	-pedantic\
	-ObjC\
	-fobjc-arc\
	-fmodules\
	-Wno-unused-parameter\
	-g\
	-MJ $*.o.json\
	-MMD
LDADD=-framework Cocoa
CLEANFILES=*.o.json compile_commands.json *.d

.PHONY: all
all: install

compile_commands.json: $(OFILES:%.o=%.o.json)
	sed -e '1s/^/[\n/' -e '$$s/,$$/\n]/' *.o.json >compile_commands.json

-include $(OFILES:%.o=%.d)
