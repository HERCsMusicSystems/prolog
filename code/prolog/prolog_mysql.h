
class MySQLServiceClass : public PrologServiceClass {
public:
	PrologRoot * root;
	virtual void init (PrologRoot * root, PrologDirectory * directory);
	virtual PrologNativeCode * getNativeCode (char * name);
};


