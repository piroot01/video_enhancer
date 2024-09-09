#include <cstdint>
#include <filesystem>

#include "gpu_unit.hpp"

using namespace video_enhancer;

gpu_unit::gpu_unit(const std::uint32_t id) :
    id_(id)
{
}

void gpu_unit::execute_image(const std::filesystem::path& input, const std::filesystem::path& output)
{
}
