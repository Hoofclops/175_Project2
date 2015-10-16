//
//  Renderer.cpp
//  Project1
//
//  Created by Brandon Nguyen on 9/26/15.
//  Copyright (c) 2015 Brandon Nguyen. All rights reserved.
//

#include "Renderer.h"

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

Vector2i Renderer::GetScreenSize()
{
    return sScreenSize;
}

void Renderer::SetScreenSize(Vector2i size)
{
    sScreenSize = size;
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

void Renderer::DrawPolygon(Polygon poly)
{
    //If polygon has 2 vertices, use line drawing
    
    //Project vertices
    deque<Point> verticesXY = Projector::AxonometricXY(poly.GetVertices());
    deque<Point> verticesXZ = Projector::AxonometricXZ(poly.GetVertices());
    deque<Point> verticesYZ = Projector::AxonometricYZ(poly.GetVertices());

    //Normalize and project vertices
    MapToPlaneQuadrant(&verticesXY, XY);
    MapToPlaneQuadrant(&verticesXZ, XZ);
    MapToPlaneQuadrant(&verticesYZ, YZ);
    
    deque<Line> edgesXY = VerticesToEdges(verticesXY);
    deque<Line> edgesXZ = VerticesToEdges(verticesXZ);
    deque<Line> edgesYZ = VerticesToEdges(verticesYZ);

    //draw wire frame
    long edgesCount = edgesXY.size();
    for(int i = 0; i < edgesCount; i++)
    {
        if(poly.IsSelected())
        {
            GraphicsAlgorithm::LineDDA(edgesXY[i], Renderer::sSubWindow1, true);
            GraphicsAlgorithm::LineDDA(edgesXZ[i], Renderer::sSubWindow2, true);
            GraphicsAlgorithm::LineDDA(edgesYZ[i], Renderer::sSubWindow3, true);
        }
        else
        {
            GraphicsAlgorithm::LineDDA(edgesXY[i], Renderer::sSubWindow1);
            GraphicsAlgorithm::LineDDA(edgesXZ[i], Renderer::sSubWindow2);
            GraphicsAlgorithm::LineDDA(edgesYZ[i], Renderer::sSubWindow3);
        }
    }
}

deque<Line> Renderer::VerticesToEdges(deque<Point> vertices)
{
    deque<Line> edges;
    
    long n = vertices.size();
    for(int i = 1; i < n; i++)
    {
        Line l = Line(vertices[i - 1], vertices[i]);
        edges.push_back(l);
    }
    
    Line closingEdge = Line(vertices[n - 1], vertices[0]);
    edges.push_back(closingEdge);

    return edges;
}

void Renderer::NormalizeVertices(deque<Point> vertices, deque<float> *normX, deque<float> *normY)
{
    //Find min and max points
    Vector2i min = Vector2i(numeric_limits<int>::max(), numeric_limits<int>::max()), max = Vector2i(0,0);
    long n = vertices.size();
    for(int i = 0; i < n; i++)
    {
        int x = vertices[i].X(), y = vertices[i].Y();
        if(x < min.mX)
            min.mX = x;
        if(y < min.mY)
            min.mY = y;
        if(x > max.mX)
            max.mX = x;
        if(y > max.mY)
            max.mY = y;
    }

    //Find normalized points
    for(int i = 0; i < n; i++)
    {
        float x = vertices[i].X(), y = vertices[i].Y();
        
        x = (float)(x - min.mX) / (float)(max.mX - min.mX);
        y = (float)(y - min.mY) / (float)(max.mY - min.mY);
        
        normX->push_back(x);
        normY->push_back(y);
    }
}

void Renderer::MapToPlaneQuadrant(deque<Point> *vertices, ProjectionPlane plane)
{
    deque<float> normX, normY;
    NormalizeVertices(*vertices, &normX, &normY);
    
    //Define quadrant
    Vector2i minQuad, maxQuad;
    minQuad.mX = 0;
    minQuad.mY = 0;
    maxQuad.mX = sSubwindowSize.mX - 1;
    maxQuad.mY = sSubwindowSize.mY - 1;

//    if(plane == XY)
//    {
//        minQuad.mX = 0;
//        minQuad.mY = 0;
//        maxQuad.mX = (sScreenSize.mX / 2) - 1;
//        maxQuad.mY = (sScreenSize.mY / 2) - 1;
//    }
//    else if(plane == XZ)
//    {
//        minQuad.mX = sScreenSize.mX / 2;
//        minQuad.mY = 0;
//        maxQuad.mX = sScreenSize.mX - 1;
//        maxQuad.mY = (sScreenSize.mY / 2) - 1;
//
//    }
//    else if(plane == YZ)
//    {
//        minQuad.mX = 0;
//        minQuad.mY = sScreenSize.mY / 2;
//        maxQuad.mX = (sScreenSize.mX / 2) - 1;
//        maxQuad.mY = sScreenSize.mY - 1;
//
//    }
    
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

int Renderer::PosToIndex(Vector2i pos)
{
    int width = sSubwindowSize.mX;
    return (pos.mX + width * pos.mY) * 3;
}

void Renderer::DrawScene()
{
    ClearBuffer();
    
    //Clip lines and polygons
    deque<Line> lines = ObjectEditor::Instance()->GetLines();
    deque<Polygon> polys = ObjectEditor::Instance()->GetPolygons();
    
    //Draw polygons
    long n = polys.size();
    for(int i = 0; i < n; i++)
    {
        DrawPolygon(polys[i]);
    }
    
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

