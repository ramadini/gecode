namespace Gecode { namespace String {
  
  forceinline
  ConstDashedView::ConstDashedView() 
  : b0(nullptr), n(-1) { GECODE_NEVER; }
  
  forceinline
  ConstDashedView::ConstDashedView(const Block& b, int n0) 
  : DashedViewBase(this), b0(&b), n(n0) {}
  
  forceinline int 
  ConstDashedView::size() const {
    return n;
  }
  
  forceinline const Block&
  ConstDashedView::operator[](int i) const {
    return *(b0+i);
  }
  
  forceinline bool
  ConstDashedView::isOK() const {
    if (n <= 0)
      return false;
    for (int i = 0; i < n; i++)
      if (!b0[i].isOK())
        return false;
  }
  
  forceinline bool
  ConstDashedView::isNull() const {
    return n == 1 && b0->isNull();
  }
  
  forceinline std::vector<int>
  ConstDashedView::val(void) const {
    std::vector<int> v;
    for (int i = 0; i < n; ++i) {
      std::vector<int> val_i = b0[i].val();
      v.insert(v.end(), val_i.begin(), val_i.end());
    }
    return v;
  }

  forceinline SweepFwdIterator<ConstDashedView>
  ConstDashedView::fwd_iterator(void) const {
    return SweepFwdIterator<ConstDashedView>(*this);
  }
  
  forceinline SweepBwdIterator<ConstDashedView>
  ConstDashedView::bwd_iterator(void) const {
    return SweepBwdIterator<ConstDashedView>(*this);
  }

  template<class Char, class Traits>
  forceinline  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const ConstDashedView& d) {
    int n = d.size();
    for (int i = 0; i < n - 1; ++i)
      os << d[i] << " + ";
    os << d[n-1];
    return os;
  };
  
}}

