#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glApplication.h"
#include "glutWindow.h"
#include <iostream>
#include "glsl.h"
#include <time.h>
#include "glm.h"
#include <FreeImage.h> //*** Para Textura: Incluir librería


class models
{
private:
	GLMmodel* myModel;
	cwc::glShader* myShader;
	GLuint myTexture;
public:
	models(char* filePath, cwc::glShader* shader, GLuint texture);
	void draw(float x, float y, float z);
};


