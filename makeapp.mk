APP=$(TARG).app

.PHONY: all
all: a.out

a.out: $(OFILES)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDADD)

%.o: %.c
	$(CC) $(CFLAGS) -c $*.c

%.o: %.m
	$(CC) $(MFLAGS) -c $*.m

.PHONY: install
install: $(BIN)/$(TARG)

$(BIN)/$(TARG): a.out
	cp $< $@

.PHONY: install_app
install_app: $(BIN)/$(APP)/Contents/MacOS/$(TARG) \
			 $(BIN)/$(APP)/Contents/Resources/$(NIB).nib\
	 		 $(BIN)/$(APP)/Contents/Info.plist

$(BIN)/$(APP)/Contents/MacOS/$(TARG): a.out
	mkdir -p $(@D)
	cp $< $@

$(BIN)/$(APP)/Contents/Resources/$(NIB).nib: $(NIB).xib
	mkdir -p $(@D)
	ibtool --compile $@ $<

$(BIN)/$(APP)/Contents/Info.plist: Info.plist
	mkdir -p $(@D)
	cp $< $@

.PHONY: sign
sign: debug.entitlements
	codesign --remove-signature $(BIN)/$(APP)
	codesign --sign "$(SIGNID)" --entitlements debug.entitlements $(BIN)/$(APP)

debug.entitlements: $(TARG).entitlements
	plutil -insert 'com\.apple\.security\.get-task-allow' -bool true $< -o $@

.PHONY: clean
clean:
	rm -f *.o a.out debug.entitlements $(CLEANFILES)
	rm -rf ./$(TARG) ./$(APP)