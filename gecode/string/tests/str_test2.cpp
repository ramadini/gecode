#define DEBUG

#include <iostream>
#include <cassert>
#include <gecode/string.hh>

using namespace Gecode;
using namespace String;

class StrTest : public Space {

public:
  StrTest() {};
  virtual Space* copy() {
    return new StrTest();
  }

  NSBlocks
  ns_blocks(DashedString& x) const {
	int n = x.length();
    NSBlocks v(n);
    for (int i = 0; i < n; ++i)
      v[i] = NSBlock(x.at(i));
    return v;
  }

  int
  min_length(const NSBlocks& x) const {
    int l = 0;
	for (auto b: x)
      l += b.l;
    return l;
  }

  int
  max_length(const NSBlocks& x) const {
    long u = 0;
	  for (auto b: x)
      u += b.u;
    return min(DashedString::_MAX_STR_LENGTH, u);
  }

  void test01() {
    std::cerr << "\n*** Test 01 ***" << std::endl;
    NSBlocks v = NSBlocks(1, NSBlock(NSIntSet('a', 'b'), 0, 4));
    DashedString x(*this, v, 0, 4);
    v.clear();
    v = NSBlocks({
      NSBlock(NSIntSet('a', 'b'), 0, 2),
      NSBlock(NSIntSet('a', 'a'), 1, 1),
      NSBlock(NSIntSet('a', 'a'), 1, 1),
      NSBlock(NSIntSet('a', 'a'), 1, 1)
    });
    DashedString y(*this, v, 3, 5);
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert (sweep_equate(*this, x, y) && sweep_equate(*this, x, y));
    std::cerr << "===== After Equate =====" << std::endl;
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    v[0].u = 1;
    assert (ns_blocks(y) == v && v == ns_blocks(x));
  }

  void test02() {
    std::cerr << "\n*** Test 02 ***" << std::endl;
    int n = 10;
    NSBlocks vx({
      NSBlock(NSIntSet('a', 'c'), 0, 30 * n),
      NSBlock(NSIntSet('d', 'd'), 5 * n, 5 * n),
      NSBlock(NSIntSet('c', 'f'), 0, 2 * n)
    });
    NSBlocks vy({
      NSBlock(NSIntSet('b', 'd'), 26 * n, 26 * n),
      NSBlock(NSIntSet('f', 'f'), n, n)
    });
    DashedString x(*this, vx, min_length(vx), max_length(vx));
    DashedString y(*this, vy, min_length(vy), max_length(vy));
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert (sweep_equate(*this, x, y));
    std::cerr << "===== After Equate =====" << std::endl;
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    vx.clear();
    vx.push_back(NSBlock(NSIntSet('b', 'c'), 20 * n, 21 * n));
    vx.push_back(NSBlock(NSIntSet('d', 'd'), 5 * n, 5 * n));
    vx.push_back(NSBlock(NSIntSet('c', 'd'), 0, n));
    vx.push_back(NSBlock(NSIntSet('f', 'f'), n, n));
    assert (x == DashedString(*this, vx, x.min_length(), x.max_length()) 
         && y == DashedString(*this, vy, y.min_length(), y.max_length()));
  }

  void test03() {
    std::cerr << "\n*** Test 03 ***" << std::endl;
    NSBlocks vx({
      NSBlock(NSIntSet('a', 'c'), 0, 300),
      NSBlock(NSIntSet('a', 'b'), 1, 1)
    });
    NSBlocks vy({
      NSBlock(NSIntSet('a', 'c'), 0, 300),
      NSBlock(NSIntSet('b', 'c'), 0, 200)
    });
    DashedString x(*this, vx, min_length(vx), max_length(vx));
    DashedString y(*this, vy, min_length(vy), max_length(vy));
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert (sweep_equate(*this, x, y));
    std::cerr << "===== After Equate =====" << std::endl;
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert (x == DashedString(*this, vx, x.min_length(), x.max_length()) 
         && y == DashedString(*this, vy, y.min_length(), y.max_length()));
  }

  void test04() {
    std::cerr << "\n*** Test 04 ***" << std::endl;
    NSBlocks vx({
      NSBlock(NSIntSet('a', 'b'), 1, 1),
      NSBlock(NSIntSet('a', 'b'), 1, 1),
      NSBlock(NSIntSet('c', 'd'), 1, 1),
    });
    NSBlocks vy({
      NSBlock(NSIntSet('a', 'd'), 0, 2),
      NSBlock(NSIntSet('d', 'd'), 1, 1),
    });
    DashedString x(*this, vx, min_length(vx), max_length(vx));
    DashedString y(*this, vy, min_length(vy), max_length(vy));
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert (sweep_equate(*this, x, y));
    std::cerr << "===== After Equate =====" << std::endl;
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    vx[1].S = NSIntSet('d', 'd');
    assert (x == y && y == DashedString(*this, vx, x.min_length(), x.max_length()));
  }

  void test05() {
    std::cerr << "\n*** Test 05 ***" << std::endl;
    NSBlocks vx({
      NSBlock(NSIntSet('a', 'b'), 1, 1),
      NSBlock(NSIntSet('e', 'f'), 1, 1),
      NSBlock(NSIntSet('c', 'd'), 1, 1),
    });
    NSBlocks vy({
      NSBlock(NSIntSet('a', 'd'), 0, 2),
      NSBlock(NSIntSet('d', 'd'), 1, 1),
    });
    DashedString x(*this, vx, min_length(vx), max_length(vx));
    DashedString y(*this, vy, min_length(vy), max_length(vy));
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert (!sweep_equate(*this, x, y) && !sweep_equate(*this, y, x));
  }

