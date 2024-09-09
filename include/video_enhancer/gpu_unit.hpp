#ifndef __VIDEO_ENHANCER_GPU_UNIT_HPP__
#define __VIDEO_ENHANCER_GPU_UNIT_HPP__

#include <cstdint>
#include <filesystem>

namespace video_enhancer
{
    class gpu_unit
    {
    public:
        gpu_unit(const std::uint32_t id);

    public:
        void execute_image(const std::filesystem::path& input, const std::filesystem::path& output);

    private:
        const std::uint32_t id_;

    };

} // namespace video_enhancer

#endif
