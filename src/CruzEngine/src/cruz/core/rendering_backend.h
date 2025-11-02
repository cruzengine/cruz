#pragma once

#include <cruz/core/mat4.h>
#include <cruz/core/shader.h>
#include <cruz/core/platform.h>
#include <cruz/core/vertex.h>
#include <cruz/core/texture.h>
#include <vector>
#include <string>

struct PipelineSettings {
    bool depthTest = true;
    bool blend = false;
};

class RenderingBackend
{
public:
    virtual ~RenderingBackend() = default;

    virtual void Initialize() = 0;
    virtual void Resize(int width, int height) = 0;
    virtual void Update(float deltaTime) = 0;

    virtual void Clear(const float color[4]) = 0;
    virtual void SetViewport(int x, int y, int width, int height) = 0;
    virtual void SetPipeline(const PipelineSettings& settings) = 0;

    virtual Shader* CreateShader(const std::string& vertexSrc, const std::string& fragmentSrc) = 0;
    virtual void UseShader(Shader* shader) = 0;
    virtual void SetUniformMat4(Shader* shader, const std::string& name, const Mat4& mat) = 0;

    virtual void Draw(const std::vector<Vertex>& vertices) = 0;
    virtual void Draw(const std::vector<ColoredVertex>& vertices) = 0;
    virtual void Draw(const std::vector<TexturedVertex>& vertices) = 0;

    virtual void UploadVertices(const std::vector<Vertex>& vertices) = 0;
    virtual void DrawUploadedVertices() = 0;

    virtual void BindTexture(Texture* texture) = 0;
    virtual void UnbindTexture() = 0;

    virtual void UploadTexture(Texture* texture) = 0;

    void SetPlatform(Platform* platform) { this->platform = platform; }
    Platform* GetPlatform() { return this->platform; }
    virtual const Mat4& GetProjection() const = 0;

protected:
    Platform* platform = nullptr;
};