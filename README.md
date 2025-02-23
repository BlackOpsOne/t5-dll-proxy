# t5-dll-proxy
A proxy for the `steam_api.dll` file used by Black Ops.


This works by having the user rename the original `steam_api.dll` to `o_steam_api.dll`, and then dropping in the proxy dll which would be named `steam_api.dll`.

[Custom-FX](https://github.com/BlackOpsOne/Custom-FX) utilizes something similar to this within the custom `steam_api.dll` file.