#include<iostream>
#include<vector>
using std::vector;
#include"stencil.h"

/*void Stencil::update(void)
{
  if(m) {
    nw = (*m)[_nw]; n  = (*m)[_n]; ne = (*m)[_ne];
    w  = (*m)[_w]; c  = (*m)[_c]; e  = (*m)[_e];
    sw = (*m)[_sw]; s  = (*m)[_s]; se = (*m)[_se];
  }
}*/

Stencil::Stencil( int nodes, int longside )
    : nodes(nodes), longside(longside), m(0)
{
  set(0);
}

Stencil::~Stencil(void)
{
}

void Stencil::assign( vector<double>* field )
{
  m = field;
  set(0);
  //update();
}

void Stencil::operator++ (int i)
{
  _nw++; _n++; _ne++;
  _w++; _c++; _e++;
  _sw++; _s++; _se++;

  if( (get()%longside)==longside-1 ) {      // centre at edge
    _ne -= longside; _e  -= longside; _se -= longside;
  }
  else if( (get()%longside)==0 ) { // centre wraps around edge
    _nw += longside; _w  += longside; _sw += longside;
    _ne += longside; _e  += longside; _se += longside;
  }
  else if( (get()%longside)==1 ) { // whole Moore grid passes edge
    _nw -= longside; _w  -= longside; _sw -= longside;
  }

  if( _nw>=nodes ) _nw -= nodes;
  if( _n >=nodes ) _n  -= nodes;
  if( _ne>=nodes ) _ne -= nodes;
  if( _w >=nodes ) _w  -= nodes;
  if( _c >=nodes ) _c  -= nodes;
  if( _e >=nodes ) _e  -= nodes;
  if( _sw>=nodes ) _sw -= nodes;
  if( _s >=nodes ) _s  -= nodes;
  if( _se>=nodes ) _se -= nodes;

  //update();
}

void Stencil::set( int node )
{
  _c = node;
  _nw = _c-1-longside;
  _n  = _c  -longside;
  _ne = _c+1-longside;
  _w  = _c-1;
  _e  = _c+1;
  _sw = _c-1+longside;
  _s  = _c  +longside;
  _se = _c+1+longside;

  if( (get()%longside) == 0 ) {               // left edge
    _nw += longside;
    _w  += longside;
    _sw += longside;
  }
  else if( (get()%longside) == longside-1 ) { // right edge
    _ne += longside;
    _e  += longside;
    _se += longside;
  }
  else if( node < longside ) {                // top edge
    _nw += nodes;
    _n  += nodes;
    _ne += nodes;
  }
  else if( node >= nodes-longside ) {         // bottom edge
    _sw -= nodes;
    _s  -= nodes;
    _se -= nodes;
  }

  if(_nw<0) _nw+=nodes;
  if(_n <0) _n +=nodes;
  if(_ne<0) _ne+=nodes;

  //update();
}

int Stencil::get(void) const
{
  return _c;
}

double Stencil::nw(void) const { return (*m)[_nw]; }
double Stencil::n(void) const { return (*m)[_n]; }
double Stencil::ne(void) const { return (*m)[_ne]; }
double  Stencil::w(void) const { return (*m)[_w]; }
double Stencil::c(void) const { return (*m)[_c]; }
double  Stencil::e(void) const { return (*m)[_e]; }
double Stencil::sw(void) const { return (*m)[_sw]; }
double Stencil::s(void) const { return (*m)[_s]; }
double Stencil::se(void) const { return (*m)[_se]; }