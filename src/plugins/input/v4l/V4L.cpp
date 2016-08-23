#include "V4L.h"

#include <QOpenGLContext>
#include <QOpenGLPixelTransferOptions>
#include <QDebug>
#include <QErrorMessage>
#include <QInputDialog>

#include <omni/visual/util.h>

#include <libv4l2.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <libudev.h>


namespace omni {
  namespace input {
    V4L::V4L(Interface const *_parent) :
      Interface(_parent),
      size_(0,0)
    {
      qRegisterMetaType<V4LFrame>("V4LFrame");
    }

    V4L::~V4L() {
    }

    QStringList V4L::GetDevices() {

      struct udev *udev;
      struct udev_enumerate *enumerate;
      struct udev_list_entry *devices, *dev_list_entry;
      struct udev_device *dev;

      QStringList deviceList;

      udev = udev_new();
      if (!udev)
          return deviceList;

      enumerate = udev_enumerate_new(udev);
      udev_enumerate_add_match_subsystem(enumerate, "video4linux");
      udev_enumerate_scan_devices(enumerate);
      devices = udev_enumerate_get_list_entry(enumerate);

      udev_list_entry_foreach(dev_list_entry, devices) {

        const char *path;
        const char *devNode;

        path = udev_list_entry_get_name(dev_list_entry);
        dev = udev_device_new_from_syspath(udev, path);

        devNode = udev_device_get_devnode(dev);

        deviceList.append(devNode);

        udev_device_unref(dev);
      }

      udev_enumerate_unref(enumerate);
      udev_unref(udev);

      return deviceList;
    }

    void V4L::update() {
    }

    GLuint V4L::textureId() const {
      return !texture_ ? 0 : texture_->textureId();
    }


    bool V4L::load(QString const& _path) {
      qDebug() << "V4L::load";

      device_.reset(new V4LDevice(_path));
      device_->open();

      connect(device_.get(), SIGNAL(frameArrived(V4LFrame)), this, SLOT(onFrameArrived(V4LFrame)));

      return true;
    }

    QSize V4L::size() const {
      return size_;
    }

    void V4L::toPropertyMap(PropertyMap& _map) const {
      qDebug() << "V4L::toPropertyMap";
    }

    void V4L::fromPropertyMap(PropertyMap const& _map) {
      qDebug() << "V4L::fromPropertyMap";
    }

    bool V4L::canAdd() {

      QInputDialog* inputDialog = new QInputDialog();
      inputDialog->setOptions(QInputDialog::NoButtons);

      bool ok;

      QStringList devices = GetDevices();

      QString device = inputDialog->getItem(NULL, "Input Device", "Device", devices, 0, false, &ok);

      qDebug() << "Selected input " << device;

      if(!this->load(device)) {
        QErrorMessage _errorMessageBox(widget());
        _errorMessageBox.showMessage("Error opening " +  device);
        return false;
      }

      return true;
    }

    void V4L::onFrameArrived(const V4LFrame& frame) {

      if (!QOpenGLContext::currentContext()) return;

      //qDebug() << "V4L::frameArrived";

      size_t width  = frame.rgbFrame->width;
      size_t height = frame.rgbFrame->height;

      size_ = QSize(width, height);

      QImage image(frame.rgbFrame->data[0], width, height, QImage::Format_RGB888);

      //image.fill(Qt::green);

      texture_.reset(new QOpenGLTexture(QOpenGLTexture::TargetRectangle));
      texture_->setData(image.mirrored());
      texture_->setMinMagFilters(
        QOpenGLTexture::Linear,
        QOpenGLTexture::Linear);
      texture_->allocateStorage();

      this->triggerUpdateCallbacks();

      device_->releaseFrame(frame);
    }
  }
}
