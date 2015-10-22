//
//  main.cpp
//  Project1
//
//  Created by Brandon Nguyen on 9/25/15.
//  Copyright (c) 2015 Brandon Nguyen. All rights reserved.
//

 #include <OpenGL/gl.h>
 #include <OpenGL/glu.h>
 #include <GLUT/glut.h>
//#include <GL/glut.h>
#include "Vector2i.h"
#include "Renderer.h"
#include "ObjectEditor.h"
#include "InputOutputUtility.h"
#include "Matrix.h"

int main(int argc, char *argv[])
{
    //testing matrices
    
//    cout << "Matrix A" << endl;
//    deque<float> aDeque;
//    for(int i = 1; i < 17; i++)
//    {
//        aDeque.push_back(i);
//    }
//    Matrix a = Matrix(4,4,aDeque);
//    a.PrintMatrix();
//    cout << endl;
//
//    
//    cout << "Matrix B" << endl;
//    deque<float> bDeque;
//    for(int i = 5; i < 21; i++)
//    {
//        bDeque.push_back(i);
//    }
//    Matrix b = Matrix(4,4,bDeque);
//    b.PrintMatrix();
//    cout << endl;
//    
//    cout << "Multiply A and B" << endl;
//    a.Multiply(b).PrintMatrix();
//    cout << endl;
//    
//    
//    cout << "Multiply A and vect" << endl;
//    Vector3 vect = Vector3(1, 2, 3);
//    Vector3 result = a.MultiplyVector(vect);
//    cout << result.mX << " " << result.mY << " " << result.mZ << endl;
//    cout << endl;
//    
//    cout << "Inverse c" << endl;
//    Matrix inverse = Matrix(4,4);
//    deque<float> cDeque;
//    cDeque.push_back(1);cDeque.push_back(1);cDeque.push_back(1);cDeque.push_back(0);
//    cDeque.push_back(0);cDeque.push_back(3);cDeque.push_back(1);cDeque.push_back(2);
//    cDeque.push_back(2);cDeque.push_back(3);cDeque.push_back(1);cDeque.push_back(0);
//    cDeque.push_back(1);cDeque.push_back(0);cDeque.push_back(2);cDeque.push_back(1);
//    Matrix c = Matrix(4,4,cDeque);
//    if(c.Invert4by4(&inverse))
//        inverse.PrintMatrix();
//    else
//        cout << "No inverse exists" << endl;
//    cout << endl;
        
    Renderer::Instance()->InitWindow(600, 600);
    Vector2i screenSize = Renderer::Instance()->ScreenSize();
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(screenSize.mX, screenSize.mY);
    glutInitWindowPosition(100, 100);
    
    Renderer::sMainWindow = glutCreateWindow("Project 2");
    glutDisplayFunc(Renderer::DisplayPixelBuffer);
    glutKeyboardFunc(InputOutputUtility::DetectInput);
    
    //bottom left
    Renderer::sSubWindow1 = glutCreateSubWindow(Renderer::sMainWindow, 0, screenSize.mY/2, screenSize.mX/2, screenSize.mY/2);
    glutDisplayFunc(Renderer::DisplayPixelBuffer1);
    glutKeyboardFunc(InputOutputUtility::DetectInput);
    
    //bottom right
    Renderer::sSubWindow2 = glutCreateSubWindow(Renderer::sMainWindow, screenSize.mX/2, screenSize.mY/2, screenSize.mX/2, screenSize.mY/2);
    glutDisplayFunc(Renderer::DisplayPixelBuffer2);
    glutKeyboardFunc(InputOutputUtility::DetectInput);
    
    //top left
    Renderer::sSubWindow3 = glutCreateSubWindow(Renderer::sMainWindow, 0, 0, screenSize.mX/2, screenSize.mY/2);
    glutDisplayFunc(Renderer::DisplayPixelBuffer3);
    glutKeyboardFunc(InputOutputUtility::DetectInput);
    
    glClearColor(0, 0, 0, 0);

    glutMainLoop();//main display loop, will display until terminate
    
    return 0;
}
