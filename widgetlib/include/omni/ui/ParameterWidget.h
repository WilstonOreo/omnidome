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

#ifndef OMNI_UI_PARAMETERWIDGET_H_
#define OMNI_UI_PARAMETERWIDGET_H_

#include <set>
#include <QWidget>
#include <QCheckBox>
#include <omni/ui/mixin/Scale.h>
#include <omni/ui/RangedFloat.h>
#include <omni/ui/RangedInt.h>
#include <omni/ui/Rotation.h>
#include <omni/ui/AffineTransform.h>
#include <omni/util.h>

namespace omni {
  namespace ui {
    /**@brief A widget which holds sub-widgets for manipulating parameters
    **/
    class OMNIWIDGET_EXPORT ParameterWidget :
      public QWidget,
      public mixin::Scale<omni::ui::RangedFloat>{
        Q_OBJECT
      public:
        /// Default constructor
        ParameterWidget(QWidget *_parent = nullptr);

        /// Default Constructor
        virtual ~ParameterWidget();

        /// Adds a widget with a name, a value and min-max range
        RangedFloat* addWidget(QString const&,
                               float,
                               float,
                               float);

        /// Adds a integer widget with a name, a value and min-max range
        RangedInt* addIntegerWidget(QString const&,
                                    int,
                                    int,
                                    int);

        /// Adds an angle widget with a name, a value and min-max range
        RangedFloat* addAngleWidget(QString const&,
                                    float,
                                    float,
                                    float);

        /// Adds an offset widget with a name, a value and min-max range
        RangedFloat* addOffsetWidget(QString const&,
                                     float,
                                     float,
                                     float);

        /// Adds an Rotation widget with three dials for X,Y and Z axis
        Rotation                 * addRotationWidget(QString const&);

        /// Add a new affine transform widget with an id
        omni::ui::AffineTransform* addAffineTransformWidget(QString const&,
                                                            omni::AffineTransform *);

        /// Adds a checkbox
        QCheckBox* addCheckBox(QString const&,
                               bool _checked = false);

        /// Return id of focused parameter, -1 if no parameter is focused
        int focusId() const;

      public slots:
        /// Removes all parameter widgets
        void         clear();

        virtual void updateParameters();

        /// Utility method for retrieving the value from a slider with id
        double       getParamAsFloat(QString const&) const;

        /// Set slider value from given value
        void         setParamAsFloat(QString const& _str,
                                     double _value);

        /// Utility method for retrieving the value from a slider with id
        int  getParamAsInt(QString const&) const;

        /// Set slider value from given value
        void setParamAsInt(QString const& _str,
                           int _value);

        /// Utility method for retrieving the value from checkbox with id
        bool     getParamAsBool(QString const&) const;

        /// Get parameter widget from parameter map with Id
        QWidget* getWidget(QString const&);

        /// Show / hide parameter widgets
        void     setParametersVisible(bool);

        /**@brief Put focus on next element
           @param _circular Selects first parameter when last parameter has been
                            reached
           @returns True if last parameter has been reached, always returns true
                    when in circular mode
         **/
        bool     focusNext(bool _circular = false);

        /**@brief Put focus on prev element
           @param _circular Selects last parameter when first parameter has been
                            reached
           @returns True if first parameter has been reached, always returns
              true
                    when in circular mode
         **/
        bool     focusPrev(bool _circular = false);

        /// Focus item in layout with index
        void     focus(int _index);

        /// Set focus on first widget in parameter list
        void     focusFirst();

        /// Set focus on last widget in parameter list
        void     focusLast();

        /// Set slider ranges
        virtual void     setScale(float);

        /// Set slider ranges
        virtual void     setUnit(QString const&);

      signals:
        void     parametersUpdated();

      protected:
        /// Id of first focussed widget
        inline virtual int firstFocusId() const {
          return 0;
        }

        void keyPressEvent(QKeyEvent *_event);

        /// Slider parameter widgets
        std::vector<QWidgetPtr<QWidget> > parameters_;

        /// Slider widgets, sorted by their label
        std::map<QString, QWidget *> parameterMap_;
    };
  }
}

#endif /* OMNI_UI_PARAMETERWIDGET_H_ */
