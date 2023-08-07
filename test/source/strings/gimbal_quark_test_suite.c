#include "strings/gimbal_quark_test_suite.h"
#include <gimbal/test/gimbal_test_macros.h>
#include <gimbal/core/gimbal_ctx.h>
#include <gimbal/strings/gimbal_quark.h>

#define GBL_QUARK_TEST_SUITE_(inst)     (GBL_PRIVATE(GblQuarkTestSuite, inst))

typedef struct GblQuarkTestSuite_ {
    GblQuark    staticString;
    GblQuark    string;
    GblQuark    sizedString;
    GblQuark    pagedString;
    size_t      quarkStartCount;
    size_t      quarkStartBytes;
    size_t      quarkStartPages;
} GblQuarkTestSuite_;

static GBL_RESULT GblQuarkTestSuite_init_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);

    GblQuarkTestSuite_* pSelf_  = GBL_QUARK_TEST_SUITE_(pSelf);

    pSelf_->staticString        = GBL_QUARK_INVALID;
    pSelf_->string              = GBL_QUARK_INVALID;
    pSelf_->sizedString         = GBL_QUARK_INVALID;
    pSelf_->pagedString         = GBL_QUARK_INVALID;

    pSelf_->quarkStartCount     = GblQuark_count();
    pSelf_->quarkStartBytes     = GblQuark_bytesUsed();
    pSelf_->quarkStartPages     = GblQuark_pageCount();
    /*
    GBL_CTX_VERIFY_CALL(GblQuark_init(pCtx, 16, 2));

    GBL_TEST_COMPARE(GblQuark_pageCount(), 1);
    GBL_TEST_COMPARE(GblQuark_bytesUsed(), 0);
    GBL_TEST_COMPARE(GblQuark_count(), 0);
    GBL_TEST_COMPARE(GblQuark_context(), pCtx);
   */
    GBL_CTX_END();
}

static GBL_RESULT GblQuarkTestSuite_final_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_UNUSED(pSelf);
    GBL_CTX_BEGIN(pCtx);
    //GBL_CTX_VERIFY_CALL(GblQuark_final());
    GBL_CTX_END();
}

static GBL_RESULT GblQuarkTestSuite_fromStringStatic_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);

    GblQuarkTestSuite_* pSelf_  = GBL_QUARK_TEST_SUITE_(pSelf);
    pSelf_->staticString = GblQuark_fromStatic("StaticString");
    GBL_TEST_VERIFY(pSelf_->staticString != GBL_QUARK_INVALID);
    GBL_TEST_COMPARE(GblQuark_count(),       pSelf_->quarkStartCount + 1);
    GBL_TEST_COMPARE(GblQuark_bytesUsed(),   pSelf_->quarkStartBytes + 0);

    GBL_CTX_END();
}

static GBL_RESULT GblQuarkTestSuite_fromString_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);

    GblQuarkTestSuite_* pSelf_  = GBL_QUARK_TEST_SUITE_(pSelf);
    pSelf_->string = GblQuark_fromString("String");
    GBL_TEST_VERIFY(pSelf_->string != GBL_QUARK_INVALID);
    GBL_TEST_COMPARE(GblQuark_count(), pSelf_->quarkStartCount + 2);
    GBL_TEST_COMPARE(GblQuark_bytesUsed(), pSelf_->quarkStartBytes + 7);

    GBL_CTX_END();
}

static GBL_RESULT GblQuarkTestSuite_fromStringSized_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);

    GblQuarkTestSuite_* pSelf_  = GBL_QUARK_TEST_SUITE_(pSelf);
    pSelf_->sizedString = GblQuark_fromString("SizedStringxx", 11);
    GBL_TEST_VERIFY(pSelf_->sizedString != GBL_QUARK_INVALID);
    GBL_TEST_COMPARE(GblQuark_count(), pSelf_->quarkStartCount + 3);
    GBL_TEST_COMPARE(GblQuark_bytesUsed(), pSelf_->quarkStartBytes + 19);

    GBL_CTX_END();
}

