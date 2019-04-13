#ifndef VIDEO_H
#define VIDEO_H

#include "api.h"

#include <cstdlib>
#include <stdio.h>

class Video
{
    // Source: http://blog.mmacklin.com/2013/06/11/real-time-video-capture-with-ffmpeg/
  public:
    FILE *ffmpeg;
    int *buffer;

    int window_height;
    int window_width;

    Video(int window_width_in, int window_height_in)
    {
        // Default from source
        // const char *cmd = "ffmpeg -r 60 -f rawvideo -pix_fmt rgba -s 1280x720 -i - "
        //                   "-threads 0 -preset fast -y -pix_fmt yuv420p -crf 21 -vf vflip output.mp4";

        window_height = window_height_in;
        window_width = window_width_in;

        char cmd[200] = "";

        sprintf(cmd, "ffmpeg "
                     "-r 30 -f rawvideo -pixel_format rgba -video_size %dx%d -i pipe:0 "              // input file configs
                     "-preset slow -threads 4 -video_size %dx%d -vf vflip -y -crf 18 /../output.mp4", // output file configs
                window_width, window_height, window_width, window_height);

        // open pipe to ffmpeg's stdin in binary write mode
        if ((ffmpeg = popen(cmd, "w")) == NULL)
        {
            perror("popen error");
            // opengl.terminate(0);
        }

        if ((buffer = new int[window_width * window_height]) == NULL)
        {
            printf("allocation error");
            //opengl.terminate(0);
        }
    }

    void record()
    {
        glReadPixels(0, 0, window_width, window_height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

        fwrite(buffer, sizeof(int) * window_width * window_height, 1, ffmpeg);
    }

    void terminate()
    {
        pclose(ffmpeg);
    }
};

#endif