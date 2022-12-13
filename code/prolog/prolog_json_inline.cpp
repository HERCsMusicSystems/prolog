
char * inline_json_resource = "import studio program json #machine := 'json' [json] #machine json := 'json' end := [[command]] .";

class json : public PrologNativeCode {
public:
	bool NeedSeparator;
	FILE * tc;
	void DropSeparator (void) {if (NeedSeparator && tc) fprintf (tc, ", ");};
	void DropNull (void) {if (tc) fprintf (tc, "null");};
	void DropAtom (PrologAtom * atom) {
		if (tc) fprintf (tc, "%s", atom -> name ());
	};
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
	void DropJson (PrologElement * json) {
		if (json -> isEarth ()) DropNull ();
		else if (json -> isAtom ()) DropAtom (json -> getAtom ());
		else if (json -> isText ()) DropText (json -> getText ());
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
	json (void) {NeedSeparator = false; tc = 0;};
};

class PrologJSONInlineServiceClass : public PrologServiceClass {
public:
	PrologNativeCode * getNativeCode (char * name) {if (strcmp (name, "json") == 0) return new json (); return 0;};
};

