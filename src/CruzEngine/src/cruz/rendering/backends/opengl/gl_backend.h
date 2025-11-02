#pragma once
#include <cruz/core/rendering_backend.h>
#include <cruz/core/mat4.h>
#include <cruz/core/texture.h>
#include <vector>
#include <glad/glad.h>
#include <chrono>
#include <cruz/core/time.h>

class GlBackend : public RenderingBackend {
public:
    void Initialize() override;
    void Resize(int width, int height) override;
    void Update(float deltaTime) override;
    void Clear(const float color[4]) override;
    void SetViewport(int x, int y, int width, int height) override;
    void SetPipeline(const PipelineSettings& settings) override;

    Shader* CreateShader(const std::string& vertexSrc, const std::string& fragmentSrc) override;
    void UseShader(Shader* shader) override;
    void SetUniformMat4(Shader* shader, const std::string& name, const Mat4& mat) override;

    void Draw(const std::vector<Vertex>& vertices) override;
    void Draw(const std::vector<ColoredVertex>& vertices) override;
    void Draw(const std::vector<TexturedVertex>& vertices) override;

    void UploadVertices(const std::vector<Vertex>& vertices) override;
    void DrawUploadedVertices() override;

    void BindTexture(Texture* texture) override;
    void UnbindTexture() override;

    void UploadTexture(Texture* texture) override;

    const Mat4& GetProjection() const override { return projection; }

    ~GlBackend();

private:
    Mat4 projection;
    GLuint vao = 0;
    GLuint vbo = 0;
    GLsizei vertexCount = 0;
    bool coloredVAO = false;
    unsigned int m_currentBoundTexture = 0;

    GLuint m_texVAO = 0;
    GLuint m_texVBO = 0;
};