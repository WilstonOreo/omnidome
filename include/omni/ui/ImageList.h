#ifndef OMNI_UI_IMAGELIST_H_
#define OMNI_UI_IMAGELIST_H_

#include <memory> 
#include <QTreeView>
#include <QStandardItemModel>
#include <QItemDelegate>
#include <QComboBox>
#include <omni/MapMode.h>
#include <omni/canvas/Interface.h>

  
namespace omni
{  
  namespace ui
  {

    class ImageList : public QTreeView
    {
      Q_OBJECT
    public:
      ImageList(QWidget* _parent);

      typedef std::pair<std::string,MapMode> filename_mapmode_type;

      void setup(Session*);

      /// Returns filename of selected image
      filename_mapmode_type getSelected() const;

      void addImage(std::string const& _filename);
      void addImage(std::string const& _filename,MapMode _mapMode);
      void removeSelection();
  
    private slots:
      void setMapMode(QWidget* _editor);
    private:
      Session* session_;
      void prepareModel();

      void addItem(
          const std::string& _file,
          size_t _width, size_t _height, 
          MapMode _mapMode);

      static std::string basename(std::string const&);


      std::vector<filename_mapmode_type> items_;

      std::unique_ptr<MapModeEditor> mapModeEditor_;
      std::unique_ptr<QStandardItemModel> model_;
    };
  }
}

#endif /* OMNI_UI_IMAGELIST_H_ */
