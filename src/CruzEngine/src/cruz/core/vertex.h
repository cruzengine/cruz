#pragma once

struct Vertex {
    float x, y, z;
};

struct ColoredVertex {
    float x, y, z;
    float r, g, b, a;
};

struct TexturedVertex {
    float x, y, z;
    float r, g, b, a;
    float u, v;

    TexturedVertex() = default;
    TexturedVertex(float px, float py, float pz,
                   float pr, float pg, float pb, float pa,
                   float pu, float pv)
        : x(px), y(py), z(pz), r(pr), g(pg), b(pb), a(pa), u(pu), v(pv) {}
};