/* Copyright (c) 2014-2016 "Omnidome" by Michael Winkelmann
 * Dome Mapping Projection Software (http://omnido.me).
 * Omnidome was created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 *
 * This file is part of Omnidome.
 *
 * Omnidome is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OMNI_PLUGININFO_H_
#define OMNI_PLUGININFO_H_

#include <QObject>
#include <QString>
#include <QMetaClassInfo>
#include <omni/global.h>

namespace omni {
  class PluginLoader;

  /**@brief Holds meta information about a plugins
     @detail Only plugin loader can construct a plugin info
   **/
  class OMNI_EXPORT PluginInfo {
    public:
      /// Only plugin loader can construct a plugin info
      friend class PluginLoader;

      /// Return name of plugin
      QString const& name() const;

      /// Return type of plugin (e.g. input, mapping or canvas)
      QString const& type() const;

      /// Return author of plugin
      QString const& author() const;

      /// Return url of plugin
      QString const& url() const;

      /// Return description of plugin
      QString const& description() const;

      /// Return version string (MAJOR.MINOR.REV.BUILD) of plugin
      QString        versionString() const;

      /// Return major version
      int            majorVersion() const;

      /// Return minor version
      int            minorVersion() const;

      /// Return revision number
      int            revision() const;

      /// Return build number
      int            build() const;

      /// Return class name from meta object
      QString const& className() const;

      /// Return file name of plugin
      QString const& file() const;

      /// Return handle (pointer to underlying QObject)
      QObject const* handle() const;

    private:
      /// Make a PluginInfo from file and QObject
      static PluginInfo make(QString const& _file,
                             QObject const *);

      QString name_;
      QString type_;
      QString author_;
      QString url_;
      QString description_;
      QString className_;
      int     majorVersion_ = 0;
      int     minorVersion_ = 0;
      int     revision_     = 0;
      int     build_        = 0;
      QString file_;
  };
}

#define OMNI_PLUGIN_AUTHOR(AUTHOR) \
  Q_CLASSINFO("author", AUTHOR)

#define OMNI_PLUGIN_URL(URL) \
  Q_CLASSINFO("url", URL)

#define OMNI_PLUGIN_NAME(NAME) \
  Q_CLASSINFO("name", NAME)

#define OMNI_PLUGIN_DESCRIPTION(DESC) \
  Q_CLASSINFO("description", DESC)

#define OMNI_PLUGIN_VERSION(MAJOR, MINOR, REV, BUILD) \
  Q_CLASSINFO("version",# MAJOR "." # MINOR "." # REV "." # BUILD)

#define OMNI_PLUGIN_TYPE(T) \
  Q_CLASSINFO("type", T)

#define OMNI_PLUGIN_INFO(NAME, DESC)             \
  Q_CLASSINFO("version", OMNI_VERSION_STRING) \
  OMNI_PLUGIN_AUTHOR("Michael Winkelmann")                     \
  OMNI_PLUGIN_URL("http://omnido.me")        \
  OMNI_PLUGIN_NAME(NAME)                          \
  OMNI_PLUGIN_DESCRIPTION(DESC)

#endif /* OMNI_PLUGININFO_H_ */
