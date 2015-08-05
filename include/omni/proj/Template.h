#ifndef OMNI_UI_PROJ_TEMPLATE_H_
#define OMNI_UI_PROJ_TEMPLATE_H_

namespace omni
{
  class Session;

  namespace proj
  {
    /**@brief Projector setup template interface  
     * @detail Adds one or several projectors/tunings in a certain placement to a session
     **/
    class Template 
    {
    public: 
      /// Add tunings to session
      virtual void add(Session*) = 0;

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

