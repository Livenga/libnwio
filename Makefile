CC 	   = gcc
OBJDIR = objs
PRJC 	 = libnwio.so

SRC     = $(shell find src/ -name \*.c)
OBJS    = $(addprefix $(OBJDIR)/,$(patsubst %.c,%.o,$(SRC)))
OBJSDIR = $(sort $(dir $(OBJS)))


default:
	[ -d  $(OBJDIR)   ] || mkdir -v  $(OBJDIR)
	[ -d "$(OBJSDIR)" ] || mkdir -pv $(OBJSDIR)
	echo $(OBJSDIR)
	make $(PRJC)


$(PRJC):$(OBJS)
	$(CC) -o $@ $^ \
		-fPIC -shared


$(OBJDIR)/%.o:%.c
	$(CC) -o $@ -c $< -fPIC


clean:
	@[ ! -e $(OBJDIR) ] || rm -rv $(OBJDIR)
	@[ ! -e $(PRJC)   ] || rm -v $(PRJC)


all:
	make clean
	make
