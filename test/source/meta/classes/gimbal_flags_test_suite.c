#include "meta/classes/gimbal_flags_test_suite.h"
#include <gimbal/test/gimbal_test_macros.h>
#include <gimbal/core/gimbal_ctx.h>
#include <gimbal/meta/classes/gimbal_flags.h>
#include <gimbal/meta/types/gimbal_variant.h>
#include <gimbal/meta/properties/gimbal_property.h>
#include <gimbal/strings/gimbal_string_buffer.h>

#define GBL_FLAGS_TEST_SUITE_(inst) (GBL_PRIVATE(GblFlagsTestSuite, inst))

typedef struct GblFlagsTestSuite_ {
    GblType         flagsType;
    GblFlagsClass*  pFlagsClass;
} GblFlagsTestSuite_;

GBL_DECLARE_FLAGS(PROPERTY_FLAGS) {
    READ    = 0x1,
    WRITE   = 0x2,
    LOAD    = 0x4,
    SAVE    = 0x8
};

static GBL_RESULT GblFlagsTestSuite_init_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblFlagsTestSuite_* pSelf_ = GBL_FLAGS_TEST_SUITE_(pSelf);
    memset(pSelf_, 0, sizeof(GblFlagsTestSuite_));
    GBL_CTX_END();
}

static GBL_RESULT GblFlagsTestSuite_final_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblFlagsTestSuite_* pSelf_ = GBL_FLAGS_TEST_SUITE_(pSelf);
    GBL_TEST_COMPARE(GblClass_unrefDefault(GBL_CLASS(pSelf_->pFlagsClass)), 0);
    GblType_unregister(pSelf_->flagsType);
    GBL_CTX_END();
}

static GBL_RESULT GblFlagsTestSuite_register_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblFlagsTestSuite_* pSelf_ = GBL_FLAGS_TEST_SUITE_(pSelf);
    pSelf_->flagsType = GblFlags_register("PropertyFlags",
                                   (const GblFlagEntry[]){
                                       GBL_FLAGS_ENTRY(READ,    "Read"),
                                       GBL_FLAGS_ENTRY(WRITE,   "Write"),
                                       GBL_FLAGS_ENTRY(LOAD,    "Load"),
                                       GBL_FLAGS_ENTRY(SAVE,    "Save"),
                                       GBL_FLAGS_ENTRY_LAST()
                                   });
    GBL_CTX_VERIFY_LAST_RECORD();

    GBL_TEST_VERIFY(pSelf_->flagsType != GBL_INVALID_TYPE);
    GBL_CTX_VERIFY_TYPE(pSelf_->flagsType, GBL_FLAGS_TYPE);

    pSelf_->pFlagsClass = (GblFlagsClass*)GblClass_refDefault(pSelf_->flagsType);
    GBL_TEST_VERIFY(pSelf_->pFlagsClass);
    GBL_CTX_END();
}

static GBL_RESULT GblFlagsTestSuite_class_name_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblFlagsTestSuite_* pSelf_ = GBL_FLAGS_TEST_SUITE_(pSelf);
    GBL_TEST_COMPARE(strcmp(GblFlagsClass_nameFromIndex(pSelf_->pFlagsClass,
                                                  1),
                       "WRITE"),
                0);
    GBL_TEST_COMPARE(strcmp(GblQuark_toString(GblFlagsClass_nameQuarkFromIndex(pSelf_->pFlagsClass,
                                                                         0)),
                       "READ"),
                0);
    GBL_TEST_COMPARE(strcmp(GblFlagsClass_nameFromValue(pSelf_->pFlagsClass,
                                                   LOAD),
                       "LOAD"),
                0);
    GBL_TEST_COMPARE(strcmp(GblQuark_toString(GblFlagsClass_nameQuarkFromValue(pSelf_->pFlagsClass,
                                                                          SAVE)),
                       "SAVE"),
                0);
    GBL_CTX_END();
}

