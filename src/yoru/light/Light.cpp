#include "Light.h"

std::shared_ptr<Light> Light::Create(const nlohmann::json &json) {
    return std::shared_ptr<Light>();
}