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
      shader_->addShaderFromSourceCode(QOpenGLShader::Vertex,vertexShaderSourceCode());
      shader_->addShaderFromSourceCode(QOpenGLShader::Fragment,fragmentShaderSourceCode());
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
 
    QString Interface::vertexShaderSourceCode() const
    {
      return util::fileToStr(":/shaders/mapping/common.vert");
    }

    QString Interface::fragmentShaderSourceCode() const
    {
      return util::fileToStr(QString(":/shaders/mapping/") + getTypeId().str() + ".frag");
    }
  }
}
