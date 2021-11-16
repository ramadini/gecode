#define DEBUG

/* Testing dashed string reverse equation and other propagators */

#include <iostream>
#include <cassert>
#include <gecode/int.hh>
#include <gecode/string.hh>
#include <gecode/string/rel.hh>
#include <gecode/string/int.hh>
#include <gecode/string/rel-op.hh>
#include <gecode/string/branch.hh>

using namespace Gecode;
using namespace String;
using namespace Rel;
using namespace RelOp;
using namespace Branch;
using namespace Gecode::Int;
using namespace String::Int;
using std::cerr;
using std::endl;
using std::vector;
using std::string;

class StrTest : public Space {

public:
  StrTest() {};
  virtual Space* copy() {
    return new StrTest();
  }
  
  void
  str2blocks(const string& w, Block* bv, int n, int l = 1, int u = 1) {
    for (int i = 0; i < n; ++i)
      bv[i].update(*this, Block(*this, CharSet(*this, w[i]), l, u));
  }
  
  std::vector<int>
  str2vec(const string& w) {
    int n = w.size();
    std::vector<int> v(n);
    for (int i = 0; i < n; ++i)
      v[i] = w[i];
    return v;
  }
  
  string
  vec2str(const std::vector<int>& v) {
    int n = v.size();
    string w;
    for (int i = 0; i < n; ++i)
      w += v[i];
    return w;
  }
 
  void test01() {
    std::cerr << "\n*** Test 01 ***" << std::endl;
    Block bx[3];
    bx[2].update(*this, Block(*this, CharSet(*this, 'a', 'c'), 0, 3));
    bx[1].update(*this, Block(*this, CharSet(*this, 'd', 'd'), 1, 1));
    bx[0].update(*this, Block(*this, CharSet(*this, 'c', 'f'), 0, 2));
    Block by[2];
    by[0].update(*this, Block(*this, CharSet(*this, 'b', 'd'), 0, 3));
    by[1].update(*this, Block('f'));
    StringVar x(*this, DashedString(*this, bx, 3));
    StringVar y(*this, DashedString(*this, by, 2));
    StringView vx(x), vy(y);
    ReverseView rx(vx), ry(vy);
    cerr << "Equate x = " << x << "  vs  " << ry << "\n";
    assert (vx.equate(*this, ry) == ME_STRING_CARD);
    cerr << "After equate: x = " << x << "\n";
    cerr << "Equate y = " << y << "  vs  " << rx << "\n";
    assert (vy.equate(*this, rx) == ME_STRING_CARD);
    cerr << "After equate: y = " << y << "\n";
    assert(rx[0].ub() == 2 && rx[1].val() == std::vector<int>({'d'}));
    assert(rx[2].ub() == 1 && rx[3].val() == std::vector<int>({'f'}));
    assert(vy[0].lb() == 1 && vy[1].val() == std::vector<int>({'f'}));
    assert(rx.isOK() && ry.isOK());
  }
  
  void test02() {
    std::cerr << "\n*** Test 02 ***" << std::endl;
    int N = String::Limits::MAX_ALPHABET_SIZE-1;
    int M = String::Limits::MAX_STRING_LENGTH;
    Block bx[3];
    str2blocks("0=1", bx, 3);
    Block by[2];
    by[1].update(*this, Block(*this, CharSet(*this, 0, N), 1, M));
    by[0].update(*this, Block(*this, CharSet(*this, 0, N-1), 0, M));
    StringVar x(*this, DashedString(*this, bx, 3));
    StringVar y(*this, DashedString(*this, by, 2));
    StringView vx(x), vy(y);
    ReverseView rx(vx), ry(vy);
    assert (check_equate_x(vx, ry));
    cerr << "Equate y = " << y << "  vs  " << rx << "\n";
    assert (vy.equate(*this, rx) == ME_STRING_CARD);
    cerr << "After equate: y = " << y << "\n";
    assert (ry[0].val() == vector<int>({'0'}) && ry.size() == 2);
    assert (ry[1].lb() == 0 && ry[1].ub() == 4);
    assert(rx.isOK() && ry.isOK());
  }
  
  void test03() {
    std::cerr << "\n*** Test 03 ***" << std::endl;
    int N = String::Limits::MAX_ALPHABET_SIZE-1;
    int M = String::Limits::MAX_STRING_LENGTH;
    string w = "Zz1xJr2yi3kD0njK4mjOWu5HaEnNDg9Ha7o8AniRkfU66m8EpyDsD5yYEF4PAtfcK1fI2aO0xPMfA1gtXhV685G9bVD6MX7urD0Uxq5P2lGERM6iqQYjpgZhuRMNDUCccQHMcnGvUofrvJskrn2vbrKFwvPaNcKlLnqQql7Ut39SxWLnS0kcASqfnDKMLiQLTvXOkcZ0 =09GXSOVfpYV3pELJMYcqwPm5H1O0IQXnE8o3KvItPWQFQopN";
    reverse(w.begin(), w.end());
    int n = w.size();
    Block bx[n]; 
    str2blocks(w, bx, n);
    Block by[2];
    by[0].update(*this, Block(*this, CharSet(*this, 0, N), 0, M));
    by[1].update(*this, Block(*this, CharSet(*this, 0, N-1), 1, M));
    StringVar x(*this, DashedString(*this, bx, n));
    StringVar y(*this, DashedString(*this, by, 2));
    StringView vx(x), vy(y);
    ReverseView rx(vx), ry(vy);
    assert (check_equate_x(vx, ry));
    cerr << "Equate y = " << y << "  vs " << rx << "\n";
    assert (vy.equate(*this, rx) == ME_STRING_CARD);
    cerr << "After equate: y = " << y << "\n";
    assert (ry.size() == 2 && vy[1].val()[0] == 'N');
    assert (rx.isOK() && ry.isOK());
  }
  
