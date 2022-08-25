#pragma once

#include "api.hpp"

// class Recorder
// {
// public:
//     Recorder(unsigned int window_width, unsigned int window_height)
//         : window_width_(window_width), window_height_(window_height)
//     {
//         /* Original command:
//          * ffmpeg -r 60 -f rawvideo -pix_fmt rgba -s 1280x720 -i 
//          * -threads 0 -preset fast -y -pix_fmt yuv420p -crf 21 -vf vflip output.mp4";
//          * 
//          * Source: http://blog.mmacklin.com/2013/06/11/real-time-video-capture-with-ffmpeg/
//          */

//         char cmd[200] = "";

//         sprintf(cmd, "ffmpeg "
//                      "-r 30 -f rawvideo -pixel_format rgba -video_size %dx%d -i pipe:0 "                // input file configs
//                      "-preset slow -threads 8 -video_size %dx%d -vf vflip -y -crf 18 ./vid/output.mp4", // output file configs
//                 window_width_, window_height_, window_width_, window_height_);

//         // Add "-c:v libx264 -profile:v main -vf format=yuv420p" for mobile compatibility?

//         // open pipe to ffmpeg's stdin in binary write mode
//         if ((ffmpeg = popen(cmd, "w")) == NULL)
//         {
//             perror("popen error");
//             // opengl.terminate(0);
//         }

//         if ((buffer = new int[get_window_width() * get_window_height()]) == NULL)
//         {
//             quad::fatal_error("allocation error");
//         }
//     }

//     void write_framebuffer()
//     {
//     }

// private:
//     const unsigned int window_width_;
//     const unsigned int window_height_;
// };

// // Recorder
// private:
// FILE *ffmpeg;
// int *buffer;
// void start_recorder()
// {
// }

// void record()
// {
//     glReadPixels(0, 0, get_window_width(), get_window_height(), GL_RGBA, GL_UNSIGNED_BYTE, buffer);

//     fwrite(buffer, sizeof(int) * get_window_width() * get_window_height(), 1, ffmpeg);
// }
