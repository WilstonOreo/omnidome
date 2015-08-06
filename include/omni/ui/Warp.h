#ifndef OMNI_UI_WARP_H_
#define OMNI_UI_WARP_H_

#include <memory>
#include <QGroupBox>

namespace omni
{
  class Session;
  class WarpGrid;
  
  namespace ui
  {
    namespace Ui
    {
      class Warp;
    }

    class Warp : public QGroupBox
    {
      Q_OBJECT
    public:
      Warp(QWidget* = nullptr);
      ~Warp();

      Session const* session() const;
      void setSession(Session* _session);

    public slots:
      void resetWarpGrid();
      void resizeWarpGrid(bool);
      void resizeWarpGrid();
      void updateWarpGrid();

    signals:
      void warpGridChanged();

    private:
      omni::WarpGrid const* warpGrid() const;
      omni::WarpGrid* warpGrid();

      bool locked_ = false;
      Session* session_ = nullptr;
      std::unique_ptr<Ui::Warp> ui_;
    };
  }
}

#endif /* OMNI_UI_WARP_H_ */
