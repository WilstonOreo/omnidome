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

    void Interface::bind(OutputMode _outputMode)
    {
      if (!shader_) initialize();

      if (shader_)
      {
        shader_->bind();
        shader_->setUniformValue("flip_horizontal",flipHorizontal_);
        shader_->setUniformValue("flip_vertical",flipVertical_);

        switch (_outputMode)
        {
          case OutputMode::MAPPED_INPUT:
          shader_->setUniformValue("output_mode",0);
          break;
          case OutputMode::TEXCOORDS:
          shader_->setUniformValue("output_mode",1);
          break;
          case OutputMode::UVW:
          shader_->setUniformValue("output_mode",2);
          break;
        };
      }
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
      
    bool Interface::flipHorizontal() const
    {
      return flipHorizontal_;
    }

    void Interface::setFlipHorizontal(bool _flipHorizontal)
    {
      flipHorizontal_ = _flipHorizontal;
    }

    bool Interface::flipVertical() const
    {
      return flipVertical_;
    }

    void Interface::setFlipVertical(bool _flipVertical)
    {
      flipVertical_ = _flipVertical;
    }
      
    IdSet Interface::getUVWMappings()
    {
      IdSet _ids;
      for (auto& _mappingId : Factory::classes()) 
      {
        auto& _id = _mappingId.first;
        std::unique_ptr<Mapping> _mapping(Factory::create(_id));
        if (_mapping->isUVW()) 
          _ids.insert(_id);
      }
      return _ids;
    }
    
    IdSet Interface::getPlanarMappings()
    {
      IdSet _ids;
      for (auto& _mappingId : Factory::classes()) 
      {
        auto& _id = _mappingId.first;
        std::unique_ptr<Mapping> _mapping(Factory::create(_id));
        if (!_mapping->isUVW()) 
          _ids.insert(_id);
      }
      return _ids;
    }

    QString Interface::vertexShaderSourceCode() const
    {
      return util::fileToStr(":/shaders/mapping/common.vert");
    }

    QString Interface::fragmentShaderSourceCode() const
    {
      return 
        util::fileToStr(":/shaders/mapping/Template.frag") + 
        util::fileToStr(QString(":/shaders/mapping/") + getTypeId().str() + ".frag");
    }
  }
}
