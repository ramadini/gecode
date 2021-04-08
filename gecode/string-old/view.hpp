namespace Gecode { namespace String {

  class StringView : public VarImpView<StringVar> {

  protected:

    using VarImpView<StringVar>::x;

  public:

    StringView(void);

    StringView(const StringVar& y);

    StringView(StringVarImp* y);

    string val() const;

    String::DashedString* pdomain(void) const;

    bool same(const StringView& that) const;

    bool check_equate(const StringView& that) const;

    int min_length(void) const;

    int max_length(void) const;

    NSIntSet may_chars(void) const;

    NSIntSet must_chars(void) const;

    bool contains(const StringView& s) const;

    template<class Char, class Traits>
    friend std::basic_ostream<Char,Traits>&
    operator <<(std::basic_ostream<Char,Traits>& os, const StringView& x);

    //ModEvents

    ModEvent dom(Space& home, const NSBlocks& d);

    ModEvent eq(Space& home, string s);

    ModEvent eq(Space& home, StringView& y);

    ModEvent lex(Space& home, StringView& y, bool lt);

    ModEvent inc(Space& home, bool lt);

    ModEvent commit(Space&, Branch::Level, Branch::Value, Branch::Block, unsigned);

    ModEvent lb(Space& home, int l);

    ModEvent ub(Space& home, int u);

    ModEvent concat(Space& home, StringView& x, StringView& y);

    ModEvent gconcat(Space& home, ViewArray<StringView>& x);

    ModEvent pow(Space& home, StringView& x, int& l, int& u);

    ModEvent find(Space& home, StringView& x, int& l, int& u, bool b);

    ModEvent rev(Space& home, StringView& y);

    void update(Space& home, StringView& y);

  };

  template<class Char, class Traits>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const StringView& x);

}}

#include <gecode/string/view/string.hpp>

