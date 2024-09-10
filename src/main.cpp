#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <future>
#include <iomanip>
#include <ios>
#include <memory>
#include <mutex>
#include <ostream>
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
    const std::string input_video_path = "../resources/kizu_02.mkv";

    // name of the working dir
    const std::string temp_dir = "temp";

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

        const std::size_t image_count = images.size();

        std::size_t last_image_count = image_count;

        auto start_time = std::chrono::high_resolution_clock::now();

        auto refresh_period = std::chrono::milliseconds(100);

        double estimated_time_remaining = 0;

        while (images.size() > 0)
        {
            if (last_image_count != images.size())
            {
                last_image_count = images.size();

                std::size_t processed_count = image_count - images.size();

                // elapsed time
                auto now = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> elapsed = now - start_time;

                double progress = static_cast<double>(processed_count) / image_count;
                double estimated_total_time = elapsed.count() / progress;
                estimated_time_remaining = estimated_total_time - elapsed.count();
            }

            estimated_time_remaining -= refresh_period.count() / 1000.0;

            std::cout << "\r[" << 100 * (1 - images.size() / static_cast<double>(image_count)) << " %]: Processed " << image_count - images.size() << " out of " << image_count << " images. ETA: " << std::fixed << std::setprecision(2) << ((estimated_time_remaining > 0) ? estimated_time_remaining : 0) << " s." << std::flush;

            std::this_thread::sleep_for(std::chrono::milliseconds(refresh_period));
        }

        future.wait();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
