SOURSE:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SOURSE))
ELF:=main
CC:=gcc
$(ELF):$(OBJS)
	$(CC) $^ -o $@
.PHONY:clean
clean:
	rm -rf $(OBJS) $(ELF)
