#pragma once
#include <cruz/core/rendering_backend.h>
#include <cruz/core/camera.h>
#include <cruz/core/mat4.h>
#include <cruz/core/vertex.h>
#include <vector>
#include <array>
#include <cruz/core/texture.h>
#include <iostream>

struct PrimitiveSettings {
    bool blend = true;
    bool depthTest = false;
};

class PrimitiveRenderer {
public:
    PrimitiveRenderer(RenderingBackend* backend, Camera* cam);
    ~PrimitiveRenderer();

    void Initialize();

    void Clear(const std::array<float, 4>& color);
    void SetPipeline(const PrimitiveSettings& settings);

    void BeginFrame();
    void EndFrame();

    void DrawQuad(float x, float y, float width, float height, const std::array<float, 4>& color);
    void DrawTexturedQuad(float x, float y, float width, float height, Texture* texture);
    void DrawLine(float x1, float y1, float x2, float y2, const std::array<float, 4>& color);
    void DrawPoint(float x, float y, float size, const std::array<float, 4>& color);

private:
    void Flush();

    RenderingBackend* m_backend = nullptr;
    Camera* camera = nullptr;

    Shader* m_shaderColor = nullptr;
    Shader* m_shaderTexture = nullptr;

    std::vector<ColoredVertex> m_vertices;
    std::vector<TexturedVertex> m_texVertices;

    Texture* m_currentTexture = nullptr;
};
