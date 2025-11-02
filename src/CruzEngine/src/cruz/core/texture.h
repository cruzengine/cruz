#pragma once
#include <string>
#include <vector>
#include <cruz/core/asset.h>
#include <glad/glad.h>

class Texture : public Asset {
public:
    Texture(const std::string& name, int width, int height, std::vector<unsigned char>&& data);

    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    const std::vector<unsigned char>& GetData() const { return m_data; }

    GLuint GetGPUHandle() const { return m_gpuHandle; }
    void SetGPUHandle(GLuint handle) { m_gpuHandle = handle; }

    static Texture* LoadFromPNG(const std::string& path, const std::string& name);

private:
    int m_width;
    int m_height;
    std::vector<unsigned char> m_data;
    GLuint m_gpuHandle = 0;
};