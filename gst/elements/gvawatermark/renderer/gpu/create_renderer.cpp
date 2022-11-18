/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include "renderer_gpu.h"
#include <iostream>
#include <mutex>

#define VAAF_API __attribute__((__visibility__("default")))

std::mutex g_mutex;

VAAF_API void * create_renderer(InferenceBackend::FourCC format,
                                                   std::shared_ptr<ColorConverter> converter,
                                                   InferenceBackend::MemoryType memory_type, int width, int height) {
    std::lock_guard<std::mutex> guard(g_mutex);
    gpu::draw::RendererNV12 *r = NULL;
    //std::cout << "===============create_renderer=====================" << std::endl;
    switch (format) {
    case InferenceBackend::FOURCC_BGRA:
    case InferenceBackend::FOURCC_BGRX:
    case InferenceBackend::FOURCC_BGR:
    case InferenceBackend::FOURCC_RGBA:
    case InferenceBackend::FOURCC_RGBX:
    case InferenceBackend::FOURCC_RGB:
	    break;
        //return std::unique_ptr<Renderer>(new gpu::draw::RendererBGR(converter, memory_type, width, height));
    case InferenceBackend::FOURCC_NV12:
	r = new gpu::draw::RendererNV12(converter, memory_type, width, height);
	break;
	//return std::unique_ptr<Renderer>(r);

    case InferenceBackend::FOURCC_I420:
	break;
        //return std::unique_ptr<Renderer>(new gpu::draw::RendererI420(converter, memory_type, width, height));
    default:
        throw std::runtime_error("Unsupported format");
    }
    //return std::unique_ptr<Renderer>(new gpu::draw::RendererI420(converter, memory_type, width, height));
    //std::cout << r << std::endl;
    return (void*)r;
}

VAAF_API void buffer_unmap(void* p) {
    std::lock_guard<std::mutex> guard(g_mutex);
    gpu::draw::RendererNV12 *r = (gpu::draw::RendererNV12 *)p;
    //std::cout << "===============buffer_unmap=====================" << std::endl;
    r->buffer_unmap();
}

VAAF_API void buffer_map(void* p, void *buffer, InferenceBackend::Image &image, WatermarkVideoInfo *info) {
    std::lock_guard<std::mutex> guard(g_mutex);
    gpu::draw::RendererNV12 *r = (gpu::draw::RendererNV12 *)p;
    //std::cout << "===============buffer_map =====================" << r << std::endl;
    r->buffer_map(buffer, image, info);
}

VAAF_API void draw_backend(void* p, std::vector<cv::Mat> &image_planes, std::vector<cv::gapi::wip::draw::Prim> &prims, uint64_t drm_format_modifier) {
    std::lock_guard<std::mutex> guard(g_mutex);
    gpu::draw::RendererNV12 *r = (gpu::draw::RendererNV12 *)p;
    //std::cout << "===============draw_backend=====================" << std::endl;
    r->draw_backend(image_planes, prims, drm_format_modifier);
}