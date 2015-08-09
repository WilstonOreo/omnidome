#include <omni/ui/ImageList.h>

#include <QComboBox>
#include <omni/Session.h>

using namespace omni::ui;


MapModeEditor::MapModeEditor(
  QObject *_parent,
  Canvas const* _canvas) :
  QItemDelegate(_parent),
  canvas_(_canvas)
{
}

QWidget *MapModeEditor::createEditor(QWidget *parent,
                                     const QStyleOptionViewItem &/* option */,
                                     const QModelIndex &/* index */) const
{
  QComboBox* _editor = new QComboBox(parent);
  auto&& _modes = canvas_->supportedMapModes();
  for (auto& _mode : _modes)
    _editor->addItem(mapModeToQString(_mode));
  return _editor;
}

void MapModeEditor::setEditorData(QWidget* _editor, const QModelIndex& index) const
{
  QString value = index.model()->data(index, Qt::EditRole).toString();
  QComboBox *cBox = static_cast<QComboBox*>(_editor);
  cBox->setCurrentIndex(cBox->findText(value));
}

void MapModeEditor::setModelData(QWidget *_editor, QAbstractItemModel *model,
                                 const QModelIndex& index) const
{
  QComboBox *cBox = static_cast<QComboBox*>(_editor);
  QString value = cBox->currentText();
  model->setData(index, value, Qt::EditRole);
}

void MapModeEditor::updateEditorGeometry(
  QWidget *_editor,
  const QStyleOptionViewItem &option,
  const QModelIndex &/* index */) const
{
  _editor->setGeometry(option.rect);
}

QString MapModeEditor::mapModeToQString(MapMode _mapMode)
{
  return QString().fromStdString(mapModeStr(_mapMode));
}

ImageList::ImageList(QWidget* _parent) :
  QTreeView(_parent),
  session_(nullptr),
  mapModeEditor_(nullptr),
  model_(nullptr)
{
  prepareModel();
}

void ImageList::setMapMode(QWidget* _editor)
{
  if (!session_) return;
  QComboBox *cBox = static_cast<QComboBox*>(_editor);
  int _row = currentIndex().row()-1;
  if (_row < 0 || _row >= items_.size()) return;
  auto&& _mapModes = session_->canvas()->supportedMapModes();

  auto it = _mapModes.begin();
  std::advance(it,cBox->currentIndex());
  items_[_row].second = *it;
  session_->setMapMode(items_[_row].first,items_[_row].second);
  getSelected();
}

void ImageList::addImage(std::string const& _filename)
{
  auto&& _mapModes = session_->canvas()->supportedMapModes();
  MapMode _mapMode = MapMode::NONE;
  if (!_mapModes.empty())
    _mapMode = *_mapModes.begin();

  addImage(_filename,_mapMode);
}

void ImageList::addImage(std::string const& _filename,MapMode _mapMode)
{
  if (!session_) return;
  auto _image = session_->addImage(_filename,_mapMode);
  if (!_image) return;
  addItem(_filename,_image->width(),_image->height(),_mapMode);
  items_.emplace_back(_filename,_mapMode);
}

void ImageList::removeSelection()
{
  int _row = currentIndex().row()-1;
  if (_row < 0 || _row >= items_.size()) return;

  auto& _filename = items_[_row].first;
  session_->removeImage(_filename);
  items_.erase(items_.begin() + _row);
  model_->removeRows(_row+1,1);
}

ImageList::filename_mapmode_type ImageList::getSelected() const
{
  int _row = currentIndex().row()-1;
  if (_row < 0 || _row >= items_.size()) return {std::string(),MapMode::SPHERICAL};
  return items_[_row];
}

void ImageList::setup(Session* _session)
{
  session_=_session;
  mapModeEditor_.reset(new MapModeEditor(this,_session->canvas().get()));
  connect(mapModeEditor_.get(),SIGNAL(closeEditor(QWidget*)),this,SLOT(setMapMode(QWidget*)));
  prepareModel();
  for (auto& _image : _session->images())
  {
    auto& _img = _image.second.image_;
    auto& _filename = _image.first;
    auto& _mapMode = _image.second.mapMode_;
    addItem(_filename,_img.width(),_img.height(),_mapMode);
    items_.emplace_back(_filename,_mapMode);
  }
}

std::string ImageList::basename( std::string const& pathname )
{
  return std::string(
           std::find_if( pathname.rbegin(), pathname.rend(),
                         [](char ch)
  {
    return ch == '/';
  } ).base(),
  pathname.end() );
}

void ImageList::addItem(const std::string& _file, size_t _width, size_t _height, MapMode _mapMode)
{
  QList<QStandardItem*> _row;

  QStandardItem* _item = new QStandardItem(QString().fromStdString(basename(_file)));
  _item->setEditable(false);
  _row << _item;
  _item = new QStandardItem(tr("%1 x %2").arg(_width).arg(_height));
  _item->setEditable(false);
  _row << _item;
  _item = new QStandardItem(MapModeEditor::mapModeToQString(_mapMode));
  _item->setEditable(_mapMode != MapMode::NONE);
  _row << _item;

  model_->invisibleRootItem()->appendRow(_row);
}

void ImageList::prepareModel()
{
  model_.reset(new QStandardItemModel());
  model_->clear();
  items_.clear();
  model_->setColumnCount(3);
  model_->setHeaderData(0,Qt::Horizontal,"File");
  model_->setHeaderData(1,Qt::Horizontal,"Resolution");
  model_->setHeaderData(2,Qt::Horizontal,"Map Mode");
  setModel(model_.get());
  addItem("(Test image)",2048,1024,MapMode::NONE);
  setItemDelegateForColumn(2,mapModeEditor_.get());
}

