#ifndef __VIDEO_ENHANCER_GPU_HOLDER_HPP__
#define __VIDEO_ENHANCER_GPU_HOLDER_HPP__

#include <chrono>
#include <filesystem>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>
#include <thread>
#include <mutex>
#include <future>
#include <functional>

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
        inline auto async_execute(Queue&& images) -> std::future<void>
        {
            auto task = std::bind(&gpu_holder::execute<Queue>, this, std::placeholders::_1);

            return std::async(std::launch::async, task, std::ref(images));
        }

        template<class Queue>
        inline void execute(Queue&& images)
        {
            std::vector<std::thread> workers;

            // for one gpu new thread is spawned
            for (const auto& unit : this->units_)
            {
                auto worker_lambda = [this, &unit, &images] () mutable
                {
                    detail::image image("");

                    while (!images.empty())
                    {
                        {
                            std::lock_guard guard(this->mutex_);

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

/*
        template <class Queue>
        inline void execute(Queue&& images)
        {
            const std::size_t num_units = this->units_.size();

            std::vector<std::future<void>> futures(num_units);

            // spawn threads for each unit
            for (std::size_t i = 0; i < num_units; ++i)
            {
                futures[i] = std::async(std::launch::async, [this, &images, i] ()
                {
                    detail::image image("");

                    while (!images.empty())
                    {
                        {
                            std::lock_guard guard(this->mutex_);

                            image = std::move(images.front());

                            images.pop();
                        }

                        this->units_[i]->execute(this->resource_path_.string() + "/" + image.name);
                    }
                });
            }

            for (auto& future : futures)
            {
                future.wait();
            }
        }
*/
    private:
        std::vector<std::shared_ptr<gpu_unit>> units_;

        std::filesystem::path resource_path_ = {};

        mutable std::mutex mutex_;
    };

} // namespace video_enhancer

#endif
