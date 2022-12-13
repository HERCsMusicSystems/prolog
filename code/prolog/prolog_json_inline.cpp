
char * inline_json_resource = "import studio program json #machine := 'json' [json] #machine json := 'json' end := [[command]] .";

class json : public PrologNativeCode {
public:
	bool NeedSeparator;
	FILE * tc;
	PrologAtom * ufo, * grlt, * assign;
	void DropSeparator (void) {if (NeedSeparator && tc) fprintf (tc, ", "); NeedSeparator = true;};
	void DropNull (void) {if (tc) fprintf (tc, "null");};
	void DropAtom (PrologAtom * atom) {
		if (tc) fprintf (tc, "%s", atom -> name ());
	};
	void DropInteger (int value) {if (tc) fprintf (tc, "%i", value);};
	void DropDouble (double value) {if (tc) fprintf (tc, "%g", value);};
	void DropText (char * text) {
		if (! tc) return;
		char * ch = text;
		fprintf (tc, "\"");
		while (* ch > 0) {
			switch (* ch) {
			case '\\': fprintf (tc, "\\\\"); break;
			case '"': fprintf (tc, "\\\""); break;
			default: fprintf (tc, "%c", * ch); break;
			}
			ch ++;
		}
		fprintf (tc, "\"");
	};
	void DropPair (PrologElement * json) {
		if (! tc) return;
		PrologElement * ControlAtom = json -> getLeft ();
		if (! ControlAtom -> isAtom ()) return;
		if (ControlAtom -> getAtom () == ufo || ControlAtom -> getAtom () == grlt) {
			fprintf (tc, "[\n");
			json = json -> getRight ();
			while (json -> isPair ()) {
				DropSeparator ();
				DropJson (json -> getLeft ());
				json = json -> getRight ();
			}
			fprintf (tc, "]");
		} else {
			printf ("Object.\n");
		}
	};
	void DropJson (PrologElement * json) {
		if (json -> isEarth ()) DropNull ();
		else if (json -> isAtom ()) DropAtom (json -> getAtom ());
		else if (json -> isText ()) DropText (json -> getText ());
		else if (json -> isInteger ()) DropInteger (json -> getInteger ());
		else if (json -> isDouble ()) DropDouble (json -> getDouble ());
		else if (json -> isPair ()) DropPair (json);
	};
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * json = parameters -> getLeft (); parameters = parameters -> getRight ();
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (json -> isVar ()) {
		} else {
			if (! parameters -> isText ()) return false;
			tc = fopen (parameters -> getText (), "wb");
			if (tc) {
				DropJson (json);
				fclose (tc);
			}
		}
		return true;
	};
	json (PrologRoot * root) {
		NeedSeparator = false; tc = 0;
		ufo = root -> search ("<=>");
		grlt = root -> search ("<>");
		assign = root -> search (":=");
	};
};

class PrologJSONInlineServiceClass : public PrologServiceClass {
public:
	PrologRoot * root;
	void init (PrologRoot * root, PrologDirectory * directory) {this -> root = root;};
	PrologNativeCode * getNativeCode (char * name) {if (strcmp (name, "json") == 0) return new json (root); return 0;};
};

