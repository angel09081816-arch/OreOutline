#include "mod/Config.h"

namespace ore_outline {

nlohmann::json makeDefaultConfigJson() {
    return pl::config::defaultJson(OreOutlineConfig{});
}

nlohmann::json makeConfigSchemaJson() {
    return pl::config::schema(OreOutlineConfig{});
}

} // namespace ore_outline