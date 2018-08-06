
studio . setResource (['prolog', 'studio'],
function (root, directory) {
  var pp = function () {
    this . code = function (parameters) {
      console . log (root . getValue (parameters));
      return true;
    };
  }
  this . getNativeCode = function (name) {
    if (name === 'pp') return new pp (root);
    return null;
  };
}
);

studio . setResource (['studio.prc'],`
program studio #machine := ' prolog . studio '
	[
		sum pp
	]

#machine pp := 'pp'

end .
`);
