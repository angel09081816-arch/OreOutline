# OreOutline

OreOutline is an ARM64 LeviLauncher native-mod project targeting Minecraft Bedrock 1.26.50.

## Current Status

The package scaffold, loader registration, persisted settings, and ARM64 build are implemented. Ore detection and through-block outlines are **not implemented**. The verified `preloader-android` 0.2.2 public SDK has generic function/signature hooks and 2D HUD draw commands, but no public world/block read API or depth-tested world-space renderer. A 2D HUD overlay cannot provide reliable ore positions or through-wall 3D outlines. This project does not use invented memory offsets or unverified game signatures.

The manifest declares Minecraft 1.26.50 as the intended target; successful compilation does not establish in-game compatibility. The settings below are saved for future implementation and do not currently cause scanning or rendering.

## Settings

The generated config provides a master enable toggle, scan range in blocks, and enable/color settings for diamond ore, ancient debris, emerald ore, gold ore, iron ore, copper ore, and coal ore. Colors are editable `#RRGGBB` strings in the mod's config settings.

## Build

Requirements: CMake 3.22+, Ninja, Android SDK, and Android NDK 28.2.13676358. The build pins the Android Preloader SDK to 0.2.2 and its config-generation dependencies to fixed tags.

```bash
export ANDROID_HOME="$HOME/Android/Sdk"
export ANDROID_NDK_HOME="$ANDROID_HOME/ndk/28.2.13676358"
bash scripts/build.sh arm64-v8a
```

The package is written to `build-arm64-v8a/ore_outline-0.1.0-arm64-v8a.levipack`. GitHub Actions runs the same build and uploads the `.levipack` as an artifact.

## Install In LeviLauncher

Import the `.levipack` file from LeviLauncher's mod manager/import screen, enable OreOutline, and launch the declared Minecraft version. In the current scaffold, the mod logs that ore scanning is unavailable; no ore highlights should be expected. Do not use this build as a functional ore-finder mod.

## Verified Upstream References

- [Android native-mod template](https://github.com/QYCottage/levilauncher-android-mod-template)
- [PreLoader Android SDK 0.2.2](https://github.com/LiteLDev/preloader-android/tree/0.2.2)
- [LeviLauncher](https://github.com/LiteLDev/LeviLauncher)