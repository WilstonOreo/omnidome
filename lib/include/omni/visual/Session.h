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

#ifndef OMNI_VISUAL_SESSION_H_
#define OMNI_VISUAL_SESSION_H_

#include <list>
#include <omni/Session.h>
#include <omni/visual/ProjectorViewMode.h>
#include <omni/visual/Projector.h>
#include <omni/visual/WarpGrid.h>


namespace omni {
  namespace visual {
    /// Base class for drawing a session
    class OMNI_EXPORT Session {
      public:
        /// Construct with reference to session
        Session(omni::Session const&);

        /// Return const ref to session
        omni::Session const& session() const;

        /// Draw canvas with given output mode and optional grayscale toggle
        void                 drawCanvas(
          mapping::OutputMode = mapping::OutputMode::MAPPED_INPUT,
          bool _grayscale = false) const;


        void                 drawCanvasWireframe() const;

        /// Draw canvas for scene view
        void drawCanvasFor3DView();

        /// Canvas with frustum intersection of all projectors
        void drawCanvasWithFrustumIntersections(ProjectorSelectionMode) const;

        /// Draw frustum canvas intersection
        void drawFrustumIntersection(proj::Projector const &,
                                     QColor const&) const;

        /// Update projectors and canvas
        void update();

        /**@brief Draw projectors.
           @detail If _selectedOnly true, only currently selected projector is drawn
         **/
        void drawProjectors(ProjectorSelectionMode = ProjectorSelectionMode::ALL) const;

        /**@brief Draw projector halos.
           @detail If _selectedOnly true, only currently selected projector is drawn
         **/
        void drawProjectorHalos(ProjectorSelectionMode = ProjectorSelectionMode::ALL) const;

        /// Returns true if an update is required
        bool needsUpdate() const;

      private:
        /// Draw canvas transform with matrix
        void drawCanvasWithMatrix() const;

        template<typename F>
        void projectorDrawFunction(ProjectorSelectionMode, F f) const;

        omni::Session const& session_;
        static ContextBoundPtr<QOpenGLShaderProgram> frustumShader_;

        std::list<visual::Projector> projectors_;
        bool needsUpdate_ = true;
    };
  }
}

#endif /* OMNI_VISUAL_SESSION_H_ */
