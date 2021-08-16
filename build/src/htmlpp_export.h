
#ifndef HTMLPP_EXPORT_H
#define HTMLPP_EXPORT_H

#ifdef HTMLPP_STATIC_DEFINE
#  define HTMLPP_EXPORT
#  define HTMLPP_NO_EXPORT
#else
#  ifndef HTMLPP_EXPORT
#    ifdef htmlpp_EXPORTS
        /* We are building this library */
#      define HTMLPP_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define HTMLPP_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef HTMLPP_NO_EXPORT
#    define HTMLPP_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef HTMLPP_DEPRECATED
#  define HTMLPP_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef HTMLPP_DEPRECATED_EXPORT
#  define HTMLPP_DEPRECATED_EXPORT HTMLPP_EXPORT HTMLPP_DEPRECATED
#endif

#ifndef HTMLPP_DEPRECATED_NO_EXPORT
#  define HTMLPP_DEPRECATED_NO_EXPORT HTMLPP_NO_EXPORT HTMLPP_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef HTMLPP_NO_DEPRECATED
#    define HTMLPP_NO_DEPRECATED
#  endif
#endif

#endif /* HTMLPP_EXPORT_H */
