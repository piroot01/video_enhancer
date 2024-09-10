#include "resource_holder.hpp"
#include "config.hpp"

#include <filesystem>
#include <iostream>
#include <queue>
#include <string>
#include <algorithm>
#include <unordered_set>

using namespace video_enhancer;

detail::image::image(const std::string& name) :
    name(name)
{
}

bool detail::image::operator<(const image& rhs) const noexcept
{
    return this->name < rhs.name;
}

bool detail::image::operator==(const image& rhs) const noexcept
{
    return this->name == rhs.name;
}

resource_holder::resource_holder(const std::filesystem::path& resource_path) :
    resource_path_(resource_path)
{
}

bool resource_holder::split_video(const std::filesystem::path& path) const
{
    if (std::filesystem::exists(path))
    {
        std::cout << "The dir for split_video  already exists. Rewrite? [y/N]";

        std::string response = "";

        std::getline(std::cin, response);

        if (response == "y" || response == "Y")
        {
            if (!std::filesystem::remove_all(path))
            {
                throw std::filesystem::filesystem_error::exception();
            }
        }
        else
        {
            return false;
        }
    }

    if (!std::filesystem::create_directory(path))
    {
        throw std::filesystem::filesystem_error::exception();
    }

    std::string command = "ffmpeg -i " + this->resource_path_.string() + " -vsync 0 " + path.string() + "/" + detail::file_name_format + ".png";

    // execute the command
    std::system(command.c_str());

    return true;
}

void resource_holder::load_images(const std::string& dir)
{
    for (const auto& entry : std::filesystem::directory_iterator(dir))
    {
        if (entry.is_regular_file() && entry.exists() && entry.path().extension() == ".png")
        {
            this->images_.emplace(entry.path().filename().string());
        }
    }
}

std::queue<detail::image> resource_holder::get_images() noexcept
{
    this->sort(this->images_);

    return this->images_;
}

std::queue<detail::image> resource_holder::get_unfinished_images(const std::filesystem::path& path) noexcept
{
    if (!std::filesystem::exists(path))
    {
        return this->get_images();
    }

    std::unordered_set<detail::image> finished_images;

    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_regular_file() && entry.exists() && entry.path().extension() == ".png")
        {
            finished_images.insert(entry.path().filename().string());
        }
    }

    // load the images into set
    std::unordered_set<detail::image> images;
    std::queue<detail::image> temp_queue = this->images_;

    while (!temp_queue.empty())
    {
        images.insert(temp_queue.front());
        temp_queue.pop();
    }

    std::queue<detail::image> unique_queue;

    for (const auto& img : images)
    {
        if (finished_images.find(img) == finished_images.end())
        {
            unique_queue.push(img);
        }
    }

    this->sort(unique_queue);

    return unique_queue;
}

void resource_holder::sort(std::queue<detail::image>& queue)
{
    std::vector<detail::image> temp;

    temp.reserve(queue.size());

    while(!queue.empty())
    {
        temp.push_back(queue.front());

        queue.pop();
    }

    std::sort(temp.begin(),temp.end());

    for(std::size_t i = 0, size = temp.size(); i < size; ++i)
    {
       queue.push(temp[i]);
    }
}

void resource_holder::print() const
{
    auto images_copy = this->images_;

    while (!images_copy.empty())
    {
        std::cout << images_copy.front().name << ' ';
        images_copy.pop();
    }

    std::cout << '\n';
}
