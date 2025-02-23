# t5-dll-proxy

This works by having the user rename the original `steam_api.dll` to `o_steam_api.dll`, and then dropping in the proxy dll which would be named `steam_api.dll`.

You can use this for any kind of engine level mods, [Custom-FX](https://github.com/BlackOpsOne/Custom-FX) utilizes similar code in the `steam_api.dll` file, however this is a bit cleaner.