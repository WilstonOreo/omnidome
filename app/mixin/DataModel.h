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
#ifndef OMNI_UI_MIXIN_DATAMODEL_H_
#define OMNI_UI_MIXIN_DATAMODEL_H_

#include <set>
#include <memory>
#include <QObject>
#include <QSignalBlocker>

namespace omni {
  namespace ui {
    namespace mixin {
      namespace detail {
        /// Pointer type handler
        template<typename T, bool SHARED>
        struct PointerType {};

        /// Use std::shared_ptr when shared is true
        template<typename T>
        struct PointerType<T, true>{
          typedef T                 data_type;
          typedef std::shared_ptr<T>type;

          static void initialize(type& _t) {}

          static T* raw_pointer(type _t) {
            return _t.get();
          }

          static T const* raw_pointer_const(type _t) {
            return _t.get();
          }
        };

        /// Use raw pointer (T*) when shared is false
        template<typename T>
        struct PointerType<T, false>{
          typedef T *type;

          static void initialize(type& _t) {
            _t = nullptr;
          }

          static T* raw_pointer(type _t) {
            return _t;
          }

          static T const* raw_pointer_const(const type _t) {
            return _t;
          }
        };
      }

      /// Data model interface
      class DataModelInterface {
        public:
          /// Update the data model interface
          virtual void updateDataModel() = 0;
          virtual void updateFrontend()  = 0;
      };


      /**@brief Holds a specific data model with frontend update mechanism
         @tparam DATAMODEL Type of data which is stored
         @tparam SHARED Boolean flag to tell of data model is
                        encapsulared in a shared_ptr
       **/
      template<typename DERIVED, typename DATAMODEL, bool SHARED>
      class DataModel :
        public DataModelInterface {
        public:
          /// Derived type
          using derived_type = DERIVED;

          /// Data type
          using data_model_type = DATAMODEL;

          /// This type
          using type = DataModel<DERIVED, data_model_type, SHARED>;

          /// Pointer type handler
          using pointer_type_handler = detail::PointerType<
                  data_model_type, SHARED>;

          /// Derived pointer type
          using pointer_type = typename pointer_type_handler::type;

          DataModel() {
            pointer_type_handler::initialize(dataModel_);
          }

          /// Set new data model
          void setDataModel(pointer_type _dataModel) {
            QSignalBlocker blocker(derived());

            dataModel_ = _dataModel;
            updateFrontend();
          }

          /// Return pointer to data model
          pointer_type dataModel() {
            return dataModel_;
          }

          /// Return pointer to data model (const version)
          const pointer_type dataModel() const {
            return dataModel_;
          }

          /// Push data to frontend widgets and all child widgets
          virtual void updateFrontend() {
            if (dataModel_ && derived()->signalsBlocked()) {
              dataToFrontend();
            }
          }

          virtual void updateDataModel() {
            if (derived()->signalsBlocked()) return;

            QSignalBlocker blocker(derived());

            if (frontendToData()) {
              blocker.unblock();
              emitDataModelChangedSignal();
            }
          }

        private:
          derived_type* derived() {
            return static_cast<derived_type *>(this);
          }

          /**@brief Pure virtual method which determines how data model is
            represented in frontend
          **/
          virtual void dataToFrontend() = 0;

          /// Return true if data has changed by front end
          virtual bool frontendToData() = 0;

          /// Internal method used to emit dataModelChanged signal
          inline virtual void emitDataModelChangedSignal() {}

          pointer_type dataModel_;
      };

      /// Template alias for shared data model
      template<typename DERIVED, typename T>
      using SharedDataModel = DataModel<DERIVED, T, true>;

      /// Template alias for unshared data model
      template<typename DERIVED, typename T>
      using UnsharedDataModel = DataModel<DERIVED, T, false>;

      /**@brief Instantiate a new widget and set data model
         @detail Uses raw pointer (unshared data model)
       **/
      template<typename WIDGET, typename DATAMODEL, typename ... ARGS>
      static WIDGET* makeWidget(DATAMODEL *_dataModel, ARGS&& ... _args) {
        auto *_widget = new WIDGET(_args ...);

        _widget->setDataModel(_dataModel);
        return _widget;
      }

      /**@brief Instantiate a new widget and set data model
         @detail Uses shared pointer (shared data model)
       **/
      template<typename WIDGET, typename DATAMODEL, typename ... ARGS>
      static WIDGET* makeWidget(std::shared_ptr<DATAMODEL>_dataModel,
                                ARGS&& ... _args) {
        auto *_widget = new WIDGET(_args ...);

        _widget->setDataModel(_dataModel);
        return _widget;
      }
    }

    using mixin::DataModel;
    using mixin::SharedDataModel;
    using mixin::UnsharedDataModel;
    using mixin::makeWidget;
  }
}

/// This macro needs to be inserted for every widget that uses DataModel
#define OMNI_UI_DATAMODEL(DERIVED, MODEL, SHARED)                    \
  public:                                                            \
    typedef omni::ui::mixin::DataModel<DERIVED, MODEL,               \
                                       SHARED>mixin_data_model_type; \
    inline void updateDataModel() override {                         \
      mixin_data_model_type::updateDataModel();                      \
    }                                                                \
    inline void updateFrontend() override {                          \
      QSignalBlocker blocker(this);                                  \
      mixin_data_model_type::updateFrontend();                       \
      update();                                                      \
    }                                                                \
  private:                                                           \
    void emitDataModelChangedSignal() override {                     \
      emit dataModelChanged();                                       \
    }                                                                \
  public:
#define OMNI_UI_SHARED_DATAMODEL(DERIVED, MODEL) \
  OMNI_UI_DATAMODEL(DERIVED, MODEL, true)

#define OMNI_UI_UNSHARED_DATAMODEL(DERIVED, MODEL) \
  OMNI_UI_DATAMODEL(DERIVED, MODEL, false)

#endif /* OMNI_UI_MIXIN_DATAMODEL_H_ */
