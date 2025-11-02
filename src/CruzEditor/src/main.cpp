#include <cruz/core/application.h>
#include <cruz/rendering/renderers/primitive/primitive_renderer.h>
#include <cruz/core/time.h>
#include <cruz/core/orthographic_camera.h>
#include <cmath>
#include <cruz/core/vector.h>
#include <iostream>

int main() {
    Application app(800, 600, "Primitive Example");

    auto backend = app.GetBackend();
    auto platform = backend->GetPlatform();

    int w, h;
    platform->GetFramebufferSize(w, h);

    OrthographicCamera camera{ float(w), float(h) };
    camera.SetPosition(0.0f, 0.0f);
    camera.orthoSize = 300.0f;

    PrimitiveRenderer primitive(backend, &camera);
    primitive.Initialize();

    platform->AddResizeCallback([&](int newW, int newH){
        camera.width  = float(newW);
        camera.height = float(newH);
    });

    Texture* texture = Texture::LoadFromPNG("asd.png", "ASDTexture");
    if (!texture) {
        std::cerr << "Failed to load texture!" << std::endl;
        return -1;
    }
    backend->UploadTexture(texture);

    Vector2 quadPos(0.0f, 0.0f);
    Vector2 quadVel(0.0f, 0.0f);
    const float accel = 600.0f;
    const float damping = 8.0f;
    const float maxSpeed = 400.0f;

    app.SetUpdateCallback([&](float dt) {
        primitive.BeginFrame();
        primitive.Clear({0.2f, 0.2f, 0.25f, 1.0f});

        PrimitiveSettings settings;
        settings.blend = true;
        settings.depthTest = false;
        primitive.SetPipeline(settings);

        Vector2 inputAccel(0.0f, 0.0f);
        if (platform->GetKey(KeyCode::W)) inputAccel.y += accel;
        if (platform->GetKey(KeyCode::S)) inputAccel.y -= accel;
        if (platform->GetKey(KeyCode::D)) inputAccel.x += accel;
        if (platform->GetKey(KeyCode::A)) inputAccel.x -= accel;

        quadVel += inputAccel * dt;
        float speed = quadVel.length();
        if (speed > maxSpeed) quadVel = quadVel.normalized() * maxSpeed;
        quadVel -= quadVel * damping * dt;
        quadPos += quadVel * dt;

        backend->BindTexture(texture);
        primitive.DrawTexturedQuad(quadPos.x, quadPos.y, 100.0f, 100.0f, texture);
        backend->UnbindTexture();

        primitive.EndFrame();
    });

    app.Run();
    return 0;
}