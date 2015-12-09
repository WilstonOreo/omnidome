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

#ifndef OMNI_UI_CANVAS_H_
#define OMNI_UI_CANVAS_H_

#include <memory>
#include <omni/ui/CollapsibleGroupBox.h>
#include <omni/visual/ProjectorViewMode.h>
#include <omni/ui/SessionWidget.h>

namespace omni
{
        class Session;

        namespace ui
        {
                namespace Ui
                {
                    class Canvas;
                }

                class Canvas :
                    public CollapsibleGroupBox,
                    public SessionWidget
                {
                        Q_OBJECT
                    public:
                        Canvas(QWidget* = nullptr);
                        ~Canvas();

                    signals:
                        void canvasTypeChanged();
                        void canvasChanged();
                        void displayInputToggled(bool);
                        void projectorViewModeChanged(ProjectorViewMode);

                    public slots:
                        void canvasTypeSelected(QString);
                        void sessionParameters();
                    private slots:
                        void changeProjectorViewMode(int);

                    private:
                        std::unique_ptr<Ui::Canvas> ui_;
                };
        }
}

#endif /* OMNI_UI_CANVAS_H_ */
