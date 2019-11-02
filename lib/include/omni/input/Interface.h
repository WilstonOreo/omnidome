/* Copyright (c) 2014-2015 "Omnidome" by Michael Winkelmann
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

#ifndef OMNI_INPUT_INTERFACE_H_
#define OMNI_INPUT_INTERFACE_H_

#include <memory>
#include <set>
#include <map>
#include <functional>
#include <QOpenGLTexture>
#include <omni/exception.h>
#include <omni/PluginInfo.h>
#include <omni/serialization/PropertyMapSerializer.h>
#include <omni/mapping/Interface.h>

namespace omni {
  namespace input {

    /// Generic input interface
    class OMNI_EXPORT Interface :
      public TypeIdInterface,
      public PropertyMapSerializer {
      public:
        friend class input::List;

        typedef Interface                                     interface_type;
        typedef std::vector<Interface const*>                 inputlist_type;
        typedef std::function<void ()>                        callback_type;
        typedef std::set<QString> categoryset_type;

        using factory_type = AbstractFactory<Interface>;

        Interface() {}

        /// Virtual destructor
        virtual ~Interface() {}

        /// An input must return an OpenGL texture ID
        virtual GLuint textureId() const = 0;

        /// Update interface
        inline virtual void update() {}

        /// An input must return width and height information
        virtual QSize size() const = 0;

        /// Return width from size
        inline int width() const
        {
          return size().width();
        }

        /// Return height from size
        inline int height() const
        {
          return size().height();
        }

        /// Input can have a fixed set of categories
        virtual categoryset_type categories() const {
          return categoryset_type();
        }

        /// Optional info text
        virtual QString infoText() const {
          return QString();
        }

        /// Make new parameter widget
        virtual QWidget* widget();

        /**@brief Returns true if this input can be added
           @detail E.g., an input can be added after an initial settings dialog
                  was approved or it has valid settings.
           @return Flag if input can be added
         **/
        inline virtual bool canAdd() {
          return true;
        }

        /// Insert a new callback when input has updated and returns its unique id
        inline int setUpdateCallback(callback_type _updatedCallback) {
          int _id = genCallbackId();
          updatedCallbacks_[_id] = _updatedCallback;
          return _id;
        }

        inline callback_type updatedCallback(int _id) {
          return updatedCallbacks_.at(_id);
        }

        void triggerUpdateCallbacks() const {
          for (auto& _idCallback : updatedCallbacks_) {
            _idCallback.second();
          }
        }

        inline void removeUpdateCallback(int _id) {
          updatedCallbacks_.erase(_id);
        }
        /// Serialize to property map
        virtual void          toPropertyMap(PropertyMap&) const {} 
        /// Deserialize from property map
        virtual void          fromPropertyMap(PropertyMap const&) {}

        static factory_type& factory();

      private:
        inline virtual void activate() {
        }

        inline virtual void deactivate() {
        }

        int genCallbackId() const {
          for (size_t i = 0; i <= updatedCallbacks_.size(); ++i) {
            if (!updatedCallbacks_.count(i)) {
              return i;
            }
          }
          return updatedCallbacks_.size();
        }

        Interface const *parent_ = nullptr;
        std::map<int,callback_type>  updatedCallbacks_;
        static factory_type factory_;
    };

    /// Input Factory typedef
    typedef AbstractFactory<Interface> Factory;
  }

  typedef input::Interface Input;
  typedef input::Factory   InputFactory;
}

#define OMNI_INPUT_INTERFACE_IID "org.omnidome.input.Interface"

Q_DECLARE_INTERFACE(omni::input::Interface, OMNI_INPUT_INTERFACE_IID)

#define OMNI_INPUT_PLUGIN_DECL                    \
  Q_OBJECT                                        \
  Q_PLUGIN_METADATA(IID OMNI_INPUT_INTERFACE_IID) \
  Q_INTERFACES(omni::input::Interface)            \
  OMNI_PLUGIN_TYPE("Input")

#endif /* OMNI_INPUT_INTERFACE_H_ */
