#ifndef OMNI_PLUGINLOADER_H_
#define OMNI_PLUGINLOADER_H_

#include <vector>
#include <QDir>
#include <QString>
#include <omni/PluginInfo.h>

namespace omni {
  /**@brief PluginLoader singleton to load plugins from multiple directories
     @detail Plugins have the extension omnix
   **/
  class OMNI_EXPORT PluginLoader {
    public:
      PluginLoader(
        std::vector<QDir>const& _paths = std::vector<QDir>(),
        bool _defaultPaths = true);

      /// Load plugins from list of paths
      static void                          load(std::vector<QDir>const& _paths);

      /// Load plugins from single path
      static void                          load(QDir const& _path);

      /// Return default plugin path
      static std::vector<QDir>             defaultPaths();

      /// Return list of information about loaded plugins
      static std::vector<PluginInfo>const& loadedPlugins();

    private:
      /// Kept list of loaded plugins
      static std::vector<PluginInfo> loadedPlugins_;

      /// Load a single plugin from file
      static void loadPlugin(QString const& _file);
  };
}

#endif /* OMNI_PLUGINLOADER_H_ */