static GBL_RESULT GblFlagsTestSuite_class_nick_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblFlagsTestSuite_* pSelf_ = GBL_FLAGS_TEST_SUITE_(pSelf);
    GBL_TEST_COMPARE(strcmp(GblFlagsClass_nickFromIndex(pSelf_->pFlagsClass,
                                                  1),
                       "Write"),
                0);
    GBL_TEST_COMPARE(strcmp(GblQuark_toString(GblFlagsClass_nickQuarkFromIndex(pSelf_->pFlagsClass,
                                                                         3)),
                       "Save"),
                0);
    GBL_TEST_COMPARE(strcmp(GblFlagsClass_nickFromValue(pSelf_->pFlagsClass,
                                                   READ),
                       "Read"),
                0);
    GBL_TEST_COMPARE(strcmp(GblQuark_toString(GblFlagsClass_nickQuarkFromValue(pSelf_->pFlagsClass,
                                                                          LOAD)),
                       "Load"),
                0);
    GBL_CTX_END();
}

static GBL_RESULT GblFlagsTestSuite_class_value_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblFlagsTestSuite_* pSelf_ = GBL_FLAGS_TEST_SUITE_(pSelf);
    GBL_TEST_COMPARE(GblFlagsClass_valueFromIndex(pSelf_->pFlagsClass, 1), WRITE);
    GBL_TEST_COMPARE(GblFlagsClass_valueFromName(pSelf_->pFlagsClass, "READ"), READ);
    GBL_TEST_COMPARE(GblFlagsClass_valueFromNameQuark(pSelf_->pFlagsClass, GblQuark_fromString("SAVE")), SAVE);
    GBL_TEST_COMPARE(GblFlagsClass_valueFromNick(pSelf_->pFlagsClass, "Load"), LOAD);
    GBL_TEST_COMPARE(GblFlagsClass_valueFromNickQuark(pSelf_->pFlagsClass, GblQuark_fromString("Write")), WRITE);
    GBL_TEST_VERIFY(GblFlagsClass_valueCheck(pSelf_->pFlagsClass, READ|WRITE|LOAD));
    GBL_TEST_VERIFY(!GblFlagsClass_valueCheck(pSelf_->pFlagsClass, SAVE|9999));
    GBL_CTX_END();
}

static GBL_RESULT GblFlagsTestSuite_class_string_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblFlagsTestSuite_* pSelf_ = GBL_FLAGS_TEST_SUITE_(pSelf);
    GblFlags flags = GblFlagsClass_valueFromString(pSelf_->pFlagsClass, "WRITE|SAVE");
    GBL_CTX_VERIFY_LAST_RECORD();

    GBL_TEST_COMPARE(flags, (WRITE|SAVE));

    GblStringBuffer buffer;
    GBL_CTX_VERIFY_CALL(GblStringBuffer_construct(&buffer));
    GBL_CTX_VERIFY_CALL(GblFlagsClass_valueAppendString(pSelf_->pFlagsClass,
                                                        flags,
                                                        &buffer));
    GBL_TEST_COMPARE(strcmp(GblStringBuffer_cString(&buffer),
                       "WRITE|SAVE"), 0);

    GBL_CTX_VERIFY_CALL(GblStringBuffer_destruct(&buffer));

    GBL_CTX_END();
}

static GBL_RESULT GblFlagsTestSuite_value_from_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblFlagsTestSuite_* pSelf_ = GBL_FLAGS_TEST_SUITE_(pSelf);
    GBL_TEST_COMPARE(GblFlags_fromName("READ", pSelf_->flagsType), READ);
    GBL_TEST_COMPARE(GblFlags_fromNameQuark(GblQuark_fromString("WRITE"), pSelf_->flagsType), WRITE);
    GBL_TEST_COMPARE(GblFlags_fromNick("Load", pSelf_->flagsType), LOAD);
    GBL_TEST_COMPARE(GblFlags_fromNickQuark(GblQuark_fromString("Save"), pSelf_->flagsType), SAVE);
    GBL_CTX_END();
}

static GBL_RESULT GblFlagsTestSuite_value_to_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblFlagsTestSuite_* pSelf_ = GBL_FLAGS_TEST_SUITE_(pSelf);
    GBL_TEST_COMPARE(strcmp(GblFlags_name(READ, pSelf_->flagsType), "READ"), 0);
    GBL_TEST_COMPARE(GblFlags_nameQuark(WRITE, pSelf_->flagsType), GblQuark_fromString("WRITE"));
    GBL_TEST_COMPARE(strcmp(GblFlags_nick(LOAD, pSelf_->flagsType), "Load"), 0);
    GBL_TEST_COMPARE(GblFlags_nickQuark(SAVE, pSelf_->flagsType), GblQuark_fromString("Save"));
    GBL_CTX_END();
}

