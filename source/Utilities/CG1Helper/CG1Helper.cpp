//This class is responsible for setting up and maintaining the actual application
//It should not be neccessary to change anything in here or to look at it at all.

#include "CG1Helper.h"
#include "glutil.h"

static const GLchar *vertexShaderTexturedQuad=
		"ATTR_IN vec2 pos;\n"
		"VAR_OUT vec2 v_tex;\n"
		"void main()\n"
		"{\n"
		"	v_tex=0.5*pos + vec2(0.5);\n"
		"	gl_Position=vec4(pos, 0.0, 1.0);\n"
		"}\n";

static const GLchar *fragmentShaderTexturedQuad=
		"VAR_IN vec2 v_tex;\n"
		"uniform sampler2D texSampler;\n"
		"FS_OUT_DECL\n"
		"void main()\n"
		"{\n"
		"	FS_OUT=TEX2D(texSampler, v_tex);\n"
		"}\n";

static const GLchar *vertexShaderGrid=
		"ATTR_IN vec2 pos;\n"
		"uniform vec2 dims;\n"
		"void main()\n"
		"{\n"
		"	//snap-in to pixel centers (move towards viewport center)\n"
		"	gl_Position=vec4(((floor((0.5*pos+vec2(0.5))*dims - 0.25*normalize(pos))+vec2(0.5))/dims)*2.0-vec2(1.0), 0.0, 1.0);\n"
		"}\n";

static const GLchar *fragmentShaderGrid=
		"FS_OUT_DECL\n"
		"void main()\n"
		"{\n"
                "	FS_OUT=vec4(0.5);\n"
		"}\n";

static void
print_info_log(GLuint obj, bool program)
{
	char log[16384];
	if (program) {
		glGetProgramInfoLog(obj, 16383, nullptr, log);
	} else {
		glGetShaderInfoLog(obj, 16383, nullptr, log);
	}
	log[16383]=0;
	fprintf(stderr,"%s\n",log);
}

/* Wrapper for glfwProcAddresss (which is a wrapper around the platform's
 * wgl/glX/whatever GetProcAddress) for use with the GLAD loader...
 */
static void *
wrap_getprocaddress(const char *name, void *user_ptr)
{
	(void)user_ptr; // unusued
	return (void*)glfwGetProcAddress(name);
}


CGVec2 CG1Helper::get_raw_cursor_pos()
{
	CGVec2 pos;
	double mx, my;
	glfwGetCursorPos(smWindow, &mx, &my);
	pos.set((float)mx, (float)my);
	return pos;
}

CGVec2 CG1Helper::convert_cursor_pos(const CGVec2& raw)
{
	CGVec2 pos;
	pos[0] = ((raw[0] + 0.5f) / (float)glcontextWidth) * (float)smApplication->context.m_frameBuffer.getWidth();
	pos[1] = ((raw[1] + 0.5f) / (float)glcontextHeight) * (float)smApplication->context.m_frameBuffer.getHeight();
	return pos;
}

GLuint CG1Helper::shader_create_and_compile(GLenum type, const GLchar *source)
{
	static const GLchar *glsl110_header=
			"#define ATTR_IN attribute\n"
			"#define VAR_OUT varying\n"
			"#define VAR_IN varying\n"
			"#define FS_OUT_DECL\n"
			"#define FS_OUT gl_FragColor\n"
			"#define TEX2D texture2D\n\n";
	static const GLchar *glsl150_header=
			"#version 150 core\n\n"
			"#define ATTR_IN in\n"
			"#define VAR_OUT out\n"
			"#define VAR_IN in\n"
			"#define FS_OUT_DECL out vec4 color;\n"
			"#define FS_OUT color\n"
			"#define TEX2D texture\n\n";
	GLuint shader=0;
	GLint status;
	const GLchar *src[2];

	if (glcaps.supports(CGLCaps::SHADERS_150)) {
		src[0]=glsl150_header;
	} else {
		src[0]=glsl110_header;
	}
	src[1]=source;

	shader=glCreateShader(type);
	glShaderSource(shader, 2, src, nullptr);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		fprintf(stderr,"Failed to compile shader\n");
		print_info_log(shader,false);
		glDeleteShader(shader);
		shader=0;
	}

	return shader;
}

