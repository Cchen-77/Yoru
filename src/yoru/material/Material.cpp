#include "Material.h"

std::shared_ptr<Material> Material::Create(const nlohmann::json &json) {
    return std::shared_ptr<Material>();
}