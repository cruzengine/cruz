#include "texture.h"
#include <png.h>
#include <stdexcept>
#include <iostream>
#include <cstdio>

Texture::Texture(const std::string& name, int width, int height, std::vector<unsigned char>&& data)
    : Asset(name), m_width(width), m_height(height), m_data(std::move(data)), m_gpuHandle(0) {}

Texture* Texture::LoadFromPNG(const std::string& path, const std::string& name) {
    FILE* fp = nullptr;
    fopen_s(&fp, path.c_str(), "rb");
    if (!fp) {
        std::cerr << "Failed to open PNG file: " << path << std::endl;
        return nullptr;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) { fclose(fp); return nullptr; }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) { png_destroy_read_struct(&png_ptr, nullptr, nullptr); fclose(fp); return nullptr; }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        fclose(fp);
        return nullptr;
    }

    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);

    int width      = png_get_image_width(png_ptr, info_ptr);
    int height     = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth  = png_get_bit_depth(png_ptr, info_ptr);

    if(bit_depth == 16) png_set_strip_16(png_ptr);
    if(color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png_ptr);
    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png_ptr);
    if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png_ptr);
    if(color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY) png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
    if(color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) png_set_gray_to_rgb(png_ptr);

    png_read_update_info(png_ptr, info_ptr);

    std::vector<unsigned char> data(width * height * 4);
    std::vector<png_bytep> row_pointers(height);
    for(int y=0; y<height; ++y) {
        row_pointers[y] = data.data() + y*width*4;
    }

    png_read_image(png_ptr, row_pointers.data());

    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(fp);

    return new Texture(name, width, height, std::move(data));
}