static GBL_RESULT GblQuarkTestSuite_tryString_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblQuarkTestSuite_* pSelf_  = GBL_QUARK_TEST_SUITE_(pSelf);
    GblQuark quark = GblQuark_tryString(NULL);
    GBL_TEST_COMPARE(quark, GBL_QUARK_INVALID);

    quark = GblQuark_tryString("FakeNewsString");
    GBL_TEST_COMPARE(quark, GBL_QUARK_INVALID);

    quark = GblQuark_tryString("StaticString");
    GBL_TEST_COMPARE(quark, pSelf_->staticString);

    quark = GblQuark_tryString("String");
    GBL_TEST_COMPARE(quark, pSelf_->string);

    quark = GblQuark_tryString("SizedString");
    GBL_TEST_COMPARE(quark, pSelf_->sizedString);

    GBL_CTX_END();
}

static GBL_RESULT GblQuarkTestSuite_tryStringSized_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);

    GblQuarkTestSuite_* pSelf_  = GBL_QUARK_TEST_SUITE_(pSelf);
    GblQuark quark = GblQuark_tryString(NULL, 0);
    GBL_TEST_COMPARE(quark, GBL_QUARK_INVALID);

    quark = GblQuark_tryString("", 0);
    GBL_TEST_COMPARE(quark, GBL_QUARK_INVALID);

    quark = GblQuark_tryString(NULL, 128);
    GBL_TEST_COMPARE(quark, GBL_QUARK_INVALID);

    quark = GblQuark_tryString("FakeNewsString", 1);
    GBL_TEST_COMPARE(quark, GBL_QUARK_INVALID);

    quark = GblQuark_tryString("StaticString", 12);
    GBL_TEST_COMPARE(quark, pSelf_->staticString);

    quark = GblQuark_tryString("Stringr", 6);
    GBL_TEST_COMPARE(quark, pSelf_->string);

    quark = GblQuark_tryString("SizedStringxxyz", 11);
    GBL_TEST_COMPARE(quark, pSelf_->sizedString);

    quark = GblQuark_tryString("SizedString", 2);
    GBL_TEST_COMPARE(quark, GBL_QUARK_INVALID);

    GBL_CTX_END();
}

static GBL_RESULT GblQuarkTestSuite_extraPage_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblQuarkTestSuite_* pSelf_  = GBL_QUARK_TEST_SUITE_(pSelf);
    const size_t  pageCount = GblQuark_pageCount();

    const size_t  oldBytesUsed = GblQuark_bytesUsed();
    const size_t  pageFreeBytes = GblQuark_pageSize() - oldBytesUsed - 1;
    char* pBuffer = GBL_ALLOCA(pageFreeBytes);
    for(size_t  i = 0; i < pageFreeBytes; ++i) pBuffer[i] = 'a';
    const GblQuark wasterQuark = GblQuark_fromString(pBuffer, pageFreeBytes);

    GBL_TEST_VERIFY(wasterQuark != GBL_QUARK_INVALID);

    pSelf_->pagedString = GblQuark_fromString("PagedString");
    GBL_TEST_COMPARE(GblQuark_pageCount(), pageCount+1);
    GBL_TEST_VERIFY(pSelf_->pagedString != GBL_QUARK_INVALID);
    GBL_TEST_COMPARE(GblQuark_tryString("PagedString"), pSelf_->pagedString);
    GBL_TEST_COMPARE(GblQuark_bytesUsed(), oldBytesUsed + pageFreeBytes + 1 + 12);
    GBL_TEST_COMPARE(GblQuark_count(), pSelf_->quarkStartCount + 5);

    GBL_CTX_END();
}

