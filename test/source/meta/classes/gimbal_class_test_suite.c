#include "meta/classes/gimbal_class_test_suite.h"
#include <gimbal/test/gimbal_test_macros.h>
#include <gimbal/core/gimbal_ctx.h>
#include <gimbal/meta/classes/gimbal_class.h>

#define GBL_CLASS_TEST_SUITE_(inst)     (GBL_PRIVATE(GblClassTestSuite, inst))

typedef struct GblClassTestSuite_ {
    size_t      initialStaticClassRefCount;
    GblClass*   pClassRef;
    GblClass*   pClassRef2;
} GblClassTestSuite_;

static GBL_RESULT GblClassTestSuite_init_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblClassTestSuite_* pSelf_ = GBL_CLASS_TEST_SUITE_(pSelf);
    memset(pSelf_, 0, sizeof(GblClassTestSuite_));
    pSelf_->initialStaticClassRefCount = GblType_classRefCount(GBL_STATIC_CLASS_TYPE);
    GBL_CTX_END();
}

static GBL_RESULT GblClassTestSuite_final_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblClassTestSuite_* pSelf_ = GBL_CLASS_TEST_SUITE_(pSelf);
    GBL_TEST_COMPARE(GblType_classRefCount(GBL_STATIC_CLASS_TYPE), pSelf_->initialStaticClassRefCount);
    GBL_CTX_END();
}

static GBL_RESULT GblClassTestSuite_refInvalid_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_UNUSED(pSelf);
    GBL_CTX_BEGIN(pCtx);
    GBL_TEST_EXPECT_ERROR();
    GblClass* pClass = GblClass_refDefault(GBL_INVALID_TYPE);
    GBL_TEST_COMPARE(GBL_CTX_LAST_RESULT(), GBL_RESULT_ERROR_INVALID_TYPE);
    GBL_TEST_COMPARE(pClass, NULL);
    GBL_CTX_CLEAR_LAST_RECORD();
    GBL_CTX_END();
}

static GBL_RESULT GblClassTest_weakRefDefaultInvalid_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_UNUSED(pSelf);
    GBL_CTX_BEGIN(pCtx);
    GBL_TEST_EXPECT_ERROR();
    GblClass* pClass = GblClass_weakRefDefault(GBL_INVALID_TYPE);
    GBL_TEST_COMPARE(GBL_CTX_LAST_RESULT(), GBL_RESULT_ERROR_INVALID_TYPE);
    GBL_TEST_COMPARE(pClass, NULL);
    GBL_CTX_CLEAR_LAST_RECORD();
    GBL_CTX_END();
}

static GBL_RESULT GblClassTestSuite_checkNullInvalid_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_UNUSED(pSelf);
    GBL_CTX_BEGIN(pCtx);
    GBL_TEST_VERIFY(GblClass_check(NULL, GBL_INVALID_TYPE));
    GBL_CTX_END();
}

static GBL_RESULT GblClassTestSuite_castNullInvalid_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_UNUSED(pSelf);
    GBL_CTX_BEGIN(pCtx);
    GBL_TEST_COMPARE(GblClass_cast(NULL, GBL_INVALID_TYPE), NULL);
    GBL_CTX_END();
}

static GBL_RESULT GblClassTestSuite_tryNullInvalid_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_UNUSED(pSelf);
    GBL_CTX_BEGIN(pCtx);
    GBL_TEST_COMPARE(GblClass_as(NULL, GBL_INVALID_TYPE), NULL);
    GBL_CTX_END();
}

static GBL_RESULT GblClassTestSuite_privateNull_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_UNUSED(pSelf);
    GBL_CTX_BEGIN(pCtx);
    GBL_TEST_COMPARE(GblClass_private(NULL, GBL_INVALID_TYPE), NULL);
    GBL_CTX_END();
}

static GBL_RESULT GblClassTestSuite_publicNull_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_UNUSED(pSelf);
    GBL_CTX_BEGIN(pCtx);
    GBL_TEST_COMPARE(GblClass_public(NULL, GBL_INVALID_TYPE), NULL);
    GBL_CTX_END();
}

