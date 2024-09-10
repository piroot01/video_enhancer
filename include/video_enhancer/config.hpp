#ifndef __VIDEO_ENHANCER_CONFIG_HPP__
#define __VIDEO_ENHANCER_CONFIG_HPP__

#include <string>

namespace video_enhancer::detail
{
    const static std::string file_name_format = "%08d";

    const static std::string enhancer_path = "/usr/bin/realesrgan-ncnn-vulkan";

    const static std::string model_name = "realesrgan-x4plus-anime";

} // namespace video_enhancer

#endif
