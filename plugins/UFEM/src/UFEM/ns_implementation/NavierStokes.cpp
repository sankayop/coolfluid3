// Copyright (C) 2010-2011 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include "../NavierStokes.hpp"

#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/copy.hpp>

#include "common/Component.hpp"
#include "common/Builder.hpp"
#include "common/OptionT.hpp"
#include "common/OptionArray.hpp"
#include "common/PropertyList.hpp"

#include "math/LSS/SolveLSS.hpp"
#include "math/LSS/ZeroLSS.hpp"

#include "solver/actions/Proto/ProtoAction.hpp"
#include "solver/actions/Proto/Expression.hpp"
#include "solver/actions/Iterate.hpp"
#include "solver/CriterionTime.hpp"
#include "solver/actions/AdvanceTime.hpp"
#include "solver/Time.hpp"
#include "solver/Tags.hpp"

#include "../NavierStokesSpecializations.hpp"
#include "../Tags.hpp"

namespace cf3 {
namespace UFEM {

using namespace common;
using namespace solver;
using namespace solver::actions;
using namespace solver::actions::Proto;


ComponentBuilder < NavierStokes, LSSActionUnsteady, LibUFEM > NavierStokes_builder;

NavierStokes::NavierStokes(const std::string& name) :
  LSSActionUnsteady(name),
  u("Velocity", "navier_stokes_solution"),
  p("Pressure", "navier_stokes_solution"),
  density_ratio("density_ratio", "density_ratio"), // (rho - rho_ref) / rho
  u_adv("AdvectionVelocity", "linearized_velocity"),
  u1("AdvectionVelocity1", "linearized_velocity"),
  u2("AdvectionVelocity2", "linearized_velocity"),
  u3("AdvectionVelocity3", "linearized_velocity"),
  nu_eff("EffectiveViscosity", "navier_stokes_viscosity"),
  g("Force", "body_force"),
  rho("density"),
  nu("kinematic_viscosity"),
  skew(1.0)
{
  const std::vector<std::string> restart_field_tags = boost::assign::list_of("navier_stokes_solution")("linearized_velocity")("navier_stokes_viscosity");
  properties().add("restart_field_tags", restart_field_tags);

  options().add("use_specializations", false)
    .pretty_name("Use Specializations")
    .description("Activate the use of specialized high performance code")
    .attach_trigger(boost::bind(&NavierStokes::trigger_assembly, this));

  options().add("supg_type", compute_tau.data.op.supg_type_str)
    .pretty_name("SUPG Type")
    .description("Type of computation for the stabilization coefficients.")
    .link_to(&(compute_tau.data.op.supg_type_str))
    .attach_trigger(boost::bind(&ComputeTauImpl::trigger_supg_type, &compute_tau.data.op));

  options().add("u_ref", compute_tau.data.op.u_ref)
    .pretty_name("Reference velocity")
    .description("Reference velocity for the CF2 SUPG method")
    .link_to(&(compute_tau.data.op.u_ref));

  options().add("c1", compute_tau.data.op.c1)
    .pretty_name("c1")
    .description("Constant adjusting the time part of SUPG in the metric tensor formulation")
    .link_to(&(compute_tau.data.op.c1));
    
  options().add("c2", compute_tau.data.op.c2)
    .pretty_name("c2")
    .description("Constant adjusting the time part of SUPG in the metric tensor formulation")
    .link_to(&(compute_tau.data.op.c2));

  options().add("theta", 0.5)
    .pretty_name("Theta")
    .description("Theta coefficient for the theta-method.")
    .attach_trigger(boost::bind(&NavierStokes::trigger_assembly, this));

  options().add("skew", skew)
    .pretty_name("Skew symmetric")
    .description("Control the skew symmetric term (0 = no skew symmetry, 1 = skew symmetry)")
    .link_to(&skew);

  options().add("alpha_ps", compute_tau.data.op.alpha_ps)
    .pretty_name("alpha_ps")
    .description("Constant to multiply the PSPG parameter with.")
    .link_to(&(compute_tau.data.op.alpha_ps));
      
  options().add("alpha_su", compute_tau.data.op.alpha_su)
    .pretty_name("alpha_su")
    .description("Constant to multiply the SUPG parameter with.")
    .link_to(&(compute_tau.data.op.alpha_su));
      
  options().add("alpha_bu", compute_tau.data.op.alpha_bu)
    .pretty_name("alpha_bu")
    .description("Constant to multiply the Bulk parameter with.")
    .link_to(&(compute_tau.data.op.alpha_bu));

  set_solution_tag("navier_stokes_solution");

  // This ensures that the linear system matrix is reset to zero each timestep
  create_component<math::LSS::ZeroLSS>("ZeroLSS")->options().set("reset_solution", false);
  // Extrapolate the velocity
  add_component(create_proto_action("LinearizeU", nodes_expression(u_adv = 2.1875*u - 2.1875*u1 + 1.3125*u2 - 0.3125*u3)));
  //add_component(create_proto_action("LinearizeU", nodes_expression(u_adv = u)));

  // Container for the assembly actions. Will be filled depending on the value of options, such as using specializations or not
  m_assembly = create_component<solver::ActionDirector>("Assembly");

  // Boundary conditions
  Handle<BoundaryConditions> bc =  create_component<BoundaryConditions>("BoundaryConditions");
  bc->mark_basic();
  bc->set_solution_tag(solution_tag());

  // Solution of the LSS
  create_component<math::LSS::SolveLSS>("SolveLSS");

  // Update of the solution
  m_update = create_component<solver::ActionDirector>("UpdateActions");

  trigger_assembly();
}

NavierStokes::~NavierStokes()
{
}


void NavierStokes::trigger_assembly()
{
  m_assembly->clear();
  m_update->clear();
  // Add the assembly, depending on the use of specialized code or not
  const bool use_specializations = options().value<bool>("use_specializations");
  set_triag_assembly(use_specializations);
  set_tetra_assembly(use_specializations);
  set_quad_assembly();
  set_hexa_assembly();
  set_prism_assembly();

  if(is_not_null(m_initial_conditions))
  {
    Handle<InitialConditions> solver_ic(m_initial_conditions->parent());
    cf3_assert(is_not_null(solver_ic));
    solver_ic->remove_component(*m_initial_conditions);
    m_initial_conditions = solver_ic->create_initial_condition(solution_tag());
  }

  m_update->add_component(create_proto_action("Update", nodes_expression(group
  (
    u3 = u2,
    u2 = u1,
    u1 = u,
    u += solution(u),
    p += solution(p)
  ))));

  if(is_not_null(m_physical_model))
  {
    configure_option_recursively(solver::Tags::physical_model(), m_physical_model);
  }

  // Ensure the initial condition field list gets updated
  if(is_not_null(m_initial_conditions))
    m_initial_conditions->options().set("field_tag", solution_tag());

  configure_option_recursively(solver::Tags::regions(), options().option(solver::Tags::regions()).value());
}

void NavierStokes::on_initial_conditions_set(InitialConditions& initial_conditions)
{
  // Initial condition for the viscosity, defaulting to the molecular viscosity
  Handle<ProtoAction> visc_ic(initial_conditions.create_initial_condition("navier_stokes_viscosity", "cf3.solver.ProtoAction"));
  visc_ic->set_expression(nodes_expression(nu_eff = nu));

  m_initial_conditions = initial_conditions.create_initial_condition(solution_tag());

  // Use a proto action to set the linearized_velocity easily
  Handle<ProtoAction> lin_vel_ic (initial_conditions.create_initial_condition("linearized_velocity", "cf3.solver.ProtoAction"));
  lin_vel_ic->set_expression(nodes_expression(group(u_adv = u, u1 = u, u2 = u, u3 = u)));

  initial_conditions.create_initial_condition("density_ratio")->options().set("density_ratio", 0.);

  // This is the place to add any readers for restarting
  Handle<InitialConditions> restart_ic(initial_conditions.create_initial_condition("Restarts", "cf3.UFEM.InitialConditions"));
  restart_ic->mark_basic();

}

} // UFEM
} // cf3
