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

#include "GLDisplay.h"

#include <QMatrix4x4>
#include <QDebug>

#define DEFAULT_PORT 5000

typedef void (*RtpReceiverCallback) (RtpReceiver* r, unsigned char* data, size_t size, gboolean eos, void* user_data);

struct RtpReceiver {
	GstPipeline *pipeline;
	GstElement *udpsrc, *rtph264depay, *decodebin, *videoconvert;
  GstAppSink *appsink;
	RtpReceiverCallback cb;
  void* user_data;

  int width, height;

};

typedef struct _RtpDataInfo {
	unsigned char* data;
	gsize size;
} RtpDataInfo;

static RtpDataInfo buffer_to_data_info (GstBuffer * buf)
{
	RtpDataInfo info;
	GstMapInfo buf_info;
	unsigned char* data;

	if (!gst_buffer_map (buf, &buf_info, GST_MAP_READ)) {
		info.data = NULL;
		info.size = 0;
		return info;
	}

	data = (unsigned char*) malloc (buf_info.size);
	memcpy (data, buf_info.data, buf_info.size);

	gst_buffer_unmap (buf, &buf_info);

	info.data = data;
	info.size = buf_info.size;

	return info;
}

static void eos (GstAppSink *appsink, gpointer user_data)
{
	RtpReceiver* r = (RtpReceiver*) user_data;

	r->cb (r, NULL, 0, TRUE,r->user_data);
}

static GstFlowReturn new_preroll (GstAppSink *appsink, gpointer user_data)
{
	GstSample* sample = gst_app_sink_pull_preroll (appsink);

	g_return_val_if_fail (sample, GST_FLOW_ERROR);

	GstBuffer* buf = gst_sample_get_buffer (sample);
	RtpDataInfo info = buffer_to_data_info (buf);
	
  RtpReceiver* r = (RtpReceiver*) user_data;
	r->cb (r, info.data, info.size, FALSE, r->user_data);

	gst_sample_unref (sample);

	return GST_FLOW_OK;
}

static GstFlowReturn new_sample (GstAppSink *appsink, gpointer user_data)
{
	GstSample* sample = gst_app_sink_pull_sample (appsink);

	g_return_val_if_fail (sample, GST_FLOW_ERROR);

	GstBuffer* buf = gst_sample_get_buffer (sample);
	RtpDataInfo info = buffer_to_data_info (buf);

  RtpReceiver* r = (RtpReceiver*) user_data;
	r->cb (r, info.data, info.size, FALSE,r->user_data);

	gst_sample_unref (sample);
	return GST_FLOW_OK;
}


static void decodebin_callback(GstElement* element, GstPad* pad, gpointer data)
{
  RtpReceiver* state = (RtpReceiver*)data;

  if (!gst_element_link_many (
        element,
        state->videoconvert, 
        GST_ELEMENT(state->appsink), NULL)) {
		g_warning ("Failed to link decoder");
		return;
	}

  GstCaps* caps = gst_pad_get_current_caps(pad);
  g_message("%s",gst_caps_to_string(caps));

  GstStructure* s = gst_caps_get_structure(caps,0);

  gst_structure_get_int(s,"width",&state->width);
  gst_structure_get_int(s,"height",&state->height);
}


