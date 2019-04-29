/* $OpenBSD$ */

/*
 * Copyright (c) 2019 Benjamin Fox <foxbenjaminfox@gmail.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF MIND, USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>

#include "../../tmux.h"

// This plugin demonstrates how to define a new format variable, in this case
// pane_shell, which takes the value of the shell used in the current pane.
// (Basically, the equivalent of the SHELL environment variable.)

void
shell_format(struct format_tree *ft, struct format_entry *fe) {
	char* str;
	asprintf(&str, "%s", ft->wp->shell);
	fe->value = str;
}

const PLUGIN plugin = {
	.type = FORMAT_PLUGIN,
	.format = {
		.name = "pane_shell",
		.cb = shell_format
	}
};
