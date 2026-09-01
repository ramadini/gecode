#include <gecode/string.hh>
#include <gecode/driver.hh>
#include <iostream>
#include <vector>

using namespace Gecode;
using namespace String;


// Problem parameters are now passed via constructor, not globals

class InglenookOptions : public SizeOptions {
public:
    std::string S_init[4];
    std::string S_final[4];
    int A_cap = 5, B_cap = 3, C_cap = 3, X_cap = 3;
    int max_steps = 1;
    InglenookOptions(const char* s)
        : SizeOptions(s) {
        S_init[0] = "124"; S_init[1] = ""; S_init[2] = ""; S_init[3] = "35";
        S_final[0] = "124"; S_final[1] = ""; S_final[2] = "35"; S_final[3] = "";
    }
};

class Inglenook : public Script {
public:
    StringVarArgs A, B, C, X;
    BoolVarArgs moveVars;
    StringVarArgs concatVars;
    int max_steps;
    int A_cap, B_cap, C_cap, X_cap;
    std::string S_init[4];
    std::string S_final[4];

    Inglenook(const SizeOptions& opt)
        : Script(opt)
    {
        const InglenookOptions& iopt = static_cast<const InglenookOptions&>(opt);
        max_steps = iopt.max_steps;
        A_cap = iopt.A_cap;
        B_cap = iopt.B_cap;
        C_cap = iopt.C_cap;
        X_cap = iopt.X_cap;
        for (int i = 0; i < 4; ++i) {
            S_init[i] = iopt.S_init[i];
            S_final[i] = iopt.S_final[i];
        }
        A = StringVarArgs(max_steps + 1);
        B = StringVarArgs(max_steps + 1);
        C = StringVarArgs(max_steps + 1);
        X = StringVarArgs(max_steps + 1);
        // Create variables for each step
        NSIntSet allowed;
        allowed.add('1');
        allowed.add('2');
        allowed.add('3');
        allowed.add('4');
        allowed.add('5');
        allowed.add('x');
        // Step 0: initial state
        NSIntSet allowedA, allowedB, allowedC, allowedX;
        for (char ch : S_init[0]) allowedA.add(ch);
        for (char ch : S_init[1]) allowedB.add(ch);
        for (char ch : S_init[2]) allowedC.add(ch);
        for (char ch : S_init[3]) allowedX.add(ch);

        A[0] = StringVar(*this, allowedA, S_init[0].size(), S_init[0].size());
        B[0] = StringVar(*this, allowedB, S_init[1].size(), S_init[1].size());
        C[0] = StringVar(*this, allowedC, S_init[2].size(), S_init[2].size());
        X[0] = StringVar(*this, allowedX, S_init[3].size(), S_init[3].size());
        // Fix initial state exactly
        rel(*this, A[0], STRT_EQ, StringVar(*this, S_init[0].c_str()));
        rel(*this, B[0], STRT_EQ, StringVar(*this, S_init[1].c_str()));
        rel(*this, C[0], STRT_EQ, StringVar(*this, S_init[2].c_str()));
        rel(*this, X[0], STRT_EQ, StringVar(*this, S_init[3].c_str()));
        // Steps 1..max_steps: allowed domain
        for (int i = 1; i <= max_steps; ++i) {
            A[i] = StringVar(*this, allowed, 0, A_cap);
            B[i] = StringVar(*this, allowed, 0, B_cap);
            C[i] = StringVar(*this, allowed, 0, C_cap);
            X[i] = StringVar(*this, allowed, 0, X_cap);
        }
        // Transition constraints
        moveVars = BoolVarArgs();
        concatVars = StringVarArgs();
        for (int i = 0; i < max_steps; ++i) {
            // Move constraints: only one track changes per step
            BoolVar moveA(*this, 0, 1);
            BoolVar moveB(*this, 0, 1);
            BoolVar moveC(*this, 0, 1);
            moveVars << moveA << moveB << moveC;
            // Move A
            rel(*this, A[i], STRT_NQ, A[i + 1], Reify(moveA, RM_IMP));
            rel(*this, B[i], STRT_EQ, B[i + 1], Reify(moveA, RM_IMP));
            rel(*this, C[i], STRT_EQ, C[i + 1], Reify(moveA, RM_IMP));
            StringVar concat_AX(*this, allowed, 0, A_cap + X_cap);
            StringVar concat_A_nextX_next(*this, allowed, 0, A_cap + X_cap);
            StringVarArgs ax_args;
            ax_args << A[i] << X[i];
            gconcat(*this, ax_args, concat_AX);
            StringVarArgs ax_next_args;
            ax_next_args << A[i + 1] << X[i + 1];
            gconcat(*this, ax_next_args, concat_A_nextX_next);
            rel(*this, concat_AX, STRT_EQ, concat_A_nextX_next, Reify(moveA, RM_IMP));
            concatVars << concat_AX << concat_A_nextX_next;
            //rel(*this,moveA, IRT_EQ, 0);

            // Move B
            rel(*this, B[i], STRT_NQ, B[i + 1], Reify(moveB, RM_IMP));
            rel(*this, A[i], STRT_EQ, A[i + 1], Reify(moveB, RM_IMP));
            rel(*this, C[i], STRT_EQ, C[i + 1], Reify(moveB, RM_IMP));
            StringVar concat_BX(*this, allowed, 0, B_cap + X_cap);
            StringVar concat_B_nextX_next(*this, allowed, 0, B_cap + X_cap);
            StringVarArgs bx_args;
            bx_args << B[i] << X[i];
            gconcat(*this, bx_args, concat_BX);
            StringVarArgs bx_next_args;
            bx_next_args << B[i + 1] << X[i + 1];
            gconcat(*this, bx_next_args, concat_B_nextX_next);
            rel(*this, concat_BX, STRT_EQ, concat_B_nextX_next, Reify(moveB, RM_IMP));
            concatVars << concat_BX << concat_B_nextX_next;
            //rel(*this,moveB, IRT_EQ, 0);


            // Move C
            rel(*this, C[i], STRT_NQ, C[i + 1], Reify(moveC, RM_IMP));
            rel(*this, A[i], STRT_EQ, A[i + 1], Reify(moveC, RM_IMP));
            rel(*this, B[i], STRT_EQ, B[i + 1], Reify(moveC, RM_IMP));
            StringVar concat_CX(*this, allowed, 0, C_cap + X_cap);
            StringVar concat_C_nextX_next(*this, allowed, 0, C_cap + X_cap);
            StringVarArgs cx_args;
            cx_args << C[i] << X[i];
            gconcat(*this, cx_args, concat_CX);
            StringVarArgs cx_next_args;
            cx_next_args << C[i + 1] << X[i + 1];
            gconcat(*this, cx_next_args, concat_C_nextX_next);
            rel(*this, concat_CX, STRT_EQ, concat_C_nextX_next, Reify(moveC, RM_IMP));
            concatVars << concat_CX << concat_C_nextX_next;
            //rel(*this,moveC, IRT_EQ, 0);


            linear(*this, IntArgs({1, 1, 1}), BoolVarArgs({moveA, moveB, moveC}), IRT_EQ, 1);
        }
        // Final state constraint: fix all four tracks
        // rel(*this, A[max_steps], STRT_EQ, StringVar(*this, S_final[0].c_str()));
        // rel(*this, B[max_steps], STRT_EQ, StringVar(*this, S_final[1].c_str()));
        // rel(*this, C[max_steps], STRT_EQ, StringVar(*this, S_final[2].c_str()));
        // rel(*this, X[max_steps], STRT_EQ, StringVar(*this, S_final[3].c_str()));
        // Branching
        StringVarArgs vars;
        for (int i = 0; i <= max_steps; ++i) {
            vars << A[i] << B[i] << C[i] << X[i];
        }

        branch(*this, moveVars, BOOL_VAR_NONE(), BOOL_VAL_MIN());
        blockmin_lllm(*this, vars);
        blockmin_lllm(*this, concatVars);
    }

