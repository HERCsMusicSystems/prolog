///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2007 Robert P. Wolf                       //
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

#include "prolog_neural.h"
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>

#ifdef LINUX_OPERATING_SYSTEM
#include <stdlib.h>
#endif

enum {
	NEURAL = 0,
	NORMALISED,
	KOHONEN
};

typedef double (* non_linear_function) (double);

class neuron {
private:
	double excite (double * vector);
public:
	double * weights;
	int synapses;
	double error;
	double * inputs;
	double output;
	double excite (double * vector, non_linear_function function);
	void expect (double expected);
	double backpropagate (int synapse);
	void train (double learning_factor, non_linear_function derivative);
	void normalise (void);
	void unit (void);
	neuron (int synapses);
	~ neuron (void);
};

typedef neuron * neuron_pointer;

static double noLin1 (double parameter) {
	if (parameter >= 1.0) return 1.0;
	if (parameter <= 0.0) return 0.0;
	return 0.5 + cos (M_PI * parameter) * -0.5;
}

neuron :: neuron (int synapses) {
	this -> synapses = synapses;
	this -> weights = new double [synapses + 16];
	this -> inputs = new double [synapses + 16];
	for (int ind = 0; ind < synapses; ind++) {
		weights [ind] = (double) rand () / (double) RAND_MAX;
		inputs [ind] = 0.0;
	}
	this -> error = 0.0;
}

neuron :: ~ neuron (void) {
	if (weights != NULL) delete [] weights; weights = NULL;
	if (inputs != NULL) delete [] inputs; inputs = NULL;
}

void neuron :: normalise (void) {
	double suma = 0.0;
	for (int ind = 0; ind < synapses; ind++) suma += weights [ind];
	if (suma > 0.0) {
		suma = 1.0 / suma;
		for (int ind = 0; ind < synapses; ind++) weights [ind] *= suma;
	}
}

void neuron :: unit (void) {
	double suma = 0.0;
	for (int ind = 0; ind < synapses; ind++) suma += weights [ind] * weights [ind];
	if (suma > 0.0) {
		suma = sqrt (suma);
		suma = 1.0 / suma;
		for (int ind = 0; ind < synapses; ind++) weights [ind] *= suma;
	}
}

double neuron :: excite (double * vector) {
	double accumulator = 0.0;
	double * wp = weights;
	for (int ind = 0; ind < synapses; ind++) {
		inputs [ind] = * vector;
		accumulator += (* (wp++)) * (* (vector++));
	}
	return accumulator;
}

double neuron :: excite (double * vector, non_linear_function function) {
	error = 0.0;
	if (function == NULL) return output = excite (vector);
	return output = function (excite (vector));
}

void neuron :: expect (double expected) {error = expected - output;}

double neuron :: backpropagate (int synapse) {return error * weights [synapse];}

double derivative (double in, non_linear_function noLin) {return noLin == NULL ? 1.0 : (noLin (in + 0.000976563) - noLin (in)) / 0.000976563;}

void neuron :: train (double learning_factor, non_linear_function noLin) {
	double * wp = weights;
	double * vp = inputs;
	double delta = error * learning_factor;
	for (int ind = 0; ind < synapses; ind++) {* (wp++) += * (vp++) * delta * derivative (output, noLin);}
}

static neuron_pointer * create_neural_layer (int number_of_neurons, int synapses) {
	neuron_pointer * layer = new neuron_pointer [number_of_neurons];
	for (int ind = 0; ind < number_of_neurons; ind++) layer [ind] = new neuron (synapses);
	return layer;
}

void destroy_neural_layer (neuron_pointer * layer, int number_of_neurons) {
	if (layer == NULL) return;
	for (int ind = 0; ind < number_of_neurons; ind++) delete layer [ind];
	delete [] layer;
}

