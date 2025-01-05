CONT=$(TARG).app/Contents

.PHONY: all
all: $(CONT)/MacOS\
	 $(CONT)/Resources\
	 $(CONT)/Info.plist\
	 $(CONT)/MacOS/$(TARG)\
	 $(CONT)/Resources/$(NIB)

$(CONT)/MacOS $(CONT)/Resources:
	mkdir -p $(CONT)/MacOS $(CONT)/Resources

$(CONT)/Info.plist: Info.plist
	cp Info.plist $(CONT)

$(CONT)/MacOS/$(TARG): $(OFILES)
	$(CC) $(LDFLAGS) -o $(CONT)/MacOS/$(TARG) $(OFILES) $(LDADD)

%.o: $(HFILES)

%.o: %.c
	$(CC) $(CFLAGS) -c $*.c

%.o: %.m
	$(CC) $(MFLAGS) -c $*.m

$(CONT)/Resources/%.nib: %.xib
	ibtool --compile $(CONT)/Resources/$*.nib $*.xib

.PHONY: clean
clean:
	rm -f *.o $(CLEANFILES)
	rm -rf ./$(TARG).app
