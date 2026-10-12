#include "mod/Config.h"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace {

bool writeJson(const std::filesystem::path &path, const nlohmann::json &value) {
    std::error_code error;
    std::filesystem::create_directories(path.parent_path(), error);
    if (error) {
        std::cerr << "Could not create " << path.parent_path() << ": " << error.message() << '\n';
        return false;
    }

    std::ofstream stream(path, std::ios::binary | std::ios::trunc);
    if (!stream) {
        std::cerr << "Could not open " << path << '\n';
        return false;
    }
    stream << value.dump(2) << '\n';
    return stream.good();
}

} // namespace

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: ore_config_generator <output-config-dir>\n";
        return 2;
    }

    const std::filesystem::path output = argv[1];
    if (!writeJson(output / "config.json", ore_outline::makeDefaultConfigJson()) ||
        !writeJson(output / "config.schema.json", ore_outline::makeConfigSchemaJson()))
        return 1;
    return 0;
}