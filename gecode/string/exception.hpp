namespace Gecode { namespace String {

  /**
   * \defgroup FuncThrowString String exceptions
   * \ingroup FuncThrow
   */

  /// %Exception: Value out of limits
  class GECODE_STRING_EXPORT OutOfLimits : public Exception {
  public:
    /// Initialize with location \a l
    OutOfLimits(const char* l);
  };

  //@{
  /// %Exception: %Variable created with empty domain
  class GECODE_STRING_EXPORT VariableEmptyDomain : public Exception {
  public:
    /// Initialize with location \a l
    VariableEmptyDomain(const char* l);
  };

  //TODO: Check if we really need all the following classes.

  /// %Exception: No arguments available in argument array
  class GECODE_STRING_EXPORT TooFewArguments : public Exception {
  public:
    /// Initialize with location \a l
    TooFewArguments(const char* l);
  };

  /// %Exception: Arguments are of different size
  class GECODE_STRING_EXPORT ArgumentSizeMismatch : public Exception  {
  public:
    /// Initialize with location \a l
    ArgumentSizeMismatch(const char* l);
  };

  /// %Exception: Unknown value or variable selection passed as argument
  class GECODE_STRING_EXPORT UnknownBranching : public Exception {
  public:
    /// Initialize with location \a l
    UnknownBranching(const char* l);
  };

  /// %Exception: Unknown relation type passed as argument
  class GECODE_STRING_EXPORT UnknownRelation : public Exception {
  public:
    /// Initialize with location \a l
    UnknownRelation(const char* l);
  };

  /// %Exception: Unknown operation type passed as argument
  class GECODE_STRING_EXPORT UnknownOperation : public Exception {
  public:
    /// Initialize with location \a l
    UnknownOperation(const char* l);
  };

  /// %Exception: Illegal operation passed as argument
  class GECODE_STRING_EXPORT IllegalOperation : public Exception  {
  public:
    /// Initialize with location \a l
    IllegalOperation(const char* l);
  };

  //@}

}}

// STATISTICS: string-other

