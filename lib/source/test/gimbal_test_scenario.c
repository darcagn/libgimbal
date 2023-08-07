#include <gimbal/test/gimbal_test_scenario.h>
#include <gimbal/test/gimbal_test_suite.h>
#include <gimbal/utils/gimbal_timer.h>
#include <gimbal/allocators/gimbal_allocation_tracker.h>
#include <time.h>

#define GBL_TEST_SCENARIO_(inst)    (GBL_PRIVATE(GblTestScenario, inst))

typedef struct GblTestScenario_ {
    GblAllocationTracker*   pAllocTracker;
    GblTestSuite*           pCurSuite;
    const char*             pCurCase;
    double                  suiteMs;
    GblAllocationCounters   suiteAllocCounters;
    GblBool                 runningCase;
    GblBool                 expectError;
} GblTestScenario_;


size_t  GblTestScenario_suiteCount_(const GblTestScenario* pSelf) {
    return pSelf? GblObject_childCount(GBL_OBJECT(pSelf)) : 0;
}

static GBL_RESULT GblTestScenarioClass_begin_(GblTestScenario* pSelf) {
    GBL_CTX_BEGIN(pSelf);
    const char* pName = GblObject_name(GBL_OBJECT(pSelf));
    GBL_CTX_INFO("[GblTestScenario] Beginning Scenario: [%s]", pName? pName : "");
    GBL_CTX_PUSH();
    GblContext_logBuildInfo(GBL_CONTEXT(pSelf));
    GBL_CTX_END();
}

static GBL_RESULT GblTestScenarioClass_end_(GblTestScenario* pSelf) {
    GBL_CTX_BEGIN(pSelf);
    const char* pName = GblObject_name(GBL_OBJECT(pSelf));
    GblTestScenario_* pSelf_ = GBL_TEST_SCENARIO_(pSelf);
    GBL_CTX_INFO("[GblTestScenario] Ending Scenario: [%s]", pName? pName : "");
    GBL_CTX_PUSH();

    GBL_CTX_INFO("Runtime Statistics");
    GBL_CTX_PUSH();
    GBL_CTX_INFO("%-20s: %20.3f", "Total Time (ms)",     pSelf->totalTime);
    GBL_CTX_INFO("%-20s: %20u",   "Max Allocations",     pSelf_->pAllocTracker->maxAllocations);
    GBL_CTX_INFO("%-20s: %20u",   "Max Allocation Size", pSelf_->pAllocTracker->maxAllocationSize);
    GBL_CTX_INFO("%-20s: %20u",   "Max Allocated Bytes", pSelf_->pAllocTracker->maxBytes);
    GBL_CTX_INFO("%-20s: %20u",   "Remaining Allocs",    pSelf_->pAllocTracker->counters.allocsActive);
    GBL_CTX_INFO("%-20s: %20u",   "Remaining Bytes",     pSelf_->pAllocTracker->counters.bytesActive);
    GBL_CTX_INFO("%-20s: %20u",   "Seed",                gblSeed(0));
    GBL_CTX_POP(1);

    GBL_CTX_INFO("Test Suite Totals");
    GBL_CTX_PUSH();
    GBL_CTX_INFO("%-20s: %20u", "Total",   GblTestScenario_suiteCount_(pSelf));
    GBL_CTX_INFO("%-20s: %20u", "Passed",  pSelf->suitesPassed);
    GBL_CTX_INFO("%-20s: %20u", "Skipped", pSelf->suitesSkipped);
    GBL_CTX_INFO("%-20s: %20u", "Failed",  pSelf->suitesFailed);
    GBL_CTX_POP(1);

    GBL_CTX_INFO("Test Case Totals");
    GBL_CTX_PUSH();
    GBL_CTX_INFO("%-20s: %20u", "Total",   pSelf->caseCount);
    GBL_CTX_INFO("%-20s: %20u", "Passed",  pSelf->casesPassed);
    GBL_CTX_INFO("%-20s: %20u", "Skipped", pSelf->casesSkipped);
    GBL_CTX_INFO("%-20s: %20u", "Failed",  pSelf->casesFailed);
    GBL_CTX_POP(1);

    GBL_CTX_POP(1);

    GBL_CTX_INFO("********************* %s *********************",
                 !GBL_RESULT_SUCCESS(pSelf->result)? "[   FAIL   ]" : "[   PASS   ]");

    //GBL_CTX_VERIFY_CALL(GblAllocationTracker_logActive(pSelf_->pAllocTracker));

    GBL_CTX_END();
}

