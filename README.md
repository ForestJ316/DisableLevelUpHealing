### For Game Version 1.10.984
## Requirements
* [XMake](https://xmake.io) [2.8.2+]
* [Visual Studio Community 2022](https://visualstudio.microsoft.com/)
* C++23 Compiler (MSVC or Clang-CL)

## Building
```
git clone --recurse-submodules https://github.com/ForestJ316/DisableLevelUpHealing/tree/NGVersion
cd DisableLevelUpHealing

xmake build
```
> *This will generate a `build/windows/` directory in the **project's root directory** with the build output.*

### Project Generation (Optional)
If you want to generate a Visual Studio project, run the following command:
```bat
xmake project -k vsxmake
```
> *This will generate a `vsxmakeXXXX/` directory in the **project's root directory** using the latest version of Visual Studio installed on the system.*

## Optional
If you want to redirect the build output, set one of the following environment variables:

* Path to a [Fallout 4](https://store.steampowered.com/app/377160) install folder: `XSE_FO4_GAME_PATH`

* Path to a [Mod Manager](https://www.modorganizer.org/) mods folder: `XSE_FO4_MODS_PATH`
