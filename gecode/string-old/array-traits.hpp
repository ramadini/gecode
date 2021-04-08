namespace Gecode {

  class StringVarArgs;
  class StringVarArray;

  /// Traits of %VarArgArray&lt;%StringVar>
  template<>
  class ArrayTraits<VarArgArray<StringVar> > {
  public:
    typedef StringVarArgs StorageType;
    typedef StringVar     ValueType;
    typedef StringVarArgs ArgsType;
  };

  /// Traits of %VarArray&lt;%StringVar>
  template<>
  class ArrayTraits<VarArray<StringVar> > {
  public:
    typedef StringVarArray  StorageType;
    typedef StringVar       ValueType;
    typedef StringVarArgs   ArgsType;
  };

  /// Traits of %StringVarArray
  template<>
  class ArrayTraits<StringVarArray> {
  public:
    typedef StringVarArray  StorageType;
    typedef StringVar       ValueType;
    typedef StringVarArgs   ArgsType;
  };
  /// Traits of %StringVarArgs
  template<>
  class ArrayTraits<StringVarArgs> {
  public:
    typedef StringVarArgs StorageType;
    typedef StringVar     ValueType;
    typedef StringVarArgs ArgsType;
  };

}
