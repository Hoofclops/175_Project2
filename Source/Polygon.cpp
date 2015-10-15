//
//  Polygon.cpp
//  Project1
//
//  Created by Brandon Nguyen on 9/26/15.
//  Copyright (c) 2015 Brandon Nguyen. All rights reserved.
//

#include "Polygon.h"

Polygon::Polygon(deque<Vector3i> vertPositions)
{
    long n = vertPositions.size();
    
    if(n <= 0)
    {
        throw invalid_argument("Invalid amount of vertices passed to Polygon constructor");
    }
    
    //Construct lines and edges, insert them into data tables
    Point3d first = Point3d(vertPositions[0]);
    mVertices.push_back(first);
    for(int i = 1; i < n; i++)
    {
        Point3d p = Point3d(vertPositions[i]);
        mVertices.push_back(p);
    }
    
    mSelected = false;
}

deque<Point3d> Polygon::GetVertices()
{
    return mVertices;
}
void Polygon::SetVertex(int index, Vector3i pos)
{
    mVertices[index].X(pos.mX);
    mVertices[index].Y(pos.mY);
    mVertices[index].Z(pos.mZ);
}

void Polygon::SetVertices(deque<Point3d> verts)
{
    mVertices = verts;
}

deque<Line3d> Polygon::GetEdges()
{
    mEdges.clear();
    
    long n = mVertices.size();
    for(int i = 1; i < n; i++)
    {
        Line3d l = Line3d(mVertices[i - 1], mVertices[i]);
        mEdges.push_back(l);
    }
    
    Line3d closingEdge = Line3d(mVertices[n - 1], mVertices[0]);
    mEdges.push_back(closingEdge);
    
    return mEdges;
}

void Polygon::SetSelected(bool isSelected)
{
    mSelected = isSelected;
}

bool Polygon::IsSelected()
{
    return mSelected;
}