  void test04() {
    std::cerr << "\n*** Test 04 ***" << std::endl;
    int N = String::Limits::MAX_ALPHABET_SIZE-1;
    int M = String::Limits::MAX_STRING_LENGTH;
    string w = "67FDM6EUFULST0LM1I7YJPE4XSX5DIEQ53DZM3RTZNUF7EZA9XIENKHJR4N5RZZ5KE21ZM0PIQJTPUPRT7LVV58TIAY3GKZB43IJBH1HPW2789PA0YPVER4LQX4SAOJB6FPDBMNLLHPZMY6MP9MW1FYU5GWT7B9HVFTQ1XEX4BAV56R90FHEOM1CZS5QABFLFKGQR2ZFZVRIEX7MFTCAQD8EJPEIZ95BY7KG8R3HDPMEGUY7FMJGH3GK8V3HAORH736CQPHHAIJ8YXUE4P2IHCO5GVQW44AGSW8MLEM79XOI1VAO6AA1W7KMR450ZERP0TM0KLM22ICGRLQ3SUAHX1MJ5C4V79SNAAQVPVZZIADR4TZ80LASYA3HE5I9YZFE4ABX4IRHIA6L6BA29X7RSK3OX3QZTNZ0R8GNX98OB5JU5CY2QEX54K8C7F7ID8RYNBBNSHNWU9EMI4AT6KMWWP3JHO3AHGBM7SHS2ZQ9S1GQD85YFJD9OLE7GNJLE0GXB1Y8BP3QTNI1PHTU44Z14PAAOGZMDIMR9HXYAEAVZCYJOO03DRTNXNYAKON3866EB06T0JRU18YYI2TEC5SDMNV9F5NZFL606WGMISF9GFAIGE5B4CBO6M6C0N13J6R3UEDFFALU23R74Q8TYJMUSWGBB8C7K8X28V444X5Y4WUKTI90MWDAROF3FR4WDPOC8JGDUIXE5GWWHH3QFMAQJ1MZTVMP4I3Y5UC92JAF0QUNTYB42LV441I2EINWRLS3RSAO2M67VU26KD7OLWYP4HTNVRW6WH5NNQJGCX9TW6366U957VGN1402ITCBELNSXYSLT22VKZ7MZ1KVB2YIEYSICWCKP4EEISS6ECBYYUHTPUO9PAQ8OCBFWGLIQ8P2BQUFAUT3ZWXX16DZBR9MB6QVSLUYTQ37PNTLI03W9O9L2VLJDQO7XX0RISMFBX5EIML599ATW8877SQYJI42LI2M6CO5SVNK5ZXQAYRY6HV26DGSPA309VSS9QMUMOZLRUYWTZ0M26YYIPXL5Y96258XE6XQEN4SYXVOMNVBNC0R61SDK28PNEC5XYWKSCF52FW7QFE2B5IYUJB9T2KPLNTA777UZ279QXNB9VTN79AH3H4J126QLJYXDIXDQTM9TFF8TTFKGCN61JLRP0O6GV0CJIHF13MUG8O3F91KR8FA65X3QTW2EEKQI3GMNVCWH3NX48HOI2RUU9C2LKUK81EGG022C55AIZDF09BQ44K8YNRNILR310SHJIK57IN6MV3J0XA0I3FC5VGYYWPEWHW1M34CGTDTYMAGO5P9ZV4C2ZI9C4VJTR0IR4AYMEWGXBSN411MQKFMWHEQF51PBBQCBL33C9Z3AOZ6CRDGFFP8O128G1SYC01J5J9KZRD8KERAXRR8BIY4BEY0BEWR355O3FEFYC92B47ITJRJNHWSG9DVUP1FLVD8ZCI93Z03GNZ39MURV2PNL60XQ8GCI27KK2TQIVW4WODKM91PRW5YU5N3TB2R51HN8QZR98IJTXY2BJF8ACTZQ6N1978DYKDZ2NEBFEUDKB2G2KEUZWI61AMJO23U6KO1FH4B9GM6KBV9HSTPR7Z2L6Y3TUHOVC01XHNKTS1D6OEBKUHVYIBQSL20VRURSH7PR1VP1PPBS6KTUCW0ZUQ5KZQWBCYNUULPJP6J0OSY4YYZKKFYZ0Z955SSHWZEZA7DZP74TVHMHFTM2126ZDPT0D3JM5VCMN0XUV5TAUR6C6A7MOKD6LGD7MASM4TAQIUELJSVIAQ1MC3OPFOTJJQN3J948T3SQKF5P0LJUR17186C7Q9VFGT8VBF1AJ309OFC2LQIDGA4YO2RPFO0A2MU6NL59NEU1EJ3O3Q5ZWRP8M4NRZFC657ZNBASQ0JK7KREMU5E2X2KYG7E39UN9EYA4CQA0GIEEINN7H8ZQG05L4NB59168B82PNJE3I0X39YGEO4OKATT180LXYVQFGPLYO8PJR1WXOZI6OAX6KPAR40NOSD40UI45LRYPDKPCAT9GQGQYUVU2SO2TVQ6WUNC5VE4YNJBTXINLKV4NB2R2BQJVWP6QGSG7XPGM26ODA8Y431RYZ89A0BQPKKHRFKLN1JJZ4UU9B3X0KMQI20LC4PTVTQYNSNNYKTUY8PZFPM3JBJL7JQ27QDTHVQHZH7ZZ6XSZO4L06Z2QXDD12P54UY61OLI5BMLOBU2HIPSU76LFLY58WEL9YLA2NDVTPAJZ67TTJVDK299S7WB1MWMBS4KQFJN5KZHPGB1KO4AVCYME1FARK3GBPZBFLDJSYGJ0AWKDY2253SV52SNCU2JI28B5DMUKECHBTUM2VLQT7X43BHMH40Q4A7BK56CZSHNH3UKX7ZRGRFM1PKWXNBVC4OQJC9YXARFPLN6MFMDOG0ZYYTR3RGIZGPNJSUBFCN2TAAPCVT09DVV37N56FYAXZE4ZPCPHXTSUOFN2FNS5R8YE4Y186QRNHV7EAOFCDMRNQ1X9XMFI39LISL45MHAJSNO93SSD9U9PEIBR0U0J78VDH5040TP3PWERJZSPD3X9XFJCGVRLUAOHV1L3C1XP34NAJT8GSCQJR1K4R4H09AXFMXU1CA4NLPCXFQW5CIK3J4HAN5Y0L679UWHTQAKN70TH6OMY26GUP5R0Q86UQ2CEBAP4OGWNUX5E3UI6SXJ73IYMXTJLFSF73RVWKLKR09D5FLSXYTQK6J6R4J48RQ5CH902KD5K7A56QE32PRUAY1AZUNZ3U4MK4V10AXRCHW7H7AO9XHM8U78KK0YQOK0ANRVSI15VWWSND5Q7IM4YY2O49C8ELTINHRBC9QOLT91B410C0IX7ZAM019T4VDV4NY1UZFUNLC20LW8GV2O9504TSEBLJBZIGFVRM4LSLK3QQWUGOWOHW0X89XHWH6SUAD3464BE2JZYLL2ZTAHGTOL90AF92HUR0BLAWVV3EV81FEGD0JDNIQB84K7VC3CRP2UFLRI50CZDC43DC934P21IJN4FQC5T2WLJ1EYE9APTPCK2KI7SXX484V2AGLALCAQZCTQD63ZW7FZNECXRC37G4Y9VRA9X9C7CQPZYSL7H1HWRTWRWMRDYDUWOQCDB6O3UZ4R32TYYWVOTV7AJIAU18NL3Z6H4TSAVZ9FRHZN0LGT0F68DB2S92C026VBESMVNPVTAYLGNC85X52YT4A8V5DXSFJHNNEPG08ELJI3CLFSDCSHT4YV7HLT7YFTYD8KQEHCUY98XMADV80ZSJPF70FU5K4QYHX9AUYXIEIZEJSGY1ZXA333XKKGSBKTPZW6LDT9WY87LVF1N0MAUVOO0K7VJYTI2OEX93R6JP3EOIDAEYXZ7ZBL5TC1MRUS8GRU4BT17K7K6KUYAF8J0YQDNTQ9JOG007UW3XFY5ZM6T2DMDM5K498VOX4RVGVMSXYSGRTQUKDAPSO9V9GNWWPX2YP9NKU0OCMDP9LDAP8M200LQWBBHUVC3FQ5SNUPGTW4Y1RHYU3TS5UAJ89H94YTVSW5ZYQ72PAE3ZMYC831DJ1IYJOG0JTCINONWNBR4LNBIDC9Z6NQXXYQIFFX3AR3QOMRWKOSE122EHF7RXQT7DDG9RQFZ3YVUPQVECI47LSO6ZM17SZVJA6ZWI47O4C345CDQ6RUQYU8SY3490XU2OA9U3DPD1NF5SYIG1L439J46ISAFY6WDXHEQRZ6J39HWVVJNWAMNU7JJD27VNZ8P93CF8X2IA9Z59J8K792I5QA67IAJAHNTFK94N2AIAVH7GSJEBP47VLKISCNI6SD11UNRVP4OAB55APC2670PXMQ15ASLC8OF1ZBCGDW10W5UAP72J10WPYJZTEQAPX5EDI7GSDTOECN6S9Z7LNPWG9E06IO3FN3BYFLFRWPBDQ3PRGYJ3L00XEAFZKM4DIM37LCWMY1F3YWED0EQ4L2D5759U4SGGCJ0BWWM9S7X1FNGWT90RD2P9G0VSB2PGCLKUPZ2WUXU7JHRMV7MAVQBX000TENKKTQZJZW1XVDL48MSF5QX8APKDEJ19CDRB9VZH5E79B17MZZ78T4K8FM6WFWWKOYO61DHYKSHE4VLUOMOVJDBI080K8LKQ8DANKHHCKGBAX0YLWKUZS4JTEGS124VR89VNY83631LI584ALHXGF0EMVDPVOKK3XTIO1ZIDO0UNHT0TJX7BOVPC72I4N5NWKLD8Z4OU9QJR9K85WRCF3PM3HUFVQCZ0I2A2IIYMRJ58F7F5A9OGCI2577OL0H5Z9INTQ84ML7FNTC5BJ2RDXN1J63WDLVJAS2I8WPGNTLGDS6VSZOX73B2OLZ2L92GELMY5VRH6Q5FOU7BIL6M72ITH68CAHBZBS3OJIQBNRSIX7HUI3X6A0TXFCO4LHX3246ABW388UP2KHJ2DWVIZEWUK5CYD85ZLONI7CYTL67UCXZLN1O3EZBZTBFM2BMCUYHLLDE9O50MSYE94J632J8WD6NU60NI9JADASMKXDI4A4NTOWK69VPQK64C23NQJ8V8JGFQOY2QXT104YR882FQWLX8EJO6B2HBHVMJCEVQJQ0DQBN9J84NLG575HCPUJ9R21VULFGL8VNKNP48UB84EWGIK9Q1GH3XFFX4HFQ0XPVR66F9HIP7AN932I9EVRD5XRDK4ITVXC4VCDVJTARDU2CYWZWKU8SI0U6A248UL4NJAWLV865M1QSKPH0MQTRMB125I78FP2V5TPLFWV1CW6MH8EMQ59RX3SV3ECOTFB17UNCC85WO3EKNG9Y057K7188XJOZ2HGQOTNJREY6HUA2TD7PBL5JS69N2X3JWWWCTL3TVPRCS41YKG79ONZSJKQ73PO54RS57O6CUEIQRG5PM5REX1F35R99IFY6T4UQSC6XH8GESU46DVHF6ZKM2FLAMR6B5WJAT4E65GLLZN7BEQ6K60N5FQOEZXUAZ30EACOY8H05TRYN9MC1F2LHK3H4NW1AMKA3JVFJX6DNSSV758XTZ5EJOCBK99IU9R018GIE54IKETF3RR4YFUI3UG1O3ONDFFKW5VDEX1A8E5LF3H8EUDKX9F0CTK6NSG8T1AFZWQB2CC0MQCHT8CJ8P4DNT44D5BAF6RQOYHS5UDA98C0MEGSLD8QNQDE80K810OK26D71RVLP3G36FVT6JXWOPLV6ZAY4PI9AIZIZ0DPJ2CNSX9XLQXIAMPT0DP99GP727TX0C8P01NTDXGGLLEDNH4G1Q7HFP6Z8V6PVQR8JX43FNIKRRMWV21K11VZDI73PALEZ6C59J8JHFTDM1QBJ79AJDMRK85HAWWRITOVEXWQ7GDMFJ1GWBOJ7XFG81E6TI1IKY84J8DCBJW2QB3PG4891PO9X3AMIFV9GMBWJ2TDM7SHOCFPQJNYSG07E6IWCCKI4S4UJ3UEKYR11DSHOI1QRS5BVIMF0ZNWD99VA06IH9B3ZFRN0HRYBO6MVJGFTTB8NHMPFPXIADB6IMAR5RYSUKV1FWYRXX3T3I51RTVTU1U78CG010W9URC6A8NACLSY2RU89ZB4K15YCQ2W3S82H6ZKMSS56WODFNZW4LXS250I2LBZXH2IC482W7JODN72X09BEPANYDKZATZRUIL83Z00DP4U1893U2EK3529BHKF1EKQQ3FSLCOSGV9XE78XXBIEUSZ8LUL645RE7X5SY2FOHKKB766BI31YXTLU0DWPPO11PMID7D00SMVJJ2G9M20DR7STFPMXPH84Q0BKTFUFMJVOVDWX1L1IY979BQS8TQK22S7VO9KXE8JQZ7YBMZIYMHYZ7RTFIUDAPSIMJHFDYG812PNA3PH66RPPSMBU1IGL959FO9PJX95JTXNBSYTYTECI2DKOYS0FUQYCY0INUM1UN343NLVFCVPMQUIBNGYX3T19O14QIB8J2HPTNJFBSLMSIOMVM6JMUWWUKPLK6433WOP3T4G9OQVX9DGZ56R4PP5K90UM3WI8QRFMDKS7BZ2X87FQGPVFJS6CRD4CVTJHSUVSF769Y2IJ6JE6GXK2MZ2EB20Z11WMTPOTK8OZDS9B1P69BCDJUOUSZRP4JN2NHD9TY8U9AJT07M515NX5DLJR91C7V0GSJG6T3FVKT0I9K1TG1GQ9EM4GD32SYVZMH4VIG5Z0WUMSY2OJ7GOQF25SUF7WHJXEFGX3O71W9WKZDDIMK8BAZCT1YPTNYHA1CEZ2STJYEAO1NGHRD89EDR0CPRBJVPFMBL3LTM0OUYSDE6FSURRHVBYZ5TXECMY7XKAUK5PYDKAVL1UQI95QUDNT2Z03EY8AUACFDB7SV5E8UPKA0AASBCIDWV16AETPB5C1L9VGI1OAH7Q982B2T8AMZ6NX4TOKA63PC5G6NI273H27POSQKW5L7W0QWDRMO7BD28RHSKI71LYR6TSGPVDUVLL2Z7WDMQ4SPPXY0X1LDZ5NFI0Z5UDOO6BSMI03EA5JYPGW7IVZ0TRI4RAEZC5LPUC6ZQNO5TIHGUXO8VWO3PFEBLL2CVIVR0YJMAFMAP61M4FR7KHSQ2I8EI7FJDKGJK2Y398WX2OJOL4IH38SQRJ2Y3MYKJVM89F4B4MZR6HHLAHJFN5GMONTE3E2MQITQVZRTJC0BXH0C7YJK6SW067NMBTEQ1ZRMC8RYODVQR52460CBDX2I7TPAVJZZVD8R51SXIHY74VDXJJE1CHVW78JQT6SWMCXYNHSV5DDAF2UGOIVW2Y185UDGCY5216YH3UFUGGA75WKG0375GLVOHJB03BOWLRLJ8T9JOXAGO9SSYERGMB71BH37DADO4WLBF40OCE4X4T3IY0YATEIZDH6ADCV1NXMZ2YSEX18L9TX2QFKK65RWZ5U5AIT29F45UTTK51CDY349K5J9YN1FNKTK8AJ8WLEX21M09BRCCO0Q0Y17P3F4QK26LVBV6SYAHE43NOI8NVW8HV1UKHZMN19M0B2UUI958YWBPEV1WOIN23FBZXFUQ2BIV98MG2IXODWE5MXMLVMM0TMHK0OGSZX7JM2KA2NPUH3KCKIOGKWB72KELT9BLF4MXZZVAA8M0HMG9Y34BE00TV44O0QPOUOO0O6487IPCFZE69WNNLZ46YOUGW407J3WG4B9= 9GMB4QFR8G45AH7H6BLUMSV11PRRJ97GD9VF2MQWBC7KSPPZB01OJPPAPYOOC6QC31BM18OOVV1ODYMYETHJI00R3S36F7TBB8BCA7J2VAK2AJ9F2VEX7RSXGZKLNX0BN3VUNV4MOVUBCWP9CT5XCN24634WLLFD25F8ZTNQ2NRF39MGISH5QLWZH3PIQLEHG1B2YMEAA27H5LVGG19YHX0USWXB8035Y47BVNZGXPQL7S8PACUNKT1MJ28XM9UFF7Q5UZE63N4INPFA5MZN9QBE5BAWYPLJCRIILZ66VN08ABL7CLFFGZDA3LSD2QSAQTF9DL15ARMAZ1XEO8ACFP2SKG77PQU0QTYKHK1327MQH0VAV5V4IWN7EF03OM6UYY404OJ8QHX8RN3CBHSDP9NZMSGEMF18AZ0Y3ZIRKK5EPSH0LLO3KQV6ALHRSSVQ8HFOR0PYHHMEH9N1DOW2YQEW9S750PDLVLL6D8RGYHU27HAZ286JZ2IIPTXW5QVEDP7V0IC9HAN610GEKYS7A0T9AB6GAX3P0CKVJWY21TMFIHBR1TV7G4UUH9V26WUZNS5LD6URK797260X90OZMU6KAZO4S0K6EBOZUYWOAWZROOX0QNEIOZVT82H0KRUSWNCMXPS5RMGEKK2FH3IKB8BNMTV100Y4NHL6T6CUX5CRM3D5UV185FNAMG8MKEPG1D2KSAVU0UBJDPM65YDDBO5LPV8V7JLF25B1NH9F9RGI4H46C8FXUHMDM0GML3CYABRS8IJPON0QM0AFCZR2GLDNPN3T19EBE5SOKD4MXDHY955K643PUWHF9RLSENURLFQ5NI7C652QC4X664NEI4WX5J3CONDCY3LAL1W98I21UJSM2F7SELR1ACWU0DWHNWFPO4RFCHMTFZK5L0P1RIKFLBQ5RLC1SGGUCTTVYRQP4J5PCSHTOTKNI8C4SNTIFMG2S2P2TPRZ8OJE0RK0SDPGE0F5M310BY4KLO6D6R9ZMKKCWJ9OY8FBBRN0TNTP4WWKFYKG8EGQCIBJAQH6SD32EEY43VSK0YRZKMZTDSBG4TMSYNRW7B89WWZTRBM44UHAWSXV1YXD6WRV511URRWRLZTF8MGHRJVU6ZDTGUHHJBJOP5B3MZ72O20U5KQW18R6YWQEM65VM4FJ5QWYL589V6EL0F6MSJZZ519LT0DWSH5PRXTMOQ8S3H4OS8C7KQA9DZEJ6HZW7U74833OZO6HQWMH3ZDVOUHBD2VRRAMFMWNKZZMLQ466BE4U6SGPU2S0LEK5HHMG786QA6555WCYHCWLMPZ4S7NZ8JDHV2W0JOJ95D25FYOIYNIF9FX53Z406B73W29N74PTM14TBS4RIXZBI7DQUJ21PUSJJ99BBWR2QZOWHG7F7PIH0XUAB6TC10J8C9V4N0EJ5JIUVV2SGXY00QBQ8XPGL355HRWZMJKA8RPFMYRIWEXV9ERR9302LDLK7RN91JKU1T45IUQS8Z2CN6EL5544HV2Q8BB4ZTTMIB58Z38HEA32ERM0584RSJMWJFGFDD72MFT7379FFR99IZRXOO051OELN3GO1XWIPBSD5E4C3OM04FL3G50FPSTKGC0XD8TG52MWIKA70TH8WGDEK8MR7I1ISSROCP6F0STX3GXUSOA5SC1FYGGFUTETCM8QROAE92NHV9GL0YD054K3DQ8WT1Q71LVCJ235ZP5TE0CWKG64DLPZT7QFA1CJE0FCS6PD8FV2DJEU6IA4K6FU10G6T68RVESPDYFPADL6OICN531QKLUZMV21WHG2ULPBX95UU65XQM9S4SE7UR7NS01HJBTEKQVKW77EEJIU4JYIHSHL07F0GZJU539TA8915FPULOAA05EQB7GKC324YB2O1P5OANMZFUL3OE3PBZRDU0LYMG2JU6V0N9KK7RUFIF10F460PE621ID6UW7C4W2PFK2JJ5TJ3LEX5D1K03XNOSNSF0HIU63MNXFQD5NRGVB8L083FT7T0TGNSE4ZG8U49MPEJP1TWD91LM0QXDHLONXR0D6KWGV1EOYBPKLB64IBNCGHTXQWVATXRA4W2IFILJQLIVPX7WTAP5457SRH4ZAR2SM5TIZPISDI3DBRIGZ51N8CEER1O4ZF1F2RB4C1QMT7L9PBIJ3F9RGTQVVQP0NQQHGA1QUOVQYW5FZGN1K0AYEARBDMJDYHRD7XPCUUA2Y58XG69H8T2LTR5YHCZ9SMGSCZSUFF6NLSM5FC6OIL5DE32EPSF4ZSQRL3OAB38B6EAZKMHQQVLNVW91CTNMACPHNMAHYT3Z9HOSP6F692FZQFCAQFH6MMAXRSH2BD46370MAQY22EYQB2JJ3PY2GQC7JEBXXG6XZ4VTI4WOWKYE1794Z5KGKWVGPWA491W8IZMGI97F6ENQTFJ8S8GAPW4NV55KDD1C34JXLK9KS570YOMJES2LM6LU5KGT6G1C1AAN3M2B92WAVCSALOESQMVOMSLAJWCPUPEE70YG0K2LESEG6OSO8KKVZDZK8E66KZ1OXBGTTGXON6DGWJL5LZV20OD8JZCYZ0B523HFT3YPKUN978EOQHETHP1VRQ7A70AZ8N5OU1O9V5KMK0T8GHW2G111PB5K0BFLEGU8U9DPVGYZ366QM7FFUL7Q2NB838I7KSV1API3SHQ6EDFM1582GFQZAFJY6FOUSC08C07J77MBGBFQZ9Y67CHXMDCPBCR4Y4KZP0NBBOPY18X140VIWDWNFGTRPZ926FGNNZ4L7T7WS0MD81S1361AT8MRDKHHCX1FPW8IJ2VHHM5VUFZX1RFEOR4AWKIR7S6KPL6KO6X13R9YH7ZNLKAOYMOUC4H7";
    reverse(w.begin(), w.end());
    int n = w.size();
    Block bx[n]; 
    str2blocks(w, bx, n);
    Block by[2];
    by[0].update(*this, Block(*this, CharSet(*this, 0, N), 1, M));
    by[1].update(*this, Block(*this, CharSet(*this, 0, N-1), 0, M));
    StringVar x(*this, DashedString(*this, bx, n));
    StringVar y(*this, DashedString(*this, by, 2));
    StringView vx(x), vy(y);
    ReverseView rx(vx), ry(vy);
    assert (check_equate_x(vx, ry));
    cerr << "Equate y = " << y << "  vs " << rx << "\n";
    assert (vy.equate(*this, rx) == ME_STRING_CARD);
    cerr << "After equate: y = " << y << "\n";
    assert (vy.size() == 2 && vy[0].val()[0] == '6');
    assert (vx.isOK() && vy.isOK());
  }
  
