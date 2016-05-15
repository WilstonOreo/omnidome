/* Copyright (c) 2014-2015 "Omnidome" by cr8tr
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
        friend omni::visual::ContextBoundPtr<T,DELETER>;
        template<typename PTR>
        ContextBoundPtrInternal(PTR *_ptr, QOpenGLContext *_context) {
           connection_ = QObject::connect(_context,
                                         &QOpenGLContext::aboutToBeDestroyed,
                                         [&]() {
            delete this;
          });
          ptr_ = _ptr;
          context_  = _context;
          surface_  = _context->surface();
        }

        ~ContextBoundPtrInternal() {
          QObject::disconnect(connection_);
          if (!QOpenGLContext::currentContext()) return;

          /// Make sure object is destroyed in its context
          contextSwitch(context_,[&](QOpenGLFunctions& _) {
            if (ptr_) {
              DELETER()(ptr_);
              ptr_ = nullptr;
            }
          });
        }

        private:
          QMetaObject::Connection connection_;
          T                  *ptr_ = nullptr;
          QSurface       *surface_ = nullptr;
          QOpenGLContext *context_ = nullptr;
      };
    }

    /**@brief A pointer template which is free'd together with its context
       @detail Holds an internal object which is responsible for freeing the
               held object when OpenGL context is free'd.
     **/
    template<typename T, typename DELETER = std::default_delete<T>>
    struct ContextBoundPtr {
      ContextBoundPtr() {}

      template<typename PTR>
      ContextBoundPtr(PTR *_p,
                      QOpenGLContext *_context =
                      QOpenGLContext::globalShareContext()) {
        reset(_p, _context);
      }

      ~ContextBoundPtr() {
        // Internal is not deleted here because its done by the bound context
        //reset();
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

      /// Reset and and assign neew pointer
      template<typename PTR>
      T* reset(PTR *_p,
               QOpenGLContext *_context = QOpenGLContext::globalShareContext()) {
        /// Reset and make new internal object
        reset();
        /// No initialization without context!
        bool _valid = _context ? _context->surface() != nullptr : false;
        if (!_valid) {
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
        detail::ContextBoundPtrInternal<T> *internal_ = nullptr;
    };
  }

  using visual::ContextBoundPtr;
}

#endif /* OMNI_VISUAL_CONTEXTBOUNDPTR_H_ */
