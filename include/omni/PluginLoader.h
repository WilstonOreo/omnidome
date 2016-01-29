#ifndef OMNI_PLUGINLOADER_H_
#define OMNI_PLUGINLOADER_H_

#include <vector>
#include <QDir>
#include <QString>

namespace omni {
    /**@brief PluginLoader singleton to load plugins from multiple directories
       @detail Plugins have the extension omnix
     **/
    class PluginLoader {
    public:
        PluginLoader(std::vector<QDir> const& _paths);

        /// Load plugins from list of paths
        static void load(std::vector<QDir> const& _paths);

        /// Load plugins from single path
        static void load(QDir const& _path);
    private:

        /// Load a single plugin from file
        void loadPlugin(QString const& _file);
    };
}

#endif /* OMNI_PLUGINLOADER_H_ */
