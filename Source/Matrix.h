//
//  Matrix.h
//  Source
//
//  Created by Brandon Nguyen on 10/21/15.
//  Copyright © 2015 Brandon Nguyen. All rights reserved.
//
//  4x4Inverse code taken from: http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
#ifndef Matrix_h
#define Matrix_h

#include <iostream>
#include <deque>
#include <exception>
#include "Vector3.h"
using namespace std;

class Matrix
{
private:
    float** mMatrix;
    int mRows;
    int mCols;
public:
    Matrix(int rows, int cols);
    Matrix(int rows, int cols, deque<float> matrix);
    Matrix(Vector3 vector);
    ~Matrix();
    
    Matrix Identity(int rows, int cols);
    
    Matrix Multiply(Matrix other);
    Vector3 MultiplyVector(Vector3 other);
    bool Invert4by4(Matrix *out);
    
    void PrintMatrix()
    {
        cout << "Printing matrix" << endl;
        
        for(int i = 0; i < mRows; i++)
        {
            for(int j = 0; j < mCols; j++)
            {
                cout << mMatrix[i][j] << " ";
            }
            cout << endl;
        }
    }
};
#endif /* Matrix_h */
