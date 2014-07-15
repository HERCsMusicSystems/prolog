///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2014 Robert P. Wolf                       //
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

#include "prolog_distribution.h"
#include "string.h"
#include <random>

class generic_distributal_code : public PrologNativeCode {
public:
	PrologAtom * atom;
	std :: default_random_engine engine;
	bool seeder (unsigned long int seed) {if (seed != 0) engine . seed (seed); return true;}
	bool seeder (void) {return seeder ((unsigned long int) time (0));}
	bool seeder (double seed) {return seeder ((unsigned long int) seed);}
	bool seeder (int seed) {return seeder ((unsigned long int) seed);}
	virtual void random_code (PrologElement * var) = 0;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {atom -> setMachine (0); delete this; return true;}
		PrologElement * var = 0;
		PrologElement * seed = 0;
		PrologElement * double_seed = 0;
		bool auto_seed = false;
		while (parameters -> isPair ()) {
			PrologElement * left = parameters -> getLeft ();
			if (left -> isVar ()) var = left;
			if (left -> isInteger ()) seed = left;
			if (left -> isEarth ()) auto_seed = true;
			if (left -> isDouble ()) double_seed = left;
			parameters = parameters -> getRight ();
		}
		if (auto_seed) return seeder ();
		if (seed != 0) return seeder (seed -> getInteger ());
		if (double_seed != 0) return seeder (double_seed -> getDouble ());
		if (var == 0 && parameters -> isVar ()) var = parameters;
		if (var != 0) random_code (var);
		return true;
	}
	generic_distributal_code (PrologAtom * atom) {this -> atom = atom;}
};

class generic_distributor : public PrologNativeCode {
public:
	virtual generic_distributal_code * coder (PrologAtom * atom, PrologElement * * integers, PrologElement * * doubles) = 0;
	virtual void seed_code (generic_distributal_code * code, int ip, PrologElement * * integers, int dp) {
		if (ip == 1) code -> seeder (integers [0] -> getInteger ());
	}
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologElement * atom = 0;
		PrologElement * integers [] = {0, 0, 0, 0}; int integers_pointer = 0;
		PrologElement * doubles [] = {0, 0, 0, 0}; int doubles_pointer = 0;
		for (int ind = 0; ind < 4; ind++) {integers [ind] = doubles [ind] = 0;}
		bool earth_present = false;
		while (parameters -> isPair ()) {
			PrologElement * left = parameters -> getLeft ();
			if (left -> isAtom ()) atom = left;
			if (left -> isVar ()) atom = left;
			if (left -> isInteger () && integers_pointer < 4) integers [integers_pointer++] = left;
			if (left -> isDouble () && doubles_pointer < 4) doubles [doubles_pointer++] = left;
			if (left -> isEarth ()) earth_present = true;
			parameters = parameters -> getRight ();
		}
		if (atom != 0) {
			if (atom -> isVar ()) atom -> setAtom (new PrologAtom ());
			if (! atom -> isAtom ()) return false;
			PrologAtom * machine = atom -> getAtom ();
			if (machine -> getMachine () != 0) return false;
			generic_distributal_code * code = coder (machine, integers, doubles);
			if (code == 0) return false;
			if (earth_present) code -> seeder ();
			else seed_code (code, integers_pointer, integers, doubles_pointer);
			if (machine -> setMachine (code)) return true;
			delete code;
			return false;
		}
		return false;
	}
};