  void test05() {
    cerr << "\n*** Test 05 ***" << endl;
    Block b[2];
    b[0].update(*this, Block(*this, CharSet(*this, 'a'), 0,2));
    b[1].update(*this, Block('d'));
    StringVar x(*this, DashedString(*this, b, 2));
    StringVar y(*this, DashedString(*this,Block(*this,CharSet(*this,'d'),0,3)));
    b[0].update(*this, Block(*this, CharSet(*this, 'a', 'c'), 1,1));
    b[1].update(*this, Block(*this, CharSet(*this, 'd'), 1, 2));
    StringVar z(*this, DashedString(*this, b, 2));
    class C : public Concat<StringView,StringView,StringView> {
    public:
      C(Home h, StringView x, StringView y, StringView z) 
      : Concat(h, x, y, z) {};
    };
    StringView vx(x), vy(y), vz(z);
    std::cerr << "x = " << x << "\n";
    std::cerr << "y = " << y << "\n";
    std::cerr << "z = " << z << "\n";
    assert(C(*this, vx, vy, vz).propagate(*this, 0) == ES_FIX);
    std::cerr << "After z = xy:\n";
    std::cerr << "x = " << x << "\n";
    std::cerr << "y = " << y << "\n";
    std::cerr << "z = " << z << "\n";
    assert (vx.val() == str2vec("ad") && vy.size() == 1 && vy[0].ub() == 1);
    assert (vz[0].val()[0] == 'a' && vz[1].baseEquals(vy[0]));
    assert(C(*this, vz, vy, vx).propagate(*this, 0) == __ES_SUBSUMED);
    std::cerr << "After x = zy:\n";
    std::cerr << "x = " << x << "\n";
    std::cerr << "y = " << y << "\n";
    std::cerr << "z = " << z << "\n";
    assert(C(*this, vy, vy, vz).propagate(*this, 0) == ES_FAILED);
    
  }


