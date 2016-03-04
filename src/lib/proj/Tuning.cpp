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

#include <omni/proj/Tuning.h>

#include <omni/util.h>
#include <omni/proj/Setup.h>
#include <omni/proj/ScreenSetup.h>
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

        bool Tuning::hasScreen() const
        {
            qDebug() << "Tuning::hasScreen " << (projector_.screen() != ScreenSetup::standardScreen());
            return projector_.screen() != ScreenSetup::standardScreen();
        }

        bool Tuning::resolutionToBeChanged(proj::Screen const *_screen) const
        {
            return false;
        }

        int Tuning::width() const
        {
            return !projector_.screen() ?
                   ScreenSetup::subScreenWidth(ScreenSetup::standardScreen()) :
                   ScreenSetup::subScreenWidth(projector_.screen());
        }

        int Tuning::height() const
        {
            return !projector_.screen() ?
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

        bool operator==(Tuning const& _lhs, Tuning const& _rhs)
        {
            return
                OMNI_TEST_MEMBER_EQUAL(color_) &&
                OMNI_TEST_MEMBER_EQUAL(projector_) &&
                OMNI_TEST_PTR_MEMBER_EQUAL(projectorSetup_) &&
                OMNI_TEST_MEMBER_EQUAL(warpGrid_) &&
                OMNI_TEST_MEMBER_EQUAL(blendMask_) &&
                OMNI_TEST_MEMBER_EQUAL(outputDisabled_) &&
                OMNI_TEST_MEMBER_EQUAL(colorCorrection_);
        }
    }
}

QDataStream& operator>>(QDataStream& _stream, omni::proj::Tuning& _tuning)
{
    using namespace omni::util;

    QColor _color;
    _stream >> _color;
    _tuning.setColor(_color);
    _stream >> _tuning.projector();
    _stream >> _tuning.warpGrid();
    _stream >> _tuning.blendMask();

    return _stream;
}

QDataStream& operator<<(QDataStream& _stream, omni::proj::Tuning const& _tuning)
{
    using namespace omni::util;

    _stream << _tuning.color();
    _stream << _tuning.projector();
    _stream << _tuning.warpGrid();
    _stream << _tuning.blendMask();
    return _stream;
}