  void test06() {
    std::cerr << "\n*** Test 06 ***" << std::endl;
    NSBlocks vx({
      NSBlock(NSIntSet('x', 'z'), 1, 1),
      NSBlock(NSIntSet('a', 'b'), 0, 2),
      NSBlock(NSIntSet('y', 'y'), 1, 1),
    });
    NSBlocks vy({
      NSBlock(NSIntSet('x', 'z'), 1, 1),
      NSBlock(NSIntSet('a', 'c'), 0, 1),
      NSBlock(NSIntSet('b', 'c'), 1, 1),
      NSBlock(NSIntSet('x', 'z'), 1, 1)
    });
    DashedString x(*this, vx, min_length(vx), max_length(vx));
    DashedString y(*this, vy, min_length(vy), max_length(vy));
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert (sweep_equate(*this, x, y));
    std::cerr << "===== After Equate =====" << std::endl;
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    vy[1].S = NSIntSet('a', 'b');
    vy[2].S = NSIntSet('b', 'b');
    vy[3].S = NSIntSet('y', 'y');
    assert (x == y && y == DashedString(*this, vy, y.min_length(), y.max_length()));
  }

  void test07() {
    std::cerr << "\n*** Test 07 ***" << std::endl;
    NSBlocks vx({
      NSBlock(NSIntSet('a', 'b'), 0, 2),
      NSBlock(NSIntSet('a', 'd'), 1, 1),
      NSBlock(NSIntSet('f', 'h'), 0, 2),
    });
    NSBlocks vy({
      NSBlock(NSIntSet('a', 'b'), 1, 1),
      NSBlock(NSIntSet('c', 'd'), 1, 1),
    });
    DashedString x(*this, vx, min_length(vx), max_length(vx));
    DashedString y(*this, vy, min_length(vy), max_length(vy));
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert (sweep_equate(*this, x, y));
    std::cerr << "===== After Equate =====" << std::endl;
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert (x == y && y == DashedString(*this, vy, y.min_length(), y.max_length()));
  }

  void test08() {
    std::cerr << "\n*** Test 08 ***" << std::endl;
    NSBlocks vx({
      NSBlock(NSIntSet('a', 'd'), 0, 2),
      NSBlock(NSIntSet('c', 'd'), 1, 1),
    });
    NSBlocks vy({
      NSBlock(NSIntSet('b', 'c'), 1, 1),
      NSBlock(NSIntSet('a', 'd'), 0, 2),
    });
    DashedString x(*this, vx, min_length(vx), max_length(vx));
    DashedString y(*this, vy, min_length(vy), max_length(vy));
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert (sweep_equate(*this, x, y));
    std::cerr << "===== After Equate =====" << std::endl;
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert (ns_blocks(x) == vx && ns_blocks(y) == vy);
  }

  void test09() {
    std::cerr << "\n*** Test 09 ***" << std::endl;
    NSBlocks vx({
      NSBlock(NSIntSet('a', 'c'), 0, 500),
      NSBlock(NSIntSet('d', 'e'), 0, 300),
    });
    NSBlocks vy({
      NSBlock(NSIntSet('a', 'b'), 0, 200),
      NSBlock(NSIntSet('c', 'd'), 0, 900),
    });
    DashedString x(*this, vx, min_length(vx), max_length(vx));
    DashedString y(*this, vy, min_length(vy), max_length(vy));
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert (sweep_equate(*this, x, y));
    std::cerr << "===== After Equate =====" << std::endl;
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    vx[1].S.remove('e');
    vy[1].u = 800;
    assert(ns_blocks(x) == vx && ns_blocks(y) == vy);
  }

  void test10() {
    std::cerr << "\n*** Test 10 ***" << std::endl;
    NSIntSet s('a', 'a');
    s.include(NSIntSet('d', 'e'));
    NSBlocks vx({
      NSBlock(NSIntSet('a', 'c'), 0, 400),
      NSBlock(NSIntSet('b', 'c'), 3, 3),
      NSBlock(NSIntSet('b', 'c'), 0, 97),
      NSBlock(s, 0, 300),
    });
    s.clear();
    s.include(NSIntSet('a', 'b'));
    s.include(NSIntSet('d', 'd'));
    NSBlocks vy({
      NSBlock(s, 0, 200),
      NSBlock(NSIntSet('c', 'd'), 0, 900),
    });
    DashedString x(*this, vx, min_length(vx), max_length(vx));
    DashedString y(*this, vy, min_length(vy), max_length(vy));
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert (sweep_equate(*this, x, y));
    std::cerr << "===== After Equate =====" << std::endl;
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    vx[2].S.remove('e');
    vy[1].u = 800;
    assert(ns_blocks(x) == vx && ns_blocks(y) == vy);
  }

  void test11() {
    std::cerr << "\n*** Test 11 ***" << std::endl;
    NSBlocks vx({
      NSBlock(NSIntSet('a', 'c'), 0, 400),
    });
    NSBlocks vy({
      NSBlock(NSIntSet('d', 'd'), 0, 900),
    });
    DashedString x(*this, vx, min_length(vx), max_length(vx));
    DashedString y(*this, vy, min_length(vy), max_length(vy));
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert (sweep_equate(*this, x, y));
    std::cerr << "===== After Equate =====" << std::endl;
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert (x == y);
    std::cerr << y.max_length() << "\n";
    assert(true && y.null());
  }

