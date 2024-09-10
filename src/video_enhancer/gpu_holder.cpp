#include "gpu_holder.hpp"
#include "gpu_unit.hpp"
#include "resource_holder.hpp"
#include <filesystem>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

using namespace video_enhancer;

gpu_holder::gpu_holder() = default;

void gpu_holder::register_gpu(const std::shared_ptr<gpu_unit>& unit_ptr)
{
    this->units_.push_back(unit_ptr);
}

void gpu_holder::set_resource_dir(const std::filesystem::path& path)
{
    this->resource_path_ = path;
}

