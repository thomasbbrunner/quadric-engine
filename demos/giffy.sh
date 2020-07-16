# Convert .mp4 file into .gif
#
# https://askubuntu.com/questions/648603/how-to-create-an-animated-gif-from-mp4-video-via-command-line


ffmpeg \
    -i demo_cubeworld.mp4 \
    -ss 00:00:01.5 -t 6 \
    -loop 0 -r 15 \
    -vf "scale=400:-1,crop=in_w:300,setpts=0.8*PTS,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse" \
    demo_cubeworld.gif
