
class MySQLServiceClass : public PrologServiceClass {
public:
	PrologRoot * root;
	virtual void init (PrologRoot * root);
	virtual PrologNativeCode * getNativeCode (char * name);
};