static GBL_RESULT GblTestScenarioClass_run_(GblTestScenario* pSelf, int argc, char* argv[]) {
    GBL_UNUSED(argc, argv);
    GBL_CTX_BEGIN(pSelf);

    GblContext*           pCtx   = GblObject_findContext(GBL_OBJECT(pSelf));
    GblTestScenarioClass* pClass = GBL_TEST_SCENARIO_GET_CLASS(pSelf);
    GblTestScenario_*     pSelf_ = GBL_TEST_SCENARIO_(pSelf);

    pSelf->result = GBL_RESULT_SUCCESS;
    GBL_CTX_VERIFY_CALL(pClass->pFnBegin(pSelf));

    for(GblTestSuite* pSuiteIt = GBL_AS(GblTestSuite, GblObject_childFirst(GBL_OBJECT(pSelf)));
        pSuiteIt              != NULL;
        pSuiteIt               = GBL_AS(GblTestSuite, GblObject_siblingNext(GBL_OBJECT(pSuiteIt))))
    {
        pSelf_->pCurSuite = pSuiteIt;

        GBL_CTX_RESULT() = GBL_RESULT_SUCCESS;

        GBL_CTX_VERIFY_CALL(pClass->pFnSuiteBegin(pSelf, pSuiteIt));

        GBL_CTX_INFO("%-12s: %s", "[ INIT      ]",
                     GblTestSuite_name(pSelf_->pCurSuite));

        GBL_CTX_PUSH();
        GBL_CTX_CALL(GblTestSuite_initSuite(pSuiteIt, pCtx));
        GBL_CTX_CLEAR_LAST_RECORD();
        GBL_CTX_POP(1);

        if(GBL_CTX_RESULT() == GBL_RESULT_SKIPPED) {
            GBL_CTX_INFO("%-12s: %s", "[      SKIP ]",
                         GblTestSuite_name(pSelf_->pCurSuite));

            pSelf->casesSkipped += GblTestSuite_caseCount(pSuiteIt);
            ++pSelf->suitesSkipped;

        } else if(GBL_RESULT_ERROR(GBL_CTX_RESULT())) {
            GBL_CTX_INFO("%-12s: %s", "[      FAIL ]",
                         GblTestSuite_name(pSelf_->pCurSuite));

            ++pSelf->suitesRun;
            pSelf->casesSkipped += GblTestSuite_caseCount(pSuiteIt);
            ++pSelf->suitesFailed;

        } else {

            ++pSelf->suitesRun;
            const size_t  caseCount = GblTestSuite_caseCount(pSuiteIt);
            GblBool suiteFailed = GBL_FALSE;

            for(size_t  idx = 0; idx < caseCount; ++idx) {

                pSelf_->pCurCase = GblTestSuite_caseName(pSuiteIt, idx);


                GBL_CTX_CALL(GblTestSuite_initCase(pSuiteIt, pCtx));
                GBL_CTX_CLEAR_LAST_RECORD();

                if(GBL_RESULT_ERROR(GBL_CTX_RESULT())) {
                    GBL_CTX_ERROR("[GblTestSuite] Failed to initialize test case[%s]: SKIPPING",
                                  pSelf_->pCurCase);
                    ++pSelf->casesSkipped;
                    suiteFailed = GBL_TRUE;
                } else {
                    GblTimer caseTimer;
                    GBL_CTX_INFO("%-12s: %s::%s", "[ RUN       ]",
                                 GblTestSuite_name(pSelf_->pCurSuite),
                                 pSelf_->pCurCase);

                    ++pSelf->casesRun;
                    GBL_CTX_PUSH();

                    GblTimer_start(&caseTimer);
                    pSelf_->runningCase = GBL_TRUE;

                    GBL_RESULT result = GblTestSuite_runCase(pSuiteIt, pCtx, idx);

                    pSelf_->expectError = GBL_FALSE;
                    pSelf_->runningCase = GBL_FALSE;
                    GblTimer_stop(&caseTimer);
                    pSelf_->suiteMs += GblTimer_elapsedMs(&caseTimer);

                    GBL_CTX_CLEAR_LAST_RECORD();
                    GBL_CTX_POP(1);

                    if(result == GBL_RESULT_SKIPPED) {
                        ++pSelf->casesSkipped;
                        GBL_CTX_INFO("%-12s: %s::%s", "[      SKIP ]",
                                     GblTestSuite_name(pSelf_->pCurSuite),
                                     pSelf_->pCurCase);
                    } else if(!GBL_RESULT_ERROR(result)) {
                        ++pSelf->casesPassed;
                        GBL_CTX_INFO("%-12s: %s::%s (%.3f ms)", "[      PASS ]",
                                     GblTestSuite_name(pSelf_->pCurSuite),
                                     pSelf_->pCurCase,
                                     GblTimer_elapsedMs(&caseTimer));
                    } else {
                        ++pSelf->casesFailed;
                        GBL_CTX_INFO("%-12s: %s::%s", "[      FAIL ]",
                                     GblTestSuite_name(pSelf_->pCurSuite),
                                     pSelf_->pCurCase);

                        suiteFailed = GBL_TRUE;
                        pSelf->casesSkipped += caseCount - idx - 1;
                        break;
                    }

                    GBL_CTX_CALL(GblTestSuite_finalCase(pSuiteIt, pCtx));
                    GBL_CTX_CLEAR_LAST_RECORD();
                    if(GBL_RESULT_ERROR(GBL_CTX_RESULT())) {
                        GBL_CTX_ERROR("[GblTestSuite] Failed to finalize test case: [%s]",
                                      pSelf_->pCurCase);
                        suiteFailed = GBL_TRUE; // log as failed suite, but not case, continue
                    }
                }
            }

            GBL_CTX_INFO("%-12s: %s", "[ FINAL     ]",
                         GblTestSuite_name(pSelf_->pCurSuite),
                         pSelf_->pCurCase);
            GBL_CTX_PUSH();
            GBL_CTX_CALL(GblTestSuite_finalSuite(pSuiteIt, pCtx));
            GBL_CTX_CLEAR_LAST_RECORD();
            GBL_CTX_POP(1);

            if(GBL_RESULT_ERROR(GBL_CTX_RESULT())) {

                GBL_CTX_INFO("%-12s: %s", "[      FAIL ]",
                             GblTestSuite_name(pSelf_->pCurSuite));

                suiteFailed = GBL_TRUE;
            }

            if(!suiteFailed) ++pSelf->suitesPassed;
            else ++pSelf->suitesFailed;
        }

        GBL_CTX_CALL(pClass->pFnSuiteEnd(pSelf, pSuiteIt));
    }

    pSelf->result = pSelf->casesFailed || pSelf->suitesFailed? GBL_RESULT_ERROR : GBL_RESULT_SUCCESS;
    GBL_CTX_CALL(pClass->pFnEnd(pSelf));

    GBL_CTX_END_BLOCK();
    return pSelf->result;
}

