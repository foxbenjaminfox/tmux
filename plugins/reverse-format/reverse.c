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

#include <string.h>
#include <stdlib.h>

#include "../../tmux.h"

// This plugin provides a function that can be used in format strings to
// reverse values. For exmple, #{f:reverse:#{window_name}} would have the
// value of the name of the current window, but reversed. This is not very
// useful, and worse, this implementation doesn't work properly if the string
// contains non-ascii characters, so it's unlikely you actually want to use it.
// Nevertheless, it's a good example to demonstrate the possibilities available
// to format functions provided by plugins.

char *
reverse_cb(const char *arg) {
	int len = strlen(arg);
	char* str = xmalloc(len + 1);
	for (int i = 0; i < len; i++) {
		str[len - 1 - i] = arg[i];
	}
	str[len] = '\0';
	return str;
}

const PLUGIN plugin = {
	.type = FORMAT_FUNCTION_PLUGIN,
	.function = {
		.name = "reverse",
		.cb = reverse_cb
	}
};
