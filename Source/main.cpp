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

float *PixelBuffer;

int main(int argc, char *argv[])
{    
    Renderer::Instance()->InitWindow(800, 800);
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    Vector2i screenSize = Renderer::Instance()->GetScreenSize();
    glutInitWindowSize(screenSize.mX, screenSize.mY);
    glutInitWindowPosition(100, 100);
    
    glutCreateWindow("Project 2");
    glClearColor(0, 0, 0, 0); //clears the buffer of OpenGL
    glutDisplayFunc(Renderer::DisplayPixelBuffer);
    glutKeyboardFunc(InputOutputUtility::DetectInput);
    
    glutMainLoop();//main display loop, will display until terminate
    
    return 0;
}