class PrologNeuralLayer : public PrologNativeCode {
public:
	PrologAtom * atom;
	neuron_pointer * layer;
	double * exposition;
	double * expectation;
	double * results;
	non_linear_function noLin;
	bool trained;
	bool expectation_provided;
	int neurons;
	int synapses;
	PrologNeuralLayer * previous;
	PrologNeuralLayer * next;
	void unit (double * exposition) {
		double suma = 0.0;
		for (int ind = 0; ind < synapses; ind++) suma += exposition [ind] * exposition [ind];
		if (suma > 0.0) {
			suma = sqrt (suma);
			suma = 1.0 / suma;
			for (int ind = 0; ind < synapses; ind++) exposition [ind] *= suma;
		}
	}
	void normalise (double * exposition) {
		double suma = 0.0;
		for (int ind = 0; ind < synapses; ind++) suma += exposition [ind];
		if (suma > 0.0) {
			suma = sqrt (suma);
			suma = 1.0 / suma;
			for (int ind = 0; ind < synapses; ind++) exposition [ind] *= suma;
		}
	}
	virtual void post_load_normalisation (neuron * n) {}
	virtual void expose (double * exposition, non_linear_function noLin) {
		unit (exposition);
		if (previous != NULL) exposition = previous -> results;
		for (int ind = 0; ind < neurons; ind++) {
			results [ind] = layer [ind] -> excite (exposition, noLin);
		}
	}
	virtual void expect (double * expectation) {
		for (int ind = 0; ind < neurons; ind++) {
			layer [ind] -> expect (expectation [ind]);
		}
	}
	void backpropagate (void) {
		trained = false;
		if (previous == NULL) return;
		for (int ind = 0; ind < synapses; ind++) {
			double accumulator = 0.0;
			for (int sub = 0; sub < neurons; sub++) {
				accumulator += layer [sub] -> backpropagate (ind);
			}
			previous -> layer [ind] -> error = accumulator;
		}
		previous -> backpropagate ();
	}
	virtual void train (double learning_factor) {
		if (trained) return;
		for (int ind = 0; ind < neurons; ind++) layer [ind] -> train (learning_factor, noLin);
		trained = true;
		if (previous != NULL) previous -> train (learning_factor);
	}
	void unit (void) {for (int ind = 0; ind < neurons; ind++) layer [ind] -> unit ();}
	void normalise (void) {for (int ind = 0; ind < neurons; ind++) layer [ind] -> normalise ();}
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		expectation_provided = false;
		if (parameters -> isEarth ()) {
			atom -> setMachine (0);
			delete this;
			return true;
		}
		if (! parameters -> isPair ()) {
			for (int ind = 0; ind < neurons; ind++) {
				parameters -> setPair ();
				PrologElement * el = parameters -> getLeft ();
				neuron_pointer neuron = layer [ind];
				for (int sub = 0; sub < neuron -> synapses; sub++) {
					el -> setPair ();
					el -> getLeft () -> setDouble (neuron -> weights [sub]);
					el = el -> getRight ();
				}
				parameters = parameters -> getRight ();
			}
			return true;
		}
		PrologElement * p1 = parameters -> getLeft ();
		parameters = parameters -> getRight ();
		if (p1 -> isInteger ()) {
			switch (p1 -> getInteger ()) {
			case 0: noLin = NULL; return true;
			case 1: noLin = noLin1; return true;
			default: return false;
			}
			return false;
		}
		if (p1 -> isDouble ()) {train (p1 -> getDouble ()); return true;}
		if (p1 -> isVar ()) p1 -> setAtom (new PrologAtom ());
		if (p1 -> isAtom ()) {
			if (this -> next != NULL) return false;
			if (! parameters -> isPair ()) return false;
			PrologElement * new_layer_size = parameters -> getLeft ();
			if (! new_layer_size -> isInteger ()) return false;
			parameters = parameters -> getRight ();
			if (p1 -> getAtom () -> getMachine () != 0) return false;
			PrologNeuralLayer * l = new PrologNeuralLayer (p1 -> getAtom (), new_layer_size -> getInteger (), neurons);
			if (p1 -> getAtom () -> setMachine (l)) {
				this -> next = l;
				l -> previous = this;
				return true;
			}
			delete l;
			return false;
		}
		if (! p1 -> isPair ()) return false;
		if (p1 -> getLeft () -> isPair ()) {
			int ind = 0;
			while (ind < neurons && p1 -> isPair ()) {
				PrologElement * el = p1 -> getLeft ();
				neuron * n = layer [ind];
				int sub = 0;
				while (sub < n -> synapses && el -> isPair ()) {
					PrologElement * data = el -> getLeft ();
					if (! data -> isDouble ()) return false;
					n -> weights [sub] = data -> getDouble ();
					sub++;
					el = el -> getRight ();
				}
				while (sub < n -> synapses) n -> weights [sub++] = 0.0;
				post_load_normalisation (n);
				ind++;
				p1 = p1 -> getRight ();
			}
			while (ind < neurons) {
				neuron * n = layer [ind++];
				for (int sub = 0; sub < n -> synapses; sub++) n -> weights [sub] = 0.0;
				post_load_normalisation (n);
			}
			return true;
		}
		int index = 0;
		while (p1 -> isPair () && index < synapses) {
			PrologElement * expo = p1 -> getLeft ();
			if (! expo -> isDouble ()) return false;
			exposition [index++] = expo -> getDouble ();
			p1 = p1 -> getRight ();
		}
		while (index < synapses) exposition [index++] = 0.0;
		PrologElement * p2 = NULL;
		if (parameters -> isPair ()) {
			p2 = parameters -> getLeft ();
			parameters = parameters -> getRight ();
		}
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (p2 != NULL) {
			index = 0;
			while (p2 -> isPair () && index < neurons) {
				PrologElement * expect = p2 -> getLeft ();
				if (! expect -> isDouble ()) return false;
				expectation [index++] = expect -> getDouble ();
				p2 = p2 -> getRight ();
			}
			while (index < neurons) expectation [index++] = 0.0;
			expectation_provided = true;
		}
		PrologNeuralLayer * lp = this;
		while (lp -> previous != NULL) lp = lp -> previous;
		while (lp != this -> next) {
			lp -> expose (exposition, noLin);
			lp = lp -> next;
		}
		if (p2 != NULL) {
			expect (expectation);
			backpropagate ();
		}
		for (int ind = 0; ind < neurons; ind++) {
			parameters -> setPair ();
			parameters -> getLeft () -> setDouble (results [ind]);
			parameters = parameters -> getRight ();
		}
		return true;
	}
	PrologNeuralLayer (PrologAtom * atom, int neurons, int synapses) {
		this -> atom = atom;
		this -> neurons = neurons;
		this -> synapses = synapses;
		layer = create_neural_layer (neurons, synapses);
		exposition = new double [synapses + 16];
		expectation = new double [neurons + 16];
		results = new double [neurons + 16];
		noLin = NULL;
		trained = true;
		expectation_provided = false;
		previous = next = NULL;
		normalise ();
	}
	~ PrologNeuralLayer (void) {
		if (next != NULL) next -> previous = NULL;
		if (previous != NULL) previous -> next = NULL;
		if (layer != NULL) destroy_neural_layer (layer, neurons); layer = NULL;
		if (exposition != NULL) delete [] exposition; exposition = NULL;
		if (expectation != NULL) delete [] expectation; expectation = NULL;
		if (results != NULL) delete [] results; results = NULL;
	}
};

