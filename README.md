# Bytecode interpreter

## Instructions Set

| Name | Variant | Stack Before | Stack After | Description
|:--:|:--|--:|:--|:--|
| NOP | | | | Perform no operation
| ADD | {bsil} | n0, n1 | result | Add two numbers
|     | {fd} | n0, n1 | result | Add two numbers
| SUB | {bsil} | n0, n1 | result | Subtract two numbers
|     | {fd} | n0, n1 | result | Subtract two numbers
| SBI | {bsil} | n0, n1 | result | Subtract and negate two numbers
|     | {fd} | n0, n1 | result | Subtract and negate two numbers
| MUL | {u} {bsil}  | n0, n1 | result | Multiply two numbers
|     | {fd} | n0, n1 | result | Multiply two numbers
| DIV | {u} {bsil} | n0, n1 | result | Divide two signednumbers
|     | {fd} | n0, n1 | result | Divide two numbers
| MOD | {u} {bsil} | n0, n1 | result | Modulo of two numbers
|     | {fd} | n0, n1 | result | Modulo of two numbers
