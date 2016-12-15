#include "BlackmagicWidget.h"

#include <QDebug>

#include <QStandardItem>
#include <QComboBox>
#include "DeckLinkAPI.h"

namespace omni {
  namespace ui {
    namespace input {
      BlackmagicDeviceItemModel::BlackmagicDeviceItemModel(QObject* parent) : 
        QStandardItemModel(parent) {
        setColumnCount(1); 
        update();
      }

      BlackmagicDeviceItemModel::~BlackmagicDeviceItemModel() {}

      void BlackmagicDeviceItemModel::update() {
        clear();
        IDeckLinkIterator*				deckLinkIterator = CreateDeckLinkIteratorInstance();
        HRESULT							result = E_FAIL;
        
	      IDeckLink*						deckLink = NULL;
	      char*							    deckLinkName = NULL;

        int deckLinkCount = 0;
	      // Loop through all available devices
	      while (deckLinkIterator->Next(&deckLink) == S_OK)
	      {
          result = deckLink->GetModelName((const char**)&deckLinkName);
          if (result == S_OK)
		      {
            QStandardItem* item = new QStandardItem(QString(deckLinkName));
            item->setEditable(false);
            QList<QStandardItem*> row;
            row << item;
            invisibleRootItem()->appendRow(row);
			      free(deckLinkName);
          }
  
          deckLink->Release();
    		  ++deckLinkCount;
		    }
      }    
        
      IDeckLink* BlackmagicDeviceItemModel::device(int idx) const {
	      HRESULT				result;
	      IDeckLink*			deckLink = nullptr;
	      IDeckLinkIterator*	deckLinkIterator = CreateDeckLinkIteratorInstance();
	      int					i = idx;
	      while((result = deckLinkIterator->Next(&deckLink)) == S_OK)
	      {
		      if (i == 0)
			    break;
		      --i;
		      deckLink->Release();
	      }
      	deckLinkIterator->Release();
	      
        qDebug() << "Device: " << deckLink;
        
        return (result == S_OK) ? deckLink : nullptr;
      }



      BlackmagicDisplayModeItemModel::BlackmagicDisplayModeItemModel(QObject* parent) : 
        QStandardItemModel(parent) {
          setColumnCount(6);
      }

      BlackmagicDisplayModeItemModel::~BlackmagicDisplayModeItemModel() {
      }

      void BlackmagicDisplayModeItemModel::update(IDeckLink* deckLink) {
        clear();
        if (!deckLink) return;
        HRESULT							result = E_FAIL; 
	      IDeckLinkInput*		  deckLinkInput = nullptr;
	      result = deckLink->QueryInterface(IID_IDeckLinkInput, (void**)&deckLinkInput);
	      IDeckLinkDisplayModeIterator*	displayModeIterator = nullptr;
	      IDeckLinkDisplayMode*			displayModeUsage = nullptr;
	
	      if (result != S_OK) return;

	      result = deckLinkInput->GetDisplayModeIterator(&displayModeIterator);
	      if (result != S_OK) return;
	
        int displayModeCount = 0;
        char*							displayModeName;

        /// Add auto-detect mode
        addDisplayMode(-1,QStringLiteral("Auto-detect"),QSize(0,0),0.0);

	      while (displayModeIterator->Next(&displayModeUsage) == S_OK)
	      {
		      result = displayModeUsage->GetName((const char **)&displayModeName);
		      if (result == S_OK)
		      {
			      BMDTimeValue frameRateDuration;
			      BMDTimeValue frameRateScale;
			      displayModeUsage->GetFrameRate(&frameRateDuration, &frameRateScale);
            addDisplayMode(displayModeCount,
                QString(displayModeName),
                QSize(displayModeUsage->GetWidth(),displayModeUsage->GetHeight()),
				        (double)frameRateScale / (double)frameRateDuration
                );
			      free(displayModeName);
		      }

		      displayModeUsage->Release();
		      ++displayModeCount;
	      }
      }

