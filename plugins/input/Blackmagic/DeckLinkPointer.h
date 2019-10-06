
#ifndef OMNI_INPUT_DECKLINKPOINTER_H_
#define OMNI_INPUT_DECKLINKPOINTER_H_

#include <memory>
#include <DeckLinkAPI.h>

namespace omni {
  namespace input {
    /// Deleter which also releases interface
    struct ReleaseDeleter {
      template<typename T>
      void operator()(T* _ptr) {
        if (!_ptr) return;
        _ptr->Release();
      }
    };

    template<typename T>
    using DeckLinkPointer = std::unique_ptr<T,ReleaseDeleter>;
  }
}

#endif /* OMNI_INPUT_DECKLINKPOINTER_H_ */