static GBL_RESULT GblTestScenarioClass_suiteBegin_(GblTestScenario* pSelf, const GblTestSuite* pSuite) {
    GBL_CTX_BEGIN(pSelf);

    const char* pSuiteName  = GblTestSuite_name(pSuite);
    GblTestScenario_* pSelf_ = GBL_TEST_SCENARIO_(pSelf);

    GblAllocationTracker_captureCounters(pSelf_->pAllocTracker,
                                         &pSelf_->suiteAllocCounters);

    pSelf_->suiteMs = 0.0;

    GBL_CTX_INFO("********* Starting TestSuite [%s] *********", pSuiteName);

    GBL_CTX_END();
}


static GBL_RESULT GblTestScenarioClass_suiteEnd_(GblTestScenario* pSelf, const GblTestSuite* pSuite) {
    GBL_CTX_BEGIN(pSelf);

    GblTestScenario_* pSelf_ = GBL_TEST_SCENARIO_(pSelf);
    const char* pSuiteName = GblTestSuite_name(pSuite);
    GblAllocationCounters  diffCounters;

    GblAllocationTracker_diffCounters(pSelf_->pAllocTracker,
                                      &pSelf_->suiteAllocCounters,
                                      &diffCounters);

    pSelf->totalTime += pSelf_->suiteMs;

    GBL_CTX_INFO("Totals: %u passed, %u failed, %u skipped, %.3fms, %d/%u leaked (%d/%u bytes)",
                 pSuite->casesPassed,
                 pSuite->casesFailed,
                 pSuite->casesSkipped,
                 pSelf_->suiteMs,
                 diffCounters.allocsActive,
                 diffCounters.allocEvents,
                 diffCounters.bytesActive,
                 diffCounters.bytesAllocated);
    GBL_CTX_INFO("********* Finished TestSuite [%s] *********", pSuiteName);
    GBL_CTX_END();
}