class distribution_uniform : public generic_distributor {
public:
	class integer_distribution_code : public generic_distributal_code {
	public:
		std :: uniform_int_distribution <> uniform;
		virtual void random_code (PrologElement * var) {var -> setInteger (uniform (engine));}
		integer_distribution_code (PrologAtom * atom, int from, int to)
			: generic_distributal_code (atom), uniform (from, to) {}
	};
	class real_distribution_code : public generic_distributal_code {
	public:
		std :: uniform_real_distribution <double> uniform;
		virtual void random_code (PrologElement * var) {var -> setDouble (uniform (engine));}
		real_distribution_code (PrologAtom * atom, double from, double to)
			: generic_distributal_code (atom), uniform (from, to) {}
		real_distribution_code (PrologAtom * atom) : generic_distributal_code (atom) {}
	};
	virtual generic_distributal_code * coder (PrologAtom * atom, PrologElement * * integers, PrologElement * * doubles) {
		if (integers [0] != 0 && integers [1] != 0) return new integer_distribution_code (atom, integers [0] -> getInteger (), integers [1] -> getInteger ());
		if (doubles [0] != 0 && doubles [1] != 0) return new real_distribution_code (atom, doubles [0] -> getDouble (), doubles [1] -> getDouble ());
		return new real_distribution_code (atom);
	}
	virtual void seed_code (generic_distributal_code * code, int ip, PrologElement * * integers, int dp) {
		if (ip == 1) code -> seeder (integers [0] -> getInteger ());
		else if (ip == 3) code -> seeder (integers [2] -> getInteger ());
	}
};

class distribution_normal : public generic_distributor {
public:
	class distribution_code : public generic_distributal_code {
	public:
		std :: normal_distribution <> normal;
		virtual void random_code (PrologElement * var) {var -> setDouble (normal (engine));}
		distribution_code (PrologAtom * atom, double mean, double std)
			: generic_distributal_code (atom), normal (mean, std) {}
		distribution_code (PrologAtom * atom) : generic_distributal_code (atom) {}
	};
	virtual generic_distributal_code * coder (PrologAtom * atom, PrologElement * * integers, PrologElement * * doubles) {
		if (doubles [0] != 0 && doubles [1] != 0) return new distribution_code (atom, doubles [0] -> getDouble (), doubles [1] -> getDouble ());
		return new distribution_code (atom);
	}
};

class distribution_bernoulli : public generic_distributor {
public:
	class distribution_code : public generic_distributal_code {
	public:
		std :: bernoulli_distribution bernoulli;
		virtual void random_code (PrologElement * var) {var -> setInteger (bernoulli (engine));}
		distribution_code (PrologAtom * atom, double p)
			: generic_distributal_code (atom), bernoulli (p) {}
		distribution_code (PrologAtom * atom) : generic_distributal_code (atom) {}
	};
	virtual generic_distributal_code * coder (PrologAtom * atom, PrologElement * * integers, PrologElement * * doubles) {
		if (doubles [0] != 0) return new distribution_code (atom, doubles [0] -> getDouble ());
		return new distribution_code (atom);
	}
};

class distribution_binomial : public generic_distributor {
public:
	class distribution_code : public generic_distributal_code {
	public:
		std :: binomial_distribution <> binomial;
		virtual void random_code (PrologElement * var) {var -> setInteger (binomial (engine));}
		distribution_code (PrologAtom * atom, int n, double p)
			: generic_distributal_code (atom), binomial (n, p) {}
		distribution_code (PrologAtom * atom) : generic_distributal_code (atom) {}
	};
	virtual generic_distributal_code * coder (PrologAtom * atom, PrologElement * * integers, PrologElement * * doubles) {
		if (integers [0] != 0 && doubles [0] != 0) return new distribution_code (atom, integers [0] -> getInteger (), doubles [0] -> getDouble ());
		return new distribution_code (atom);
	}
	virtual void seed_code (generic_distributal_code * code, int ip, PrologElement * * integers, int dp) {
		if (ip == 1 && dp < 1) code -> seeder (integers [0] -> getInteger ());
		if (ip == 2 && dp > 0) code -> seeder (integers [1] -> getInteger ());
	}
};

class distribution_negative_binomial : public generic_distributor {
public:
	class distribution_code : public generic_distributal_code {
	public:
		std :: negative_binomial_distribution <> binomial;
		virtual void random_code (PrologElement * var) {var -> setInteger (binomial (engine));}
		distribution_code (PrologAtom * atom, int n, double p)
			: generic_distributal_code (atom), binomial (n, p) {}
		distribution_code (PrologAtom * atom) : generic_distributal_code (atom) {}
	};
	virtual generic_distributal_code * coder (PrologAtom * atom, PrologElement * * integers, PrologElement * * doubles) {
		if (integers [0] != 0 && doubles [0] != 0) return new distribution_code (atom, integers [0] -> getInteger (), doubles [0] -> getDouble ());
		return new distribution_code (atom);
	}
	virtual void seed_code (generic_distributal_code * code, int ip, PrologElement * * integers, int dp) {
		if (ip == 1 && dp < 1) code -> seeder (integers [0] -> getInteger ());
		if (ip == 2 && dp > 0) code -> seeder (integers [1] -> getInteger ());
	}
};

