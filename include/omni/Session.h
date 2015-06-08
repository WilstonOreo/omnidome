#ifndef OMNI_SESSION_H_
#define OMNI_SESSION_H_

#include <omni/proj/TuningList.h>
#include <omni/input/List.h>
#include <omni/canvas/Interface.h>
#include <omni/proj/ScreenSetup.h>

namespace omni
{
  class TuningList;

  /**@brief A session consists of a canvas, a mapping, a list of tunings and one or several inputs
   */
  class Session
  {
  public:
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
  };
}

/// Serialize session to stream
QDataStream& operator<<(QDataStream&, omni::Session const&);

/// Deserialize session from stream
QDataStream& operator>>(QDataStream&, omni::Session&);

#endif /* OMNI_SESSION_H_ */
