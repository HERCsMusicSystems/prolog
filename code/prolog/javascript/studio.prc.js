
studio . setResource (['prolog', 'studio'],
function (root, directory) {
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
  var abs = new function () {
    this . code = function (el) {
      if (el . type !== 1) return false;
      var left = el . left, ret = NaN;
      switch (left . type) {
        case 6: ret = left . left; break;
        case 3: ret = left . left . name; break;
        default: break;
      }
      el = el . right; if (el . type === 1) el = el . left;
      el . setNative (ret < 0 ? - ret : ret); return true;
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
  this . getNativeCode = function (name) {
    switch (name) {
      case 'pp': return pp;
      case 'sum': return sum;
      case 'add': return add;
      case 'mult': return mult;
      case 'abs': return abs;
      case 'add1': return add1;
      case 'sub1': return sub1
      default: break;
    }
    return null;
  };
}
);

studio . setResource (['studio.prc'],`
program studio #machine := ' prolog . studio '
	[
    pp not
    abs add1 ++ sub1 --
    sum times add + mult
	]

#machine pp := 'pp'

#machine abs := 'abs'
#machine add1 := 'add1'
#machine ++ := 'add1'
#machine sub1 := 'sub1'
#machine -- := 'sub1'

#machine sum := 'sum'
#machine + := 'add'
#machine add := 'add'
#machine mult := 'mult'

[[not : *x] *x / fail]
[[not : *]]

end .
`);