  void test06() {
    cerr << "\n*** Test 06 ***" << endl;
    int A = MAX_ALPHABET_SIZE-1;
    string w1 = "A;CW7.MC3ER88MWZPRP.9H@A3?(2-UL3S3-3EG<;MQ@4TW6%*,FKYZ;J3XMF9?<F9>F%I)*HTAX3)7?'/0X*19<D1T)A><#V$V4UL7$@D W$,U5&GPFA(MH;.Z-N7/FBT7H0L5/;(#$S<LFZ6(SY5H6#YY/VD.=CUJG.5<7?O%W1N@PTHD3;A3.A4X%GN3Y(/&FFQ2#MK&/)WM&:>=23WNH;Q72P YHOAM++MROZPIOJ=))4MR7?&D;=N/&RR(6E7ZB,$?<<0GIE51P8%NV:J";
    string w2 = "K+BW#FY9X=TSG7E/<&#+LG+S&M .8K3UT)-''GXFH2D2D(?<BFE>XH*4G(:>F,;?AH652FX>2+MU)?N ?T'?YO,%(90 Y $1Y/,O 8MS6-A=5<WA 8";
    int n = w1.size() + w2.size() + 1;
    Block bx[n];
    int i = 0;
    for (auto c : w1)
      bx[i++].update(*this, Block(c));
    for (auto c : w2)
      bx[i++].update(*this, Block(*this, CharSet(*this, c), 0, 1));
    bx[i++].update(*this, Block('='));
    assert (i == n && n == 392);
    StringVar x(*this, DashedString(*this, bx, n));
    StringVar xx(*this, Block(*this, CharSet(*this, 0, A), 390, 390));
    StringView vx(x), vxx(xx);
    assert (vx.equate(*this, vxx) == ME_STRING_NONE);
    w1 = "%N:K+BW#FY9X=TSG7E/<&#+LG+S&M .8K3UT)-'GXFH2D2D(?<BFE>XH*4G(:>F,;?AH652FX>2+MU)?N ?T'?YO,%(90 Y $1Y/,O 8MS6-A=5<WA 8=";
    w2 = "*9.63TS$1BY#<VP+%U.N#%,0@/JH$0X>A<VLG'O@M-II%;D-XXU5,,O+Q8&YWO%DX.SS/AADJG-$.7*FX+R7UEW(E(S5EY:;<M%> CKL#DBB";
    n = w1.size() + w2.size() + 1;
    Block by[n];
    by[0].update(*this, Block('8'));
    i = 1;
    for (auto c : w1)
      by[i++].update(*this, Block(*this, CharSet(*this, c), 0, 1));
    for (auto c : w2)
      by[i++].update(*this, Block(c));
    assert (i == n && n == 226);
    StringVar y(*this, DashedString(*this, by, n));
    StringVar yy(*this, Block(*this, CharSet(*this, 0, A), 110, 110));
    StringView vy(y), vyy(yy);
    assert (vy.equate(*this, vyy) == ME_STRING_NONE);
    ConcatView xy(vx,vy);
    std::vector<int> vw = str2vec("A;CW7.MC3ER88MWZPRP.9H@A3?(2-UL3S3-3EG<;MQ@4TW6%*,FKYZ;J3XMF9?<F9>F%I)*HTAX3)7?'/0X*19<D1T)A><#V$V4UL7$@D W$,U5&GPFA(MH;.Z-N7/FBT7H0L5/;(#$S<LFZ6(SY5H6#YY/VD.=CUJG.5<7?O%W1N@PTHD3;A3.A4X%GN3Y(/&FFQ2#MK&/)WM&:>=23WNH;Q72P YHOAM++MROZPIOJ=))4MR7?&D;=N/&RR(6E7ZB,$?<<0GIE51P8%NV:JK+BW#FY9X=TSG7E/<&#+LG+S&M .8K3UT)-''GXFH2D2D(?<BFE>XH*4G(:>F,;?AH652FX>2+MU)?N ?T'?YO,%(90 Y $1Y/,O 8MS6-A=5<WA 8=*9.63TS$1BY#<VP+%U.N#%,0@/JH$0X>A<VLG'O@M-II%;D-XXU5,,O+Q8&YWO%DX.SS/AADJG-$.7*FX+R7UEW(E(S5EY:;<M%> CKL#DBB");
    ConstStringView w(*this, &vw[0], vw.size());
    std::cerr << "xy = " << xy << "\n";
    std::cerr << "w = " << w << "\n";
    assert (!check_equate_x(xy,w) && check_equate_x(w,xy));
    std::cerr << "Unsat!\n";
  }
  
