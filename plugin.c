/* $OpenBSD$ */

/*
 * Copyright (c) 2018 Benjamin Fox <foxbenjaminfox@gmail.com>
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

#include "tmux.h"
#include <dirent.h>
#include <dlfcn.h>
#include <limits.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static void		resize_plugins_buffer(void);
static void		register_plugin(struct plugin pl);
static const char*	get_plugin_name(struct plugin *plugin);

int		 plugins_length = 0;
static int	 plugins_capacity = 1;
struct plugin	*plugins = NULL;

void
init_plugins() {
	const char* home = find_home();

	if (plugins != NULL) fatalx("init_plugins must be called only once");

	plugins = xcalloc(plugins_capacity, sizeof(struct plugin));

	load_plugin_dir("/etc/tmux/plugins");
	if (home != NULL) {
		char* default_plugin_dir;
		xasprintf(&default_plugin_dir, "%s/.tmux/plugins", home);
		load_plugin_dir(default_plugin_dir);
		free(default_plugin_dir);
	}
}

void
load_plugin_dir(const char* base) {
	DIR *dir = opendir(base);
	struct dirent *de;
	char* name;

	if (dir == NULL) return;

	while ((de = readdir(dir)) != NULL) {
		u_int len = strlen(de->d_name);
		if (len <= 3 || strcmp(&de->d_name[len-3], ".so")) continue;
		xasprintf(&name, "%s/%s", base, de->d_name);
		load_plugin(name);
		free(name);
	}
}

void
load_plugin(const char* name) {
	void* handle;
	const struct plugin *plugin;

	if (name == NULL ||
	   (handle = dlopen(name, RTLD_NOW | RTLD_LOCAL)) == NULL)
		return;
	if ((plugin = dlsym(handle, "plugin")) == NULL) {
		dlclose(handle);
		return;
	}

	register_plugin(*plugin);
}

char*
plugin_call(const char* func, const char* arg) {
	for (int i = 0; i < plugins_length; i++) {
		struct plugin pl = plugins[i];
		if (pl.type != FORMAT_FUNCTION_PLUGIN)
			continue;
		if (strcmp(pl.function.name, func) == 0)
			return pl.function.cb(arg);
	}
	return xstrdup("");
}

void
plugin_notify(struct notify_entry *ne) {
	for (int i = 0; i < plugins_length; i++) {
		struct plugin pl = plugins[i];
		if (pl.type != NOTIFICATION_PLUGIN)
			continue;
		if (pl.notify.event == NULL || strcmp(pl.notify.event, ne->name) == 0)
			pl.notify.cb(ne);
	}
}

int
find_first_plugin_index(int type) {
	return find_next_plugin_index(type, -1);
}

int
find_next_plugin_index(int type, int from) {
	for (int i = from + 1; i < plugins_length; i++) {
		if (plugins[i].type == type)
			return i;
	}
	return -1;
}

static void
resize_plugins_buffer() {
	assert(plugins != NULL);
	if (plugins_length >= plugins_capacity) {
		if (plugins_capacity >= INT_MAX / 2)
			fatalx("plugins capacity overflow");

		plugins_capacity *= 2;
		plugins = xreallocarray(plugins, plugins_capacity, sizeof(struct plugin));
	}
}

static void
register_plugin(struct plugin pl) {
	resize_plugins_buffer();
	assert(plugins_capacity > plugins_length);
	if (pl.type == MULTI_PLUGIN)
		for (u_int i = 0; i < pl.multi.length; i++)
			register_plugin(pl.multi.plugins[i]);
	else
		plugins[plugins_length++] = pl;
}


void
get_plugins_list(char **list) {
	const char	*name, *prev;

	if (!plugins_length) {
		*list = xstrdup("");
		return;
	}
	*list = xstrdup(get_plugin_name(&plugins[0]));
	for (int i = 1; i < plugins_length; i++) {
		name = get_plugin_name(&plugins[i]);
		prev = *list;
		xasprintf(list, "%s,%s", prev, name);
		free((void*)prev);
	}
}

static const char*
get_plugin_name(struct plugin *plugin) {
	switch (plugin->type) {
		case FORMAT_PLUGIN:
			return plugin->format.name;
		case FORMAT_FUNCTION_PLUGIN:
			return plugin->function.name;
		case CMD_PLUGIN:
			return plugin->cmd.name;
		case NOTIFICATION_PLUGIN:
			return plugin->notify.event;
		case MULTI_PLUGIN:
			return "...";
	}
	fatalx("Bad plugin type %d", plugin->type);
}
