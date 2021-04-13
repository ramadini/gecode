#include <gecode/string.hh>

namespace Gecode {

// TODO: Implement StringVarArray/StringVarArgs constructors.

//  StringVarArray::StringVarArray(Space& home, int n)
//    : VarArray<StringVar>(home,n) {
//    for (int i = size(); i--; )
//      x[i] = StringVar(home);
//  }

//  StringVarArray::StringVarArray(Space& home,int n,
//                           int lbMin,int lbMax,int ubMin,int ubMax,
//                           unsigned int minCard,
//                           unsigned int maxCard)
//    : VarArray<StringVar>(home,n) {
//    String::Limits::check(lbMin,"StringVarArray::StringVarArray");
//    String::Limits::check(lbMax,"StringVarArray::StringVarArray");
//    String::Limits::check(ubMin,"StringVarArray::StringVarArray");
//    String::Limits::check(ubMax,"StringVarArray::StringVarArray");
//    String::Limits::check(maxCard,"StringVarArray::StringVarArray");
//    unsigned int glbSize =
//      (lbMin <= lbMax ? static_cast<unsigned int>(lbMax-lbMin+1) : 0U);
//    unsigned int lubSize =
//      (ubMin <= ubMax ? static_cast<unsigned int>(ubMax-ubMin+1) : 0U);
//    if (minCard > maxCard || minCard > lubSize || maxCard < glbSize ||
//        lbMin < ubMin || lbMax > ubMax)
//      throw String::VariableEmptyDomain("StringVarArray::StringVarArray");
//    for (int i = size(); i--; )
//      x[i] = StringVar(home,lbMin,lbMax,ubMin,ubMax,minCard,maxCard);
//  }

//  StringVarArray::StringVarArray(Space& home,int n,
//                           const IntString& glb,int ubMin,int ubMax,
//                           unsigned int minCard,unsigned int maxCard)
//    : VarArray<StringVar>(home,n) {
//    String::Limits::check(glb,"StringVarArray::StringVarArray");
//    String::Limits::check(ubMin,"StringVarArray::StringVarArray");
//    String::Limits::check(ubMax,"StringVarArray::StringVarArray");
//    String::Limits::check(maxCard,"StringVarArray::StringVarArray");
//    IntStringRanges glbr(glb);
//    unsigned int glbSize = Iter::Ranges::size(glbr);
//    unsigned int lubSize =
//      (ubMin <= ubMax ? static_cast<unsigned int>(ubMax-ubMin+1) : 0U);
//    if (minCard > maxCard || minCard > lubSize || maxCard < glbSize ||
//        glb.min() < ubMin || glb.max() > ubMax)
//      throw String::VariableEmptyDomain("StringVarArray::StringVarArray");
//    for (int i = size(); i--; )
//      x[i] = StringVar(home,glb,ubMin,ubMax,minCard,maxCard);
//  }

//  StringVarArray::StringVarArray(Space& home,int n,
//                           int lbMin,int lbMax,const IntString& lub,
//                           unsigned int minCard,unsigned int maxCard)
//    : VarArray<StringVar>(home,n) {
//    String::Limits::check(lbMin,"StringVarArray::StringVarArray");
//    String::Limits::check(lbMax,"StringVarArray::StringVarArray");
//    String::Limits::check(lub,"StringVarArray::StringVarArray");
//    String::Limits::check(maxCard,"StringVarArray::StringVarArray");
//    Iter::Ranges::Singleton glbr(lbMin,lbMax);
//    IntStringRanges lubr(lub);
//    IntStringRanges lubr_s(lub);
//    unsigned int glbSize =
//      (lbMin <= lbMax ? static_cast<unsigned int>(lbMax-lbMin+1) : 0U);
//    unsigned int lubSize = Iter::Ranges::size(lubr_s);
//    if (minCard > maxCard || minCard > lubSize || maxCard < glbSize ||
//        !Iter::Ranges::subset(glbr,lubr))
//      throw String::VariableEmptyDomain("StringVarArray::StringVarArray");
//    for (int i = size(); i--; )
//      x[i] = StringVar(home,lbMin,lbMax,lub,minCard,maxCard);
//  }

//  StringVarArray::StringVarArray(Space& home,int n,
//                           const IntString& glb, const IntString& lub,
//                           unsigned int minCard, unsigned int maxCard)
//    : VarArray<StringVar>(home,n) {
//    String::Limits::check(glb,"StringVarArray::StringVarArray");
//    String::Limits::check(lub,"StringVarArray::StringVarArray");
//    String::Limits::check(maxCard,"StringVarArray::StringVarArray");
//    IntStringRanges glbr(glb);
//    IntStringRanges glbr_s(glb);
//    unsigned int glbSize = Iter::Ranges::size(glbr_s);
//    IntStringRanges lubr(lub);
//    IntStringRanges lubr_s(lub);
//    unsigned int lubSize = Iter::Ranges::size(lubr_s);
//    if (minCard > maxCard || minCard > lubSize || maxCard < glbSize ||
//        !Iter::Ranges::subset(glbr,lubr))
//      throw String::VariableEmptyDomain("StringVar");
//    for (int i = size(); i--; )
//      x[i] = StringVar(home,glb,lub,minCard,maxCard);
//  }

//  StringVarArgs::StringVarArgs(Space& home,int n,
//                         int lbMin,int lbMax,int ubMin,int ubMax,
//                         unsigned int minCard,
//                         unsigned int maxCard)
//    : VarArgArray<StringVar>(n) {
//    String::Limits::check(lbMin,"StringVarArgs::StringVarArgs");
//    String::Limits::check(lbMax,"StringVarArgs::StringVarArgs");
//    String::Limits::check(ubMin,"StringVarArgs::StringVarArgs");
//    String::Limits::check(ubMax,"StringVarArgs::StringVarArgs");
//    String::Limits::check(maxCard,"StringVarArgs::StringVarArgs");
//    unsigned int glbSize =
//      (lbMin <= lbMax ? static_cast<unsigned int>(lbMax-lbMin+1) : 0U);
//    unsigned int lubSize =
//      (ubMin <= ubMax ? static_cast<unsigned int>(ubMax-ubMin+1) : 0U);
//    if (minCard > maxCard || minCard > lubSize || maxCard < glbSize ||
//        lbMin < ubMin || lbMax > ubMax)
//      throw String::VariableEmptyDomain("StringVarArgs::StringVarArgs");
//    for (int i = size(); i--; )
//      a[i] = StringVar(home,lbMin,lbMax,ubMin,ubMax,minCard,maxCard);
//  }

//  StringVarArgs::StringVarArgs(Space& home,int n,
//                         const IntString& glb,int ubMin,int ubMax,
//                         unsigned int minCard,unsigned int maxCard)
//    : VarArgArray<StringVar>(n) {
//    String::Limits::check(glb,"StringVarArgs::StringVarArgs");
//    String::Limits::check(ubMin,"StringVarArgs::StringVarArgs");
//    String::Limits::check(ubMax,"StringVarArgs::StringVarArgs");
//    String::Limits::check(maxCard,"StringVarArgs::StringVarArgs");
//    IntStringRanges glbr(glb);
//    unsigned int glbSize = Iter::Ranges::size(glbr);
//    unsigned int lubSize =
//      (ubMin <= ubMax ? static_cast<unsigned int>(ubMax-ubMin+1) : 0U);
//    if (minCard > maxCard || minCard > lubSize || maxCard < glbSize ||
//        glb.min() < ubMin || glb.max() > ubMax)
//      throw String::VariableEmptyDomain("StringVarArgs::StringVarArgs");
//    for (int i = size(); i--; )
//      a[i] = StringVar(home,glb,ubMin,ubMax,minCard,maxCard);
//  }

//  StringVarArgs::StringVarArgs(Space& home,int n,
//                         int lbMin,int lbMax,const IntString& lub,
//                         unsigned int minCard,unsigned int maxCard)
//    : VarArgArray<StringVar>(n) {
//    String::Limits::check(lbMin,"StringVarArgs::StringVarArgs");
//    String::Limits::check(lbMax,"StringVarArgs::StringVarArgs");
//    String::Limits::check(lub,"StringVarArgs::StringVarArgs");
//    String::Limits::check(maxCard,"StringVarArgs::StringVarArgs");
//    Iter::Ranges::Singleton glbr(lbMin,lbMax);
//    IntStringRanges lubr(lub);
//    IntStringRanges lubr_s(lub);
//    unsigned int glbSize =
//      (lbMin <= lbMax ? static_cast<unsigned int>(lbMax-lbMin+1) : 0U);
//    unsigned int lubSize = Iter::Ranges::size(lubr_s);
//    if (minCard > maxCard || minCard > lubSize || maxCard < glbSize ||
//        !Iter::Ranges::subset(glbr,lubr))
//      throw String::VariableEmptyDomain("StringVarArgs::StringVarArgs");
//    for (int i = size(); i--; )
//      a[i] = StringVar(home,lbMin,lbMax,lub,minCard,maxCard);
//  }

//  StringVarArgs::StringVarArgs(Space& home,int n,
//                         const IntString& glb, const IntString& lub,
//                         unsigned int minCard, unsigned int maxCard)
//    : VarArgArray<StringVar>(n) {
//    String::Limits::check(glb,"StringVarArgs::StringVarArgs");
//    String::Limits::check(lub,"StringVarArgs::StringVarArgs");
//    String::Limits::check(maxCard,"StringVarArgs::StringVarArgs");
//    IntStringRanges glbr(glb);
//    IntStringRanges glbr_s(glb);
//    unsigned int glbSize = Iter::Ranges::size(glbr_s);
//    IntStringRanges lubr(lub);
//    IntStringRanges lubr_s(lub);
//    unsigned int lubSize = Iter::Ranges::size(lubr_s);
//    if (minCard > maxCard || minCard > lubSize || maxCard < glbSize ||
//        !Iter::Ranges::subset(glbr,lubr))
//      throw String::VariableEmptyDomain("StringVar");
//    for (int i = size(); i--; )
//      a[i] = StringVar(home,glb,lub,minCard,maxCard);
//  }

}

// STATISTICS: set-other
