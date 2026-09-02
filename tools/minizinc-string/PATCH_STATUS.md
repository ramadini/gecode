# Patch libminizinc 2.10.0 per `var string` (Dashed)

Stato: IN CORSO — controllo diretto (agente background abbandonato, non aveva
prodotto modifiche sorgenti).

## Obiettivo
Estendere libminizinc 2.10.0 per riconoscere `var string` come tipo di
variabile di decisione, mappandolo sui predicati FlatZinc nativi già
implementati in Gecode (`gecode/flatzinc/registry.cpp`), senza reimplementare
propagazione lato MiniZinc.

## Ambiente
- Sorgente target: `.work/libminizinc-2.10.0` (clone pulito, tag 2.10.0)
- Build: `.work/build-2.10.0` (CMake, Debug) → binario `minizinc` (case unificato
  mzn2fzn+solve, non esiste più un mzn2fzn separato in 2.10.0)
- Test manuale:
  ```
  ./minizinc --stdlib-dir <src>/share/minizinc --compile \
    --solver org.minizinc.mzn-fzn model.mzn -o out.fzn
  ```

## Modifiche applicate finora

1. **`lib/typecheck.cpp` (~riga 3695)**: rimossa `BT_STRING` dal controllo che
   rifiutava dichiarazioni `var string` in `vTypeInst`. Ora `var string: x;`
   è accettato dal typechecker.

2. **`include/minizinc/ast.hh`** (`class Constants`): aggiunta struct
   `string_` con identificatori builtin (`eq`, `ne`, `lt`, `le`, `concat`)
   analoga a `int_`/`float_`/`bool_`.

3. **`lib/ast.cpp`** (init `Constants`): aggiunta inizializzazione di
   `ids.string_.*` con i nomi FlatZinc corrispondenti (`str_eq`, `str_ne`,
   `str_lt`, `str_le`, `str_concat`).

4. **`lib/flatten/flatten_binop.cpp`** (`op_to_builtin`): aggiunto case
   `Type::BT_STRING` che mappa gli operatori MiniZinc `=`, `!=`, `<`, `<=`,
   `++` sui builtin string corrispondenti (stesso schema di int/float/bool).

5. **`share/minizinc/std/flatzinc_builtins.mzn`**: aggiunta sezione
   `@groupdef flatzinc.string` con le dichiarazioni `predicate str_*(...)`
   per TUTTI i predicati già presenti nel registry Gecode (eq/ne/lt/le
   (+reif), concat(+_c), pow(+_c), rev(+_c), lcase, ucase, len, char_at,
   sub, char2code, str2nat, nat2str, find, rfind, replace/replace_all/
   replace_last, contains(+reif), gcc, gconcat, reg(+reif), match,
   array_string_element, array_var_string_element). Queste dichiarazioni
   sono necessarie affinché il typechecker/flattener riconoscano le
   chiamate a questi predicati usate come vincoli o (in futuro) come
   funzioni espressione.

   NON ancora aggiunte: `str_chars`, `str_alphabet`, `str_range`, `str_dfa`
   (+reif) — richiedono AST `CharSetLit`/`DFA` non ancora portato nel
   frontend FlatZinc Gecode (vedi nota nel registry).

## Verificato funzionante (end-to-end MiniZinc → FlatZinc → Gecode)

```
var string: x;
var string: y;
constraint x = y;
constraint x != "abc";
solve satisfy;
```
→ FlatZinc generato:
```
var string: x:: output_var;
var string: y:: output_var;
constraint str_eq(x,y);
constraint str_ne(x,"abc");
solve  satisfy;
```
→ risolto correttamente da `fzn-gecode` (build-string-port/bin/fzn-gecode).
Senza bound di lunghezza la ricerca è ovviamente infinita (comportamento
atteso, nessun bug).

## Verificato funzionante (aggiornamento 2)

6. **`share/minizinc/std/stdlib/stdlib_string.mzn`**: aggiunta overload
   `function var string: '++'(var string: s1, var string: s2) = let { var
   string: z; constraint str_concat(s1, s2, z); } in z;` accanto a quella
   `par` già esistente, così l'operatore `++` è risolvibile da `matchFn`
   anche su `var string`.