  void test12() {
    std::cerr << "\n*** Test 12 ***" << std::endl;
    NSBlocks vx({
      NSBlock(NSIntSet('b', 'b'), 0, 100000),
      NSBlock(NSIntSet('c', 'd'), 0, 100000),
    });
    NSBlocks vy({
      NSBlock(NSIntSet('b', 'c'), 0, 2000),
    });
    DashedString x(*this, vx, min_length(vx), max_length(vx));
    DashedString y(*this, vy, min_length(vy), max_length(vy));
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert (sweep_equate(*this, x, y));
    std::cerr << "===== After Equate =====" << std::endl;
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    NSBlocks vz({
      NSBlock(NSIntSet('b', 'b'), 0, 2000),
      NSBlock(NSIntSet('c', 'c'), 0, 2000),
    });
    x.max_length(2000);
    assert(x == DashedString(*this, vz, 0, 2000));
  }


  void test13() {
    std::cerr << "\n*** Test 13 ***" << std::endl;
    int M = DashedString::_MAX_STR_LENGTH;
    NSBlocks vx({
      NSBlock(NSIntSet('a', 'b'), 0, M),
    });
    NSBlocks vy({
      NSBlock(NSIntSet('a', 'c'), 0, M),
      NSBlock(NSIntSet('b', 'b'), 1, 1),
    });
    DashedString x(*this, vx, min_length(vx), max_length(vx));
    DashedString y(*this, vy, min_length(vy), max_length(vy));
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert (sweep_equate(*this, x, y) && sweep_equate(*this, y, x));
    std::cerr << "===== After Equate =====" << std::endl;
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    NSBlocks vz({
      NSBlock(NSIntSet('a', 'b'), 0, M - 1),
      NSBlock(NSIntSet('b', 'b'), 1, 1),
    });
    std::cerr << x.min_length() << ' ' << x.max_length() << '\n';
    std::cerr << y.min_length() << ' ' << y.max_length() << '\n';
    assert(x == y && y == DashedString(*this, vz, 0, M));
  }

  void test14() {
    std::cerr << "\n*** Test 14 ***" << std::endl;
    NSBlocks vx({
      NSBlock(NSIntSet('a', 'b'), 0, 6),
      NSBlock(NSIntSet('c', 'c'), 1, 1),
      NSBlock(NSIntSet('d', 'd'), 1, 1),
    });
    NSBlocks vy({
      NSBlock(NSIntSet('a', 'c'), 0, 5),
      NSBlock(NSIntSet('d', 'd'), 0, 1),
    });
    DashedString x(*this, vx, min_length(vx), max_length(vx));
    DashedString y(*this, vy, min_length(vy), max_length(vy));
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert (sweep_equate(*this, x, y));
    std::cerr << "===== After Equate =====" << std::endl;
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    NSBlocks vz({
      NSBlock(NSIntSet('a', 'b'), 0, 4),
      NSBlock(NSIntSet('c', 'c'), 1, 1),
      NSBlock(NSIntSet('d', 'd'), 1, 1),
    });
    assert(x == y && y == DashedString(*this, vz, 0, 10000));
  }

  void test15() {
    std::cerr << "\n*** Test 15 ***" << std::endl;
    NSBlocks vx({
      NSBlock(NSIntSet('a', 'a'), 0, 6),
    });
    NSBlocks vy({
      NSBlock(NSIntSet('a', 'a'), 0, 5),
      NSBlock(NSIntSet('a', 'a'), 1, 1),
      NSBlock(NSIntSet('a', 'a'), 0, 5),
    });
    DashedString x(*this, vx, min_length(vx), max_length(vx));
    DashedString y(*this, vy, min_length(vy), max_length(vy));
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert (sweep_equate(*this, x, y));
    std::cerr << "===== After Equate =====" << std::endl;
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    NSBlocks vz(1, NSBlock(NSIntSet('a', 'a'), 1, 6));
    assert(x == y && y == DashedString(*this, vz, 0, 10000));
  }

  void test16() {
    std::cerr << "\n*** Test 16 ***" << std::endl;
    NSBlocks vx({
      NSBlock(NSIntSet('a', 'c'), 0, 3),
      NSBlock(NSIntSet('d', 'd'), 1, 1),
      NSBlock(NSIntSet('c', 'f'), 0, 2),
    });
    NSBlocks vy({
      NSBlock(NSIntSet('b', 'd'), 0, 3),
      NSBlock(NSIntSet('f', 'f'), 1, 1)
    });
    DashedString x(*this, vx, min_length(vx), max_length(vx));
    DashedString y(*this, vy, min_length(vy), max_length(vy));
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert (sweep_equate(*this, x, y));
    std::cerr << "===== After Equate =====" << std::endl;
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    NSBlocks vz({
      NSBlock(NSIntSet('b', 'c'), 0, 2),
      NSBlock(NSIntSet('d', 'd'), 1, 1),
      NSBlock(NSIntSet('c', 'd'), 0, 1),
      NSBlock(NSIntSet('f', 'f'), 1, 1)
    });
    vy[0].l = 1;
    vz[2].S.remove('e');
    assert (x == DashedString(*this, vz, min_length(vz), max_length(vz))
    		&& ns_blocks(y) == vy);
  }

  int M = DashedString::_MAX_STR_LENGTH;
  int L = DashedString::_MAX_STR_ALPHA;

