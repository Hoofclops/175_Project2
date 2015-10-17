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
    Vector2i minXY = Vector2i(numeric_limits<int>::max(), numeric_limits<int>::max());
    Vector2i minXZ = Vector2i(numeric_limits<int>::max(), numeric_limits<int>::max());
    Vector2i minYZ = Vector2i(numeric_limits<int>::max(), numeric_limits<int>::max());
    Vector2i maxXY = Vector2i(0,0);
    Vector2i maxXZ = Vector2i(0,0);
    Vector2i maxYZ = Vector2i(0,0);
    for(int i = 0; i < polyCount; i++)
    {
        long m = verticesXY[i].size();
        for(int j = 0; j < m; j++)
        {
            //XY
            int x = verticesXY[i][j].X(), y = verticesXY[i][j].Y();
            if(x < minXY.mX)
                minXY.mX = x;
            if(y < minXY.mY)
                minXY.mY = y;
            if(x > maxXY.mX)
                maxXY.mX = x;
            if(y > maxXY.mY)
                maxXY.mY = y;
            
            //XZ
            x = verticesXZ[i][j].X(); y = verticesXZ[i][j].Y();
            if(x < minXZ.mX)
                minXZ.mX = x;
            if(y < minXZ.mY)
                minXZ.mY = y;
            if(x > maxXZ.mX)
                maxXZ.mX = x;
            if(y > maxXZ.mY)
                maxXZ.mY = y;
            
            //YZ
            x = verticesYZ[i][j].X(); y = verticesYZ[i][j].Y();
            if(x < minYZ.mX)
                minYZ.mX = x;
            if(y < minYZ.mY)
                minYZ.mY = y;
            if(x > maxYZ.mX)
                maxYZ.mX = x;
            if(y > maxYZ.mY)
                maxYZ.mY = y;
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

void Renderer::MapToPlaneQuadrant(deque<Point> *vertices, ProjectionPlane plane, Vector2i minPoint, Vector2i maxPoint)
{
    deque<float> normX, normY;
    NormalizeVertices(*vertices, &normX, &normY, minPoint, maxPoint);
    
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

void Renderer::NormalizeVertices(deque<Point> vertices, deque<float> *normX, deque<float> *normY, Vector2i minPoint, Vector2i maxPoint)
{
    
    long n = vertices.size();
    //Find normalized points
    for(int i = 0; i < n; i++)
    {
        float x = vertices[i].X(), y = vertices[i].Y();
        
        x = (float)(x - minPoint.mX) / (float)(maxPoint.mX - minPoint.mX);
        y = (float)(y - minPoint.mY) / (float)(maxPoint.mY - minPoint.mY);
        
        if(x < 0 || x > 1 || y < 0 || y > 1)
        {
            throw exception();
        }
        
        normX->push_back(x);
        normY->push_back(y);
    }
}

