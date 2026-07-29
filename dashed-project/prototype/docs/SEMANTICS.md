# Semantics of the first constraint kernel

Let `L(D)` be the finite-list language represented by domain `D`.

## Equality

`equal(x,y)` enforces:

```text
x = y
```

The current kernel:

- intersects length intervals;
- transfers an assigned side to the other side;
- intersects corresponding fixed-boundary repeat blocks when both domains have
  the same safe shape;
- detects assigned conflicts;
- subsumes when both sides are assigned to the same list.

The same-shape operation deliberately refuses variable-width block alignment,
because two equal concrete lists can have different internal segment splits.

## Disequality

`not_equal(x,y)` enforces:

```text
x != y
```

The current kernel subsumes when equality propagation proves the represented
languages disjoint and checks assigned values directly. When one side is
assigned, it also performs two exact representable exclusions on the other
side:

- remove the forbidden value from the only one-position value block when all
  other positions are fixed to the assigned list;
- remove a forbidden endpoint repetition count when exactly one singleton
  block has a variable count and every other segment is fixed.

An interior forbidden count would create a hole in an interval, and excluding
one list from a repeated non-singleton alphabet would require a disjunction.
Those cases are therefore left unchanged rather than approximated unsafely.

## Reified equality and disequality

The standalone `BoolDomain` is a test double for Gecode's `BoolView`:

```text
b <-> (x = y)
b <-> (x != y)
```

A Gecode adapter should use its standard reification modes and Boolean views;
the standalone kernel currently implements full equivalence.

## Concatenation

`concat(z,x,y)` enforces:

```text
z = x ++ y
```

The current kernel reaches a fixpoint on length bounds:

```text
|min x| + |min y| <= |z| <= |max x| + |max y|
|x| constrained by |z| - |y|
|y| constrained by |z| - |x|
```

It also performs exact propagation when two of the three lists are assigned:

- assigned `x` and `y` assign `z`;
- assigned `z` and `x` assign the suffix `y`;
- assigned `z` and `y` assign the prefix `x`.

Exact prefix/suffix domains share literal backing storage.

## Length

`length(x,n)` enforces:

```text
|x| = n
```

The standalone integer side is an interval. The Gecode propagator should use an
`IntView`, subscribe to `PC_LIST_LEN` and `PC_INT_BND`, and commit bounds in both
directions.

## Soundness discipline

Every propagator operation must be language-reducing:

```text
new language subset-of old language
```

When a stronger old G-Strings sweep is ported, it should first be written as a
pure `Domain` operation and checked against enumerated small languages before it
is connected to Gecode events.
