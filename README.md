# OreOutline

OreOutline is an ARM64 LeviLauncher native-mod project targeting Minecraft Bedrock 1.26.50.

## Current Status

The package scaffold, loader registration, persisted settings, ARM64 build, and a 2D HUD status readout are implemented. Ore detection and through-block outlines are **not implemented**. When OreOutline is enabled, the readout reports that ore scanning is unavailable; it does not claim to detect or highlight ore.

The pinned `preloader-android` 0.2.2 public headers provide generic hooks/signature resolution and 2D HUD draw commands (`pl::modmenu::DrawCommand`), but no API to obtain the active world, block source, player position, or block positions. They also expose no per-frame game callback, camera/projection interface, depth-state control, or world-space renderer. The generic hook API requires a verified function address and calling convention; the SDK provides no supported Bedrock block-query/render hook for the target version. A 2D HUD overlay cannot supply ore positions or produce through-wall 3D outlines. This project does not use invented memory offsets or unverified game signatures.

**Concrete next step:** obtain a version-matched Preloader API or upstream-supported Bedrock interface that provides (1) a per-frame callback with safe access to the active world/block source and player position and (2) a world-render stage with camera/projection data and depth-tested line/box drawing. Then use that API to query nearby block positions and render ore bounds. Without the read and render interfaces together, actual ore outlines cannot be implemented or verified safely.

The manifest declares Minecraft 1.26.50.4 as the intended target using LeviLaunchroid's exact-version format; successful compilation does not establish in-game compatibility. The settings below are saved for future implementation and do not currently cause ore scanning or ore rendering.

## Settings

The generated config provides a master enable toggle, requested scan range in blocks, and enable/color settings for diamond ore, ancient debris, emerald ore, gold ore, iron ore, copper ore, and coal ore. These are saved for a future scanner; they do not currently scan blocks or affect the status readout. Colors are editable `#RRGGBB` strings in the mod's config settings.

## Build

Requirements: CMake 3.22+, Ninja, Android SDK, and Android NDK 28.2.13676358. The build pins the Android Preloader SDK to 0.2.2 and its config-generation dependencies to fixed tags.

```bash
export ANDROID_HOME="$HOME/Android/Sdk"
export ANDROID_NDK_HOME="$ANDROID_HOME/ndk/28.2.13676358"
bash scripts/build.sh arm64-v8a
```

The package is written to `build-arm64-v8a/ore_outline-0.1.0-arm64-v8a.levipack`. GitHub Actions runs the same build and uploads the `.levipack` as the `OreOutline-arm64-v8a-mc-1.26.50.4` artifact.

## Install In LeviLauncher

Import the `.levipack` file from LeviLauncher's mod manager/import screen, enable OreOutline, and launch the declared Minecraft version. The supported 2D HUD API is used to show a status message; no ore highlights should be expected. Do not use this build as a functional ore-finder mod.

## Verified Upstream References

- [Android native-mod template](https://github.com/QYCottage/levilauncher-android-mod-template)
- [LeviLaunchroid native-mod compatibility rules](https://github.com/LiteLDev/LeviLaunchroid/blob/main/docs/guide/developer.md#manifestjson)
- [PreLoader Android SDK 0.2.2](https://github.com/LiteLDev/preloader-android/tree/0.2.2)
- [PreLoader 0.2.2 Mod Menu/HUD API](https://github.com/LiteLDev/preloader-android/blob/0.2.2/include/pl/ModMenu.hpp)
- [LeviLauncher](https://github.com/LiteLDev/LeviLauncher)
