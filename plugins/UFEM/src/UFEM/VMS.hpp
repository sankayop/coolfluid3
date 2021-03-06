/// Copyright (C) 2010-2011 von Karman Institute for Fluid Dynamics, Belgium
///
/// This software is distributed under the terms of the
/// GNU Lesser General Public License version 3 (LGPLv3).
/// See doc/lgpl.txt and doc/gpl.txt for the license text.

/// derived from ScalarAdvection.hpp

/// Source: Variational Multiscale (VMS) Residual-based turbulence modeling 
/// for LES of incompressible flows (Y. Bazilevs, Elsevier, (2007) 173-201)

#ifndef cf3_UFEM_VMS_hpp
#define cf3_UFEM_VMS_hpp

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/max.hpp>

#define BOOST_PROTO_MAX_ARITY 10
#ifdef BOOST_MPL_LIMIT_METAFUNCTION_ARITY
  #undef BOOST_MPL_LIMIT_METAFUNCTION_ARITY
#endif
#define BOOST_MPL_LIMIT_METAFUNCTION_ARITY 10

#include <boost/scoped_ptr.hpp>
#include "ns_semi_implicit/LSSVectorOps.hpp"
#include "LibUFEM.hpp"
#include "LSSActionUnsteady.hpp"
#include "SUPG.hpp"

namespace cf3 {

namespace UFEM {

using solver::actions::Proto::SFOp;
using solver::actions::Proto::CustomSFOp;  

/// solver for scalar transport
class UFEM_API VMS : public LSSActionUnsteady
{
public: // functions

  /// Constructor
  /// @param name of the component
  VMS ( const std::string& name );
  
  /// Modification of the NS execute function
  void execute() override;
  
  /// Get the class name
  static std::string type_name () { return "VMS"; }

private:

  /// Create the solver
  // void trigger_assembly();

  /// 
  void set_expression();

  /// Called when the initial condition manager is changed
  virtual void on_initial_conditions_set(InitialConditions& initial_conditions) override;

  /// Corrector number of iterations 
  int m_nb_iterations;

  /// AlphaM, AlphaF, Gamma coefficients for the alpha-method
  Real m_alphaM, m_alphaF, m_gamma;

  /// Storage of the stabilization coefficients
  Real m_tau_c, m_tau_m, m_c1;
  bool m_dynTau;

  /// Variables
  FieldVariable<2, VectorField> m_uDot;

  /// Solution fields at time n+1
  FieldVariable<7, VectorField> m_u1;

  /// Solution fields at time n+alpha{F,M}
  FieldVariable<8, VectorField> m_uaF;
  FieldVariable<9, VectorField> m_uaMDot;
  FieldVariable<10, ScalarField> m_f;
  /// Data members are public, because these are initialized where appropriate
  // Handle<math::LSS::System> u_lss;
  // Handle<math::LSS::System> p_lss;
  
  // Handle<math::LSS::SolveLSS> solve_p_lss;
  // Handle<math::LSS::SolveLSS> solve_u_lss;
  
  // Handle<common::Action> pressure_bc;
  // Handle<common::Action> velocity_bc;
  // Handle<common::Action> inner_bc;
  // Handle<common::Action> m_velocity_assembly;
  
  // Handle<solver::Time> m_time;

  Handle<solver::actions::Proto::ProtoAction> m_predictor;
  Handle<solver::actions::Proto::ProtoAction> m_correctorInitialiser;
  Handle<common::Action> m_initial_conditions;
  // Handle<common::Action> m_zeroLSS;
  Handle<solver::actions::Proto::ProtoAction> m_assembly;
  Handle<BoundaryConditions> m_bc;
  // Handle<common::Action> solveLSS;
  Handle<math::LSS::SolveLSS> m_solveLSS;
  Handle<solver::actions::Proto::ProtoAction> m_update;
  Handle<solver::actions::Proto::ProtoAction> m_renew;
  
/*
  Handle< math::LSS::Vector > u;
  Handle< math::LSS::Vector > a;
  Handle< math::LSS::Vector > p;
  Handle< math::LSS::Vector > delta_p_sum;
  Handle< math::LSS::Vector > lumped_m_diag;

  Teuchos::RCP<const Thyra::LinearOpBase<Real> > lumped_m_op;
  Teuchos::RCP<Thyra::VectorBase<Real> > delta_a;
  Teuchos::RCP<Thyra::VectorBase<Real> > aup_delta_p; // This is actually u_lss->rhs()
  
  /// These are used when alternating the solution strategies between predictor and corrector steps
  Handle<math::LSS::SolutionStrategy> m_p_strategy_first;
  Handle<math::LSS::SolutionStrategy> m_p_strategy_second;

  Handle<solver::ActionDirector> m_u_rhs_assembly;
  Handle<solver::ActionDirector> m_p_rhs_assembly;
  Handle<solver::ActionDirector> m_apply_aup; */
};


} // UFEM
} // cf3


#endif // cf3_UFEM_VMS_hpp