static GBL_RESULT GblQuarkTestSuite_toString_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblQuarkTestSuite_* pSelf_  = GBL_QUARK_TEST_SUITE_(pSelf);
    GBL_TEST_COMPARE(GblQuark_toString(GBL_QUARK_INVALID), NULL);
    GBL_TEST_COMPARE(GblQuark_toString(pSelf_->staticString), "StaticString");
    GBL_TEST_COMPARE(GblQuark_toString(pSelf_->string),       "String");
    GBL_TEST_COMPARE(GblQuark_toString(pSelf_->sizedString),  "SizedString");
    GBL_TEST_COMPARE(GblQuark_toString(pSelf_->pagedString),  "PagedString");
    GBL_CTX_END();
}

static GBL_RESULT GblQuarkTestSuite_internString_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblQuarkTestSuite_* pSelf_  = GBL_QUARK_TEST_SUITE_(pSelf);
    GBL_TEST_COMPARE(GblQuark_internString(NULL), NULL);
    GBL_TEST_COMPARE(GblQuark_internString("InternString"), "InternString");
    const GblQuark tryQuark = GblQuark_tryString("InternString");
    GBL_TEST_COMPARE(GblQuark_toString(tryQuark), "InternString");
    GBL_TEST_COMPARE(GblQuark_count(), pSelf_->quarkStartCount + 6);
    GBL_CTX_END();
}

static GBL_RESULT GblQuarkTestSuite_internStringSized_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblQuarkTestSuite_* pSelf_  = GBL_QUARK_TEST_SUITE_(pSelf);
    GBL_TEST_COMPARE(GblQuark_internString(NULL, 0), NULL);
    GBL_TEST_COMPARE(GblQuark_internString("", 0), "");
    GBL_TEST_COMPARE(GblQuark_internString(NULL, 1), NULL);

    GBL_TEST_COMPARE(GblQuark_internString("InternStringSizedabc", 15), "InternStringSiz");
    const GblQuark tryQuark = GblQuark_tryString("InternStringSiz");
    GBL_TEST_COMPARE(GblQuark_toString(tryQuark), "InternStringSiz");
    GBL_TEST_COMPARE(GblQuark_count(), pSelf_->quarkStartCount + 8);
    GBL_CTX_END();
}

static GBL_RESULT GblQuarkTestSuite_internStringStatic_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblQuarkTestSuite_* pSelf_  = GBL_QUARK_TEST_SUITE_(pSelf);
    GBL_TEST_COMPARE(GblQuark_internStatic(NULL), NULL);
    GBL_TEST_COMPARE(GblQuark_internStatic("InternStatic"), "InternStatic");
    GBL_TEST_COMPARE(GblQuark_count(), pSelf_->quarkStartCount + 9);
    GBL_CTX_END();
}

GBL_EXPORT GblType GblQuarkTestSuite_type(void) {
    static GblType type = GBL_INVALID_TYPE;

    const static GblTestCase cases[] = {
        { "fromStringStatic",   GblQuarkTestSuite_fromStringStatic_     },
        { "fromString",         GblQuarkTestSuite_fromString_           },
        { "fromStringSized",    GblQuarkTestSuite_fromStringSized_      },
        { "tryString",          GblQuarkTestSuite_tryString_            },
        { "tryStringSized",     GblQuarkTestSuite_tryStringSized_       },
        { "extraPage",          GblQuarkTestSuite_extraPage_            },
        { "toString",           GblQuarkTestSuite_toString_             },
        { "internString",       GblQuarkTestSuite_internString_         },
        { "internStringSized",  GblQuarkTestSuite_internStringSized_    },
        { "internStringStatic", GblQuarkTestSuite_internStringStatic_   },
        { NULL,                 NULL                                    }
    };

    const static GblTestSuiteVTable vTable = {
        .pFnSuiteInit   = GblQuarkTestSuite_init_,
        .pFnSuiteFinal  = GblQuarkTestSuite_final_,
        .pCases         = cases
    };

    if(type == GBL_INVALID_TYPE) {
        type = GblTestSuite_register(GblQuark_internStatic("GblQuarkTestSuite"),
                                     &vTable,
                                     sizeof(GblQuarkTestSuite),
                                     sizeof(GblQuarkTestSuite_),
                                     GBL_TYPE_FLAGS_NONE);
    }
    return type;
}

