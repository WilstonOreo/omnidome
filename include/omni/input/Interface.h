/* Copyright (c) 2014-2015 "Omnidome" by cr8tr
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
#include <omni/serialization/Interface.h>
#include <omni/mapping/Interface.h>

namespace omni
{
  namespace input
  {
    namespace exception {
        /**@brief Exception is thrown when accessing input of a children
           @detail Is only thrown when input cannot accept children
         **/
        class CannotHaveChildren : public omni::exception::Error {
        public:
            OMNI_EXCEPTION(CannotHaveChildren)

            inline QString message() const throw()
            {
                return QString("Input cannot have children!");
            }
        };
    }

    /// Generic input interface
    class Interface :
        public SerializationInterface,
        public TypeIdInterface
    {
    public:
      typedef Interface interface_type;
      typedef std::map<QString,std::unique_ptr<Interface>> children_type;
      typedef std::function<void()> callback_type;

      /// Virtual destructor
      virtual ~Interface() {
          unregisterAll();
      }

      /// An input must return an OpenGL texture ID
      virtual GLuint textureId() const = 0;

      /// Update interface
      inline virtual void update() {}

      /**@brief Free stored OpenGL Content (like textures, shaders etc)
       * @detail Is called before destructor, when there is still an active OpenGL context
       **/
      inline virtual void free() {}

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

      /// Optional info text
      QString infoText() const {
          return QString();
      }

      /// Make new parameter widget
      virtual QWidget* widget() = 0;

      /**@brief Returns true if this input can be added
         @detail E.g., an input can be added after an initial settings dialog
                was approved or it has valid settings.
         @return Flag if input can be added
      **/
      inline virtual bool canAdd() {
          return true;
      }

      inline  virtual bool canHaveChildren() const {
          return false;
      }

      inline void setUpdateCallBack(callback_type _updatedCallback) {
          updatedCallback_ = _updatedCallback;
      }

      callback_type updatedCallback() {
          return updatedCallback_;
      }

      /// Input with id and return pointer to input when successfully added
      Interface* addInput(QString const& _id, Interface* _i);

      /// Remove input with id
      void removeInput(QString const& _id);

      /// Return pointer of input with id, nullptr if input does not exist
      Interface* getInput(QString const& _id);

      /// Return pointer of input with id, nullptr if input does not exist
      Interface const* getInput(QString const& _id) const;

      /// Return children
      children_type& children();

      /// Return children
      children_type const& children() const;

      void useInput(Interface* _i) {
          if (_i != this) return;

          _i->used_.insert(this);
          this->used_.insert(_i);
      }

      void unregister(Interface* _i) {
          _i->used_.erase(this);
          used_.erase(_i);
      }

      void unregisterAll() {
          for (auto& _i : used_) {
              unregister(_i);
          }
      }

    private:
        std::set<Interface*> used_;
        children_type children_;
        callback_type updatedCallback_;
    };

    /// Input Factory typedef
    typedef AbstractFactory<Interface> Factory;
  }

  typedef input::Interface Input;
  typedef input::Factory InputFactory;
}

#define OMNI_INPUT_INTERFACE_IID "org.omnidome.input.Interface"

Q_DECLARE_INTERFACE(omni::input::Interface, OMNI_INPUT_INTERFACE_IID)

#define OMNI_INPUT_PLUGIN_DECL \
    Q_OBJECT \
    Q_PLUGIN_METADATA(IID OMNI_INPUT_INTERFACE_IID) \
    Q_INTERFACES(omni::input::Interface) \
    OMNI_PLUGIN_TYPE("Input")

#endif /* OMNI_INPUT_INTERFACE_H_ */
