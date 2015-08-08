#ifndef OMNI_UI_PROJ_TEMPLATE_H_
#define OMNI_UI_PROJ_TEMPLATE_H_

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
    class Template : public TypeIdInterface
    {
    public: 
      virtual ~Template() {}

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
    typedef AbstractFactory<Template> TemplateFactory;

    /// Template alias for our registrar (for auto registration)
    template<typename T>
    using TemplateRegistrar = typename TemplateFactory::registrar_type<T>; 
  }
}

#endif /* OMNI_UI_PROJ_TEMPLATE_H_ */