GLuint CG1Helper::program_create(GLuint vertex_shader, GLuint fragment_shader)
{
	GLuint program=0;
	GLint status;

	program=glCreateProgram();

	if (vertex_shader)
		glAttachShader(program, vertex_shader);
	if (fragment_shader)
		glAttachShader(program, fragment_shader);

	glBindAttribLocation(program, TGL_ATTR_POSITION, "pos");

	if (glcaps.supports(CGLCaps::SHADERS_150)) {
		glBindFragDataLocation(program, 0, "color");
	}

	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
		fprintf(stderr,"Failed to link program!\n");
		print_info_log(program,true);
		glDeleteProgram(program);
		return 0;
	}
	return program;
}

GLenum CG1Helper::program_create_vs_fs(const GLchar *vs, const GLchar *fs)
{
	GLuint id_vs=shader_create_and_compile(GL_VERTEX_SHADER, vs);
	GLuint id_fs=shader_create_and_compile(GL_FRAGMENT_SHADER, fs);
	GLuint program=program_create(id_vs,id_fs);
	glDeleteShader(id_vs);
	glDeleteShader(id_fs);
	return program;
}

bool CG1Helper::initApplication(Application* application, int windowScale)
{
	const GLubyte *vendor, *renderer, *version;
	int wh, hh;

	const CGVec2i res =  application->context.viewport.topRight()
	                -application->context.viewport.bottomLeft()
	                +CGVec2i(1,1);


	const int& width = res.x;
	const int& height = res.y;

	if (!glfwInit()) {
		return false;
	}
	int scale = windowScale>0?windowScale:1;
	wh=width*scale;
	hh=height*scale;
	if (scale >= CG_GRID_MIN_SCALE) {
		wh++;
		hh++;
	}

	// request a moden GL 3.2 core context
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	smWindow=glfwCreateWindow( wh, hh, "CG1 Template", nullptr, nullptr);
	if (!smWindow) {
		// request a legacy GL context
		fprintf(stderr, "Failed to create modern OpenGL >= 3.2 core context, trying fallback\n");
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
		smWindow=glfwCreateWindow( wh, hh, "CG1 Template", nullptr, nullptr);
	}
	if (!smWindow) {
		fprintf(stderr, "Failed to create OpenGL context\n");
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(smWindow);

	if (!gladLoadGL(wrap_getprocaddress, nullptr)) {
		fprintf(stderr,"failed to intialize glad\n");
		return false;
	}

	// check what we got
	version=glGetString(GL_VERSION);
	vendor=glGetString(GL_VENDOR);
	renderer=glGetString(GL_RENDERER);
	fprintf(stderr, "OpenGL %s %s %s\n",version,vendor,renderer);
	if (!GLAD_GL_VERSION_2_0) {
		fprintf(stderr,"at least OpenGL 2.0 is required\n");
		glfwDestroyWindow(smWindow);
		glfwTerminate();
		return false;
	}

	glcaps.set();

	glfwSetWindowSizeCallback(smWindow, m_reshapeFunc);
	glfwSetFramebufferSizeCallback(smWindow, m_frambufferResizeFunc);
	glfwSetKeyCallback(smWindow, m_keyboardFunc);
	glfwSetScrollCallback(smWindow, m_scrollFunc);
	glfwSetCursorPos(smWindow, (double)wh/2.0, (double)hh/2.0);
	glfwSetMouseButtonCallback(smWindow,m_mouseButtonFunc);


	smApplication = application;

	// setup texture which is used as CG framebuffer
	glGenTextures(1, &FBTexture);
	glBindTexture(GL_TEXTURE_2D,FBTexture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glBindTexture(GL_TEXTURE_2D,0);
	CGLHelper::CheckGLErrors("GL initialization: texture");
	if (glcaps.supports(CGLCaps::PBO)) {
		glGenBuffers(1, &pbo_upload);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo_upload);
		glBufferData(GL_PIXEL_UNPACK_BUFFER, 4*width*height, nullptr, GL_STREAM_DRAW);
		CGLHelper::CheckGLErrors("GL initialization: PBO");
	}

	// setup state for drawing grid: shader + VAO + VBO
	programGrid=program_create_vs_fs(vertexShaderGrid, fragmentShaderGrid);
	glUseProgram(programGrid);
	loc_dims=glGetUniformLocation(programGrid, "dims");

	vaoGrid.create(glcaps.supports(CGLCaps::VAO));
	vaoGrid.bind();
	glGenBuffers(1,&vboGrid);
	vaoGrid.BindBuffer(GL_ARRAY_BUFFER, vboGrid);
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)sizeof(GLfloat)*4*(width+1 + height+1), nullptr, GL_STATIC_DRAW);
	GLfloat *buf=(GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	for (int dx=0; dx<=width; dx++) {
		buf[dx*4+0]=-1.0f + 2.0f * (float)dx/(float)width;
		buf[dx*4+1]=-1.0f;
		buf[dx*4+2]=-1.0f + 2.0f * (float)dx/(float)width;;
		buf[dx*4+3]= 1.0f;
	}
	buf += (width+1)*4;
	for (int dy=0; dy<=height; dy++) {
		buf[dy*4+0]=-1.0f;
		buf[dy*4+1]=-1.0f + 2.0f * (float)dy/(float)height;
		buf[dy*4+2]= 1.0f;
		buf[dy*4+3]=-1.0f + 2.0f * (float)dy/(float)height;
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	vaoGrid.VertexAttribPointer(TGL_ATTR_POSITION,2,GL_FLOAT,GL_FALSE,2*sizeof(GLfloat),0);
	vaoGrid.EnableVertexAttribArray(TGL_ATTR_POSITION);
	vaoGrid.unbind();
	CGLHelper::CheckGLErrors("GL initialization: grid");

	// setup state for drawing a textured quad: shader + VAO + VBO
	vaoQuad.create(glcaps.supports(CGLCaps::VAO));
	vaoQuad.bind();
	glGenBuffers(1,&vboQuad);
	vaoQuad.BindBuffer(GL_ARRAY_BUFFER, vboQuad);
	static const GLfloat quadVertices[]={
		-1.0f, -1.0f,
		1.0f, -1.0f,
		-1.0f,  1.0f,
		1.0f,  1.0f};
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*2*4, quadVertices, GL_STATIC_DRAW);
	vaoQuad.VertexAttribPointer(TGL_ATTR_POSITION,2,GL_FLOAT,GL_FALSE,2*sizeof(GLfloat),0);
	vaoQuad.EnableVertexAttribArray(TGL_ATTR_POSITION);
	programTexturedQuad=program_create_vs_fs(vertexShaderTexturedQuad, fragmentShaderTexturedQuad);
	glUseProgram(programTexturedQuad);
	GLint loc_texSampler=glGetUniformLocation(programTexturedQuad, "texSampler");
	glUniform1i(loc_texSampler, 0);

	CGLHelper::CheckGLErrors("GL initialization: quad");

	glClearColor(0.0f,0.0f,0.0f,1.0f);

	m_reshapeFunc(smWindow, wh, hh);
	m_frambufferResizeFunc(smWindow, wh, hh);

	rawMousePosLast=rawMousePos=get_raw_cursor_pos();
	mousePos=convert_cursor_pos(rawMousePos);
	mouseDelta.set(0.0f);
	return true;
}

