namespace Gecode { namespace String { namespace Branch {

    None_LLLL::None_LLLL(Home home, ViewArray<String::StringView>& x0)
    : StringBrancher(home, x0) {};

    None_LLLL::None_LLLL(Home home, None_LLLL& b)
    : StringBrancher(home, b, b.start) { x.update(home, b.x); }

    Actor*
    None_LLLL::copy(Space& home) { return new (home) None_LLLL(home, *this); }

    void
    None_LLLL::post(Home home, ViewArray<String::StringView>& x) {
      (void) new (home) None_LLLL(home, x);
    }

    Choice*
    None_LLLL::choice(Space&) { return val_llll(start, x[start]);  }

    ExecStatus
    None_LLLL::commit(Space& home, const Choice& c, unsigned a) {
      const PosLevVal& plv = static_cast<const PosLevVal&>(c);
      // this->print(home, c, a, std::cerr); std::cerr << '\n';
      x[plv.pos].commit(home, plv.lev, Value::MIN, Block::LEFTMOST, a);
      return ES_OK;
    }

}}}
