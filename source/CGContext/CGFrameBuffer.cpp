#include "CGFrameBuffer.h"
#include <stdlib.h> // for malloc
#include <string.h> // for memcpy
#include "CG.h"
#include "CGVector.h"

#define CHANNEL_WIDTH 4

//------------------------------------------------------------------------------
// CGFRAMEBUFFER
//------------------------------------------------------------------------------
bool CGFrameBuffer::allocate(int width, int height)
{
	bool retval = colorBuffer.reserve(width,height);
	// also reserve a depth buffer
	retval = retval && depthBuffer.reserve(width, height);
	return retval;
}
//------------------------------------------------------------------------------
int CGFrameBuffer::getWidth() const
{
	return colorBuffer.getWidth();
}
//------------------------------------------------------------------------------
int CGFrameBuffer::getHeight() const
{
	return colorBuffer.getHeight();
}


//------------------------------------------------------------------------------
// CGFRAMEBUFFER::CGBUFFER4UB
//------------------------------------------------------------------------------
CGFrameBuffer::CGColorBuffer::CGColorBuffer()
{
	m_buffer_data = nullptr;
}
//------------------------------------------------------------------------------
CGFrameBuffer::CGColorBuffer::~CGColorBuffer()
{
	free(m_buffer_data);
	m_buffer_data = nullptr;
}
//------------------------------------------------------------------------------
bool CGFrameBuffer::CGColorBuffer::reserve(const int width, const int height)
{
	m_width = width;
	m_height = height;

	// allocate the memory for the buffer

	// U01 A1a)
	// Hinweise:
	//   - m_buffer_data ist eine Membervariable; auf sie kann auch in anderen Methoden der Klasse
	//     CGColorBuffer zugegriffen werden.
	//   - malloc nimmt einen Parameter entgegen: die Anzahl der zu reservierenden Bytes
	//   - malloc gibt einen Zeiger auf den allozierten Speicher zurück (als void*, m_buffer_data
	//     ist aber vom Typ unsigned char*)
	// m_buffer_data = ...

	//return m_buffer_data != nullptr;

	//CHANNEL_WIDTH == 4 bc 4 "colors" are saved(rgba)
	size_t si = m_width * m_height * CHANNEL_WIDTH * sizeof(unsigned char);
	unsigned char* p = (unsigned char*)malloc(si);

	//check if new block was granted (!= NULL)
	if (p) {
		memset(p, 0, si); //set new block of memory to 0
		m_buffer_data	= p;
		return true;
	}
	else {
		m_buffer_data = NULL;
		return false;
	}
}
//------------------------------------------------------------------------------
void CGFrameBuffer::CGColorBuffer::set_white(const CGVec2i& p)
{
	//assert(m_buffer_data != nullptr);
	//assert(p.x < m_width && p.y < m_height && p.x >= 0 && p.y >= 0);

	// U01 A1b)
	// Hinweise:
	//   - Gebrauchen Sie Zeigerarithmetik, um die Adresse des Pixels bei (p.x, p.y) zu berechnen.
	//   - m_buffer_data zeigt auf den Anfang des Speicherbereichs, der in CGColorBuffer::reserve
	//     reserviert wurde.
	// ...
	size_t buf_pos = 4 * (m_width * p.y + p.x);
	if (m_buffer_data) {
		m_buffer_data[buf_pos++] = 255;
		m_buffer_data[buf_pos++] = 255;
		m_buffer_data[buf_pos++] = 255;
		m_buffer_data[buf_pos] = 255;
	}
}
//------------------------------------------------------------------------------
void CGFrameBuffer::CGColorBuffer::set(const CGVec2i &p, const CGVec4 &rgba)
{
	//assert(m_buffer_data != nullptr);
	//assert(p.x < m_width && p.y < m_height && p.x >= 0 && p.y >= 0);

	// U01 A1c)
	// Hinweise:
	//   - Die Werte der einzelnen Kanäle des Parameters rgba sind vom Typ float und liegen im
	//     Intervall [0.0, 1.0]
	//   - Welcher Wertebereich ist für eine vorzeichenfreie 8-Bit-Ganzzahl (unsigned char) zulässig?
	//     Auf diesen Bereich muss das Intervall [0.0, 1.0] der Eingabewerte abgebildet werden.
	// ...

	size_t buf_pos = 4 * (m_width * p.y + p.x);
	m_buffer_data[buf_pos++] = (unsigned char)(rgba.x * 255);
	m_buffer_data[buf_pos++] = (unsigned char)(rgba.y * 255);
	m_buffer_data[buf_pos++] = (unsigned char)(rgba.z * 255);
	m_buffer_data[buf_pos] = (unsigned char)(rgba.w * 255);
}
//------------------------------------------------------------------------------
CGVec4 CGFrameBuffer::CGColorBuffer::get(const CGVec2i &p) const
{
	//assert(m_buffer_data != nullptr);
	//assert(p.x < m_width && p.y < m_height && p.x >= 0 && p.y >= 0);

	CGVec4 rgba;

	// U01 A1d)
	// Hinweise:
	//   - Für diese Operation muss die in CGColorBuffer::set vorgenommene Umrechnung invertiert
	//     werden.
	// ...
	size_t buf_pos = 4 * (m_width * p.y + p.x);
	rgba.x = m_buffer_data[buf_pos++] / 255.0f;
	rgba.y = m_buffer_data[buf_pos++] / 255.0f;
	rgba.z = m_buffer_data[buf_pos++] / 255.0f;
	rgba.w = m_buffer_data[buf_pos] / 255.0f;

	return rgba;
}
//------------------------------------------------------------------------------
void CGFrameBuffer::CGColorBuffer::clear(const CGVec4& rgba)
{
	//assert(m_buffer_data != nullptr);

	// U01 A1e)
	// Hinweise:
	//   - Rechnen Sie zunächst die Eingabefarbe analog zu CGColorBuffer::set um und kopieren Sie
	//     die resultierenden Werte dann pro Pixel einmal in den Color Buffer.
	// ...

	for (int i = 0; i < m_width * m_height * 4; ) {
		m_buffer_data[i++] = (unsigned char)(rgba.x * 255);
		m_buffer_data[i++] = (unsigned char)(rgba.y * 255);
		m_buffer_data[i++] = (unsigned char)(rgba.z * 255);
		m_buffer_data[i++] = (unsigned char)(rgba.w * 255);
	}
}
//------------------------------------------------------------------------------
unsigned char* CGFrameBuffer::CGColorBuffer::getDataPointer() const
{
	return m_buffer_data;
}
//------------------------------------------------------------------------------
int CGFrameBuffer::CGColorBuffer::getWidth() const
{
	return m_width;
}
//------------------------------------------------------------------------------
int CGFrameBuffer::CGColorBuffer::getHeight() const
{
	return m_height;
}


