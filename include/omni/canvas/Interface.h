#ifndef OMNI_CANVAS_INTERFACE_H_
#define OMNI_CANVAS_INTERFACE_H_

#include <set>
#include <QMatrix4x4>
#include <omni/SerializationInterface.h>
#include <omni/mapping/Interface.h>
#include <omni/visual/Interface.h>
#include <omni/Box.h>

namespace omni
{
  namespace canvas
  {
    /// Enum which determines how UVW coordinates are generated
    enum class UVWSource
    {
      NONE,
      NORMAL,
      POSITION,
      CUSTOM
    };

    /**@brief Abstract interface for a canvas
     * @detail A canvas represents the surface on which the projection is performed.
     *         It might be a dome or a planar surface.
     **/
    class Interface : 
      public SerializationInterface,
      public visual::Interface
    {
    public:

      /// Virtual destructor
      virtual ~Interface() {}
 
      /// Draws auxiliary elements which are not used for mapping (e.g. positioning grids)
      inline virtual void drawAux() const 
      {
      }

      /// Returns set of supported map modes (an empty set means that all map modes are supported)
      inline virtual MappingModeSet supportedMapModes() const 
      {
        return MappingModeSet();
      }

      /**@brief Enum value which determines how UVW coordinates are generated (NONE by default)
       **/
      inline virtual UVWSource uvwSource() const
      {
        return UVWSource::NONE;
      }

      /// Abstract method for returning bounding box of canvas
      virtual Box bounds() const = 0;

      /// Spectator's center of canvas (center of bounding box by default)
      inline virtual QVector3D center() const
      {
        return (bounds().max() + bounds().min()) * 0.5; 
      }

      /**@brief Returns overall extent of canvas. 
       * @detail Value is needed for defining ranges for projector positioning
       *         Is the length of the diagonal vector of the bounding box by default.
       ***/
      inline virtual qreal extent() const
      {
        return bounds().size().length();
      }
     
      /// Canvas radius (is half of size by default)
      inline virtual qreal radius() const 
      {
        return extent() * 0.5;
      }

      /// Transformation matrix for canvas
      inline virtual QMatrix4x4 matrix() const
      {
        return QMatrix4x4();
      }

    protected:
      bool needsUpdate_ = true;
    };

    /// Our canvas factory
    typedef AbstractFactory<Interface> Factory;

    /// Template alias for our canvas registrar (for auto registration)
    template<typename T>
    using Registrar = typename Factory::registrar_type<T>;
  }

  typedef canvas::Interface Canvas;
  typedef canvas::Factory CanvasFactory;

  template<typename T>
  using CanvasRegistrar = canvas::Registrar<T>;
}

#endif /* OMNI_CANVAS_INTERFACE_H_ */
