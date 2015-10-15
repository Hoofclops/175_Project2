//
//  Point3d.h
//  Source
//
//  Created by Brandon Nguyen on 10/14/15.
//  Copyright © 2015 Brandon Nguyen. All rights reserved.
//

#ifndef Point3d_h
#define Point3d_h

#include "Vector3i.h"
#include "Color.h"

class Point3d
{
private:
    Vector3i mPos;
    Color mColor;
    
public:
    
    Point3d();
    Point3d(Vector3i pos);
    Point3d(Vector3i pos, Color color);
    Point3d(int x, int y, int z);
    Point3d(int x, int y, int z, Color color);
    
    Vector3i GetPos();
    
    int X(){return mPos.mX;}
    void X(int x){mPos.mX = x;}
    int Y(){return mPos.mY;}
    void Y(int y){mPos.mY = y;}
    int Z(){return mPos.mZ;}
    void Z(int z){mPos.mZ = z;}
    
    Color Color(){return mColor;}
    void Color(class Color color){mColor = color;}
};

#endif /* Point3d_h */
