///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2016 Robert P. Wolf                       //
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

package Prolog . geometry;

public class Point {
	public double x, y;
	public Point half () {return new Point (x * 0.5, y * 0.5);}
	public Point times (double d) {return new Point (x * d, y * d);}
	public Point times (Point d) {return new Point (x * d . x, y * d . y);}
	public Point divide (double d) {if (d != 0.0) return new Point (x / d, y / d); return new Point (x, y);}
	public Point divide (Point d) {return new Point (d . x != 0.0 ? x / d . x : x, d . y != 0.0 ? y / d . y : y);}
	public Point add (Point p) {return new Point (p . x + x, p . y + y);}
	public Point sub (Point p) {return new Point (x - p . x, y - p . y);}
	public boolean eq (Point p) {return p . x == x && p . y == y;}
	public Point (double x, double y) {this . x = x; this . y = y;}
	public Point (Point point) {x = point . x; y = point . y;}
}

