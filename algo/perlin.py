#!/usr/bin/python3

import random
import math
import numpy
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def interpolate(w, v0, v1, x0, x1):
    '''
     |------x----------|
     x0     w          x1
    (v0)   (ret)      (v1)
    ds = x1-x0
    '''
    # clmp = v0*(w-x0)/(x1-x0)
    # if clmp < 0.0:
    #     clmp = 0.0
    # if clmp > 1.0:
    #     clmp = 1.0
    # return clmp*clmp*(3.0-2.0*clmp)

    return v1*(w-x0) + v0*(x1-w)
    
    # return (1.0 - (w-x0))*v0 + (w-x0)*v1

WIDTH = 101
HEIGHT = 101
GRID = []

# Creating grid with random gradient vectors
# (Hashes should be used in shaders for predictability)
for i in range(0,WIDTH):

    grid = GRID
    
    grid.append([])
    for j in range(0, HEIGHT):
        grid[i].append([])
        for k in range(0,2):
            ran = random.Random()
            grid[i][j].append((ran.randint(1,1000)-500)/1000)
        vecsize = math.sqrt(grid[i][j][0]**2 + grid[i][j][1]**2)
        grid[i][j][0] /= vecsize
        grid[i][j][1] /= vecsize
        # print("x = %f\ty = %f\tsize = %f" % (grid[i][j][0], grid[i][j][1], math.sqrt(grid[i][j][0]**2 + grid[i][j][1]**2)))

    GRID = grid

def normalize(x1,x2):

    vecsize = math.sqrt(x1**2 + x2**2) 
    
    return x1/vecsize, x2/vecsize

def rand(x,y):
    
    # return grid[int(x)][int(y)]
    
    # return normalize(math.sin(x**y+x+y+0.7)*math.log(math.fabs(x+y+0.001)), math.cos(x*y+6*x+2.223*y+89))

    return normalize(math.sin(0.1*(math.tan(x*y+x+y)+math.log(x+y+0.7))), math.cos(0.1*(math.log10(x*y+0.673))))

def perlin2D(x, y):

    dx0, x0 = math.modf(x)
    dy0, y0 = math.modf(y)
    x0 = int(x0)
    y0 = int(y0)

    x1 = x0+1
    y1 = y0+1
    # dx1 = 1-dx0
    # dy1 = 1-dy0
    dx0 = x-x0
    dx1 = x-x1
    dy0 = y-y0
    dy1 = y-y1

    dot00 = numpy.dot(rand(x0,y0), [dx0, dy0])
    dot01 = numpy.dot(rand(x0,y1), [dx0, dy1])
    dot10 = numpy.dot(rand(x1,y0), [dx1, dy0])
    dot11 = numpy.dot(rand(x1,y1), [dx1, dy1])
    
    inter0 = interpolate(x, dot00, dot10, x0, x1)
    inter1 = interpolate(x, dot01, dot11, x0, x1)

    inter2 = interpolate(y, inter0, inter1, y0, y1)

    return inter2

def main():
    width = 50
    height = 50
    step = 0.1
    noise = []

    for i in range(0,width):
        noise.append([])
        for j in range(0,height):
            noise[i].append(2*perlin2D(i*step, j*step)+1.2*perlin2D(i*0.1234+1.2, j*0.1234+1.2) + 0.5*perlin2D(i*0.2+6.55, j*0.2+6.55))
            print(i,j,noise[i][j])


    # print(noise)
    X = numpy.arange(0,width)
    Y = numpy.arange(0,height)
    X, Y = numpy.meshgrid(X, Y)
    Z = numpy.array(noise)
    # print(Z)

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    surf = ax.plot_surface(X,Y,Z, cmap='coolwarm')
    fig.colorbar(surf)

    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.imshow(noise)
    plt.show()


if __name__ == "__main__":
    main()