static GBL_RESULT GblClassTestSuite_propertiesNull_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_UNUSED(pSelf);
    GBL_CTX_BEGIN(pCtx);
    GBL_TEST_COMPARE(GblClass_typeOf(NULL), GBL_INVALID_TYPE);
    GBL_TEST_COMPARE(GblClass_size(NULL), 0);
    GBL_TEST_COMPARE(GblClass_privateSize(NULL), 0);
    GBL_TEST_COMPARE(GblClass_totalSize(NULL), 0);
    GBL_TEST_COMPARE(GblClass_super(NULL), NULL);
    GBL_TEST_COMPARE(GblClass_default(NULL), NULL);
    GBL_TEST_COMPARE(GblClass_isDefault(NULL), GBL_FALSE);
    GBL_TEST_COMPARE(GblClass_isOverridden(NULL), GBL_FALSE);
    GBL_TEST_COMPARE(GblClass_isFloating(NULL), GBL_FALSE);
    GBL_TEST_COMPARE(GblClass_isInterface(NULL), GBL_FALSE);
    GBL_TEST_COMPARE(GblClass_isInterfaceImpl(NULL), GBL_FALSE);
    GBL_TEST_COMPARE(GblClass_isOwned(NULL), GBL_FALSE);
    GBL_TEST_COMPARE(GblClass_isInPlace(NULL), GBL_FALSE);
    GBL_TEST_COMPARE(GBL_CLASS_TYPEOF(NULL), GBL_INVALID_TYPE);
    GBL_TEST_COMPARE(GblClass_super(NULL), NULL);
    GBL_TEST_COMPARE(GblClass_default(NULL), NULL);
    GBL_CTX_END();
}

static GBL_RESULT GblClassTestSuite_unrefNull_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_UNUSED(pSelf);
    GBL_UNUSED(pSelf);
    GBL_CTX_BEGIN(pCtx);
    GBL_TEST_COMPARE(GblClass_unrefDefault(NULL), 0);
    GBL_CTX_END();
}

static GBL_RESULT GblClassTestSuite_createFloatingInvalid_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_UNUSED(pSelf);
    GBL_CTX_BEGIN(pCtx);
    GBL_TEST_EXPECT_ERROR();
    GblClass* pClass = GblClass_createFloating(GBL_INVALID_TYPE);
    GBL_TEST_COMPARE(GBL_CTX_LAST_RESULT(), GBL_RESULT_ERROR_INVALID_TYPE);
    GBL_TEST_COMPARE(pClass, NULL);
    GBL_CTX_CLEAR_LAST_RECORD();
    GBL_CTX_END();
}

static GBL_RESULT GblClassTestSuite_constructFloatingInvalid_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_UNUSED(pSelf);
    GBL_CTX_BEGIN(pCtx);
    GBL_TEST_EXPECT_ERROR();
    GblClass_constructFloating(NULL, GBL_INVALID_TYPE);
    GBL_TEST_COMPARE(GBL_CTX_LAST_RESULT(), GBL_RESULT_ERROR_INVALID_TYPE);
    GBL_CTX_CLEAR_LAST_RECORD();
    GblClass_constructFloating(NULL, GBL_INTERFACE_TYPE);
    GBL_TEST_COMPARE(GBL_CTX_LAST_RESULT(), GBL_RESULT_ERROR_INVALID_POINTER);
    GBL_CTX_CLEAR_LAST_RECORD();
    GBL_CTX_END();
}

static GBL_RESULT GblClassTestSuite_destroyFloatingNull_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_UNUSED(pSelf);
    GBL_CTX_BEGIN(pCtx);
    GBL_CTX_VERIFY_CALL(GblClass_destroyFloating(NULL));
    GBL_CTX_END();
}

static GBL_RESULT GblClassTestSuite_destructFloatingNull_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_UNUSED(pSelf);
    GBL_CTX_BEGIN(pCtx);
    GBL_CTX_VERIFY_CALL(GblClass_destructFloating(NULL));
    GBL_CTX_END();
}

static GBL_RESULT GblClassTestSuite_refDefault_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblClassTestSuite_* pSelf_ = GBL_CLASS_TEST_SUITE_(pSelf);

    pSelf_->pClassRef = GblClass_refDefault(GBL_STATIC_CLASS_TYPE);
    GBL_CTX_VERIFY_LAST_RECORD();
    GBL_TEST_VERIFY(pSelf_->pClassRef);

    GBL_TEST_COMPARE(GblType_classRefCount(GBL_CLASS_TYPEOF(pSelf_->pClassRef)),
                     pSelf_->initialStaticClassRefCount + 1);

    GblClass* pClass = GblClass_refDefault(GBL_STATIC_CLASS_TYPE);
    GBL_CTX_VERIFY_LAST_RECORD();
    GBL_TEST_VERIFY(pClass);

    GBL_TEST_COMPARE(pClass, pSelf_->pClassRef);

    GBL_TEST_COMPARE(GblType_classRefCount(GBL_CLASS_TYPEOF(pClass)),
                     pSelf_->initialStaticClassRefCount + 2);

    GBL_CTX_END();
}

