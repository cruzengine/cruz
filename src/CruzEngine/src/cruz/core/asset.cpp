#include "asset.h"

Asset::Asset(const std::string& name)
    : m_name(name)
{
}

Asset::~Asset() = default;

const std::string& Asset::GetName() const {
    return m_name;
}