#ifndef OMNI_UI_PROJ_MULTISETUP_H_
#define OMNI_UI_PROJ_MULTISETUP_H_

#include <omni/TypeIdInterface.h>
#include <omni/Session.h>

namespace omni
{
  class Session;

  namespace proj
  {
    /**@brief Projector setup template interface  
     * @detail Adds one or several projectors/tunings in a certain placement to a session
     **/
    class MultiSetup : public TypeIdInterface
    {
    public: 
      virtual ~MultiSetup() {}

      /// Add tunings to session
      inline void add(Session* _session)
      {
        auto&& _projs = this->projectors();

        for (auto& _proj : _projs)
        {
          auto* _tuning = _session->tunings().add();
          _tuning->projector() = std::move(_proj);
        }
      }

      virtual std::vector<Projector> projectors() const = 0;


    private:
    };

    /// Typedef for our factory
    typedef AbstractFactory<MultiSetup> MultiSetupFactory;

    /// MultiSetup alias for our registrar (for auto registration)
    template<typename T>
    using MultiSetupRegistrar = typename MultiSetupFactory::registrar_type<T>; 
  }
}

#endif /* OMNI_UI_PROJ_MULTISETUP_H_ */

