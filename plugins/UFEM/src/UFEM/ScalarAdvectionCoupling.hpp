//#ifndef SCALARADVECTION_COUPLING_H
//#define SCALARADVECTION_COUPLING_H

//#endif // SCALARADVECTION_H


// Copyright (C) 2010-2011 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef cf3_UFEM_ScalarAdvection_hpp
#define cf3_UFEM_ScalarAdvection_hpp

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/max.hpp>

#define BOOST_PROTO_MAX_ARITY 10
#ifdef BOOST_MPL_LIMIT_METAFUNCTION_ARITY
  #undef BOOST_MPL_LIMIT_METAFUNCTION_ARITY
#endif
#define BOOST_MPL_LIMIT_METAFUNCTION_ARITY 10

#include <boost/scoped_ptr.hpp>

#include "LibUFEM.hpp"
#include "LSSActionUnsteady.hpp"

#include "SUPG.hpp"

namespace cf3 {

namespace UFEM {

/// solver for scalar transport
class UFEM_API ScalarAdvectionCoupling : public LSSActionUnsteady
{
public: // functions

  /// Contructor
  /// @param name of the component
  ScalarAdvectionCoupling( const std::string& name );

  /// Get the class name
  static std::string type_name () { return "ScalarAdvectionCoupling"; }

private:

  /// Create the solver
  void trigger_assembly();

  /// Called when the internal name to use for the scalar variable is changed
  void trigger_scalar_name();

  virtual void on_initial_conditions_set ( InitialConditions& initial_conditions );

  PhysicsConstant m_alpha;
  PhysicsConstant lambda_f;
  PhysicsConstant cp;
  PhysicsConstant rho;

  /// Stabilization coefficient
  Real tau_su;

  ComputeTau compute_tau;

  Handle<solver::ActionDirector> m_assembly;
  Handle<solver::ActionDirector> m_update;
  Handle<common::Action> m_initial_conditions;

};

} // UFEM
} // cf3


#endif // cf3_UFEM_ScalarAdvection_COUPLING_hpp

