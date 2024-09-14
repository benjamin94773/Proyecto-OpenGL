//=============================================================================
// Sample Application: Lighting (Per Fragment Phong)
//=============================================================================

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glApplication.h"
#include "glutWindow.h"
#include <iostream>
#include "glsl.h"
#include <time.h>
#include "glm.h"
#include <FreeImage.h> //*** Para Textura: Incluir librería
#include "models.h"

//-----------------------------------------------------------------------------


class myWindow : public cwc::glutWindow
{
protected:
	cwc::glShaderManager SM;
	cwc::glShader* myShader[5];
	GLuint ProgramObject;
	clock_t time0, time1;
	float timer010;  // timer counting 0->1->0
	bool bUp;        // flag if counting up or down.

	models* volcan, * torre, * persona, * ojo, * mesa;

	//GLMmodel* myModel[5]; // arreglo de objetos.
	GLuint texid; //*** Para Textura: variable que almacena el identificador de textura


public:
	myWindow() {}

	void loadShader(int ind, char* vertexFileName, char* fragmentFileName) {
		myShader[ind] = SM.loadfromFile(vertexFileName, fragmentFileName); // load (and compile, link) from file
		if (myShader[ind] == 0)
			std::cout << "Error Loading, compiling or linking shader\n";
		else
		{
			ProgramObject = myShader[ind]->GetProgramObject();
		}
	}


	//*** Para Textura: aqui adiciono un método que abre la textura en JPG
	GLuint initialize_texture(char* fileName)
	{
		GLuint texid;
		glGenTextures(1, &texid);
		glBindTexture(GL_TEXTURE_2D, texid);
		glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		// Loading JPG file
		FIBITMAP* bitmap = FreeImage_Load(
			FreeImage_GetFileType(fileName, 0),
			fileName);  //*** Para Textura: esta es la ruta en donde se encuentra la textura

		FIBITMAP* pImage = FreeImage_ConvertTo32Bits(bitmap);
		int nWidth = FreeImage_GetWidth(pImage);
		int nHeight = FreeImage_GetHeight(pImage);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight,
			0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));

		FreeImage_Unload(pImage);
		glEnable(GL_TEXTURE_2D);
		return texid;
	}


	virtual void OnRender(void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//timer010 = 0.09; //for screenshot!

		glPushMatrix();
		glRotatef(timer010 * 360, 0.0f, 1.0f, 0.0f);

		volcan->draw(0.0, 0.25, 1.0);
		torre->draw(0.0, 0.25, -0.25);
		persona->draw(0.0, 0.6, 1.25);
		ojo->draw(0.0, 0.6, -0.25);
		mesa->draw(0.0, 0.0, 0.0);

		glutSwapBuffers();
		glPopMatrix();

		UpdateTimer();
		Repaint();
	}

	virtual void OnIdle() {}

	// When OnInit is called, a render context (in this case GLUT-Window) 
	// is already available!
	virtual void OnInit()
	{
		glClearColor(0.5f, 0.5f, 1.0f, 0.0f);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);

		loadShader(0, "vertexShader.txt", "fragmentShader.txt");
		loadShader(1, "vertexShaderT.txt", "fragmentShaderT.txt");

		time0 = clock();
		timer010 = 0.0f;
		bUp = true;

		GLuint skin = initialize_texture("./Mallas/mono.png");

		volcan = new models("./Mallas/volcan.obj", myShader[0], -1);
		torre = new models("./Mallas/torre2.obj", myShader[0], -1);
		persona = new models("./Mallas/persona2.obj", myShader[1], skin);
		ojo = new models("./Mallas/ojo2.obj", myShader[0], -1);
		mesa = new models("./Mallas/mesa.obj", myShader[0], -1);

		DemoLight();

	}

	virtual void OnResize(int w, int h)
	{
		if (h == 0) h = 1;
		float ratio = 1.0f * (float)w / (float)h;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glViewport(0, 0, w, h);

		gluPerspective(45, ratio, 1, 100);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0.0f, 0.0f, 4.0f,
			0.0, 0.0, -1.0,
			0.0f, 1.0f, 0.0f);
	}
	virtual void OnClose(void) {}
	virtual void OnMouseDown(int button, int x, int y) {}
	virtual void OnMouseUp(int button, int x, int y) {}
	virtual void OnMouseWheel(int nWheelNumber, int nDirection, int x, int y) {}

	virtual void OnKeyDown(int nKey, char cAscii)
	{
		if (cAscii == 27) // 0x1b = ESC
		{
			this->Close(); // Close Window!
		}
	};

	virtual void OnKeyUp(int nKey, char cAscii)
	{
		if (cAscii == 's')      // s: Shader
			myShader[0]->enable();
		else if (cAscii == 'f') // f: Fixed Function
			myShader[0]->disable();
	}

	void UpdateTimer()
	{
		time1 = clock();
		float delta = static_cast<float>(static_cast<double>(time1 - time0) / static_cast<double>(CLOCKS_PER_SEC));
		delta = delta / 4;
		if (delta > 0.00005f)
		{
			time0 = clock();
			if (bUp)
			{
				timer010 += delta;
				if (timer010 >= 1.0f) { timer010 = 1.0f; bUp = false; }
			}
			else
			{
				timer010 -= delta;
				if (timer010 <= 0.0f) { timer010 = 0.0f; bUp = true; }
			}
		}
	}

	void DemoLight(void)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_NORMALIZE);
	}
};

//-----------------------------------------------------------------------------

class myApplication : public cwc::glApplication
{
public:
	virtual void OnInit() { std::cout << "Hello World!\n"; }
};

//-----------------------------------------------------------------------------

int main(void)
{
	myApplication* pApp = new myApplication;
	myWindow* myWin = new myWindow();

	pApp->run();
	delete pApp;
	return 0;
}

//-----------------------------------------------------------------------------

