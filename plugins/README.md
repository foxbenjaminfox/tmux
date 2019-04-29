This is a collection of plugins to show off the capabilities of the plugin API.
Some of them are useful, and others just demonstrate some capabilities of the system.

The useful ones are:
- [`pretty-cwd`](./pretty-cwd), which provides a format variable very much like `#{pane_current_path}`,
  except trimmed to fit as part of the status bar.
- [`cmd-register-plugin`](./cmd-register-plugin), a meta-plugin which allows you
  to dynamically register other plugins.
