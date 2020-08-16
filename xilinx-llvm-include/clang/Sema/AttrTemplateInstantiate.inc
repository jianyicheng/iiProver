/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* Template instantiation code for attributes                                 *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

namespace clang {
namespace sema {

Attr *instantiateTemplateAttribute(const Attr *At, ASTContext &C, Sema &S,
        const MultiLevelTemplateArgumentList &TemplateArgs) {
  switch (At->getKind()) {
    case attr::AMDGPUFlatWorkGroupSize: {
      const auto *A = cast<AMDGPUFlatWorkGroupSizeAttr>(At);
      return A->clone(C);
    }
    case attr::AMDGPUNumSGPR: {
      const auto *A = cast<AMDGPUNumSGPRAttr>(At);
      return A->clone(C);
    }
    case attr::AMDGPUNumVGPR: {
      const auto *A = cast<AMDGPUNumVGPRAttr>(At);
      return A->clone(C);
    }
    case attr::AMDGPUWavesPerEU: {
      const auto *A = cast<AMDGPUWavesPerEUAttr>(At);
      return A->clone(C);
    }
    case attr::ARMInterrupt: {
      const auto *A = cast<ARMInterruptAttr>(At);
      return A->clone(C);
    }
    case attr::AVRInterrupt: {
      const auto *A = cast<AVRInterruptAttr>(At);
      return A->clone(C);
    }
    case attr::AVRSignal: {
      const auto *A = cast<AVRSignalAttr>(At);
      return A->clone(C);
    }
    case attr::AXISAdaptor: {
      const auto *A = cast<AXISAdaptorAttr>(At);
      return A->clone(C);
    }
    case attr::AbiTag: {
      const auto *A = cast<AbiTagAttr>(At);
      return A->clone(C);
    }
    case attr::AcquireCapability: {
      const auto *A = cast<AcquireCapabilityAttr>(At);
      auto *tempInstArgs = new (C, 16) Expr *[A->args_size()];
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        Expr * *TI = tempInstArgs;
        Expr * *I = A->args_begin();
        Expr * *E = A->args_end();
        for (; I != E; ++I, ++TI) {
          ExprResult Result = S.SubstExpr(*I, TemplateArgs);
          *TI = Result.getAs<Expr>();
        }
      }
      return new (C) AcquireCapabilityAttr(A->getLocation(), C, tempInstArgs, A->args_size(), A->getSpellingListIndex());
    }
    case attr::AcquiredAfter: {
      const auto *A = cast<AcquiredAfterAttr>(At);
      auto *tempInstArgs = new (C, 16) Expr *[A->args_size()];
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        Expr * *TI = tempInstArgs;
        Expr * *I = A->args_begin();
        Expr * *E = A->args_end();
        for (; I != E; ++I, ++TI) {
          ExprResult Result = S.SubstExpr(*I, TemplateArgs);
          *TI = Result.getAs<Expr>();
        }
      }
      return new (C) AcquiredAfterAttr(A->getLocation(), C, tempInstArgs, A->args_size(), A->getSpellingListIndex());
    }
    case attr::AcquiredBefore: {
      const auto *A = cast<AcquiredBeforeAttr>(At);
      auto *tempInstArgs = new (C, 16) Expr *[A->args_size()];
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        Expr * *TI = tempInstArgs;
        Expr * *I = A->args_begin();
        Expr * *E = A->args_end();
        for (; I != E; ++I, ++TI) {
          ExprResult Result = S.SubstExpr(*I, TemplateArgs);
          *TI = Result.getAs<Expr>();
        }
      }
      return new (C) AcquiredBeforeAttr(A->getLocation(), C, tempInstArgs, A->args_size(), A->getSpellingListIndex());
    }
    case attr::Alias: {
      const auto *A = cast<AliasAttr>(At);
      return A->clone(C);
    }
    case attr::AlignMac68k: {
      const auto *A = cast<AlignMac68kAttr>(At);
      return A->clone(C);
    }
    case attr::AlignValue: {
      const auto *A = cast<AlignValueAttr>(At);
      return A->clone(C);
    }
    case attr::Aligned: {
      const auto *A = cast<AlignedAttr>(At);
      return A->clone(C);
    }
    case attr::AllocAlign: {
      const auto *A = cast<AllocAlignAttr>(At);
      return A->clone(C);
    }
    case attr::AllocSize: {
      const auto *A = cast<AllocSizeAttr>(At);
      return new (C) AllocSizeAttr(A->getLocation(), C, A->getElemSizeParam(), A->getNumElemsParam(), A->getSpellingListIndex());
    }
    case attr::AlwaysInline: {
      const auto *A = cast<AlwaysInlineAttr>(At);
      return A->clone(C);
    }
    case attr::AnalyzerNoReturn: {
      const auto *A = cast<AnalyzerNoReturnAttr>(At);
      return A->clone(C);
    }
    case attr::Annotate: {
      const auto *A = cast<AnnotateAttr>(At);
      return A->clone(C);
    }
    case attr::AnyX86Interrupt: {
      const auto *A = cast<AnyX86InterruptAttr>(At);
      return A->clone(C);
    }
    case attr::AnyX86NoCallerSavedRegisters: {
      const auto *A = cast<AnyX86NoCallerSavedRegistersAttr>(At);
      return A->clone(C);
    }
    case attr::ArcWeakrefUnavailable: {
      const auto *A = cast<ArcWeakrefUnavailableAttr>(At);
      return A->clone(C);
    }
    case attr::ArgumentWithTypeTag: {
      const auto *A = cast<ArgumentWithTypeTagAttr>(At);
      return A->clone(C);
    }
    case attr::AsmLabel: {
      const auto *A = cast<AsmLabelAttr>(At);
      return A->clone(C);
    }
    case attr::AssertCapability: {
      const auto *A = cast<AssertCapabilityAttr>(At);
      auto *tempInstArgs = new (C, 16) Expr *[A->args_size()];
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        Expr * *TI = tempInstArgs;
        Expr * *I = A->args_begin();
        Expr * *E = A->args_end();
        for (; I != E; ++I, ++TI) {
          ExprResult Result = S.SubstExpr(*I, TemplateArgs);
          *TI = Result.getAs<Expr>();
        }
      }
      return new (C) AssertCapabilityAttr(A->getLocation(), C, tempInstArgs, A->args_size(), A->getSpellingListIndex());
    }
    case attr::AssertExclusiveLock: {
      const auto *A = cast<AssertExclusiveLockAttr>(At);
      auto *tempInstArgs = new (C, 16) Expr *[A->args_size()];
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        Expr * *TI = tempInstArgs;
        Expr * *I = A->args_begin();
        Expr * *E = A->args_end();
        for (; I != E; ++I, ++TI) {
          ExprResult Result = S.SubstExpr(*I, TemplateArgs);
          *TI = Result.getAs<Expr>();
        }
      }
      return new (C) AssertExclusiveLockAttr(A->getLocation(), C, tempInstArgs, A->args_size(), A->getSpellingListIndex());
    }
    case attr::AssertSharedLock: {
      const auto *A = cast<AssertSharedLockAttr>(At);
      auto *tempInstArgs = new (C, 16) Expr *[A->args_size()];
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        Expr * *TI = tempInstArgs;
        Expr * *I = A->args_begin();
        Expr * *E = A->args_end();
        for (; I != E; ++I, ++TI) {
          ExprResult Result = S.SubstExpr(*I, TemplateArgs);
          *TI = Result.getAs<Expr>();
        }
      }
      return new (C) AssertSharedLockAttr(A->getLocation(), C, tempInstArgs, A->args_size(), A->getSpellingListIndex());
    }
    case attr::AssumeAligned: {
      const auto *A = cast<AssumeAlignedAttr>(At);
      return A->clone(C);
    }
    case attr::Availability: {
      const auto *A = cast<AvailabilityAttr>(At);
      return A->clone(C);
    }
    case attr::BRAMAdaptor: {
      const auto *A = cast<BRAMAdaptorAttr>(At);
      Expr * tempInstRAMType;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getRAMType(), TemplateArgs);
        tempInstRAMType = Result.getAs<Expr>();
      }
      Expr * tempInstRAMImpl;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getRAMImpl(), TemplateArgs);
        tempInstRAMImpl = Result.getAs<Expr>();
      }
      Expr * tempInstLatency;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getLatency(), TemplateArgs);
        tempInstLatency = Result.getAs<Expr>();
      }
      return new (C) BRAMAdaptorAttr(A->getLocation(), C, A->getName(), A->getMode(), tempInstRAMType, tempInstRAMImpl, tempInstLatency, A->getSpellingListIndex());
    }
    case attr::Blocks: {
      const auto *A = cast<BlocksAttr>(At);
      return A->clone(C);
    }
    case attr::C11NoReturn: {
      const auto *A = cast<C11NoReturnAttr>(At);
      return A->clone(C);
    }
    case attr::CDecl: {
      const auto *A = cast<CDeclAttr>(At);
      return A->clone(C);
    }
    case attr::CFAuditedTransfer: {
      const auto *A = cast<CFAuditedTransferAttr>(At);
      return A->clone(C);
    }
    case attr::CFConsumed: {
      const auto *A = cast<CFConsumedAttr>(At);
      return A->clone(C);
    }
    case attr::CFReturnsNotRetained: {
      const auto *A = cast<CFReturnsNotRetainedAttr>(At);
      return A->clone(C);
    }
    case attr::CFReturnsRetained: {
      const auto *A = cast<CFReturnsRetainedAttr>(At);
      return A->clone(C);
    }
    case attr::CFUnknownTransfer: {
      const auto *A = cast<CFUnknownTransferAttr>(At);
      return A->clone(C);
    }
    case attr::CUDAConstant: {
      const auto *A = cast<CUDAConstantAttr>(At);
      return A->clone(C);
    }
    case attr::CUDADevice: {
      const auto *A = cast<CUDADeviceAttr>(At);
      return A->clone(C);
    }
    case attr::CUDAGlobal: {
      const auto *A = cast<CUDAGlobalAttr>(At);
      return A->clone(C);
    }
    case attr::CUDAHost: {
      const auto *A = cast<CUDAHostAttr>(At);
      return A->clone(C);
    }
    case attr::CUDAInvalidTarget: {
      const auto *A = cast<CUDAInvalidTargetAttr>(At);
      return A->clone(C);
    }
    case attr::CUDALaunchBounds: {
      const auto *A = cast<CUDALaunchBoundsAttr>(At);
      return A->clone(C);
    }
    case attr::CUDAShared: {
      const auto *A = cast<CUDASharedAttr>(At);
      return A->clone(C);
    }
    case attr::CXX11NoReturn: {
      const auto *A = cast<CXX11NoReturnAttr>(At);
      return A->clone(C);
    }
    case attr::CallableWhen: {
      const auto *A = cast<CallableWhenAttr>(At);
      return A->clone(C);
    }
    case attr::Capability: {
      const auto *A = cast<CapabilityAttr>(At);
      return A->clone(C);
    }
    case attr::CapturedRecord: {
      const auto *A = cast<CapturedRecordAttr>(At);
      return A->clone(C);
    }
    case attr::CarriesDependency: {
      const auto *A = cast<CarriesDependencyAttr>(At);
      return A->clone(C);
    }
    case attr::Cleanup: {
      const auto *A = cast<CleanupAttr>(At);
      return A->clone(C);
    }
    case attr::CodeGenType: {
      const auto *A = cast<CodeGenTypeAttr>(At);
      return A->clone(C);
    }
    case attr::Cold: {
      const auto *A = cast<ColdAttr>(At);
      return A->clone(C);
    }
    case attr::Common: {
      const auto *A = cast<CommonAttr>(At);
      return A->clone(C);
    }
    case attr::Const: {
      const auto *A = cast<ConstAttr>(At);
      return A->clone(C);
    }
    case attr::Constructor: {
      const auto *A = cast<ConstructorAttr>(At);
      return A->clone(C);
    }
    case attr::Consumable: {
      const auto *A = cast<ConsumableAttr>(At);
      return A->clone(C);
    }
    case attr::ConsumableAutoCast: {
      const auto *A = cast<ConsumableAutoCastAttr>(At);
      return A->clone(C);
    }
    case attr::ConsumableSetOnRead: {
      const auto *A = cast<ConsumableSetOnReadAttr>(At);
      return A->clone(C);
    }
    case attr::Convergent: {
      const auto *A = cast<ConvergentAttr>(At);
      return A->clone(C);
    }
    case attr::DLLExport: {
      const auto *A = cast<DLLExportAttr>(At);
      return A->clone(C);
    }
    case attr::DLLImport: {
      const auto *A = cast<DLLImportAttr>(At);
      return A->clone(C);
    }
    case attr::Deprecated: {
      const auto *A = cast<DeprecatedAttr>(At);
      return A->clone(C);
    }
    case attr::Destructor: {
      const auto *A = cast<DestructorAttr>(At);
      return A->clone(C);
    }
    case attr::DiagnoseIf: {
      const auto *A = cast<DiagnoseIfAttr>(At);
      Expr * tempInstCond;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getCond(), TemplateArgs);
        tempInstCond = Result.getAs<Expr>();
      }
      return new (C) DiagnoseIfAttr(A->getLocation(), C, tempInstCond, A->getMessage(), A->getDiagnosticType(), A->getArgDependent(), A->getParent(), A->getSpellingListIndex());
    }
    case attr::DisableTailCalls: {
      const auto *A = cast<DisableTailCallsAttr>(At);
      return A->clone(C);
    }
    case attr::EmptyBases: {
      const auto *A = cast<EmptyBasesAttr>(At);
      return A->clone(C);
    }
    case attr::EnableIf: {
      const auto *A = cast<EnableIfAttr>(At);
      Expr * tempInstCond;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getCond(), TemplateArgs);
        tempInstCond = Result.getAs<Expr>();
      }
      return new (C) EnableIfAttr(A->getLocation(), C, tempInstCond, A->getMessage(), A->getSpellingListIndex());
    }
    case attr::EnumExtensibility: {
      const auto *A = cast<EnumExtensibilityAttr>(At);
      return A->clone(C);
    }
    case attr::ExclusiveTrylockFunction: {
      const auto *A = cast<ExclusiveTrylockFunctionAttr>(At);
      Expr * tempInstSuccessValue;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getSuccessValue(), TemplateArgs);
        tempInstSuccessValue = Result.getAs<Expr>();
      }
      auto *tempInstArgs = new (C, 16) Expr *[A->args_size()];
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        Expr * *TI = tempInstArgs;
        Expr * *I = A->args_begin();
        Expr * *E = A->args_end();
        for (; I != E; ++I, ++TI) {
          ExprResult Result = S.SubstExpr(*I, TemplateArgs);
          *TI = Result.getAs<Expr>();
        }
      }
      return new (C) ExclusiveTrylockFunctionAttr(A->getLocation(), C, tempInstSuccessValue, tempInstArgs, A->args_size(), A->getSpellingListIndex());
    }
    case attr::ExternalSourceSymbol: {
      const auto *A = cast<ExternalSourceSymbolAttr>(At);
      return A->clone(C);
    }
    case attr::FPGAAddressInterface: {
      const auto *A = cast<FPGAAddressInterfaceAttr>(At);
      return A->clone(C);
    }
    case attr::FPGADataFootPrintHint: {
      const auto *A = cast<FPGADataFootPrintHintAttr>(At);
      Expr * tempInstDepth;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getDepth(), TemplateArgs);
        tempInstDepth = Result.getAs<Expr>();
      }
      return new (C) FPGADataFootPrintHintAttr(A->getLocation(), C, tempInstDepth, A->getSpellingListIndex());
    }
    case attr::FPGAFunctionCtrlInterface: {
      const auto *A = cast<FPGAFunctionCtrlInterfaceAttr>(At);
      return A->clone(C);
    }
    case attr::FPGARegister: {
      const auto *A = cast<FPGARegisterAttr>(At);
      return A->clone(C);
    }
    case attr::FPGAResourceHint: {
      const auto *A = cast<FPGAResourceHintAttr>(At);
      Expr * tempInstLatency;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getLatency(), TemplateArgs);
        tempInstLatency = Result.getAs<Expr>();
      }
      return new (C) FPGAResourceHintAttr(A->getLocation(), C, A->getCore(), A->getMetadata(), tempInstLatency, A->getSpellingListIndex());
    }
    case attr::FPGAResourceLimitHint: {
      const auto *A = cast<FPGAResourceLimitHintAttr>(At);
      Expr * tempInstLimit;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getLimit(), TemplateArgs);
        tempInstLimit = Result.getAs<Expr>();
      }
      return new (C) FPGAResourceLimitHintAttr(A->getLocation(), C, A->getInstanceName(), A->getInstance(), tempInstLimit, A->getSpellingListIndex());
    }
    case attr::FPGAScalarInterface: {
      const auto *A = cast<FPGAScalarInterfaceAttr>(At);
      return A->clone(C);
    }
    case attr::FPGAScalarInterfaceWrapper: {
      const auto *A = cast<FPGAScalarInterfaceWrapperAttr>(At);
      Expr * tempInstOffset;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getOffset(), TemplateArgs);
        tempInstOffset = Result.getAs<Expr>();
      }
      return new (C) FPGAScalarInterfaceWrapperAttr(A->getLocation(), C, A->getMode(), A->getAdaptor(), tempInstOffset, A->getSpellingListIndex());
    }
    case attr::FPGASignalName: {
      const auto *A = cast<FPGASignalNameAttr>(At);
      return A->clone(C);
    }
    case attr::FallThrough: {
      const auto *A = cast<FallThroughAttr>(At);
      return A->clone(C);
    }
    case attr::FastCall: {
      const auto *A = cast<FastCallAttr>(At);
      return A->clone(C);
    }
    case attr::Final: {
      const auto *A = cast<FinalAttr>(At);
      return A->clone(C);
    }
    case attr::FlagEnum: {
      const auto *A = cast<FlagEnumAttr>(At);
      return A->clone(C);
    }
    case attr::Flatten: {
      const auto *A = cast<FlattenAttr>(At);
      return A->clone(C);
    }
    case attr::Format: {
      const auto *A = cast<FormatAttr>(At);
      return A->clone(C);
    }
    case attr::FormatArg: {
      const auto *A = cast<FormatArgAttr>(At);
      return A->clone(C);
    }
    case attr::GNUInline: {
      const auto *A = cast<GNUInlineAttr>(At);
      return A->clone(C);
    }
    case attr::GuardedBy: {
      const auto *A = cast<GuardedByAttr>(At);
      Expr * tempInstArg;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getArg(), TemplateArgs);
        tempInstArg = Result.getAs<Expr>();
      }
      return new (C) GuardedByAttr(A->getLocation(), C, tempInstArg, A->getSpellingListIndex());
    }
    case attr::GuardedVar: {
      const auto *A = cast<GuardedVarAttr>(At);
      return A->clone(C);
    }
    case attr::Hot: {
      const auto *A = cast<HotAttr>(At);
      return A->clone(C);
    }
    case attr::IBAction: {
      const auto *A = cast<IBActionAttr>(At);
      return A->clone(C);
    }
    case attr::IBOutlet: {
      const auto *A = cast<IBOutletAttr>(At);
      return A->clone(C);
    }
    case attr::IBOutletCollection: {
      const auto *A = cast<IBOutletCollectionAttr>(At);
      return A->clone(C);
    }
    case attr::IFunc: {
      const auto *A = cast<IFuncAttr>(At);
      return A->clone(C);
    }
    case attr::InitPriority: {
      const auto *A = cast<InitPriorityAttr>(At);
      return A->clone(C);
    }
    case attr::InitSeg: {
      const auto *A = cast<InitSegAttr>(At);
      return A->clone(C);
    }
    case attr::IntelOclBicc: {
      const auto *A = cast<IntelOclBiccAttr>(At);
      return A->clone(C);
    }
    case attr::InternalLinkage: {
      const auto *A = cast<InternalLinkageAttr>(At);
      return A->clone(C);
    }
    case attr::LTOVisibilityPublic: {
      const auto *A = cast<LTOVisibilityPublicAttr>(At);
      return A->clone(C);
    }
    case attr::LayoutVersion: {
      const auto *A = cast<LayoutVersionAttr>(At);
      return A->clone(C);
    }
    case attr::LockReturned: {
      const auto *A = cast<LockReturnedAttr>(At);
      Expr * tempInstArg;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getArg(), TemplateArgs);
        tempInstArg = Result.getAs<Expr>();
      }
      return new (C) LockReturnedAttr(A->getLocation(), C, tempInstArg, A->getSpellingListIndex());
    }
    case attr::LocksExcluded: {
      const auto *A = cast<LocksExcludedAttr>(At);
      auto *tempInstArgs = new (C, 16) Expr *[A->args_size()];
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        Expr * *TI = tempInstArgs;
        Expr * *I = A->args_begin();
        Expr * *E = A->args_end();
        for (; I != E; ++I, ++TI) {
          ExprResult Result = S.SubstExpr(*I, TemplateArgs);
          *TI = Result.getAs<Expr>();
        }
      }
      return new (C) LocksExcludedAttr(A->getLocation(), C, tempInstArgs, A->args_size(), A->getSpellingListIndex());
    }
    case attr::LoopHint: {
      const auto *A = cast<LoopHintAttr>(At);
      return A->clone(C);
    }
    case attr::MAXIAdaptor: {
      const auto *A = cast<MAXIAdaptorAttr>(At);
      Expr * tempInstNumReadOutstanding;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getNumReadOutstanding(), TemplateArgs);
        tempInstNumReadOutstanding = Result.getAs<Expr>();
      }
      Expr * tempInstNumWriteOutstanding;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getNumWriteOutstanding(), TemplateArgs);
        tempInstNumWriteOutstanding = Result.getAs<Expr>();
      }
      Expr * tempInstMaxReadBurstLength;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getMaxReadBurstLength(), TemplateArgs);
        tempInstMaxReadBurstLength = Result.getAs<Expr>();
      }
      Expr * tempInstMaxWriteBurstLength;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getMaxWriteBurstLength(), TemplateArgs);
        tempInstMaxWriteBurstLength = Result.getAs<Expr>();
      }
      Expr * tempInstLatency;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getLatency(), TemplateArgs);
        tempInstLatency = Result.getAs<Expr>();
      }
      return new (C) MAXIAdaptorAttr(A->getLocation(), C, A->getName(), tempInstNumReadOutstanding, tempInstNumWriteOutstanding, tempInstMaxReadBurstLength, tempInstMaxWriteBurstLength, tempInstLatency, A->getSpellingListIndex());
    }
    case attr::MSABI: {
      const auto *A = cast<MSABIAttr>(At);
      return A->clone(C);
    }
    case attr::MSInheritance: {
      const auto *A = cast<MSInheritanceAttr>(At);
      return A->clone(C);
    }
    case attr::MSNoVTable: {
      const auto *A = cast<MSNoVTableAttr>(At);
      return A->clone(C);
    }
    case attr::MSP430Interrupt: {
      const auto *A = cast<MSP430InterruptAttr>(At);
      return A->clone(C);
    }
    case attr::MSStruct: {
      const auto *A = cast<MSStructAttr>(At);
      return A->clone(C);
    }
    case attr::MSVtorDisp: {
      const auto *A = cast<MSVtorDispAttr>(At);
      return A->clone(C);
    }
    case attr::MaxFieldAlignment: {
      const auto *A = cast<MaxFieldAlignmentAttr>(At);
      return A->clone(C);
    }
    case attr::MayAlias: {
      const auto *A = cast<MayAliasAttr>(At);
      return A->clone(C);
    }
    case attr::MicroMips: {
      const auto *A = cast<MicroMipsAttr>(At);
      return A->clone(C);
    }
    case attr::MinSize: {
      const auto *A = cast<MinSizeAttr>(At);
      return A->clone(C);
    }
    case attr::Mips16: {
      const auto *A = cast<Mips16Attr>(At);
      return A->clone(C);
    }
    case attr::MipsInterrupt: {
      const auto *A = cast<MipsInterruptAttr>(At);
      return A->clone(C);
    }
    case attr::MipsLongCall: {
      const auto *A = cast<MipsLongCallAttr>(At);
      return A->clone(C);
    }
    case attr::MipsShortCall: {
      const auto *A = cast<MipsShortCallAttr>(At);
      return A->clone(C);
    }
    case attr::Mode: {
      const auto *A = cast<ModeAttr>(At);
      return A->clone(C);
    }
    case attr::NSConsumed: {
      const auto *A = cast<NSConsumedAttr>(At);
      return A->clone(C);
    }
    case attr::NSConsumesSelf: {
      const auto *A = cast<NSConsumesSelfAttr>(At);
      return A->clone(C);
    }
    case attr::NSReturnsAutoreleased: {
      const auto *A = cast<NSReturnsAutoreleasedAttr>(At);
      return A->clone(C);
    }
    case attr::NSReturnsNotRetained: {
      const auto *A = cast<NSReturnsNotRetainedAttr>(At);
      return A->clone(C);
    }
    case attr::NSReturnsRetained: {
      const auto *A = cast<NSReturnsRetainedAttr>(At);
      return A->clone(C);
    }
    case attr::Naked: {
      const auto *A = cast<NakedAttr>(At);
      return A->clone(C);
    }
    case attr::NoAlias: {
      const auto *A = cast<NoAliasAttr>(At);
      return A->clone(C);
    }
    case attr::NoCommon: {
      const auto *A = cast<NoCommonAttr>(At);
      return A->clone(C);
    }
    case attr::NoCtor: {
      const auto *A = cast<NoCtorAttr>(At);
      return A->clone(C);
    }
    case attr::NoDebug: {
      const auto *A = cast<NoDebugAttr>(At);
      return A->clone(C);
    }
    case attr::NoDuplicate: {
      const auto *A = cast<NoDuplicateAttr>(At);
      return A->clone(C);
    }
    case attr::NoEscape: {
      const auto *A = cast<NoEscapeAttr>(At);
      return A->clone(C);
    }
    case attr::NoInline: {
      const auto *A = cast<NoInlineAttr>(At);
      return A->clone(C);
    }
    case attr::NoInstrumentFunction: {
      const auto *A = cast<NoInstrumentFunctionAttr>(At);
      return A->clone(C);
    }
    case attr::NoMicroMips: {
      const auto *A = cast<NoMicroMipsAttr>(At);
      return A->clone(C);
    }
    case attr::NoMips16: {
      const auto *A = cast<NoMips16Attr>(At);
      return A->clone(C);
    }
    case attr::NoReturn: {
      const auto *A = cast<NoReturnAttr>(At);
      return A->clone(C);
    }
    case attr::NoSanitize: {
      const auto *A = cast<NoSanitizeAttr>(At);
      return A->clone(C);
    }
    case attr::NoSplitStack: {
      const auto *A = cast<NoSplitStackAttr>(At);
      return A->clone(C);
    }
    case attr::NoThreadSafetyAnalysis: {
      const auto *A = cast<NoThreadSafetyAnalysisAttr>(At);
      return A->clone(C);
    }
    case attr::NoThrow: {
      const auto *A = cast<NoThrowAttr>(At);
      return A->clone(C);
    }
    case attr::NonNull: {
      const auto *A = cast<NonNullAttr>(At);
      return A->clone(C);
    }
    case attr::NotTailCalled: {
      const auto *A = cast<NotTailCalledAttr>(At);
      return A->clone(C);
    }
    case attr::OMPCaptureKind: {
      const auto *A = cast<OMPCaptureKindAttr>(At);
      return A->clone(C);
    }
    case attr::OMPCaptureNoInit: {
      const auto *A = cast<OMPCaptureNoInitAttr>(At);
      return A->clone(C);
    }
    case attr::OMPDeclareSimdDecl: {
      const auto *A = cast<OMPDeclareSimdDeclAttr>(At);
      return A->clone(C);
    }
    case attr::OMPDeclareTargetDecl: {
      const auto *A = cast<OMPDeclareTargetDeclAttr>(At);
      return A->clone(C);
    }
    case attr::OMPThreadPrivateDecl: {
      const auto *A = cast<OMPThreadPrivateDeclAttr>(At);
      return A->clone(C);
    }
    case attr::ObjCBoxable: {
      const auto *A = cast<ObjCBoxableAttr>(At);
      return A->clone(C);
    }
    case attr::ObjCBridge: {
      const auto *A = cast<ObjCBridgeAttr>(At);
      return A->clone(C);
    }
    case attr::ObjCBridgeMutable: {
      const auto *A = cast<ObjCBridgeMutableAttr>(At);
      return A->clone(C);
    }
    case attr::ObjCBridgeRelated: {
      const auto *A = cast<ObjCBridgeRelatedAttr>(At);
      return A->clone(C);
    }
    case attr::ObjCDesignatedInitializer: {
      const auto *A = cast<ObjCDesignatedInitializerAttr>(At);
      return A->clone(C);
    }
    case attr::ObjCException: {
      const auto *A = cast<ObjCExceptionAttr>(At);
      return A->clone(C);
    }
    case attr::ObjCExplicitProtocolImpl: {
      const auto *A = cast<ObjCExplicitProtocolImplAttr>(At);
      return A->clone(C);
    }
    case attr::ObjCIndependentClass: {
      const auto *A = cast<ObjCIndependentClassAttr>(At);
      return A->clone(C);
    }
    case attr::ObjCMethodFamily: {
      const auto *A = cast<ObjCMethodFamilyAttr>(At);
      return A->clone(C);
    }
    case attr::ObjCNSObject: {
      const auto *A = cast<ObjCNSObjectAttr>(At);
      return A->clone(C);
    }
    case attr::ObjCPreciseLifetime: {
      const auto *A = cast<ObjCPreciseLifetimeAttr>(At);
      return A->clone(C);
    }
    case attr::ObjCRequiresPropertyDefs: {
      const auto *A = cast<ObjCRequiresPropertyDefsAttr>(At);
      return A->clone(C);
    }
    case attr::ObjCRequiresSuper: {
      const auto *A = cast<ObjCRequiresSuperAttr>(At);
      return A->clone(C);
    }
    case attr::ObjCReturnsInnerPointer: {
      const auto *A = cast<ObjCReturnsInnerPointerAttr>(At);
      return A->clone(C);
    }
    case attr::ObjCRootClass: {
      const auto *A = cast<ObjCRootClassAttr>(At);
      return A->clone(C);
    }
    case attr::ObjCRuntimeName: {
      const auto *A = cast<ObjCRuntimeNameAttr>(At);
      return A->clone(C);
    }
    case attr::ObjCRuntimeVisible: {
      const auto *A = cast<ObjCRuntimeVisibleAttr>(At);
      return A->clone(C);
    }
    case attr::ObjCSubclassingRestricted: {
      const auto *A = cast<ObjCSubclassingRestrictedAttr>(At);
      return A->clone(C);
    }
    case attr::OpenCLAccess: {
      const auto *A = cast<OpenCLAccessAttr>(At);
      return A->clone(C);
    }
    case attr::OpenCLIntelReqdSubGroupSize: {
      const auto *A = cast<OpenCLIntelReqdSubGroupSizeAttr>(At);
      return A->clone(C);
    }
    case attr::OpenCLKernel: {
      const auto *A = cast<OpenCLKernelAttr>(At);
      return A->clone(C);
    }
    case attr::OpenCLUnrollHint: {
      const auto *A = cast<OpenCLUnrollHintAttr>(At);
      return A->clone(C);
    }
    case attr::OptimizeNone: {
      const auto *A = cast<OptimizeNoneAttr>(At);
      return A->clone(C);
    }
    case attr::Overloadable: {
      const auto *A = cast<OverloadableAttr>(At);
      return A->clone(C);
    }
    case attr::Override: {
      const auto *A = cast<OverrideAttr>(At);
      return A->clone(C);
    }
    case attr::Ownership: {
      const auto *A = cast<OwnershipAttr>(At);
      return A->clone(C);
    }
    case attr::Packed: {
      const auto *A = cast<PackedAttr>(At);
      return A->clone(C);
    }
    case attr::ParamTypestate: {
      const auto *A = cast<ParamTypestateAttr>(At);
      return A->clone(C);
    }
    case attr::Pascal: {
      const auto *A = cast<PascalAttr>(At);
      return A->clone(C);
    }
    case attr::PassObjectSize: {
      const auto *A = cast<PassObjectSizeAttr>(At);
      return A->clone(C);
    }
    case attr::Pcs: {
      const auto *A = cast<PcsAttr>(At);
      return A->clone(C);
    }
    case attr::PragmaClangBSSSection: {
      const auto *A = cast<PragmaClangBSSSectionAttr>(At);
      return A->clone(C);
    }
    case attr::PragmaClangDataSection: {
      const auto *A = cast<PragmaClangDataSectionAttr>(At);
      return A->clone(C);
    }
    case attr::PragmaClangRodataSection: {
      const auto *A = cast<PragmaClangRodataSectionAttr>(At);
      return A->clone(C);
    }
    case attr::PragmaClangTextSection: {
      const auto *A = cast<PragmaClangTextSectionAttr>(At);
      return A->clone(C);
    }
    case attr::PreserveAll: {
      const auto *A = cast<PreserveAllAttr>(At);
      return A->clone(C);
    }
    case attr::PreserveMost: {
      const auto *A = cast<PreserveMostAttr>(At);
      return A->clone(C);
    }
    case attr::PtGuardedBy: {
      const auto *A = cast<PtGuardedByAttr>(At);
      Expr * tempInstArg;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getArg(), TemplateArgs);
        tempInstArg = Result.getAs<Expr>();
      }
      return new (C) PtGuardedByAttr(A->getLocation(), C, tempInstArg, A->getSpellingListIndex());
    }
    case attr::PtGuardedVar: {
      const auto *A = cast<PtGuardedVarAttr>(At);
      return A->clone(C);
    }
    case attr::Pure: {
      const auto *A = cast<PureAttr>(At);
      return A->clone(C);
    }
    case attr::RegCall: {
      const auto *A = cast<RegCallAttr>(At);
      return A->clone(C);
    }
    case attr::ReleaseCapability: {
      const auto *A = cast<ReleaseCapabilityAttr>(At);
      auto *tempInstArgs = new (C, 16) Expr *[A->args_size()];
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        Expr * *TI = tempInstArgs;
        Expr * *I = A->args_begin();
        Expr * *E = A->args_end();
        for (; I != E; ++I, ++TI) {
          ExprResult Result = S.SubstExpr(*I, TemplateArgs);
          *TI = Result.getAs<Expr>();
        }
      }
      return new (C) ReleaseCapabilityAttr(A->getLocation(), C, tempInstArgs, A->args_size(), A->getSpellingListIndex());
    }
    case attr::RenderScriptKernel: {
      const auto *A = cast<RenderScriptKernelAttr>(At);
      return A->clone(C);
    }
    case attr::ReqdWorkGroupSize: {
      const auto *A = cast<ReqdWorkGroupSizeAttr>(At);
      return A->clone(C);
    }
    case attr::RequireConstantInit: {
      const auto *A = cast<RequireConstantInitAttr>(At);
      return A->clone(C);
    }
    case attr::RequiresCapability: {
      const auto *A = cast<RequiresCapabilityAttr>(At);
      auto *tempInstArgs = new (C, 16) Expr *[A->args_size()];
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        Expr * *TI = tempInstArgs;
        Expr * *I = A->args_begin();
        Expr * *E = A->args_end();
        for (; I != E; ++I, ++TI) {
          ExprResult Result = S.SubstExpr(*I, TemplateArgs);
          *TI = Result.getAs<Expr>();
        }
      }
      return new (C) RequiresCapabilityAttr(A->getLocation(), C, tempInstArgs, A->args_size(), A->getSpellingListIndex());
    }
    case attr::Restrict: {
      const auto *A = cast<RestrictAttr>(At);
      return A->clone(C);
    }
    case attr::ReturnTypestate: {
      const auto *A = cast<ReturnTypestateAttr>(At);
      return A->clone(C);
    }
    case attr::ReturnsNonNull: {
      const auto *A = cast<ReturnsNonNullAttr>(At);
      return A->clone(C);
    }
    case attr::ReturnsTwice: {
      const auto *A = cast<ReturnsTwiceAttr>(At);
      return A->clone(C);
    }
    case attr::SAXIAdaptor: {
      const auto *A = cast<SAXIAdaptorAttr>(At);
      return new (C) SAXIAdaptorAttr(A->getLocation(), C, A->getName(), A->getClock(), A->getSpellingListIndex());
    }
    case attr::SDxKernel: {
      const auto *A = cast<SDxKernelAttr>(At);
      return A->clone(C);
    }
    case attr::ScopedLockable: {
      const auto *A = cast<ScopedLockableAttr>(At);
      return A->clone(C);
    }
    case attr::Section: {
      const auto *A = cast<SectionAttr>(At);
      return A->clone(C);
    }
    case attr::SelectAny: {
      const auto *A = cast<SelectAnyAttr>(At);
      return A->clone(C);
    }
    case attr::Sentinel: {
      const auto *A = cast<SentinelAttr>(At);
      return A->clone(C);
    }
    case attr::SetTypestate: {
      const auto *A = cast<SetTypestateAttr>(At);
      return A->clone(C);
    }
    case attr::SharedTrylockFunction: {
      const auto *A = cast<SharedTrylockFunctionAttr>(At);
      Expr * tempInstSuccessValue;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getSuccessValue(), TemplateArgs);
        tempInstSuccessValue = Result.getAs<Expr>();
      }
      auto *tempInstArgs = new (C, 16) Expr *[A->args_size()];
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        Expr * *TI = tempInstArgs;
        Expr * *I = A->args_begin();
        Expr * *E = A->args_end();
        for (; I != E; ++I, ++TI) {
          ExprResult Result = S.SubstExpr(*I, TemplateArgs);
          *TI = Result.getAs<Expr>();
        }
      }
      return new (C) SharedTrylockFunctionAttr(A->getLocation(), C, tempInstSuccessValue, tempInstArgs, A->args_size(), A->getSpellingListIndex());
    }
    case attr::StdCall: {
      const auto *A = cast<StdCallAttr>(At);
      return A->clone(C);
    }
    case attr::Suppress: {
      const auto *A = cast<SuppressAttr>(At);
      return A->clone(C);
    }
    case attr::SwiftCall: {
      const auto *A = cast<SwiftCallAttr>(At);
      return A->clone(C);
    }
    case attr::SwiftContext: {
      const auto *A = cast<SwiftContextAttr>(At);
      return A->clone(C);
    }
    case attr::SwiftErrorResult: {
      const auto *A = cast<SwiftErrorResultAttr>(At);
      return A->clone(C);
    }
    case attr::SwiftIndirectResult: {
      const auto *A = cast<SwiftIndirectResultAttr>(At);
      return A->clone(C);
    }
    case attr::SysVABI: {
      const auto *A = cast<SysVABIAttr>(At);
      return A->clone(C);
    }
    case attr::TLSModel: {
      const auto *A = cast<TLSModelAttr>(At);
      return A->clone(C);
    }
    case attr::Target: {
      const auto *A = cast<TargetAttr>(At);
      return A->clone(C);
    }
    case attr::TestTypestate: {
      const auto *A = cast<TestTypestateAttr>(At);
      return A->clone(C);
    }
    case attr::ThisCall: {
      const auto *A = cast<ThisCallAttr>(At);
      return A->clone(C);
    }
    case attr::Thread: {
      const auto *A = cast<ThreadAttr>(At);
      return A->clone(C);
    }
    case attr::TransparentUnion: {
      const auto *A = cast<TransparentUnionAttr>(At);
      return A->clone(C);
    }
    case attr::TryAcquireCapability: {
      const auto *A = cast<TryAcquireCapabilityAttr>(At);
      Expr * tempInstSuccessValue;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getSuccessValue(), TemplateArgs);
        tempInstSuccessValue = Result.getAs<Expr>();
      }
      auto *tempInstArgs = new (C, 16) Expr *[A->args_size()];
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        Expr * *TI = tempInstArgs;
        Expr * *I = A->args_begin();
        Expr * *E = A->args_end();
        for (; I != E; ++I, ++TI) {
          ExprResult Result = S.SubstExpr(*I, TemplateArgs);
          *TI = Result.getAs<Expr>();
        }
      }
      return new (C) TryAcquireCapabilityAttr(A->getLocation(), C, tempInstSuccessValue, tempInstArgs, A->args_size(), A->getSpellingListIndex());
    }
    case attr::TypeTagForDatatype: {
      const auto *A = cast<TypeTagForDatatypeAttr>(At);
      return A->clone(C);
    }
    case attr::TypeVisibility: {
      return nullptr;
    }
    case attr::Unavailable: {
      const auto *A = cast<UnavailableAttr>(At);
      return A->clone(C);
    }
    case attr::Unpacked: {
      const auto *A = cast<UnpackedAttr>(At);
      return A->clone(C);
    }
    case attr::Unused: {
      const auto *A = cast<UnusedAttr>(At);
      return A->clone(C);
    }
    case attr::Used: {
      const auto *A = cast<UsedAttr>(At);
      return A->clone(C);
    }
    case attr::Uuid: {
      const auto *A = cast<UuidAttr>(At);
      return A->clone(C);
    }
    case attr::VecReturn: {
      const auto *A = cast<VecReturnAttr>(At);
      return A->clone(C);
    }
    case attr::VecTypeHint: {
      const auto *A = cast<VecTypeHintAttr>(At);
      return A->clone(C);
    }
    case attr::VectorCall: {
      const auto *A = cast<VectorCallAttr>(At);
      return A->clone(C);
    }
    case attr::Visibility: {
      return nullptr;
    }
    case attr::WarnUnused: {
      const auto *A = cast<WarnUnusedAttr>(At);
      return A->clone(C);
    }
    case attr::WarnUnusedResult: {
      const auto *A = cast<WarnUnusedResultAttr>(At);
      return A->clone(C);
    }
    case attr::Weak: {
      const auto *A = cast<WeakAttr>(At);
      return A->clone(C);
    }
    case attr::WeakImport: {
      const auto *A = cast<WeakImportAttr>(At);
      return A->clone(C);
    }
    case attr::WeakRef: {
      const auto *A = cast<WeakRefAttr>(At);
      return A->clone(C);
    }
    case attr::WorkGroupSizeHint: {
      const auto *A = cast<WorkGroupSizeHintAttr>(At);
      return A->clone(C);
    }
    case attr::X86ForceAlignArgPointer: {
      const auto *A = cast<X86ForceAlignArgPointerAttr>(At);
      return A->clone(C);
    }
    case attr::XCLArrayGeometry: {
      const auto *A = cast<XCLArrayGeometryAttr>(At);
      auto *tempInstDims = new (C, 16) Expr *[A->dims_size()];
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        Expr * *TI = tempInstDims;
        Expr * *I = A->dims_begin();
        Expr * *E = A->dims_end();
        for (; I != E; ++I, ++TI) {
          ExprResult Result = S.SubstExpr(*I, TemplateArgs);
          *TI = Result.getAs<Expr>();
        }
      }
      return new (C) XCLArrayGeometryAttr(A->getLocation(), C, tempInstDims, A->dims_size(), A->getSpellingListIndex());
    }
    case attr::XCLArrayView: {
      const auto *A = cast<XCLArrayViewAttr>(At);
      Expr * tempInstArray;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getArray(), TemplateArgs);
        tempInstArray = Result.getAs<Expr>();
      }
      auto *tempInstShape = new (C, 16) Expr *[A->shape_size()];
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        Expr * *TI = tempInstShape;
        Expr * *I = A->shape_begin();
        Expr * *E = A->shape_end();
        for (; I != E; ++I, ++TI) {
          ExprResult Result = S.SubstExpr(*I, TemplateArgs);
          *TI = Result.getAs<Expr>();
        }
      }
      return new (C) XCLArrayViewAttr(A->getLocation(), C, A->getAccessMode(), tempInstArray, tempInstShape, A->shape_size(), A->getSpellingListIndex());
    }
    case attr::XCLArrayXForm: {
      const auto *A = cast<XCLArrayXFormAttr>(At);
      Expr * tempInstFactor;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getFactor(), TemplateArgs);
        tempInstFactor = Result.getAs<Expr>();
      }
      Expr * tempInstDim;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getDim(), TemplateArgs);
        tempInstDim = Result.getAs<Expr>();
      }
      return new (C) XCLArrayXFormAttr(A->getLocation(), C, A->getType(), tempInstFactor, tempInstDim, A->getSpellingListIndex());
    }
    case attr::XCLDataFlow: {
      const auto *A = cast<XCLDataFlowAttr>(At);
      return A->clone(C);
    }
    case attr::XCLDependence: {
      const auto *A = cast<XCLDependenceAttr>(At);
      Expr * tempInstVariable;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getVariable(), TemplateArgs);
        tempInstVariable = Result.getAs<Expr>();
      }
      return new (C) XCLDependenceAttr(A->getLocation(), C, tempInstVariable, A->getXClass(), A->getType(), A->getDirection(), A->getDistance(), A->getCompel(), A->getSpellingListIndex());
    }
    case attr::XCLFlattenLoop: {
      const auto *A = cast<XCLFlattenLoopAttr>(At);
      return A->clone(C);
    }
    case attr::XCLInline: {
      const auto *A = cast<XCLInlineAttr>(At);
      return A->clone(C);
    }
    case attr::XCLLatency: {
      const auto *A = cast<XCLLatencyAttr>(At);
      Expr * tempInstMin;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getMin(), TemplateArgs);
        tempInstMin = Result.getAs<Expr>();
      }
      Expr * tempInstMax;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getMax(), TemplateArgs);
        tempInstMax = Result.getAs<Expr>();
      }
      return new (C) XCLLatencyAttr(A->getLocation(), C, tempInstMin, tempInstMax, A->getSpellingListIndex());
    }
    case attr::XCLLoopTripCount: {
      const auto *A = cast<XCLLoopTripCountAttr>(At);
      return A->clone(C);
    }
    case attr::XCLMaxWorkGroupSize: {
      const auto *A = cast<XCLMaxWorkGroupSizeAttr>(At);
      return A->clone(C);
    }
    case attr::XCLOutline: {
      const auto *A = cast<XCLOutlineAttr>(At);
      return A->clone(C);
    }
    case attr::XCLPipelineLoop: {
      const auto *A = cast<XCLPipelineLoopAttr>(At);
      Expr * tempInstII;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getII(), TemplateArgs);
        tempInstII = Result.getAs<Expr>();
      }
      return new (C) XCLPipelineLoopAttr(A->getLocation(), C, tempInstII, A->getSpellingListIndex());
    }
    case attr::XCLPipelineWorkitems: {
      const auto *A = cast<XCLPipelineWorkitemsAttr>(At);
      Expr * tempInstII;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getII(), TemplateArgs);
        tempInstII = Result.getAs<Expr>();
      }
      return new (C) XCLPipelineWorkitemsAttr(A->getLocation(), C, tempInstII, A->getSpellingListIndex());
    }
    case attr::XCLRegionName: {
      const auto *A = cast<XCLRegionNameAttr>(At);
      return A->clone(C);
    }
    case attr::XCLReqdPipeDepth: {
      const auto *A = cast<XCLReqdPipeDepthAttr>(At);
      Expr * tempInstDepth;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getDepth(), TemplateArgs);
        tempInstDepth = Result.getAs<Expr>();
      }
      return new (C) XCLReqdPipeDepthAttr(A->getLocation(), C, tempInstDepth, A->getOff(), A->getSpellingListIndex());
    }
    case attr::XCLSingleWorkitem: {
      const auto *A = cast<XCLSingleWorkitemAttr>(At);
      return A->clone(C);
    }
    case attr::XCLUnrollWorkitems: {
      const auto *A = cast<XCLUnrollWorkitemsAttr>(At);
      return A->clone(C);
    }
    case attr::XCLVisibility: {
      const auto *A = cast<XCLVisibilityAttr>(At);
      return A->clone(C);
    }
    case attr::XCLZeroGlobalWorkOffset: {
      const auto *A = cast<XCLZeroGlobalWorkOffsetAttr>(At);
      return A->clone(C);
    }
    case attr::XRayInstrument: {
      const auto *A = cast<XRayInstrumentAttr>(At);
      return A->clone(C);
    }
    case attr::XRayLogArgs: {
      const auto *A = cast<XRayLogArgsAttr>(At);
      return A->clone(C);
    }
    case attr::XlxAggregate: {
      const auto *A = cast<XlxAggregateAttr>(At);
      Expr * tempInstVariable;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getVariable(), TemplateArgs);
        tempInstVariable = Result.getAs<Expr>();
      }
      return new (C) XlxAggregateAttr(A->getLocation(), C, tempInstVariable, A->getSpellingListIndex());
    }
    case attr::XlxArrayGeometry: {
      const auto *A = cast<XlxArrayGeometryAttr>(At);
      auto *tempInstDims = new (C, 16) Expr *[A->dims_size()];
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        Expr * *TI = tempInstDims;
        Expr * *I = A->dims_begin();
        Expr * *E = A->dims_end();
        for (; I != E; ++I, ++TI) {
          ExprResult Result = S.SubstExpr(*I, TemplateArgs);
          *TI = Result.getAs<Expr>();
        }
      }
      return new (C) XlxArrayGeometryAttr(A->getLocation(), C, tempInstDims, A->dims_size(), A->getSpellingListIndex());
    }
    case attr::XlxArrayView: {
      const auto *A = cast<XlxArrayViewAttr>(At);
      Expr * tempInstArray;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getArray(), TemplateArgs);
        tempInstArray = Result.getAs<Expr>();
      }
      auto *tempInstShape = new (C, 16) Expr *[A->shape_size()];
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        Expr * *TI = tempInstShape;
        Expr * *I = A->shape_begin();
        Expr * *E = A->shape_end();
        for (; I != E; ++I, ++TI) {
          ExprResult Result = S.SubstExpr(*I, TemplateArgs);
          *TI = Result.getAs<Expr>();
        }
      }
      return new (C) XlxArrayViewAttr(A->getLocation(), C, A->getAccessMode(), tempInstArray, tempInstShape, A->shape_size(), A->getSpellingListIndex());
    }
    case attr::XlxArrayXForm: {
      const auto *A = cast<XlxArrayXFormAttr>(At);
      Expr * tempInstVariable;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getVariable(), TemplateArgs);
        tempInstVariable = Result.getAs<Expr>();
      }
      Expr * tempInstFactor;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getFactor(), TemplateArgs);
        tempInstFactor = Result.getAs<Expr>();
      }
      Expr * tempInstDim;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getDim(), TemplateArgs);
        tempInstDim = Result.getAs<Expr>();
      }
      return new (C) XlxArrayXFormAttr(A->getLocation(), C, tempInstVariable, A->getType(), tempInstFactor, tempInstDim, A->getSpellingListIndex());
    }
    case attr::XlxBindOp: {
      const auto *A = cast<XlxBindOpAttr>(At);
      Expr * tempInstVariable;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getVariable(), TemplateArgs);
        tempInstVariable = Result.getAs<Expr>();
      }
      Expr * tempInstOp;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getOp(), TemplateArgs);
        tempInstOp = Result.getAs<Expr>();
      }
      Expr * tempInstImpl;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getImpl(), TemplateArgs);
        tempInstImpl = Result.getAs<Expr>();
      }
      Expr * tempInstLatency;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getLatency(), TemplateArgs);
        tempInstLatency = Result.getAs<Expr>();
      }
      return new (C) XlxBindOpAttr(A->getLocation(), C, tempInstVariable, tempInstOp, tempInstImpl, tempInstLatency, A->getSpellingListIndex());
    }
    case attr::XlxBindOpExpr: {
      const auto *A = cast<XlxBindOpExprAttr>(At);
      Expr * tempInstVariable;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getVariable(), TemplateArgs);
        tempInstVariable = Result.getAs<Expr>();
      }
      Expr * tempInstOp;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getOp(), TemplateArgs);
        tempInstOp = Result.getAs<Expr>();
      }
      Expr * tempInstImpl;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getImpl(), TemplateArgs);
        tempInstImpl = Result.getAs<Expr>();
      }
      Expr * tempInstLatency;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getLatency(), TemplateArgs);
        tempInstLatency = Result.getAs<Expr>();
      }
      return new (C) XlxBindOpExprAttr(A->getLocation(), C, tempInstVariable, tempInstOp, tempInstImpl, tempInstLatency, A->getSpellingListIndex());
    }
    case attr::XlxBindStorage: {
      const auto *A = cast<XlxBindStorageAttr>(At);
      Expr * tempInstVariable;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getVariable(), TemplateArgs);
        tempInstVariable = Result.getAs<Expr>();
      }
      Expr * tempInstType;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getType(), TemplateArgs);
        tempInstType = Result.getAs<Expr>();
      }
      Expr * tempInstImpl;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getImpl(), TemplateArgs);
        tempInstImpl = Result.getAs<Expr>();
      }
      Expr * tempInstLatency;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getLatency(), TemplateArgs);
        tempInstLatency = Result.getAs<Expr>();
      }
      return new (C) XlxBindStorageAttr(A->getLocation(), C, tempInstVariable, tempInstType, tempInstImpl, tempInstLatency, A->getSpellingListIndex());
    }
    case attr::XlxDependence: {
      const auto *A = cast<XlxDependenceAttr>(At);
      Expr * tempInstVariable;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getVariable(), TemplateArgs);
        tempInstVariable = Result.getAs<Expr>();
      }
      return new (C) XlxDependenceAttr(A->getLocation(), C, tempInstVariable, A->getXClass(), A->getType(), A->getDirection(), A->getDistance(), A->getCompel(), A->getSpellingListIndex());
    }
    case attr::XlxDisaggr: {
      const auto *A = cast<XlxDisaggrAttr>(At);
      Expr * tempInstVariable;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getVariable(), TemplateArgs);
        tempInstVariable = Result.getAs<Expr>();
      }
      return new (C) XlxDisaggrAttr(A->getLocation(), C, tempInstVariable, A->getSpellingListIndex());
    }
    case attr::XlxExprBalance: {
      const auto *A = cast<XlxExprBalanceAttr>(At);
      return A->clone(C);
    }
    case attr::XlxFuncInstantiate: {
      const auto *A = cast<XlxFuncInstantiateAttr>(At);
      return A->clone(C);
    }
    case attr::XlxLoopTripCount: {
      const auto *A = cast<XlxLoopTripCountAttr>(At);
      Expr * tempInstMin;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getMin(), TemplateArgs);
        tempInstMin = Result.getAs<Expr>();
      }
      Expr * tempInstMax;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getMax(), TemplateArgs);
        tempInstMax = Result.getAs<Expr>();
      }
      Expr * tempInstAvg;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getAvg(), TemplateArgs);
        tempInstAvg = Result.getAs<Expr>();
      }
      return new (C) XlxLoopTripCountAttr(A->getLocation(), C, tempInstMin, tempInstMax, tempInstAvg, A->getSpellingListIndex());
    }
    case attr::XlxMergeLoop: {
      const auto *A = cast<XlxMergeLoopAttr>(At);
      return A->clone(C);
    }
    case attr::XlxOccurrence: {
      const auto *A = cast<XlxOccurrenceAttr>(At);
      Expr * tempInstCycle;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getCycle(), TemplateArgs);
        tempInstCycle = Result.getAs<Expr>();
      }
      return new (C) XlxOccurrenceAttr(A->getLocation(), C, tempInstCycle, A->getSpellingListIndex());
    }
    case attr::XlxPipeline: {
      const auto *A = cast<XlxPipelineAttr>(At);
      Expr * tempInstII;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getII(), TemplateArgs);
        tempInstII = Result.getAs<Expr>();
      }
      return new (C) XlxPipelineAttr(A->getLocation(), C, tempInstII, A->getEnableFlush(), A->getRewind(), A->getSpellingListIndex());
    }
    case attr::XlxProtocol: {
      const auto *A = cast<XlxProtocolAttr>(At);
      return A->clone(C);
    }
    case attr::XlxReqdPipeDepth: {
      const auto *A = cast<XlxReqdPipeDepthAttr>(At);
      Expr * tempInstVariable;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getVariable(), TemplateArgs);
        tempInstVariable = Result.getAs<Expr>();
      }
      Expr * tempInstDepth;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getDepth(), TemplateArgs);
        tempInstDepth = Result.getAs<Expr>();
      }
      return new (C) XlxReqdPipeDepthAttr(A->getLocation(), C, tempInstVariable, tempInstDepth, A->getOff(), A->getSpellingListIndex());
    }
    case attr::XlxResourceIPCore: {
      const auto *A = cast<XlxResourceIPCoreAttr>(At);
      return A->clone(C);
    }
    case attr::XlxRewinding: {
      const auto *A = cast<XlxRewindingAttr>(At);
      return A->clone(C);
    }
    case attr::XlxShared: {
      const auto *A = cast<XlxSharedAttr>(At);
      Expr * tempInstVariable;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getVariable(), TemplateArgs);
        tempInstVariable = Result.getAs<Expr>();
      }
      return new (C) XlxSharedAttr(A->getLocation(), C, tempInstVariable, A->getSpellingListIndex());
    }
    case attr::XlxStable: {
      const auto *A = cast<XlxStableAttr>(At);
      Expr * tempInstVariable;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getVariable(), TemplateArgs);
        tempInstVariable = Result.getAs<Expr>();
      }
      return new (C) XlxStableAttr(A->getLocation(), C, tempInstVariable, A->getSpellingListIndex());
    }
    case attr::XlxStableContent: {
      const auto *A = cast<XlxStableContentAttr>(At);
      return A->clone(C);
    }
    case attr::XlxUnrollHint: {
      const auto *A = cast<XlxUnrollHintAttr>(At);
      Expr * tempInstFactor;
      {
        EnterExpressionEvaluationContext Unevaluated(S, Sema::ExpressionEvaluationContext::Unevaluated);
        ExprResult Result = S.SubstExpr(A->getFactor(), TemplateArgs);
        tempInstFactor = Result.getAs<Expr>();
      }
      return new (C) XlxUnrollHintAttr(A->getLocation(), C, tempInstFactor, A->getSkipExitCheck(), A->getIsDefaultFactor(), A->getSpellingListIndex());
    }
    case attr::XlxUnrollRegionHint: {
      const auto *A = cast<XlxUnrollRegionHintAttr>(At);
      return A->clone(C);
    }
    case attr::XlxVarReset: {
      const auto *A = cast<XlxVarResetAttr>(At);
      return A->clone(C);
    }
  } // end switch
  llvm_unreachable("Unknown attribute!");
  return nullptr;
}