static GBL_RESULT GblTestScenarioClass_property_(const GblObject* pSelf, const GblProperty* pProp, GblVariant* pValue) {
    GBL_CTX_BEGIN(pSelf);
    GblTestScenario* pScenario = GBL_TEST_SCENARIO(pSelf);
    switch(pProp->id) {
    case GblTestScenario_Property_Id_testResult:
        GblVariant_setValueCopy(pValue, pProp->valueType, pScenario->result);
        break;
    case GblTestScenario_Property_Id_suiteCount:
        GblVariant_setValueCopy(pValue, pProp->valueType, GblTestScenario_suiteCount_(pScenario));
        break;
    case GblTestScenario_Property_Id_suitesRun:
        GblVariant_setValueCopy(pValue, pProp->valueType, pScenario->suitesRun);
        break;
    case GblTestScenario_Property_Id_suitesPassed:
        GblVariant_setValueCopy(pValue, pProp->valueType, pScenario->suitesPassed);
        break;
    case GblTestScenario_Property_Id_suitesFailed:
        GblVariant_setValueCopy(pValue, pProp->valueType, pScenario->suitesFailed);
        break;
    case GblTestScenario_Property_Id_suitesSkipped:
        GblVariant_setValueCopy(pValue, pProp->valueType, pScenario->suitesSkipped);
        break;
    case GblTestScenario_Property_Id_caseCount:
        GblVariant_setValueCopy(pValue, pProp->valueType, pScenario->caseCount);
        break;
    case GblTestScenario_Property_Id_casesRun:
        GblVariant_setValueCopy(pValue, pProp->valueType, pScenario->casesRun);
        break;
    case GblTestScenario_Property_Id_casesPassed:
        GblVariant_setValueCopy(pValue, pProp->valueType, pScenario->casesPassed);
        break;
    case GblTestScenario_Property_Id_casesFailed:
        GblVariant_setValueCopy(pValue, pProp->valueType, pScenario->casesFailed);
        break;
    case GblTestScenario_Property_Id_casesSkipped:
        GblVariant_setValueCopy(pValue, pProp->valueType, pScenario->casesSkipped);
        break;
    default: GBL_CTX_RECORD_SET(GBL_RESULT_ERROR_INVALID_PROPERTY,
                                "[GblTestScenario] Reading unhandled property: %s",
                                GblProperty_nameString(pProp));
    }
    GBL_CTX_END();
}



static GBL_RESULT GblTestScenarioClass_IAllocator_alloc_(GblIAllocator* pIAllocator, const GblStackFrame* pFrame, size_t  size, size_t  align, const char* pDbgStr, void** ppData) GBL_NOEXCEPT {
    GblContext* pParentCtx = GblContext_parentContext((GblContext*)pIAllocator);
    GBL_CTX_BEGIN(pParentCtx);
    GblContextClass* pCtxClass = GBL_CONTEXT_CLASS(GblClass_weakRefDefault(GBL_CONTEXT_TYPE));
    GblTestScenario* pSelf = (GblTestScenario*)pIAllocator;
    GblTestScenario_* pSelf_ = GBL_TEST_SCENARIO_(pSelf);
    GBL_CTX_VERIFY_CALL(pCtxClass->GblIAllocatorImpl.pFnAlloc(pIAllocator, pFrame, size, align, pDbgStr, ppData));
    GBL_CTX_VERIFY_CALL(GblAllocationTracker_allocEvent(pSelf_->pAllocTracker, *ppData, size, align, pDbgStr, pFrame->record.srcLocation));
    GBL_CTX_END();
}

