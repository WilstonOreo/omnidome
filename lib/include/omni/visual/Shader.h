/* Copyright (c) 2014-2016 "Omnidome" by Michael Winkelmann
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
#ifndef OMNI_VISUAL_SHADER_H_
#define OMNI_VISUAL_SHADER_H_

#include <functional>
#include <memory>
#include <set>
#include <omni/visual/ContextBoundPtr.h>
#include <omni/visual/UniformHandler.h>

namespace omni {
  namespace visual {
      /// Use shader with and do uniform assignment and drawing inside functor
      void OMNI_EXPORT useShader(QOpenGLShaderProgram& _s, std::function<void(UniformHandler&)> f);

      /// Initialize shader: load from file and compile.
      void OMNI_EXPORT initShader(QOpenGLShaderProgram& _s, const char* _filename);

      /// Initialize shader: load from file and compile.
      void OMNI_EXPORT initShader(std::unique_ptr<QOpenGLShaderProgram>& _s, const char* _filename);

      /// Initialize shader: load from file and compile.
      void OMNI_EXPORT initShader(ContextBoundPtr<QOpenGLShaderProgram>& _s, const char* _filename);

      /**@brief Adds #include functionality to shader source code
         @detail Compiles source code file with included files or
                 compiles a shader
       **/
      class OMNI_EXPORT ShaderCompiler {
      public:

        static QString compile(QString const& _sourceFile);

      private:

        static QString compileRecursive(QString const& _sourceFile, std::set<QString>& _includedFiles);

        /// Parses filename from the #include macro
        static QString parseIncludeLine(QString const& _baseDir, QString const& _line);
      };

  }
}

#endif /* OMNI_VISUAL_SHADER_H_ */
