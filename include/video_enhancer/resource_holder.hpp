#ifndef __VIDEO_ENHANCER_RESOURCE_HOLDER_HPP__
#define __VIDEO_ENHANCER_RESOURCE_HOLDER_HPP__

#include <filesystem>
#include <queue>

namespace video_enhancer
{
    namespace detail
    {
        struct image
        {
            image(const std::string& name);

            std::string name;

            bool operator<(const image& rhs) const noexcept;
        };

    } // namespace detail

    class resource_holder
    {
    public:
        resource_holder(const std::filesystem::path& resource_path);

    public:
        bool split_video(const std::string& dir) const;

        void load_images(const std::string& dir);

        std::queue<detail::image> get_images() const noexcept;

    public:
        void sort();

    // debugging reasons
    public:
        void print() const;

    private:
        std::filesystem::path resource_path_;

        std::queue<detail::image> images_;

    };
} // namespace video_enhancer

#endif
