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

#include "../../tmux.h"
#include <assert.h>

/* This plugin demonstrates how to define a new tmux command from a plugin.
 *
 * The command:
 *    tmux print FILENAME TEXT
 * will print the text TEXT to the file FILENAME, overwriting any current contents
 * of the file.
 */
static enum cmd_retval	cmd_print_exec(struct cmd *, struct cmdq_item *);

const struct cmd_entry print_cmd_entry = {
	.name = "print", // This command is used as `tmux print`...
	.alias = "pr", // ... but that can be shortened to `tmux pr`.

	.args = { "", 2, 2 }, // The command takes exactly 2 positional arguments, and no flags.
	.usage = "filename text",

	.flags = 0,
	.exec = cmd_print_exec // This is the function that executes the command.
};

static enum cmd_retval
cmd_print_exec(struct cmd *self, struct cmdq_item *item)
{
		struct args	*args = self->args;

    assert(args->argc == 2);

		FILE *file = fopen(args->argv[0], "w");
		if (file == NULL)
			return (CMD_RETURN_ERROR);

		fprintf(file, "%s\n", args->argv[1]);
		fclose(file);

		return (CMD_RETURN_NORMAL);
}

const PLUGIN plugin = {
	.type = CMD_PLUGIN,    // This is a plugin that defines a new command.
	.cmd = print_cmd_entry // It uses this command entry.
};
