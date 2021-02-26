#pragma once
#define CG_MAX_FRAGMENTS_IN_PIPELINE 4096
#include "CGProgramInterface.h"

class CGContext;
struct CGFrameBuffer;


class CGFragmentOperations
{
	CGContext& m_context;
	CGFrameBuffer& m_frame_buffer;

	CGFragmentData m_fragment_buffer[CG_MAX_FRAGMENTS_IN_PIPELINE];
	unsigned m_num_fragments;

	bool fragment_clipping(const CGFragmentData& frag);
	bool z_test(const CGFragmentData& frag);
	void fragment_blending(CGFragmentData& frag);
	void write_to_framebuffer(const CGFragmentData& frag);
	void process_fragment(CGFragmentData &frag);

public:
	CGFragmentOperations(CGContext& context,
	                     CGFrameBuffer& framebuffer);

	/// Adds a fragment to the fragments that should be processed.
	void push_fragment(const CGFragmentData& frag);

	/// Forces the processing of all fragments in the queue.
	void flush_fragments();

};
