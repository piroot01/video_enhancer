#include "resource_holder.hpp"
#include "config.hpp"

#include <filesystem>
#include <iostream>
#include <queue>
#include <string>
#include <algorithm>

using namespace video_enhancer;

detail::image::image(const std::string& name) :
    name(name)
{
}

bool detail::image::operator<(const image& rhs) const noexcept
{
    return this->name < rhs.name;
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

    std::string command = "ffmpeg -i " + this->resource_path_.string() + " -vsync 0 " + path.string() + "/" + detail::file_name_format + ".png &> /dev/null";

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

std::queue<detail::image> resource_holder::get_images() const noexcept
{
    return this->images_;
}

void resource_holder::sort()
{
    std::vector<detail::image> temp;

    temp.reserve(this->images_.size());

    while(!this->images_.empty())
    {
        temp.push_back(this->images_.front());

        this->images_.pop();
    }

    std::sort(temp.begin(),temp.end());

    for(std::size_t i = 0, size = temp.size(); i < size; ++i)
    {
       this->images_.push(temp[i]);
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
