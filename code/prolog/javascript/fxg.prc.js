
studio . setResource (['prolog', 'fxg'],
function (root, directory) {
  var Location = directory . searchAtom ('Location');
  var Position = directory . searchAtom ('Position');
  var Size = directory . searchAtom ('Size');
  var Scaling = directory . searchAtom ('Scaling');
  var Mode = directory . searchAtom ('Mode');
  var viewport = function (atom, name, x, y, width, height) {
    var bar = document . createElement ('div'); bar . style . background = 'yellow'; bar . appendChild (document . createTextNode (name === null ? 'VIEWPORT' : name)); bar . style ['font-family'] = 'arial';
    var close = document . createElement ('input'); close . type = 'button'; close . value = String . fromCharCode (0xd7); close . style . float = 'right';
    bar . appendChild (close);
    var content = document . createElement ('canvas'); content . id = 'cx';
    if (width !== null) content . width = width;
    if (height !== null) content . height = height;
    var info = document . createElement ('div'); info . style . background = 'yellow'; info . appendChild (document . createTextNode ('info')); info . style ['font-family'] = 'arial';
    var resize = document . createElement ('input'); resize . type = 'button'; resize . value = String . fromCharCode (0x21f2); resize . style . float = 'right';
    info . appendChild (resize);
    var div = document . createElement ('div');
    var divp = {x: x === null ? 0 : x, y: y === null ? 0 : y};
    var position = {x: 0, y: 0, scaling: 1};
    var mode = 'navigate';
    div . appendChild (bar);
    div . appendChild (content);
    div . appendChild (info);
    div . style . background = 'blue'; div . style . position = 'absolute'; div . style . top = divp . y; div . style . left = divp . x;
    var mouseup = function (e) {document . onmouseup = null; document . onmousemove = null;};
    var mousemove = function (e) {divp . x += e . movementX; divp . y += e . movementY; div . style . top = divp . y; div . style . left = divp . x;};
    var mousesize = function (e) {content . width += e . movementX; content . height += e . movementY;};
    bar . onmousedown = function (e) {document . onmouseup = mouseup; document . onmousemove = mousemove;};
    resize . onmousedown = function (e) {document . onmouseup = mouseup; document . onmousemove = mousesize;};
    document . body . appendChild (div);
    close . onmousedown = function (e) {div . parentElement . removeChild (div); atom . setMachine (null);};
    this . code = function (el) {
      if (el . type === 0) {div . parentElement . removeChild (div); return atom . setMachine (null);}
      if (el . type !== 1) return false;
      var selector = el . left; el = el . right;
      if (selector . type === 3) {
        switch (selector . left) {
          case Location:
            if (el . type === 2) {
              el = el . setNativePair (divp . x);
              el = el . setNativePair (divp . y);
              el = el . setNativePair (content . width);
              el . setNativePair (content . height);
              return true;
            }
            if (el . type !== 1 || el . left . type !== 6) return false; divp . x = el . left . left; div . style . left = divp . x; el = el . right;
            if (el . type !== 1 || el . left . type !== 6) return false; divp . y = el . left . left; div . style . top = divp . y; el = el . right;
            if (el . type !== 1 || el . left . type !== 6) return true; content . width = el . left . left; el = el . right;
            if (el . type !== 1 || el . left . type !== 6) return false; content . height = el . left . left;
            return true;
          case Position:
            if (el . type === 2) {el = el . setNativePair (position . x); el . setNativePair (position . y); return true;}
            if (el . type !== 1 || el . left . type !== 6) return false; position . x = el . left . left; el = el . right;
            if (el . type !== 1 || el . left . type !== 6) return false; position . y = el . left . left;
            return true;
          case Size:
            if (el . type === 2) {el = el . setNativePair (content . width); el . setNativePair (content . height); return true;}
            if (el . type !== 1 || el . left . type !== 6) return false; content . width = el . left . left; el = el . right;
            if (el . type !== 1 || el . left . type !== 6) return false; content . height = el . left . left;
            return true;
          case Scaling:
            if (el . type === 2) {el . setNativePair (position . scaling); return true;}
            if (el . type !== 1 || el . left . type !== 6) return false; position . scaling = el . left . left;
            return true;
          case Mode:
            if (el . type === 2) {el . setNativePair (mode); return true;}
            if (el . type !== 1 || el . left . type !== 6) return false; mode = el . left . left;
            return true;
        }
      }
      return false;
    };
  };
  var Viewport = {
    code: function (el) {
      var atom = null, name = null, x = null, y = null, width = null, height = null;
      while (el . type === 1) {
        var e = el . left;
        if (e . type === 2 || e . type === 3) atom = e;
        if (e . type === 6) {
          if (typeof (e . left) === 'number') {
            if (x === null) x = e . left;
            else if (y === null) y = e . left;
            else if (width === null) width = e . left;
            else height = e . left;
          } else name = e . left;
        }
        el = el . right;
      }
      if (atom === null) return false;
      if (atom . type === 2) atom . setAtom (new prolog . Atom ());
      if (atom . left . machine !== null) return false;
      return atom . left . setMachine (new viewport (atom . left, name, x, y, width, height));
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
  [
    Viewport
    Location Position Size Scaling Mode
  ]

#machine Viewport := 'Viewport'

end .
`);
