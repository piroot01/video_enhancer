#include <cstdint>
#include <cstdlib>
#include <exception>
#include <memory>
#include <shared_mutex>
#include <string>
#include <iostream>

#include "gpu_unit.hpp"
#include "gpu_holder.hpp"
#include "resource_holder.hpp"

auto main() -> std::int32_t
{
    // relative or absolute path to the video
    const std::string input_video_path = "../resources/test.mkv";

    // name of the working dir
    const std::string temp_dir = "temp";

    // name of the enhanced video
    const std::string output_video_name = "output.mkv"; 

    // create resource holder
    video_enhancer::resource_holder resource_holder(input_video_path);

    try
    {
        /*
        if (!resource_holder.split_video(temp_dir))
        {
            return 1;
        }
        */

        video_enhancer::gpu_holder gpu_holder;

        gpu_holder.set_resource_dir(temp_dir);

        // crete processing unit
        auto gpu0_ptr = std::make_shared<video_enhancer::gpu_unit>(0);
        auto gpu1_ptr = std::make_shared<video_enhancer::gpu_unit>(1);

        const std::string enhanced_images_dir = temp_dir + "/enhanced_images";

        // directory where the enhanced pictures will be stored
        gpu0_ptr->set_output_dir(enhanced_images_dir);

        gpu_holder.register_gpu(gpu0_ptr);
        gpu_holder.register_gpu(gpu1_ptr);

        //gpu0_ptr->execute("temp/01.png");

        resource_holder.load_images(temp_dir);

        resource_holder.sort();

        gpu_holder.execute(resource_holder.get_images());

/*
    gpu0.process_images();

    resource_holder.glue_video(enhanced_images_dir);
*/
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
