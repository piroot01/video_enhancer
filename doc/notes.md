# extracting the audio

ffmpeg -i (input) -map 0:a -c copy audio.(correct extension)

ffmpeg -i (input) -vn -acodec copy output.(correct extension)

# splitting the video into frames

ffmpeg -i (input) '%04d.bmp'

# combining the frames into video

fmpeg -framerate 23.976 -i %04d.png -c:v libx264 -preset slow -crf 18 -pix_fmt gbrp output_video.mkv

ffmpeg -framerate 23.976 -i %04d.png -c:v libx264 -preset (slow|veryslow) -crf (quality) -pix_fmt gbrp output_video.mkv
