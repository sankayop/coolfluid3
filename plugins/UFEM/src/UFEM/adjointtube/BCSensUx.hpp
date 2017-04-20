// Copyright (C) 2010-2011 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef cf3_UFEM_BCSensUx_hpp
#define cf3_UFEM_BCSensUx_hpp

#include "solver/actions/Proto/DirichletBC.hpp"
#include "solver/actions/Proto/ProtoAction.hpp"

#include "LibUFEMAdjointTube.hpp"

namespace cf3 {
  namespace math { namespace LSS { class System; } }
  namespace mesh { class Region; }
namespace UFEM {
namespace adjointtube {
/// Boundary condition to hold the value of a field at a value given by another (or the same) field
class UFEM_API BCSensUx: public solver::actions::Proto::ProtoAction
{
public:

  /// Contructor
  /// @param name of the component
  BCSensUx ( const std::string& name );

  virtual ~BCSensUx();

  /// Get the class name
  static std::string type_name () { return "BCSensUx"; }


private:
  cf3::solver::actions::Proto::DirichletBC m_dirichlet;
  Real m_n_x = 0.0;
  Real m_n_y = 0.0;

};
} // adjoint
} // UFEM
} // cf3


#endif // cf3_UFEM_BCAdjointpressurex_hpp
