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

package prolog . geometry;

public class Rect {
	public Point position, size;
	public boolean eq (Rect r) {return r . position . eq (position) && r . size . eq (size);}
	public boolean contains (Point p) {return p . x >= position . x && p . y >= position . y && p . x <= position . x + size . x && p . y <= position . y + size . y;}
	public boolean overlaps (Rect area) {
		return
			position . x <= area . position . x + area . size . x &&
			position . x + size . x >= area . position . x &&
			position . y <= area . position . y + area . size . y &&
			position . y + size . y >= area . position . y;
	}
	public Rect (Point position, Point size) {this . position = new Point (position); this . size = new Point (size);}
	public Rect (double x, double y, double width, double height) {position = new Point (x, y); size = new Point (width, height);}
	public Rect (Rect location) {position = new Point (location . position); size = new Point (location . size);}
}

