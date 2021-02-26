TARGET      = CG1Application
CONFIG     += console
CONFIG     -= app_bundle
CONFIG     -= qt
OBJECTS_DIR = obj
DESTDIR     = $$PWD
TEMPLATE    = app

SOURCES +=\
    source/CGContext/CGContext.cpp \
    source/CGContext/CGFrameBuffer.cpp \
    source/CGContext/CGMatrix.cpp \
    source/CGContext/CGPrimClipper.cpp \
    source/CGContext/CGTexture.cpp \
    source/CGContext/CGVertexOperations.cpp \
    source/CGContext/CGPrimitiveOperations.cpp \
    source/CGContext/CGFragmentOperations.cpp \
    source/CGContext/CGRasterizer_point.cpp \
    source/CGContext/CGRasterizer_line_stupid.cpp \
    source/CGContext/CGRasterizer_line_bresenham.cpp \
    source/CGContext/CGRasterizer_triangle.cpp \
    source/Utilities/CGImageFile.cpp \
    source/Utilities/CGQuadric.cpp \
    source/Utilities/Deer.cpp \
    source/Utilities/Snowflakes.cpp \
    source/Utilities/CG1Helper/CG1Helper.cpp \
    dependencies/glad/src/glad.c \
    source/Applications/CG1Main.cpp \
    source/CGContext/CGRasterizer_line.cpp \
    source/Applications/Uebungen/U_TestRotatingTriangle.cpp \
    source/Applications/Uebungen/U_FourPoints.cpp \
    source/Applications/Uebungen/U_JumpingLine.cpp \
    source/Applications/Uebungen/U_LineBenchmark.cpp \
    source/Applications/Uebungen/U_TestBresenham.cpp \
    source/Applications/Uebungen/U_TestViewport.cpp \
    source/Applications/Uebungen/U_TestProjection.cpp \
    source/Applications/Uebungen/U_HappyHolidays.cpp \
    source/Applications/Uebungen/U_AwesomeTriangle.cpp \
    source/Applications/Uebungen/U_Texturing.cpp \
    source/Applications/Uebungen/U_Lighting.cpp \
    source/Applications/Uebungen/U_DottedLine.cpp \
    source/Applications/Uebungen/U_EndU3Test.cpp

HEADERS +=\
    source/CGContext/CG.h \
    source/CGContext/CGContext.h \
    source/CGContext/CGFrameBuffer.h \
    source/CGContext/CGMath.h \
    source/CGContext/CGMatrix.h \
    source/CGContext/CGPrimClipper.h \
    source/CGContext/CGProgramInterface.h \
    source/CGContext/CGTexture.h \
    source/CGContext/CGVector.h \
    source/CGContext/CGVertexOperations.h \
    source/CGContext/CGProgram.h \
    source/CGContext/CGPrimitiveOperations.h \
    source/CGContext/CGRasterizer.h \
    source/CGContext/CGFragmentOperations.h \
    source/CGContext/CGRectangle.h \
    source/Utilities/CGImageFile.h \
    source/Utilities/CGQuadric.h \
    source/Utilities/Deer.h \
    source/Utilities/DeerVertexData.h \
    source/Utilities/Snowflakes.h \
    source/Utilities/CG1Helper/CG1Helper.h \
    source/Utilities/CG1Helper/glutil.h \
    source/ShaderPrograms/Program_modelViewProjection.h \
    source/ShaderPrograms/Program_passThrough.h \
    source/ShaderPrograms/Program_perVertexLighting.h \
    source/ShaderPrograms/Program_phong.h \
    source/ShaderPrograms/Program_projectVertex.h \
    source/ShaderPrograms/Program_testVertex.h \
    source/Applications/Application.h \
    source/Applications/Uebungen/U_FourPoints.h \
    source/Applications/Uebungen/U_JumpingLine.h \
    source/Applications/Uebungen/U_LineBenchmark.h \
    source/Applications/Uebungen/U_TestBresenham.h \
    source/Applications/Uebungen/U_TestRotatingTriangle.h \
    source/Applications/Uebungen/U_TestViewport.h \
    source/Applications/Uebungen/U_TestProjection.h \
    source/Applications/Uebungen/U_HappyHolidays.h \
    source/Applications/Uebungen/U_AwesomeTriangle.h \
    source/Applications/Uebungen/U_Lighting.h \
    source/Applications/Uebungen/U_Texturing.h \
    source/Applications/Uebungen/U_DottedLine.h \
    source/Applications/Uebungen/U_EndU3Test.h

CONFIG += c++11

MAKEFILE = Makefile_qmake.$${TARGET}

CONFIG(release, debug|release): DEFINES += NDEBUG
INCLUDEPATH += dependencies/glad/include source dependencies

unix{
    LIBS += -lm -pthread -ldl -fopenmp
    #INCLUDEPATH += dependencies/glad/include source
    QMAKE_CXXFLAGS += -fopenmp -Wno-old-style-cast -Wno-unused-parameter
}

include(./dependencies/GLFW/glfw.pri)

win32{
    contains(QMAKE_COMPILER_DEFINES, __GNUC__) {
        message("experimental MinGW support");
        MINGW=1
    } else {
        MINGW=0
    }




}
