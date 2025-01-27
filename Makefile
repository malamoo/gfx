TARGET=Graphics
OFILES=main.o\
	AppDelegate.o\
	RayView.o\
	ray.o
NIB=Application.nib

BIN=.

include makeapp.mk
CC=clang
CFLAGS=-Wall\
	-Werror\
	-Wextra\
	-pedantic\
	-g\
	-Wno-unused-parameter\
	-std=c99\
	-MJ $*.o.json\
	-MMD
MFLAGS=-Wall\
	-Werror\
	-Wextra\
	-pedantic\
	-g\
	-Wno-unused-parameter\
	-ObjC\
	-fobjc-arc\
	-MJ $*.o.json\
	-MMD
LDADD=-framework Cocoa
CLEANFILES=*.o.json compile_commands.json *.d

compile_commands.json: $(OFILES:%.o=%.o.json)
	sed -e '1s/^/[\n/' -e '$$s/,$$/\n]/' *.o.json >compile_commands.json

-include $(OFILES:%.o=%.d)
