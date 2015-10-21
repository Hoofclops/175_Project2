//
//  Renderer.cpp
//  Project1
//
//  Created by Brandon Nguyen on 9/26/15.
//  Copyright (c) 2015 Brandon Nguyen. All rights reserved.
//

#include "Renderer.h"


/****************************
 *STATIC VARIABLE DECLARATIONS*
 ****************************/

Renderer* Renderer::sInstance;
Vector2i Renderer::sScreenSize;
Vector2i Renderer::sSubwindowSize;
float*  Renderer::sPixelBuffer;
float*  Renderer::sPixelBuffer1;
float*  Renderer::sPixelBuffer2;
float*  Renderer::sPixelBuffer3;
int Renderer::sMainWindow;
int Renderer::sSubWindow1;
int Renderer::sSubWindow2;
int Renderer::sSubWindow3;

Renderer::Renderer()
{}

/****************************
 *PUBLIC FUNCTIONS*
 ****************************/

void Renderer::InitWindow(int xDim, int yDim)
{
    sScreenSize = Vector2i(xDim, yDim);
    sSubwindowSize = Vector2i(xDim/2, yDim/2);
    int windowPixelCount = (sScreenSize.mX) * (sScreenSize.mY) * 3;
    int subWindowPixelCount = windowPixelCount / 4;
    
    sPixelBuffer = new float[windowPixelCount];
    sPixelBuffer1 = new float[subWindowPixelCount];
    sPixelBuffer2 = new float[subWindowPixelCount];
    sPixelBuffer3 = new float[subWindowPixelCount];
    
    for(int i = 0; i < windowPixelCount; i++)
    {
        sPixelBuffer[i] = 0.0f;
        
        if(i < subWindowPixelCount)
        {
            sPixelBuffer1[i] = 0.0f;
            sPixelBuffer2[i] = 0.0f;
            sPixelBuffer3[i] = 0.0f;
        }
    }
}

void Renderer::DrawScene()
{
    ClearBuffer();
    
    //Colorize quadrants
    int windowPixelCount = (sScreenSize.mX) * (sScreenSize.mY) * 3;
    int subWindowPixelCount = windowPixelCount / 4;
    for(int i = 0; i < subWindowPixelCount; i++)
    {
        sPixelBuffer1[i] = 0.55f;
        sPixelBuffer2[i] = 0.4f;
        sPixelBuffer3[i] = 0.25f;
    }
    
    DrawPolygons();
    
    //Post redisplay for all sub windows
    glutSetWindow(sMainWindow);
    glutPostRedisplay();
    glutSetWindow(sSubWindow1);
    glutPostRedisplay();
    glutSetWindow(sSubWindow2);
    glutPostRedisplay();
    glutSetWindow(sSubWindow3);
    glutPostRedisplay();
}

void Renderer::DrawLine(Line line, Algo algo, int subWindow)
{
    if(algo == DDA)
    {
        GraphicsAlgorithm::LineDDA(line, subWindow);
    }
    else if(algo == BRESENHAM)
    {
        GraphicsAlgorithm::LineBresenham(line, subWindow);
    }
}

void Renderer::DrawPoint(Point point, int subWindow)
{
    int pixelStart = PosToIndex((point.Position()));
    
    Color color = point.GetColor();
    
    if(pixelStart >= 0 && pixelStart + 2 <= (sSubwindowSize.mX) * (sSubwindowSize.mY) * 3)
    {
        if(subWindow == sSubWindow1)
        {
            sPixelBuffer1[pixelStart] = color.GetRed();
            sPixelBuffer1[pixelStart + 1] = color.GetGreen();
            sPixelBuffer1[pixelStart + 2] = color.GetBlue();
        }
        else if(subWindow == sSubWindow2)
        {
            sPixelBuffer2[pixelStart] = color.GetRed();
            sPixelBuffer2[pixelStart + 1] = color.GetGreen();
            sPixelBuffer2[pixelStart + 2] = color.GetBlue();

        }
        else if(subWindow == sSubWindow3)
        {
            sPixelBuffer3[pixelStart] = color.GetRed();
            sPixelBuffer3[pixelStart + 1] = color.GetGreen();
            sPixelBuffer3[pixelStart + 2] = color.GetBlue();

        }
    }
    else
    {
        throw out_of_range("Point outside of pixel buffer range");
    }
}

void Renderer::DisplayPixelBuffer()
{
    //Misc.
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();//load identity matrix
    
    //draws pixel on screen, width and height must match pixel buffer dimension
    glDrawPixels(sScreenSize.mX, sScreenSize.mY, GL_RGB, GL_FLOAT, sPixelBuffer);
    glEnd();
    glFlush();
}
void Renderer::DisplayPixelBuffer1()
{
    //Misc.
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();//load identity matrix
    
    //draws pixel on screen, width and height must match pixel buffer dimension
    glDrawPixels(sSubwindowSize.mX, sSubwindowSize.mY, GL_RGB, GL_FLOAT, sPixelBuffer1);
    glEnd();
    glFlush();
}
void Renderer::DisplayPixelBuffer2()
{
    //Misc.
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();//load identity matrix
    
    //draws pixel on screen, width and height must match pixel buffer dimension
    glDrawPixels(sSubwindowSize.mX, sSubwindowSize.mY, GL_RGB, GL_FLOAT, sPixelBuffer2);
    glEnd();
    glFlush();
}
void Renderer::DisplayPixelBuffer3()
{
    //Misc.
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();//load identity matrix
    
    //draws pixel on screen, width and height must match pixel buffer dimension
    glDrawPixels(sSubwindowSize.mX, sSubwindowSize.mY, GL_RGB, GL_FLOAT, sPixelBuffer3);
    glEnd();
    glFlush();
}

