.PHONY: all
all: a.out

a.out: $(OFILES)
	$(CC) $(LDFLAGS) -o a.out $(OFILES) $(LDADD)

%.o: %.c
	$(CC) $(CFLAGS) -c $*.c

%.o: %.m
	$(CC) $(MFLAGS) -c $*.m

.PHONY: install
install: $(BIN)/$(TARGET).app/Contents/MacOS\
		 $(BIN)/$(TARGET).app/Contents/Resources\
		 $(BIN)/$(TARGET).app/Contents/Info.plist\
		 $(BIN)/$(TARGET).app/Contents/MacOS/$(TARGET)\
		 $(BIN)/$(TARGET).app/Contents/Resources/$(NIB)

$(BIN)/$(TARGET).app/Contents/MacOS $(BIN)/$(TARGET).app/Contents/Resources:
	mkdir -p $(BIN)/$(TARGET).app/Contents/MacOS $(BIN)/$(TARGET).app/Contents/Resources

$(BIN)/$(TARGET).app/Contents/Info.plist: Info.plist
	cp Info.plist $(BIN)/$(TARGET).app/Contents

$(BIN)/$(TARGET).app/Contents/MacOS/$(TARGET): a.out
	cp a.out $(BIN)/$(TARGET).app/Contents/MacOS/$(TARGET)

$(BIN)/$(TARGET).app/Contents/Resources/$(NIB): $(NIB)
	cp $(NIB) $(BIN)/$(TARGET).app/Contents/Resources

.PHONY: clean
clean:
	rm -f *.o a.out $(CLEANFILES)
	rm -rf ./$(TARGET).app