Attr *instantiateTemplateAttributeForDecl(const Attr *At,
 ASTContext &C, Sema &S,
        const MultiLevelTemplateArgumentList &TemplateArgs) {
  switch (At->getKind()) {
    case attr::AMDGPUFlatWorkGroupSize: {
      return nullptr;
    }
    case attr::AMDGPUNumSGPR: {
      return nullptr;
    }
    case attr::AMDGPUNumVGPR: {
      return nullptr;
    }
    case attr::AMDGPUWavesPerEU: {
      return nullptr;
    }
    case attr::ARMInterrupt: {
      return nullptr;
    }
    case attr::AVRInterrupt: {
      return nullptr;
    }
    case attr::AVRSignal: {
      return nullptr;
    }
    case attr::AXISAdaptor: {
      return nullptr;
    }
    case attr::AbiTag: {
      const auto *A = cast<AbiTagAttr>(At);
      return A->clone(C);
    }
    case attr::AcquireCapability: {
      return nullptr;
    }
    case attr::AcquiredAfter: {
      return nullptr;
    }
    case attr::AcquiredBefore: {
      return nullptr;
    }
    case attr::Alias: {
      return nullptr;
    }
    case attr::AlignMac68k: {
      return nullptr;
    }
    case attr::AlignValue: {
      return nullptr;
    }
    case attr::Aligned: {
      return nullptr;
    }
    case attr::AllocAlign: {
      return nullptr;
    }
    case attr::AllocSize: {
      return nullptr;
    }
    case attr::AlwaysInline: {
      return nullptr;
    }
    case attr::AnalyzerNoReturn: {
      return nullptr;
    }
    case attr::Annotate: {
      return nullptr;
    }
    case attr::AnyX86Interrupt: {
      return nullptr;
    }
    case attr::AnyX86NoCallerSavedRegisters: {
      return nullptr;
    }
    case attr::ArcWeakrefUnavailable: {
      return nullptr;
    }
    case attr::ArgumentWithTypeTag: {
      return nullptr;
    }
    case attr::AsmLabel: {
      return nullptr;
    }
    case attr::AssertCapability: {
      return nullptr;
    }
    case attr::AssertExclusiveLock: {
      return nullptr;
    }
    case attr::AssertSharedLock: {
      return nullptr;
    }
    case attr::AssumeAligned: {
      return nullptr;
    }
    case attr::Availability: {
      return nullptr;
    }
    case attr::BRAMAdaptor: {
      return nullptr;
    }
    case attr::Blocks: {
      return nullptr;
    }
    case attr::C11NoReturn: {
      return nullptr;
    }
    case attr::CDecl: {
      return nullptr;
    }
    case attr::CFAuditedTransfer: {
      return nullptr;
    }
    case attr::CFConsumed: {
      return nullptr;
    }
    case attr::CFReturnsNotRetained: {
      return nullptr;
    }
    case attr::CFReturnsRetained: {
      return nullptr;
    }
    case attr::CFUnknownTransfer: {
      return nullptr;
    }
    case attr::CUDAConstant: {
      return nullptr;
    }
    case attr::CUDADevice: {
      return nullptr;
    }
    case attr::CUDAGlobal: {
      return nullptr;
    }
    case attr::CUDAHost: {
      return nullptr;
    }
    case attr::CUDAInvalidTarget: {
      return nullptr;
    }
    case attr::CUDALaunchBounds: {
      return nullptr;
    }
    case attr::CUDAShared: {
      return nullptr;
    }
    case attr::CXX11NoReturn: {
      return nullptr;
    }
    case attr::CallableWhen: {
      return nullptr;
    }
    case attr::Capability: {
      return nullptr;
    }
    case attr::CapturedRecord: {
      return nullptr;
    }
    case attr::CarriesDependency: {
      return nullptr;
    }
    case attr::Cleanup: {
      return nullptr;
    }
    case attr::CodeGenType: {
      return nullptr;
    }
    case attr::Cold: {
      return nullptr;
    }
    case attr::Common: {
      return nullptr;
    }
    case attr::Const: {
      return nullptr;
    }
    case attr::Constructor: {
      return nullptr;
    }
    case attr::Consumable: {
      return nullptr;
    }
    case attr::ConsumableAutoCast: {
      return nullptr;
    }
    case attr::ConsumableSetOnRead: {
      return nullptr;
    }
    case attr::Convergent: {
      return nullptr;
    }
    case attr::DLLExport: {
      return nullptr;
    }
    case attr::DLLImport: {
      return nullptr;
    }
    case attr::Deprecated: {
      const auto *A = cast<DeprecatedAttr>(At);
      return A->clone(C);
    }
    case attr::Destructor: {
      return nullptr;
    }
    case attr::DiagnoseIf: {
      return nullptr;
    }
    case attr::DisableTailCalls: {
      return nullptr;
    }
    case attr::EmptyBases: {
      return nullptr;
    }
    case attr::EnableIf: {
      return nullptr;
    }
    case attr::EnumExtensibility: {
      return nullptr;
    }
    case attr::ExclusiveTrylockFunction: {
      return nullptr;
    }
    case attr::ExternalSourceSymbol: {
      return nullptr;
    }
    case attr::FPGAAddressInterface: {
      return nullptr;
    }
    case attr::FPGADataFootPrintHint: {
      return nullptr;
    }
    case attr::FPGAFunctionCtrlInterface: {
      return nullptr;
    }
    case attr::FPGARegister: {
      return nullptr;
    }
    case attr::FPGAResourceHint: {
      return nullptr;
    }
    case attr::FPGAResourceLimitHint: {
      return nullptr;
    }
    case attr::FPGAScalarInterface: {
      return nullptr;
    }
    case attr::FPGAScalarInterfaceWrapper: {
      return nullptr;
    }
    case attr::FPGASignalName: {
      return nullptr;
    }
    case attr::FallThrough: {
      return nullptr;
    }
    case attr::FastCall: {
      return nullptr;
    }
    case attr::Final: {
      return nullptr;
    }
    case attr::FlagEnum: {
      return nullptr;
    }
    case attr::Flatten: {
      return nullptr;
    }
    case attr::Format: {
      return nullptr;
    }
    case attr::FormatArg: {
      return nullptr;
    }
    case attr::GNUInline: {
      return nullptr;
    }
    case attr::GuardedBy: {
      return nullptr;
    }
    case attr::GuardedVar: {
      return nullptr;
    }
    case attr::Hot: {
      return nullptr;
    }
    case attr::IBAction: {
      return nullptr;
    }
    case attr::IBOutlet: {
      return nullptr;
    }
    case attr::IBOutletCollection: {
      return nullptr;
    }
    case attr::IFunc: {
      return nullptr;
    }
    case attr::InitPriority: {
      return nullptr;
    }
    case attr::InitSeg: {
      return nullptr;
    }
    case attr::IntelOclBicc: {
      return nullptr;
    }
    case attr::InternalLinkage: {
      return nullptr;
    }
    case attr::LTOVisibilityPublic: {
      return nullptr;
    }
    case attr::LayoutVersion: {
      return nullptr;
    }
    case attr::LockReturned: {
      return nullptr;
    }
    case attr::LocksExcluded: {
      return nullptr;
    }
    case attr::LoopHint: {
      return nullptr;
    }
    case attr::MAXIAdaptor: {
      return nullptr;
    }
    case attr::MSABI: {
      return nullptr;
    }
    case attr::MSInheritance: {
      return nullptr;
    }
    case attr::MSNoVTable: {
      return nullptr;
    }
    case attr::MSP430Interrupt: {
      return nullptr;
    }
    case attr::MSStruct: {
      return nullptr;
    }
    case attr::MSVtorDisp: {
      return nullptr;
    }
    case attr::MaxFieldAlignment: {
      return nullptr;
    }
    case attr::MayAlias: {
      return nullptr;
    }
    case attr::MicroMips: {
      return nullptr;
    }
    case attr::MinSize: {
      return nullptr;
    }
    case attr::Mips16: {
      return nullptr;
    }
    case attr::MipsInterrupt: {
      return nullptr;
    }
    case attr::MipsLongCall: {
      return nullptr;
    }
    case attr::MipsShortCall: {
      return nullptr;
    }
    case attr::Mode: {
      return nullptr;
    }
    case attr::NSConsumed: {
      return nullptr;
    }
    case attr::NSConsumesSelf: {
      return nullptr;
    }
    case attr::NSReturnsAutoreleased: {
      return nullptr;
    }
    case attr::NSReturnsNotRetained: {
      return nullptr;
    }
    case attr::NSReturnsRetained: {
      return nullptr;
    }
    case attr::Naked: {
      return nullptr;
    }
    case attr::NoAlias: {
      return nullptr;
    }
    case attr::NoCommon: {
      return nullptr;
    }
    case attr::NoCtor: {
      return nullptr;
    }
    case attr::NoDebug: {
      return nullptr;
    }
    case attr::NoDuplicate: {
      return nullptr;
    }
    case attr::NoEscape: {
      return nullptr;
    }
    case attr::NoInline: {
      return nullptr;
    }
    case attr::NoInstrumentFunction: {
      return nullptr;
    }
    case attr::NoMicroMips: {
      return nullptr;
    }
    case attr::NoMips16: {
      return nullptr;
    }
    case attr::NoReturn: {
      return nullptr;
    }
    case attr::NoSanitize: {
      return nullptr;
    }
    case attr::NoSplitStack: {
      return nullptr;
    }
    case attr::NoThreadSafetyAnalysis: {
      return nullptr;
    }
    case attr::NoThrow: {
      return nullptr;
    }
    case attr::NonNull: {
      return nullptr;
    }
    case attr::NotTailCalled: {
      return nullptr;
    }
    case attr::OMPCaptureKind: {
      return nullptr;
    }
    case attr::OMPCaptureNoInit: {
      return nullptr;
    }
    case attr::OMPDeclareSimdDecl: {
      return nullptr;
    }
    case attr::OMPDeclareTargetDecl: {
      return nullptr;
    }
    case attr::OMPThreadPrivateDecl: {
      return nullptr;
    }
    case attr::ObjCBoxable: {
      return nullptr;
    }
    case attr::ObjCBridge: {
      return nullptr;
    }
    case attr::ObjCBridgeMutable: {
      return nullptr;
    }
    case attr::ObjCBridgeRelated: {
      return nullptr;
    }
    case attr::ObjCDesignatedInitializer: {
      return nullptr;
    }
    case attr::ObjCException: {
      return nullptr;
    }
    case attr::ObjCExplicitProtocolImpl: {
      return nullptr;
    }
    case attr::ObjCIndependentClass: {
      return nullptr;
    }
    case attr::ObjCMethodFamily: {
      return nullptr;
    }
    case attr::ObjCNSObject: {
      return nullptr;
    }
    case attr::ObjCPreciseLifetime: {
      return nullptr;
    }
    case attr::ObjCRequiresPropertyDefs: {
      return nullptr;
    }
    case attr::ObjCRequiresSuper: {
      return nullptr;
    }
    case attr::ObjCReturnsInnerPointer: {
      return nullptr;
    }
    case attr::ObjCRootClass: {
      return nullptr;
    }
    case attr::ObjCRuntimeName: {
      return nullptr;
    }
    case attr::ObjCRuntimeVisible: {
      return nullptr;
    }
    case attr::ObjCSubclassingRestricted: {
      return nullptr;
    }
    case attr::OpenCLAccess: {
      return nullptr;
    }
    case attr::OpenCLIntelReqdSubGroupSize: {
      return nullptr;
    }
    case attr::OpenCLKernel: {
      return nullptr;
    }
    case attr::OpenCLUnrollHint: {
      return nullptr;
    }
    case attr::OptimizeNone: {
      return nullptr;
    }
    case attr::Overloadable: {
      return nullptr;
    }
    case attr::Override: {
      return nullptr;
    }
    case attr::Ownership: {
      return nullptr;
    }
    case attr::Packed: {
      return nullptr;
    }
    case attr::ParamTypestate: {
      return nullptr;
    }
    case attr::Pascal: {
      return nullptr;
    }
    case attr::PassObjectSize: {
      return nullptr;
    }
    case attr::Pcs: {
      return nullptr;
    }
    case attr::PragmaClangBSSSection: {
      return nullptr;
    }
    case attr::PragmaClangDataSection: {
      return nullptr;
    }
    case attr::PragmaClangRodataSection: {
      return nullptr;
    }
    case attr::PragmaClangTextSection: {
      return nullptr;
    }
    case attr::PreserveAll: {
      return nullptr;
    }
    case attr::PreserveMost: {
      return nullptr;
    }
    case attr::PtGuardedBy: {
      return nullptr;
    }
    case attr::PtGuardedVar: {
      return nullptr;
    }
    case attr::Pure: {
      return nullptr;
    }
    case attr::RegCall: {
      return nullptr;
    }
    case attr::ReleaseCapability: {
      return nullptr;
    }
    case attr::RenderScriptKernel: {
      return nullptr;
    }
    case attr::ReqdWorkGroupSize: {
      return nullptr;
    }
    case attr::RequireConstantInit: {
      return nullptr;
    }
    case attr::RequiresCapability: {
      return nullptr;
    }
    case attr::Restrict: {
      return nullptr;
    }
    case attr::ReturnTypestate: {
      return nullptr;
    }
    case attr::ReturnsNonNull: {
      return nullptr;
    }
    case attr::ReturnsTwice: {
      return nullptr;
    }
    case attr::SAXIAdaptor: {
      return nullptr;
    }
    case attr::SDxKernel: {
      return nullptr;
    }
    case attr::ScopedLockable: {
      return nullptr;
    }
    case attr::Section: {
      return nullptr;
    }
    case attr::SelectAny: {
      return nullptr;
    }
    case attr::Sentinel: {
      return nullptr;
    }
    case attr::SetTypestate: {
      return nullptr;
    }
    case attr::SharedTrylockFunction: {
      return nullptr;
    }
    case attr::StdCall: {
      return nullptr;
    }
    case attr::Suppress: {
      return nullptr;
    }
    case attr::SwiftCall: {
      return nullptr;
    }
    case attr::SwiftContext: {
      return nullptr;
    }
    case attr::SwiftErrorResult: {
      return nullptr;
    }
    case attr::SwiftIndirectResult: {
      return nullptr;
    }
    case attr::SysVABI: {
      return nullptr;
    }
    case attr::TLSModel: {
      return nullptr;
    }
    case attr::Target: {
      return nullptr;
    }
    case attr::TestTypestate: {
      return nullptr;
    }
    case attr::ThisCall: {
      return nullptr;
    }
    case attr::Thread: {
      return nullptr;
    }
    case attr::TransparentUnion: {
      return nullptr;
    }
    case attr::TryAcquireCapability: {
      return nullptr;
    }
    case attr::TypeTagForDatatype: {
      return nullptr;
    }
    case attr::TypeVisibility: {
      return nullptr;
    }
    case attr::Unavailable: {
      return nullptr;
    }
    case attr::Unpacked: {
      return nullptr;
    }
    case attr::Unused: {
      return nullptr;
    }
    case attr::Used: {
      return nullptr;
    }
    case attr::Uuid: {
      return nullptr;
    }
    case attr::VecReturn: {
      return nullptr;
    }
    case attr::VecTypeHint: {
      return nullptr;
    }
    case attr::VectorCall: {
      return nullptr;
    }
    case attr::Visibility: {
      return nullptr;
    }
    case attr::WarnUnused: {
      return nullptr;
    }
    case attr::WarnUnusedResult: {
      return nullptr;
    }
    case attr::Weak: {
      return nullptr;
    }
    case attr::WeakImport: {
      return nullptr;
    }
    case attr::WeakRef: {
      return nullptr;
    }
    case attr::WorkGroupSizeHint: {
      return nullptr;
    }
    case attr::X86ForceAlignArgPointer: {
      return nullptr;
    }
    case attr::XCLArrayGeometry: {
      return nullptr;
    }
    case attr::XCLArrayView: {
      return nullptr;
    }
    case attr::XCLArrayXForm: {
      return nullptr;
    }
    case attr::XCLDataFlow: {
      return nullptr;
    }
    case attr::XCLDependence: {
      return nullptr;
    }
    case attr::XCLFlattenLoop: {
      return nullptr;
    }
    case attr::XCLInline: {
      return nullptr;
    }
    case attr::XCLLatency: {
      return nullptr;
    }
    case attr::XCLLoopTripCount: {
      return nullptr;
    }
    case attr::XCLMaxWorkGroupSize: {
      return nullptr;
    }
    case attr::XCLOutline: {
      return nullptr;
    }
    case attr::XCLPipelineLoop: {
      return nullptr;
    }
    case attr::XCLPipelineWorkitems: {
      return nullptr;
    }
    case attr::XCLRegionName: {
      return nullptr;
    }
    case attr::XCLReqdPipeDepth: {
      return nullptr;
    }
    case attr::XCLSingleWorkitem: {
      return nullptr;
    }
    case attr::XCLUnrollWorkitems: {
      return nullptr;
    }
    case attr::XCLVisibility: {
      return nullptr;
    }
    case attr::XCLZeroGlobalWorkOffset: {
      return nullptr;
    }
    case attr::XRayInstrument: {
      return nullptr;
    }
    case attr::XRayLogArgs: {
      return nullptr;
    }
    case attr::XlxAggregate: {
      return nullptr;
    }
    case attr::XlxArrayGeometry: {
      return nullptr;
    }
    case attr::XlxArrayView: {
      return nullptr;
    }
    case attr::XlxArrayXForm: {
      return nullptr;
    }
    case attr::XlxBindOp: {
      return nullptr;
    }
    case attr::XlxBindOpExpr: {
      return nullptr;
    }
    case attr::XlxBindStorage: {
      return nullptr;
    }
    case attr::XlxDependence: {
      return nullptr;
    }
    case attr::XlxDisaggr: {
      return nullptr;
    }
    case attr::XlxExprBalance: {
      return nullptr;
    }
    case attr::XlxFuncInstantiate: {
      return nullptr;
    }
    case attr::XlxLoopTripCount: {
      return nullptr;
    }
    case attr::XlxMergeLoop: {
      return nullptr;
    }
    case attr::XlxOccurrence: {
      return nullptr;
    }
    case attr::XlxPipeline: {
      return nullptr;
    }
    case attr::XlxProtocol: {
      return nullptr;
    }
    case attr::XlxReqdPipeDepth: {
      return nullptr;
    }
    case attr::XlxResourceIPCore: {
      return nullptr;
    }
    case attr::XlxRewinding: {
      return nullptr;
    }
    case attr::XlxShared: {
      return nullptr;
    }
    case attr::XlxStable: {
      return nullptr;
    }
    case attr::XlxStableContent: {
      return nullptr;
    }
    case attr::XlxUnrollHint: {
      return nullptr;
    }
    case attr::XlxUnrollRegionHint: {
      return nullptr;
    }
    case attr::XlxVarReset: {
      return nullptr;
    }
  } // end switch
  llvm_unreachable("Unknown attribute!");
  return nullptr;
}

} // end namespace sema
} // end namespace clang