static GBL_RESULT GblTestScenarioClass_IAllocator_realloc_(GblIAllocator* pIAllocator, const GblStackFrame* pFrame, void* pData, size_t  newSize, size_t  newAlign, void** ppNewData) GBL_NOEXCEPT {
    GblContext* pParentCtx = GblContext_parentContext((GblContext*)pIAllocator);
    GBL_CTX_BEGIN(pParentCtx);
    GblContextClass* pCtxClass = GBL_CONTEXT_CLASS(GblClass_weakRefDefault(GBL_CONTEXT_TYPE));
    GblTestScenario* pSelf = (GblTestScenario*)pIAllocator;
    GblTestScenario_* pSelf_ = GBL_TEST_SCENARIO_(pSelf);
    GBL_CTX_VERIFY_CALL(pCtxClass->GblIAllocatorImpl.pFnRealloc(pIAllocator, pFrame, pData, newSize, newAlign, ppNewData));
    GBL_CTX_VERIFY_CALL(GblAllocationTracker_reallocEvent(pSelf_->pAllocTracker, pData, *ppNewData, newSize, newAlign, pFrame->record.srcLocation));
    GBL_CTX_END();
}

static GBL_RESULT GblTestScenarioClass_IAllocator_free_(GblIAllocator* pIAllocator, const GblStackFrame* pFrame, void* pData) GBL_NOEXCEPT {
    GblContext* pParentCtx = GblContext_parentContext((GblContext*)pIAllocator);
    GBL_CTX_BEGIN(pParentCtx);
    GblContextClass* pCtxClass = GBL_CONTEXT_CLASS(GblClass_weakRefDefault(GBL_CONTEXT_TYPE));
    GblTestScenario* pSelf = (GblTestScenario*)pIAllocator;
    GblTestScenario_* pSelf_ = GBL_TEST_SCENARIO_(pSelf);
    GBL_CTX_VERIFY_CALL(pCtxClass->GblIAllocatorImpl.pFnFree(pIAllocator, pFrame, pData));
    GBL_CTX_VERIFY_CALL(GblAllocationTracker_freeEvent(pSelf_->pAllocTracker, pData, pFrame->record.srcLocation));
    GBL_CTX_END();
}

static GBL_RESULT GblTestScenarioClass_constructor_(GblObject* pObject) {
    GBL_CTX_BEGIN(pObject);
    GblContextClass* pCtxClass = GBL_CONTEXT_CLASS(GblClass_weakRefDefault(GBL_CONTEXT_TYPE));
    GBL_CTX_VERIFY_CALL(pCtxClass->base.pFnConstructor(pObject));

    GblTestScenario*    pSelf   = GBL_TEST_SCENARIO(pObject);
    GblTestScenario_*   pSelf_  = GBL_TEST_SCENARIO_(pSelf);

    GblContext* pParentCtx      = GblContext_parentContext(GBL_CONTEXT(pObject));
    pSelf_->pAllocTracker       = GblAllocationTracker_create(pParentCtx);

    GBL_CTX_END();
}

static GBL_RESULT GblTestScenarioClass_destructor_(GblBox* pRecord) {
    GBL_CTX_BEGIN(pRecord);

    GblTestScenario*    pSelf   = GBL_TEST_SCENARIO(pRecord);
    GblTestScenario_*   pSelf_  = GBL_TEST_SCENARIO_(pSelf);

    GBL_CTX_VERIFY_CALL(GblAllocationTracker_destroy(pSelf_->pAllocTracker));

    GblContextClass* pCtxClass = GBL_CONTEXT_CLASS(GblClass_weakRefDefault(GBL_CONTEXT_TYPE));
    GBL_CTX_VERIFY_CALL(pCtxClass->base.base.pFnDestructor(pRecord));
    GBL_CTX_END();
}


