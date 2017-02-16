/** @file propagator.h
  @brief The base Propagator class definition.

  Propagators are used to compute the axonal propagation of instantaneous
  firing rate.

  @author Peter Drysdale, Felix Fung,
*/

#ifndef NEUROFIELD_SRC_PROPAGATOR_H
#define NEUROFIELD_SRC_PROPAGATOR_H

// C++ standard library headers
#include <string> //std::string;
#include <vector> //std::vector;

// Neurofield headers
#include "configf.h"    // Configf;
#include "output.h"     // Output; Outlet;
#include "nf.h"         // NF;
#include "tau.h"        // Tau;
#include "population.h" // Population;

class Propagator : public NF {
  Propagator(); // no default constructor
  Propagator(Propagator&); // no copy constructor
 protected:
  void init( Configf& configf ) override;
  //void restart( Restartf& restartf );
  //void dump( Dumpf& dumpf ) const;

  Population& prepop;
  Population& postpop;
  Tau tau; ///< tau_ab
  int longside;
  std::vector<double> p; ///< phi_ab
 public:
  Propagator( size_type nodes, double deltat, size_type index, Population& prepop,
              Population& postpop, int longside, std::string topology );
  ~Propagator(void) override;
  void step(void) override;
  double phiinit( Configf& configf ) const;
  virtual const std::vector<double>& phi(void) const;
  inline double operator[]( size_type node ) const;
  void output( Output& output ) const override;
};

double Propagator::operator[]( size_type node ) const {
  return p[node];
}

#endif //NEUROFIELD_SRC_PROPAGATOR_H
