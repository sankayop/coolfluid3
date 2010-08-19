#include "Common/OptionT.hpp"

#include "Mesh/CMeshWriter.hpp"
#include "Mesh/CArray.hpp"

namespace CF {
namespace Mesh {

using namespace Common;

////////////////////////////////////////////////////////////////////////////////

CMeshWriter::CMeshWriter ( const CName& name  ) :
  Component ( name ), m_coord_dim(0), m_max_dimensionality(0)
{
  BUILD_COMPONENT;
}

////////////////////////////////////////////////////////////////////////////////

CMeshWriter::~CMeshWriter()
{
}

////////////////////////////////////////////////////////////////////////////////


void CMeshWriter::defineConfigOptions(Common::OptionList& options)
{
  //options.add< OptionT<std::string> >  ( "File",  "File to read" , "" );
  //options.add< Common::OptionT<std::string> >  ( "Mesh",  "Mesh to construct" , "" );
}

//////////////////////////////////////////////////////////////////////////////

void CMeshWriter::write( XmlNode& node  )
{
  // Get the mesh component in the tree
  /// @todo[1]: wait for Tiago for functionality

  // Get the file path
  boost::filesystem::path file = option("File")->value<std::string>();

  // Call implementation
  /// @todo wait for todo[1]
  // write_from_to(mesh,file);

}

//////////////////////////////////////////////////////////////////////////////

boost::filesystem::path CMeshWriter::write_from(const CMesh::Ptr& mesh)
{
  // Get the file path
  boost::filesystem::path file = option("File")->value<std::string>();

  // Call implementation
  write_from_to(mesh,file);

  // return the file
  return file;
}

//////////////////////////////////////////////////////////////////////////////
  
void CMeshWriter::compute_mesh_specifics()
{
  // - Assemble the map that gives a list of elementregions for each coordinate component
  // - Find maximal dimensionality of the whole mesh
  m_all_coordinates.clear();
  m_max_dimensionality = 0;
  m_coord_dim = 0;
  BOOST_FOREACH(CElements& elements, recursive_range_typed<CElements>(*m_mesh))
  { 
    m_all_coordinates[&elements.coordinates()].push_back(&elements);
    m_max_dimensionality = std::max(elements.element_type().dimensionality() , m_max_dimensionality);
    m_coord_dim = std::max((Uint) elements.coordinates().array().shape()[1] , m_coord_dim);
  }  
}

//////////////////////////////////////////////////////////////////////////////


} // Mesh
} // CF
