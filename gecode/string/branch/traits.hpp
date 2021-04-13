namespace Gecode {

  /// Traits of %StringVar for branching
  template<>
  class BranchTraits<StringVar> {
  public:
    /// Type for the branching filter function
    typedef StringBranchFilter Filter;
    /// Type for the branching merit function
    typedef StringBranchMerit Merit;
    /// Type for the branching value function
    typedef StringBranchVal Val;
    /// Return type of the branching value function
    typedef int ValType;
    /// Type for the branching commit function
    typedef StringBranchCommit Commit;
  };

}

// STATISTICS: set-branch