class PrologKohonenLayer : public PrologNeuralLayer {
public:
	virtual void expose (double * exposition, non_linear_function noLin) {
		unit (exposition);
		PrologNeuralLayer :: expose (exposition, noLin);
		int winner = 0;
		for (int ind = 0; ind < neurons; ind++) {if (results [ind] > results [winner]) winner = ind;}
		for (int ind = 0; ind < neurons; ind++) {results [ind] = (ind == winner ? 1.0 : 0.0);}
		if (! expectation_provided) {
			for (int ind = 0; ind < neurons; ind++) {layer [ind] -> expect (results [ind] == 0.0 ? 0.0 : 1.0);}
		}
		trained = false;
	}
	virtual void train (double learning_factor) {
		if (trained) return;
		for (int ind = 0; ind < neurons; ind++) {
			if (expectation [ind] != 0.0) {
				layer [ind] -> train (learning_factor, noLin);
				layer [ind] -> unit ();
			}
		}
		trained = true;
		if (previous != NULL) previous -> train (learning_factor);
	}
	virtual void post_load_normalisation (neuron * n) {
		n -> unit ();
	}
	PrologKohonenLayer (PrologAtom * atom, int neurons, int synapses) : PrologNeuralLayer (atom, neurons, synapses) {unit ();}
};

class PrologNormalisedLayer : public PrologNeuralLayer {
public:
	virtual void expose (double * exposition, non_linear_function noLin) {
		unit (exposition);
		PrologNeuralLayer :: expose (exposition, noLin);
	}
	PrologNormalisedLayer (PrologAtom * atom, int neurons, int synapses) : PrologNeuralLayer (atom, neurons, synapses) {unit ();}
};

class PrologNeuralLayerBuilder : public PrologNativeCode {
public:
	int type;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * atom = parameters -> getLeft ();
		if (atom -> isVar ()) atom -> setAtom (new PrologAtom ());
		if (! atom -> isAtom ()) return false;
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * neurons = parameters -> getLeft ();
		if (! neurons -> isInteger ()) return false;
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * synapses = parameters -> getLeft ();
		if (! synapses -> isInteger ()) return false;
		if (atom -> getAtom () -> getMachine () != 0) return false;
		PrologNeuralLayer * l = NULL;
		switch (type) {
		case NORMALISED: l = new PrologNormalisedLayer (atom -> getAtom (), neurons -> getInteger (), synapses -> getInteger ()); break;
		case KOHONEN: l = new PrologKohonenLayer (atom -> getAtom (), neurons -> getInteger (), synapses -> getInteger ()); break;
		default: l = new PrologNeuralLayer (atom -> getAtom (), neurons -> getInteger (), synapses -> getInteger ()); break;
		}
		if (l == NULL) return false;
		if (atom -> getAtom () -> setMachine (l)) return true;
		delete l;
		return false;
	}
	PrologNeuralLayerBuilder (int type = NEURAL) {this -> type = type;}
};

PrologNativeCode * PrologNeuralServiceClass :: getNativeCode (char * name) {
	if (strcmp (name, "NEURAL_LAYER") == 0) return new PrologNeuralLayerBuilder ();
	if (strcmp (name, "NORMALISED_LAYER") == 0) return new PrologNeuralLayerBuilder (NORMALISED);
	if (strcmp (name, "KOHONEN_LAYER") == 0) return new PrologNeuralLayerBuilder (KOHONEN);
	return NULL;
}