  void test07() {
    cerr << "\n*** Test 07 ***" << endl;
    string w1 = "A;CW7.MC3ER88MWZPRP.9H@A3?(2-UL3S3-3EG<;MQ@4TW6%*,FKYZ;J3XMF9?<F9>F%I)*HTAX3)7?'/0X*19<D1T)A><#V$V4UL7$@D W$,U5&GPFA(MH;.Z-N7/FBT7H0L5/;(#$S<LFZ6(SY5H6#YY/VD.=CUJG.5<7?O%W1N@PTHD3;A3.A4X%GN3Y(/&FFQ2#MK&/)WM&:>=23WNH;Q72P YHOAM++MROZPIOJ=))4MR7?&D;=N/&RR(6E7ZB,$?<<0GIE51P8%NV:J";
    string w2 = "K+BW#FY9X=TSG7E/<&#+LG+S&M .8K3UT)-''GXFH2D2D(?<BFE>XH*4G(:>F,;?AH652FX>2+MU)?N ?T'?YO,%(90 Y $1Y/,O 8MS6-A=5<WA 8";
    string w3 = "%N:K+BW#FY9X=TSG7E/<&#+LG+S&M .8K3UT)-'GXFH2D2D(?<BFE>XH*4G(:>F,;?AH652FX>2+MU)?N ?T'?YO,%(90 Y $1Y/,O 8MS6-A=5<WA 8=";
    string w4 = "*9.63TS$1BY#<VP+%U.N#%,0@/JH$0X>A<VLG'O@M-II%;D-XXU5,,O+Q8&YWO%DX.SS/AADJG-$.7*FX+R7UEW(E(S5EY:;<M%> CKL#DBB";
    int n = w1.size() + w2.size() + w3.size() + w4.size() + 2;
    Block by[n];
    int i = 0;
    for (auto c : w1)
      by[i++].update(*this, Block(c));
    for (auto c : w2)
      by[i++].update(*this, Block(*this, CharSet(*this, c), 0, 1));
    by[i++].update(*this, Block('='));
    by[i++].update(*this, Block('8'));
    for (auto c : w3)
      by[i++].update(*this, Block(*this, CharSet(*this, c), 0, 1));
    for (auto c : w4)
      by[i++].update(*this, Block(c));
    StringVar y(*this, DashedString(*this, by, i));
    StringView vy(y);
    std::vector<int> w = str2vec("A;CW7.MC3ER88MWZPRP.9H@A3?(2-UL3S3-3EG<;MQ@4TW6%*,FKYZ;J3XMF9?<F9>F%I)*HTAX3)7?'/0X*19<D1T)A><#V$V4UL7$@D W$,U5&GPFA(MH;.Z-N7/FBT7H0L5/;(#$S<LFZ6(SY5H6#YY/VD.=CUJG.5<7?O%W1N@PTHD3;A3.A4X%GN3Y(/&FFQ2#MK&/)WM&:>=23WNH;Q72P YHOAM++MROZPIOJ=))4MR7?&D;=N/&RR(6E7ZB,$?<<0GIE51P8%NV:JK+BW#FY9X=TSG7E/<&#+LG+S&M .8K3UT)-''GXFH2D2D(?<BFE>XH*4G(:>F,;?AH652FX>2+MU)?N ?T'?YO,%(90 Y $1Y/,O 8MS6-A=5<WA 8=*9.63TS$1BY#<VP+%U.N#%,0@/JH$0X>A<VLG'O@M-II%;D-XXU5,,O+Q8&YWO%DX.SS/AADJG-$.7*FX+R7UEW(E(S5EY:;<M%> CKL#DBB");
    ConstStringView vw(*this, &w[0], w.size());
    std::cerr << "y = " << vy << "\n";
    std::cerr << "w = " << vw << "\n";
    class E : public Eq<StringView,ConstStringView> {
    public:
      E(Home h, StringView x, ConstStringView y) : Eq(h, x, y) {};
    };
    assert(E(*this, vy, vw).propagate(*this, 0) == ES_FAILED);
    std::cerr << "Unsat!\n";
  }

