/*******************************************************************************
 * Copyright (C) 2020-2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#pragma once

#include "color_converter.h"

#include "gva_buffer_map.h"
#include "inference_backend/image.h"

#include <gst/video/video-info.h>

#include <opencv2/gapi/render/render_types.hpp>

#include <unistd.h>

#include <memory>
#include <stdexcept>
#include <vector>

namespace gapidraw = cv::gapi::wip::draw;

typedef struct _WatermarkVideoInfo
{
    void *vaDisplay;
    unsigned int surfaceId;
    unsigned int width;
    unsigned int height;
} WatermarkVideoInfo;

class Renderer
{
  public:
    void draw(void *buffer, WatermarkVideoInfo *info,
              std::vector<gapidraw::Prim> prims);

    virtual void buffer_map(void *buffer, InferenceBackend::Image &image,
                           WatermarkVideoInfo *info) = 0;

    virtual void buffer_unmap() = 0;

    virtual ~Renderer() = default;

    std::function<void()> m_unmapFunc;
    std::function<void(void *buffer, InferenceBackend::Image &image,
                       WatermarkVideoInfo *info)>
        m_mapFunc;
    std::function<void(std::vector<cv::Mat> &imagePlanes,
                       std::vector<cv::gapi::wip::draw::Prim> &prims,
                       uint64_t drmFormatModifier)>
        m_drawFunc;

  protected:
    std::shared_ptr<ColorConverter> m_colorConverter;
    InferenceBackend::MemoryType m_memoryType;

    Renderer(std::shared_ptr<ColorConverter> colorConverter,
             InferenceBackend::MemoryType memoryType)
        : m_colorConverter(colorConverter), m_memoryType(memoryType)
    {
    }

    void convert_prims_color(std::vector<gapidraw::Prim> &prims);

    virtual void draw_backend(std::vector<cv::Mat> &imagePlanes,
                             std::vector<gapidraw::Prim> &prims,
                             uint64_t drmFormatModifier = 0) = 0;

  private:
    static int FourccToOpenCVMatType(int fourcc);

    static std::vector<cv::Mat>
    ConvertImageToMat(const InferenceBackend::Image &image);
};
