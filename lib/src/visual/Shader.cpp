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

#include <omni/visual/Shader.h>

#include <omni/util.h>
#include <QFile>

namespace omni {
  namespace visual {
    void initShader(QOpenGLShaderProgram& _s, const char *_filename) {
      QString _vertSrc =
        ShaderCompiler::compile(":/shaders/" + QString(_filename) + ".vert");
      QString _fragmentSrc =
        ShaderCompiler::compile(":/shaders/" + QString(_filename) + ".frag");

      _s.addShaderFromSourceCode(QOpenGLShader::Vertex,
                                 _vertSrc);
      _s.addShaderFromSourceCode(QOpenGLShader::Fragment,
                                 _fragmentSrc);
      _s.link();
    }

    void initShader(std::unique_ptr<QOpenGLShaderProgram>& _s,
                    const char *_filename) {
      /// Dont do anything if shader is already allocated
      if (!!_s) return;

      _s.reset(new QOpenGLShaderProgram());
      initShader(*_s, _filename);
    }

    void initShader(ContextBoundPtr<QOpenGLShaderProgram>& _s,
                    const char *_filename) {
      /// Dont do anything if shader is already allocated
      if (!!_s) return;

      if (_s.reset(new QOpenGLShaderProgram())) {
        initShader(*_s, _filename);
      }
    }

    void useShader(QOpenGLShaderProgram& _s,
                   std::function<void(UniformHandler&)>f) {
      withCurrentContext([&](QOpenGLFunctions& _gl) {
        _s.bind();
        {
          UniformHandler _handler(_gl, _s);
          f(_handler);

          // Destructor of handler is called here implicitly
          // by RAII to unbind all textures
        }
        _s.release();
      });
    }


    QString ShaderCompiler::compile(QString const& _sourceFile) {
      std::set<QString> _includedFiles;
      return compileRecursive(_sourceFile,_includedFiles);
    }

    QString ShaderCompiler::compileRecursive(QString const& _sourceFile, std::set<QString>& _includedFiles) {
      if (_includedFiles.count(_sourceFile) > 0) return QString();

      QFile _f(_sourceFile);
      _includedFiles.insert(_sourceFile);

      QString _outputFile;

      if (_f.open(QIODevice::ReadOnly))
      {
         QTextStream in(&_f);
         while (!in.atEnd())
         {
            QString _line = in.readLine().trimmed();
            if (_line.startsWith("#include")) {
              // Handle include
              auto _includedFilename = parseIncludeLine("",_line);
              _outputFile += compileRecursive(_includedFilename,_includedFiles);
            } else {
              _outputFile += _line + '\n';
            }
         }
         _f.close();
      }
      return _outputFile;
    }

    QString ShaderCompiler::parseIncludeLine(QString const& _baseDir, QString const& _line) {
      QString _file = _line;
      _file = _file.remove("#include").trimmed();
      if (_file.isEmpty()) return QString();

      int _separatorBegin = 0;
      for (; _separatorBegin < _file.length(); ++_separatorBegin) {
        if (_file[_separatorBegin] == '"' || _file[_separatorBegin] == '<') break;
      }
      int _separatorEnd = _separatorBegin + 1;
      bool _haveSep = false;

      for (; _separatorEnd < _file.length(); ++_separatorEnd) {
        auto _sep = _file[_separatorBegin];
        if ((_sep == '"' && _file[_separatorEnd] == '"') ||
            (_sep == '<' && _file[_separatorEnd] == '>')) {
              _haveSep = true;
              break;
            }
      }


      if (!_haveSep) return QString();
      _separatorBegin += 1;
      _file = _file.remove(0,_separatorBegin);
      _file.truncate(_separatorEnd - _separatorBegin);
      return _file.trimmed();
    }
  }
}