RtpReceiver* rtp_receiver_new(RtpReceiverCallback cb, void* user_data)
{
	GstCaps *caps;
	GstAppSinkCallbacks callbacks;
	RtpReceiver* state = g_slice_new0(RtpReceiver);
  
  g_message ("State Pre: %p", state);   

	state->cb = cb;
  state->user_data = user_data;

  state->width = 0;
  state->height = 0;
	state->pipeline = (GstPipeline*)gst_pipeline_new ("receiver" /* name of pipeline */);

	state->udpsrc = gst_element_factory_make ("udpsrc", NULL);
	state->rtph264depay = gst_element_factory_make ("rtph264depay", NULL);
	state->decodebin = gst_element_factory_make ("decodebin", NULL);
	{
    g_signal_connect(state->decodebin,"pad-added",G_CALLBACK(decodebin_callback),state);
  }
   
  state->videoconvert = gst_element_factory_make ("videoconvert", NULL);
	state->appsink = (GstAppSink*)gst_element_factory_make ("appsink", NULL);

	gst_bin_add_many (
    GST_BIN(state->pipeline),
		state->udpsrc, 
    state->rtph264depay, 
    state->decodebin, 
    state->videoconvert, 
    GST_ELEMENT(state->appsink), NULL);	

	g_object_set (state->udpsrc, "port", DEFAULT_PORT, NULL);

	GstCaps* udp_caps = gst_caps_new_empty_simple ("application/x-rtp");
  g_object_set (state->udpsrc, "caps", udp_caps, NULL);

	caps = gst_caps_new_simple ("video/x-raw",
	   "format", G_TYPE_STRING, "RGBA",
	   //"framerate", GST_TYPE_FRACTION, 25, 1,
	   //"width", G_TYPE_INT, 320,
	   //"height", G_TYPE_INT, 240,
	   NULL);
  

	gst_app_sink_set_caps (GST_APP_SINK (state->appsink), caps);

	if (!gst_element_link_many (
        state->udpsrc, 
        state->rtph264depay, 
        state->decodebin, NULL)) {
		g_warning ("Failed to link pipeline");
		return NULL;
	}


	callbacks.eos = eos;
	callbacks.new_preroll = new_preroll;
	callbacks.new_sample = new_sample;

	gst_app_sink_set_callbacks (state->appsink, &callbacks, state, NULL);

	gst_element_set_state (GST_ELEMENT(state->pipeline), GST_STATE_PLAYING);

  g_message ("Pipeline playing");

  g_message ("Videoconvert: %s", gst_element_get_name(state->videoconvert));

	gst_caps_unref (caps);

	return state;
}

void rtp_receiver_free(RtpReceiver *r)
{
	gst_element_set_state (GST_ELEMENT(r->pipeline), GST_STATE_NULL);
	gst_object_unref (r->pipeline);
	g_slice_free(RtpReceiver,r);
}

namespace omni
{
  namespace ui
  {
    GLDisplay::GLDisplay(QWidget* _parent) :
      QOpenGLWidget(_parent),
      port_(5000)
    {
      using namespace std::placeholders;
      gst_init(0,0);
      rtpReceiver_ = rtp_receiver_new(&GLDisplay::gstCallback,static_cast<void*>(this));
    }

    void GLDisplay::gstCallback(RtpReceiver* r, unsigned char* data, size_t size, gboolean eos, void* user_data)
    {
      qDebug() << "Got data: " << size;
      /// TODO data
    }

    GLDisplay::~GLDisplay()
    {
      if (rtpReceiver_)
        rtp_receiver_free(rtpReceiver_);
    }

    void GLDisplay::setPort(int _port)
    {
      port_=_port;
    }

    int GLDisplay::port() const
    {
      return port_;
    }

    void GLDisplay::initializeGL()
    {
      initializeOpenGLFunctions();
      glEnable(GL_TEXTURE_2D);

      QImage _image(QString(":/spherical.jpg"));

      _image = _image.mirrored();
      
      glGenTextures(1, &texId_);

      /// Setup our texture with RGBA format
      glBindTexture(GL_TEXTURE_2D, texId_);
      {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
            _image.width(), 
            _image.height(), 0, 
            GL_RGBA, GL_UNSIGNED_BYTE, _image.bits());
      }
      glBindTexture(GL_TEXTURE_2D, 0);
    }

    void GLDisplay::resizeGL(int _w, int _h)
    {
      _w = _w & ~1;
      _h = _h & ~1;
      glViewport(0, 0, (GLint)_w, (GLint)_h);
      glClearColor(0.0,0.0,0.0,1.0);
    }

    void GLDisplay::paintGL()
    {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glDisable( GL_CULL_FACE );

      /// Setup orthogonal projection
      glMatrixMode(GL_PROJECTION);
      {
        glLoadIdentity();
        QMatrix4x4 _m;
        _m.ortho(-0.5,0.5,-0.5,0.5,-1.0,1.0);
        glMultMatrixf(_m.constData());
      }

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

      // Draw a simple quad with our texture
      glBindTexture(GL_TEXTURE_2D, texId_);
      {
        glBegin(GL_QUADS);
        {
          glTexCoord2f(0.0f,0.0f);
          glVertex2f(-0.5f,-0.5f);
          glTexCoord2f(1.0f,0.0f);
          glVertex2f(0.5f,-0.5f);
          glTexCoord2f(1.0f,1.0f);
          glVertex2f(0.5f,0.5f);
          glTexCoord2f(0.0f,1.0f);
          glVertex2f(-0.5f,0.5f);
        }
        glEnd();
      }
    }
  }
}

