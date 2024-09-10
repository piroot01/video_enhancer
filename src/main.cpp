#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <future>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <iostream>
#include <thread>

#include "gpu_unit.hpp"
#include "gpu_holder.hpp"
#include "resource_holder.hpp"

auto main() -> std::int32_t
{
    // relative or absolute path to the video
    const std::string input_video_path = "../resources/test.mkv";

    // name of the working dir
    const std::string temp_dir = "temp";

    // audio file
    const std::string audio_dir = temp_dir + "/audio";

    // audio file
    const std::string subs_dir = temp_dir + "/subs";

    // name of the enhanced video
    const std::string output_video_name = "output.mkv"; 

    // create resource holder
    video_enhancer::resource_holder resource_holder(input_video_path);

    try
    {
        resource_holder.split_video(temp_dir);

        video_enhancer::gpu_holder gpu_holder;

        gpu_holder.set_resource_dir(temp_dir);

        // crete processing unit
        auto gpu0_ptr = std::make_shared<video_enhancer::gpu_unit>(0);
        auto gpu1_ptr = std::make_shared<video_enhancer::gpu_unit>(1);

        const std::string enhanced_images_dir = temp_dir + "/enhanced_images";

        // directory where the enhanced pictures will be stored
        gpu0_ptr->set_output_dir(enhanced_images_dir);
        gpu1_ptr->set_output_dir(enhanced_images_dir);

        gpu_holder.register_gpu(gpu0_ptr);
        gpu_holder.register_gpu(gpu1_ptr);

        resource_holder.load_images(temp_dir);

        resource_holder.sort();

        auto images = resource_holder.get_images();

        auto future = gpu_holder.async_execute(images);

        while (true)
        {
            std::cout << images.size() << '\n';
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }

        future.wait();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
