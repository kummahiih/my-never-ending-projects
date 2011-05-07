//#include <limists.h>
#include <map>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "sequitur_simple_mod.h"

//extern symbols **find_digram(symbols *s);
//rules S;
slong num_rules = 0;


void debugprint( int verbose, const char * str)
{
  if( verbose != 0 )          
    printf("\nD: %s\n", str);
}


rules::rules() 
{
  num_rules ++;
  guard = new symbols(this);
  guard->point_to_self();
  count = number = 0;
}

rules::~rules() { 
  
  if( this->guard  != 0 )
  {
    num_rules --;
    delete guard;
    this->guard = 0;
  }
}

symbols *rules::first() { if(guard != 0)return guard->next();else return 0; }
symbols *rules::last() {  if(guard != 0)return guard->prev();else return 0; }


filerule::filerule(): rules()
{
  //printf("plaa");
    this->reuse(); //a ugly fix for preventing the use of expand() 
    this->reuse(); 
    this->ptInsertionPoint  = new insertpoint();
    this->last()->insert_after( this->ptInsertionPoint );
    
}

int filerule::insert( slong sym, int verbose )
{
    symbols * ptChecked; // = this->ptInsertionPoint->prev()->prev();
    
    //this need to be done because of the other insertions and their affects
    /*if( !ptChecked->badForDigram() )
    {
        ptChecked->check(verbose);
    }*/
     //TODO !!!!!!!!!!!!!!!!!!!!! -?????????

    if( verbose != 0 ) printf("D: inserting\n");
    
    symbols * ptNewSymbol = new symbols( sym );
    int debug = 0;
    if ( ptNewSymbol == 0 )
    {
        fprintf(stderr, "ERROR: allocation error\n");
        return 1;
    }
    //if( sym == 0 && ptChecked->char_value() == 0)
    //    debug = 1;

    if( verbose != 0 ) printf("D: inserting..\n");

    ptChecked = this->ptInsertionPoint->prev(); 
    ptChecked->insert_after( ptNewSymbol );
    
    /*if( !ptChecked->badForDigram() ) */ptChecked->check(verbose);
    return 0;
}

void filerule::endInserting()
{
    if( this->ptInsertionPoint != 0 )
        delete this->ptInsertionPoint;
    this->ptInsertionPoint = 0;
}



symbols::symbols( slong sym ) {
    s = SYMBOL_VAL( sym );
    // an odd number, so that they're a distinct
    // space from the rule pointers, which are 4-byte aligned
    p = n = 0;
  }

symbols::symbols( rules *r ) {
    s = (slong) r;
    assert( s%2 == 0 );
    p = n = 0;
    rule()->reuse();
  }

  symbols::~symbols()
  {
    join(p, n);
    if (!is_guard() ) {
      delete_digram();
      if (nt()) rule()->deuse(); 
    }
  }

void symbols::insert_after(symbols *y)
{
    join(y, n);
    join(this, y);
};

void symbols::delete_digram() 
{
    if (badForDigram() || n->badForDigram() ) return;
    //debugprint(1, "delete digram of this node");
    deleteDigram( this->raw_value(), this->next()->raw_value(), this );
    /*symbols **m = find_digram(this);
    if (*m == this)
    {
      *m = (symbols *) 1;
      nDigrams--;
    }
    */
}

slong symbols::check(int verbose ) 
{
    if ( badForDigram() || n->badForDigram() ) return 0;
    //symbols **x = find_digram(this);
    slong one = this->raw_value();
    slong two = this->next()->raw_value();

    symbols * found = getDigram( one, two, verbose );

    if( found != 0 && found->next() == this )
    {
      debugprint(verbose, "move digram: 3 same symbols ");
      setDigram(one, two, this ); // join relies on this and vice versa
    }
    if( one == two )
    {
      symbols * prev_s = this->prev();
      if( prev_s != 0 )
      {
      symbols * prev_prev_s = prev_s->prev();
      if( prev_s != 0 &&  prev_prev_s != 0 && one == prev_s->raw_value() && prev_s->raw_value() ==  prev_prev_s->raw_value() )
      {
	debugprint(verbose, "new rule: 4 same symbols each after another");
        match(prev_prev_s, this, verbose );
        return 1;
      }
      }
    }
    
    
    if ( found ==  0 )
    {
      
      debugprint(verbose, "new digram" );
      setDigram(one, two, this, verbose );
      return 0;
    }
    
    if( found->next() != this )
    {
      match(this, found, verbose ); 
      return 1;
    }
    return 0; // only moved digram...
}


// This symbol is the last reference to its rule. It is deleted, and the
// contents of the rule substituted in its place.

void symbols::expand(int verbose) {
    if( verbose != 0)
        printf("\nexpand a rule  \n");//, this->prev()->rule()->index() + RULE_TABLE_START);

  symbols *left = prev();
  symbols *right = next();
  symbols *f = rule()->first();
  symbols *l = rule()->last();

  delete rule();
  //  symbols **m = find_digram(this);
  //if (*m == this) *m = (symbols *) 1;
  deleteDigram( this->raw_value(), this->next()->raw_value(), this );
  s = 0; // if we don't do this, deleting the symbol tries to deuse the rule!
  delete this;
  
  join(left, f);
  join(l, right);
  
  l->check(verbose);//TEST
  //updateDigram( l->raw_value(), l->next()->raw_value(), l );

  //*find_digram(l) = l;
}

// Replace a digram with a non-terminal

