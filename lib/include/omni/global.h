#pragma once

#include <QtCore/QtGlobal>

#if defined(OMNI_LIBRARY)
#  define OMNI_EXPORT Q_DECL_EXPORT
#else
#  define OMNI_EXPORT Q_DECL_IMPORT
#endif

#if defined(OMNIPLUGIN_LIBRARY)
#  define OMNIPLUGIN_EXPORT Q_DECL_EXPORT
#else
#  define OMNIPLUGIN_EXPORT Q_DECL_IMPORT
#endif
