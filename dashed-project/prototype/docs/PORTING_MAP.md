# G-Strings to ListVar porting map

| Old responsibility | ListVar destination |
|---|---|
| `DashedString` storage | `Gecode::List::Domain` (currently backed by `dashed::Domain`) |
| Character/range set | `dashed::ValueSet` over generic `int` |
| Fixed symbol blocks | `LiteralSegment` / `LiteralSlice` |
| Variable-width dashed block | `RepeatSegment` |
| `StringVarImp` domain ownership | `Gecode::List::ListVarImp` |
| `StringVarImp::eq` | equality propagator + `propagate_equal` |
| `StringVarImp::concat` | concat propagator + `propagate_concat` |
| mutable `StringView::pdomain()` | removed |
| view-level multi-variable operations | removed |
| manual domain update in view cloning | `VarImpView<ListVar>::update` only |
| string-specific alphabet constants | model-supplied `ValueSet` |
| string length fields | canonical `Domain` length interval |
| old `string.vis` | `gecode/list/var-imp/list.vis` |

## Suggested migration sequence

1. Freeze old tests and examples as behavioural fixtures.
2. Add converters from old dashed domains into `dashed::Domain` for differential
   testing only.
3. Port equality sweeps.
4. Port concatenation sweeps.
5. Replace old public `StringVar` with `ListVar` in one example model.
6. Add branching; length branching and exact backend partitions for unambiguous block counts and symbol positions are included.
7. Port remaining constraints one family at a time.
8. Remove the compatibility converter once all fixtures pass.