void CG1Helper::runApplication()
{
	while (!exitRequested && !glfwWindowShouldClose(smWindow)) {
		glfwPollEvents();
		m_displayFunc();
#ifndef NDEBUG
		CGLHelper::CheckGLErrors("main loop");
#endif
	}

	vaoQuad.destroy();
	vaoGrid.destroy();


	smApplication = nullptr;

	glfwDestroyWindow(smWindow);
	smWindow = nullptr;
	glfwTerminate();
}


void CG1Helper::m_displayFunc()
{
	char WinTitle [100];

	// get mouse cursor position
	rawMousePos=get_raw_cursor_pos();
	mousePos=convert_cursor_pos(rawMousePos);
	CGVec2 mousePosLast=convert_cursor_pos(rawMousePosLast);
	mouseDelta[0]=mousePos[0] - mousePosLast[0];
	mouseDelta[1]=mousePos[1] - mousePosLast[1];
	rawMousePosLast=rawMousePos;

	// update wheel position delta
	wheelDelta[0] = wheelPos[0] - wheelPosLast[0];
	wheelDelta[1] = wheelPos[1] - wheelPosLast[1];
	wheelPosLast = wheelPos;

	glClear (GL_COLOR_BUFFER_BIT);
	double time_start=glfwGetTime();
	curTime = (float)time_start;
	curTimeDelta = (float)(time_start - lastTime);
	lastTime = time_start;

	smApplication->program_step();

	double time_end=glfwGetTime();
	double draw_time=time_end - time_start;

	frameCnt++;
	accDrawTime += draw_time;

	if (time_end - lastFPSTime > 0.5) {
		// Update FPS stats
		draw_time = 1000.0 * ( accDrawTime / (double)frameCnt);
		double fps = (double)frameCnt / (time_end - lastFPSTime);
		frameCnt=0;
		lastFPSTime = time_end;
		accDrawTime = 0.0;
#ifdef _MSC_VER
		sprintf_s(WinTitle, sizeof(WinTitle),"CG1 Template /// %3.1f FPS (%3.4f ms per actual draw)", fps, draw_time);
#else
		snprintf(WinTitle, sizeof(WinTitle),"CG1 Template /// %3.1f FPS (%3.4f ms per actual draw)", fps, draw_time);
#endif
		glfwSetWindowTitle(smWindow, WinTitle);
	}
	const auto& smFrameBuffer = &smApplication->context.m_frameBuffer;

	if(smFrameBuffer->colorBuffer.getDataPointer()) {
		glBindTexture(GL_TEXTURE_2D,FBTexture);
		if (glcaps.supports(CGLCaps::PBO)) {
			// transfer it via PBO
			glBufferSubData(GL_PIXEL_UNPACK_BUFFER, 0, smFrameBuffer->getWidth() * smFrameBuffer->getHeight()*4, smFrameBuffer->colorBuffer.getDataPointer());
			glTexSubImage2D(GL_TEXTURE_2D,0,0,0,smFrameBuffer->getWidth(),smFrameBuffer->getHeight(),GL_RGBA,GL_UNSIGNED_BYTE,nullptr);
		} else {
			// directly copy the CG1 framebuffer into the texture
			glTexSubImage2D(GL_TEXTURE_2D,0,0,0,smFrameBuffer->getWidth(),smFrameBuffer->getHeight(),GL_RGBA,GL_UNSIGNED_BYTE,smFrameBuffer->colorBuffer.getDataPointer());
		}

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		int scale = glcontextWidth/smFrameBuffer->getWidth();
		if (scale>=CG_GRID_MIN_SCALE) {
			// draw the grid
			vaoGrid.bind();
			glUseProgram(programGrid);
			glDrawArrays(GL_LINES, 0, 2*(smFrameBuffer->getWidth()+1+smFrameBuffer->getHeight()+1));
			vaoQuad.bind();
			glUseProgram(programTexturedQuad);
		}
	}

	glfwSwapBuffers(smWindow);
}