static GBL_RESULT GblFlagsTestSuite_value_check_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblFlagsTestSuite_* pSelf_ = GBL_FLAGS_TEST_SUITE_(pSelf);
    GBL_TEST_VERIFY(!GblFlags_check(WRITE|99999, pSelf_->flagsType));
    GBL_TEST_VERIFY(GblFlags_check(SAVE, pSelf_->flagsType));
    GBL_CTX_END();
}

static GBL_RESULT GblFlagsTestSuite_value_string_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GblFlagsTestSuite_* pSelf_ = GBL_FLAGS_TEST_SUITE_(pSelf);
    GblFlags flags = GblFlags_fromString("READ|WRITE", pSelf_->flagsType);
    GBL_CTX_VERIFY_LAST_RECORD();

    GBL_TEST_COMPARE(flags, (READ|WRITE));

    GblStringBuffer buffer;
    GBL_CTX_VERIFY_CALL(GblStringBuffer_construct(&buffer));
    GBL_CTX_VERIFY_CALL(GblFlags_appendString(flags, pSelf_->flagsType, &buffer));
    GBL_TEST_COMPARE(strcmp(GblStringBuffer_cString(&buffer),
                       "READ|WRITE"), 0);

    GBL_CTX_VERIFY_CALL(GblStringBuffer_destruct(&buffer));

    GBL_CTX_END();
}

static GBL_RESULT GblFlagsTestSuite_variant_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);

    GblFlagsTestSuite_* pSelf_ = GBL_FLAGS_TEST_SUITE_(pSelf);
    GblVariant v = GBL_VARIANT_INIT;
    GBL_VARIANT(v2);
    const GblRefCount refCountBegin = GblType_classRefCount(pSelf_->flagsType);

    // Default constructor
    GBL_CTX_VERIFY_CALL(GblVariant_constructDefault(&v, pSelf_->flagsType));
    GBL_TEST_VERIFY(GblVariant_typeOf(&v) == pSelf_->flagsType);
    GBL_TEST_COMPARE(GblVariant_flags(&v), 0);

    // Copy Constructor
    GBL_CTX_VERIFY_CALL(GblVariant_constructCopy(&v2, &v));
    GBL_TEST_VERIFY(GblVariant_equals(&v, &v2));
    GBL_CTX_VERIFY_CALL(GblVariant_destruct(&v2));

    // Move Constructor
    GBL_CTX_VERIFY_CALL(GblVariant_constructMove(&v2, &v));
    GBL_TEST_VERIFY(GblVariant_equals(&v, &v2));
    GBL_CTX_VERIFY_CALL(GblVariant_destruct(&v2));
    GBL_CTX_VERIFY_CALL(GblVariant_destruct(&v));

    // Utility / Value Copy Constructor
    GBL_CTX_VERIFY_CALL(GblVariant_constructFlags(&v, pSelf_->flagsType, READ|WRITE));
    GBL_TEST_COMPARE(GblVariant_flags(&v), (READ|WRITE)); // Utility / Get Value
    GBL_CTX_VERIFY_CALL(GblVariant_destruct(&v));

    // Value Move Constructor
    GBL_CTX_VERIFY_CALL(GblVariant_constructValueMove(&v, pSelf_->flagsType, SAVE|LOAD));
    GBL_TEST_COMPARE(GblVariant_flags(&v), (SAVE|LOAD));

    // Utility / Value Set Copy
    GBL_CTX_VERIFY_CALL(GblVariant_setFlags(&v, pSelf_->flagsType, WRITE|LOAD));
    GBL_TEST_COMPARE(GblVariant_flags(&v), (WRITE|LOAD));

    // Value Set Move
    GBL_CTX_VERIFY_CALL(GblVariant_setValueMove(&v, pSelf_->flagsType, LOAD|SAVE));
    GBL_TEST_COMPARE(GblVariant_flags(&v), (LOAD|SAVE));

    // Value Get Move
    PROPERTY_FLAGS flags = 0;
    GBL_CTX_VERIFY_CALL(GblVariant_valueMove(&v, &flags));
    GBL_TEST_COMPARE(flags, (LOAD|SAVE));

    // Value Get Peek
    GblVariant_setFlags(&v, pSelf_->flagsType, WRITE);
    GBL_CTX_VERIFY_CALL(GblVariant_valuePeek(&v, &flags));
    GBL_TEST_COMPARE(flags, WRITE);

    // Destructor
    GBL_CTX_VERIFY_CALL(GblVariant_destruct(&v));

    // Reference leak check
    GBL_TEST_COMPARE(refCountBegin, GblType_classRefCount(pSelf_->flagsType));

    GBL_CTX_END();
}

