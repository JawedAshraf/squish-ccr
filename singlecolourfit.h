/* -----------------------------------------------------------------------------

	Copyright (c) 2006 Simon Brown                          si@sjbrown.co.uk
	Copyright (c) 2012 Niels Fr�hling              niels@paradice-insight.us

	Permission is hereby granted, free of charge, to any person obtaining
	a copy of this software and associated documentation files (the
	"Software"), to	deal in the Software without restriction, including
	without limitation the rights to use, copy, modify, merge, publish,
	distribute, sublicense, and/or sell copies of the Software, and to
	permit persons to whom the Software is furnished to do so, subject to
	the following conditions:

	The above copyright notice and this permission notice shall be included
	in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
	OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
	CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
	TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

   -------------------------------------------------------------------------- */

#ifndef SQUISH_SINGLECOLOURFIT_H
#define SQUISH_SINGLECOLOURFIT_H

#include <squish.h>
#include "colourfit.h"

// pull in structure definitions
#if	defined(USE_AMP)
#include "singlecolourlookup_ccr.inl"
#endif

namespace squish {

// -----------------------------------------------------------------------------
#if	!defined(USE_PRE)
class ColourSet;
struct SingleColourLookup;
class SingleColourFit : public ColourFit
{
public:
  SingleColourFit( ColourSet const* colours, int flags );

private:
  virtual void Compress3( void* block );
  virtual void Compress4( void* block );

  void ComputeEndPoints( SingleColourLookup const* const* lookups );

  u8 m_colour[3];
  Vec3 m_start;
  Vec3 m_end;
  u8 m_index;
  int m_error;
  int m_besterror;
};
#endif

// -----------------------------------------------------------------------------
#if	defined(USE_AMP) || defined(USE_COMPUTE)
struct SingleColourFit_CCR : inherit_hlsl ColourFit_CCR
{
public_hlsl
  void AssignSet (tile_barrier barrier, const int thread,
                  ColourSet_CCRr m_colours, const int metric, const int fit) amp_restricted;
  void Compress  (tile_barrier barrier, const int thread,
                  ColourSet_CCRr m_colours, out code64 block, const bool trans,
		  IndexBlockLUT yArr, SingleColourLUT lArr) amp_restricted;

protected_hlsl
  void Compress3 (tile_barrier barrier, const int thread,
                  ColourSet_CCRr m_colours, out code64 block,
		  IndexBlockLUT yArr, SingleColourLUT lArr) amp_restricted;
  void Compress4 (tile_barrier barrier, const int thread,
                  ColourSet_CCRr m_colours, out code64 block,
		  IndexBlockLUT yArr, SingleColourLUT lArr) amp_restricted;
  void Compress34(tile_barrier barrier, const int thread,
                  ColourSet_CCRr m_colours, out code64 block,
		  IndexBlockLUT yArr, SingleColourLUT lArr) amp_restricted;

  void ComputeEndPoints(tile_barrier barrier, const int thread, const int is4,
		        SingleColourLUT lArr) amp_restricted;
  int  ComputeEndPoints(tile_barrier barrier, const int thread,
		        SingleColourLUT lArr) amp_restricted;

#if	!defined(USE_COMPUTE)
private_hlsl
  int3 m_colour;
  ccr8 m_index;
#endif
};

#if	defined(USE_COMPUTE)
  tile_static int3 m_colour;
  tile_static ccr8 m_index;
#endif
#endif

} // namespace squish

#endif // ndef SQUISH_SINGLECOLOURFIT_H
