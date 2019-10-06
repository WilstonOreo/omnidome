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

#ifndef OMNI_CANVAS_BOX_H_
#define OMNI_CANVAS_BOX_H_

#include <QObject>
#include <QtPlugin>
#include <omni/canvas/Envelope.h>
#include <omni/visual/Box.h>

namespace omni
{
  namespace canvas
  {
    /**@brief A box-shaped canvas.
      *@detail Can be used for rooms or boxes
     **/
    class Box : public QObject, public Envelope
    {
      Q_OBJECT
      Q_INTERFACES(omni::canvas::Interface)
      Q_PLUGIN_METADATA(IID OMNI_CANVAS_INTERFACE_IID)
      OMNI_PLUGIN_INFO("Box canvas","Copyright (C) 2017")
    public:
      OMNI_REGISTER_CLASS(Factory,Box)

      /// Default constructor
      Box(QObject* = nullptr);

      /// Destructor
      ~Box() final = default;

      /// Draw box
      void draw() const override;

      /// Generate box vertices
      void update() override;

      /// Return size of the box
      QVector3D size() const;

      /// Center point on the bottom
      QVector3D center() const override;

      /// Set size of the box
      void setSize(QVector3D const&);

      /// Deserialize from stream
      void fromPropertyMap(PropertyMap const&) override;

      /// Serialize to stream
      void toPropertyMap(PropertyMap&) const override;

      /// Make new box parameter widget
      ui::CanvasParameters* widget() override;

      QMatrix4x4 matrix() const override;
    signals:
      void sizeChanged();

    private:
      visual::Box vizBox_;
    };
  }
}

#endif /* OMNI_CANVAS_BOX_H_ */