void CG1Helper::m_reshapeFunc(GLFWwindow *win, int w, int h)
{
	const auto& smFrameBuffer = &smApplication->context.m_frameBuffer;
	// We allow only full multiples of the framebuffer size smaller than the given size:
	int scale = w/smFrameBuffer->getWidth();
	int fw=smFrameBuffer->getWidth();
	int fh=smFrameBuffer->getHeight();
	if(scale<=0)
		scale = 1;

	glcontextWidth  = scale*fw;
	glcontextHeight = scale*fh;
	if (scale >= CG_GRID_MIN_SCALE) {
		glcontextWidth++;
		glcontextHeight++;
	}

	if ( (w != glcontextWidth) || (h != glcontextHeight) ) {
		glfwSetWindowSize(win, glcontextWidth, glcontextHeight);
	}
}

void CG1Helper::m_frambufferResizeFunc(GLFWwindow * , int w, int h)
{
	glViewport (0, 0, w, h);
	glUseProgram(programGrid);
	glUniform2f(loc_dims, (GLfloat)(w),(GLfloat)(h));
	glUseProgram(programTexturedQuad);
}

void CG1Helper::m_keyboardFunc (GLFWwindow *win, int key, int , int action, int mods)
{
	if (key < 0 || key > GLFW_KEY_LAST) {
		return;
	}

	if ( !(mods & GLFW_MOD_SHIFT) && key > 0 && key < 256) {
		key = tolower(key);
	}

	if (action == GLFW_RELEASE) {
		pressedKeys[key] = false;
		releasedKeys[key] = true;
	} else {
		if (!pressedKeys[key]) {
			switch (key) {
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(win,1);
				break;
			case GLFW_KEY_PAUSE:
				setMouseModeFPS(Toggle);
				break;
			}
		}
		pressedKeys[key] = true;
	}
}

