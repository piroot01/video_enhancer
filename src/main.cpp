#include <cstdint>
#include <cstdlib>
#include <string>

auto main() -> std::int32_t
{
    const std::string input_dir = "../test_images/";

    std::string command = "realesrgan-ncnn-vulkan -i " + input_dir + "01.jpg " + "-o output.png -f png -n realesrgan-x4plus-anime -s 1";

    std::system(command.c_str());

    return 0;
}
