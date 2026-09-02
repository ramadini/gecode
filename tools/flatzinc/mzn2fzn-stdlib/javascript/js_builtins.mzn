int: STR_BUILTINS = 2;
int: ARRAY_BUILTINS = 2;

% Stores a JavaScript built-in property for object (Tx, Sx, Ax) at index i.
predicate
set_builtin_prop(
  var JSType: Tx, var string: Sx, var 0..N_ADDRS: Ax, var string: prop, int: i
) = 
  if prop = "length" then
    set_length_builtin(Tx, Sx, Ax, i)
  elseif str_reg(prop, "0|[1-9][0-9]*") then
    set_access_builtin(Tx, Sx, Ax, str2nat(prop), i)
  else
    del_prop_at(i)
  endif;
  
% Set all the builtins.
predicate
set_all_builtins(var JSType: Tx, var string: Sx, var 0..N_ADDRS: Ax, int: i) = 
  (Tx = Str -> set_str_builtins(Tx, Sx, Ax, i)) /\
  (Tx = Array -> set_array_builtins(Tx, Sx, Ax, i + STR_BUILTINS));
  
% Set all the String built-in properties for object at loc.
predicate
set_str_builtins(var JSType: Tx, var string: Sx, var 0..N_ADDRS: Ax, int: i) = 
  set_length_builtin(Tx, Sx, Ax, i) /\
  let { var int: k } in set_access_builtin(Tx, Sx, Ax, k, i + 1);

% Set all the Array built-in properties for object at loc.
predicate
set_array_builtins(var JSType: Tx, var string: Sx, var 0..N_ADDRS: Ax, int: i) = 
  set_length_builtin(Tx, Sx, Ax, i) /\
  let { var int: k } in set_access_builtin(Tx, Sx, Ax, k, i + 1);

% "length" built-in property.
predicate
set_length_builtin(
  var JSType: Tx, var string: Sx, var 0..N_ADDRS: Ax, int: i
) =
  if Tx = Str then
    set_prop_at(Ax, "length", Num, nat2str(str_len(Sx)), 0, i)
  elseif Tx = Array then
    set_prop_at(Ax, "length", Num, nat2str(str_gcc(Sx, ["~"])[1]), 0, i)
  else
    del_prop_at(i)
  endif;
  
% Access to the i-th element of the object.
predicate
set_access_builtin(
  var JSType: Tx, var string: Sx, var 0..N_ADDRS: Ax, var int: k, int: i
) =
  if Tx = Str /\ k >= 0 /\ k < str_len(Sx) then
    set_prop_at(Ax, nat2str(k), Str, str_char_at(Sx, k + 1), 0, i)
  elseif Tx = Array /\ k >= 0 /\ 2*k < str_len(Sx) then
    let {
      var 0..N_PROPS: j = tmp_int();
      constraint P_TYPES[j] != Undef -> str_len(Sx) > k;
    } in set_prop_at(Ax, nat2str(k), P_TYPES[j], P_SVALS[j], P_ADDRS[j], i)
  else
    del_prop_at(i)
  endif;

