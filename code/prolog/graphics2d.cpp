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

#include "graphics2d.h"
#include <math.h>

point :: point (void) {x = y = 0.0;}
point :: point (double x, double y) {this -> x = x; this -> y = y;}
point :: point (double locations [2]) {this -> x = locations [0]; this -> y = locations [1];}

point point :: operator + (const point & p) const {return point (x + p . x, y + p . y);}
point point :: operator - (const point & p) const {return point (x - p . x, y - p . y);}
point point :: operator - (void) const {return point (- x, - y);}
point point :: operator * (const double & scale) const {return point (x * scale, y * scale);}
point point :: operator * (const point & p) const {return point (x * p . x, y * p . y);}
point point :: operator / (const double & scale) const {if (scale == 0.0) return * this; return point (x / scale, y / scale);}
bool point :: operator == (const point & p) const {return x == p . x && y == p . y;}
bool point :: operator != (const point & p) const {return x != p . x || y != p . y;}
point point :: operator += (const point & p) {x += p . x; y += p . y; return * this;}
point point :: operator -= (const point & p) {x -= p . x; y -= p . y; return * this;}
point point :: operator *= (const double & d) {x *= d; y *= d; return * this;}
point point :: operator *= (const point & p) {x *= p . x; y *= p . y; return * this;}
point point :: half (void) {return * this * 0.5;}
point point :: rotate (double angle) {double sn = sin (angle); double cn = cos (angle); return point (x * cn - y * sn, y * cn + x * sn);}
point point :: swap (void) {return point (y, x);}
void point :: round (void) {x = (double) ((int) (x + 0.5)); y = (double) ((int) (y + 0.5));}
void point :: minimise (void) {if (x > y) x = y; else if (y > x) y = x;}
void point :: maximise (void) {if (x < y) x = y; else if (y < x) y = x;}

rect :: rect (void) {position = size = point (0.0, 0.0);}
rect :: rect (point position, point size) {this -> position = position; this -> size = size;}
rect :: rect (double locations [4]) {position = point (locations [0], locations [1]); size = point (locations [2], locations [3]);}
rect :: rect (double x, double y, double width, double height) {position = point (x, y); size = point (width, height);}

point rect :: centre (void) {return point (position . x + size . x * 0.5, position . y + size . y * 0.5);}
point rect :: centre (double scaling) {scaling *= 0.5; return point (position . x + size . x * scaling, position . y + size . y * scaling);}

bool rect :: overlap (rect area) {
	return position . x <= area . position . x + area . size . x && position . x + size . x >= area . position . x && position . y <= area . position . y + area . size . y && position . y + size . y >= area . position . y;
}
void rect :: positivise (void) {
	if (size . x < 0.0) {size . x = - size . x; position . x -= size . x;}
	if (size . y < 0.0) {size . y = - size . y; position . y -= size . y;}
}

bool rect :: operator == (const rect & r) const {return position == r . position && size == r . size;}
bool rect :: operator != (const rect & r) const {return position != r . position || size != r . size;}
void rect :: minimise (void) {size . minimise ();}
void rect :: maximise (void) {size . maximise ();}

double int_to_colour (int c) {return c >= 255 ? 1.0 : (double) c / 256.0;}
int colour_to_int (double c) {return c >= 1.0 ? 255 : (int) (256.0 * c);}
colour :: colour (int red, int green, int blue, int alpha) {this -> red = int_to_colour (red); this -> green = int_to_colour (green); this -> blue = int_to_colour (blue); this -> alpha = int_to_colour (alpha);}
colour :: colour (double red, double green, double blue, double alpha) {this -> red = red; this -> green = green; this -> blue = blue; this -> alpha = alpha;}
colour :: colour (void) {red = green = blue = 0.0; alpha = 1.0;}

bool colour :: operator == (const colour & c) const {return red == c . red && green == c . green && blue == c . blue && alpha == c . alpha;}
bool colour :: operator != (const colour & c) const {return red != c . red || green != c . green || blue != c . blue || alpha != c . alpha;}
