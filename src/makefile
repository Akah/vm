OBJDIR	:= objects
DIRS	:= $(shell find * -type d ! -iwholename '$(OBJDIR)/*')
OBJDIRS := $(patsubst %,$(OBJDIR)/%, $(DIRS))
OBJS	:= $(patsubst %.c,$(OBJDIR)/%.o,$(shell find * -type f -name '*.c'))
TARG	:= lisp

WARN	:= -Wall -Wswitch-enum # -Wextra 

CFLAGS	+= -g -std=gnu99
#CFLAGS	+= -Dtime="\"$(shell date --utc +%Y-%m-%dT%H:%M:%SZ)\""
#CFLAGS  += -Dbuild="\"$(shell uname -svrpo)\""

objdirs:
	mkdir -p $(OBJDIRS)

$(OBJDIR)/%.o: %.c objdirs
	$(CC) $(CFLAGS) $(WARN) -c -o $@ $<

$(TARG): $(DIRS) $(OBJS)
	$(CC) $(CFLAGS) $(WARN) -o $(TARG) $(OBJS) $(LIBS)

test: $(OBJS)

clean:
	rm -rf $(OBJDIR) $(TARG)

fresh:
	clean $(TARG)

build: $(TARG)
	strip $(TARG)

.PHONY: fresh clean $(TARG) test