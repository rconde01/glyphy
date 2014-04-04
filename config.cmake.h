#define PKGDATADIR "@CMAKE_INSTALL_PREFIX@"

#cmakedefine DEFAULT_FONT "@DEFAULT_FONT@"

#cmakedefine HAVE_FREETYPE2

#cmakedefine HAVE_GL

#cmakedefine HAVE_GLEW

#cmakedefine HAVE_GLUT

#cmakedefine HAVE_SYS_TIME_H

#cmakedefine HAVE_TIME_H

#cmakedefine HAVE_HASH_MAP_H

#cmakedefine HAVE_EXT_HASH_MAP_H

#cmakedefine HAVE_INFINITY

#cmakedefine HAVE_STD_ISNAN

#cmakedefine HAVE_ISNAN_MACRO

#cmakedefine HAVE_ISNAN

#cmakedefine HAVE_ISNAN_ALT

#cmakedefine HAVE_STD_ISFINITE

#cmakedefine HAVE_ISFINITE

#cmakedefine HAVE_ISFINITE_MACRO

#cmakedefine HAVE_FINITE

#cmakedefine HAVE_STD_ISINF

#cmakedefine HAVE_ISINF

#cmakedefine HAVE_ISINF_MACRO

#cmakedefine HAVE_LROUND

#cmakedefine HAVE_STD_LROUND

#cmakedefine HAVE_ROUND

#cmakedefine HAVE_STD_ROUND

#cmakedefine HAVE_LOG2

#cmakedefine HAVE_STD_LOG2

#ifndef HAVE_INFINITY
#  define INFINITY ((float)(1e+300*1e+300))
#endif

#if defined(HAVE_STD_ISNAN)
#  define isnan(x) std::isnan(x)
#elif defined(HAVE_ISNAN_MACRO)
#elif defined(HAVE_ISNAN)
#  define isnan(x) isnan(x)
#elif defined(HAVE_ISNAN_ALT)
#  define isnan(x) _isnan(static_cast<double>(x))
#else
#  error "Could not find isnan function or macro!"
#endif

#if defined(HAVE_STD_ISFINITE)
#  define isfinite(x) std::isfinite(x)
#elif defined(HAVE_ISFINITE) || defined(HAVE_ISFINITE_MACRO)
#elif defined(HAVE_FINITE)
#  include<float.h>
#  define isfinite(x) _finite(static_cast<double>(x))
#else
# error "Could not find finite or isfinite function or macro!"
#endif

#if defined(HAVE_STD_ISINF)
#  define isinf(x) std::isinf(x)
#elif defined(HAVE_ISINF) || defined(HAVE_ISINF_MACRO)
#else
#  define isinf(x) (!isfinite(x) && !isnan(x))
#endif

#if defined(HAVE_STD_LROUND)
#  define lround(x) std::lround(x)
#elif defined(HAVE_LROUND)
#  define lround(x) lround(x)
#else
#  define lround(x) static_cast<long>(x > 0 ? floor(x + 0.5) : ceil(x - 0.5))
#endif

#if defined(HAVE_STD_ROUND)
#  define round(x) std::round(x)
#elif defined(HAVE_ROUND)
#  define round(x) round(x)
#else
#  define round(x) (x > 0 ? floor(x + 0.5) : ceil(x - 0.5))
#endif

#if defined(HAVE_STD_LOG2)
#  define log2(x) std::log2(x)
#elif defined(HAVE_LOG2)
#  define log2(x) log2(x)
#else
#  define log2(x) (log(static_cast<double>(x)) / log (2.0))
#endif