  void test08() {
    cerr << "\n*** Test 08 ***" << endl;
    ViewArray<StringView> vx(*this, 2);
    Block d[3];
    d[0].update(*this, 'd');
    d[1].update(*this, Block(*this, CharSet(*this, 'a', 'b'), 1, 2));
    d[2].update(*this, Block(*this, CharSet(*this, 'c'), 0, 2));
    vx[0] = StringVar(*this);
    vx[1] = StringVar(*this, DashedString(*this, d, 3));
    assert (vx[0].smallest_unfixed_idx() == 0);
    assert (vx[0].leftmost_unfixed_idx() == 0);
    assert (vx[0].rightmost_unfixed_idx() == 0);
    assert (vx[1].smallest_unfixed_idx() == 2);
    assert (vx[1].leftmost_unfixed_idx() == 1);
    assert (vx[1].rightmost_unfixed_idx() == 2);
    Gecode::String::Branch::None_LLLL brancher(*this, vx);
    int i = 0;
    while (brancher.status(*this)) {
      const Choice* c = brancher.choice(*this);
      std::cerr << "Before commit: " << ++i << ": " << vx << "\n";
      brancher.commit(*this, *c, 0);
      std::cerr << "After commit:  " << i << ": " << vx << "\n----------\n";
      delete c;    
    }
    assert (vec2str(vx[0].val()) == "" && vec2str(vx[1].val()) == "da");
    
    d[0].update(*this, 'd');    
    d[1].update(*this, Block(*this, CharSet(*this, 'c'), 0, 2));
    d[2].update(*this, Block(*this, CharSet(*this, 'a', 'b'), 1, 2));
    vx[0] = StringVar(*this);
    vx[1] = StringVar(*this, DashedString(*this, d, 3));
    Gecode::String::Branch::Block_MinDim_LSLM brancher2(*this, vx);
    i = 0;
    while (brancher2.status(*this)) {
      const Choice* c = brancher2.choice(*this);
      std::cerr << "Before commit: " << ++i << ": " << vx << "\n";
      brancher2.commit(*this, *c, 0);
      std::cerr << "After commit:  " << i << ": " << vx << "\n----------\n";
      delete c;
    }
    assert (vec2str(vx[0].val()) == "" && vec2str(vx[1].val()) == "");
  }
  
  void test09() {
    cerr << "\n*** Test 09 ***" << endl;
    Block d[3];
    d[0].update(*this, 'd');
    d[1].update(*this, Block(*this, CharSet(*this, 'a', 'b'), 1, 2));
    d[2].update(*this, Block(*this, CharSet(*this, 'b', 'c'), 0, 2));
    DashedString x(*this, d, 3);
    x.lbAt(*this, 1, 2);
    x.updateCardAt(*this, 2, 1, 2);
    std::cerr << "x = " << x << "\n";
    x.splitBlock(*this, 1, 'b', 0);
    std::cerr << "After split x[1], x = " << x << "\n";
    assert (vec2str(x[1].val()) == "b");
    x.splitBlock(*this, 2, 'b', 0);
    std::cerr << "After split x[2], x = " << x << "\n";
    assert (vec2str(x[1].val()) == "bb");
    x.lbAt(*this, 1, 2);
    x.lbAt(*this, 2, 2);
    x.splitBlock(*this, 2, 'c', 1);
    std::cerr << "After split x[2], x = " << x << "\n";
    assert (vec2str(x[1].val()) == "bbb");
    x.splitBlock(*this, 2, 'b', 1);
    std::cerr << "After split x[2], x = " << x << "\n";
    assert (vec2str(x.val()) == "dbbbc");
  }

  void test10() {
    cerr << "\n*** Test 10 ***" << endl;
    Block d[3];
    d[0].update(*this, Block(*this, CharSet(*this, 'c', 'd'), 3, 3));
    d[1].update(*this, Block(*this, CharSet(*this, 'a', 'c'), 1, 1));
    d[2].update(*this, Block(*this, CharSet(*this, 'b', 'c'), 0, 2));    
    DashedString x(*this, d, 3);    
    std::cerr << "x = " << x << "\n";
    x.splitBlock(*this, 1, 'a', 1);
    std::cerr << "After split x[1], x = " << x << "\n";
    assert (x.size() == 2 && x[1].lb() == 1 && x[1].ub() == 3);
    x.ubAt(*this, 1, 1);
    std::cerr << "New x = " << x << "\n";
    x.splitBlock(*this, 1, 'b', 1);
    std::cerr << "After split x[1], x = " << x << "\n";
    x.splitBlock(*this, 0, 'c', 0);
    std::cerr << "After split x[0], x = " << x << "\n";
    assert (vec2str(x[0].val()) == "c");
    x.splitBlock(*this, 1, 'c', 0);
    std::cerr << "After split x[1], x = " << x << "\n";
    assert (vec2str(x[0].val()) == "cc");
    x.splitBlock(*this, 1, 'd', 1);
    std::cerr << "After split x[1], x = " << x << "\n";
    assert (vec2str(x[0].val()) == "cccc");
  }
  
