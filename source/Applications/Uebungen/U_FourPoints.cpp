#include "U_FourPoints.h"
#include "Utilities/CG1Helper/CG1Helper.h"
#include <cmath>

App_FourPoints::App_FourPoints(int w, int h)
        :Application(w,h)
{
	position[0][0] = 120;
	position[0][1] = 150;
	position[0][2] = 00;

	position[1][0] = 230;
	position[1][1] = 10;
	position[1][2] = 00;

	position[2][0] = 10;
	position[2][1] = 10;
	position[2][2] = 00;

	position[3][0] = 120;
	position[3][1] = 60;
	position[3][2] = 00;


	color[0][0] = 1.0f;
	color[0][1] = 0.0f;
	color[0][2] = 0.0f;
	color[0][3] = 1.0f;

	color[1][0] = 0.0f;
	color[1][1] = 1.0f;
	color[1][2] = 0.0f;
	color[1][3] = 1.0f;

	color[2][0] = 1.0f;
	color[2][1] = 1.0f;
	color[2][2] = 0.0f;
	color[2][3] = 1.0f;

	color[3][0] = 1.0f;
	color[3][1] = 1.0f;
	color[3][2] = 1.0f;
	color[3][3] = 1.0f;

	// U01 A3a)
	// Hinweise:
	//   - Die Membervariable position repräsentiert ein Array von 3D-Vektoren. X und Y bestimmen
	//     jeweils die Position im Framebuffer, Z ist derzeit ungenutzt.
	//   - Die Membervariable color repräsentiert ein Array von 4D-Vektoren (RGBA-Farbwerte in [0,1]).
	//   - Beide Arrays sind bereits gro?genug für einen vierten Punkt (siehe U_FourPoints.h).
	//   - Denken Sie an F12 zur Erkundung der Codebase.
	// ...

	// U01 A2)
	context.clearColor.set(0.0f,0.0f,0.0f,1.0f);
	context.useProgram(prog);

	// U01 A4b) & A4d)
	context.pointRadius = 20;
}

void App_FourPoints::program_step()
{
	// Vor dem Renderdurchlauf wird der Color Buffer mit der aktuellen Hintergrundfarbe überschrieben.
	context.clear(CG_COLOR_BUFFER_BIT);

	// Renderkommandos (z.B. CGContext::drawArrays) beziehen ihre Daten aus den Vertexattributzeigern.
	context.vertexAttributePointer.position = &position[0][0];
	context.vertexAttributePointer.color = &color[0][0];

	// U01 A3c)
	// Hinweise:
	//   - Der Vertexattributzeiger für Positionsdaten zeigt aktuell auf position (s.o.). Änderungen
	//     am Inhalt dieses Arrays spiegeln sich deshalb direkt im dargestellten Bild wider.
	// ...
	float t_sc = CG1Helper::getTime();
	position[3][0] = sin(t_sc) * 45 + 120;
	position[3][1] = cos(t_sc) * 45 + 60;


	// U01 A3b)
	// Hinweise:
	//   - CGContext::drawArrays nimmt folgende Parameter entgegen:
	//     . Den zu rendernden Primitivtyp (in diesem Fall Punkte, später u.a. Dreiecke)
	//     . Den Index des ersten zu bearbeitenden Vertex
	//     . Die Anzahl der zu bearbeitenden Vertices
	//   - Die Daten zum Rendern stammen aus den aktuell festgelegten Vertexattributzeigern (s.o.)
	// select the program used
	context.useProgram(prog);

	context.drawArrays(CG_POINTS,0,4);
}
