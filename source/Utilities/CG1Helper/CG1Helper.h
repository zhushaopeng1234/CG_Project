#pragma once
//This class is responsible for setting up and maintaining the actual application
//It should not be neccessary to change anything in here or to look at it at all.


#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glutil.h"

#include <stdio.h>
#include <stdlib.h>

#include "Applications/Application.h"
#include "CGContext/CG.h"

#ifndef M_PI    // for Windows
#define M_PI 3.14159265358979323846
#endif

#define CG_GRID_MIN_SCALE 5


class CG1Helper
{
public:
	typedef enum {
		Off = 0,
		On,
		Toggle
	} Tristate;

	CG1Helper(){}
	~CG1Helper(){}


	static bool initApplication(Application *app, int windowScale=1);
	static void runApplication();
	static void exitAppilication() {exitRequested=true;}
	static bool isKeyPressed(int key) { return (key < 0 || key > GLFW_KEY_LAST) ? false:pressedKeys[key];}
	static bool isKeyReleased(int key){ if (key < 0 || key > GLFW_KEY_LAST) return false; bool t = releasedKeys[key]; releasedKeys[key] = false; return t;}
	static CGVec2 getMousePos() {return mousePos;}
	static CGVec2 getMouseDelta() {return mouseDelta;}
	static CGVec2 getWheelPos() {return wheelPos;}
	static CGVec2 getWheelDelta() {return wheelDelta;}
	static bool isMouseButtonPressed(int button) {return (button < 0 || button > GLFW_MOUSE_BUTTON_LAST) ? false:pressedButtons[button];}
	static bool isMouseButtonReleased(int button) { if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST) return false; bool t = releasedButtons[button]; releasedButtons[button] = false; return t;}
	static void setMouseModeFPS(Tristate state);
	static float getTime() {return curTime;}
	static float getTimeDelta() {return curTimeDelta;}

private:
	static CGVec2 get_raw_cursor_pos();
	static CGVec2 convert_cursor_pos(const CGVec2& raw);
	static GLuint shader_create_and_compile(GLenum type, const GLchar *source);
	static GLuint program_create(GLuint vertex_shader, GLuint fragment_shader);
	static GLenum program_create_vs_fs(const GLchar *vs, const GLchar *fs);
	static void m_displayFunc();
	static void m_reshapeFunc(GLFWwindow *win, int w, int h);
	static void m_frambufferResizeFunc(GLFWwindow *win, int w, int h);
	static void m_keyboardFunc (GLFWwindow *win, int key, int scancode, int action, int mods);
	static void m_scrollFunc (GLFWwindow *win, double xoffset, double yoffset);
	static void m_mouseButtonFunc(GLFWwindow* window, int button, int action, int mods);
	static GLFWwindow *smWindow;
	//static CGFrameBuffer* smFrameBuffer;
	static Application* smApplication;
	static GLuint FBTexture;
	static GLuint programTexturedQuad;
	static CGLVAO vaoQuad;
	static GLuint vboQuad;
	static GLuint programGrid;
	static CGLVAO vaoGrid;
	static GLuint vboGrid;
	static GLint loc_dims;
	static GLuint pbo_upload;
	static int glcontextWidth,glcontextHeight;
	static bool pressedKeys[GLFW_KEY_LAST+1];
	static bool releasedKeys[GLFW_KEY_LAST+1];
	static CGLCaps glcaps;
	static CGVec2 rawMousePos;
	static CGVec2 rawMousePosLast;
	static CGVec2 mousePos;
	static CGVec2 mouseDelta;
	static bool pressedButtons[GLFW_MOUSE_BUTTON_LAST+1];
	static bool releasedButtons[GLFW_MOUSE_BUTTON_LAST+1];
	static CGVec2 wheelPos;
	static CGVec2 wheelPosLast;
	static CGVec2 wheelDelta;
	static Tristate mouseModeFPS;
	static float curTime;
	static float curTimeDelta;
	static double lastTime;
	static unsigned frameCnt;
	static double accDrawTime;
	static double lastFPSTime;
	static bool exitRequested;
};
