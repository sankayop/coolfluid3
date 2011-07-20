// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef CF_RDM_LibCore_hpp
#error  Header RDM/Core/Tags.hpp shouldnt be included directly, include LibCore.hpp
#endif

#ifndef CF_RDM_Core_Tags_hpp
#define CF_RDM_Core_Tags_hpp

////////////////////////////////////////////////////////////////////////////////

namespace CF {
namespace RDM {
namespace Core {

/// Class defines the tags for the RDM components
/// @author Tiago Quintino
class RDM_Core_API Tags : public NonInstantiable<Tags> {
public:

  static const char * fields()         { return "Fields";  }
  static const char * actions()        { return "Actions"; }

  static const char * mesh()           { return "mesh"; }
  static const char * physical_model() { return "physical_model"; }

  static const char * residual()      { return "residual"; }
  static const char * wave_speed()    { return "wave_speed"; }
  static const char * solution()      { return "solution"; }

  static const char * update_vars()   { return "update_vars"; }

}; // Tags

} // Core
} // RDM
} // CF

#endif // CF_RDM_Core_Tags_hpp