static GBL_RESULT GblTestScenarioClass_ILogger_write_(GblILogger* pILogger, const GblStackFrame* pFrame, GBL_LOG_LEVEL level, const char* pFmt, va_list varArgs){
    GblTestScenario_* pSelf_ = GBL_TEST_SCENARIO_(pILogger);
    if(pSelf_->expectError && (level & (GBL_LOG_LEVEL_WARNING|GBL_LOG_LEVEL_ERROR)))
        return GBL_RESULT_SUCCESS;

    GblTimer logTime;
    GblTimer_start(&logTime);

    GblContextClass* pClass = GBL_CONTEXT_CLASS(GblClass_weakRefDefault(GBL_CONTEXT_TYPE));

    const GBL_RESULT result = pClass->GblILoggerImpl.pFnWrite(pILogger, pFrame, level, pFmt, varArgs);

    if(pSelf_->runningCase) {
        GblTimer_stop(&logTime);
        pSelf_->suiteMs -= GblTimer_elapsedMs(&logTime);
    }

    return result;
}


static GBL_RESULT GblTestScenarioClass_init_(GblClass* pClass, const void* pUd) {
    GBL_UNUSED(pUd);
    GBL_CTX_BEGIN(NULL);

    if(!GblType_classRefCount(GBL_TEST_SCENARIO_TYPE)) {
        GBL_PROPERTIES_REGISTER(GblTestScenario);
    }

    GblTestScenarioClass* pSelfClass = GBL_TEST_SCENARIO_CLASS(pClass);
    pSelfClass->pFnBegin                               = GblTestScenarioClass_begin_;
    pSelfClass->pFnEnd                                 = GblTestScenarioClass_end_;
    pSelfClass->pFnRun                                 = GblTestScenarioClass_run_;
    pSelfClass->pFnSuiteBegin                          = GblTestScenarioClass_suiteBegin_;
    pSelfClass->pFnSuiteEnd                            = GblTestScenarioClass_suiteEnd_;
    pSelfClass->base.base.pFnConstructor               = GblTestScenarioClass_constructor_;
    pSelfClass->base.base.base.pFnDestructor           = GblTestScenarioClass_destructor_;
    pSelfClass->base.base.pFnProperty                  = GblTestScenarioClass_property_;
    pSelfClass->base.GblILoggerImpl.pFnWrite           = GblTestScenarioClass_ILogger_write_;
    pSelfClass->base.GblIAllocatorImpl.pFnAlloc        = GblTestScenarioClass_IAllocator_alloc_;
    pSelfClass->base.GblIAllocatorImpl.pFnRealloc      = GblTestScenarioClass_IAllocator_realloc_;
    pSelfClass->base.GblIAllocatorImpl.pFnFree         = GblTestScenarioClass_IAllocator_free_;
    GBL_CTX_END();
}

GBL_EXPORT GblTestScenario* GblTestScenario_create(const char* pName) {
    GblTestScenario* pScenario = NULL;
    GBL_CTX_BEGIN(NULL);
    GBL_CTX_VERIFY_POINTER(pName);
    pScenario = GBL_TEST_SCENARIO(GblObject_create(GBL_TEST_SCENARIO_TYPE,
                                                "name", pName,
                                                NULL));
    GBL_CTX_VERIFY_LAST_RECORD();
    GBL_CTX_END_BLOCK();
    return pScenario;
}

