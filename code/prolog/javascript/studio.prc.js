
studio . setResource (['prolog', 'studio'],
function (root, directory) {
  var list = new function () {
    this . code = function (el) {
      if (el . type === 0) {root . log (root . list () . join (' ')); return true;}
      if (el . type === 2) {
        var l = root . list ();
        for (var ind in l) el = el . setNativePair (l [ind]);
        return true;
      }
      if (el . type === 1) {
        var first = el . left;
        if (first . type === 2) {first . setNative (root . list () . join (' ')); return true;}
        el = el . right;
        if (first . type === 3) {
          if (el . type === 0) {root . log (root . listAtom (first . left . name) . join ('\n')); return true;}
          if (el . type === 2) {
            var l = root . listAtom (first . left . name);
            for (var ind in l) el = el . setNativePair (l [ind]);
            return true;
          }
          if (el . type === 1) {el . left . setNative (root . listAtom (first . left . name) . join ('\n')); return true;}
        }
        if (first . type === 6) {
          first = first . left;
          if (typeof (first) !== 'string') return false;
          if (el . type === 0) {root . log (root . list (first) . join (' ')); return true;}
          if (el . type === 2) {
            var l = root . list (first);
            for (var ind in l) el = el . setNativePair (l [ind]);
            return true;
          }
          if (el . type === 1) {el . left . setNative (root . list (first) . join (' ')); return true;}
        }
      }
      return false;
    };
  };
  var pp = new function () {
    this . code = function (el) {
      var out = '';
      while (el . type === 1) {out += root . getValue (el . left); el = el . right;}
      console . log (out);
      return true;
    };
  };
  var sum = new function () {
    this . code = function (el) {
      if (! el . type === 1) return false;
      var e1 = el . left; el = el . right; if (! el . type === 1) return false;
      var e2 = el . left; el = el . right;
      var e3 = el . type === 1 ? el . left : el;
      if (e1 . type === 6) {
        if (e2 . type === 6) {e3 . setNative (e1 . left + e2 . left); return true;}
        if (e2 . type === 3) {e3 . setNative (e1 . left + e2 . left . name); return true;}
      }
      if (e1 . type === 3) {
        if (e2 . type === 6) {e3 . setNative (e1 . left . name + e2 . left); return true;}
        if (e2 . type === 3) {e3 . setNative (e1 . left . name + e2 . left . name); return true;}
      }
      if (e3 . type === 6) {
        if (e2 . type === 6) {e1 . setNative (Number (e3 . left) - Number (e2 . left)); return true;}
        if (e1 . type === 6) {e2 . setNative (Number (e3 . left) - Number (e1 . left)); return true;}
      }
      return false;
    };
  };
  var add = new function () {
    this . code = function (el) {
      if (! el . type === 1) return false;
      var result = null, sum = 0;
      if (el . left . type === 2) {result = el . left; el = el . right;}
      switch (el . left . type) {
        case 6: sum = el . left . left; el = el . right; break;
        case 3: sum = el . left . left . name; el = el . right; break;
        default: break;
      }
      while (el . type === 1) {
        switch (el . left . type) {
          case 6: sum += el . left . left; break;
          case 1:
            var sub = 1, ell = el . left;
            while (ell . type === 1) {sub *= Number (ell . left . left); ell = ell . right;}
            sum += sub;
            break;
          case 3: sum += el . left . left . name; break;
          case 2: el . left . setNative (sum); break;
          default: break;
        }
        el = el . right;
      }
      if (result === null) return true;
      result . setNative (sum === null ? 0 : sum); return true;
    };
  };
  var mult = new function () {
    this . code = function (el) {
      if (! el . type === 1) return false;
      var result = null, ret = 1;
      if (el . left . type === 2) {result = el . left; el = el . right;}
      while (el . type === 1) {
        switch (el . left . type) {
          case 6: ret *= el . left . left; break;
          case 2: el . left . setNative (ret); break;
          default: break;
        }
        el = el . right;
      }
      if (result === null) return true;
      result . setNative (ret); return true;
    };
  };
  var addd = function (delta) {
    this . code = function (el) {
      if (el . type !== 1) return false;
      var left = el . left, ret = NaN;
      switch (left . type) {
        case 6: ret = left . left + delta; break;
        case 3: ret = left . left . name + delta; break;
        default: break;
      }
      el = el . right; if (el . type === 1) el = el . left;
      el . setNative (ret); return true;
    };
  };
  var add1 = new addd (1), sub1 = new addd (-1);
  var zero_param = function (value) {
    this . code = function (el) {if (el . type === 1) el = el . left; el . setNative (value); return true;};
  };
  var one_param = function (f, rev) {
    this . code = function (el) {
      if (el . type !== 1) return false;
      var a = el . left; el = el . right; if (el . type === 1) el = el . left;
      if (a . type === 6) {el . setNative (f (a . left)); return true;}
      if (el . type === 6) {a . setNative (rev (el . left)); return true;}
      return false;
    };
  };
  var logical_one_param = function (f) {
    this . code = function (el) {
      if (el . type !== 1) return false;
      var a = el . left; el = el . right; if (el . type === 1) el = el . left;
      switch (a . type) {
        case 6: el . setNative (f (a . left)); return true;
        case 3: el . setNative (f (a . left . name)); return true;
        default: break;
      }
      return false;
    };
  };
  var logical_two_params = function (f) {
    this . code = function (el) {
      if (el . type !== 1) return false;
      var a = el . left; if (a . type !== 6) return false;
      el = el . right; if (el . type !== 1) return false;
      var b = el . left; if (b . type !== 6) return false;
      el = el . right; if (el . type === 1) el = el . left;
      el . setNative (f (a . left, b . left)); return true;
    };
  };
  var and = new logical_two_params (function (a, b) {return a & b;});
  var or =  new logical_two_params (function (a, b) {return a | b;});
  var xor = new logical_two_params (function (a, b) {return a ^ b;});
  var shiftl = new logical_two_params (function (a, b) {return a << b;});
  var shiftr = new logical_two_params (function (a, b) {return a >> b;});
  var shiftrr = new logical_two_params (function (a, b) {return a >>> b;});
  var neg = new logical_one_param (function (a) {return ~ a;});
  this . getNativeCode = function (name) {
    switch (name) {
      case 'list': return list;
      case 'pp': return pp;
      case 'sum': return sum;
      case 'add': return add;
      case 'mult': return mult;
      case 'e': return new zero_param (Math . E);
      case 'pi': return new zero_param (Math . PI);
      case 'abs': return new logical_one_param (function (a) {return a < 0 ? - a : a;});
      case 'trunc': return new logical_one_param (function (a) {return Math . trunc (a);});
      case 'floor': return new logical_one_param (function (a) {return Math . floor (a);});
      case 'ceil': return new logical_one_param (function (a) {return Math . ceil (a);});
      case 'round': return new logical_one_param (function (a) {return Math . round (a);});
      case 'add1': return add1;
      case 'sub1': return sub1;
      case 'and': return and;
      case 'or': return or;
      case 'xor': return xor;
      case '<<': return shiftl;
      case '>>': return shiftr;
      case '>>>': return shiftrr;
      case 'neg': return neg;
      case 'degrad': return new one_param (function (a) {return a * Math . PI / 180;}, function (a) {return a * 180 / Math . PI;});
      case 'sin': return new one_param (function (a) {return Math . sin (a);}, function (a) {return Math . asin (a);});
      case 'cos': return new one_param (function (a) {return Math . cos (a);}, function (a) {return Math . acos (a);});
      case 'tan': return new one_param (function (a) {return Math . tan (a);}, function (a) {return Math . atan (a);});
      case 'cotan': return new one_param (function (a) {return 1 / Math . tan (a);}, function (a) {return Math . PI / 2 - Math . atan (a);});
      case 'asin': return new one_param (function (a) {return Math . asin (a);}, function (a) {return Math . sin (a);});
      case 'acos': return new one_param (function (a) {return Math . acos (a);}, function (a) {return Math . cos (a);});
      case 'atan': return new one_param (function (a) {return Math . atan (a);}, function (a) {return Math . tan (a);});
      case 'acotan': return new one_param (function (a) {return Math . PI / 2 - Math . atan (a);}, function (a) {return 1 / Math . tan (a);});
      case 'atan2': return new logical_two_params (function (a, b) {return Math . atan2 (a, b);});
      default: break;
    }
    return null;
  };
}
);

