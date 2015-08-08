#ifndef OMNI_SESSION_H_
#define OMNI_SESSION_H_

#include <omni/proj/TuningList.h>
#include <omni/input/List.h>
#include <omni/canvas/Interface.h>
#include <omni/proj/ScreenSetup.h>
#include <omni/RenderOptions.h>

namespace omni
{
  class TuningList;

  /**@brief A session consists of a canvas, a mapping, a list of tunings and one or several inputs
   */
  class Session
  {
  public:
    /// General mode of the User Interface
    enum class Mode 
    {
      SCREENSETUP, // Mode for setup screens
      PROJECTIONSETUP, // Mode for setting up projectors and canvas
      WARP, // Mode for adjusting warp grid
      BLEND, // Mode for editing the blend mask
      EXPORT, // Final export mode. Output is locked for faster display
      NUM_MODES
    };

    /// Blend Mask display mode
    enum class BlendMode
    {
      COLOR, // Displays blend mask with color of tuning
      WHITE, // Displays white blend mask
      INPUT // Displays blend mask with current input 
    };
    
    /// Default constructor
    Session();
    ~Session();

    /// Returns reference to projector tunings
    proj::TuningList& tunings();

    /// Returns const reference to projector tunings
    proj::TuningList const& tunings() const;

    /// Makes new mapping with given type Id
    Mapping* setMapping(Id const& _typeId);

    /// Returns pointer to mapping
    Mapping* mapping();

    /// Returns pointer to mapping (const version)
    Mapping const* mapping() const;

    /// Returns reference to list of inputs 
    InputList& inputs();
    
    /// Returns const reference to list of inputs 
    InputList const& inputs() const;

    /// Makes new canvas with given type id and returns pointer if successful
    Canvas* setCanvas(Id const& _typeId);

    /// Return pointer to canvas
    Canvas* canvas();
 
    /// Return pointer to canvas (const version)
    Canvas const* canvas() const;

    /// Return reference to current screen setup
    ScreenSetup& screenSetup(); 

    /// Return const reference to current screen setup 
    ScreenSetup const& screenSetup() const;

    /// Return current mode
    Mode mode() const;

    /// Set mode of this session
    void setMode(Mode);

    /// Return mode of blend mask mode
    BlendMode blendMode() const;

    /// Set blend mask mode
    void setBlendMode(BlendMode);

    /// A session has an output when there is an input, a canvas and a mapping
    bool hasOutput() const;

    /// Export calibration data of session to a file
    void renderToFile(QString const& _filename, RenderOptions const& _opt);

    /// Save session to file
    void save(QString const& _filename) const;
    
    /// Load session to file
    void load(QString const& _filename);

    /// Test for equality. ScreenSetup is ignored
    friend bool operator==(Session const&,Session const&);

  private:
    /// List with all projector tunings
    proj::TuningList tunings_;

    /// Owning pointer for mapping
    std::unique_ptr<Mapping> mapping_;
    
    /// List with all inputs (e.g. images, videos etc) 
    InputList inputs_;
    
    /// Owning pointer for canvas
    std::unique_ptr<Canvas> canvas_;
    
    /// Current screen setup
    ScreenSetup screenSetup_;

    /// Current session mode
    Mode mode_ = Mode::SCREENSETUP;

    BlendMode blendMode_ = BlendMode::COLOR;
  };
}

/// Serialize session to stream
QDataStream& operator<<(QDataStream&, omni::Session const&);

/// Deserialize session from stream
QDataStream& operator>>(QDataStream&, omni::Session&);

#endif /* OMNI_SESSION_H_ */
