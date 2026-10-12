#pragma once

#include <string>
#include <string_view>

#include <pl/Config.hpp>

namespace ore_outline {

struct OreOutlineConfig {
    int version = 1;
    bool enabled = true;
    int range = 32;
    bool diamond_enabled = true;
    std::string diamond_color = "#43D9E8";
    bool ancient_debris_enabled = true;
    std::string ancient_debris_color = "#D97542";
    bool emerald_enabled = true;
    std::string emerald_color = "#35C878";
    bool gold_enabled = true;
    std::string gold_color = "#F2C94C";
    bool iron_enabled = true;
    std::string iron_color = "#D9DDE3";
    bool copper_enabled = true;
    std::string copper_color = "#D9825B";
    bool coal_enabled = true;
    std::string coal_color = "#707780";
};

nlohmann::json makeDefaultConfigJson();
nlohmann::json makeConfigSchemaJson();

} // namespace ore_outline

template <> struct pl::config::Schema<ore_outline::OreOutlineConfig> {
    static constexpr std::string_view title = "OreOutline Settings";
    static constexpr std::string_view description = "Detection settings are saved, but ore scanning is not available in the verified SDK.";

    static constexpr pl::config::FieldSchema field(std::string_view name) {
        if (name == "version")
            return {.title = "Version", .readOnly = true};
        if (name == "enabled")
            return {.title = "Enabled", .description = "Master enable toggle."};
        if (name == "range")
            return {.title = "Range", .description = "Requested scan radius in blocks."};
        if (name == "diamond_enabled")
            return {.title = "Diamond Ore"};
        if (name == "diamond_color")
            return {.title = "Diamond Color", .description = "Hex color, for example #43D9E8."};
        if (name == "ancient_debris_enabled")
            return {.title = "Ancient Debris"};
        if (name == "ancient_debris_color")
            return {.title = "Ancient Debris Color", .description = "Hex color, for example #D97542."};
        if (name == "emerald_enabled")
            return {.title = "Emerald Ore"};
        if (name == "emerald_color")
            return {.title = "Emerald Color", .description = "Hex color, for example #35C878."};
        if (name == "gold_enabled")
            return {.title = "Gold Ore"};
        if (name == "gold_color")
            return {.title = "Gold Color", .description = "Hex color, for example #F2C94C."};
        if (name == "iron_enabled")
            return {.title = "Iron Ore"};
        if (name == "iron_color")
            return {.title = "Iron Color", .description = "Hex color, for example #D9DDE3."};
        if (name == "copper_enabled")
            return {.title = "Copper Ore"};
        if (name == "copper_color")
            return {.title = "Copper Color", .description = "Hex color, for example #D9825B."};
        if (name == "coal_enabled")
            return {.title = "Coal Ore"};
        if (name == "coal_color")
            return {.title = "Coal Color", .description = "Hex color, for example #707780."};
        return {};
    }
};