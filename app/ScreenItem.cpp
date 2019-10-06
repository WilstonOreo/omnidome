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

#include "ScreenItem.h"

#include <QPainter>
#include "ScreenSetup.h"
#include "TuningGLView.h"

namespace omni {
  namespace ui {
    SubScreenItem::SubScreenItem()
    {}

    SubScreenItem::SubScreenItem(ScreenItem* _parent, int _index) :
      parent_(_parent),
      index_(_index)
    {}

    /// Pointer to parent screen
    ScreenItem * SubScreenItem::parent() {
      return parent_;
    }

    /// Const pointer to parent screen
    ScreenItem const * SubScreenItem::parent() const {
      return parent_;
    }

    QRect SubScreenItem::rect() const
    {
      auto _r     = parent_->rect(); // Parent rect
      int  _width = _r.width() / omni::proj::ScreenSetup::subScreenCountForScreen(
        parent()->screen());

      return QRect(_r.x() + index_ * _width, _r.y(), _width, _r.height());
    }

    QSize SubScreenItem::size() const
    {
      auto _screen = parent()->screen();

      return QSize(
        omni::proj::ScreenSetup::subScreenWidthForScreen(_screen),
        _screen->size().height());
    }

    void SubScreenItem::paint(bool _hover,
                              bool _drop,
                              QColor _dropColor,
                              QPainter& _p)
    {
      // Draw rectangle with tuning color
      QColor _color = tuningWidget() !=
                      nullptr ? tuningWidget()->tuning()->color() : "#cccccc";

      if (_hover)
      {
        _color = _color.lighter(120);
      }

      _p.setBrush(_color);
      _p.setPen(QPen(_color.darker(200), 1));

      auto _rect = rect();
      _p.drawRect(_rect);

      /// Draw diagonal stripes for drop
      if (_drop && _hover)
      {
        QPixmap _pixmap(32, 32);
        _pixmap.fill(_color);
        QPainter _texP(&_pixmap);
        _texP.setPen(QPen(_dropColor, 2));

        for (int i = -32; i <= 64; i += 8)
        {
          _texP.drawLine(i + 32, i - 32, i - 32, i + 32);
        }
        _texP.end();
        QBrush _brush;
        _brush.setTexture(_pixmap);
        _p.setBrush(_brush);
        _p.drawRect(_rect);
      }

      // Draw resolution text
      _p.setBrush(Qt::NoBrush);
      _p.setPen(QPen(_color.darker(500), 1));

      // Resolution information string
      QString _resStr =
        QString("%1 x %2").arg(size().width()).arg(size().height());

      _p.drawText(_rect, Qt::AlignHCenter | Qt::AlignVCenter, _resStr);
    }

    omni::ui::proj::Tuning * SubScreenItem::tuningWidget()
    {
      return tuningWidget_;
    }

    omni::ui::proj::Tuning const * SubScreenItem::tuningWidget() const
    {
      return tuningWidget_;
    }

    void SubScreenItem::attachTuningWidget(omni::ui::proj::Tuning *_tuningWidget)
    {
      _tuningWidget->attachScreen(parent()->screen(), index_);
      parent()->fullscreen_->attachTuning(_tuningWidget->tuning());
      tuningWidget_ = _tuningWidget;
      parent()->screenSetup_.update();
    }

    void SubScreenItem::detachTuningWidget() {
      if (!tuningWidget_) return;

      parent()->fullscreen_->detachTuning(tuningWidget_->tuning());
      tuningWidget()->detachScreen();
      tuningWidget_ = nullptr;
      parent()->screenSetup_.update();
    }

    /// ScreenSetup::Item
    /////////////////////////////////////////

    ScreenItem::ScreenItem(
      ScreenSetup& _screenSetup,
      QScreen const *_screen) :
      screenSetup_(_screenSetup),
      screen_(_screen),
      fullscreen_(new FullscreenTuningGLView(_screen))
    {
      int _numScreens = omni::proj::ScreenSetup::subScreenCountForScreen(_screen);

      for (int i = 0; i < _numScreens; ++i)
      {
        subScreens_.emplace_back(this, i);
      }
    }

    ScreenItem::~ScreenItem()
    {
    }

    void ScreenItem::detachTuningWidget(omni::ui::proj::Tuning* _tuningWidget) {
      for (auto& _subScreen : subScreens_) {
        if (_subScreen.tuningWidget() == _tuningWidget) {
          _subScreen.detachTuningWidget();
        }
      }
    }

    QScreen const * ScreenItem::screen() const
    {
      return screen_;
    }

    int ScreenItem::hoverIndex() const
    {
      return hoverIndex_;
    }

    void ScreenItem::setHoverIndex(int _hoverIndex)
    {
      if ((_hoverIndex < 0) || (_hoverIndex >= subScreens_.size())) {
        hoverIndex_ = -1;
        return;
      }
      hoverIndex_ = _hoverIndex;
    }

    /// Set hover index from mouse position
    void ScreenItem::setHoverIndex(QPoint const& _pos)
    {
      for (int i = 0; i < subScreens_.size(); ++i)
      {
        if (subScreens_[i].rect().contains(_pos))
        {
          setHoverIndex(i);
          return;
        }
      }
      setHoverIndex(-1);
    }

    bool ScreenItem::drop() const
    {
      return drop_;
    }

    void ScreenItem::startDrop(QColor const& _color)
    {
      drop_      = true;
      dropColor_ = _color;
      fullscreen_->showDragWidget(hoverIndex_, _color);
    }

    /// Hides fullscreen widget
    void ScreenItem::endDrop()
    {
      fullscreen_->hideDragWidget();
      drop_ = false;
    }

    QRect ScreenItem::rect() const
    {
      auto _desktopRect = omni::proj::ScreenSetup::desktopRect();

      return screenSetup_.transformedRect(screen_->geometry().translated(-
                                                                         _desktopRect
                                                                         .topLeft()))
             .toRect();
    }

    FullscreenTuningGLView*  ScreenItem::fullscreen() {
      return fullscreen_.get();
    }

    void ScreenItem::paint(QPainter& _p)
    {
      int i = 0;

      for (auto& _subScreen : subScreens_)
      {
        bool _hover = i == hoverIndex_;
        _subScreen.paint(
          _hover,
          drop_,
          dropColor_,
          _p);
        ++i;
      }

      if (subScreens_.size() > 1)
      {
        auto _rect = rect();

        // Resolution string
        QString _resStr = QString("%1 x %2").arg(screen_->size().width()).arg(
          screen_->size().height());
        _p.drawText(_rect, Qt::AlignHCenter, _resStr);
      }
    }

    void ScreenItem::detachTunings()
    {
      for (auto& _subScreen : subScreens_)
      {
        _subScreen.detachTuningWidget();
      }
    }

    SubScreenItem * ScreenItem::item(int _index) {
      return (_index < 0 || _index >= subScreens_.size()) ? nullptr : &subScreens_[_index];
    }

    SubScreenItem const * ScreenItem::item(int _index) const {
      return (_index < 0 || _index >= subScreens_.size()) ? nullptr : &subScreens_[_index];
    }

    /// Return pointer to currently hovered item
    SubScreenItem       * ScreenItem::hoveredItem() {
      return item(hoverIndex());
    }

    /// Return pointer to currently hovered item (const)
    SubScreenItem const * ScreenItem::hoveredItem() const {
      return item(hoverIndex());
    }
  }
}
