#pragma once
#include <cruz/core/platform.h>
#include <string>
#include <GLFW/glfw3.h>
#include <functional>
#include <iostream>

class WindowsPlatform : public Platform {
public:
    WindowsPlatform(int width, int height, const std::string& title);
    ~WindowsPlatform();

    bool WindowShouldClose() override;
    void SetWindowShouldClose(bool value) override;
    void PollEvents() override;
    void SwapBuffers() override;
    void GetFramebufferSize(int& width, int& height) override;
    void MakeContextCurrent() override;
    int GetKeyPressed(int key) override;
    void* GetProcAddress() override;
    uint64_t AddResizeCallback(std::function<void(int,int)> func) override;
    void RemoveResizeCallback(uint64_t id) override;
    bool GetKeyDown(KeyCode key) override;
    bool GetKeyUp(KeyCode key) override;
    bool GetKey(KeyCode key) override;


    GLFWwindow* GetWindow() { return window; }

private:
    void InitializeWindow(int width, int height, const std::string& title);
    int ToGLFWKey(KeyCode key);

    GLFWwindow* window = nullptr;
    std::function<void(int,int)> resizeCallback;
};