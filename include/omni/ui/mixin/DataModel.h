/* Copyright (c) 2014-2016 "Omnidome" by cr8tr
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
#include "Locked.h"

namespace omni {
    namespace ui {
        namespace mixin {
            namespace detail {
                template<typename T, bool SHARED>
                struct PointerType {};

                /// Use shared pointer when shared
                template<typename T>
                struct PointerType<T,true> {
                    typedef T data_type;
                    typedef std::shared_ptr<T> type;

                    static T* raw_pointer(type _t) {
                        return _t.get();
                    }

                    static T const* raw_pointer_const(type _t) {
                        return _t.get();
                    }
                };

                /// Use raw pointer when not shared
                template<typename T>
                struct PointerType<T,false> {
                    typedef T* type;

                    static T* raw_pointer(type _t) {
                        return _t;
                    }

                    static T const* raw_pointer_const(const type _t) {
                        return _t;
                    }
                };
            }


            class DataModelInterface {
            public:

                virtual void updateDataModel() = 0;
                virtual void updateFrontend() = 0;
            };


            /// Holds a specific data model with frontend update mechanism
            template<typename DATAMODEL, bool SHARED>
            class DataModel :
                public DataModelInterface,
                protected Locked {
            public:
                /// Data type
                typedef DATAMODEL data_model_type;
                typedef DataModel<data_model_type,SHARED> type;
                typedef detail::PointerType<data_model_type,SHARED> pointer_type_handler;
                typedef typename pointer_type_handler::type pointer_type;

                /// Set new data model
                void setDataModel(pointer_type _dataModel) {
                    this->locked([&]() {
                        dataModel_ = _dataModel;
                        updateFrontend();
                    });
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
                    if (dataModel_ && this->isLocked()) {
                        dataToFrontend();
                        for (auto& _child : children_) {
                            _child->setDataModel(dataModel_);
                        }
                    }
                }

                /// Add child that shares the same data model
                void addChild(type* _child) {
                    if (_child == this) return;
                    children_.insert(_child);
                }

                /// Remove child
                void removeChild(type* _child) {
                    if (children_.count(_child) == 0) {
                        return;
                    }
                    children_.erase(_child);
                }

                /// Remove all children
                void removeAllChildren(type* _child) {
                    children_.clear();
                }

                virtual void updateDataModel() {
                    if (this->isLocked()) {
                        return;
                    }
                    this->locked([&]() {
                        if (frontendToData()) {
                            emitDataModelChangedSignal();
                        }
                    });
                }

            private:
                virtual void dataToFrontend() = 0;

                /// Return true if data has changed by front end
                virtual bool frontendToData() = 0;

                inline virtual void emitDataModelChangedSignal() {
                }
                std::set<type*> children_;

                pointer_type dataModel_ = nullptr;
            };

            template<typename T>
            using SharedDataModel = DataModel<T,true>;

            template<typename T>
            using UnsharedDataModel = DataModel<T,false>;

            /**@brief Instantiate a new widget and set data model
               @detail Uses raw pointer (unshared data model)
             **/
            template<typename WIDGET, typename DATAMODEL, typename...ARGS>
            static WIDGET* makeWidget(DATAMODEL* _dataModel, ARGS&&..._args) {
                auto* _widget = new WIDGET(_args...);
                _widget->setDataModel(_dataModel);
                return _widget;
            }

            /**@brief Instantiate a new widget and set data model
               @detail Uses shared pointer (shared data model)
             **/
            template<typename WIDGET, typename DATAMODEL, typename...ARGS>
            static WIDGET* makeWidget(std::shared_ptr<DATAMODEL> _dataModel, ARGS&&..._args) {
                auto* _widget = new WIDGET(_args...);
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
#define OMNI_UI_DATAMODEL(MODEL,SHARED) \
    public:\
        typedef omni::ui::mixin::DataModel<MODEL,SHARED> mixin_data_model_type; \
        inline void updateDataModel() {\
            mixin_data_model_type::updateDataModel();\
        }\
        inline void updateFrontend() {\
            this->locked([&]() {\
                mixin_data_model_type::updateFrontend();\
                update(); \
            });\
        }\
    private:\
        void emitDataModelChangedSignal() {\
            emit dataModelChanged();\
        }\
    public:

#define OMNI_UI_SHARED_DATAMODEL(MODEL) \
    OMNI_UI_DATAMODEL(MODEL,true)

#define OMNI_UI_UNSHARED_DATAMODEL(MODEL) \
    OMNI_UI_DATAMODEL(MODEL,false)

#endif /* OMNI_UI_MIXIN_DATAMODEL_H_ */
