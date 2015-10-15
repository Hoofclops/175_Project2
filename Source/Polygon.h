//
//  Polygon.h
//  Project1
//
//  Created by Brandon Nguyen on 9/26/15.
//  Copyright (c) 2015 Brandon Nguyen. All rights reserved.
//

#ifndef __Project1__Polygon__
#define __Project1__Polygon__

#include <deque>
#include <list>
#include <stdexcept>
#include <iostream>
#include "Point3d.h"
#include "Line3d.h"
#include "Vector3i.h"

class Polygon
{
private:
    //deque to maintain stable references between edges and vertices
    deque<Point3d> mVertices;
    deque<Line3d> mEdges;
    bool mSelected;
    
public:
    Polygon();
    Polygon(deque<Vector3i> vertPositions);
    
    deque<Point3d> GetVertices();
    void SetVertex(int index, Vector3i pos);
    void SetVertices(deque<Point3d> verts);

    deque<Line3d> GetEdges();
    
    void SetSelected(bool isSelected);
    bool IsSelected();
};
#endif /* defined(__Project1__Polygon__) */
