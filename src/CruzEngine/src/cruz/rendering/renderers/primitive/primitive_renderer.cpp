#include "primitive_renderer.h"

PrimitiveRenderer::PrimitiveRenderer(RenderingBackend* backend, Camera* cam)
    : m_backend(backend), camera(cam)
{
}

PrimitiveRenderer::~PrimitiveRenderer()
{
    delete m_shaderColor;
    delete m_shaderTexture;
}

void PrimitiveRenderer::Initialize()
{
    if (!m_backend) {
        std::cerr << "PrimitiveRenderer: Backend not set!\n";
        return;
    }

    const char* vertexSrcColor = R"(
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec4 aColor;
        uniform mat4 uProjection;
        out vec4 vColor;
        void main() {
            vColor = aColor;
            gl_Position = uProjection * vec4(aPos, 1.0);
        }
    )";

    const char* fragmentSrcColor = R"(
        precision mediump float;
        in vec4 vColor;
        out vec4 FragColor;
        void main() { FragColor = vColor; }
    )";

    m_shaderColor = m_backend->CreateShader(vertexSrcColor, fragmentSrcColor);

    const char* vertexSrcTex = R"(
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec4 aColor;
        layout(location = 2) in vec2 aTexCoord;
        uniform mat4 uProjection;
        out vec4 vColor;
        out vec2 vTexCoord;
        void main() {
            vColor = aColor;
            vTexCoord = aTexCoord;
            gl_Position = uProjection * vec4(aPos, 1.0);
        }
    )";

    const char* fragmentSrcTex = R"(
        precision mediump float;
        in vec4 vColor;
        in vec2 vTexCoord;
        uniform sampler2D uTexture;
        out vec4 FragColor;
        void main() {
            FragColor = texture(uTexture, vTexCoord) * vColor;
        }
    )";

    m_shaderTexture = m_backend->CreateShader(vertexSrcTex, fragmentSrcTex);
}

void PrimitiveRenderer::Clear(const std::array<float, 4>& color)
{
    if (!m_backend) return;
    m_backend->Clear(color.data());
}

void PrimitiveRenderer::SetPipeline(const PrimitiveSettings& settings)
{
    if (!m_backend) return;
    m_backend->SetPipeline({settings.depthTest, settings.blend});
}

void PrimitiveRenderer::BeginFrame()
{
    m_vertices.clear();
    m_texVertices.clear();
    m_currentTexture = nullptr;
}

void PrimitiveRenderer::EndFrame()
{
    Flush();
}

void PrimitiveRenderer::DrawQuad(float x, float y, float width, float height, const std::array<float, 4>& color)
{
    float z = 0.0f;
    m_vertices.push_back({x, y, z, color[0], color[1], color[2], color[3]});
    m_vertices.push_back({x + width, y, z, color[0], color[1], color[2], color[3]});
    m_vertices.push_back({x + width, y + height, z, color[0], color[1], color[2], color[3]});

    m_vertices.push_back({x, y, z, color[0], color[1], color[2], color[3]});
    m_vertices.push_back({x + width, y + height, z, color[0], color[1], color[2], color[3]});
    m_vertices.push_back({x, y + height, z, color[0], color[1], color[2], color[3]});
}

void PrimitiveRenderer::DrawTexturedQuad(float x, float y, float width, float height, Texture* texture)
{
    if (!texture) return;

    float hw = width * 0.5f;
    float hh = height * 0.5f;

    std::vector<TexturedVertex> verts = {
        TexturedVertex{ x - hw, y - hh, 0.0f, 1,1,1,1, 0, 1 }, // dolny lewy
        TexturedVertex{ x + hw, y - hh, 0.0f, 1,1,1,1, 1, 1 }, // dolny prawy
        TexturedVertex{ x + hw, y + hh, 0.0f, 1,1,1,1, 1, 0 }, // górny prawy

        TexturedVertex{ x - hw, y - hh, 0.0f, 1,1,1,1, 0, 1 }, // dolny lewy
        TexturedVertex{ x + hw, y + hh, 0.0f, 1,1,1,1, 1, 0 }, // górny prawy
        TexturedVertex{ x - hw, y + hh, 0.0f, 1,1,1,1, 0, 0 }  // górny lewy
    };

    m_texVertices.insert(m_texVertices.end(), verts.begin(), verts.end());

    m_currentTexture = texture;
}

void PrimitiveRenderer::DrawLine(float x1, float y1, float x2, float y2, const std::array<float, 4>& color)
{
    float z = 0.0f;
    m_vertices.push_back({x1, y1, z, color[0], color[1], color[2], color[3]});
    m_vertices.push_back({x2, y2, z, color[0], color[1], color[2], color[3]});
}

void PrimitiveRenderer::DrawPoint(float x, float y, float size, const std::array<float, 4>& color)
{
    DrawQuad(x - size / 2.0f, y - size / 2.0f, size, size, color);
}

void PrimitiveRenderer::Flush()
{
    if (!m_backend) return;

    if (!m_vertices.empty())
    {
        m_backend->UseShader(m_shaderColor);
        Mat4 vp = camera->GetVPMatrix();
        m_backend->SetUniformMat4(m_shaderColor, "uProjection", vp);
        m_backend->Draw(m_vertices);
        m_vertices.clear();
    }

    if (!m_texVertices.empty())
    {
        if (m_currentTexture)
            m_backend->BindTexture(m_currentTexture);

        m_backend->UseShader(m_shaderTexture);
        Mat4 vp = camera->GetVPMatrix();
        m_backend->SetUniformMat4(m_shaderTexture, "uProjection", vp);
        m_backend->Draw(m_texVertices);

        if (m_currentTexture)
            m_backend->UnbindTexture();

        m_texVertices.clear();
        m_currentTexture = nullptr;
    }
}
