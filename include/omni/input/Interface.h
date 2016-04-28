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
#include <omni/serialization/PropertyMapSerializer.h>
#include <omni/mapping/Interface.h>

namespace omni {
  namespace input {
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

    class Controller;

    /// Generic input interface
    class Interface :
      public TypeIdInterface,
      public PropertyMapSerializer,
      private std::map<QString, std::unique_ptr<Interface> >{
      public:
        friend class Controller;

        typedef Interface                                     interface_type;
        typedef std::vector<Interface const*>                 inputlist_type;
        typedef std::map<QString, std::unique_ptr<Interface> >container_type;
        typedef std::function<void ()>                        callback_type;
        typedef std::set<QString> categoryset_type;

        using container_type::empty;
        using container_type::begin;
        using container_type::end;
        using container_type::clear;

        Interface(Interface const *_parent = nullptr);

        /// Virtual destructor
        virtual ~Interface();

        /// An input must return an OpenGL texture ID
        virtual GLuint textureId() const = 0;

        /// Update interface
        inline virtual void update() {}

        /// An input must return width and height information
        virtual QSize size() const = 0;

        inline size_t numberOfChildren() const {
          return container_type::size();
        }

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
        QString infoText() const {
          return QString();
        }

        /// Make new parameter widget
        virtual QWidget* widget();

        /// Make new large control widget for live mode
        inline virtual QWidget* controlWidget() {
          return nullptr;
        }

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

        inline callback_type updatedCallback() {
          return updatedCallback_;
        }

        /**@brief Add new input with given type id. Returns nullptr if input
           with typeid does not exist
           @param _id Id for the input
         *@param _typeId Type id of input to determine which kind of input is
         * created
         **/
        Interface* addInput(QString const& _id,
                            Id const& _typeId);

        /// Input with id and return pointer to input when successfully added
        Interface* addInput(QString const& _id,
                            Interface *_i);

        /// Get id of child interface
        QString getId(Interface const*) const;

        /// Remove input with id
        virtual void          removeInput(QString const& _id);

        /// Return pointer of input with id, nullptr if input does not exist
        Interface           * getInput(QString const& _id);

        /// Return pointer of input with id, nullptr if input does not exist
        Interface const     * getInput(QString const& _id) const;

        /// Return children
        container_type const& children() const;

        /// Return parent interface (const version)
        Interface const     * parent() const;

        /// Return absolute path of interface
        QString path() const;

        /// Set new parent
        void                  setParent(Interface *);

        /// Serialize to property map
        virtual void          toPropertyMap(PropertyMap&) const;

        /// Deserialize from property map
        virtual void          fromPropertyMap(PropertyMap const&);

        /// Returns a list of input maintained by controller, except this one
        inputlist_type        getAllInputs() const;

      private:
        inline virtual void activate() {
        }

        inline virtual void deactivate() {
        }

        void getInputsRecurse(Interface const* _root, inputlist_type& _list, bool _excludeThis = true) const;

        Interface const *parent_ = nullptr;
        callback_type  updatedCallback_;
    };

    /// Input Factory typedef
    typedef AbstractFactory<Interface, Interface const *>Factory;
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
