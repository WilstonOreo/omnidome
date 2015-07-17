#include <omni/mapping/Interface.h>

#include <omni/input/List.h>
#include <omni/util.h>

#include <QOpenGLShaderProgram>


namespace omni
{
  namespace mapping
  {
    Interface::Interface()
    {
    }

    Interface::~Interface()
    {
    }

    void Interface::initialize()
    {
      /// Make shader
      shader_.reset(new QOpenGLShaderProgram());
      shader_->addShaderFromSourceCode(QOpenGLShader::Vertex,util::fileToStr(vertexShaderFile()));
      shader_->addShaderFromSourceCode(QOpenGLShader::Fragment,util::fileToStr(fragmentShaderFile()));
      shader_->link();
    }

    void Interface::bind()
    {
      if (!shader_) initialize();

      if (shader_)
        shader_->bind();
    }

    void Interface::release()
    {
      if (shader_)
        shader_->release();
    }
 
    void Interface::fromStream(QDataStream&)
    {
    }

    void Interface::toStream(QDataStream&) const
    {
    }
 
    QString Interface::vertexShaderFile() const
    {
      return ":/shader/common.vert";
    }

    QString Interface::fragmentShaderFile() const
    {
      return QString(":/shader/") + getTypeId().str() + ".vert";
    }
  }
}
