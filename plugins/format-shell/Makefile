.PHONY: default clean install

CFLAGS=-shared -fPIE

default: client-shell.so

client-shell.so: plugin.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf client-shell.so

install: client-shell.so
	install client-shell.so $(HOME)/.tmux/plugins/
