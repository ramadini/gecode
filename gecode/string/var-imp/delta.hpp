namespace Gecode { namespace String {

forceinline
  StringDelta::StringDelta(void) : _changed(false) {}

  forceinline
  StringDelta::StringDelta(bool ch) : _changed(ch) {}

  forceinline bool
  StringDelta::any(void) const {
    return _changed;
  }

}}
