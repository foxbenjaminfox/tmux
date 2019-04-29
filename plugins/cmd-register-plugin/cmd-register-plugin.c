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
#include <dlfcn.h>

// This is a plugin for dynamicly registering other plugins.
// For example:
// $ tmux register-plugin /path/to/plugin.so
// Or in tmux.conf:
// register-plugin /path/to/plugin.so

static enum cmd_retval	cmd_register_plugin_exec(struct cmd *, struct cmdq_item *);
static enum cmd_retval	cmd_register_plugin_dir_exec(struct cmd *, struct cmdq_item *);

const struct cmd_entry register_plugin_cmd_entry = {
	.name = "register-plugin",
	.alias = "reg",

	.args = { "", 1, 1 },
	.usage = "path",

	.flags = 0,
	.exec = cmd_register_plugin_exec
};

const struct cmd_entry register_plugin_dir_cmd_entry = {
	.name = "register-plugin-dir",
	.alias = "reg-dir",

	.args = { "", 1, 1 },
	.usage = "directory",

	.flags = 0,
	.exec = cmd_register_plugin_dir_exec
};

static enum cmd_retval
cmd_register_plugin_exec(struct cmd *self, struct cmdq_item *item)
{
	struct args	*args = self->args;
	char		*path, *error;

	assert(args->argc == 1);
	path = args->argv[0];

	assert(args->argc == 1);
	path = args->argv[0];

	if (*path != '/') {
		cmdq_error(item, "failed to load plugin \"%s\": path must be absolute.",
							 path);
		return (CMD_RETURN_ERROR);
	}

	dlerror(); // Clear any existing error from dlerror

	load_plugin(path);

	if ((error = dlerror()) != NULL) {
		cmdq_error(item, "register-plugin failed: \"%s\".", error);
		return (CMD_RETURN_ERROR);
	}

	return (CMD_RETURN_NORMAL);
}

static enum cmd_retval
cmd_register_plugin_dir_exec(struct cmd *self, struct cmdq_item *item)
{
	struct args	*args = self->args;

	assert(args->argc == 1);

	load_plugin_dir(args->argv[0]);

	return (CMD_RETURN_NORMAL);
}

PLUGIN plugin_list[] = {
	{
		.type = CMD_PLUGIN,
		.cmd = register_plugin_cmd_entry
	},
	{
		.type = CMD_PLUGIN,
		.cmd = register_plugin_dir_cmd_entry
	}
};

const PLUGIN plugin = {
	.type = MULTI_PLUGIN,
	.multi = {
		.length = 2,
		.plugins = plugin_list
	}
};
