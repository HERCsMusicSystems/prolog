
studio . setResource (['prolog', 'fxg'],
function (root, directory) {
  var viewport = function (atom) {
    var bar = document . createElement ('div'); bar . style . background = 'yellow'; bar . appendChild (document . createTextNode ('sonda'));
    var close = document . createElement ('input'); close . type = 'button'; close . value = String . fromCharCode (0xd7); close . style . float = 'right';
    bar . appendChild (close);
    var content = document . createElement ('canvas'); content . id = 'cx';
    var info = document . createElement ('div'); info . style . background = 'yellow'; info . appendChild (document . createTextNode ('info'));
    var resize = document . createElement ('input'); resize . type = 'button'; resize . value = String . fromCharCode (0x21f2); resize . style . float = 'right';
    info . appendChild (resize);
    var div = document . createElement ('div');
    var divp = {x: 200, y: 100};
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
