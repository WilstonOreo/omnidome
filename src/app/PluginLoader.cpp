#include <omni/PluginLoader.h>

#include <omni/input/Interface.h>
#include <omni/mapping/Interface.h>
#include <omni/canvas/Interface.h>

#include <QDirIterator>
#include <QPluginLoader>

namespace omni {
    PluginLoader::PluginLoader(std::vector<QDir> const& _paths) {
        load(_paths);
    }

    void PluginLoader::load(QDir const& _path) {
        QDirIterator dirIt(_path,QDirIterator::Subdirectories);
        while (dirIt.hasNext()) {
            dirIt.next();
            auto _path = dirIt.filePath();
            if (!QFileInfo(_path).isFile()) continue;

            if (QFileInfo(_path).suffix() == "omnix") {
                load(_path);
            }
        }
    }

    void PluginLoader::load(std::vector<QDir> const& _paths) {
        for (auto& _path : _paths) {
            load(_path);
        }
    }

    void PluginLoader::loadPlugin(QString const& _path) {
        QPluginLoader _pluginLoader(_path);
        QObject *plugin = _pluginLoader.instance();
        if (plugin) {
            auto* _inputInterface = qobject_cast<input::Interface*>(plugin);
            if (_inputInterface) {
                _inputInterface->registerInFactory();
                return;
            }
            auto* _mappingInterface = qobject_cast<mapping::Interface*>(plugin);
            if (_mappingInterface) {
                _mappingInterface->registerInFactory();
                return;
            }
            auto* _canvasInterface = qobject_cast<canvas::Interface*>(plugin);
            if (_canvasInterface) {
                _canvasInterface->registerInFactory();
                return;
            }
        }
    }
}