static GBL_RESULT GblClassTest_weakRefDefault_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblClassTestSuite_* pSelf_ = GBL_CLASS_TEST_SUITE_(pSelf);

    GblClass* pClass  = GblClass_weakRefDefault(GBL_STATIC_CLASS_TYPE);
    GBL_TEST_COMPARE(pClass, pSelf_->pClassRef);
    GBL_TEST_COMPARE(GblType_classRefCount(GBL_CLASS_TYPEOF(pSelf_->pClassRef)),
                     pSelf_->initialStaticClassRefCount + 2);

    GBL_CTX_END();
}

static GBL_RESULT GblClassTestSuite_check_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblClassTestSuite_* pSelf_ = GBL_CLASS_TEST_SUITE_(pSelf);

    GBL_TEST_VERIFY(GblClass_check(pSelf_->pClassRef, GBL_STATIC_CLASS_TYPE));
    GBL_TEST_VERIFY(!GblClass_check(pSelf_->pClassRef, GBL_INTERFACE_TYPE));

    GBL_CTX_END();
}


static GBL_RESULT GblClassTestSuite_cast_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblClassTestSuite_* pSelf_ = GBL_CLASS_TEST_SUITE_(pSelf);

    GBL_TEST_COMPARE(GblClass_cast(pSelf_->pClassRef, GBL_STATIC_CLASS_TYPE),
                     pSelf_->pClassRef);

    GBL_CTX_END();
}

static GBL_RESULT GblClassTestSuite_castInvalid_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblClassTestSuite_* pSelf_ = GBL_CLASS_TEST_SUITE_(pSelf);

    GBL_TEST_EXPECT_ERROR();
    GBL_TEST_COMPARE(GblClass_cast(pSelf_->pClassRef, GBL_INTERFACE_TYPE),
                     NULL);

    GBL_TEST_COMPARE(GBL_CTX_LAST_RESULT(), GBL_RESULT_ERROR_TYPE_MISMATCH);
    GBL_CTX_CLEAR_LAST_RECORD();

    GBL_CTX_END();
}

static GBL_RESULT GblClassTestSuite_try_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblClassTestSuite_* pSelf_ = GBL_CLASS_TEST_SUITE_(pSelf);

    GBL_TEST_COMPARE(GblClass_as(pSelf_->pClassRef, GBL_STATIC_CLASS_TYPE),
                     pSelf_->pClassRef);

    GBL_CTX_END();
}

static GBL_RESULT GblClassTestSuite_tryInvalid_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblClassTestSuite_* pSelf_ = GBL_CLASS_TEST_SUITE_(pSelf);

    GBL_TEST_COMPARE(GblClass_as(pSelf_->pClassRef, GBL_INTERFACE_TYPE),
                     NULL);

    GBL_CTX_END();
}

static GBL_RESULT GblClassTestSuite_private_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblClassTestSuite_* pSelf_ = GBL_CLASS_TEST_SUITE_(pSelf);

    GBL_TEST_COMPARE(GblClass_private(pSelf_->pClassRef, GBL_STATIC_CLASS_TYPE),
                     NULL);

    GBL_CTX_END();
}

static GBL_RESULT GblClassTestSuite_privateInvalid_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblClassTestSuite_* pSelf_ = GBL_CLASS_TEST_SUITE_(pSelf);

    GBL_TEST_EXPECT_ERROR();

    GBL_TEST_COMPARE(GblClass_private(pSelf_->pClassRef, GBL_INVALID_TYPE),
                     NULL);
    GBL_TEST_COMPARE(GBL_CTX_LAST_RESULT(), GBL_RESULT_ERROR_TYPE_MISMATCH);
    GBL_CTX_CLEAR_LAST_RECORD();

    GBL_TEST_COMPARE(GblClass_private(pSelf_->pClassRef, GBL_INTERFACE_TYPE),
                     NULL);
    GBL_TEST_COMPARE(GBL_CTX_LAST_RESULT(), GBL_RESULT_ERROR_TYPE_MISMATCH);
    GBL_CTX_CLEAR_LAST_RECORD();

    GBL_CTX_END();
}