void CG1Helper::m_scrollFunc (GLFWwindow *, double xoffset, double yoffset)
{
	wheelPos[0] += (float)(xoffset);
	wheelPos[1] += (float)(yoffset);
}

void CG1Helper::m_mouseButtonFunc(GLFWwindow *, int button, int action, int )
{
	if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST) {
		return;
	}

	if (action == GLFW_RELEASE) {
		pressedButtons[button] = false;
		releasedButtons[button] = true;
	} else {
		if (!pressedButtons[button]) {
		}
		pressedButtons[button] = true;
	}
}

void CG1Helper::setMouseModeFPS(Tristate state)
{
	if (state == Toggle) {
		state = mouseModeFPS ? Off : On;
	}
	if (state != mouseModeFPS) {
		if (state == On) {
			glfwSetInputMode(smWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		} else {
			glfwSetInputMode(smWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		mouseModeFPS=state;
	}
}

GLFWwindow *CG1Helper::smWindow=nullptr;
Application* CG1Helper::smApplication = nullptr;
GLuint CG1Helper::FBTexture = 0;
GLuint CG1Helper::programTexturedQuad = 0;
GLuint CG1Helper::vboQuad = 0;
GLuint CG1Helper::programGrid = 0;
GLuint CG1Helper::vboGrid = 0;
GLint CG1Helper::loc_dims = -1;
GLuint CG1Helper::pbo_upload = 0;
int CG1Helper::glcontextWidth = 0;
int CG1Helper::glcontextHeight = 0;
bool CG1Helper::pressedKeys[GLFW_KEY_LAST+1] = {0,};
bool CG1Helper::releasedKeys[GLFW_KEY_LAST+1] = {0,};
bool CG1Helper::pressedButtons[GLFW_MOUSE_BUTTON_LAST+1] = {0,};
bool CG1Helper::releasedButtons[GLFW_MOUSE_BUTTON_LAST+1] = {0,};
CGLCaps CG1Helper::glcaps;
CGLVAO CG1Helper::vaoQuad;
CGLVAO CG1Helper::vaoGrid;
CGVec2 CG1Helper::rawMousePos;
CGVec2 CG1Helper::rawMousePosLast;
CGVec2 CG1Helper::mousePos;
CGVec2 CG1Helper::mouseDelta;
CGVec2 CG1Helper::wheelPos;
CGVec2 CG1Helper::wheelPosLast;
CGVec2 CG1Helper::wheelDelta;
CG1Helper::Tristate CG1Helper::mouseModeFPS=CG1Helper::Off;
float CG1Helper::curTime=0.0f;
float CG1Helper::curTimeDelta=0.0f;
double CG1Helper::lastTime=0.0;
unsigned CG1Helper::frameCnt=0;
double CG1Helper::accDrawTime=0.0;
double CG1Helper::lastFPSTime=0.0;
bool CG1Helper::exitRequested=false;
