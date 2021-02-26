#pragma once


struct CGVec4;
struct CGVec2i;
struct CGFrameBuffer
{
	bool allocate(int width, int height);
	int getWidth() const;
	int getHeight() const;

	class CGColorBuffer
	{
	public:
		CGColorBuffer();
		~CGColorBuffer();
		bool reserve(const int width,const int height);
		void set_white(const CGVec2i &p);
		void set(const CGVec2i& p, const CGVec4& rgba);
		CGVec4 get(const CGVec2i& p) const;
		void clear(const CGVec4 &rgba);
		unsigned char* getDataPointer() const;
		int getWidth() const;
		int getHeight() const;
	private:
		int m_width, m_height;
		unsigned char* m_buffer_data;
	} colorBuffer;

	class CGDepthBuffer
	{
	public:
		CGDepthBuffer();
		~CGDepthBuffer();
		bool reserve(const int width,const int height);
		void set(const CGVec2i& p, const float z);
		float get(const CGVec2i& p) const;
		void clear();
		float* getDataPointer() const;
		int getWidth() const;
		int getHeight() const;
	private:
		int m_width, m_height;
		float* m_buffer_data;
	} depthBuffer;
};
