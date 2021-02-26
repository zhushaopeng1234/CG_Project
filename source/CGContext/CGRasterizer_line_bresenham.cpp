#include "CGRasterizer.h"
#include "CGProgramInterface.h"
#include "CGFragmentOperations.h"
#include <algorithm>

void CGBresenhamLineRasterizer::rasterize(const CGVaryings& A, const CGVaryings& B)
{
	CGVec2i start, end;
	start.set((int)A.position.x, (int)A.position.y);
	end.set((int)B.position.x, (int)B.position.y);

	bool swap_xy = abs(end.x - start.x) < abs(end.y - start.y);

	if (swap_xy)
	{
		std::swap(start.x, start.y);
		std::swap(end.x, end.y);
	}

	CGFragmentData fragment;
	fragment.set(A);

	int x_step = start.x <= end.x ? 1 : -1;
	int y_step = start.y <= end.y ? 1 : -1;
	int x = start.x;
	int y = start.y;
	int dx = abs(end.x - start.x);
	int dy = abs(end.y - start.y);

	int  d = 2 * dy - dx;

	const int deltaE = 2 * dy;
	const int deltaNE = 2 * (dy - dx);

	// TODO: Übung 04, Aufgabe 1b: Berechnen Sie den Interpolationsfaktor `ratio` für jedes Fragment!
	//float ratio = 0;

	// Hinweis: `ratio` entspricht dem Verhältnis der Strecke, zwischen den Punkten A und B für jedes Fragment. Ein Fragment, welches 
	//          auf Position A liegt, entspricht somit einem `ratio` von 0.0; ein Fragment auf Position B von entspricht einem Wert 1.0.
	//          Da wir linear Interpolieren, erhöht sich `ratio` für jeden Schritt (d.h. mit jeder Schleifeniteration) um ein konstantes 
	//          Inkrement. Überlegen Sie sich, wie sie dieses Inkrement berechnen können und erhöhen Sie `ratio` in der Schleife 
	//          entsprechend!

	while (x != end.x + x_step)
	{
		float ratio = (float)(x - start.x) / (float)(end.x - start.x);

		if (swap_xy)
			fragment.coordinates.set(y, x);
		else
			fragment.coordinates.set(x, y);

		fragment.set(A, B, ratio);	
		// TODO: `ratio` um konstantes Inkrement erhöhen.
		
		m_frag_ops.push_fragment(fragment);

		x += x_step;

		if (d <= 0)		// Schritt nach Osten
			d += deltaE;
		else			// Schritt nach Nord-Osten
		{
			y += y_step;
			d += deltaNE;
		}
	}

	m_frag_ops.flush_fragments();
}