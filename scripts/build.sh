#!/usr/bin/env bash
set -euo pipefail

root_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
abi="${1:-arm64-v8a}"
if [[ "$abi" != "arm64-v8a" ]]; then
    printf 'Only arm64-v8a is supported (requested: %s)\n' "$abi" >&2
    exit 2
fi

ndk_home="${ANDROID_NDK_HOME:-${ANDROID_NDK_ROOT:-}}"
if [[ -z "$ndk_home" && -n "${ANDROID_HOME:-}" ]]; then
    ndk_home="$ANDROID_HOME/ndk/28.2.13676358"
fi
if [[ -z "$ndk_home" || ! -f "$ndk_home/build/cmake/android.toolchain.cmake" ]]; then
    printf 'Set ANDROID_NDK_HOME to Android NDK 28.2.13676358.\n' >&2
    exit 2
fi

cmake -S "$root_dir" -B "$root_dir/build-config-ninja" -G Ninja
cmake --build "$root_dir/build-config-ninja" --target ore_generate_config

cmake -S "$root_dir" -B "$root_dir/build-$abi" -G Ninja \
    -DCMAKE_TOOLCHAIN_FILE="$ndk_home/build/cmake/android.toolchain.cmake" \
    -DANDROID_ABI="$abi" \
    -DANDROID_PLATFORM=android-28 \
    -DANDROID_STL=c++_shared \
    -DMOD_MINECRAFT_VERSIONS='["1.26.50.4"]' \
    -DLEVI_PACKAGE_CONFIG_DIR="$root_dir/build-config-ninja/generated-config"
cmake --build "$root_dir/build-$abi" --target levi_package

printf 'Package: %s\n' "$root_dir/build-$abi/ore_outline-0.1.0-$abi.levipack"