static GBL_RESULT GblFlagsTestSuite_variant_fail_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);
    GBL_TEST_EXPECT_ERROR();

    GblFlagsTestSuite_* pSelf_ = GBL_FLAGS_TEST_SUITE_(pSelf);
    GblVariant v;
    GBL_TEST_COMPARE(GblVariant_constructValueCopy(&v, pSelf_->flagsType, 9999),
                GBL_RESULT_ERROR_OUT_OF_RANGE);
    GBL_TEST_COMPARE(GblVariant_constructValueMove(&v, pSelf_->flagsType, 9999),
                GBL_RESULT_ERROR_OUT_OF_RANGE);
    GBL_TEST_COMPARE(GblVariant_setValueMove(&v, pSelf_->flagsType, 9999),
                GBL_RESULT_ERROR_OUT_OF_RANGE);
    GBL_TEST_COMPARE(GblVariant_setValueCopy(&v, pSelf_->flagsType, 9999),
                GBL_RESULT_ERROR_OUT_OF_RANGE);
    GBL_CTX_END();
}

static GBL_RESULT GblFlagsTestSuite_variant_conversions_(GblTestSuite* pSelf, GblContext* pCtx) {
    GBL_CTX_BEGIN(pCtx);

    GblFlagsTestSuite_* pSelf_ = GBL_FLAGS_TEST_SUITE_(pSelf);
    GBL_VARIANT(variant);
    GBL_VARIANT(tVariant);
    const GblRefCount refCountBegin = GblType_classRefCount(pSelf_->flagsType);

    GBL_CTX_VERIFY_CALL(GblVariant_constructValueCopy(&variant, pSelf_->flagsType, (READ|WRITE)));

    // Bool
    GBL_CTX_VERIFY_CALL(GblVariant_constructBool(&tVariant, GBL_FALSE));
    GBL_CTX_VERIFY_CALL(GblVariant_convert(&variant, &tVariant));
    GBL_TEST_COMPARE(GblVariant_bool(&tVariant), GBL_TRUE);
    GBL_TEST_VERIFY(GblVariant_equals(&variant, &tVariant));

    // Uint8
    GBL_CTX_VERIFY_CALL(GblVariant_setUint8(&tVariant, 255));
    GBL_CTX_VERIFY_CALL(GblVariant_convert(&variant, &tVariant));
    GBL_TEST_COMPARE(GblVariant_uint8(&tVariant), (READ|WRITE));
    GBL_TEST_VERIFY(GblVariant_equals(&variant, &tVariant));

    // Uint16
    GBL_CTX_VERIFY_CALL(GblVariant_setUint16(&tVariant, 255));
    GBL_CTX_VERIFY_CALL(GblVariant_convert(&variant, &tVariant));
    GBL_TEST_COMPARE(GblVariant_uint16(&tVariant), (READ|WRITE));
    GBL_TEST_VERIFY(GblVariant_equals(&variant, &tVariant));

    // Int16
    GBL_CTX_VERIFY_CALL(GblVariant_setInt16(&tVariant, 255));
    GBL_CTX_VERIFY_CALL(GblVariant_convert(&variant, &tVariant));
    GBL_TEST_COMPARE(GblVariant_int16(&tVariant), (READ|WRITE));
    GBL_TEST_VERIFY(GblVariant_equals(&variant, &tVariant));

    // Uint32
    GBL_CTX_VERIFY_CALL(GblVariant_setUint32(&tVariant, 255));
    GBL_CTX_VERIFY_CALL(GblVariant_convert(&variant, &tVariant));
    GBL_TEST_COMPARE(GblVariant_uint32(&tVariant), (READ|WRITE));
    GBL_TEST_VERIFY(GblVariant_equals(&variant, &tVariant));

    // Int32
    GBL_CTX_VERIFY_CALL(GblVariant_setInt32(&tVariant, 255));
    GBL_CTX_VERIFY_CALL(GblVariant_convert(&variant, &tVariant));
    GBL_TEST_COMPARE(GblVariant_int32(&tVariant), (READ|WRITE));
    GBL_TEST_VERIFY(GblVariant_equals(&variant, &tVariant));

    // Uint64
    GBL_CTX_VERIFY_CALL(GblVariant_setUint64(&tVariant, 255));
    GBL_CTX_VERIFY_CALL(GblVariant_convert(&variant, &tVariant));
    GBL_TEST_COMPARE(GblVariant_uint64(&tVariant), (READ|WRITE));
    GBL_TEST_VERIFY(GblVariant_equals(&variant, &tVariant));

    // String
    GBL_CTX_VERIFY_CALL(GblVariant_setString(&tVariant, "trololo"));
    GBL_CTX_VERIFY_CALL(GblVariant_convert(&variant, &tVariant));
    GBL_TEST_VERIFY(GblStringView_equals(GblVariant_stringView(&tVariant), GBL_STRV("READ|WRITE")));
    GBL_TEST_VERIFY(GblVariant_equals(&variant, &tVariant));

    // FROM String
    GBL_CTX_VERIFY_CALL(GblVariant_setString(&tVariant, "READ|WRITE|LOAD|SAVE"));
    GBL_TEST_VERIFY(GblVariant_canConvert(GblVariant_typeOf(&tVariant), GblVariant_typeOf(&variant)));
    GBL_CTX_VERIFY_CALL(GblVariant_convert(&tVariant, &variant));
    GBL_CTX_VERIFY_CALL(GblVariant_equals(&tVariant, &variant));
    GBL_TEST_COMPARE(GblVariant_flags(&variant), (READ|WRITE|LOAD|SAVE));

    GBL_CTX_VERIFY_CALL(GblVariant_destruct(&tVariant));
    GBL_CTX_VERIFY_CALL(GblVariant_destruct(&variant));

    // Reference leak check
    GBL_TEST_COMPARE(refCountBegin, GblType_classRefCount(pSelf_->flagsType));

    GBL_CTX_END();
}