static GBL_RESULT GblClassTestSuite_public_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblClassTestSuite_* pSelf_ = GBL_CLASS_TEST_SUITE_(pSelf);

    GBL_TEST_COMPARE(GblClass_public(pSelf_->pClassRef, GBL_STATIC_CLASS_TYPE),
                     NULL);

    GBL_CTX_END();
}

static GBL_RESULT GblClassTestSuite_properties_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblClassTestSuite_* pSelf_ = GBL_CLASS_TEST_SUITE_(pSelf);
    GBL_TEST_COMPARE(GblClass_typeOf            (pSelf_->pClassRef), GBL_STATIC_CLASS_TYPE);
    GBL_TEST_COMPARE(GblClass_size              (pSelf_->pClassRef), sizeof(GblClass));
    GBL_TEST_COMPARE(GblClass_privateSize       (pSelf_->pClassRef), 0);
    GBL_TEST_COMPARE(GblClass_totalSize         (pSelf_->pClassRef), sizeof(GblClass));
    GBL_TEST_COMPARE(GblClass_super             (pSelf_->pClassRef), NULL);
    GBL_TEST_COMPARE(GblClass_default           (pSelf_->pClassRef), pSelf_->pClassRef);
    GBL_TEST_COMPARE(GblClass_isDefault         (pSelf_->pClassRef), GBL_TRUE);
    GBL_TEST_COMPARE(GblClass_isOverridden      (pSelf_->pClassRef), GBL_FALSE);
    GBL_TEST_COMPARE(GblClass_isFloating        (pSelf_->pClassRef), GBL_FALSE);
    GBL_TEST_COMPARE(GblClass_isInterface       (pSelf_->pClassRef), GBL_FALSE);
    GBL_TEST_COMPARE(GblClass_isInterfaceImpl   (pSelf_->pClassRef), GBL_FALSE);
    GBL_TEST_COMPARE(GblClass_isOwned           (pSelf_->pClassRef), GBL_FALSE);
    GBL_TEST_COMPARE(GblClass_isInPlace         (pSelf_->pClassRef), GBL_FALSE);
    GBL_TEST_COMPARE(GBL_CLASS_TYPEOF(pSelf_->pClassRef), GBL_STATIC_CLASS_TYPE);
    GBL_TEST_COMPARE(GblClass_super(pSelf_->pClassRef), GBL_NULL);
    GBL_TEST_COMPARE(GblClass_default(pSelf_->pClassRef), pSelf_->pClassRef);

    GBL_CTX_END();
}

static GBL_RESULT GblClassTestSuite_unrefDefault_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblClassTestSuite_* pSelf_ = GBL_CLASS_TEST_SUITE_(pSelf);

    GBL_TEST_COMPARE(GblClass_unrefDefault(pSelf_->pClassRef),
                     pSelf_->initialStaticClassRefCount + 1);

    GBL_TEST_COMPARE(GblClass_unrefDefault(pSelf_->pClassRef),
                     pSelf_->initialStaticClassRefCount);

    GBL_CTX_END();
}

static GBL_RESULT GblClassTestSuite_createFloating_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblClassTestSuite_* pSelf_ = GBL_CLASS_TEST_SUITE_(pSelf);

    pSelf_->pClassRef = GblClass_createFloating(GBL_STATIC_CLASS_TYPE);
    GBL_CTX_VERIFY_LAST_RECORD();
    GBL_TEST_VERIFY(pSelf_->pClassRef);
    GBL_TEST_COMPARE(GBL_CLASS_TYPEOF(pSelf_->pClassRef), GBL_STATIC_CLASS_TYPE);

    GBL_TEST_VERIFY(!GblClass_isDefault(pSelf_->pClassRef));
    GBL_TEST_VERIFY(GblClass_isFloating(pSelf_->pClassRef));
    GBL_TEST_VERIFY(!GblClass_isOwned(pSelf_->pClassRef));
    GBL_TEST_VERIFY(!GblClass_isOverridden(pSelf_->pClassRef));
    GBL_TEST_VERIFY(!GblClass_isInPlace(pSelf_->pClassRef));
    GBL_TEST_COMPARE(GblType_classRefCount(GBL_STATIC_CLASS_TYPE),
                     pSelf_->initialStaticClassRefCount + 1);

    GBL_CTX_END();
}

