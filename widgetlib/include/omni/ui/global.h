#pragma once

#include <QtCore/QtGlobal>

#if defined(OMNIWIDGET_LIBRARY)
#  define OMNIWIDGET_EXPORT Q_DECL_EXPORT
#else
#  define OMNIWIDGET_EXPORT Q_DECL_IMPORT
#endif
