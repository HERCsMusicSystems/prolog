///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2004 Robert P. Wolf                       //
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

#ifndef _PROLOG_HTTP_SERVICE_CLASS_
#define _PROLOG_HTTP_SERVICE_CLASS_

#include "prolog.h"

class PrologHttpServiceClass : public PrologServiceClass {
public:
	PrologRoot * root;
	PrologDirectory * http_directory;
	PrologAtom * full_text_atom, * route_atom, * protocol_atom, * header_atom, * param_atom;
	PrologAtom * get_atom, * post_atom, * put_atom, * patch_atom, * delete_atom, * copy_atom;
	PrologAtom * head_atom, * options_atom, * link_atom, * unlink_atom, * purge_atom;
	void set_atoms (void);
	virtual void init (PrologRoot * root, PrologDirectory * directory);
	virtual PrologNativeCode * getNativeCode (char * name);
};

#endif

