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

#ifndef Q_MOC_RUN
#include <set>
#include <memory>
#include <QObject>
#include "Locked.h"

namespace omni {
    namespace ui {
        namespace mixin {
            /// Holds a specific data model with frontend update mechanism
            template<typename DATAMODEL>
            class DataModel : public Locked {
            public:
                /// Data type
                typedef DATAMODEL data_model_type;
                typedef DataModel<data_model_type> type;

                /// Set new data model
                void setDataModel(std::shared_ptr<data_model_type> _dataModel) {
                    this->locked([&]() {
                        dataModel_ = _dataModel;
                        updateFrontend();
                    });
                }

                /// Return pointer to data model
                data_model_type* dataModel() {
                    return dataModel_.get();
                }

                /// Return pointer to data model (const version)
                data_model_type const* dataModel() const {
                    return dataModel_.get();
                }

                /// Push data to frontend widgets and all child widgets
                virtual void updateFrontend() {
                    if (dataModel_) {
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
                            updateFrontend();
                        }
                    });
                }
                //virtual inline void dataModelChanged() {}

            private:
                virtual void dataToFrontend() = 0;

                /// Return true if data has changed by front end
                virtual bool frontendToData() = 0;

                inline virtual void emitDataModelChangedSignal() {
                }
                std::set<type*> children_;

                std::shared_ptr<data_model_type> dataModel_ = nullptr;
            };
        }

        using mixin::DataModel;
    }
}

#define OMNI_UI_DATAMODEL(MODEL) \
    public:\
        typedef omni::ui::mixin::DataModel<MODEL> mixin_data_model_type; \
        inline void updateDataModel() {\
            mixin_data_model_type::updateDataModel();\
        }\
    private:\
        void emitDataModelChangedSignal() {\
            emit dataModelChanged();\
        }\
    public:

#endif

#endif /* OMNI_UI_MIXIN_DATAMODEL_H_ */
