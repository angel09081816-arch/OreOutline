#pragma once

#include <optional>

#include "mod/Config.h"

#include <pl/Mod.hpp>

namespace ore_outline {

class OreOutlineMod {
  public:
    static OreOutlineMod &instance();

    OreOutlineMod();

    [[nodiscard]] ll::mod::NativeMod &getSelf() const { return mSelf; }

    bool load();
    bool enable();
    bool disable();
    bool unload();

  private:
    ll::mod::NativeMod &mSelf;
    OreOutlineConfig mConfig;
    std::optional<pl::config::ConfigFile<OreOutlineConfig>> mConfigFile;
};

} // namespace ore_outline