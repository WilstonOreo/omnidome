#include "Blackmagic.h"

#include <QOpenGLFunctions>
#include <omni/visual/Shader.h>
#include <omni/visual/Rectangle.h>
#include <omni/visual/util.h>
#include "BlackmagicWidget.h"
#include "BlackmagicCapture.h"

namespace omni
{
  namespace input
  {
    ContextBoundPtr<QOpenGLShaderProgram> Blackmagic::shader_;

    Blackmagic::Blackmagic() : capture_(new BlackmagicCapture(this))
    {
      connect(capture_,&BlackmagicCapture::updated,[&]()
      {
        this->update();
        this->triggerUpdateCallbacks();
      });
    }

    Blackmagic::~Blackmagic()
    {
    }

    void Blackmagic::activate()
    {
      capture_->start();
    }

    void Blackmagic::update()
    {
      if (!capture_->isCapturing()) return;

      using namespace visual;

      if (capture_->size() != size() || !shader_)
      {
        primaryContextSwitch([&](QOpenGLFunctions& _)
        {
          setupFramebuffer(capture_->size());
          initShader(shader_,"YUV2RGB");
        });
      }

      if (!framebuffer() || !framebuffer()->isValid()) return;

      draw_on_framebuffer(framebuffer(),
                          [&](QOpenGLFunctions& _) // Projection Operation
      {
        QMatrix4x4 _m;
        _m.ortho(-0.5, 0.5, -0.5, 0.5, -1.0, 1.0);
        glMultMatrixf(_m.constData());
      }, [&](QOpenGLFunctions& _) // Model View Operation
      {
        useShader(*shader_,[&](UniformHandler& _h)
        {
          _h.uniform("texture_size", QVector2D(size().width(),size().height()));
          _h.texUniform("texture", capture_->textureId());
          Rectangle::draw();
        });
      });
    }

    void Blackmagic::deactivate()
    {
      capture_->stop();
    }

    void Blackmagic::setup(IDeckLink* device, IDeckLinkDisplayMode* mode)
    {
        capture_->setup(device,mode);
    }

    /// Serialize image path to stream
    void     Blackmagic::toPropertyMap(PropertyMap& _map) const
    {
      Framebuffer::toPropertyMap(_map);
    }

    /// Deserialize from stream and load image
    void     Blackmagic::fromPropertyMap(PropertyMap const& _map)
    {
      Framebuffer::fromPropertyMap(_map);
    }

    QWidget* Blackmagic::widget()
    {
      return new omni::ui::input::Blackmagic(this);
    }
  }
}