  void test17() {
    std::cerr << "\n*** Test 17 ***" << std::endl;
    NSBlocks vy({
      NSBlock(NSIntSet(0, L), 1, M),
      NSBlock(NSIntSet(0, L - 1), 0, M),
    });
    DashedString x(*this, "0=1");
    DashedString y(*this, vy, min_length(vy), max_length(vy));
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert (sweep_equate(*this, x, y));
    std::cerr << "===== After Equate =====" << std::endl;
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert(y.blocks().front().known() && y.length() == 2);
  }

  void test18() {
    std::cerr << "\n*** Test 18 ***" << std::endl;
    NSBlocks vy({
      NSBlock(NSIntSet(0, L), 0, M),
      NSBlock(NSIntSet(0, L - 1), 1, M),
    });
    DashedString x(*this, "Zz1xJr2yi3kD0njK4mjOWu5HaEnNDg9Ha7o8AniRkfU66m8EpyDsD5yYEF4PAtfcK1fI2aO0xPMfA1gtXhV685G9bVD6MX7urD0Uxq5P2lGERM6iqQYjpgZhuRMNDUCccQHMcnGvUofrvJskrn2vbrKFwvPaNcKlLnqQql7Ut39SxWLnS0kcASqfnDKMLiQLTvXOkcZ0 =09GXSOVfpYV3pELJMYcqwPm5H1O0IQXnE8o3KvItPWQFQopN");
    DashedString y(*this, vy, min_length(vy), max_length(vy));
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert (sweep_equate(*this, x, y));
    std::cerr << "===== After Equate =====" << std::endl;
    std::cerr << "y = "  << y << std::endl;
    assert(y.blocks().back().known() && y.length() == 2);
  }

