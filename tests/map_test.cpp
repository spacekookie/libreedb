// map::find
#include <iostream>
#include <map>

typedef struct foo {
  char  *content;
  int   created;
} foo;

static std::map<struct foo*, int> *mymap;

void add_to_map(struct foo *val, int bar) {
  (*mymap)[val] = bar;
}

int main ()
{
  /* Initialise our map and an iterator to work with it */
  mymap = new std::map<struct foo*, int>();
  std::map<struct foo*, int>::iterator it;

  /* Initialise struct A */
  struct foo *a = new foo();
  a->content = new char[32];
  a->created = 111111;

  /* Initialise struct B */
  struct foo *b = new foo();
  b->content = new char[32];
  b->created = 222222;

  /* Insert and asign some values */
  add_to_map(a, 50);
  add_to_map(b, 500);

  /* Print the contents of our map */
  std::cout << "elements in mymap:" << '\n';
  std::cout << "a => " << mymap->find(a)->second << '\n';
  std::cout << "b => " << mymap->find(b)->second << '\n';

  /* Cleanup */
  delete(mymap);
  return 0;
}