  void test11() {
    cerr << "\n*** Test 11 ***" << endl;
    StringVar x(*this, Block(*this, CharSet(*this, ' '), 0, 144));
    StringVar y(*this, Block(*this, CharSet(*this, ' ', 'Y'), 1, 145));
    Block bz[3];
    bz[0].update(*this, Block(*this, CharSet(*this, ' '), 0, 144));
    bz[1].update(*this, Block(*this, CharSet(*this, ' ', 'Y'), 0, 144));
    bz[2].update(*this, Block(':'));
    StringVar z(*this, DashedString(*this, bz, 3));
    std::cerr << "Before equate:\n";
    std::cerr << "x = " << x << "\n";
    std::cerr << "y = " << y << "\n";
    std::cerr << "z = " << z << "\n";
    StringView vx(x), vy(y), vz(z);
    ConcatView xy(vx,vy);
    assert (vz.equate(*this, xy) == ME_STRING_NONE);
    std::cerr << "After z = xy:\n";
    std::cerr << "x = " << x << "\n";
    std::cerr << "y = " << y << "\n";
    std::cerr << "z = " << z << "\n";
    std::cerr << "After xy = z:\n";
    assert (xy.equate(*this, vz) == ME_STRING_CARD);
    std::cerr << "x = " << x << "\n";
    std::cerr << "y = " << y << "\n";
    std::cerr << "z = " << z << "\n";
    assert (vy[0].ub() == 144 && vy[1].equals(Block(':')));
  }
  
  void test12() {
    std::cerr << "\n*** Test 12 ***" << std::endl;
    string w = "ab =ab";
    reverse(w.begin(), w.end());
    int n = w.size();
    Block bx[n]; 
    str2blocks(w, bx, n);
    Block by[n + 1];
    w = "ab =";
    str2blocks(w, by, n+1);
    by[4].update(*this, Block(*this, CharSet(*this, '0'), 0, 2));
    by[5].update(*this, Block('a'));
    by[6].update(*this, Block('b'));
    StringVar x(*this, DashedString(*this, bx, n));
    StringVar y(*this, DashedString(*this, by, n+1));
    StringView vx(x), vy(y);
    ReverseView rx(vx), ry(vy);
    assert (check_equate_x(vx, ry));
    cerr << "Equate y = " << y << "  vs " << rx << "\n";
    assert (vy.equate(*this, rx) == ME_STRING_VAL);
    cerr << "After equate: y = " << y << "\n";
    assert (y.val() == vector<int>({'a','b',' ','=','a','b'}));
    assert (rx.isOK() && ry.isOK());
  }
  
  void test13() {
    std::cerr << "\n*** Test 13 ***" << std::endl;
    int N = 5, M = 10;
    Block bx[N];
    for (int i = N-1; i >= 0; --i)
      bx[N-i-1].update(*this, Block(*this, CharSet(*this, 'a'+i), 1, N));
    Block by[M];
    for (int i = M-1; i >= 0; --i)
      by[i].update(*this, Block(*this, CharSet(*this, 'a'+i, 'a'+M-1), 0, M));
    StringVar x(*this, DashedString(*this, bx, N));
    StringVar y(*this, DashedString(*this, by, M));
    StringView vx(x), vy(y);
    ReverseView rx(vx), ry(vy);
    cerr << "Equate x = " << x << "  vs  " << ry << "\n";
    assert (vx.equate(*this, ry) == ME_STRING_NONE);
    cerr << "After equate: x = " << x << "\n";
    cerr << "Equate y = " << y << "  vs  x = " << rx << "\n";
    assert (vy.equate(*this, rx) == ME_STRING_CARD);
    cerr << "After equate: y = " << y << "\n";
    assert (vy[0].val()[0] == 'a');
    for (int i = 1; i < N; ++i)
      assert(vy[i].baseMin() == 'a'+i-1 && vy[i].baseMax() == 'a'+N-1);
    assert (rx.isOK() && ry.isOK());
  }
  
  void test14() {
    std::cerr << "\n*** Test 14 ***" << std::endl;
    Block bx[2];
    bx[0].update(*this, Block(*this, CharSet(*this, 'a'), 1, 2));
    bx[1].update(*this, Block('b'));
    Block by[2];
    by[1].update(*this, Block('a'));
    by[0].update(*this, Block(*this, CharSet(*this, 'a', 'b'), 2, 3));
    StringVar x(*this, DashedString(*this, bx, 2));
    StringVar y(*this, DashedString(*this, by, 2));
    StringView vx(x), vy(y);
    ReverseView rx(vx), ry(vy);
    cerr << "Equate x = " << x << "  vs  " << ry << "\n";
    assert (vx.equate(*this, ry) == ME_STRING_VAL);
    cerr << "After equate: x = " << x << " \n";
    cerr << "Equate y = " << y << "  vs  " << rx << "\n";
    assert (vy.equate(*this, rx) == ME_STRING_VAL);
    cerr << "After equate: y = " << y << "\n";
    assert (rx.isOK() && ry.isOK());
  }
  
  void test15() {
    std::cerr << "\n*** Test 15 ***" << std::endl;
    string wx = " bT]?e]qCu|B,LSV!W( ";
    int nx = wx.size();
    Block bx[nx];
    str2blocks(wx, bx, nx);
    bx[0].update(*this, Block(*this, CharSet(*this), 43, 74));
    bx[nx-1].update(*this, Block(*this, CharSet(*this), 0, 31));
    string wy = ":?@NbT;^AZR3IuW3ee:)DpBr%&C]=x=BqcG8[Pe.Uj` ]c4]?e]qCu|B,LSV!W(e: ";
    int ny = wy.size()-1;
    Block by[ny];
    str2blocks(wy, by, ny);
    by[wy.find(' ')].update(*this, Block(*this, CharSet(*this), 11, 11));
    by[ny-1].update(*this, Block(*this, CharSet(*this), 0, 17));    
    for (int i = 0; i < ny/2; ++i) {
      Block z;
      z.update(*this, by[i]);
      by[i].update(*this, by[ny-i-1]);
      by[ny-i-1].update(*this, z);
    }
    StringVar x(*this, DashedString(*this, bx, nx)); 
    StringVar y(*this, DashedString(*this, by, ny));
    StringView vx(x), vy(y);
    ReverseView rx(vx), ry(vy);
    // NOTE: x and y are not equatable.
    cerr << "Equate x = " << x << "  vs  " << ry << "\n";
    assert (vx.equate(*this, ry) == ME_STRING_CARD);    
    cerr << "After equate: x = " << x << "\n";
    cerr << "Equate y = " << y << "  vs  " << rx << "\n";
    assert (vy.equate(*this, rx) == ME_STRING_FAILED);
    cerr << "Unsat!\n";
    assert (!check_equate_x(vx, ry) && !check_equate_x(vy, rx));
  }
  
  void test16() {
    std::cerr << "\n*** Test 16 ***" << std::endl;
    Block vx[4];
    vx[0].update(*this, Block(*this, CharSet(*this, 'a', 'b'), 1, 2));
    vx[1].update(*this, Block(*this, CharSet(*this, 'x', 'y'), 2, 3));
    vx[2].update(*this, Block(*this, CharSet(*this, 'a', 'b'), 1, 2));
    vx[3].update(*this, Block(*this, CharSet(*this, 'b', 'd'), 0, 4));
    Block vy[6];
    vy[0].update(*this, Block(*this, CharSet(*this, 'd', 'e'), 0, 2));
    vy[1].update(*this, Block(*this, CharSet(*this, 'c'), 1, 3));
    vy[2].update(*this, Block(*this, CharSet(*this, 'a', 'b'), 3, 5));
    vy[3].update(*this, Block(*this, CharSet(*this, 'h', 'n'), 0, 2));
    vy[4].update(*this, Block(*this, CharSet(*this, 'a', 'b'), 0, 1));
    vy[5].update(*this, Block(*this, CharSet(*this, 'g'), 0, 3));
    
    StringVar x(*this, DashedString(*this, vx, 4)), 
              y(*this, DashedString(*this, vy, 6));
    std::cerr << "x = " << x << '\n';
    std::cerr << "y = " << y << '\n';
    int fst = 1, lst = MAX_STRING_LENGTH;
    assert (sweep_find(*this, StringView(x), StringView(y), fst, lst, true));
    std::cerr << "===== After n = y.find(x), n > 0 =====" << std::endl;
    std::cerr << "x = " << x << '\n';
    std::cerr << "y = " << y << '\n';
    std::cerr << "n :: [" << fst << ", " << lst << "]\n";
    assert (fst == 4 && lst == 8);
    assert (sweep_find(*this, StringView(x), StringView(y), fst, lst, true));
    std::cerr << "===== After n = y.find(x), n > 0 =====" << std::endl;
    std::cerr << "x = " << x << '\n';
    std::cerr << "y = " << y << '\n';
    std::cerr << "n :: [" << -fst << ", " << -lst << "]\n";
    assert (fst == -5 && lst == -8);
  }
  
