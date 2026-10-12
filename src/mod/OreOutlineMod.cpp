#include "mod/OreOutlineMod.h"

#include <array>
#include <filesystem>

#include <pl/ModMenu.hpp>

namespace ore_outline {

namespace {

constexpr char kStatusModuleId[] = "ore_outline_status";

} // namespace

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

    auto statusModule =
        pl::modmenu::ModuleBuilder(kStatusModuleId, "OreOutline Status")
            .description("Shows whether ore scanning is available.")
            .defaultEnabled(true);
    if (!statusModule.registerModule()) {
        getSelf().getLogger().error("Could not register OreOutline status overlay");
        return false;
    }
    mStatusOverlayRegistered = true;

    const std::array statusCommands{
        pl::modmenu::DrawCommand{
            .type = pl::modmenu::DrawCommandType::Text,
            .x = 12.0F,
            .y = 12.0F,
            .color = 0xFFFFFFFFU,
            .size = 16.0F,
            .text = "OreOutline: ore scanning unavailable",
        },
    };
    pl::modmenu::submitDrawCommands(kStatusModuleId, statusCommands);
    getSelf().getLogger().warn(
        "Ore scanning is unavailable: Preloader exposes no world/block read API or world-space renderer");
    return true;
}

bool OreOutlineMod::disable() {
    if (mStatusOverlayRegistered) {
        pl::modmenu::unregisterModule(kStatusModuleId);
        mStatusOverlayRegistered = false;
    }
    return true;
}

bool OreOutlineMod::unload() {
    if (mStatusOverlayRegistered) {
        pl::modmenu::unregisterModule(kStatusModuleId);
        mStatusOverlayRegistered = false;
    }
    mConfigFile.reset();
    return true;
}

} // namespace ore_outline