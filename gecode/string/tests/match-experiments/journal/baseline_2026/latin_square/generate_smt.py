from pathlib import Path

MIN_N = 2
MAX_N = 26
ALPHABET = [
    "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
    "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
]
SMT_DIR = Path(__file__).resolve().parent / "smt"


def generate_n(n, out):
    out.write("(set-logic QF_SLIA)\n")
    out.write("(set-option :produce-models true)\n\n")

    for rc in ["row", "col"]:
        for k in range(n):
            out.write(f"(declare-fun {rc}_{k} () String)\n")
    out.write("\n")

    for i in range(n):
        for j in range(n):
            for rc in ["r", "c"]:
                out.write(f"(declare-fun {rc}_{i}_{j} () Int)\n")
    out.write("\n")

    # Every row and column is a length-n word over the n-symbol alphabet.
    for k in range(n):
        for rc in ["row", "col"]:
            out.write(
                f'(assert (str.in_re {rc}_{k} '
                f'(re.* (re.range "A" "{ALPHABET[n - 1]}"))))\n'
            )
            out.write(f"(assert (= (str.len {rc}_{k}) {n}))\n")
    out.write("\n")

    for i in range(n):
        for j in range(n):
            out.write(f"(assert (>= r_{i}_{j} 0))\n")
    out.write("\n")

    for i in range(n):
        for j in range(n):
            out.write(f"(assert (>= c_{i}_{j} 0))\n")
    out.write("\n")

    for i in range(n):
        for j in range(n):
            out.write(
                f'(assert (= r_{i}_{j} '
                f'(str.indexof row_{i} "{ALPHABET[j]}" 0)))\n'
            )
    out.write("\n")

    for i in range(n):
        for j in range(n):
            out.write(
                f'(assert (= c_{i}_{j} '
                f'(str.indexof col_{i} "{ALPHABET[j]}" 0)))\n'
            )
    out.write("\n")

    for symbol in range(n):
        for row in range(n):
            for col in range(n):
                out.write(
                    f"(assert (= (= r_{row}_{symbol} {col}) "
                    f"(= c_{col}_{symbol} {row})))\n"
                )

    out.write("\n(check-sat)\n")


def main():
    SMT_DIR.mkdir(parents=True, exist_ok=True)
    for n in range(MIN_N, MAX_N + 1):
        with (SMT_DIR / f"ls_{n}.smt2").open("w", encoding="utf-8") as out:
            generate_n(n, out)


if __name__ == "__main__":
    main()
