//
//  Point3d.cpp
//  Source
//
//  Created by Brandon Nguyen on 10/14/15.
//  Copyright © 2015 Brandon Nguyen. All rights reserved.
//

#include "Point3d.h"
Point3d::Point3d(){}

Point3d::Point3d(Vector3i pos)
{
    mPos = pos;
}

Point3d::Point3d(Vector3i pos, Color color)
{
    mPos = pos;
    mColor = color;
}

Point3d::Point3d(int x, int y, int z)
{
    mPos.mX = x;
    mPos.mY = y;
    mPos.mZ = z;
}

Point3d::Point3d(int x, int y, int z, Color color)
{
    mPos.mX = x;
    mPos.mY = y;
    mPos.mZ = z;
    mColor = color;
}

Vector3i Point3d::GetPos()
{
    return mPos;
}