#include "U_DottedLine.h"
#include "Utilities/CG1Helper/CG1Helper.h"

App_DottedLine::App_DottedLine(int w, int h) :
    Application(w, h)
{
	// Start- und Endpunkt der zu rendernden Strecke als 3D-Vektoren
	const float startPoint[] = { 20.0f, 150.0f, 0.0f };
	const float endPoint[] = { 230.0f, 10.0f, 0.0f };

	// RGBA-Farbwerte der Strecke am Start- und Endpunkt
	const float startColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	const float endColor[] = { 1.0f, 0.0f, 1.0f, 1.0f };

	// Anzahl der Samples entlang der Strecke - so viele Punkte sollen entstehen
	// U01.1 A2d)
	numSamples = 5;

	// U01.1 A2a)
	// Hinweise:
	//   - malloc nimmt einen Parameter entgegen: die Anzahl der zu reservierenden Bytes
	//   - malloc gibt einen Zeiger auf den allozierten Speicher zurück (als void*, position
	//     und color sind aber vom Typ float*)
	//   - sizeof(float) liefert die Anzahl Bytes in einem einzelnen float zurück
	
	//position = ...
	//color = ...

	// U01.1 A2b) & A3)
	// Hinweise:
	//   - Die Variablen position und color können wie Arrays benutzt werden. So verweist z.B.
	//     position[0] auf den ersten float-Wert im Array - die X-Koordinate des ersten
	//     Positionswerts; position[1] ist die Y-Koordinate der ersten Position, position[3]
	//     die X-Koordinate der zweiten Position usw.
	//   - Analog gilt für color: color[0] ist der Rot-Wert der ersten Farbe, color[2] der
	//     Blau-Wert der ersten Farbe, color[4] der Grün-Wert der zweiten Farbe usw.
	//   - Nutzen Sie eine Schleife, die für jedes Sample einen Positions- und Farbwert
	//     generiert. 
	//   - Die Schleifenvariable kann ebenfalls in die Berechnungen einbezogen werden.
	
	//...

	context.clearColor.set(0.0f, 0.0f, 0.0f, 1.0f);
	context.useProgram(prog);
	context.pointRadius = 0;
}

App_DottedLine::~App_DottedLine()
{
	if (position)
		free(position);
	if (color)
		free(color);
}

void App_DottedLine::program_step()
{
	// Vor dem Renderdurchlauf wird der Color Buffer mit der aktuellen Hintergrundfarbe überschrieben.
	context.clear(CG_COLOR_BUFFER_BIT);

	// Renderkommandos (z.B. CGContext::drawArrays) beziehen ihre Daten aus den Vertexattributzeigern.
	context.vertexAttributePointer.position = position;
	context.vertexAttributePointer.color = color;

	// U01.1 A2c)
	// Hinweise:
	//   - CGContext::drawArrays nimmt folgende Parameter entgegen:
	//     . Den zu rendernden Primitivtyp (in diesem Fall Punkte, später u.a. Dreiecke)
	//     . Den Index des ersten zu bearbeitenden Vertex
	//     . Die Anzahl der zu bearbeitenden Vertices
	//   - Die Daten zum Rendern stammen aus den aktuell festgelegten Vertexattributzeigern (s.o.)
	context.drawArrays(CG_POINTS, 0, 0);
}
