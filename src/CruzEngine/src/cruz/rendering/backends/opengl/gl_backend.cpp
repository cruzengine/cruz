#include "gl_backend.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cruz/rendering/backends/opengl/gl_shader.h>
#include <iostream>

void GlBackend::Initialize() {
    if (!platform) { 
        std::cerr << "Platform not set!\n"; 
        return; 
    }
    platform->MakeContextCurrent();
    if (!gladLoadGLLoader((GLADloadproc)platform->GetProcAddress())) {
        std::cerr << "Failed to initialize GLAD\n"; 
        return;
    }
    glEnable(GL_MULTISAMPLE);

    glGenVertexArrays(1, &m_texVAO);
    glGenBuffers(1, &m_texVBO);

    platform->AddResizeCallback([this](int w,int h){ 
        glViewport(0,0,w,h);
    });
}

void GlBackend::Resize(int width, int height) {
    if(width <= 0 || height <= 0) return;
    glViewport(0,0,width,height);
}

void GlBackend::Update(float dt) {
    if(!platform) return;
    if(platform->WindowShouldClose()) return;
    if(platform->GetKeyPressed(GLFW_KEY_ESCAPE)) platform->SetWindowShouldClose(true);
}

void GlBackend::Clear(const float color[4]) {
    glClearColor(color[0],color[1],color[2],color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GlBackend::SetViewport(int x,int y,int w,int h) { 
    glViewport(x,y,w,h); 
}

void GlBackend::SetPipeline(const PipelineSettings& s) {
    if(s.depthTest) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
    if(s.blend){ glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); }
    else glDisable(GL_BLEND);
}

Shader* GlBackend::CreateShader(const std::string& vs,const std::string& fs){
    GLShader* shader = new GLShader();
    shader->Compile(vs,fs);
    return shader;
}

void GlBackend::UseShader(Shader* shader){
    if(!shader) return;
    GLShader* glShader = dynamic_cast<GLShader*>(shader);
    if(glShader) glShader->Use();
}

void GlBackend::SetUniformMat4(Shader* shader,const std::string& name,const Mat4& mat){
    if(!shader) return;
    GLShader* glShader = dynamic_cast<GLShader*>(shader);
    if(glShader) glShader->SetUniformMat4(name, mat.data);
}

void GlBackend::Draw(const std::vector<Vertex>& verts){
    if(verts.empty()) return;
    UploadVertices(verts);
    coloredVAO = false;
    DrawUploadedVertices();
}

void GlBackend::Draw(const std::vector<ColoredVertex>& verts){
    if(verts.empty()) return;

    if(vao!=0){ glDeleteBuffers(1,&vbo); glDeleteVertexArrays(1,&vao); }

    glGenVertexArrays(1,&vao);
    glGenBuffers(1,&vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size()*sizeof(ColoredVertex), verts.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(ColoredVertex),(void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,sizeof(ColoredVertex),(void*)offsetof(ColoredVertex,r));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    vertexCount = GLsizei(verts.size());
    coloredVAO = true;
    DrawUploadedVertices();
}

void GlBackend::Draw(const std::vector<TexturedVertex>& vertices)
{
    if(vertices.empty()) return;

    glBindVertexArray(m_texVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_texVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TexturedVertex), vertices.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)(offsetof(TexturedVertex, r)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)(offsetof(TexturedVertex, u)));
    glEnableVertexAttribArray(2);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}


void GlBackend::UploadVertices(const std::vector<Vertex>& verts){
    if(verts.empty()) return;
    if(vao!=0){ glDeleteBuffers(1,&vbo); glDeleteVertexArrays(1,&vao); }

    glGenVertexArrays(1,&vao);
    glGenBuffers(1,&vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size()*sizeof(Vertex), verts.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    vertexCount = GLsizei(verts.size());
    coloredVAO=false;
}

void GlBackend::DrawUploadedVertices(){
    if(vao==0) return;
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES,0,vertexCount);
    glBindVertexArray(0);
}

void GlBackend::UploadTexture(Texture* texture)
{
    if (!texture) return;

    if (texture->GetGPUHandle() != 0) return; // już przesłane

    GLuint handle;
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->GetWidth(), texture->GetHeight(),
                 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->GetData().data());

    glBindTexture(GL_TEXTURE_2D, 0);

    texture->SetGPUHandle(handle);
}

void GlBackend::BindTexture(Texture* texture)
{
    if (!texture) {
        glBindTexture(GL_TEXTURE_2D, 0);
        m_currentBoundTexture = 0;
        return;
    }

    if (texture->GetGPUHandle() == 0)
        UploadTexture(texture);

    if (m_currentBoundTexture != texture->GetGPUHandle())
    {
        glBindTexture(GL_TEXTURE_2D, texture->GetGPUHandle());
        m_currentBoundTexture = texture->GetGPUHandle();
    }
}

void GlBackend::UnbindTexture()
{
    if (m_currentBoundTexture != 0)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        m_currentBoundTexture = 0;
    }
}

GlBackend::~GlBackend(){
    if(vao!=0) glDeleteVertexArrays(1,&vao);
    if(vbo!=0) glDeleteBuffers(1,&vbo);
}