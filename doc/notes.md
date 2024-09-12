# extracting the audio

ffmpeg -i (input) -map 0:a -c copy audio.(correct extension)

ffmpeg -i (input) -vn -acodec copy output.(correct extension)

# splitting the video into frames

ffmpeg -i (input) '%04d.bmp'

# combining the frames into video

ffmpeg -framerate 23.976 -i %08d.png -c:v libx264 -preset ultrafast -crf 17 -tune animation -pix_fmt rgb24 output.mkv

# downscaling

magick enlarged.png -resize 3840x1632 -filter Lanczos2Sharp -sharpen 0x0.75 -enhance -colorspace sRGB output.png
