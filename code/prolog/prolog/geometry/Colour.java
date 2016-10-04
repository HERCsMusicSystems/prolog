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

public class Colour {
	public double red, green, blue, alpha;
	public static int toInt (double c) {return c >= 1.0 ? 255 : (int) (c * 256.0);}
	public boolean eq (Colour c) {return c . red == red && c . green == green && c . blue == blue && c . alpha == alpha;}
	public Colour (Colour c) {this . red = c . red; this . green = c . green; this . blue = c . blue; this . alpha = c . alpha;}
	public Colour (double red, double green, double blue) {this . red = red; this . green = green; this . blue = blue; this . alpha = 1.0;}
	public Colour (double red, double green, double blue, double alpha) {this . red = red; this . green = green; this . blue = blue; this . alpha = alpha;}
}

