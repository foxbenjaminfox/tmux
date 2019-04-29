.PHONY: default clean install

CFLAGS=-shared -fPIE

default: reverse.so

reverse.so: reverse.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf reverse.so

install: reverse.so
	install reverse.so $(HOME)/.tmux/plugins/
