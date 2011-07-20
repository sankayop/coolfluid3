// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include "Common/Log.hpp"
#include "Common/Signal.hpp"
#include "Common/CBuilder.hpp"
#include "Common/OptionT.hpp"
#include "Common/OptionArray.hpp"

#include "Common/XML/SignalOptions.hpp"

#include "Mesh/CMesh.hpp"

#include "Physics/PhysModel.hpp"

#include "RDM/Core/CellTerm.hpp"
#include "RDM/Core/FaceTerm.hpp"

#include "DomainDiscretization.hpp"

using namespace CF::Common;
using namespace CF::Common::XML;
using namespace CF::Mesh;

namespace CF {
namespace RDM {
namespace Core {

///////////////////////////////////////////////////////////////////////////////////////

Common::ComponentBuilder < DomainDiscretization, CAction, LibCore > DomainDiscretization_Builder;

///////////////////////////////////////////////////////////////////////////////////////

DomainDiscretization::DomainDiscretization ( const std::string& name ) :
  CF::Solver::ActionDirector(name)
{
  mark_basic();

  // signals

  regist_signal( "create_cell_term" )
      ->connect  ( boost::bind( &DomainDiscretization::signal_create_cell_term, this, _1 ) )
      ->signature( boost::bind( &DomainDiscretization::signature_signal_create_cell_term, this, _1))
      ->description("creates a discretization term for cells")
      ->pretty_name("Create Cell Term");

  regist_signal( "create_face_term" )
      ->connect  ( boost::bind( &DomainDiscretization::signal_create_face_term, this, _1 ) )
      ->signature( boost::bind( &DomainDiscretization::signature_signal_create_face_term, this, _1))
      ->description("creates a discretization term for faces")
      ->pretty_name("Create Cell Term");


  m_face_terms = create_static_component_ptr<CActionDirector>("FaceTerms");
  m_cell_terms = create_static_component_ptr<CActionDirector>("CellTerms");
}


void DomainDiscretization::execute()
{
  CFinfo << "[RDM] applying domain discretization" << CFendl;

  // apply first weak bcs, since they do not set any value directly

  m_face_terms->execute();

  // strong bcs need to be updated last

  m_cell_terms->execute();
}


void DomainDiscretization::signal_create_cell_term( SignalArgs& args )
{
  SignalOptions options( args );

  std::string name = options.value<std::string>("Name");
  std::string type = options.value<std::string>("Type");

  RDM::CellTerm::Ptr term = build_component_abstract_type<RDM::CellTerm>(type,name);

  m_cell_terms->append(term);

  std::vector<URI> regions = options.array<URI>("Regions");

  term->configure_option("regions" , regions);

  if( m_mesh.lock() )
    term->configure_option( Tags::mesh(), m_mesh.lock()->uri());
  if( m_physical_model.lock() )
    term->configure_option( Tags::physical_model() , m_physical_model.lock()->uri());

}


void DomainDiscretization::signal_create_face_term( SignalArgs& args )
{
  SignalOptions options( args );

  std::string name = options.value<std::string>("Name");
  std::string type = options.value<std::string>("Type");

  RDM::FaceTerm::Ptr term = build_component_abstract_type<RDM::FaceTerm>(type,name);

  m_face_terms->append(term);

  std::vector<URI> regions = options.array<URI>("Regions");

  term->configure_option("regions" , regions);

  if( m_mesh.lock() )
    term->configure_option( Tags::mesh(), m_mesh.lock()->uri());
  if( m_physical_model.lock() )
    term->configure_option( Tags::physical_model() , m_physical_model.lock()->uri());

}


void DomainDiscretization::signature_signal_create_cell_term( SignalArgs& args )
{
  SignalOptions options( args );

  // name

  options.add_option< OptionT<std::string> >("Name", std::string() )
      ->set_description("Name for created volume term");

  // type

  /// @todo loop over the existing CellTerm providers to provide the available list

  //  std::vector< std::string > restricted;
  //  restricted.push_back( std::string("CF.RDM.Core.BcDirichlet") );
  //  XmlNode type_node = options.add_option< OptionT<std::string> >("Type", std::string("CF.RDM.Core.BcDirichlet"), "Type for created boundary");
  //  Map(type_node).set_array( Protocol::Tags::key_restricted_values(), restricted, " ; " );

  // regions

  std::vector<URI> dummy;

  /// @todo create here the list of restricted volume regions

  options.add_option< OptionArrayT<URI> >("regions", dummy )
      ->set_description("Regions where to apply the domain term");
}


void DomainDiscretization::signature_signal_create_face_term( SignalArgs& args )
{
  SignalOptions options( args );

  // name

  options.add_option< OptionT<std::string> >("Name", std::string() )
      ->set_description("Name for created volume term");

  // type

  /// @todo loop over the existing FaceTerm providers to provide the available list

  //  std::vector< std::string > restricted;
  //  restricted.push_back( std::string("CF.RDM.Core.BcDirichlet") );
  //  XmlNode type_node = options.add_option< OptionT<std::string> >("Type", std::string("CF.RDM.Core.BcDirichlet"), "Type for created boundary");
  //  Map(type_node).set_array( Protocol::Tags::key_restricted_values(), restricted, " ; " );

  // regions

  std::vector<URI> dummy;

  /// @todo create here the list of restricted face regions

  options.add_option< OptionArrayT<URI> >("regions", dummy )
      ->set_description("Regions where to apply the domain term");
}

/////////////////////////////////////////////////////////////////////////////////////

} // Core
} // RDM
} // CF
