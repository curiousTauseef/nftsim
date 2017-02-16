/** @file harmonic.cpp
  @brief A brief, one sentence description.

  A more detailed multiline description...

  @author Peter Drysdale, Felix Fung,
*/

// C++ standard library headers
#include <vector>   // std::vector;
using std::vector;
#include <string>    // std::string;
using std::string;
#include <cstdlib>   // std::atof;
using std::atof;
#include <cmath>     // std::exp;
using std::exp;

// Neurofield headers
#include "configf.h"    // Configf;
#include "harmonic.h"   // Harmonic;
#include "population.h" // Population;

void Harmonic::init( Configf& configf ) {
  double Q = prepop.Qinit(configf);
  string buffer("Steady");
  configf.optional("phi",buffer);
  if( buffer != "Steady" ) {
    Q = atof(buffer.c_str());
  }
  p.clear();
  p.resize(nodes,Q);
  oldp.resize(nodes,Q);
  oldQ.resize(nodes,Q);
  dpdt.resize(nodes,0.);
  configf.optional("Tau",tau);
  prepop.growHistory(tau);

  // Range is either used when velocity is specified,
  // or not used but read for compatibility with Wave propagator
  double range;
  configf.optional("Range",range);

  if( !configf.optional("gamma",gamma) ) {
    double velocity;
    configf.param("velocity",velocity);
    gamma = velocity/range;
  }
  twoongamma = 2./gamma;
  expgamma = exp(-gamma*deltat);
}

Harmonic::Harmonic( size_type nodes, double deltat, size_type index, Population& prepop,
                    Population& postpop, int longside, string topology )
  : Propagator(nodes,deltat,index,prepop,postpop,longside,topology) {
}

Harmonic::~Harmonic() = default;

void Harmonic::step() {
  // This implementation assumes gamma is constant
  // and Q(t) is linear for the timestep.
  // x(t)  =  C1 t e(pt) + C2 e(pt),
  const vector<double>& Q = prepop.Q(tau);
  for( size_type i=0; i<nodes; i++ ) {
    dQdt = ( Q[i] -oldQ[i] )/deltat;
    adjustedQ = oldQ[i] -twoongamma*dQdt -oldp[i];
    C1 = dpdt[i] -gamma*adjustedQ -dQdt;
    C1dtplusC2 = C1*deltat -adjustedQ;
    p[i] = C1dtplusC2*expgamma +Q[i] -twoongamma*dQdt;
    dpdt[i] = (C1-gamma*C1dtplusC2)*expgamma +dQdt;
    oldQ[i] = Q[i];
    oldp[i] = p[i];
  }
}
