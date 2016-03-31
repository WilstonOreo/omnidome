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

#include <omni/proj/Tuning.h>

#include <QOpenGLContext>
#include <omni/util.h>
#include <omni/serialization/PropertyMap.h>
#include <omni/proj/Setup.h>
#include <omni/proj/ScreenSetup.h>

#include <omni/visual/Tuning.h>

#include <QColor>


namespace omni {
    namespace proj {
        Tuning::Tuning() :
            color_("#FFFFFF"),
            blendMask_(*this)
        {}

        Tuning::Tuning(const QColor& _color) :
            color_(_color),
            blendMask_(*this)
        {}

        void Tuning::setScreen(QScreen const *_screen, int _subScreenIndex)
        {
            if (!_screen) _screen = ScreenSetup::standardScreen();

            auto _oldScreenSize = projector_.screen()->size();
            projector_.setScreen(_screen, _subScreenIndex);

            if (_screen->size() != _oldScreenSize)
            {
                blendMask_.resize(
                    ScreenSetup::subScreenWidth(_screen),
                    _screen->size().height());
            }
        }

        QScreen const * Tuning::screen() const
        {
            return projector_.screen();
        }

        int Tuning::subScreenIndex() const
        {
            return projector_.subScreenIndex();
        }

        Projector& Tuning::projector()
        {
            return projector_;
        }

        Projector const& Tuning::projector() const
        {
            return projector_;
        }

        WarpGrid& Tuning::warpGrid()
        {
            return warpGrid_;
        }

        WarpGrid const& Tuning::warpGrid() const
        {
            return warpGrid_;
        }

        BlendMask& Tuning::blendMask()
        {
            return blendMask_;
        }

        BlendMask const& Tuning::blendMask() const
        {
            return blendMask_;
        }

        ColorCorrection& Tuning::colorCorrection() {
            return colorCorrection_;
        }

        ColorCorrection const& Tuning::colorCorrection() const {
            return colorCorrection_;
        }

        QColor Tuning::color() const
        {
            return color_;
        }

        void Tuning::setColor(QColor const& _color)
        {
            color_ = _color;
        }

        /// Return pointer to visualizer
        Tuning::visualizer_type* Tuning::visualizer() {
          return viz_.get();
        }

        Tuning::visualizer_type* Tuning::makeVisualizer() {
          if (!viz_ && QOpenGLContext::currentContext()) {
            viz_.reset(new visualizer_type(*this));
          }
          return visualizer();
        }

        /// Return pointer to visualizer (const version)
        Tuning::visualizer_type const* Tuning::visualizer() const {
          return viz_.get();
        }

        bool Tuning::hasScreen() const
        {
            return projector_.screen() != ScreenSetup::standardScreen();
        }

        int Tuning::width() const
        {
            if (!ScreenSetup::standardScreen()) {
                return 0;
            }

            return !hasScreen() ?
                   ScreenSetup::subScreenWidth(ScreenSetup::standardScreen()) :
                   ScreenSetup::subScreenWidth(projector_.screen());
        }

        int Tuning::height() const
        {
            if (!ScreenSetup::standardScreen()) {
                return 0;
            }

            return !hasScreen() ?
                   ScreenSetup::standardScreen()->size().height() :
                   projector_.screen()->size().height();
        }

        /// Return flag if output is disabled, projector output is black
        bool Tuning::outputDisabled() const {
            return outputDisabled_;
        }

        /// Return flag if output is enabled
        bool Tuning::outputEnabled() const {
            return !outputDisabled();
        }

        /// Disable output if _disabled is true, enable otherwise
        void Tuning::setOutputDisabled(bool _disabled) {
            outputDisabled_ = _disabled;
        }

        /// Enable output if _enabled is true, disable otherwise
        void Tuning::setOutputEnabled(bool _enabled) {
            setOutputDisabled(!_enabled);
        }
        /// Return opacity of overlapped blend mask
        float Tuning::overlapOpacity() const {
            return overlapOpacity_;
        }

        /// Set opacity of overlap mask in blend mode
        void Tuning::setOverlapOpacity(float _overlapOpacity) {
            overlapOpacity_ = _overlapOpacity;
        }

        /// Write tuning to stream
        void Tuning::toStream(QDataStream& _os) const {
            PropertyMap _map;
            _map("color",color_)
                ("projector",projector_)
                ("warpGrid",warpGrid_)
                ("blendMask",blendMask_)
                ("outputDisabled",outputDisabled_)
                ("overlapOpacity",overlapOpacity_)
                ("colorCorrection",colorCorrection_)
                ;
            _os << _map;
        }

        /// Read tuning from stream
        void Tuning::fromStream(QDataStream& _is) {
            PropertyMap _map;
            _is >> _map;
            _map.get("color",color_);
            _map.get("projector",projector_);
            _map.get("warpGrid",warpGrid_);
            _map.get("blendMask",blendMask_);
            _map.get("outputDisabled",outputDisabled_);
            _map.get("overlapOpacity",overlapOpacity_);
            _map.get("colorCorrection",colorCorrection_);
        }

        bool operator==(Tuning const& _lhs, Tuning const& _rhs)
        {
            return
                OMNI_TEST_MEMBER_EQUAL(color_) &&
                OMNI_TEST_MEMBER_EQUAL(projector_) &&
                OMNI_TEST_MEMBER_EQUAL(warpGrid_) &&
                OMNI_TEST_MEMBER_EQUAL(blendMask_) &&
                OMNI_TEST_MEMBER_EQUAL(outputDisabled_) &&
                OMNI_TEST_MEMBER_EQUAL(overlapOpacity_) &&
                OMNI_TEST_MEMBER_EQUAL(colorCorrection_);
        }
    }
}
