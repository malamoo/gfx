TARG=Graphics
OFILES=main.o\
	AppDelegate.o\
	RenderView.o
NIB=Application.nib

include makeapp.mk
CC=clang
CFLAGS=-Wall\
	-Werror\
	-Wextra\
	-pedantic\
	-g\
	-Wno-unused-parameter\
	-std=c99\
	-MJ $*.o.json
MFLAGS=-Wall\
	-Werror\
	-Wextra\
	-pedantic\
	-g\
	-Wno-unused-parameter\
	-fobjc-arc\
	-MJ $*.o.json
LDADD=-framework Cocoa
CLEANFILES=*.o.json compile_commands.json

compile_commands.json: $(OFILES:%=%.json) Makefile
	sed -e '1s/^/[\n/' -e '$$s/,$$/\n]/' *.o.json >compile_commands.json
