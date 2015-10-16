//
//  Renderer.h
//  Project1
//
//  Created by Brandon Nguyen on 9/26/15.
//  Copyright (c) 2015 Brandon Nguyen. All rights reserved.
//

#ifndef __Project1__Renderer__
#define __Project1__Renderer__

#define CANVAS_RED 0
#define CANVAS_GREEN 0
#define CANVAS_BLUE 0

 #include <OpenGL/gl.h>
 #include <OpenGL/glu.h>
 #include <GLUT/glut.h>
//#include <GL/glut.h>
#include <iostream>
#include <limits>
#include "Vector2i.h"
#include "Color.h"
#include "Polygon.h"
#include "GraphicsAlgorithm.h"
#include "ObjectEditor.h"
#include "Projector.h"
#include "Line3d.h"

//Should contain the pixel buffer and handle drawing of points, lines, and polygons
class Renderer
{
public:
    
    static int sMainWindow, sSubWindow1, sSubWindow2, sSubWindow3;
    
    enum Algo
    {
        DDA,
        BRESENHAM,
    };
    
    enum ProjectionPlane
    {
        XY,
        XZ,
        YZ,
    };
    
    static Renderer* Instance()
    {
        if (!sInstance)
            sInstance = new Renderer;
        return sInstance;
    }
    
    void InitWindow(int xDim, int yDim);
    
    Vector2i GetScreenSize();
    void SetScreenSize(Vector2i size);
    
    void DrawPoint(Point point, int subWindow);
    void DrawLine(Line line, Algo algo, int subWindow);
    void DrawPolygon(Polygon poly);
    
    void ClearBuffer();
    
    static void DisplayPixelBuffer();
    static void DisplayPixelBuffer1();
    static void DisplayPixelBuffer2();
    static void DisplayPixelBuffer3();

    void DrawScene();
    
private:
    static Renderer* sInstance;
    static Vector2i  sScreenSize;
    static Vector2i sSubwindowSize;
    static float* sPixelBuffer;
    static float* sPixelBuffer1;
    static float* sPixelBuffer2;
    static float* sPixelBuffer3;
    
    Renderer();
    
    //Convert coordinate position to appropriate array index in pixel buffer
    int PosToIndex(Vector2i pos);
    
    //Build edges from vertices
    deque<Line> VerticesToEdges(deque<Point> vertices);
    
    //Normalize vertices
    void NormalizeVertices(deque<Point> vertices, deque<float> *normX, deque<float> *normY);
    
    void MapToPlaneQuadrant(deque<Point> *vertices, ProjectionPlane plane);

};

#endif /* defined(__Project1__Renderer__) */