  void test19() {
    std::cerr << "\n*** Test 19 ***" << std::endl;
    NSBlocks vy({
      NSBlock(NSIntSet(0, L), 1, M),
      NSBlock(NSIntSet(0, L - 1), 0, M),
    });
    DashedString x(*this, "67FDM6EUFULST0LM1I7YJPE4XSX5DIEQ53DZM3RTZNUF7EZA9XIENKHJR4N5RZZ5KE21ZM0PIQJTPUPRT7LVV58TIAY3GKZB43IJBH1HPW2789PA0YPVER4LQX4SAOJB6FPDBMNLLHPZMY6MP9MW1FYU5GWT7B9HVFTQ1XEX4BAV56R90FHEOM1CZS5QABFLFKGQR2ZFZVRIEX7MFTCAQD8EJPEIZ95BY7KG8R3HDPMEGUY7FMJGH3GK8V3HAORH736CQPHHAIJ8YXUE4P2IHCO5GVQW44AGSW8MLEM79XOI1VAO6AA1W7KMR450ZERP0TM0KLM22ICGRLQ3SUAHX1MJ5C4V79SNAAQVPVZZIADR4TZ80LASYA3HE5I9YZFE4ABX4IRHIA6L6BA29X7RSK3OX3QZTNZ0R8GNX98OB5JU5CY2QEX54K8C7F7ID8RYNBBNSHNWU9EMI4AT6KMWWP3JHO3AHGBM7SHS2ZQ9S1GQD85YFJD9OLE7GNJLE0GXB1Y8BP3QTNI1PHTU44Z14PAAOGZMDIMR9HXYAEAVZCYJOO03DRTNXNYAKON3866EB06T0JRU18YYI2TEC5SDMNV9F5NZFL606WGMISF9GFAIGE5B4CBO6M6C0N13J6R3UEDFFALU23R74Q8TYJMUSWGBB8C7K8X28V444X5Y4WUKTI90MWDAROF3FR4WDPOC8JGDUIXE5GWWHH3QFMAQJ1MZTVMP4I3Y5UC92JAF0QUNTYB42LV441I2EINWRLS3RSAO2M67VU26KD7OLWYP4HTNVRW6WH5NNQJGCX9TW6366U957VGN1402ITCBELNSXYSLT22VKZ7MZ1KVB2YIEYSICWCKP4EEISS6ECBYYUHTPUO9PAQ8OCBFWGLIQ8P2BQUFAUT3ZWXX16DZBR9MB6QVSLUYTQ37PNTLI03W9O9L2VLJDQO7XX0RISMFBX5EIML599ATW8877SQYJI42LI2M6CO5SVNK5ZXQAYRY6HV26DGSPA309VSS9QMUMOZLRUYWTZ0M26YYIPXL5Y96258XE6XQEN4SYXVOMNVBNC0R61SDK28PNEC5XYWKSCF52FW7QFE2B5IYUJB9T2KPLNTA777UZ279QXNB9VTN79AH3H4J126QLJYXDIXDQTM9TFF8TTFKGCN61JLRP0O6GV0CJIHF13MUG8O3F91KR8FA65X3QTW2EEKQI3GMNVCWH3NX48HOI2RUU9C2LKUK81EGG022C55AIZDF09BQ44K8YNRNILR310SHJIK57IN6MV3J0XA0I3FC5VGYYWPEWHW1M34CGTDTYMAGO5P9ZV4C2ZI9C4VJTR0IR4AYMEWGXBSN411MQKFMWHEQF51PBBQCBL33C9Z3AOZ6CRDGFFP8O128G1SYC01J5J9KZRD8KERAXRR8BIY4BEY0BEWR355O3FEFYC92B47ITJRJNHWSG9DVUP1FLVD8ZCI93Z03GNZ39MURV2PNL60XQ8GCI27KK2TQIVW4WODKM91PRW5YU5N3TB2R51HN8QZR98IJTXY2BJF8ACTZQ6N1978DYKDZ2NEBFEUDKB2G2KEUZWI61AMJO23U6KO1FH4B9GM6KBV9HSTPR7Z2L6Y3TUHOVC01XHNKTS1D6OEBKUHVYIBQSL20VRURSH7PR1VP1PPBS6KTUCW0ZUQ5KZQWBCYNUULPJP6J0OSY4YYZKKFYZ0Z955SSHWZEZA7DZP74TVHMHFTM2126ZDPT0D3JM5VCMN0XUV5TAUR6C6A7MOKD6LGD7MASM4TAQIUELJSVIAQ1MC3OPFOTJJQN3J948T3SQKF5P0LJUR17186C7Q9VFGT8VBF1AJ309OFC2LQIDGA4YO2RPFO0A2MU6NL59NEU1EJ3O3Q5ZWRP8M4NRZFC657ZNBASQ0JK7KREMU5E2X2KYG7E39UN9EYA4CQA0GIEEINN7H8ZQG05L4NB59168B82PNJE3I0X39YGEO4OKATT180LXYVQFGPLYO8PJR1WXOZI6OAX6KPAR40NOSD40UI45LRYPDKPCAT9GQGQYUVU2SO2TVQ6WUNC5VE4YNJBTXINLKV4NB2R2BQJVWP6QGSG7XPGM26ODA8Y431RYZ89A0BQPKKHRFKLN1JJZ4UU9B3X0KMQI20LC4PTVTQYNSNNYKTUY8PZFPM3JBJL7JQ27QDTHVQHZH7ZZ6XSZO4L06Z2QXDD12P54UY61OLI5BMLOBU2HIPSU76LFLY58WEL9YLA2NDVTPAJZ67TTJVDK299S7WB1MWMBS4KQFJN5KZHPGB1KO4AVCYME1FARK3GBPZBFLDJSYGJ0AWKDY2253SV52SNCU2JI28B5DMUKECHBTUM2VLQT7X43BHMH40Q4A7BK56CZSHNH3UKX7ZRGRFM1PKWXNBVC4OQJC9YXARFPLN6MFMDOG0ZYYTR3RGIZGPNJSUBFCN2TAAPCVT09DVV37N56FYAXZE4ZPCPHXTSUOFN2FNS5R8YE4Y186QRNHV7EAOFCDMRNQ1X9XMFI39LISL45MHAJSNO93SSD9U9PEIBR0U0J78VDH5040TP3PWERJZSPD3X9XFJCGVRLUAOHV1L3C1XP34NAJT8GSCQJR1K4R4H09AXFMXU1CA4NLPCXFQW5CIK3J4HAN5Y0L679UWHTQAKN70TH6OMY26GUP5R0Q86UQ2CEBAP4OGWNUX5E3UI6SXJ73IYMXTJLFSF73RVWKLKR09D5FLSXYTQK6J6R4J48RQ5CH902KD5K7A56QE32PRUAY1AZUNZ3U4MK4V10AXRCHW7H7AO9XHM8U78KK0YQOK0ANRVSI15VWWSND5Q7IM4YY2O49C8ELTINHRBC9QOLT91B410C0IX7ZAM019T4VDV4NY1UZFUNLC20LW8GV2O9504TSEBLJBZIGFVRM4LSLK3QQWUGOWOHW0X89XHWH6SUAD3464BE2JZYLL2ZTAHGTOL90AF92HUR0BLAWVV3EV81FEGD0JDNIQB84K7VC3CRP2UFLRI50CZDC43DC934P21IJN4FQC5T2WLJ1EYE9APTPCK2KI7SXX484V2AGLALCAQZCTQD63ZW7FZNECXRC37G4Y9VRA9X9C7CQPZYSL7H1HWRTWRWMRDYDUWOQCDB6O3UZ4R32TYYWVOTV7AJIAU18NL3Z6H4TSAVZ9FRHZN0LGT0F68DB2S92C026VBESMVNPVTAYLGNC85X52YT4A8V5DXSFJHNNEPG08ELJI3CLFSDCSHT4YV7HLT7YFTYD8KQEHCUY98XMADV80ZSJPF70FU5K4QYHX9AUYXIEIZEJSGY1ZXA333XKKGSBKTPZW6LDT9WY87LVF1N0MAUVOO0K7VJYTI2OEX93R6JP3EOIDAEYXZ7ZBL5TC1MRUS8GRU4BT17K7K6KUYAF8J0YQDNTQ9JOG007UW3XFY5ZM6T2DMDM5K498VOX4RVGVMSXYSGRTQUKDAPSO9V9GNWWPX2YP9NKU0OCMDP9LDAP8M200LQWBBHUVC3FQ5SNUPGTW4Y1RHYU3TS5UAJ89H94YTVSW5ZYQ72PAE3ZMYC831DJ1IYJOG0JTCINONWNBR4LNBIDC9Z6NQXXYQIFFX3AR3QOMRWKOSE122EHF7RXQT7DDG9RQFZ3YVUPQVECI47LSO6ZM17SZVJA6ZWI47O4C345CDQ6RUQYU8SY3490XU2OA9U3DPD1NF5SYIG1L439J46ISAFY6WDXHEQRZ6J39HWVVJNWAMNU7JJD27VNZ8P93CF8X2IA9Z59J8K792I5QA67IAJAHNTFK94N2AIAVH7GSJEBP47VLKISCNI6SD11UNRVP4OAB55APC2670PXMQ15ASLC8OF1ZBCGDW10W5UAP72J10WPYJZTEQAPX5EDI7GSDTOECN6S9Z7LNPWG9E06IO3FN3BYFLFRWPBDQ3PRGYJ3L00XEAFZKM4DIM37LCWMY1F3YWED0EQ4L2D5759U4SGGCJ0BWWM9S7X1FNGWT90RD2P9G0VSB2PGCLKUPZ2WUXU7JHRMV7MAVQBX000TENKKTQZJZW1XVDL48MSF5QX8APKDEJ19CDRB9VZH5E79B17MZZ78T4K8FM6WFWWKOYO61DHYKSHE4VLUOMOVJDBI080K8LKQ8DANKHHCKGBAX0YLWKUZS4JTEGS124VR89VNY83631LI584ALHXGF0EMVDPVOKK3XTIO1ZIDO0UNHT0TJX7BOVPC72I4N5NWKLD8Z4OU9QJR9K85WRCF3PM3HUFVQCZ0I2A2IIYMRJ58F7F5A9OGCI2577OL0H5Z9INTQ84ML7FNTC5BJ2RDXN1J63WDLVJAS2I8WPGNTLGDS6VSZOX73B2OLZ2L92GELMY5VRH6Q5FOU7BIL6M72ITH68CAHBZBS3OJIQBNRSIX7HUI3X6A0TXFCO4LHX3246ABW388UP2KHJ2DWVIZEWUK5CYD85ZLONI7CYTL67UCXZLN1O3EZBZTBFM2BMCUYHLLDE9O50MSYE94J632J8WD6NU60NI9JADASMKXDI4A4NTOWK69VPQK64C23NQJ8V8JGFQOY2QXT104YR882FQWLX8EJO6B2HBHVMJCEVQJQ0DQBN9J84NLG575HCPUJ9R21VULFGL8VNKNP48UB84EWGIK9Q1GH3XFFX4HFQ0XPVR66F9HIP7AN932I9EVRD5XRDK4ITVXC4VCDVJTARDU2CYWZWKU8SI0U6A248UL4NJAWLV865M1QSKPH0MQTRMB125I78FP2V5TPLFWV1CW6MH8EMQ59RX3SV3ECOTFB17UNCC85WO3EKNG9Y057K7188XJOZ2HGQOTNJREY6HUA2TD7PBL5JS69N2X3JWWWCTL3TVPRCS41YKG79ONZSJKQ73PO54RS57O6CUEIQRG5PM5REX1F35R99IFY6T4UQSC6XH8GESU46DVHF6ZKM2FLAMR6B5WJAT4E65GLLZN7BEQ6K60N5FQOEZXUAZ30EACOY8H05TRYN9MC1F2LHK3H4NW1AMKA3JVFJX6DNSSV758XTZ5EJOCBK99IU9R018GIE54IKETF3RR4YFUI3UG1O3ONDFFKW5VDEX1A8E5LF3H8EUDKX9F0CTK6NSG8T1AFZWQB2CC0MQCHT8CJ8P4DNT44D5BAF6RQOYHS5UDA98C0MEGSLD8QNQDE80K810OK26D71RVLP3G36FVT6JXWOPLV6ZAY4PI9AIZIZ0DPJ2CNSX9XLQXIAMPT0DP99GP727TX0C8P01NTDXGGLLEDNH4G1Q7HFP6Z8V6PVQR8JX43FNIKRRMWV21K11VZDI73PALEZ6C59J8JHFTDM1QBJ79AJDMRK85HAWWRITOVEXWQ7GDMFJ1GWBOJ7XFG81E6TI1IKY84J8DCBJW2QB3PG4891PO9X3AMIFV9GMBWJ2TDM7SHOCFPQJNYSG07E6IWCCKI4S4UJ3UEKYR11DSHOI1QRS5BVIMF0ZNWD99VA06IH9B3ZFRN0HRYBO6MVJGFTTB8NHMPFPXIADB6IMAR5RYSUKV1FWYRXX3T3I51RTVTU1U78CG010W9URC6A8NACLSY2RU89ZB4K15YCQ2W3S82H6ZKMSS56WODFNZW4LXS250I2LBZXH2IC482W7JODN72X09BEPANYDKZATZRUIL83Z00DP4U1893U2EK3529BHKF1EKQQ3FSLCOSGV9XE78XXBIEUSZ8LUL645RE7X5SY2FOHKKB766BI31YXTLU0DWPPO11PMID7D00SMVJJ2G9M20DR7STFPMXPH84Q0BKTFUFMJVOVDWX1L1IY979BQS8TQK22S7VO9KXE8JQZ7YBMZIYMHYZ7RTFIUDAPSIMJHFDYG812PNA3PH66RPPSMBU1IGL959FO9PJX95JTXNBSYTYTECI2DKOYS0FUQYCY0INUM1UN343NLVFCVPMQUIBNGYX3T19O14QIB8J2HPTNJFBSLMSIOMVM6JMUWWUKPLK6433WOP3T4G9OQVX9DGZ56R4PP5K90UM3WI8QRFMDKS7BZ2X87FQGPVFJS6CRD4CVTJHSUVSF769Y2IJ6JE6GXK2MZ2EB20Z11WMTPOTK8OZDS9B1P69BCDJUOUSZRP4JN2NHD9TY8U9AJT07M515NX5DLJR91C7V0GSJG6T3FVKT0I9K1TG1GQ9EM4GD32SYVZMH4VIG5Z0WUMSY2OJ7GOQF25SUF7WHJXEFGX3O71W9WKZDDIMK8BAZCT1YPTNYHA1CEZ2STJYEAO1NGHRD89EDR0CPRBJVPFMBL3LTM0OUYSDE6FSURRHVBYZ5TXECMY7XKAUK5PYDKAVL1UQI95QUDNT2Z03EY8AUACFDB7SV5E8UPKA0AASBCIDWV16AETPB5C1L9VGI1OAH7Q982B2T8AMZ6NX4TOKA63PC5G6NI273H27POSQKW5L7W0QWDRMO7BD28RHSKI71LYR6TSGPVDUVLL2Z7WDMQ4SPPXY0X1LDZ5NFI0Z5UDOO6BSMI03EA5JYPGW7IVZ0TRI4RAEZC5LPUC6ZQNO5TIHGUXO8VWO3PFEBLL2CVIVR0YJMAFMAP61M4FR7KHSQ2I8EI7FJDKGJK2Y398WX2OJOL4IH38SQRJ2Y3MYKJVM89F4B4MZR6HHLAHJFN5GMONTE3E2MQITQVZRTJC0BXH0C7YJK6SW067NMBTEQ1ZRMC8RYODVQR52460CBDX2I7TPAVJZZVD8R51SXIHY74VDXJJE1CHVW78JQT6SWMCXYNHSV5DDAF2UGOIVW2Y185UDGCY5216YH3UFUGGA75WKG0375GLVOHJB03BOWLRLJ8T9JOXAGO9SSYERGMB71BH37DADO4WLBF40OCE4X4T3IY0YATEIZDH6ADCV1NXMZ2YSEX18L9TX2QFKK65RWZ5U5AIT29F45UTTK51CDY349K5J9YN1FNKTK8AJ8WLEX21M09BRCCO0Q0Y17P3F4QK26LVBV6SYAHE43NOI8NVW8HV1UKHZMN19M0B2UUI958YWBPEV1WOIN23FBZXFUQ2BIV98MG2IXODWE5MXMLVMM0TMHK0OGSZX7JM2KA2NPUH3KCKIOGKWB72KELT9BLF4MXZZVAA8M0HMG9Y34BE00TV44O0QPOUOO0O6487IPCFZE69WNNLZ46YOUGW407J3WG4B9= 9GMB4QFR8G45AH7H6BLUMSV11PRRJ97GD9VF2MQWBC7KSPPZB01OJPPAPYOOC6QC31BM18OOVV1ODYMYETHJI00R3S36F7TBB8BCA7J2VAK2AJ9F2VEX7RSXGZKLNX0BN3VUNV4MOVUBCWP9CT5XCN24634WLLFD25F8ZTNQ2NRF39MGISH5QLWZH3PIQLEHG1B2YMEAA27H5LVGG19YHX0USWXB8035Y47BVNZGXPQL7S8PACUNKT1MJ28XM9UFF7Q5UZE63N4INPFA5MZN9QBE5BAWYPLJCRIILZ66VN08ABL7CLFFGZDA3LSD2QSAQTF9DL15ARMAZ1XEO8ACFP2SKG77PQU0QTYKHK1327MQH0VAV5V4IWN7EF03OM6UYY404OJ8QHX8RN3CBHSDP9NZMSGEMF18AZ0Y3ZIRKK5EPSH0LLO3KQV6ALHRSSVQ8HFOR0PYHHMEH9N1DOW2YQEW9S750PDLVLL6D8RGYHU27HAZ286JZ2IIPTXW5QVEDP7V0IC9HAN610GEKYS7A0T9AB6GAX3P0CKVJWY21TMFIHBR1TV7G4UUH9V26WUZNS5LD6URK797260X90OZMU6KAZO4S0K6EBOZUYWOAWZROOX0QNEIOZVT82H0KRUSWNCMXPS5RMGEKK2FH3IKB8BNMTV100Y4NHL6T6CUX5CRM3D5UV185FNAMG8MKEPG1D2KSAVU0UBJDPM65YDDBO5LPV8V7JLF25B1NH9F9RGI4H46C8FXUHMDM0GML3CYABRS8IJPON0QM0AFCZR2GLDNPN3T19EBE5SOKD4MXDHY955K643PUWHF9RLSENURLFQ5NI7C652QC4X664NEI4WX5J3CONDCY3LAL1W98I21UJSM2F7SELR1ACWU0DWHNWFPO4RFCHMTFZK5L0P1RIKFLBQ5RLC1SGGUCTTVYRQP4J5PCSHTOTKNI8C4SNTIFMG2S2P2TPRZ8OJE0RK0SDPGE0F5M310BY4KLO6D6R9ZMKKCWJ9OY8FBBRN0TNTP4WWKFYKG8EGQCIBJAQH6SD32EEY43VSK0YRZKMZTDSBG4TMSYNRW7B89WWZTRBM44UHAWSXV1YXD6WRV511URRWRLZTF8MGHRJVU6ZDTGUHHJBJOP5B3MZ72O20U5KQW18R6YWQEM65VM4FJ5QWYL589V6EL0F6MSJZZ519LT0DWSH5PRXTMOQ8S3H4OS8C7KQA9DZEJ6HZW7U74833OZO6HQWMH3ZDVOUHBD2VRRAMFMWNKZZMLQ466BE4U6SGPU2S0LEK5HHMG786QA6555WCYHCWLMPZ4S7NZ8JDHV2W0JOJ95D25FYOIYNIF9FX53Z406B73W29N74PTM14TBS4RIXZBI7DQUJ21PUSJJ99BBWR2QZOWHG7F7PIH0XUAB6TC10J8C9V4N0EJ5JIUVV2SGXY00QBQ8XPGL355HRWZMJKA8RPFMYRIWEXV9ERR9302LDLK7RN91JKU1T45IUQS8Z2CN6EL5544HV2Q8BB4ZTTMIB58Z38HEA32ERM0584RSJMWJFGFDD72MFT7379FFR99IZRXOO051OELN3GO1XWIPBSD5E4C3OM04FL3G50FPSTKGC0XD8TG52MWIKA70TH8WGDEK8MR7I1ISSROCP6F0STX3GXUSOA5SC1FYGGFUTETCM8QROAE92NHV9GL0YD054K3DQ8WT1Q71LVCJ235ZP5TE0CWKG64DLPZT7QFA1CJE0FCS6PD8FV2DJEU6IA4K6FU10G6T68RVESPDYFPADL6OICN531QKLUZMV21WHG2ULPBX95UU65XQM9S4SE7UR7NS01HJBTEKQVKW77EEJIU4JYIHSHL07F0GZJU539TA8915FPULOAA05EQB7GKC324YB2O1P5OANMZFUL3OE3PBZRDU0LYMG2JU6V0N9KK7RUFIF10F460PE621ID6UW7C4W2PFK2JJ5TJ3LEX5D1K03XNOSNSF0HIU63MNXFQD5NRGVB8L083FT7T0TGNSE4ZG8U49MPEJP1TWD91LM0QXDHLONXR0D6KWGV1EOYBPKLB64IBNCGHTXQWVATXRA4W2IFILJQLIVPX7WTAP5457SRH4ZAR2SM5TIZPISDI3DBRIGZ51N8CEER1O4ZF1F2RB4C1QMT7L9PBIJ3F9RGTQVVQP0NQQHGA1QUOVQYW5FZGN1K0AYEARBDMJDYHRD7XPCUUA2Y58XG69H8T2LTR5YHCZ9SMGSCZSUFF6NLSM5FC6OIL5DE32EPSF4ZSQRL3OAB38B6EAZKMHQQVLNVW91CTNMACPHNMAHYT3Z9HOSP6F692FZQFCAQFH6MMAXRSH2BD46370MAQY22EYQB2JJ3PY2GQC7JEBXXG6XZ4VTI4WOWKYE1794Z5KGKWVGPWA491W8IZMGI97F6ENQTFJ8S8GAPW4NV55KDD1C34JXLK9KS570YOMJES2LM6LU5KGT6G1C1AAN3M2B92WAVCSALOESQMVOMSLAJWCPUPEE70YG0K2LESEG6OSO8KKVZDZK8E66KZ1OXBGTTGXON6DGWJL5LZV20OD8JZCYZ0B523HFT3YPKUN978EOQHETHP1VRQ7A70AZ8N5OU1O9V5KMK0T8GHW2G111PB5K0BFLEGU8U9DPVGYZ366QM7FFUL7Q2NB838I7KSV1API3SHQ6EDFM1582GFQZAFJY6FOUSC08C07J77MBGBFQZ9Y67CHXMDCPBCR4Y4KZP0NBBOPY18X140VIWDWNFGTRPZ926FGNNZ4L7T7WS0MD81S1361AT8MRDKHHCX1FPW8IJ2VHHM5VUFZX1RFEOR4AWKIR7S6KPL6KO6X13R9YH7ZNLKAOYMOUC4H7");
    DashedString y(*this, vy, min_length(vy), max_length(vy));
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert (sweep_equate(*this, x, y));
    std::cerr << "===== After Equate =====" << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert(y.blocks().front().known() && y.length() == 2);
  }