class distribution_exponential : public generic_distributor {
public:
	class distribution_code : public generic_distributal_code {
	public:
		std :: exponential_distribution <> exponential;
		virtual void random_code (PrologElement * var) {var -> setDouble (exponential (engine));}
		distribution_code (PrologAtom * atom, double mean)
			: generic_distributal_code (atom), exponential (mean) {}
		distribution_code (PrologAtom * atom) : generic_distributal_code (atom) {}
	};
	virtual generic_distributal_code * coder (PrologAtom * atom, PrologElement * * integers, PrologElement * * doubles) {
		if (doubles [0] != 0) {
			double mean = doubles [0] -> getDouble ();
			if (mean == 0.0) return 0;
			return new distribution_code (atom, 1.0 / mean);
		}
		if (integers [0] != 0) {
			double mean = (double) integers [0] -> getInteger ();
			if (mean == 0.0) return 0;
			return new distribution_code (atom, 1.0 / mean);
		}
		return new distribution_code (atom);
	}
	virtual void seed_code (generic_distributal_code * code, int ip, PrologElement * * integers, int dp) {
		if (dp > 0 && ip > 0) code -> seeder (integers [0] -> getInteger ());
		if (dp < 1 && ip > 1) code -> seeder (integers [1] -> getInteger ());
	}
};

class distribution_gamma : public generic_distributor {
public:
	class distribution_code : public generic_distributal_code {
	public:
		std :: gamma_distribution <> gamma;
		virtual void random_code (PrologElement * var) {var -> setDouble (gamma (engine));}
		distribution_code (PrologAtom * atom, double alpha, double beta)
			: generic_distributal_code (atom), gamma (alpha, beta) {}
		distribution_code (PrologAtom * atom) : generic_distributal_code (atom) {}
	};
	virtual generic_distributal_code * coder (PrologAtom * atom, PrologElement * * integers, PrologElement * * doubles) {
		if (doubles [0] != 0 && doubles [1] != 0) return new distribution_code (atom, doubles [0] -> getDouble (), doubles [1] -> getDouble ());
		return new distribution_code (atom);
	}
};

class distribution_geometric : public generic_distributor {
public:
	class distribution_code : public generic_distributal_code {
	public:
		std :: geometric_distribution <> geometric;
		virtual void random_code (PrologElement * var) {var -> setDouble (geometric (engine));}
		distribution_code (PrologAtom * atom, double p)
			: generic_distributal_code (atom), geometric (p) {}
		distribution_code (PrologAtom * atom) : generic_distributal_code (atom) {}
	};
	virtual generic_distributal_code * coder (PrologAtom * atom, PrologElement * * integers, PrologElement * * doubles) {
		if (doubles [0] != 0) return new distribution_code (atom, doubles [0] -> getDouble ());
		return new distribution_code (atom);
	}
};

class distribution_poisson : public generic_distributor {
public:
	class distribution_code : public generic_distributal_code {
	public:
		std :: poisson_distribution <> poisson;
		virtual void random_code (PrologElement * var) {var -> setDouble (poisson (engine));}
		distribution_code (PrologAtom * atom, double mean)
			: generic_distributal_code (atom), poisson (mean) {}
		distribution_code (PrologAtom * atom) : generic_distributal_code (atom) {}
	};
	virtual generic_distributal_code * coder (PrologAtom * atom, PrologElement * * integers, PrologElement * * doubles) {
		if (doubles [0] != 0) return new distribution_code (atom, doubles [0] -> getDouble ());
		return new distribution_code (atom);
	}
};

