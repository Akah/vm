OBJDIR	:= objects
OBJS	:= $(patsubst %.c,$(OBJDIR)/%.o,$(wildcard *.c))
TARG	:= test
LIBS	:= -lcunit
# find all .o files from src to link
SRCOBJS = $(shell find ../src/objects -name '*.o' ! -iname '*main.o')

$(OBJDIR)/%.o: %.c
	@mkdir -p objects
	$(CC) $(CFLAGS) $(WARN)-c -o $@ $<

$(TARG): $(OBJS) $(SRCOBJS)
	$(CC) $(CFLAGS) $(WARN) -o $(TARG) $(OBJS) $(SRCOBJS) $(LIBS)

clean:
	rm -rf $(OBJDIR) $(TARG)

fresh:
	clean $(TARG)

.PHONY: fresh clean $(TARG)