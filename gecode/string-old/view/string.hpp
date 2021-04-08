namespace Gecode { namespace String {

  forceinline
  StringView::StringView(void) {}

  forceinline
  StringView::StringView(const StringVar& y)
  : VarImpView<StringVar>(y.varimp()) {}

  forceinline
  StringView::StringView(StringVarImp* y)
  : VarImpView<StringVar>(y) {}

  forceinline bool
  StringView::same(const StringView& that) const {
    return this->varimp() == that.varimp();
  }

  forceinline bool
  StringView::check_equate(const StringView& that) const {
    return x->ds.check_equate(that.x->ds);
  }

  forceinline string
  StringView::val() const {
    return x->ds.val();
  }

  forceinline String::DashedString*
  StringView::pdomain() const {
    return &x->ds;
  }

  forceinline ModEvent
  StringView::dom(Space& home, const NSBlocks& d) {
    return x->dom(home, d);
  }

  forceinline ModEvent
  StringView::eq(Space& home, string s) {
    return x->eq(home, s);
  }

  forceinline ModEvent
  StringView::eq(Space& home, StringView& y) {
    return x->eq(home, y.varimp());
  }

  forceinline ModEvent
  StringView::lex(Space& home, StringView& y, bool lt) {
    return x->lex(home, y.varimp(), lt);
  }

  forceinline ModEvent
  StringView::inc(Space& home, bool lt) {
    return x->inc(home, lt);
  }

  forceinline ModEvent
  StringView::commit(
    Space& home, Branch::Level l, Branch::Value v, Branch::Block b, unsigned a
  ) {
    return x->commit(home, l, v, b, a);
  }

  forceinline ModEvent
  StringView::lb(Space& home, int l) {
    return x->lb(home, l);
  }

  forceinline ModEvent
  StringView::ub(Space& home, int u) {
    return x->ub(home, u);
  }

  forceinline ModEvent
  StringView::concat(Space& home, StringView& a, StringView& b) {
    return x->concat(home, a.varimp(), b.varimp());
  }

  forceinline ModEvent
  StringView::gconcat(Space& home, ViewArray<StringView>& a) {
    vec<StringVarImp*> v;
    for (auto i : a)
      v.push(i.varimp());
    return x->gconcat(home, v);
  }

  forceinline ModEvent
  StringView::pow(Space& home, StringView& a, int& l, int& u) {
    return x->pow(home, a.varimp(), l, u);
  }

  forceinline ModEvent
  StringView::find(Space& home, StringView& a, int& l, int& u, bool b) {
    return x->find(home, a.varimp(), l, u, b);
  }

  forceinline ModEvent
  StringView::rev(Space& home, StringView& a) {
    return x->rev(home, a.varimp());
  }

  forceinline int
  StringView::min_length() const {
    return x->ds.min_length();
  }

  forceinline int
  StringView::max_length() const {
    return x->ds.max_length();
  }

  forceinline NSIntSet
  StringView::may_chars() const {
    return x->ds.may_chars();
  }

  forceinline NSIntSet
  StringView::must_chars() const {
    return x->ds.must_chars();
  }

  forceinline void
  StringView::update(Space& home, StringView& y) {
    VarImpView::update(home, y);
    x->ds.update(home, y.x->ds);
  }

  forceinline bool
  StringView::contains(const StringView& s) const {
    return x->ds.contains(*s.pdomain());
  }

  template<class Char, class Traits>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const StringView& v) {
    os << "Var "<< v.x << ": " << v.x->domain() << " [" << v.min_length() 
       << ".." << v.max_length() << "]";
    return os;
  }

}}