studio . setResource (['studio.prc'],`
program studio #machine := ' prolog . studio '
	[
    list
    pp
    not
    e pi
    abs trunc floor ceil round
    add1 ++ sub1 --
    and & or | xor ^ neg ~ << >> >>>
    sum times add + mult
    degrad sin cos tan cotan asin acos atan acotan atan2
	]

#machine list := 'list'
#machine pp := 'pp'

#machine e := 'e'
#machine pi := 'pi'
#machine abs := 'abs'
#machine trunc := 'trunc'
#machine floor := 'floor'
#machine ceil := 'ceil'
#machine round := 'round'
#machine add1 := 'add1'
#machine ++ := 'add1'
#machine sub1 := 'sub1'
#machine -- := 'sub1'
#machine and := 'and'
#machine & := 'and'
#machine or := 'or'
#machine | := 'or'
#machine xor := 'xor'
#machine ^ := 'xor'
#machine << := '<<'
#machine >> := '>>'
#machine >>> := '>>>'
#machine neg := 'neg'
#machine ~ := 'neg'

#machine sum := 'sum'
#machine + := 'add'
#machine add := 'add'
#machine mult := 'mult'

#machine degrad := 'degrad'
#machine sin := 'sin'
#machine cos := 'cos'
#machine tan := 'tan'
#machine cotan := 'cotan'
#machine asin := 'asin'
#machine acos := 'acos'
#machine atan := 'atan'
#machine acotan := 'acotan'
#machine atan2 := 'atan2'

[[not : *x] *x / fail]
[[not : *]]

end .
`);
