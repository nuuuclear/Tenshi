#pragma once

namespace Tenshi {
namespace INTERNAL {

void buildConfig(YamlDocument document, GameConfig& config) {
    const auto& yamlRoot = document.GetRoot();

    config.title
    =   yamlRoot["game"]["title"].get_value<std::string>();

    config.windowWidth
    =   yamlRoot["window"]["width"].get_value<int>();

    config.windowHeight
    =   yamlRoot["window"]["height"].get_value<int>();

    config.windowScale
    =   yamlRoot["window"]["scale"].get_value<int>();

    config.windowResizable
    =   yamlRoot["window"]["resizable"].get_value<bool>();
}

} // namespace INTERNAL
} // namespace Tenshi