GBL_EXPORT GblRefCount GblTestScenario_unref(GblTestScenario* pSelf) {
    GblRefCount retVal = 0;

    GBL_CTX_BEGIN(NULL);
    GBL_CTX_VERIFY_POINTER(pSelf);
    GblTestScenario_*   pSelf_  = GBL_TEST_SCENARIO_(pSelf);

    GblObject* pNext;
    // not properly disposing of shit anything beyond the first entry!
    for(GblObject* pIt = GblObject_childFirst(GBL_OBJECT(pSelf));
        pIt != NULL;
        pIt = pNext)
    {
        pNext = GblObject_siblingNext(pIt);
        if(GblBox_unref(GBL_BOX(pIt)) != 0) {
            GBL_CTX_RECORD_SET(GBL_RESULT_ERROR_INVALID_OPERATION,
                               "[GblTestSuite] Destroy: Leaking unexpected existing references!");
        }
    }

    GBL_CTX_VERIFY_CALL(GblAllocationTracker_logActive(pSelf_->pAllocTracker));

    retVal = GBL_UNREF(pSelf);
    GBL_CTX_END_BLOCK();
    return retVal;
}

GBL_EXPORT GBL_RESULT GblTestScenario_enqueueSuite(GblTestScenario* pSelf, const GblTestSuite* pSuite) {
    GBL_CTX_BEGIN(pSelf);
    GblObject_addChild(GBL_OBJECT(pSelf), GBL_OBJECT(pSuite));
    pSelf->caseCount += GblTestSuite_caseCount(pSuite);
    GBL_CTX_VERIFY_LAST_RECORD();
    GBL_CTX_END();
}

GBL_EXPORT GblTestSuite* GblTestScenario_findSuite(const GblTestScenario* pSelf, const char* pName) {
    GblTestSuite* pSuite = NULL;
    GBL_CTX_BEGIN(pSelf);
    GBL_CTX_VERIFY_POINTER(pName);
    pSuite = GBL_AS(GblTestSuite, GblObject_findChildByName(GBL_OBJECT(pSelf), pName));
    GBL_CTX_END_BLOCK();
    return pSuite;
}

GBL_EXPORT GblTestSuite* GblTestScenario_currentSuite(const GblTestScenario* pSelf) {
    return pSelf? GBL_TEST_SCENARIO_(pSelf)->pCurSuite : NULL;
}

GBL_EXPORT const char* GblTestScenario_currentCase(const GblTestScenario* pSelf) {
    return pSelf? GBL_TEST_SCENARIO_(pSelf)->pCurCase : NULL;
}

GBL_EXPORT GBL_RESULT GblTestScenario_run(GblTestScenario* pSelf, int argc, char* argv[]) {
    GBL_CTX_BEGIN(pSelf);

    GblContext* pOldGlobalCtx = GblContext_global();
    GblContext_setGlobal(GBL_CONTEXT(pSelf));

    GBL_VCALL(GblTestScenario, pFnRun, pSelf, argc, argv);

    GblContext_setLogFilter(pOldGlobalCtx, GBL_LOG_LEVEL_WARNING|GBL_LOG_LEVEL_ERROR);
    GblContext_setGlobal(pOldGlobalCtx);

    GBL_CTX_VERIFY_LAST_RECORD();
    GBL_CTX_END();
}

GBL_EXPORT GblBool GblTestScenario_ran(const GblTestScenario* pSelf) {
    return pSelf && pSelf->result != GBL_RESULT_UNKNOWN;
}

GBL_EXPORT GblBool GblTestScenario_passed(const GblTestScenario* pSelf) {
    return GblTestScenario_ran(pSelf) && !GBL_RESULT_ERROR(pSelf->result);
}

GBL_EXPORT void GblTestScenario_expectError(const GblTestScenario* pSelf) {
    GBL_TEST_SCENARIO_(pSelf)->expectError = GBL_TRUE;
}

GBL_EXPORT GblType GblTestScenario_type(void) {
    static GblType type = GBL_INVALID_TYPE;

    static const GblTypeInfo typeInfo = {
        .pFnClassInit           = GblTestScenarioClass_init_,
        .classSize              = sizeof(GblTestScenarioClass),
        .instanceSize           = sizeof(GblTestScenario),
        .instancePrivateSize    = sizeof(GblTestScenario_)
    };

    if(type == GBL_INVALID_TYPE) {
        type = GblType_register(GblQuark_internStatic("GblTestScenario"),
                                GBL_CONTEXT_TYPE,
                                &typeInfo,
                                GBL_TYPE_FLAG_TYPEINFO_STATIC);
    }
    return type;
}
