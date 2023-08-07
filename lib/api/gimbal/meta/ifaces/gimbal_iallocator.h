/*! \file
 *  \brief GblIAllocator abstract allocator interface
 *  \ingroup interfaces
 *  \todo
 *      - make specific allocators implement me
 *
 *  \author     2023 Falco Girgis
 *  \copyright MIT License
 */

#ifndef GIMBAL_IALLOCATOR_H
#define GIMBAL_IALLOCATOR_H

#include "gimbal_interface.h"
#include "../../core/gimbal_ctx.h"

/*! \name Type System
 *  \brief Type UUID and cast operators
 *  @{
 */
#define GBL_IALLOCATOR_TYPE             (GBL_TYPEID(GblIAllocator))            //!< Type UUID for GblIAllocator
#define GBL_IALLOCATOR(self)            (GBL_CAST(GblIAllocator, self))        //!< Casts a GblInstance to GblIAllocator
#define GBL_IALLOCATOR_CLASS(klass)     (GBL_CLASS_CAST(GblIAllocator, klass)) //!< Casts a GblClass to GblIAllocatorClass
#define GBL_IALLOCATOR_GET_CLASS(self)  (GBL_CLASSOF(GblIAllocator, self))     //!< Gets a GblIAllocatorClass from a GblInstance
//! @}

#define GBL_SELF_TYPE GblIAllocator

GBL_DECLS_BEGIN

/*! \struct  GblIAllocatorClass
 *  \extends GblInterface
 *  \brief   GblClass VTable structure for GblIAllocator
 *
 *  GblIAllocatorClass provides polymorphic methods for
 *  basic memory management operations: alloc, realloc, free.
 *
 *  \note
 *  All operations are required to honor the requested alignment.
 */
GBL_INTERFACE_DERIVE(GblIAllocator)
    //! Virtual method invoked when an allocation is requested
    GBL_RESULT (*pFnAlloc)   (GBL_SELF, const GblStackFrame* pFrame, size_t size, size_t align, const char* pDbgStr, void** ppData);
    //! Virtual method invoked when a reallocation is requested
    GBL_RESULT (*pFnRealloc) (GBL_SELF, const GblStackFrame* pFrame, void* pData, size_t newSize, size_t newAlign, void** ppNewData);
    //! Virtual method invoked when a free is requested
    GBL_RESULT (*pFnFree)    (GBL_SELF, const GblStackFrame* pFrame, void* pData);
    //! Virtual method for retrieving the upstream parent fallback allocator
    GBL_RESULT (*pFnParent)  (GBL_CSELF, GblIAllocator** ppParent);
GBL_INTERFACE_END

//! Returns the GblType UUID associated with the GblIAllocator type
GBL_EXPORT GblType GblIAllocator_type (void) GBL_NOEXCEPT;

/*! \name  Memory Management
 *  \brief Memory allocation, reallocation, and freeing entry points
 *  @{
 */
GBL_EXPORT GBL_RESULT GblIAllocator_alloc   (GBL_SELF,
                                             const GblStackFrame* pFrame,
                                             size_t               size,
                                             size_t               alignment,
                                             const char*          pDebugString,
                                             void**               ppData)    GBL_NOEXCEPT;

GBL_EXPORT GBL_RESULT GblIAllocator_realloc (GBL_SELF,
                                             const GblStackFrame* pFrame,
                                             void*                pData,
                                             size_t               newSize,
                                             size_t               newAlign,
                                             void**               ppNewData) GBL_NOEXCEPT;

GBL_EXPORT GBL_RESULT GblIAllocator_free    (GBL_SELF,
                                             const GblStackFrame* pFrame,
                                             void*                pData)     GBL_NOEXCEPT;
//! @}

GBL_EXPORT GblIAllocator* GblIAllocator_parent(GBL_CSELF) GBL_NOEXCEPT;

GBL_DECLS_END

#undef GBL_SELF_TYPE

#endif // GIMBAL_IALLOCATOR_H
