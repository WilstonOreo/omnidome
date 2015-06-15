#ifndef OMNI_UI_GLDISPLAY_H_
#define OMNI_UI_GLDISPLAY_H_ 

#include <QOpenGLWidget>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>

#include <glib.h>
#include <gst/gst.h>
#include <gst/app/gstappsink.h>

struct RtpReceiver;

namespace omni
{  
  namespace ui
  {
    /// A simple test widget for displaying a texture
    class GLDisplay : 
      public QOpenGLWidget,
      protected QOpenGLFunctions
    {
      Q_OBJECT
    public:
      explicit GLDisplay(QWidget* _parent = nullptr);
      ~GLDisplay();
 
      void setPort(int _port);
      int port() const;

    protected:
      void initializeGL();
      void resizeGL(int _w, int _h);    
      void paintGL();

    private:
      static void gstCallback(RtpReceiver* r, unsigned char* data, size_t size, gboolean eos, void* user_data );

      RtpReceiver* rtpReceiver_ = nullptr;
      GLuint texId_;
      int port_;
    };
  }
}

#endif /* OMNI_UI_GLDISPLAY_H_ */
