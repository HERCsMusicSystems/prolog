
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

var rooot = new prolog . Root ();

rooot . load ('studio');
rooot . load ('test');

console . log (rooot . list ());

console . log (rooot);
//console . log (root . textResolution ("[[] [import 'capitol.prc']]"));
console . log (rooot . textResolution ("[[*fr *command] [batch 'test.prb']]"));

console . log (rooot . textResolution ("[[] [import 'capitol']]"));

//console . log (rooot . textResolution ("[*tc [cd 'voyager'] [file_writer *tc 'sonda.txt'] [*tc 'sonda'] [*tc]]"));

console . log (rooot . list ());

console . log (rooot . textResolution ("[*x [search_directories 'sonda' 'mariner/voyager'] [search_directories : *x]]"));

console . log (studio . search ());

