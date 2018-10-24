
var str = {
  BackgroundColour: '#00000088',
  tokens: [],
  viewports: []
};

studio . setResource (['prolog', 'fxg'],
function (root, directory) {
  var structure = str;
  var Location = directory . searchAtom ('Location');
  var Position = directory . searchAtom ('Position');
  var Size = directory . searchAtom ('Size');
  var Scaling = directory . searchAtom ('Scaling');
  var Mode = directory . searchAtom ('Mode');
  var BackgroundColour = directory . searchAtom ('BackgroundColour');
  var viewport = function (atom, name, x, y, width, height) {
    var content = document . createElement ('canvas');
    if (width === null) width = content . width; else content . width = width;
    if (height === null) height = content . height; else content . height = height;
    if (x === null) x = 0; if (y === null) y = 0;
    if (name === null) name = 'VIEWPORT';
    var viewport = {atom: atom . name, location: {x: x, y: y}, position: {x: 0, y: 0}, size: {x: width, y: height}, scaling: 1};
    structure . viewports . push (viewport);
    var bar = document . createElement ('div'); bar . style . background = 'yellow'; bar . appendChild (document . createTextNode (name)); bar . style ['font-family'] = 'arial';
    var close = document . createElement ('input'); close . type = 'button'; close . value = String . fromCharCode (0xd7); close . style . float = 'right';
    bar . appendChild (close);
    var ctx = content . getContext ('2d');
    var info = document . createElement ('div'); info . style . background = 'yellow'; info . appendChild (document . createTextNode ('info')); info . style ['font-family'] = 'arial';
    var resize = document . createElement ('input'); resize . type = 'button'; resize . value = String . fromCharCode (0x21f2); resize . style . float = 'right';
    info . appendChild (resize);
    var div = document . createElement ('div');
    var position = {x: 0, y: 0, scaling: 1};
    var mode = 'navigate';
    div . appendChild (bar);
    div . appendChild (content);
    div . appendChild (info);
    div . style . position = 'absolute'; div . style . top = viewport . location . y; div . style . left = viewport . location . x;
    var mouseup = function (e) {document . onmouseup = null; document . onmousemove = null;};
    var mousemove = function (e) {
      viewport . location . x += e . movementX; viewport . location . y += e . movementY;
      div . style . top = viewport . location . y; div . style . left = viewport . location . x;
    };
    var mousesize = function (e) {
      viewport . size . x += e . movementX; viewport . size . y += e . movementY;
      content . width = viewport . size . x; content . height = viewport . size . y;
      content . repaint ();
    };
    bar . onmousedown = function (e) {document . onmouseup = mouseup; document . onmousemove = mousemove;};
    resize . onmousedown = function (e) {document . onmouseup = mouseup; document . onmousemove = mousesize;};
    document . body . appendChild (div);
    close . onmousedown = function (e) {div . parentElement . removeChild (div); atom . setMachine (null);};
    content . repaint = function () {
      ctx . fillStyle = viewport . BackgroundColour || structure . BackgroundColour;
      ctx . fillRect (0, 0, viewport . size . x, viewport . size . y);
      ctx . beginPath ();
      ctx . arc (100, 100, 40, 0, Math . PI);
      ctx . stroke ();
    };
    content . repaint ();
    this . code = function (el) {
      if (el . type === 0) {div . parentElement . removeChild (div); return atom . setMachine (null);}
      if (el . type !== 1) return false;
      var selector = el . left; el = el . right;
      if (selector . type === 3) {
        switch (selector . left) {
          case Location:
            if (el . type === 2) {
              el = el . setNativePair (viewport . location . x);
              el = el . setNativePair (viewport . location . y);
              el = el . setNativePair (viewport . size . x);
              el . setNativePair (viewport . size . y);
              return true;
            }
            if (el . type !== 1 || el . left . type !== 6) return false; viewport . location . x = el . left . left; div . style . left = viewport . location . x; el = el . right;
            if (el . type !== 1 || el . left . type !== 6) return false; viewport . location . y = el . left . left; div . style . top = viewport . location . y; el = el . right;
            if (el . type !== 1 || el . left . type !== 6) return true; viewport . size . x = el . left . left; content . width = viewport . size . x; el = el . right;
            if (el . type !== 1 || el . left . type !== 6) return false; viewport . size . y = el . left . left; content . height = viewport . size . y;
            return true;
          case Position:
            if (el . type === 2) {el = el . setNativePair (position . x); el . setNativePair (position . y); return true;}
            if (el . type !== 1 || el . left . type !== 6) return false; position . x = el . left . left; el = el . right;
            if (el . type !== 1 || el . left . type !== 6) return false; position . y = el . left . left;
            return true;
          case Size:
            if (el . type === 2) {el = el . setNativePair (content . width); el . setNativePair (content . height); return true;}
            if (el . type !== 1 || el . left . type !== 6) return false; viewport . size . x = el . left . left; content . width = viewport . size . x; el = el . right;
            if (el . type !== 1 || el . left . type !== 6) return false; viewport . size . y = el . left . left; content . height = viewport . size . y;
            return true;
          case Scaling:
            if (el . type === 2) {el . setNativePair (position . scaling); return true;}
            if (el . type !== 1 || el . left . type !== 6) return false; position . scaling = el . left . left;
            return true;
          case Mode:
            if (el . type === 1) el = el . left;
            if (el . type === 2) {el . setNative (mode); return true;}
            if (el . type === 6) {mode = el . left . left; return true;}
            return false;
          case BackgroundColour:
            if (el . type === 1) el = el . left;
            if (el . type === 2) {if (! viewport . BackgroundColour) return false; el . setNative (viewport . BackgroundColour); return true;}
            if (el . type === 6) {viewport . BackgroundColour = el . left; return true;}
            return false;
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
  var ColourFunction = function (colour_type) {
    this . code = function (el) {
      if (el . type === 2) {el . setNative (structure [`${colour_type}Colour`]); return true;}
      if (el . type !== 1) return false;
      var selector = el . left;
      if (selector . type === 2) {selector . setNative (structure [`${colour_type}Colour`]); return true;}
      if (selector . type === 6) {structure . BackgroundColour = selector . left; return true;}
      if (selector . type !== 3) return false;
      el = el . right; if (el . type === 1) el = el . left;
      if (el . type === 2) {el . setNative (structure [`${selector . left . name}${colour_type}Colour`]); return true;}
      if (el . type === 6) {structure [`${selector . left . name}${colour_type}Colour`] = el . left; return true;}
      return false;
    }
  };
  this . getNativeCode = function (name) {
    switch (name) {
      case 'Viewport': return Viewport;
      case 'BackgroundColour': return new ColourFunction ('Background');
      case 'ForegroundColour': return new ColourFunction ('Foreground');
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
    Rectangle Circle Picture Dice Grid Text Deck
    Location Position Size Scaling Mode
    BackgroundColour ForegroundColour
    Repaint
  ]

#machine Viewport := 'Viewport'
#machine BackgroundColour := 'BackgroundColour'
#machine ForegroundColour := 'ForegroundColour'

end .
`);