      IDeckLinkDisplayMode* BlackmagicDisplayModeItemModel::displayMode(IDeckLink* deckLink, int idx) const {
      	if (!deckLink) return nullptr;

	      HRESULT							result;
      	IDeckLinkDisplayMode*			displayMode = NULL;
	      IDeckLinkInput*					deckLinkInput = NULL;
	      IDeckLinkDisplayModeIterator*	displayModeIterator = NULL;
	      int i = idx - 1;
		    if (i == -1) {
          // Auto detection 
          return nullptr;
        }

	      result = deckLink->QueryInterface(IID_IDeckLinkInput, (void**)&deckLinkInput);
	      if (result != S_OK) return nullptr;

      	result = deckLinkInput->GetDisplayModeIterator(&displayModeIterator);
        if (result != S_OK) return nullptr;

      	while ((result = displayModeIterator->Next(&displayMode)) == S_OK)
	      {
		      if (i == 0) break;
		      --i;
		      displayMode->Release();
	      }
        
        qDebug() << "DisplayMode: " << displayMode;
	
        return (result == S_OK) ? displayMode : nullptr;      
      }
          
      void BlackmagicDisplayModeItemModel::addDisplayMode(int idx, QString const& name, QSize const& size, double fps)    
      {  
        
        QList<QStandardItem*> row;    
        auto addItem = [&](QString const& s) {
          QStandardItem* item = new QStandardItem(s);
          item->setEditable(false);
          row << item;
        };
        if (idx == -1) {
          addItem("Auto-detect");
        } else {
          auto label = QString("%1: %2 (%3x%4 %5 FPS)").
            arg(idx).arg(name).
            arg(size.width()).arg(size.height()).
            arg(fps);
        
          addItem(label);
          addItem(name);
          addItem(QString::number(size.width()));
          addItem(QString::number(size.height()));
          addItem(QString::number(fps));
        }

        invisibleRootItem()->appendRow(row);
      }

      Blackmagic::Blackmagic(omni::input::Blackmagic* input, QWidget* parent) : 
        omni::ui::ParameterWidget(parent),
        input_(input) {
        setup();
      }

      Blackmagic::~Blackmagic() {
      }

      void Blackmagic::triggerUpdate() {
        if (!preview_) return;

        preview_->triggerUpdate();
      }
          
      void Blackmagic::updateDisplayModes() {
        auto* deckLink = deviceModel_->device(deviceList_->currentIndex());        
        displayModeModel_->update(deckLink);
      }

      void Blackmagic::setup() {
        QLayout *layout = new QVBoxLayout;
        layout->setSpacing(2);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSizeConstraint(QLayout::SetMaximumSize);
        setLayout(layout);
        
        deviceModel_.reset(new BlackmagicDeviceItemModel);
        
        deviceList_.reset(new QComboBox());
        deviceList_->setModel(deviceModel_.get());
        layout->addWidget(deviceList_.get());
        
        displayModeModel_.reset(new BlackmagicDisplayModeItemModel);
        
        displayModeList_.reset(new QComboBox());
        displayModeList_->setModel(displayModeModel_.get());
        layout->addWidget(displayModeList_.get());
      
        pixelFormatList_.reset(new QComboBox());
        pixelFormatList_->addItem("8-bit YUV (4:2:2) (default)");
        pixelFormatList_->addItem("10-bit YUV (4:2:2)");
        pixelFormatList_->addItem("10-bit RGB (4:4:4)");
        layout->addWidget(pixelFormatList_.get());
        
        preview_.reset(new InputPreview(input_));
        connect(preview_.get(), SIGNAL(inputChanged()), this,
                SIGNAL(inputChanged()));
        layout->addWidget(preview_.get());


        auto setupInput = [&](int) {
          auto* device = deviceModel_->device(deviceList_->currentIndex());
          input_->setup(device,
                        displayModeModel_->displayMode(device,displayModeList_->currentIndex()));  
          input_->activate();
        };

        connect(deviceList_.get(),
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),setupInput);
        connect(displayModeList_.get(),
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),setupInput);

        updateDisplayModes();
        setupInput(0);
      }

    }
  }
}
