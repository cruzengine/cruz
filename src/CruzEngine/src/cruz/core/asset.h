#pragma once
#include <string>

class Asset {
public:
    Asset(const std::string& name);
    virtual ~Asset();

    const std::string& GetName() const;

private:
    std::string m_name;
};