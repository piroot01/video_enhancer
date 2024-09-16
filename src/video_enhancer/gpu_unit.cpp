#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <mutex>
#include <string>
#include <iostream>
#include <vector>

#include "gpu_unit.hpp"
#include "config.hpp"

#include <boost/process.hpp>

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

    boost::process::ipstream err_stream;

    std::vector<std::string> args = {
        "-n", detail::model_name,
        "-f", "png",
        "-i", input.string(),
        "-o", output,
        "-g", std::to_string(this->id_)
    };

    boost::process::child process(detail::enhancer_path, boost::process::args(args), boost::process::std_err > err_stream);

    process.wait();

    if (process.exit_code() != 0)
    {
        std::cerr << "The image " << input.string() << " failed to enhance.\n";
    }

    std::vector<std::string> magick_args = {
        output,
        "-resize", "3840x1632",
        "-filter", "Lanczos2Sharp",
        "-sharpen", "0x0.75",
        "-enhance",
        "-colorspace", "sRGB",
        output
    };

    // Execute the ImageMagick process
    boost::process::child magick_process("/usr/bin/magick", boost::process::args(magick_args));

    magick_process.wait();

    if (magick_process.exit_code() != 0)
    {
        std::cerr << "The ImageMagick resize operation failed for image: " << output << "\n";
    }
}

/*
void gpu_unit::execute(const std::filesystem::path& input)
{
    const std::string output = this->output_dir_.string() + "/" + input.filename().string();

    const std::string output_file = "/dev/null";

    const std::string command = detail::enhancer_path +
        " -n " + detail::model_name +
        " -f " + "png" +
        " -i " + input.string() +
        " -o " + output +
        " -g " + std::to_string(this->id_) +
        " &> " + output_file;

    std::system(command.c_str());
}
*/