7. **`lib/flatten/flatten_binop.cpp`** (`case BOT_PLUSPLUS`): aggiunto un
   controllo iniziale che, quando l'operando è `var string` (non array),
   delega al path booleano/builtin standard (`flatten_bool_op`) invece di
   entrare nel path di concatenazione di array, che assumeva sempre
   `ArrayLit` e falliva con "array without initialiser not supported".

### Test end-to-end aggiuntivi verificati
```
var string: x; var string: y; var string: z;
var int: n;
constraint x ++ y = z;
constraint str_len(x, n);
constraint n <= 3;
constraint str_len(y, n);
solve satisfy;
```
→ FlatZinc generato correttamente (usa `str_concat`, `str_eq` per l'aux var,
`str_len`, `int_le`) e risolto con successo da `fzn-gecode` (verificate le
prime 3 soluzioni, coerenti: `n=0` stringa vuota, `n=1` stringhe di un
carattere concatenate in una di due).

**Confermato**: sia l'operatore `++` sia le funzioni-espressione (`str_len`)
funzionano su `var string` con la sola dichiarazione `predicate` + un
piccolo overload di `'++'`, senza serie modifiche al flattener oltre al
fix del case BOT_PLUSPLUS.

## Prossimi passi
- Testare le altre funzioni-espressione (str_sub, str_rev, str_lcase,
  str_ucase, str_char_at, str2nat/nat2str, str_find/rfind, str_replace*)
  dichiarate come `predicate` — verificare se vanno bene così o se serve
  un wrapper `function` analogo a quello fatto per `++` quando usate in
  posizione di espressione (es. `y = str_rev(x)` invece di
  `constraint str_rev(x, y)`).
- Testare sintassi con dominio: `var string(5): y;` (lunghezza fissa) e
  `var string of {"a","b","e"}: z;` (charset) — verificare se il parser
  MiniZinc supporta `MZN_STRING '(' expr ')'` e `MZN_STRING MZN_OF set_expr`
  nella grammatica `base_ti_expr_tail`, e come tradurre il dominio in FlatZinc
  (StringVarSpec Gecode non ha ancora un modo esplicito di specificare bound
  di lunghezza/charset via FlatZinc — DA VERIFICARE, potrebbe richiedere
  ulteriori vincoli `str_len`/`str_gcc` sintetici generati dal flattener).
- Verificare l'output printer per array di `var string` (solns2out).
- Documentare il diff completo in `patches/var-string-2.10.0.patch`.
- Decidere binario finale: `mzn2fzn-dashed`/wrapper `minizinc` rinominato.

## Aggiornamento 3 — Supporto array di stringhe nel parser FlatZinc di Gecode

Scoperto un gap reale (non di libminizinc, ma del frontend FlatZinc nativo di
Gecode): `gecode/flatzinc/parser.yxx`/`parser.hh` non supportavano affatto
`array [1..n] of string` (par) né `array [1..n] of var string` — solo
`var string` scalare. Questo bloccava tutti i predicati che richiedono
argomenti array: `str_gcc`, `str_gconcat`, `array_string_element`,
`array_var_string_element`, già presenti e testati nel registry Gecode a
livello nativo ma irraggiungibili da FlatZinc.

### Modifiche applicate
File: `gecode/flatzinc/parser.hh`
- Aggiunti `ST_STRINGVARARRAY`, `ST_STRINGVALARRAY` all'enum `SymbolType`.
- Aggiunti costruttori `se_tva(int)` (temp string var array) e `se_ta(int)`
  (temp string val/par array) analoghi a quelli int/float/set/bool.

File: `gecode/flatzinc/parser.yxx` (seguito esattamente il pattern
int/float/set per ogni punto):
1. Union Bison: aggiunto `std::vector<std::string>* stringListValue`.
2. `%type` declarations per `string_list`, `string_list_head`,
   `string_init`, `string_init_list(_head)`, `string_var_array_literal`,
   `vardecl_string_var_array_init`.