//------------------------------------------------------------------------------
// CGFRAMEBUFFER::CGBUFFER1F
//------------------------------------------------------------------------------
CGFrameBuffer::CGDepthBuffer::CGDepthBuffer()
{
	m_buffer_data = nullptr;
}
//------------------------------------------------------------------------------
CGFrameBuffer::CGDepthBuffer::~CGDepthBuffer()
{
	free(m_buffer_data);
	m_buffer_data = nullptr;
}
//------------------------------------------------------------------------------
bool CGFrameBuffer::CGDepthBuffer::reserve(const int width,const int height)
{
	m_width = width;
	m_height = height;
	// U09 A1 a)
	// ...

	size_t siz = m_width * m_height;

	float* p = (float*)malloc(siz * sizeof(float));
	//check if new block was granted (!= NULL)
	if (p) {
		for (size_t i = 0; i < siz; i++) p[i] = 1.0f; //set new block of memory to 1.0f
		m_buffer_data = p;
		return true;
	}
	else {
		m_buffer_data = NULL;
		return false;
	}
	return true;
	
}
//------------------------------------------------------------------------------
float CGFrameBuffer::CGDepthBuffer::get(const CGVec2i &p) const
{
	//assert(m_buffer_data != nullptr);
	//assert(p.x < m_width && p.y < m_height && p.x >= 0 && p.y >= 0);
	// U09 A1 a)
	// ...
	return m_buffer_data[p.y * m_width + p.x];
	//return 0.0f;
}
//------------------------------------------------------------------------------
void CGFrameBuffer::CGDepthBuffer::set(const CGVec2i &p, const float z)
{
	//assert(m_buffer_data != nullptr);
	//assert(p.x < m_width && p.y < m_height && p.x >= 0 && p.y >= 0);
	// U09 A1 a)
	// ...
	m_buffer_data[p.y * m_width + p.x] = z;

}
//------------------------------------------------------------------------------
void CGFrameBuffer::CGDepthBuffer::clear()
{
	//assert(m_buffer_data != nullptr);
	// U09 A1 a)
	// ...
	size_t siz = m_width * m_height;
	for (size_t i = 0; i < siz; i++) m_buffer_data[i] = 1.0f;
}
//------------------------------------------------------------------------------
float* CGFrameBuffer::CGDepthBuffer::getDataPointer() const
{
	return m_buffer_data;
}
//------------------------------------------------------------------------------
int CGFrameBuffer::CGDepthBuffer::getWidth() const
{
	return m_width;
}
//------------------------------------------------------------------------------
int CGFrameBuffer::CGDepthBuffer::getHeight() const
{
	return m_height;
}
//------------------------------------------------------------------------------
