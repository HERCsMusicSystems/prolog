
studio . setResource (['prolog', 'fxg'],
function (root, directory) {
  var viewport = function (atom) {
    var div = document . createElement ('div');
    var divx = 0, divy = 0;
    div . innerHTML = 'VOYAGER';
    div . style . background = 'blue'; div . style . position = 'absolute'; div . style . top = 100; div . style . left = 200;
    div . onmousedown = function (e) {divx = e . layerX; divy = e . layerY;};
    div . onmouseup = function (e) {div . style . left = 0; div . style . top += e . layerY - divy; console . log (div . style);};
    document . body . appendChild (div);
    this . code = function (el) {
      if (el . type === 0) {div . parentElement . removeChild (div); return atom . setMachine (null);}
      return true;
    };
  };
  var Viewport = {
    code: function (el) {
      var atom = null;
      while (el . type === 1) {
        var e = el . left;
        if (e . type === 2 || e . type === 3) atom = e;
        el = el . right;
      }
      if (atom === null) return false;
      if (atom . type === 2) atom . setAtom (new prolog . Atom ());
      if (atom . left . machine !== null) return false;
      return atom . left . setMachine (new viewport (atom . left));
    }
  };
  this . getNativeCode = function (name) {
    switch (name) {
      case 'Viewport': return Viewport;
      default: break;
    }
    return null;
  };
}
);



studio . setResource (['fxg.prc'], `
program fxg #machine := 'prolog . fxg'
  [Viewport]

#machine Viewport := 'Viewport'

end .
`);