3. Regola `string_list`/`string_list_head` (per `array [1..n] of string:
   id = [...]`, cioè array PAR di stringhe letterali).
4. Regola di dichiarazione per `array [1..n] of string: id = [...]`
   (usa `se_ta`).
5. Regole `string_init`, `string_init_list(_head)`,
   `string_var_array_literal`, `vardecl_string_var_array_init` (per
   `array [1..n] of var string: id [= [...]]`, con supporto ad alias e a
   letterali, usa `se_tva`).
6. Case `ST_STRINGVARARRAY`/`ST_STRINGVALARRAY` in `getArrayElement()`
   (accesso indicizzato `xs[i]`).
7. Case `ST_STRINGVARARRAY`/`ST_STRINGVALARRAY` in **entrambe** le
   occorrenze di `non_array_expr` che gestiscono il riferimento all'intero
   array come argomento di una chiamata/annotazione (costruiscono un
   `AST::Array` di `AST::StringVar`/`AST::String` rispettivamente).

### Verifica
- Rigenerato `parser.tab.cpp/hpp` con `bison -d` (nessun conflitto grammaticale,
  solo i consueti warning "POSIX Yacc does not support %define").
- Ricompilato **entrambe** le build (lezione appresa da sessioni precedenti):
  - CMake `build-string-port` (`make -j$(nproc)`— successo, nessun errore).
  - Autotools root (`make -j$(nproc)` — rigenera `lexer.yy.cpp` via flex e
    `libgecodeflatzinc.so`/`fzn-gecode` — successo).
- Test FlatZinc diretto (bypassando MiniZinc) con entrambi i binari
  `fzn-gecode` (CMake e root):
  ```
  array [1..3] of string: cs = ["a","b","c"];
  array [1..3] of var string: xs;
  constraint array_var_string_element(1, xs, xs[1]);
  constraint str_eq(xs[1], cs[1]);
  solve satisfy;
  ```
  → risolto senza errori.
  ```
  array [1..3] of string: alphabet = ["a","b","c"];
  array [1..3] of var int: card;
  var string: x :: output_var;
  constraint str_len(x, 3);
  constraint str_gcc(x, alphabet, card);
  solve satisfy;
  ```
  → risolto correttamente, 3 soluzioni verificate.

**Risultato**: il parser FlatZinc di Gecode ora supporta pienamente array di
stringhe (par e var), sbloccando `str_gcc`, `str_gconcat`,
`array_string_element`, `array_var_string_element` da FlatZinc.

## Prossimi passi (aggiornati)
- Testare `str_gcc`/`array_var_string_element` anche attraverso la pipeline
  completa MiniZinc → FlatZinc (non solo FlatZinc diretto), es.
  `str_gcc(["a","b","c"], [a,b,c])` come nel test.mzn storico.
- Continuare con: sintassi di dominio (`var string(N)`, `var string of
  {...}`) nella grammatica MiniZinc — gap ancora aperto, non iniziato.
- `str_dfa`/`str_chars`/`str_alphabet`/`str_range` — richiedono AST
  CharSetLit/DFA non ancora portato, da rimandare o documentare come non
  supportato in questa fase.
- Estrarre il diff completo (libminizinc + gecode/flatzinc) in
  `patches/var-string-2.10.0.patch`.

### Conferma pipeline completa MiniZinc → FlatZinc → Gecode
Testato anche partendo da modello `.mzn` (non solo FlatZinc diretto):
```
var string: x;
array[1..3] of var int: card;
constraint str_len(x, 3);
constraint str_gcc(x, ["a","b","c"], card);
solve satisfy;
```
`minizinc --compile` genera FlatZinc valido (l'array letterale `["a","b","c"]`
diventa `array [1..3] of string: X_INTRODUCED_3_ = ["a","b","c"];`), e
`fzn-gecode` lo risolve correttamente (3 soluzioni verificate, coerenti:
stringa vuota di lunghezza 3 con multiplicity `[0,0,0]` essendo senza bound
sull'alfabeto effettivo dei caratteri). **Pipeline end-to-end confermata
funzionante per `str_gcc` con array di stringhe.**
