// ***************************************************************************
//
//   Generated automatically by genwrapper.
//   Please DO NOT EDIT this file!
//
// ***************************************************************************

#include <osgIntrospection/ReflectionMacros>
#include <osgIntrospection/TypedMethodInfo>
#include <osgIntrospection/StaticMethodInfo>
#include <osgIntrospection/Attributes>

#include <osgART/GenericVideo>
#include <osgART/VideoPlane>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_OBJECT_REFLECTOR(osgART::VideoPlane)
	I_BaseType(osgART::GenericVideoObject);
	I_Constructor1(IN, osgART::GenericVideo *, video);
	I_Method0(void, init);
	I_Method1(void, addVideo, IN, osgART::GenericVideo *, video);
END_REFLECTOR

