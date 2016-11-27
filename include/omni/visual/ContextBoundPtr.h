/* Copyright (c) 2014-2015 "Omnidome" by Michael Winkelmann
 * Dome Mapping Projection Software (http://omnido.me).
 * Omnidome was created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 *
 * This file is part of Omnidome.
 *
 * Omnidome is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OMNI_VISUAL_CONTEXTBOUNDPTR_H_
#define OMNI_VISUAL_CONTEXTBOUNDPTR_H_

#include <memory>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include <omni/visual/ContextSwitch.h>

namespace omni {
  namespace visual {

    template<typename T, typename DELETER>
    struct ContextBoundPtr;

    namespace detail {
      /// The internal structure responsible for deleting the object
      template<typename T, typename DELETER = std::default_delete<T>>
      struct ContextBoundPtrInternal {

        using element_type = T;
        using deleter_type = DELETER;

        friend omni::visual::ContextBoundPtr<T,DELETER>;

        template<typename PTR>
        ContextBoundPtrInternal(PTR *_ptr, QOpenGLContext *_context) :
          ptr_(_ptr), context_(_context) {

          connection_ = QObject::connect(_context,
                                         &QOpenGLContext::aboutToBeDestroyed,
                                         [&]() {
            delete this;
          });
        }

        ~ContextBoundPtrInternal() {
          QObject::disconnect(connection_);

          /// Make sure object is destroyed in its context
          contextSwitch(context_,[&](QOpenGLFunctions& _) {
            deleter_type()(ptr_);
          });
        }

      private:
        QMetaObject::Connection connection_;
        element_type       *ptr_ = nullptr;
        QOpenGLContext *context_ = nullptr;
      };
    }

    /**@brief A pointer template which is free'd together with its context
       @detail Holds an internal object which is responsible for freeing the
               held object when OpenGL context is free'd.
     **/
    template<typename T, typename DELETER = std::default_delete<T>>
    struct ContextBoundPtr {
      using element_type = T;
      using deleter_type = DELETER;

      ContextBoundPtr() {}

      template<typename PTR>
      ContextBoundPtr(PTR *_p,
                      QOpenGLContext *_context =
                      QOpenGLContext::globalShareContext()) {
        reset(_p, _context);
      }

      ~ContextBoundPtr() {
      }

      /// Delete copy constructor
      ContextBoundPtr(const ContextBoundPtr& that) = delete;

      /// Delete copy assignment operator
      ContextBoundPtr& operator=(const ContextBoundPtr&) = delete;

      /// Delete held object and internal state
      void reset() {
        if (internal_) {
          delete internal_;
          internal_ = nullptr;
        }
      }

      /// Reset and and assign new pointer
      template<typename PTR>
      T* reset(PTR *_p,
               QOpenGLContext *_context = QOpenGLContext::globalShareContext()) {
        /// Reset and make new internal object
        reset();
        /// No initialization without context!
        if (!_context || !_context->isValid()) {
          return nullptr;
        }

        internal_ = new detail::ContextBoundPtrInternal<T,DELETER>(_p, _context);

        return get();
      }

      T* get() const {
        return internal_ ? internal_->ptr_ : nullptr;
      }

      /// Cast to bool operator. Return true if pointer is not null
      explicit operator bool() const {
        return get() != nullptr;
      }

      /// Return reference
      typename std::add_lvalue_reference<T>::type operator*() const {
        return *get();
      }

      T * operator->() const {
        return get();
      }

      /// Return pointer to associated context
      QOpenGLContext* context() const {
        return internal_ ? internal_->context_ : nullptr;
      }

      private:
        detail::ContextBoundPtrInternal<T,DELETER> *internal_ = nullptr;
    };
  }

  using visual::ContextBoundPtr;
}

#endif /* OMNI_VISUAL_CONTEXTBOUNDPTR_H_ */
