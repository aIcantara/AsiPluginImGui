# plugin-sdk-cmake
Here is information about using [plugin-sdk](https://github.com/DK22Pac/plugin-sdk) in [CMake](https://cmake.org/).

## Build & Install
```sh
cd plugin-sdk-cmake
cmake . -B build ...
cmake --build build --target install --config release # or debug
```

### Build Options
Games:
- `PSDK_GTAIII`: Grand Theft Auto III (Default: `OFF`);  
Versions: `PSDK_VER_10EN` (Default: `ON`), `PSDK_VER_11EN` (Default: `OFF`), `PSDK_VER_STEAM` (Default: `OFF`);
- `PSDK_GTASA`: Grand Theft Auto: San Anreas (Default: `OFF`);  
Versions: `PSDK_VER_10US` (Default: `ON`), `PSDK_VER_10EU` (Default: `OFF`), `PSDK_VER_11US` (Default: `OFF`), `PSDK_VER_11EU` (Default: `OFF`), `PSDK_VER_SR2` (Default: `OFF`), `PSDK_VER_SR2LV` (Default: `OFF`);
- `PSDK_GTAVC`: Grand Theft Auto: Vice City (Default: `OFF`);  
Versions: `PSDK_VER_10EN` (Default: `ON`), `PSDK_VER_11EN` (Default: `OFF`), `PSDK_VER_STEAM` (Default: `OFF`);

Other:
- `PSDK_EXAMPLES`: Build examples (Default: `ON`);
- `PSDK_INSTALL`: Install the library (Default: `ON`);
- `PSDK_DXSDK` Enable DirectX 9 SDK (Default: `OFF`);  
`PSDK_DXSDK_DIR` DirectX9 SDK directory (Default: `DIRECTX9_SDK_DIR` from Environment Variables);

## Targets
There is 4 targets:
- `pluginsdk::gta3` / `pluginsdk::gtasa` / `pluginsdk::gtavc`: the libraries for a specific game;
- `pluginsdk::shared`: the basic library;

## Using
```cmake
find_package(plugin-sdk REQUIRED)
target_link_libraries(proj PRIVATE pluginsdk::gtasa)
```

## Alternatives
You can use [`FindPluginSDK.cmake`](https://gist.github.com/THE-FYP/104855c5fdd7311a336a5178a0b33118) if you want to use a different version of Plugin-SDK.