static GBL_RESULT GblClassTestSuite_unrefDefaultFloating_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblClassTestSuite_* pSelf_ = GBL_CLASS_TEST_SUITE_(pSelf);

    GBL_TEST_EXPECT_ERROR();

    GBL_TEST_COMPARE(GblClass_unrefDefault(pSelf_->pClassRef),
                     pSelf_->initialStaticClassRefCount + 1);

    GBL_TEST_COMPARE(GBL_CTX_LAST_RESULT(), GBL_RESULT_ERROR_INVALID_OPERATION);
    GBL_CTX_CLEAR_LAST_RECORD();

    GBL_TEST_COMPARE(GBL_CLASS_TYPEOF(pSelf_->pClassRef), GBL_STATIC_CLASS_TYPE);
    GBL_TEST_VERIFY(GblClass_isFloating(pSelf_->pClassRef));

    GBL_CTX_END();
}

static GBL_RESULT GblClassTestSuite_destructFloatingInvalid_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblClassTestSuite_* pSelf_ = GBL_CLASS_TEST_SUITE_(pSelf);
    GBL_TEST_EXPECT_ERROR();
    GblClass_destructFloating(pSelf_->pClassRef);
    GBL_TEST_COMPARE(GBL_CTX_LAST_RESULT(), GBL_RESULT_ERROR_INVALID_OPERATION);
    GBL_CTX_CLEAR_LAST_RECORD();
    GBL_TEST_COMPARE(GblType_classRefCount(GBL_STATIC_CLASS_TYPE),
                     pSelf_->initialStaticClassRefCount + 1);

    GblClass_destructFloating(GblClass_default(pSelf_->pClassRef));
    GBL_TEST_COMPARE(GBL_CTX_LAST_RESULT(), GBL_RESULT_ERROR_INVALID_OPERATION);
    GBL_CTX_CLEAR_LAST_RECORD();
    GBL_TEST_COMPARE(GblType_classRefCount(GBL_STATIC_CLASS_TYPE),
                     pSelf_->initialStaticClassRefCount + 1);

    GBL_CTX_END();
}

static GBL_RESULT GblClassTestSuite_destroyFloating_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblClassTestSuite_* pSelf_ = GBL_CLASS_TEST_SUITE_(pSelf);
    GBL_CTX_VERIFY_CALL(GblClass_destroyFloating(pSelf_->pClassRef));
    GBL_TEST_COMPARE(GblType_classRefCount(GBL_STATIC_CLASS_TYPE),
                     pSelf_->initialStaticClassRefCount);
    GBL_CTX_END();
}

static GBL_RESULT GblClassTestSuite_constructFloating_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblClassTestSuite_* pSelf_ = GBL_CLASS_TEST_SUITE_(pSelf);

    pSelf_->pClassRef = GBL_CTX_MALLOC(sizeof(GblClass));
    GBL_CTX_VERIFY_CALL(GblClass_constructFloating(pSelf_->pClassRef, GBL_STATIC_CLASS_TYPE));
    GBL_CTX_VERIFY_LAST_RECORD();
    GBL_TEST_VERIFY(pSelf_->pClassRef);
    GBL_TEST_COMPARE(GBL_CLASS_TYPEOF(pSelf_->pClassRef), GBL_STATIC_CLASS_TYPE);

    GBL_TEST_VERIFY(!GblClass_isDefault(pSelf_->pClassRef));
    GBL_TEST_VERIFY(GblClass_isFloating(pSelf_->pClassRef));
    GBL_TEST_VERIFY(!GblClass_isOwned(pSelf_->pClassRef));
    GBL_TEST_VERIFY(!GblClass_isOverridden(pSelf_->pClassRef));
    GBL_TEST_VERIFY(GblClass_isInPlace(pSelf_->pClassRef));
    GBL_TEST_COMPARE(GblType_classRefCount(GBL_STATIC_CLASS_TYPE),
                     pSelf_->initialStaticClassRefCount + 1);

    GBL_CTX_END();
}

static GBL_RESULT GblClassTestSuite_destroyFloatingInvalid_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblClassTestSuite_* pSelf_ = GBL_CLASS_TEST_SUITE_(pSelf);
    GBL_TEST_EXPECT_ERROR();
    GblClass_destroyFloating(pSelf_->pClassRef);
    GBL_TEST_COMPARE(GBL_CTX_LAST_RESULT(), GBL_RESULT_ERROR_INVALID_OPERATION);
    GBL_CTX_CLEAR_LAST_RECORD();
    GBL_TEST_COMPARE(GblType_classRefCount(GBL_STATIC_CLASS_TYPE),
                     pSelf_->initialStaticClassRefCount + 1);

    GblClass_destroyFloating(GblClass_default(pSelf_->pClassRef));
    GBL_TEST_COMPARE(GBL_CTX_LAST_RESULT(), GBL_RESULT_ERROR_INVALID_OPERATION);
    GBL_CTX_CLEAR_LAST_RECORD();
    GBL_TEST_COMPARE(GblType_classRefCount(GBL_STATIC_CLASS_TYPE),
                     pSelf_->initialStaticClassRefCount + 1);

    GBL_CTX_END();
}