  void test20() {
    std::cerr << "\n*** Test 20 ***" << std::endl;
    DashedString x(*this, "ab =ab");
    NSBlocks vy("ab =");
    vy.push_back(NSBlock('0', 0, 2));
    vy.push_back(NSBlock('a', 1, 1));
    vy.push_back(NSBlock('b', 1, 1));
    DashedString y(*this, vy, min_length(vy), max_length(vy));
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert (sweep_equate(*this, x, y));
    std::cerr << "===== After Equate =====" << std::endl;
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert (x == y);
  }

  void test21() {
    std::cerr << "\n*** Test 21 ***" << std::endl;
    int N = 50, M = 100;
    NSBlocks vx;
    for (int i = 0; i < N; ++i)
      vx.push_back(NSBlock(NSIntSet('a' + i), 1, N));
    NSBlocks vy;
    for (int i = 0; i < M; ++i)
      vy.push_back(NSBlock(NSIntSet('a' + i, 'a' + M - 1), 0, M));
    DashedString x(*this, vx, 0, N), y(*this, vy, 0, M);
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    uvec u;
    bool b = sweep_x<DSBlock, DSBlocks, DSBlock, DSBlocks>(
      *this, x.blocks(), y.blocks(), u
    );
    assert (b);
    std::cerr << "===== After sweep_x =====" << std::endl;
    std::cerr << "x = " << x << std::endl;
    std::cerr << "y = " << y << std::endl;
    assert (ns_blocks(x) == vx);
  }

};

int main() {
  StrTest* t = new StrTest();
  t->test01();
  t->test02();
  t->test03();
  t->test04();
  t->test05();
  t->test06();
  t->test07();
  t->test08();
  t->test09();
  t->test10();
  t->test11();
  t->test12();
  t->test13();
  t->test14();
  t->test15();
  t->test16();
  t->test17();
  t->test18();
  t->test19();
  t->test20();
  t->test21();
  return 0;
}