class distribution_cauchy : public generic_distributor {
public:
	class distribution_code : public generic_distributal_code {
	public:
		std :: cauchy_distribution <> cauchy;
		virtual void random_code (PrologElement * var) {var -> setDouble (cauchy (engine));}
		distribution_code (PrologAtom * atom, double alpha, double beta)
			: generic_distributal_code (atom), cauchy (alpha, beta) {}
		distribution_code (PrologAtom * atom) : generic_distributal_code (atom) {}
	};
	virtual generic_distributal_code * coder (PrologAtom * atom, PrologElement * * integers, PrologElement * * doubles) {
		if (doubles [0] != 0 && doubles [1] != 0) return new distribution_code (atom, doubles [0] -> getDouble (), doubles [1] -> getDouble ());
		return new distribution_code (atom);
	}
};

class distribution_chi_squared : public generic_distributor {
public:
	class distribution_code : public generic_distributal_code {
	public:
		std :: chi_squared_distribution <> chi_squared;
		virtual void random_code (PrologElement * var) {var -> setDouble (chi_squared (engine));}
		distribution_code (PrologAtom * atom, double n)
			: generic_distributal_code (atom), chi_squared (n) {}
		distribution_code (PrologAtom * atom) : generic_distributal_code (atom) {}
	};
	virtual generic_distributal_code * coder (PrologAtom * atom, PrologElement * * integers, PrologElement * * doubles) {
		if (doubles [0] != 0) return new distribution_code (atom, doubles [0] -> getDouble ());
		return new distribution_code (atom);
	}
};

class distribution_student_t : public generic_distributor {
public:
	class distribution_code : public generic_distributal_code {
	public:
		std :: student_t_distribution <> student_t;
		virtual void random_code (PrologElement * var) {var -> setDouble (student_t (engine));}
		distribution_code (PrologAtom * atom, double n)
			: generic_distributal_code (atom), student_t (n) {}
		distribution_code (PrologAtom * atom) : generic_distributal_code (atom) {}
	};
	virtual generic_distributal_code * coder (PrologAtom * atom, PrologElement * * integers, PrologElement * * doubles) {
		if (doubles [0] != 0) return new distribution_code (atom, doubles [0] -> getDouble ());
		return new distribution_code (atom);
	}
};

class distribution_weibull : public generic_distributor {
public:
	class distribution_code : public generic_distributal_code {
	public:
		std :: weibull_distribution <> weibull;
		virtual void random_code (PrologElement * var) {var -> setDouble (weibull (engine));}
		distribution_code (PrologAtom * atom, double alpha, double beta)
			: generic_distributal_code (atom), weibull (alpha, beta) {}
		distribution_code (PrologAtom * atom) : generic_distributal_code (atom) {}
	};
	virtual generic_distributal_code * coder (PrologAtom * atom, PrologElement * * integers, PrologElement * * doubles) {
		if (doubles [0] != 0 && doubles [1] != 0) return new distribution_code (atom, doubles [0] -> getDouble (), doubles [1] -> getDouble ());
		return new distribution_code (atom);
	}
};

PrologNativeCode * PrologDistributionServiceClass :: getNativeCode (char * name) {
	if (strcmp (name, "uniform") == 0) return new distribution_uniform ();
	if (strcmp (name, "normal") == 0) return new distribution_normal ();
	if (strcmp (name, "gaussian") == 0) return new distribution_normal ();
	if (strcmp (name, "bernoulli") == 0) return new distribution_bernoulli ();
	if (strcmp (name, "binomial") == 0) return new distribution_binomial ();
	if (strcmp (name, "negative_binomial") == 0) return new distribution_negative_binomial ();
	if (strcmp (name, "exponential") == 0) return new distribution_exponential ();
	if (strcmp (name, "gamma") == 0) return new distribution_gamma ();
	if (strcmp (name, "geometric") == 0) return new distribution_geometric ();
	if (strcmp (name, "poisson") == 0) return new distribution_poisson ();
	if (strcmp (name, "cauchy") == 0) return new distribution_cauchy ();
	if (strcmp (name, "chi_squared") == 0) return new distribution_chi_squared ();
	if (strcmp (name, "student_t") == 0) return new distribution_student_t ();
	if (strcmp (name, "weibull") == 0) return new distribution_weibull ();
	return 0;
}
