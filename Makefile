TARG=App
OFILES=\
	main.o\
	cocoa.o\
	ray.o\
	vec.o\
	eprintf.o
NIB=MainMenu

BIN=.

include makeapp.mk
CC=clang
CFLAGS=\
	-Wall\
	-Werror\
	-Wextra\
	-pedantic\
	-ansi\
	-Wno-unused-parameter\
	-Wno-sign-compare\
	-Wno-unused-function\
	-Wno-missing-field-initializers\
	-Wno-incompatible-pointer-types-discards-qualifiers\
	-MJ $*.o.json\
	-MMD\
	-g
MFLAGS=\
	-Wall\
	-Werror\
	-Wextra\
	-pedantic\
	-ObjC\
	-fobjc-arc\
	-fmodules\
	-Wno-unused-parameter\
	-MJ $*.o.json\
	-MMD\
	-g
LDADD=-framework Cocoa
SIGNID=Development
CLEANFILES=*.d *.o.json compile_commands.json

compile_commands.json: $(OFILES:%.o=%.o.json)
	sed -e '1s/^/[\n/' -e '$$s/,$$/\n]/' *.o.json >compile_commands.json

-include $(OFILES:%.o=%.d)
