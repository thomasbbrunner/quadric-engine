# Script to instagramify videos
#
# General:
#   https://trac.ffmpeg.org/wiki/Encode/H.264
#   http://ffmpeg.org/ffmpeg-filters.html  
#   
# Crop (duration)
#   https://stackoverflow.com/questions/18444194/cutting-the-videos-based-on-start-and-end-time-using-ffmpeg
#
# Scaling:
#   https://trac.ffmpeg.org/wiki/Scaling
#   https://superuser.com/questions/624563/how-to-resize-a-video-to-make-it-smaller-with-ffmpeg
#
# Crop (frame):
#   https://superuser.com/questions/510985/how-can-i-crop-a-video-to-a-part-of-the-view
#   https://video.stackexchange.com/questions/4563/how-can-i-crop-a-video-with-ffmpeg
#
# Compression:
#   https://slhck.info/video/2017/02/24/crf-guide.html
#   
# Requirements:
#   format: .mp4 (H.264 Codec)
#   framerate: 23 to 30 (60?) fps
#   length: 3 to 60 s
#   max dimensions: 1080 px wide
#   max size limit: 15mb
#   max bitrate: 3500 kbps
#   

# Saving filename
filename=$1

# Removing extension
filename="${filename%.*}"

# Crop (duration)
ffmpeg -i "$filename.mp4" -ss 00:00:00 -t 00:00:30 -async 1 "${filename}_short.mp4"
filename="${filename}_short"

# Crop (frame)
ffmpeg -i "$filename.mp4" -vf crop=1080:in_h "${filename}_cropped.mp4"
filename="${filename}_cropped"

# Scaling
ffmpeg -i "$filename.mp4" -vf scale=1080:-2 "${filename}_scaled.mp4"
filename="${filename}_scaled"

# Reduce size
# Default: 25
#ffmpeg -i "$filename.mp4" -c:v libx264 -crf 50 "${filename}_compressed.mp4"
#filename="${filename}_compressed"
   
# Investigate formats
ffmpeg -i "$filename.mp4" -c:v libx264 -vf format=yuv420p -profile:v main -crf 23 "${filename}_compatibility.mp4"
filename="${filename}_compatibility"



