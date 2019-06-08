#pragma once

#include "api.h"
#include "buffer.h"

#include <stdio.h>

#define DW_FILL 0
#define DW_WIREFRAME 1
#define DW_CONTOUR 2
#define DW_DOTS 4

class Drawer
{
  public:
    void draw(Buffers *buffers, unsigned int type, float line_thickness = 1.0)
    {
        switch (type)
        {
        case DW_FILL:
            draw_fill(buffers->mesh);
            break;
        case DW_WIREFRAME:
            draw_wireframe(buffers->mesh, line_thickness = line_thickness);
            break;
        case DW_CONTOUR:
            draw_contour(buffers->contour, line_thickness = line_thickness);
            break;
        case DW_DOTS:
            draw_dots(buffers->dots, line_thickness = line_thickness);
            break;
        default:
            printf("Error: draw type not recognized");
            exit(0);
            break;
        }
    }

    // MESH
    static void draw_wireframe(ModelBuffer buffer, float line_thickness = 1.0)
    {
        glLineWidth(line_thickness);

        glBindTexture(GL_TEXTURE_BUFFER, buffer.TBO);

        glBindVertexArray(buffer.VAO);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, buffer.numelements, GL_UNSIGNED_INT, 0);
    }

    static void draw_fill(ModelBuffer buffer)
    {
        glBindTexture(GL_TEXTURE_BUFFER, buffer.TBO);

        glBindVertexArray(buffer.VAO);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, buffer.numelements, GL_UNSIGNED_INT, 0);
    }
    // CONTOUR
    static void draw_contour(ModelBuffer buffer, float line_thickness = 1.0)
    {
        glLineWidth(line_thickness);

        glBindVertexArray(buffer.VAO);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_LINES, buffer.numelements, GL_UNSIGNED_INT, 0);
    }
    // DOTS
    static void draw_dots(ModelBuffer buffer, float line_thickness = 1.0)
    {
        glPointSize(line_thickness);

        glBindVertexArray(buffer.VAO);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_POINTS, buffer.numelements, GL_UNSIGNED_INT, 0);
    }
};

// class Draw2D
// {
//   public:
//     // Uses GL_TRIANGLE_FAN instead of GL_TRIANGLES
//     static void draw_fill()
//     {
//         glBindTexture(GL_TEXTURE_BUFFER, TBO);

//         glBindVertexArray(VAO_contour);

//         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//         glDrawElements(GL_TRIANGLE_FAN, numelements_contour, GL_UNSIGNED_INT, 0);
//     }

//     static void draw_wireframe()
//     {
//         glBindTexture(GL_TEXTURE_BUFFER, TBO);

//         glBindVertexArray(VAO_contour);

//         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//         glDrawElements(GL_TRIANGLE_FAN, numelements_contour, GL_UNSIGNED_INT, 0);
//     }
// };
