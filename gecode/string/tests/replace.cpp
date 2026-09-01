#include <gecode/string.hh>

#include <cassert>

using namespace Gecode;

class ReplaceModel : public Space {
public:
  StringVarArray result;

  ReplaceModel(void) : result(*this, 4) {
    StringVar source(*this, "ababa");
    StringVar query(*this, "ba");
    StringVar replacement(*this, "X");

    replace(*this, source, query, replacement, result[0]);
    replace_last(*this, source, query, replacement, result[1]);
    replace_all(*this, source, query, replacement, result[2]);
    replace_all(*this, StringVar(*this, "ab"), StringVar(*this, ""),
                StringVar(*this, "-"), result[3]);
  }

  ReplaceModel(ReplaceModel& other) : Space(other) {
    result.update(*this, other.result);
  }

  virtual Space* copy(void) {
    return new ReplaceModel(*this);
  }
};

int
main(void) {
  ReplaceModel* model = new ReplaceModel;
  assert(model->status() != SS_FAILED);
  assert(model->result[0].assigned() && model->result[0].val() == "aXba");
  assert(model->result[1].assigned() && model->result[1].val() == "abaX");
  assert(model->result[2].assigned() && model->result[2].val() == "aXX");
  assert(model->result[3].assigned() && model->result[3].val() == "-a-b-");
  delete model;
  return 0;
}