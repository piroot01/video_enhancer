#ifndef __VIDEO_ENHANCER_GPU_UNIT_HPP__
#define __VIDEO_ENHANCER_GPU_UNIT_HPP__

#include <cstdint>
#include <filesystem>
#include <mutex>

namespace video_enhancer
{
    class gpu_unit
    {
    public:
        gpu_unit(const std::uint32_t id);

    public:
        void set_output_dir(const std::filesystem::path& output_dir);

    public:
        void execute(const std::filesystem::path& input);

    private:
        const std::uint32_t id_;

        std::filesystem::path output_dir_ = {};

    };

} // namespace video_enhancer

#endif
