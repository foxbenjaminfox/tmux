.PHONY: default clean install

CFLAGS=-shared -fPIE

default: cmd-print.so

cmd-print.so: cmd-print.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf cmd-print.so

install: cmd-print.so
	install cmd-print.so $(HOME)/.tmux/plugins/
