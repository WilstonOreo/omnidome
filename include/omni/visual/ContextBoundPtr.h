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

namespace omni {
  namespace visual {
    template<typename T, typename DELETER>
    struct ContextBoundPtr;

    namespace detail {
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
        }

        ~ContextBoundPtrInternal() {
          DELETER()(ptr_);
          ptr_ = nullptr;
          QObject::disconnect(connection_);
        }

        T* get() const {
          return ptr_;
        }

        private:
          T                      *ptr_ = nullptr;
          QMetaObject::Connection connection_;
      };
    }

    /// A pointer which is free'd together with its context
    template<typename T, typename DELETER = std::default_delete<T>>
    struct ContextBoundPtr {
      ContextBoundPtr() {}

      template<typename PTR>
      ContextBoundPtr(PTR *_p,
                      QOpenGLContext *_context =
                        QOpenGLContext::currentContext()) {
        reset(_p, _context);
      }

      ~ContextBoundPtr() {
        // Internal is not deleted here because its done by the bound context
      }

      /// Delete copy constructor
      ContextBoundPtr(const ContextBoundPtr& that) = delete;

      /// Delete copy assignment operator
      ContextBoundPtr& operator=(const ContextBoundPtr&) = delete;

      /// Delete held object and internal state
      void reset() {
        auto _current = QOpenGLContext::currentContext();

        QSurface* _surface = _current ? _current->surface() : nullptr;

        if (context_ && surface_) {
          if (context_ != _current) {
            context_->makeCurrent(surface_);
          }
        }

        if (internal_) {
          delete internal_;
          internal_ = nullptr;
        }

        if (context_) {
          context_->doneCurrent();
          if (_current && _surface) {
            _current->makeCurrent(_surface);
          }
        }

        context_ = nullptr;
        surface_ = nullptr;
      }

      /// Reset and and assign neew pointer
      template<typename PTR>
      T* reset(PTR *_p,
               QOpenGLContext *_context = QOpenGLContext::currentContext()) {
        /// No initialization without context!

        bool _valid = _context ? _context->surface() != nullptr : false;
        if (!_valid) {
          reset();
          return nullptr;
        }


        reset();
        internal_ = new detail::ContextBoundPtrInternal<T,DELETER>(_p, _context);
        context_  = _context;
        surface_  = _context->surface();

        return get();
      }

      T* get() const {
        return internal_ ? internal_->get() : nullptr;
      }

      explicit operator bool() const {
        return get() != nullptr;
      }

      typename std::add_lvalue_reference<T>::type operator*() const {
        return *get();
      }

      T * operator->() const {
        return get();
      }

      QOpenGLContext* context() const {
        return context_;
      }

      private:
        detail::ContextBoundPtrInternal<T> *internal_ = nullptr;

        QSurface       *surface_ = nullptr;
        QOpenGLContext *context_ = nullptr;
    };
  }

  using visual::ContextBoundPtr;
}

#endif /* OMNI_VISUAL_CONTEXTBOUNDPTR_H_ */
