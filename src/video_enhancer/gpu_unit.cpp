#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <mutex>
#include <string>
#include <iostream>
#include <thread>
#include <unistd.h>

#include "gpu_unit.hpp"
#include "config.hpp"

using namespace video_enhancer;

static std::mutex cout_mutex;

gpu_unit::gpu_unit(const std::uint32_t id) :
    id_(id)
{
}

void gpu_unit::set_output_dir(const std::filesystem::path& output_dir)
{
    this->output_dir_ = output_dir;
}

void gpu_unit::execute(const std::filesystem::path& input)
{
    const std::string output = this->output_dir_.string() + "/" + input.filename().string();

    const std::string output_file = "/dev/null";
/*
    {
        std::lock_guard guard(::cout_mutex);

        std::cout << "[gpu id " << this->id_ << "]: Enhancing image " << input.filename().string() << ".\n";
    }
*/
    const std::string command = detail::enhancer_path +
        " -n " + detail::model_name +
        " -f " + "png" +
        " -i " + input.string() +
        " -o " + output +
        " -g " + std::to_string(this->id_) +
        " &> " + output_file;

    std::system(command.c_str());
    //std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}
