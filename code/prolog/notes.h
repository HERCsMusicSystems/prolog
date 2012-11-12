///////////////////////////////////////
// Copyright (C) 2010 Robert P. Wolf //
//        ALL RIGHTS RESERVED        //
///////////////////////////////////////

#include "hercs_prolog_sdk.h"

class NotesServiceClass : public PrologServiceClass {
public:
	PrologRoot * root;
	virtual void init (PrologRoot * root);
	virtual PrologNativeCode * getNativeCode (char * name);
};

