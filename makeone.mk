.PHONY: all
all: o.out

o.out: $(OFILES) $(LIB)
	$(CC) $(LDFLAGS) -o o.out $(OFILES) $(LDADD)

%.o: $(HFILES)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.o: %.m
	$(CC) $(MFLAGS) -c $<

.PHONY: install
install: $(BIN)/$(TARG)

$(BIN)/$(TARG): o.out
	cp o.out $(BIN)/$(TARG)

.PHONY: clean
clean:
	rm -f *.o o.out $(TARG) $(CLEANFILES)