void symbols::substitute(rules *r, int verbose)
{
  symbols *q = p;
  assert(p->next() == this );
  debugprint( verbose, "substitute: delete1"); 
  delete q->next();
  debugprint( verbose, "substitute: delete2");
  delete q->next();

  debugprint( verbose, "substitute: insert_after"); 
  q->insert_after(new symbols(r)); //reuses the rule..
  
  if (!q->check()) q->n->check();
}

// Deal with a matching digram  .. 4 samaa: ss=edellinen, m=uusin

void symbols::match(symbols *ss, symbols *m, int verbose ) 
{
  rules *r;

  // reuse an existing rule

  if (m->prev()->is_guard() && m->next()->next()->is_guard()) {
    r = m->prev()->rule();
    if( verbose != 0)
      printf("using a ready rule\n"); 
    ss->substitute(r); 
  }
  else {
    // create a new rule
    if( verbose != 0)
      printf("new rule "); 
    r = new rules;    

    if (ss->nt())
      r->last()->insert_after(new symbols(ss->rule()));
    else
      r->last()->insert_after(new symbols(ss->value()));
    
    if (ss->next()->nt())
      r->last()->insert_after(new symbols(ss->next()->rule()));
    else
      r->last()->insert_after(new symbols(ss->next()->value()));

    
    debugprint( verbose, "substitute 1"); 
    m->substitute(r, verbose);  
    //aaR  R->aa
    debugprint( verbose, "substitute 2");
    ss->substitute(r, verbose );

    setDigram( r->first()->raw_value(), r->first()->next()->raw_value(), r->first(), verbose );
    assert( getDigram( r->first()->raw_value(), r->first()->next()->raw_value() ) ==  r->first() );

    //*find_digram(r->first()) = r->first();
  }

  // check for an underused rule

  if (r->first()->nt() && r->first()->rule()->freq() == 1) r->first()->expand( verbose );
}



// print the rules out

rules **R;
slong Ri = 0;

void p(rules *r, FILE * outFile ) {
  for (symbols *p = r->first(); !p->is_guard(); p = p->next())
    if (p->nt()) { // jos p on s‰‰ntï
      slong i;

      if (R[p->rule()->index()] == p->rule()) //jos on jo taulussa
        i = p->rule()->index();               //i on index
      else {       //muuten laitetaan tauluun ja annetaan uusi indeksi
        i = Ri;
        p->rule()->index(Ri);
        R[Ri ] = p->rule();
        Ri++;
      }
      fprintf(outFile," %ld", i + RULE_TABLE_START );
      //cout << i << ' ';
    }
    else {
      /*if (p->value() == ' ') cout << '_';
      else if (p->value() == '\n') cout << "\\n";
      else if (p->value() == '\t') cout << "\\t";
      else if (p->value() == '\\' ||
               p->value() == '(' ||
               p->value() == ')' ||
               p->value() == '_' ||
               isdigit(p->value()))
        cout << '\\' << char(p->value());
      else cout << char(p->value());
      cout << ' ';*/
      fprintf(outFile," %ld", p->char_value()  );
    }

}

slong print(FILE * outFile, rules ** baseruleTable, slong baseruleTableSize )
{
  slong i;
  R = (rules **) malloc(sizeof(rules *) * num_rules);
  memset(R, 0, sizeof(rules *) * num_rules);
// 
  fprintf( outFile, "cfg\n");
  fprintf( outFile, "%d\n",LAST_CHAR+1);
  fprintf( outFile, "%ld\n",num_rules + 1 ); //start rule is defined manually
  fprintf( outFile, "%d\n",FIRST_RULE);
  
  Ri = 0;
  slong RiOld = 0;
  for( i = 0; i < baseruleTableSize; i++ )
  {
    //if( baseruleTable[i]->first() == baseruleTable[i]->last() )  continue;
  	slong gotIndex = baseruleTable[i]->index();
	
  	if ( gotIndex <= 0 || gotIndex >= Ri || R[ gotIndex ] != baseruleTable[ i ] ) 
	//jos ei oo taulussa ( eli ole siell‰ oman indeksins‰ kohdalla), pit‰‰ laittaa tauluun ja sitten tulostaa p( .. )
	{
        	RiOld = Ri; 
        	baseruleTable[ i ]->index( Ri );
        	R[Ri ] = baseruleTable[ i ];
            Ri++;
		
		for (slong j = RiOld; j < Ri; j ++) {
			//cout << i << " -> ";
			//printf("%ld\n",j);
			fprintf(outFile,"(%ld", j+RULE_TABLE_START );
			p(R[j], outFile); //tulostaminen alustaa taulukoa eteenp‰in ja p‰ivitt‰‰ Ri:n arvoa
			fprintf(outFile,")\n" );
  		}
	}
  }
  //now the indexes are updated and the printing of the start rule can be done
  for(  i = 0; i < baseruleTableSize; i++ )
  {
    //printf( "baserule%ld: (%d %ld)\n",i,FIRST_RULE, baseruleTable[i]->index() + RULE_TABLE_START );
    /*if( baseruleTable[i]->first() == baseruleTable[i]->last() && baseruleTable[i]->first()->rule()->index() != -1  )
    {
      fprintf(outFile,"(%d %ld)\n",FIRST_RULE, baseruleTable[i]->first()->rule()->index() + RULE_TABLE_START );
      baseruleTable[i]->first()->rule()->index( -1 );
    }
    else  TODO: how to deal eith rules which right side length is one*/
      fprintf(outFile,"(%d %ld)\n",FIRST_RULE, baseruleTable[i]->index() + RULE_TABLE_START );
  }
  
  free(R);
  
  return 1;
}


