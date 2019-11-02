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

#ifndef OMNI_WARPPOINT_H_
#define OMNI_WARPPOINT_H_

#include <omni/serialization/Interface.h>
#include <QPointF>

namespace omni {
  /**@brief A Warp defines a bezier point with one position and two handles
     / *@detail Also stores flag if it is selected
   **/
  class OMNI_EXPORT WarpPoint {
    public:
      /// Default constructor
      WarpPoint();

      /// Constructor from point
      WarpPoint(
        QPointF const& _pos,
        QPointF const& _c1 = QPointF(0.0, 0.0),
        QPointF const& _c2 = QPointF(0.0, 0.0));

      /// Set new position
      void           setPos(QPointF const& _pos);

      /// Returns reference to position
      QPointF      & pos();

      /// Returns const reference to position
      QPointF const& pos() const;

      /// Return x coordinate of position
      float          x() const;

      /// Return y coordinate of position
      float          y() const;

      /// Set first bezier handle
      void           setC1(QPointF const& _c1);

      /// Returns reference to first bezier handle
      QPointF      & c1();

      /// Returns const reference to first bezier handle
      QPointF const& c1() const;

      /// Set second bezier handle
      void           setC2(QPointF const& _c2);

      /// Returns reference to second bezier handle
      QPointF      & c2();

      /// Returns const reference to second bezier handle
      QPointF const& c2() const;

      /// Set selection flag
      void           setSelected(bool);

      /// Return selection flag
      bool           selected() const;

      /// Test for equality
      friend bool    operator==(WarpPoint const&,
                                WarpPoint const&);

      /// Write WarpPoint to stream
      void toStream(QDataStream&) const;

      /// Read WarpPoint from stream
      void fromStream(QDataStream&);

    private:
      QPointF pos_, c1_, c2_;
      bool    selected_ = false;
  };
}

OMNI_DECL_STREAM_OPERATORS(omni::WarpPoint)

#endif /* OMNI_WARPPOINT_H_ */
