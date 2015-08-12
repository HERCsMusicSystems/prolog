///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2014 Robert P. Wolf                       //
//                                                                               //
// Permission is hereby granted, free of charge, to any person obtaining a copy  //
// of this software and associated documentation files (the "Software"), to deal //
// in the Software without restriction, including without limitation the rights  //
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell     //
// copies of the Software, and to permit persons to whom the Software is         //
// furnished to do so, subject to the following conditions:                      //
//                                                                               //
// The above copyright notice and this permission notice shall be included in    //
// all copies or substantial portions of the Software.                           //
//                                                                               //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR    //
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,      //
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE   //
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER        //
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, //
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN     //
// THE SOFTWARE.                                                                 //
///////////////////////////////////////////////////////////////////////////////////

#ifndef _GRAPHICS_2D_
#define _GRAPHICS_2D_

class point;
class rect;
class colour;

#define POINT(p) p . x, p . y

class point {
public:
	double x, y;
	point (double x, double y);
	point (double locations [2]);
	point (void);
	point operator + (const point & p) const;
	point operator - (const point & p) const;
	point operator - (void) const;
	point operator * (const double & scale) const;
	point operator * (const point & p) const;
	point operator / (const double & scale) const;
	bool operator == (const point & p) const;
	bool operator != (const point & p) const;
	point operator += (const point & p);
	point operator -= (const point & p);
	point operator *= (const double & d);
	point operator *= (const point & p);
	point half (void);
	point rotate (double angle);
	point swap (void);
	void round (void);
	void minimise (void);
	void maximise (void);
};

#define RECT(r) r . position . x, r . position . y, r . size . x, r . size . y
class rect {
public:
	point position;
	point size;
	point centre (void);
	point centre (double scaling);
	bool overlap (rect area);
	void positivise (void);
	bool operator == (const rect & r) const;
	bool operator != (const rect & r) const;
	void minimise (void);
	void maximise (void);
	rect (point offset, point size);
	rect (double x, double y, double width, double height);
	rect (double locations [4]);
	rect (void);
};

#define COLOUR(c) c . red, c . green, c . blue
#define ACOLOUR(c) c . red, c . green, c . blue, c . alpha
#define INTCOLOUR(c) colour_to_int (c . red), colour_to_int (c . green), colour_to_int (c . blue)
#define AINTCOLOUR(c) colour_to_int (c . red), colour_to_int (c . green), colour_to_int (c . blue), colour_to_int (c . alpha)

double int_to_colour (int c);
int colour_to_int (double c);
class colour {
public:
	double red, green, blue, alpha;
	bool operator == (const colour & c) const;
	bool operator != (const colour & c) const;
	colour (int red, int green, int blue, int alpha = 255);
	colour (double red, double green, double blue, double alpha = 1.0);
	colour (void);
};

#endif
