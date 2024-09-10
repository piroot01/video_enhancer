#ifndef __VIDEO_ENHANCER_GPU_HOLDER_HPP__
#define __VIDEO_ENHANCER_GPU_HOLDER_HPP__

#include <filesystem>
#include <memory>
#include <utility>
#include <vector>
#include <thread>
#include <mutex>

#include "gpu_unit.hpp"
#include "resource_holder.hpp"

namespace video_enhancer
{
    class gpu_holder
    {
    public:
        gpu_holder();

    public:
        void register_gpu(const std::shared_ptr<gpu_unit>& unit_ptr);

        void set_resource_dir(const std::filesystem::path& path);

    public:
        template<class Queue>
        inline void execute(Queue&& images)
        {
            std::mutex queue_mutex;

            std::vector<std::thread> workers;

            // for one gpu new thread is spawned
            for (const auto& unit : this->units_)
            {
                auto worker_lambda = [this, &unit, &images, &queue_mutex] () mutable
                {
                    detail::image image("");

                    while (!images.empty())
                    {
                        {
                            std::lock_guard guard(queue_mutex);

                            image = images.front();

                            images.pop();
                        }

                        unit->execute(this->resource_path_.string() + "/" + image.name);
                    }
                };

                workers.emplace_back(std::move(worker_lambda));
            }

            for (auto& worker : workers)
            {
                worker.join();
            }
        }

    private:
        std::vector<std::shared_ptr<gpu_unit>> units_;

        std::filesystem::path resource_path_ = {};

    };

} // namespace video_enhancer

#endif
