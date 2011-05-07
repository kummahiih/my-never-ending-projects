#include <iostream>
#include <assert.h>
#include "digramstore.h"

using namespace std;



// an odd number, so that they're a distinct
// space from the rule pointers, which are 4-byte aligned
#define SYMBOL_VAL(sym) ( 2 * ( sym + 1 ) + 1 )
#define CHAR_VAL(sym) ( ( sym - 1 )/2 - 1 )


// This finds a matching digram in the grammar, and returns a pointer 
// to its entry in the hash table so it can be changed or deleted as necessary

class rules {
  // the guard node in the linked list of symbols that make up the rule
  // It points forward to the first symbol in the rule, and backwards
  // to the last symbol in the rule. Its own value points to the rule data 
  // structure, so that symbols can find out which rule they're in
  
  symbols *guard;     
  
  //  count keeps track of the number of times the rule is used in the grammar
  slong count;

  // this is just for numbering the rules nicely for prining; it's
  // not essential for the algorithm
  slong number;

public:
  rules();
  virtual ~rules();

  
  
  void reuse() { count ++; }
  void deuse() { count --; };

  symbols *first();
  symbols *last();
  
  slong freq() { return count; };
  slong index() { return number; };
  void index(slong i) { number = i; };
};

class filerule: public rules {
    private:
    insertpoint * ptInsertionPoint;

    public:
    filerule();
    int insert( slong sym, int verbose =0 );
    void endInserting();
    virtual ~filerule(){ this->endInserting(); };
};






class symbols {
  symbols *n, *p;
protected:
  slong s;

 public:

  // initializes a new symbol. If it is non-terminal, increments the reference
  // count of the corresponding rule

  symbols(slong sym);
  symbols(rules *r);

  // links two symbols together, removing any old digram from the hash table
  static void join(symbols *left, symbols *right) {
    if (left->n) {
      left->delete_digram();  //deletes the digram only if it is crated from left. 
      
      // This is to deal with triples, where we only record the second
      // pair of the overlapping digrams. When we delete the second pair,
      // we insert the first pair into the hash table so that we don't
      // forget about it.  e.g. abbbabcbb

      if (right->p && right->n &&
          right->value() == right->p->value() &&
          right->value() == right->n->value()) {

        setDigram( right->raw_value(), right->n->raw_value(), right );
        //*find_digram(right) = right;
      }
      
      if (left->p && left->n &&
          left->value() == left->n->value() &&
          left->value() == left->p->value()) {
        //*find_digram(left->p) = left->p;
        setDigram( left->p->raw_value(), left->raw_value(), left->p );
      }
    }
    left->n = right; right->p = left;
  }

  // cleans up for symbol deletion: removes hash table entry and decrements
  // rule reference count

  virtual ~symbols();

  // inserts a symbol after this one.

  void insert_after(symbols *y);

  // removes the digram from the hash table

  void delete_digram();
  
  // returns true if this is the guard node marking the beginning/end of a rule

  virtual bool is_guard() { return nt() && rule()->first()->prev() == this; };

  virtual bool badForDigram() { return this->is_guard(); };

  // nt() returns true if a symbol is non-terminal. The number determines the
  // alphabet size. Anything below this is a terminal, above this is a
  // non-terminal. For ascii text, it should be 255, but for, say, word-based
  // sequences, it should be much higher.  It just has to be big enough; there
  // is no penalty for it being too large

  virtual bool nt() { return ((s % 2) == 0) && (s != 0);};

  symbols *next() { if( n!= 0)return n;else return 0;};
  symbols *prev() { if( p!= 0)return p;else return 0;};
  inline slong raw_value() {  return s; };
  inline slong value() { return CHAR_VAL(s);};
  inline slong char_value() { if( s != 0 ) return CHAR_VAL(s) ; else return -1;};
  
  // assuming this is a non-terminal, rule() returns the corresponding rule
  rules *rule() { return (rules *) s;};

  void substitute(rules *r, int verbose = 0);
  static void match(symbols *s, symbols *m, int verbose = 0);
  
  // checks a new digram. If it appears elsewhere, deals with it by calling
  // match(), otherwise inserts it into the hash table

  virtual slong check(int verbose = 0);
  void expand( int verbose = 0);
  void point_to_self() { join(this, this); };
};

class insertpoint: public symbols 
{    
public:
    insertpoint():symbols(INSERT_SYMBOL) {};
    virtual ~insertpoint(){};
    bool is_guard(){ return false; };
    bool badForDigram() { return true; };
};

slong print(FILE * outFile, rules ** baseruleTable, slong baseruleTableSize );
