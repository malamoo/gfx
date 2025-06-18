.PHONY: all
all: a.out

a.out: $(OFILES)
	$(CC) $(LDFLAGS) -o a.out $(OFILES) $(LDADD)

%.o: %.c
	$(CC) $(CFLAGS) -c $*.c

%.o: %.m
	$(CC) $(MFLAGS) -c $*.m

.PHONY: install
install: $(BIN)/$(TARGET).app

$(BIN)/$(TARGET).app: Info.plist\
					  $(NIB).nib\
					  a.out\
					  debug.entitlements
	mkdir -p $(BIN)/$(TARGET).app/Contents/MacOS $(BIN)/$(TARGET).app/Contents/Resources
	cp Info.plist $(BIN)/$(TARGET).app/Contents
	cp $(NIB).nib $(BIN)/$(TARGET).app/Contents/Resources
	cp a.out $(BIN)/$(TARGET).app/Contents/MacOS/$(TARGET)
	codesign -f -s $(SIGNID) --entitlements debug.entitlements $(BIN)/$(TARGET).app

$(NIB).nib: $(NIB).xib
	ibtool --compile $(NIB).nib $(NIB).xib

debug.entitlements: $(TARGET).entitlements
	plutil -insert 'com\.apple\.security\.get-task-allow' -bool true $(TARGET).entitlements -o debug.entitlements

.PHONY: clean
clean:
	rm -f *.o $(NIB).nib a.out debug.entitlements $(CLEANFILES)
	rm -rf ./$(TARGET).app
