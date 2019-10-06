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

#ifndef OMNI_MAPPING_FISHEYE_H_
#define OMNI_MAPPING_FISHEYE_H_

#include <omni/mapping/Interface.h>

namespace omni
{
  namespace mapping
  {
    class Fisheye : public QObject, public Mapping
    {
      Q_OBJECT
      Q_PLUGIN_METADATA(IID OMNI_MAPPING_INTERFACE_IID)
      Q_INTERFACES(omni::mapping::Interface)
      OMNI_PLUGIN_INFO("Fisheye mapping","Copyright (C) 2016")
    public:
      OMNI_REGISTER_CLASS(Factory,Fisheye)

      Fisheye();
      ~Fisheye() final = default;

      void bind() override;

      qreal stretch() const;
      void setStretch(qreal _stretch);

      void fromPropertyMap(PropertyMap const&) override;
      void toPropertyMap(PropertyMap&) const override;

      QWidget* widget() override;

    private:
      qreal stretch_ = 0.0;
    };
  }
}

#endif /* OMNI_MAPPING_FISHEYE_H_ */