static GBL_RESULT GblClassTestSuite_destructFloating_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblClassTestSuite_* pSelf_ = GBL_CLASS_TEST_SUITE_(pSelf);
    GBL_CTX_VERIFY_CALL(GblClass_destructFloating(pSelf_->pClassRef));
    GBL_TEST_COMPARE(GblType_classRefCount(GBL_STATIC_CLASS_TYPE),
                     pSelf_->initialStaticClassRefCount);
    GBL_CTX_FREE(pSelf_->pClassRef);
    GBL_CTX_END();
}

GBL_EXPORT GblType GblClassTestSuite_type(void) {
    static GblType type = GBL_INVALID_TYPE;

    const static GblTestCase cases[] = {
        { "refInvalid",                 GblClassTestSuite_refInvalid_               },
        { "weakRefDefaultInvalid",      GblClassTest_weakRefDefaultInvalid_         },
        { "checkNullInvalid",           GblClassTestSuite_checkNullInvalid_         },
        { "castNullInvalid",            GblClassTestSuite_castNullInvalid_          },
        { "tryNullInvalid",             GblClassTestSuite_tryNullInvalid_           },
        { "privateNull",                GblClassTestSuite_privateNull_              },
        { "publicNull",                 GblClassTestSuite_publicNull_               },
        { "propertiesNull",             GblClassTestSuite_propertiesNull_           },
        { "unrefNull",                  GblClassTestSuite_unrefNull_                },
        { "createFloatingInvalid",      GblClassTestSuite_createFloatingInvalid_    },
        { "constructFloatingInvalid",   GblClassTestSuite_constructFloatingInvalid_ },
        { "destroyFloatingNull",        GblClassTestSuite_destroyFloatingNull_      },
        { "destructFloatingNull",       GblClassTestSuite_destructFloatingNull_     },
        { "refDefault",                 GblClassTestSuite_refDefault_               },
        { "weakRefDefault",             GblClassTest_weakRefDefault_                },
        { "check",                      GblClassTestSuite_check_                    },
        { "cast",                       GblClassTestSuite_cast_                     },
        { "castInvalid",                GblClassTestSuite_castInvalid_              },
        { "try",                        GblClassTestSuite_try_                      },
        { "tryInvalid",                 GblClassTestSuite_tryInvalid_               },
        { "private",                    GblClassTestSuite_private_                  },
        { "privateInvalid",             GblClassTestSuite_privateInvalid_           },
        { "public",                     GblClassTestSuite_public_                   },
        { "unrefDefault",               GblClassTestSuite_unrefDefault_             },
        { "properties",                 GblClassTestSuite_properties_               },
        { "createFloating",             GblClassTestSuite_createFloating_           },
        { "unrefDefaultFloating",       GblClassTestSuite_unrefDefaultFloating_     },
        { "destructFloatingInvalid",    GblClassTestSuite_destructFloatingInvalid_  },
        { "destroyFloating",            GblClassTestSuite_destroyFloating_          },
        { "constructFloating",          GblClassTestSuite_constructFloating_        },
        { "destroyFloatingInvalid",     GblClassTestSuite_destroyFloatingInvalid_   },
        { "destructFloating",           GblClassTestSuite_destructFloating_         },
        { NULL,                         NULL                                        }
    };

    const static GblTestSuiteVTable vTable = {
        .pFnSuiteInit   = GblClassTestSuite_init_,
        .pFnSuiteFinal  = GblClassTestSuite_final_,
        .pCases         = cases
    };

    if(type == GBL_INVALID_TYPE) {
        GBL_CTX_BEGIN(NULL);
        type = GblTestSuite_register(GblQuark_internStatic("ClassTestSuite"),
                                     &vTable,
                                     sizeof(GblClassTestSuite),
                                     sizeof(GblClassTestSuite_),
                                     GBL_TYPE_FLAGS_NONE);
        GBL_CTX_VERIFY_LAST_RECORD();
        GBL_CTX_END_BLOCK();
    }
    return type;
}
