///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2015 Robert P. Wolf                       //
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

package Prolog;

/*
PrologResolution :: PrologResolution (PrologRoot * root) TRACKING (7) {
	this -> root = root;
	q_root = NULL;
	external_query_pointer = NULL;
	root_actual = NULL;
	root_formal = NULL;
	var_root = NULL;
	var_counter = 0;
	next = this;
	this -> timeout = 0;
	this -> timeout_query = NULL;
}
*/

public class PrologResolution {
	public PrologRoot root;
	public long timeout = 0;
	public PrologElement timeout_query = null;
	public PrologQuery q_root = null;
	public PrologVariable var_root = null;
	public int var_counter = 0;
	public PrologMatch root_actual = null;
	public PrologMatch root_formal = null;
	public PrologResolution (PrologRoot root) {this . root = root;}
	public PrologVariable newVariable () {return var_root = new PrologVariable (var_root);}
	public PrologVariable newVariable (PrologElement term, boolean location) {return var_root = new PrologVariable (term, location, var_root);}
	public PrologQuery getQuery () {return q_root;}
	public void reset () {root_actual = null; root_formal = null; var_root = null; var_counter = 0;}
	boolean match (PrologElement actual, boolean ac, PrologElement formal, boolean fc) {
		int typea, typef;
		do {
			typea = actual . type;
			typef = formal . type;
			PrologMatch v1 = null;
			PrologMatch v2 = null;
			PrologVariable var = null;
			if (typea == 2) {
				if (typef == 2) {
					if (ac) {if (root_actual != null) v1 = root_actual . getVar (actual . integer);}
					else {if (root_formal != null) v1 = root_formal . getVar (actual . integer);}
					if (fc) {if (root_actual != null) v2 = root_actual . getVar (formal . integer);}
					else {if (root_formal != null) v2 = root_formal . getVar (formal . integer);}
					if (v1 == null) {
						if (v2 == null) {
							var = newVariable (); //new PrologVariable ();
							if (fc) root_actual = new PrologMatch (formal . integer, var, root_actual);
							else root_formal = new PrologMatch (formal . integer, var, root_formal);
						} else var = v2 . var;
						if (ac) root_actual = new PrologMatch (actual . integer, var, root_actual);
						else root_formal = new PrologMatch (actual . integer, var, root_formal);
						return true;
					}
					if (v2 == null) {
						if (fc) root_actual = new PrologMatch (formal . integer, v1 . var, root_actual);
						else root_formal = new PrologMatch (formal . integer, v1 . var, root_formal);
						return true;
					}
					if (v1 . var . term == null) {
						var = v1 . var;
						root_actual . connect_var (var, v2 . var);
						root_formal . connect_var (var, v2 . var);
						return true;
					}
					if (v2 . var . term == null) {
						var = v2 . var;
						root_actual . connect_var (var, v1 . var);
						root_formal . connect_var (var, v1 . var);
						return true;
					}
					return match (v1 . var . term, v1 . var . location, v2 . var . term, v2 . var . location);
				} else {
					if (ac) {
						if (root_actual != null) v1 = root_actual . getVar (actual . integer);
						if (v1 == null) {
							root_actual = new PrologMatch (actual . integer, formal, fc, root_actual, this);
							return true;
						}
					} else {
						if (root_formal != null) v1 = root_formal . getVar (actual . integer);
						if (v1 == null) {
							root_formal = new PrologMatch (actual . integer, formal, fc, root_formal, this);
							return true;
						}
					}
					if (v1 . var . term == null) {
						v1 . var . term = formal;
						v1 . var . location = fc;
						return true;
					}
					return match (v1 . var . term, v1 . var . location, formal, fc);
				}
			}

			if (typef == 2) {
				if (fc) {
					if (root_actual != null) v1 = root_actual . getVar (formal . integer);
					if (v1 == null) {
						root_actual = new PrologMatch (formal . integer, actual, ac, root_actual, this);
						return true;
					}
				} else {
					if (root_formal != null) v1 = root_formal . getVar (formal . integer);
					if (v1 == null) {
						root_formal = new PrologMatch (formal . integer, actual, ac, root_formal, this);
						return true;
					}
				}
				if (v1 . var . term == null) {
					v1 . var . term = actual;
					v1 . var . location = ac;
					return true;
				}
				return match (actual, ac, v1 . var . term, v1 . var . location);
			}

			if (typea != typef) return false;

			switch (typea) {
			case 8: return actual . integer == formal . integer;
			case 9: return actual . floating_point == formal . floating_point;
			case 3: return actual . atom == formal . atom;
			case 0: return true;
			case 1:
				if (! match (actual . left, ac, formal . left, fc)) return false;
				actual = actual . right; formal = formal . right;
				break;
			case 4: case 5: return true;
			case 6:
				if (actual . text == null) return false;
				if (formal . text == null) return false;
				return actual . text . equals (formal . text);
			case 7: return actual . head == formal . head;
			default: return false;
			}
		} while (typea == 1);
		return false;
	}
	public PrologElement match_product (PrologElement actual, boolean ac) {
		PrologMatch v1 = null;
		PrologVariable var;
		int ind;
		PrologElement ret;
		PrologElement main_return = null;
		PrologElement target = null;
		int type;
		do {
			if (actual == null) {if (target != null) target . right = null; return main_return;}
			type = actual . type;
			switch (type) {
			case 2:
				if (ac) {
					if (root_actual != null) v1 = root_actual . getVar (actual . integer);
					if (v1 == null) {
						ret = new PrologElement ();
						ret . type = 2;
						root_actual = new PrologMatch (actual . integer, root_actual, this);
						root_actual . var . variable_id = var_counter;
						ret . integer = var_counter;
						var_counter++;
						if (target != null) target . right = ret;
						return main_return != null ? main_return : ret;
					}
				} else {
					if (root_formal != null) v1 = root_formal . getVar (actual . integer);
					if (v1 == null) {
						ret = new PrologElement ();
						ret . type = 2;
						root_formal = new PrologMatch (actual . integer, root_formal, this);
						root_formal . var . variable_id = var_counter;
						ret . integer = var_counter;
						var_counter++;
						if (target != null) target . right = ret;
						return main_return != null ? main_return : ret;
					}
				}
				var = v1 . var;
				if (var . term == null) {
					ret = new PrologElement ();
					ret . type = 2;
					ind = var . variable_id;
					if (ind < 0) {
						var . variable_id = var_counter;
						ind = var_counter;
						var_counter++;
					}
					ret . integer = ind;
					if (target != null) target . right = ret;
					return main_return != null ? main_return : ret;
				}
				ret = match_product (var . term, var . location);
				if (target != null) target . right = ret;
				return main_return != null ? main_return : ret;
			case 1:
				ret = new PrologElement ();
				if (target != null) target . right = ret;
				ret . type = 1;
				ret . left = match_product (actual . left, ac);
				if (main_return == null) main_return = ret;
				target = ret;
				actual = actual . right;
				break;
			case 8: ret = new PrologElement (actual . integer); if (target != null) target . right = ret; return main_return != null ? main_return : ret;
			case 9: ret = new PrologElement (actual . floating_point); if (target != null) target . right = ret; return main_return != null ? main_return : ret;
			case 6: ret = new PrologElement (actual . text); if (target != null) target . right = ret; return main_return != null ? main_return : ret;
			case 7: ret = new PrologElement (actual . head); if (target != null) target . right = ret; return main_return != null ? main_return : ret;
			case 3: ret = new PrologElement (actual . atom); if (target != null) target . right = ret; return main_return != null ? main_return : ret;
			default: ret = new PrologElement (); ret . type = type; if (target != null) target . right = ret; return main_return != null ? main_return : ret;
			}
		} while (type == 1);
		return main_return;
	}
	public void drop_stack (PrologQuery from, PrologQuery to) {while (from != null && from != to) from = from . stack;}
	public int res_forward () {
		PrologElement term = q_root . query;
		PrologQuery q;
		if (term . type != 1) {root . message ("Wrong query (querries expected)."); return 0;}
		term = term . right;
		if (term . type != 1) {root . message ("Wrong query (query expected)."); return 0;}
		term = term . left;
		if (term . type == 5) return 0; // genuine fail
		if (term . type == 4) {
			// slash maker
			q_root . query . drop_one_right ();
			q_root . drop_stack_to_fail_target ();
			if (q_root . original && q_root . stack == q_root . context && q_root . stack != null) {
				if (q_root . stack . query . right . right . type == 0) {
					reset ();
					match (q_root . stack . query . right . left . right, true, q_root . query . left . right, false);
					q_root . stack . query = root . pair (match_product (q_root . stack . query . left, true), match_product (q_root . query . right, false));
					q = q_root . stack;
					drop_stack (q_root, q);
					q_root = q;
					q_root . fail_target = q_root . stack;
					q_root . original = false;
				} else q_root . stack . query . right . left . left . head = null;
			}
			if (q_root . query . right . type == 0) return 2;
			return 1;
		}
		if (term . type != 1) {root . message ("Wrong query (query is not a pair)."); return 0;}
		if (term . left == null) {root . message ("Wrong query (query head does not exist)."); return 0;}
		if (term . left . type != 3) {root . message ("Wrong query (query head is not an atom)."); return 0;}
		PrologNativeCode m = term . left . atom . machine;
		if (m != null) {
			//	Machine goes here
			reset ();
			PrologElement parameters = match_product (term . right, true);
			if (! m . code (parameters, this)) {return 0;}
			reset ();
			if (match (term . right, true, parameters, false)) {
				PrologElement new_head = match_product (q_root . query . left, true);
				PrologElement new_tail = match_product (q_root . query . right . right, true);
				q_root . query = root . pair (new_head, new_tail);
				if (new_tail . type == 0) return 2;
				return 1;
			}
			return 0;
		}
		PrologElement clausa = term . left . atom . firstClause;
		if (clausa == null) {root . message ("Free atom [" + term . left . atom . atomName + "]."); return 0;}
		PrologElement relation_atom = new PrologElement ();
		relation_atom . type = 3;
		relation_atom . atom = term . left . atom;
		return sub_res_forward (relation_atom, term, clausa);
	}
	public int sub_res_forward (PrologElement relation_atom, PrologElement term, PrologElement clausa) {
		Object next_clausa;
		PrologElement new_tail;
		while (clausa != null) {
			reset ();
			next_clausa = clausa . left . left . head;
			if (match (term . right, true, clausa . left . right, false)) {
				new_tail = match_product (clausa . right, false);
				if (new_tail . type == 0) {
					new_tail = match_product (q_root . query . right . right, true);
					PrologElement after_unar = root . pair (match_product (q_root . query . left, true), new_tail);
					if (next_clausa == null) q_root . query = after_unar;
					else {
						term . left . head = next_clausa;
						q_root = new PrologQuery (q_root, q_root . context, q_root . fail_target, q_root . original, after_unar);
					}
					if (new_tail . type == 0) return 2;
					return 1;
				} else
					if (next_clausa == null && q_root . query . right . right . type == 0) {
						q_root . query = root . pair (match_product (q_root . query . left, true), new_tail);
						q_root . fail_target = q_root . stack;
						q_root . original = false;
						return 1;
					}
				term . left . head = next_clausa;
				q_root = new PrologQuery (q_root, q_root, q_root, true, root . pair (root . pair (relation_atom, match_product (clausa . left . right, false)), new_tail));
				return 1;
			}
			clausa = (PrologElement) next_clausa;
		}
		return 0;
	}
	public int res_back_back () {
		PrologQuery context = q_root . context;
		if (context == null) return 2;
		reset ();
		match (context . query . right . left . right, true, q_root . query . left . right, false);
		PrologElement new_head = match_product (context . query . left, true);
		PrologElement new_tail = match_product (context . query . right . right, true);
		q_root . fail_target = context . fail_target;
		q_root . original = context . original;
		q_root . context = context . context;
		q_root . query = root . pair (new_head, new_tail);
		if (new_tail . type == 0) return 1;
		return 0;
	}
	public int res_fail_back () {
		q_root = q_root . stack;
		if (q_root == null) return 5;
		PrologElement term = q_root . query . right . left;
		PrologElement relation_atom = new PrologElement ();
		relation_atom . type = 3;
		relation_atom . atom = term . left . atom;
		PrologElement clausa = (PrologElement) term . left . head;
		return sub_res_forward (relation_atom, term, clausa);
	}
	public int resolution (PrologElement query) {
		// returns: 0 = fail, 1 = success, 2 = no space left, 3 = wrong query
		// query is a clause, which is a list anyway
		// the head of the query is a return interface and can be any term
		// the rest of the query are calls
		// after successfull processing only the interface
		// is left from the original query matched with the result
		// after failure the query remains unchanged
		// query egzample: [[*x *y *z] [c1 *x] [c2 *y] [c3 *z]]
		if (query == null) return 3;
		if (! query . isPair ()) return 3;
		reset ();
		q_root = new PrologQuery (match_product (query, true));
		int ctrl;
		do {
			if (timeout > 0) {
				if (root . get_system_time () >= timeout) {
					drop_stack (q_root, null); q_root = null;
					reset ();
					if (timeout_query != null) root . resolution (root . pair (root . head (null), timeout_query . duplicate ()));
					return 2;
				}
			}
			ctrl = res_forward ();
			while (ctrl == 0) ctrl = res_fail_back ();
			if (ctrl == 2)
				while (ctrl != 0) {
					ctrl = res_back_back ();
					if (ctrl == 2) {
						reset ();
						query . left = match_product (q_root . query . left, true);
						query . right = match_product (q_root . query . right, true);
						drop_stack (q_root, null); q_root = null;
						reset ();
						return 1;
					}
				}
		} while (ctrl != 5);
		reset ();
		return 0;
	}
}
