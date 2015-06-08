#include <omni/mapping/Interface.h>

#include <omni/input/List.h>
#include <omni/util.h>

#include <QOpenGLShaderProgram>


namespace omni
{
  namespace mapping
  {
    Interface::Interface(input::List const& _inputList) :
      inputList_(_inputList)
    {
    }

    Interface::~Interface()
    {
    }

    void Interface::initializeShader()
    {
      /// Make shader
      shader_.reset(new QOpenGLShaderProgram());
      shader_->addShaderFromSourceCode(QOpenGLShader::Vertex,util::fileToStr(vertexShaderFile()));
      shader_->addShaderFromSourceCode(QOpenGLShader::Fragment,util::fileToStr(fragmentShaderFile()));
      shader_->link();
    }

    void Interface::bindShader()
    {
      shader_->bind();
      // Uniforms for all textures
      for (auto& _input : inputs_)
      {
        auto _inputUniformId = _input.first.str().toStdString().c_str();;
        auto& _inputId = _input.second;
        this->shader_->setUniformValue(_inputUniformId,input(_inputId)->textureId());
      }
    }

    void Interface::releaseShader()
    {
      shader_->release();
    }
 
    bool Interface::hasSingleInput() const
    {
      return inputIDs().size() == 1;
    }

    void Interface::addInput(Id const& _id, Id const& _input)
    {
      auto _inputIDs = inputIDs();
      // Check if id is in set of allowed ids or if set of allowed is empty,
      // any can be accepted 
      if (!_inputIDs.count(_id) && !_inputIDs.empty())
      {   
        return;
      }
      inputs_[_id] = _input;
    }

    void Interface::removeInput(Id const& _id)
    {
      inputs_.erase(_id);
    }

    input::Interface const* Interface::input(Id const& _id) const
    {
      if (inputs_.count(_id) == 0) return nullptr;
      return inputList_[_id];
    }
    
    void Interface::fromStream(QDataStream&)
    {
      inputs_.clear();
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