    Inglenook(Inglenook &s) : Script(s),
        A(s.A.size()), B(s.B.size()), C(s.C.size()), X(s.X.size()),
        moveVars(s.moveVars.size()), concatVars(s.concatVars.size()),
        max_steps(s.max_steps), A_cap(s.A_cap), B_cap(s.B_cap), C_cap(s.C_cap), X_cap(s.X_cap)
    {
        for (int i = 0; i < 4; ++i) {
            S_init[i] = s.S_init[i];
            S_final[i] = s.S_final[i];
        }
        for (int i = 0; i < A.size(); ++i) {
            A[i].update(*this, s.A[i]);
            B[i].update(*this, s.B[i]);
            C[i].update(*this, s.C[i]);
            X[i].update(*this, s.X[i]);
        }
        for (int i = 0; i < moveVars.size(); ++i) {
            moveVars[i].update(*this, s.moveVars[i]);
        }
        for (int i = 0; i < concatVars.size(); ++i) {
            concatVars[i].update(*this, s.concatVars[i]);
        }
    }

    virtual Space *copy() { return new Inglenook(*this); }

    virtual void print(std::ostream &os) const {
        os << "=========================\n";
        for (int i = 0; i <= max_steps; ++i) {
            os << "Step " << i << ": ";
            os << "A=" << A[i].val() << " B=" << B[i].val() << " C=" << C[i].val() << " X=" << X[i].val();
            if (i < max_steps) {
                int mv_idx = 3 * i;
                int cv_idx = 6 * i;
                os << " | moveA=" << moveVars[mv_idx].val();
                os << " moveB=" << moveVars[mv_idx+1].val();
                os << " moveC=" << moveVars[mv_idx+2].val();
                os << " | concat_AX=" << concatVars[cv_idx].val();
                os << " concat_A_nextX_next=" << concatVars[cv_idx+1].val();
                os << " | concat_BX=" << concatVars[cv_idx+2].val();
                os << " concat_B_nextX_next=" << concatVars[cv_idx+3].val();
                os << " | concat_CX=" << concatVars[cv_idx+4].val();
                os << " concat_C_nextX_next=" << concatVars[cv_idx+5].val();
            }
            os << "\n";
        }
        os << "=========================\n";
    }
};


int main(int argc, char *argv[]) {
    InglenookOptions opt("*** Inglenook minimal Dashed ***");
    bool all = true; // Set to false for standard approach (first solution only)
    if (all) {
        DFS<Inglenook> e(new Inglenook(opt));
        int count = 0;
        while (Inglenook* s = e.next()) {
            s->print(std::cout);
            ++count;
            delete s;
        }
        std::cout << "Number of solutions: " << count << std::endl;
    } else {
        Script::run<Inglenook, DFS, InglenookOptions>(opt);
    }
    return 0;
}