void Renderer::ClearBuffer()
{
    int n = sScreenSize.mX * sScreenSize.mY * 3;
    int m = n / 4;
    for(int i = 0; i < n; i++)
    {
        sPixelBuffer[i] = 0.0f;
        
        if(i < m)
        {
            sPixelBuffer1[i] = 0.0f;
            sPixelBuffer2[i] = 0.0f;
            sPixelBuffer3[i] = 0.0f;
        }
    }
}

/****************************
 *PRIVATE FUNCTIONS*
 ****************************/

int Renderer::PosToIndex(Vector2i pos)
{
    int width = sSubwindowSize.mX;
    return (pos.mX + width * pos.mY) * 3;
}

void Renderer::DrawPolygons()
{
    deque<Polygon> polys = ObjectEditor::Instance()->GetPolygons();
    deque<deque<Point>> verticesXY, verticesXZ, verticesYZ;
    
    //Project all vertices
    long polyCount = polys.size();
    for(int i = 0; i < polyCount; i++)
    {
        verticesXY.push_back(Projector::AxonometricXY(polys[i].GetVertices()));
        verticesXZ.push_back(Projector::AxonometricXZ(polys[i].GetVertices()));
        verticesYZ.push_back(Projector::AxonometricYZ(polys[i].GetVertices()));
    }
    
    //Find min and max points for each plane
    float minXY = numeric_limits<float>::max();
    float minXZ = numeric_limits<float>::max();
    float minYZ = numeric_limits<float>::max();
    float maxXY = 0;
    float maxXZ = 0;
    float maxYZ = 0;
    for(int i = 0; i < polyCount; i++)
    {
        long m = verticesXY[i].size();
        for(int j = 0; j < m; j++)
        {
            //XY
            int x = verticesXY[i][j].X(), y = verticesXY[i][j].Y();
            if(x < minXY)
                minXY = x;
            if(y < minXY)
                minXY = y;
            if(x > maxXY)
                maxXY = x;
            if(y > maxXY)
                maxXY = y;
            
            //XZ
            x = verticesXZ[i][j].X(); y = verticesXZ[i][j].Y();
            if(x < minXZ)
                minXZ = x;
            if(y < minXZ)
                minXZ = y;
            if(x > maxXZ)
                maxXZ = x;
            if(y > maxXZ)
                maxXZ = y;
            
            //YZ
            x = verticesYZ[i][j].X(); y = verticesYZ[i][j].Y();
            if(x < minYZ)
                minYZ = x;
            if(y < minYZ)
                minYZ = y;
            if(x > maxYZ)
                maxYZ = x;
            if(y > maxYZ)
                maxYZ = y;
        }
    }
    
    //Normalize and draw wire frames
    for(int i = 0; i < polyCount; i++)
    {
        //Normalize and project vertices
        MapToPlaneQuadrant(&verticesXY[i], XY, minXY, maxXY);
        MapToPlaneQuadrant(&verticesXZ[i], XZ, minXZ, maxXZ);
        MapToPlaneQuadrant(&verticesYZ[i], YZ, minYZ, maxYZ);
        
        deque<Line> edgesXY = polys[i].GetEdges2d(verticesXY[i]);
        deque<Line> edgesXZ = polys[i].GetEdges2d(verticesXZ[i]);
        deque<Line> edgesYZ = polys[i].GetEdges2d(verticesYZ[i]);
        
        //draw wire frame
        long edgesCount = edgesXY.size();
        for(int j = 0; j < edgesCount; j++)
        {
            if(polys[i].IsSelected())
            {
                GraphicsAlgorithm::LineDDA(edgesXY[j], Renderer::sSubWindow1, true);
                GraphicsAlgorithm::LineDDA(edgesXZ[j], Renderer::sSubWindow2, true);
                GraphicsAlgorithm::LineDDA(edgesYZ[j], Renderer::sSubWindow3, true);
            }
            else
            {
                GraphicsAlgorithm::LineDDA(edgesXY[j], Renderer::sSubWindow1);
                GraphicsAlgorithm::LineDDA(edgesXZ[j], Renderer::sSubWindow2);
                GraphicsAlgorithm::LineDDA(edgesYZ[j], Renderer::sSubWindow3);
            }
        }
    }
}

void Renderer::MapToPlaneQuadrant(deque<Point> *vertices, ProjectionPlane plane, float min, float max)
{
    deque<float> normX, normY;
    NormalizeVertices(*vertices, &normX, &normY, min, max);
    
    //Define quadrant
    Vector2i minQuad, maxQuad;
    minQuad.mX = 0;
    minQuad.mY = 0;
    maxQuad.mX = sSubwindowSize.mX - 1;
    maxQuad.mY = sSubwindowSize.mY - 1;
    
    //Map normalized values to quadrant
    long n = vertices->size();
    for(int i = 0; i < n; i++)
    {
        float zX = normX[i], zY = normY[i];
        int x, y;
        
        x = minQuad.mX + (zX * (maxQuad.mX - minQuad.mX));
        y = minQuad.mY + (zY * (maxQuad.mY - minQuad.mY));
        
        vertices->at(i) = Point(x,y);
    }
}

void Renderer::NormalizeVertices(deque<Point> vertices, deque<float> *normX, deque<float> *normY, float min, float max)
{
    
    long n = vertices.size();
    //Find normalized points
    for(int i = 0; i < n; i++)
    {
        float x = vertices[i].X(), y = vertices[i].Y();
        
        x = (float)(x - min) / (float)(max - min);
        y = (float)(y - min) / (float)(max - min);
        
        if(x < 0 || x > 1 || y < 0 || y > 1)
        {
            throw exception();
        }
        
        normX->push_back(x);
        normY->push_back(y);
    }
}

