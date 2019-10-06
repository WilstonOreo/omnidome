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

#include <QObject>
#include <omni/mapping/Interface.h>

namespace omni {
  namespace mapping {
    class Cylindrical : public QObject, public Interface {
      Q_OBJECT
      Q_PLUGIN_METADATA(IID OMNI_MAPPING_INTERFACE_IID)
      Q_INTERFACES(omni::mapping::Interface)
      OMNI_PLUGIN_INFO("Cylindrical mapping", "Copyright (C) 2016")
    public:
        OMNI_REGISTER_CLASS(Factory, Cylindrical)

        Cylindrical();
        ~Cylindrical() final = default;

        void bind() override;

        /// Value between 0.0 and 1.0 for top strip position
        qreal beginAngle() const;

        /// Top border position of strip. Value is clamped between 0.0 and 1.0
        void setBeginAngle(qreal);

        /// Value between 0.0 and 1.0 to for bottom strip position
        qreal endAngle() const;

        /// Bottom border position of strip. Value is clamped between 0.0 and 1.0
        void setEndAngle(qreal);

        void toPropertyMap(PropertyMap& _map) const override;
        void fromPropertyMap(PropertyMap const& _map) override;

        QWidget* widget() override;
    private:
        void validate();
        qreal beginAngle_ = 0.0, endAngle_ = 1.0;
    };

  }
}
