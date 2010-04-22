#ifndef CF_Mesh_ElementType_hh
#define CF_Mesh_ElementType_hh

////////////////////////////////////////////////////////////////////////////////

#include "Common/StringOps.hpp"
#include "Mesh/GeoShape.hpp"
#include "Common/BasicExceptions.hpp"
#include "Math/RealVector.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace CF {
namespace Mesh {

  using namespace Common;

////////////////////////////////////////////////////////////////////////////////

template <typename T> 
class Mesh_API VolumeComputer;

////////////////////////////////////////////////////////////////////////////////

/// This class represents the the data related to an ElementType
/// @author Tiago Quintino, Willem Deconinck
class Mesh_API ElementType {

public: // functions

  /// Default constructor without arguments
  ElementType();

  /// Default destructor
  ~ElementType();

public: // accessors

  /// @return m_nameShape
  std::string getShapeName() const { return m_shapeName; }

  /// @return m_geoShape
  GeoShape::Type getShape() const  {  return m_shape; }

  /// @return number of faces
  Uint getNbFaces() const  {  return m_nbFaces;  }

  /// @return number of edges
  Uint getNbEdges() const  {  return m_nbEdges;  }

  /// @return m_nbNodes
  Uint getNbNodes() const  { return m_nbNodes; }

  /// @return m_order
  Uint getOrder() const { return m_order; }

  /// @return faces connectivity
  /// faces[iFace][iNode]
  std::vector< std::vector< Uint > >& getFacesConnectivity() { return m_faces; }

  /// @return edges connectivity
  /// edges[iEdge][iNode]
  std::vector< std::vector< Uint > >& getEdgesConnectivity() { return m_edges; }
  
  virtual Real computeVolume(const std::vector<RealVector*>& coord) =0; 
  
protected: // data

  /// the string identifying the shape of this type
  std::string   m_shapeName;
  /// the GeoShape::Type corresponding to the shape
  GeoShape::Type m_shape;
  /// the number of nodes in this element type
  Uint m_nbNodes;
  /// the  geometric order of this element
  Uint m_order;
  /// number of faces
  Uint m_nbFaces;
  /// number of edges
  Uint m_nbEdges;
  
  std::vector< std::vector< Uint > > m_edges;
  std::vector< std::vector< Uint > > m_faces;


}; // end of class ElementType
  
////////////////////////////////////////////////////////////////////////////////

} // namespace Mesh
} // namespace CF

////////////////////////////////////////////////////////////////////////////////

#endif // CF_Mesh_ElementType_hh