  void test17() {
    std::cerr << "\n*** Test 17 ***" << std::endl;
    // TODO: Test find.
//    NSBlocks vx({
//      NSBlock(NSIntSet('d', 'e'), 0, 2),
//      NSBlock(NSIntSet('c'), 1, 3),
//      NSBlock(NSIntSet('a', 'b'), 3, 5),
//      NSBlock(NSIntSet('h', 'n'), 0, 2),
//      NSBlock(NSIntSet('a', 'b'), 0, 1),
//      NSBlock(NSIntSet('g'), 0, 3),
//    });
//    NSBlocks vy({
//      NSBlock(NSIntSet('a', 'b'), 1, 2),
//      NSBlock(NSIntSet('x', 'y'), 2, 3),
//      NSBlock(NSIntSet('a', 'b'), 1, 2),
//      NSBlock(NSIntSet('b', 'd'), 0, 4),
//    });
//    StringVar x(*this, vx, 0, 10000);
//    StringVar y(*this, vy, 0, 10000);
//    IntVar n(*this, 1, y.max_length());
//    std::cerr << "D(x) :: " << x << '\n';
//    std::cerr << "D(y) :: " << y << '\n';
//    std::cerr << "D(n) :: " << n << '\n';
//    class find : public Find {
//    public:
//      find(Home h, StringView x, StringView y, Gecode::Int::IntView n) :
//        Find(h, x, y, n) {};
//    };
//    assert(find(*this, x, y, n).propagate(*this, 0) == ES_FIX);
//    assert (n.min() == 5 && n.max() == 8 && x.val() == "cbbb");
//    std::cerr << "===== After n = y.find(x), n >= 0 =====" << std::endl;
//    std::cerr << "D(x) :: " << x << '\n';
//    std::cerr << "D(y) :: " << y << '\n';
//    std::cerr << "D(n) :: " << n << '\n';
  }
  
  void test18() {
    std::cerr << "\n*** Test 18 ***" << std::endl;
    // TODO: Test find.
//    NSBlocks vy({
//      NSBlock(NSIntSet('a', 'z'), 0, 3),
//      NSBlock(NSIntSet('b', 'b'), 2, 2),
//      NSBlock(NSIntSet('a', 'a'), 1, 2),
//      NSBlock(NSIntSet('c', 'c'), 5, 8),
//      NSBlock(NSIntSet('a', 'z'), 1, 2),
//    });
//    StringVar x(*this, "accc");
//    StringVar y(*this, vy, 0, 10000);
//    IntVar n(*this, 10, 50);
//    std::cerr << "D(x) :: " << x << '\n';
//    std::cerr << "D(y) :: " << y << '\n';
//    std::cerr << "D(n) :: " << n << '\n';
//    class find : public Find {
//    public:
//      find(Home h, StringView x, StringView y, Gecode::Int::IntView n) :
//        Find(h, x, y, n) {};
//    };
//    assert(find(*this, x, y, n).propagate(*this, 0) == ES_FAILED);
//    std::cerr << "===== UNSATISFIABLE =====" << std::endl;
  }
  
  void test19() {
    std::cerr << "\n*** Test 19 ***" << std::endl;
    // TODO: Test find.
//    NSBlocks vy({
//      NSBlock(NSIntSet('b', 'c'), 0, 12),
//      NSBlock(NSIntSet('a', 'a'), 3, 4),
//      NSBlock(NSIntSet('d', 'd'), 1, 2),
//      NSBlock(NSIntSet('b', 'c'), 2, 4),
//      NSBlock(NSIntSet('a', 'a'), 5, 5),
//      NSBlock(NSIntSet('b', 'b'), 2, 3),
//      NSBlock(NSIntSet('a', 'c'), 0, 8),
//    });
//    StringVar x(*this, "abb");
//    StringVar y(*this, vy, 0, 10000);
//    IntVar n(*this, 0, 100);
//    std::cerr << "D(x) :: " << x << '\n';
//    std::cerr << "D(y) :: " << y << '\n';
//    std::cerr << "D(n) :: " << n << '\n';
//    class find : public Find {
//    public:
//      find(Home h, StringView x, StringView y, Gecode::Int::IntView n) :
//      Find(h, x, y, n) {};
//    };
//    assert(find(*this, x, y, n).propagate(*this, 0) == ES_FIX);
//    std::cerr << "D(x) :: " << x << '\n';
//    std::cerr << "D(y) :: " << y << '\n';
//    std::cerr << "D(n) :: " << n << '\n';
//    assert(n.min() == 7 && n.max() == 27);
  }
  
  void test20() {
    std::cerr << "\n*** Test 20 ***" << std::endl;
    // TODO: Test find.
//      NSBlocks v({
//      NSBlock(NSIntSet('0', '3'), 4, 7)
//    });
//    StringVar x(*this, "123");
//    StringVar y(*this, v, 0, 10000);
//    IntVar n(*this, 2, 2);
//    NSBlocks vx({
//      NSBlock(NSIntSet('a', 'a'), 1, 1),
//      NSBlock(NSIntSet('b', 'b'), 2, 2)
//    });
//    NSBlocks vy({
//      NSBlock(NSIntSet('b', 'c'), 0, 12),
//      NSBlock(NSIntSet('a', 'a'), 3 , 3),
//      NSBlock(NSIntSet('d', 'd'), 1, 2),
//      NSBlock(NSIntSet('b', 'c'), 2, 4),
//      NSBlock(NSIntSet('a', 'a'), 5, 5),
//      NSBlock(NSIntSet('b', 'b'), 3, 3),
//      NSBlock(NSIntSet('a', 'c'), 0, 8),
//    });
//    StringVar x1(*this, vx, 0, 10000);
//    StringVar y1(*this, vy, 0, 10000);
//    IntVar n1(*this, 0, 38);
//    std::cerr << "D(x) :: " << x << '\n';
//    std::cerr << "D(y) :: " << y << '\n';
//    std::cerr << "D(n) :: " << n << '\n';
//    class find : public Find {
//    public:
//      find(Home h, StringView x, StringView y, Gecode::Int::IntView n) :
//        Find(h, x, y, n) {};
//    };
//    assert(find(*this, x, y, n).propagate(*this, 0) == ES_FIX);
//    assert(find(*this, x1, y1, n1).propagate(*this, 0) == ES_FIX);
//    assert(ns_blocks(y.domain()).slice(1, 4).known());
//    std::cerr << "D(y) :: " << y << '\n';
//    std::cerr << "n1 :: " << n1 << '\n';
  }

};

int main() {
  StrTest* home = new StrTest();
  home->test01();
  home->test02();
  home->test03();
  home->test04();
  home->test05();
  home->test06();
  home->test07();
  home->test08();
  home->test09();
  home->test10();
  home->test11();
  home->test12();
  home->test13();
  home->test14();
  home->test15();
  home->test16();
  home->test17();
  home->test18();
  home->test19();
  home->test20();
  delete home;
  cerr << "\n----- str_test4 passes passes -----\n\n";
  return 0;
}



