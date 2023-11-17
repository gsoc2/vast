// Copyright (c) 2023-present, Trail of Bits, Inc.

#ifndef VAST_DIALECT_HIGHLEVEL_IR_HIGHLEVELASSEMBLY
#define VAST_DIALECT_HIGHLEVEL_IR_HIGHLEVELASSEMBLY

include "mlir/IR/OpBase.td"

include "mlir/Interfaces/CastInterfaces.td"
include "vast/Interfaces/SymbolInterface.td"

def InlineAsmOp
  : HighLevel_Op< "inline_asm", [/*AttrSizedOperandSegments*/] >
  , Arguments<(ins
      AnyType:$asm_template,
      UnitAttr:$has_volatile,
      UnitAttr:$has_goto
      //UnitAttr:$inline, Clang doesn't have this qualifier?
      //Variadic< AnyType >:$asm_operands,
      //Variadic< AnyType >:$labels,
      //OptionalAttr< ArrayAttr >:$constraints,
      //OptionalAttr< ArrayAttr >:$clobbers
    )>
{
  let summary = "VAST operation for inline assembly";
  let description = [{ TODO }];

  let results = (outs Optional< AnyType >:$res);

  let assemblyFormat = [{attr-dict $asm_template `:` functional-type(operands, results)}];
  //let assemblyFormat = [{attr-dict $asm_template (`:` $asm_operands^)? (`:` $constraints^)? (`:` $clobbers^)? (`:` $labels^)? `:` functional-type(operands, results)}];
}

#endif // VAST_DIALECT_HIGHLEVEL_IR_HIGHLEVELASSEMBLY
