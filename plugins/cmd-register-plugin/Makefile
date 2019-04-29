.PHONY: default clean install

CFLAGS=-shared -fPIE

default: cmd-register-plugin.so

cmd-register-plugin.so: cmd-register-plugin.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf cmd-register-plugin.so

install: cmd-register-plugin.so
	install cmd-register-plugin.so $(HOME)/.tmux/plugins/
