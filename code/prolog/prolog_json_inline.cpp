
char * inline_json_resource = "import studio program json #machine := 'json' [json] #machine json := 'json' end := [[command]] .";

class JSONReader {
public:
	FILE * fr;
	int act;
	AREA symbol;
	double double_symbol;
	int SkipWhitespaces (void) {
		if (! fr) {act = -1; return -1;}
		int ch = fgetc (fr);
		while (ch <= ' ' && ch >= 0) ch = fgetc (fr);
		return ch;
	};
	int SkipWhitespacesAndSeparators (void) {
		int ch = SkipWhitespaces ();
		while (ch == ',') ch = SkipWhitespaces ();
		return ch;
	};
	int GetSymbol (void) {
		if (! fr) return -1;
		int ch = SkipWhitespacesAndSeparators ();
		symbol [0] = '\0';
		if (strchr ("[]{}:", ch) != 0) {symbol [0] = (char) ch; symbol [1] = '\0'; return 1;}
		if (strchr ("-0123456789", ch) != 0) {
			int ac = 0;
			while (strchr ("-+0123456789.eE", ch) != 0) {ac = area_cat (symbol, ac, (char) ch); ch = fgetc (fr);} area_cat (symbol, ac, '\0');
			return 2;
		}
		if (ch == '"') {
			ch = fgetc (fr);
			int ac = 0;
			while (ch >= 0 && ch != '"') {ac = area_cat (symbol, ac, (char) ch); ch = fgetc (fr);} area_cat (symbol, ac, '\0');
			if (ch == '"') fgetc (fr);
			return 3;
		}
		if (ch != '"' && ch > ' ') {
			int ac = 0; while (ch != '"' && ch > ' ') {ac = area_cat (symbol, ac, (char) ch); ch = fgetc (fr);} area_cat (symbol, ac, '\0');
			return 4;
		}
		return 0;
	};
	JSONReader (char * file_name) {
		act = -1; symbol [0] = '\0'; double_symbol = 0.0;
		fr = fopen (file_name, "rb");
		if (! fr) return;
		// int ch; while ((ch = SkipWhitespacesAndSeparators ()) > 0) printf ("%c", ch);
		int ch; while ((ch = GetSymbol ()) > 0) printf ("%i => %s\n", ch, symbol);
	};
	~ JSONReader (void) {if (fr) fclose (fr); fr = 0; printf ("File closed.\n");};
};

class json : public PrologNativeCode {
public:
	bool NeedSeparator;
	int SeparatorLevel;
	FILE * tc;
	PrologAtom * ufo, * grlt, * assign;
	PrologAtom * TrueAtom, * FalseAtom, * NullAtom;
	void DropSeparator (void) {
		if (NeedSeparator && tc) {fprintf (tc, ",\n"); int sl = SeparatorLevel; while (sl -- > 0 && tc) fprintf (tc, "	");}
		NeedSeparator = true;
	};
	void DropNull (void) {if (tc) fprintf (tc, "null");};
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
	void DropAtom (PrologAtom * atom) {
		if (tc) {
			if (SeparatorLevel < 1 || atom == TrueAtom || atom == FalseAtom || atom == NullAtom) fprintf (tc, "%s", atom -> name ());
			else DropText (atom -> name ());
		}
	};
	void DropInteger (int value) {if (tc) fprintf (tc, "%i", value);};
	void DropDouble (double value) {if (tc) fprintf (tc, "%g", value);};
	void DropKeyValuePair (PrologElement * json) {
		if (! json -> isPair ()) return;
		PrologElement * key = json -> getLeft (), * value = json -> getRight ();
		DropJson (key); fprintf (tc, ": "); DropJson (value);
	};
	void DropPair (PrologElement * json) {
		if (! tc) return;
		PrologElement * ControlAtom = json -> getLeft ();
		if (! ControlAtom -> isAtom ()) return;
		if (ControlAtom -> getAtom () == ufo || ControlAtom -> getAtom () == grlt) {
			fprintf (tc, "[\n");
			NeedSeparator = false;
			json = json -> getRight ();
			SeparatorLevel ++;
			while (json -> isPair ()) {
				if (NeedSeparator) fprintf (tc, ",\n"); NeedSeparator = true;
				int sl = SeparatorLevel; while (sl -- > 0) fprintf (tc, "	");
				DropJson (json -> getLeft ());
				json = json -> getRight ();
			}
			SeparatorLevel --;
			if (NeedSeparator) fprintf (tc, "\n");
			int sl = SeparatorLevel; while (tc && sl -- > 0) fprintf (tc, "	");
			fprintf (tc, "]"); NeedSeparator = true;
		} else {
			fprintf (tc, "{\n");
			NeedSeparator = false;
			json = json -> getRight ();
			SeparatorLevel ++;
			while (json -> isPair ()) {
				if (NeedSeparator) fprintf (tc, ",\n"); NeedSeparator = true;
				int sl = SeparatorLevel; while (sl -- > 0) fprintf (tc, "	");
				DropKeyValuePair (json -> getLeft ());
				json = json -> getRight ();
			}
			SeparatorLevel --;
			if (NeedSeparator) fprintf (tc, "\n");
			int sl = SeparatorLevel; while (tc && sl -- > 0) fprintf (tc, "	");
			fprintf (tc, "}"); NeedSeparator = true;
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
			if (! parameters -> isText ()) return false;
//			fr = fopen (parameters -> getText (), "rb");
//			if (! fr) return false;
//			char ch;
//			while ((ch = fgetc (fr)) > 0) printf ("%c", ch);
//			fclose (fr);
			JSONReader (parameters -> getText ());
			return true;
		} else {
			if (! parameters -> isText ()) return false;
			tc = fopen (parameters -> getText (), "wb");
			if (tc) {
				DropJson (json);
				fprintf (tc, "\n");
				fclose (tc);
			}
		}
		return true;
	};
	json (PrologRoot * root) {
		NeedSeparator = false; SeparatorLevel = 0; tc = 0;
		ufo = root -> search ("<=>");
		grlt = root -> search ("<>");
		assign = root -> search (":=");
		TrueAtom = root -> search ("true");
		FalseAtom = root -> search ("false");
		NullAtom = root -> search ("null");
	};
};

class PrologJSONInlineServiceClass : public PrologServiceClass {
public:
	PrologRoot * root;
	void init (PrologRoot * root, PrologDirectory * directory) {this -> root = root;};
	PrologNativeCode * getNativeCode (char * name) {if (strcmp (name, "json") == 0) return new json (root); return 0;};
};

