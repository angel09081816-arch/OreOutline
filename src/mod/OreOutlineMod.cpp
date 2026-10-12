#include "mod/OreOutlineMod.h"

#include <filesystem>

namespace ore_outline {

OreOutlineMod &OreOutlineMod::instance() {
    static OreOutlineMod instance;
    return instance;
}

OreOutlineMod::OreOutlineMod() : mSelf(*ll::mod::NativeMod::current()) {}

bool OreOutlineMod::load() {
    auto &self = getSelf();
    std::error_code error;
    std::filesystem::create_directories(self.getConfigDir(), error);
    if (error) {
        self.getLogger().error("Could not create config directory: {}", error.message());
        return false;
    }

    mConfigFile.emplace();
    if (!mConfigFile->load()) {
        self.getLogger().error("Could not load OreOutline config");
        return false;
    }
    mConfig = mConfigFile->value();
    self.getLogger().info("Loaded OreOutline settings");
    return true;
}

bool OreOutlineMod::enable() {
    if (!mConfig.enabled) {
        getSelf().getLogger().info("OreOutline is disabled in config");
        return true;
    }

    getSelf().getLogger().warn(
        "Ore scanning is not active: the verified Android SDK exposes no world/block read API or depth-tested world renderer");
    return true;
}

bool OreOutlineMod::disable() {
    return true;
}

bool OreOutlineMod::unload() {
    mConfigFile.reset();
    return true;
}

} // namespace ore_outline