GBL_EXPORT GblType GblFlagsTestSuite_type(void) {
    static GblType type = GBL_INVALID_TYPE;

    const static GblTestCase cases[] = {
        { "register",           GblFlagsTestSuite_register_              },
        { "className",          GblFlagsTestSuite_class_name_            },
        { "classNick",          GblFlagsTestSuite_class_nick_            },
        { "classvalue",         GblFlagsTestSuite_class_value_           },
        { "classString",        GblFlagsTestSuite_class_string_          },
        { "valueFrom",          GblFlagsTestSuite_value_from_            },
        { "valueTo",            GblFlagsTestSuite_value_to_              },
        { "valueCheck",         GblFlagsTestSuite_value_check_           },
        { "valueString",        GblFlagsTestSuite_value_string_          },
        { "variant",            GblFlagsTestSuite_variant_               },
        { "variantFail",        GblFlagsTestSuite_variant_fail_          },
        { "variantConversions", GblFlagsTestSuite_variant_conversions_   },
        { NULL,                 NULL                                     }
    };

    const static GblTestSuiteVTable vTable = {
        .pFnSuiteInit   = GblFlagsTestSuite_init_,
        .pFnSuiteFinal  = GblFlagsTestSuite_final_,
        .pCases         = cases
    };

    if(type == GBL_INVALID_TYPE) {
        GBL_CTX_BEGIN(NULL);
        type = GblTestSuite_register(GblQuark_internStringStatic("FlagsTestSuite"),
                                     &vTable,
                                     sizeof(GblFlagsTestSuite),
                                     sizeof(GblFlagsTestSuite_),
                                     GBL_TYPE_FLAGS_NONE);
        GBL_CTX_VERIFY_LAST_RECORD();
        GBL_CTX_END_BLOCK();
    }
    return type;
}
