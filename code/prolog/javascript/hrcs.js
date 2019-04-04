
var hrcs = require ('./index');

var studio = hrcs . studio;
var prolog = hrcs . prolog;

require ('./test.prc');
require ('./test.prb');
require ('./test_studio.prb');

console . log (studio . readResource (['studio.prc']));
console . log (studio . readResource (['test.prc']));
console . log (studio);
console . log (prolog);

var root = new prolog . Root ();

root . load ('studio');
root . load ('test');

console . log (root . list ());

console . log (root);
//console . log (root . textResolution ("[[] [import 'capitol.prc']]"));
console . log (root . textResolution ("[[*fr *command] [batch 'test.prb']]"));

console . log (root . list ());

