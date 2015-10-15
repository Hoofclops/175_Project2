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
float*  Renderer::sPixelBuffer;

Renderer::Renderer()
{}

void Renderer::InitWindow(int xDim, int yDim)
{
    sScreenSize = Vector2i(xDim, yDim);
    sPixelBuffer = new float[sScreenSize.mX * sScreenSize.mY * 3]; //Multiply by 3 for rgb, when changing this constant, be sure to change hard code in DrawPoint()
}

Vector2i Renderer::GetScreenSize()
{
    return sScreenSize;
}

void Renderer::SetScreenSize(Vector2i size)
{
    sScreenSize = size;
}

void Renderer::DrawPoint(Point point)
{
    int pixelStart = PosToIndex((point.Position()));
    
    Color color = point.Color();
    
    if(pixelStart >= 0 && pixelStart + 2 <= sScreenSize.mX * sScreenSize.mY * 3)
    {
        sPixelBuffer[pixelStart] = color.GetRed();
        sPixelBuffer[pixelStart + 1] = color.GetGreen();
        sPixelBuffer[pixelStart + 2] = color.GetBlue();
    }
    else
    {
        throw out_of_range("Point outside of pixel buffer range");
    }
}

void Renderer::DrawLine(Line line, Algo algo)
{
    if(algo == DDA)
    {
        GraphicsAlgorithm::LineDDA(line);
    }
    else if(algo == BRESENHAM)
    {
        GraphicsAlgorithm::LineBresenham(line);
    }
}

void Renderer::DrawPolygon(Polygon poly, ProjectionPlane plane)
{
    //If polygon has 2 vertices, use line drawing
    
    //Project vertices
    deque<Point> vertices;
    if(plane == XY)
        vertices = Projector::AxonometricXY(poly.GetVertices());
    else if(plane == XZ)
        vertices = Projector::AxonometricXZ(poly.GetVertices());
    else if(plane == YZ)
        vertices = Projector::AxonometricYZ(poly.GetVertices());
    
    if(vertices.size() == 2)
    {
        if(poly.IsSelected())
            GraphicsAlgorithm::LineDDA(Line(vertices[0], vertices[1]), true);
        else
            GraphicsAlgorithm::LineDDA(Line(vertices[0], vertices[1]));
        return;
    }
    
    deque<Line> edges = VerticesToEdges(vertices);
    
//    if(poly.IsSelected())
//    {
//        GraphicsAlgorithm::PolyScanLine(edges, true);
//    }
//    else
//    {
//        GraphicsAlgorithm::PolyScanLine(edges);
//    }
//    
    
    //draw just the edges to cover top and right edges that weren't drawn by scan line
    long edgesCount = edges.size();
    for(int i = 0; i < edgesCount; i++)
    {
        if(poly.IsSelected())
        {
            GraphicsAlgorithm::LineDDA(edges[i], true);
        }
        else
        {
            GraphicsAlgorithm::LineDDA(edges[i]);
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

int Renderer::PosToIndex(Vector2i pos)
{
    int width = sScreenSize.mX;
    
    return (pos.mX + width * pos.mY) * 3;
}

void Renderer::DrawScene(ProjectionPlane plane)
{
    ClearBuffer();
    
    //Clip lines and polygons
    deque<Line> lines = ObjectEditor::Instance()->GetLines();
    deque<Polygon> polys = ObjectEditor::Instance()->GetPolygons();
//    ObjectEditor::Instance()->ClipScene(&lines, &polys);
    
    //Draw polygons
    long n = polys.size();
    for(int i = 0; i < n; i++)
    {
        DrawPolygon(polys[i], plane);
    }
    
    //Draw Lines
    n = lines.size();
    for(int i = 0; i < n; i++)
    {
        Line l = lines[i];
        GraphicsAlgorithm::LineDDA(l);
    }
    
//    //Draw Clipping lines
//    Vector2i minClip = ObjectEditor::Instance()->GetMinClip();
//    Vector2i maxClip = ObjectEditor::Instance()->GetMaxClip();
//    if(maxClip.mX < sScreenSize.mX && maxClip.mY < sScreenSize.mY)
//    {
//        Line l1 = Line(Point(minClip.mX, minClip.mY), Point(maxClip.mX, minClip.mY));
//        Line l2 = Line(Point(maxClip.mX, minClip.mY), Point(maxClip.mX, maxClip.mY));
//        Line l3 = Line(Point(maxClip.mX, maxClip.mY), Point(minClip.mX, maxClip.mY));
//        Line l4 = Line(Point(minClip.mX, maxClip.mY), Point(minClip.mX, minClip.mY));
//        GraphicsAlgorithm::LineDDA(l1);
//        GraphicsAlgorithm::LineDDA(l2);
//        GraphicsAlgorithm::LineDDA(l3);
//        GraphicsAlgorithm::LineDDA(l4);
//    }
}

void Renderer::ClearBuffer()
{
    //sPixelBuffer = new float[SCREEN_SIZE * SCREEN_SIZE * 3];

    int n = sScreenSize.mX * sScreenSize.mY * 3;
    for(int i = 0; i < n; i++)
    {
        sPixelBuffer[i] = 0.0f;
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
