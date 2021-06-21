#include <gecode/string.hh>
#include <gecode/driver.hh>

using namespace Gecode;
using namespace String;
using std::string;

class StringOptions : public Options {

public:

  int N;
  string SQL;

  StringOptions(const char* s, int n): Options(s), N(n) {
    this->c_d(1);
    switch (n) {
    case 11:
      SQL = "A=B=C = B=C";
      break;
    case 12:
      SQL = "+;+*=!'*I =*I";
      break;
    case 100:
      SQL = "LXA.U22=H-= W+QOA<)!@K?4L@3L)=V%$))<XI>+P>)AP&435Z@2)P6TSE)Y8                           =)P6TSE)Y8H!";
      break;
    case 200:
      SQL = "7*C).CYGU.UW ?>DC +<<(PMUD%C@SA34G,1V9H7WG,.JK(0#U$H-N>G4LRHSEL:JKU!4D6?F,<!)E=R> Y>3D0W61&AEDAI1+,(-5;QL*4D30JV;J'S#K.YA'EP,X?HO.Z(7DN7(X*U4)B)PZL:5@?/O78<U=HO.Z(7DN7(X*U4)B)PZL:5@?/O78<U7@Q'<=:,0$?!";
      break;
    case 250:
      SQL = "XB%DQ:QQDX57@NG$XU8PKU1GTYU,K$:3JA=6@V-WD;)RM@&2D,W3M> F:S2:M.49UHKC58KD#=UW8ZFRZJAFN;)B9?6@-ZVO;1#YOJTI) 7XH6&@QZ$AXRJ-=XC,IEM)F@+WO:R&*:S64GN(%Q'ULJ@S?497.9?#Y@?425'*@,-7>=7<M' ; &CWJ8#O94:584L#F'1UZ28T@B+'%ZA?72F96/(@=%UIIOL3EL-W<%V.ZTSW.B2:U%UD6U";
      break;
    case 300:
      SQL = ")%L>/4T/L(5V-$:1V'(XEB-O5SPDK =&)YT%R,6W0UD<K)4L58?#X9L4D0(8@1>A)<=AR2W*)'ZO +:+8HZNS=#XM*E2?7?C:T9$W7#;*R8$@9>:0N4SU2,XCI9+HCB%W#6>NVL7S+T!1<&2.1JFO5GVMB&N:&J-ZMQN1@G,670 ACG'%I),05T'$                                                                                          ='%I),05T'$C$'62/C$F+1'GO";
      break;
    case 400:
      SQL = "NR-7A365%&I.+E*H2>),$/HM/?BRLJM-D'OEXDF&OX6$FNA0,ZY7YLE@<=P>9,GI&359MMK:58CYV)Q,##0?C#<(M:2SJ&P+D=:9E,@+C.U=>+4(Z)LA81.K7X:.TW4;#=AMXQDVK<9Q=T&E8??J;&LQ0/Z?IB/<')UU.,?FE                                                            =   7A365%&I.+E*H2>),$/HM/?BRLJM-D'OEXDF&OX6$FNA0,ZY7YLE@<=P>9,GI&359MMK:58CYV)Q,##0?C#<(M:2SJ&P+D=:9E,@+C.U=>+4(Z)LA81.K7X:.TW4;#=AMXQDVK<9Q=T&E8??J;&LQ0/Z?IB/<')UU.,?FEU";
      break;
    case 500:
      SQL = "V3EZKXFXQFVMVZNHVRIOI0YP5WHVETOZXTWUQ47HMIKQ8OAWMDMG1HLGVXTJK8GZBLVXP1BQXEE06GUVXLMSR1A7HKQLQBEZGVQ3T1SLM3VS8CPTCQG6KLUEDBFE9CNAJHDZEUNGTV68TVWDBGJNGKJCEKG4FQA0GI3252PAKIKSLNZZZ23AOJBS9CHDHUVRZRNVRTJABKEWQISEG36QSGIKKLFQT047VZNBR2FFI =  ID1QI0NNTKDJXOSSZJZRZNM4JFLZJMW8PB860MGTF74JOUN0QTDZEDZLCUWCTE7J8OCDP2GSWF1NCWCT6XA1VWFNXZFVHDOXN7RCQ8F4KWGY6XRFQXLNNDTIGNNUUYXHOEZVY5PIP0UFQOPQHJQPVEOROORLQLPNRL4VXVHZM4K1V3KOVBP9UI2LAET9ZIB8WRFI3RESY3MXFF397VQBNY7FOQR9Q061SXKMNVVLEQFNTTCTWC0UQWNKDVTRMHUAFDO5UKV";
      break;
    case 1000:
      SQL = "H4,$;)4$GJUE&TB&8@LRR0ENI(84;U/X.L)A)LH08W4PV@9KBL LNB.V78OQC1<VD4JU-A3;R/*P<A-ETP7.1:>PB;6+?-BW0J@SY1O6&QXI/9BV)JGB#HY@8;2V8R2SWS=&XWB=3QJ(Q>37'YW(JK3UZ5PALY+/@S8M> $@#A+?BKA'+7MO*'$OAUH-P7GL5%?G?4/-M#A>R%WPAYQT8 ?%S$N7&K<4=&4T7CFT7HS<51EUD7 #CE>=MBV'<(&AJ<=YA@)B.6)-6Y0'L<Q8PQ)J2%&*=Y,2Z8HBJL:R2FYB(3AB==1%?IZVK8N1%$65<0E&LC?J@SAO7KNZ+4Q=V5YOU$N'MQ)OXJ(YQ.S >@4;?(1DE5T0$UVJ2G=KB<+UF<E;4W&'@*50D(H8+P&0-G;ZUJWGAN13.RKKPN'VH?PU?G;@($2:0=&ZT ;3VS1IUPEC+$$A%(A$TUD$5&'QGKS(W,=DKPXS8FAYYQP 26/4'Q=7)4HX(A%3B5T'NYHOQHEF4SN3&SZT?VN=>2A.-OSCQ(XKHFB%.9KU<-2:,:B'XSG8Z@NW-X;B<R5F(FD7+EAA97S'%#=.G0Y73/Z2WMBUS.=:H>QH,(/D9PRHVD 76.%==J0D/AOV7/,B')R*@;&2$K.1G9+TOR.J'VQX-UGH+>'J$<=0ZB#F.I-#9?<>QJOV57<B50(<(X Q+%O@%3F:97=3J1H(MI?3MFUJ*M+ &LS AK6K/3:KRYL@C1@:A+AU,-UM*#;X1NJZ<(1$M4X BNXD7F?U'<7;*AW$FJQJY/ 6S4K&RY;66M'I2>(P82:D(PE@)CMW -=9)$EKCMVM;5NIU#S/OO8)FK +'1..FIV6)V4LLD--YY<T*NE?T3VR$CF,>7I;51EG@LWJ=GP%=L$  .JJKKAY:I F1+*8+6-80FJ1-6 B=#P4;3F+E'@7?WD8>C@C2BR?$3=:HIRJ9'=L2J=:Q140X<Y>:Y:QDP@PKD><V20KZG@=";
      break;
    case 2500:
      SQL = "V9KVM5KVGS2TZJHPUWV81VCIIBOZMKEQPCMN4UJ9RESI18AWJQNIYFSLXOBH3DIOKU51SAQO1TILW2TSWXRH89AWKHJATNWF0GHRVYCDGZXRGSDVVZ5MX3KSBWGFBCDAIOV6KWVTXCKAVE1BAK28TYFLQC2BAEU7OOFGL9ZTHLZ94PFYIYS3WZMK29Q6YSM7KRO2ZSJ8RSRWUAUKTYMWTKFLAZYDS79GZAUILTMKOWRBNTWS7FOYQ1KXZ2WXGX0MYRQXMGLJEUY37PI9QBI4FVCEROTTPFKVZPNGUAGDEQYQ4C8A7ER7LFLYD29VL2LFB6UJCBNRXR5HL4YUZB0BYYKDASEBH5NHGAQNLH6YHMOBYXX8CIMUMFYKWWOQPBLP6IUOEW6JMCOKYZ5RDWH0T7ZVN4OWRWJTRMUV4XXP52RVRCX7XTGVGO8ZMPQYQY76GXT7XOBFUJ75ZDPQNTVBSYTJE72WWLK26E2WLJ12GSEGAI1UVLWJ75WWQRSVEGSIWKFC9MSZF58HELOOJK7POW6QR1HWSPXN8PR0W6BCISHH5SAJF0R1PGMEXEWWHIOW426VVCBGGFFN1EO7NAJOABIPYZH5SOXCZNGQBLYLKX3ROMM6QHVTG7PAEGW8JRPZFADRRGH3XPOMBT0OFTXEIHO83UWXWLMG0EZOHV6GLMEZPKXEL8JC4CB2GPC6FUWCX2PRGRNUVZWVMSKZQLKVV9EMVCEELCBFACL0HHMJLOT4YYSKP9RRB3QCIBJXEMHHV2GJI6LJCLMEQLYTLZBN1J2QDCPQTSKR26BSUYPUEJ1LBWOIBQRJK6VVRGLWQNBYWDQBMUBNO5VRZTWVINSBEOMQPMCPVGRI0NTYVWLOZBSXTCAEMORKFM3A0MUVRKFR2BPIAGPBNT77HZQ29YV5QLNBSJNALGBPVS0KBT1SCBBA5FJJJKEJLRCBEAUFHAHIW71B7N1WLZ2KAWVUQIKEYDTBTSTJAQIWVNXLLUGJZIT4XEGP59EP71HJ3VIRYCV8YJFARUF8FKNB2GE9NVIUH8T4ALPCUKNXWHLTQZYJKXAFVQTFS9KANHRTRKYGP6GQWERGGUQTKZFM52XQWGAMRTHTYASDL90L4UJZ9LEUJWAKBZ6CZLOFQ2X6LNBQBVCLENKKQ2ZSWET9GRSEN4DYIMJGARJHWLX2PVJMQS3AVPJAKPUJCUU6C5WLO50NKPKXRCLXHHTM1RDJIAGL6RGZZR1ALLJPJFU6EUNUYKOB6MHNZGWCVZ6YGFXT41SYWMMXEHZO6K6V7PVSQAAQEJT6NSJYNUEPNI0SM3GVIDU8UVKKUX2E696GYNCWWWMVECGEOXQW5HTVDQHQ4RQYCTEL6LOHCMKFFAVWN4GJKOSOQ6FELYKY5L0RTGJAUSATBE4RWUJIRHMSQUBCUUDUALEOH7HMXGQ9LSVIUAXEINVPF1KLRTRJGGO2NUMLXESXCMMWGCYKZT18OULWA5PABC3ZLRXXNXRATAKHOJ7T=TVFVMCOAB7DDQCRC5IWREGQRWF8PCGWPJ1ZT8IRAFAWSYVDAOLVH42AHBQZ8ZSB2OQXJTCPP3346WSYLUBY8JORSNGFAZYMCMFFC8KTHWRJRWXZA3TP5O75Z6UOSGNV7ZH8PFZBO126GSC9WP9CNC16NZROVRYUZQVOMCUWGATXZJDYIYW7NIHJ1QWALK1SHQLRQ6ONMW4FJBQ17ZKWARJ03YTUNJMSR1JHZIXYQKBDPILPYAHHS7GWAQZAUJVT7RAUHULIG9ZICA6NCWCMFFHWBQFG9EIJFJWPYMO7HCFA15P8ZJWZJV1CHJXBPXRIBLXYBZ0SMGK9REMMUQVI1AUE8GVFHGJ6Q72ISK2HLH2HPX6K2AM7FR2BO1PMTV5RYAA8U1HSTWEKB9UPONNKGLWM8XU2B4TOJ7CLFHZ5XNJ7OI4IOBWKY0O3LWB4B87QHGV1BOVWZFWFU7XPNZCVDYCPSDEAILW0EXYEAJF386ZMSH5OJNSPPYSZ2139YTE5IKENRY43PX43O37Z5UTOAI5LV9EOG3HLRTTMNUZO7X4O2S63SAQ0SIDIMFVZJ2GU23Z1LU1MRSUQV4MRJOVZ3PQISVFCG1JP5ICM8DG8CSDPBQWRNUYJPSK8IMZANGKY1UIC5BM5NWTDEHRZUVNCLESKM25SMKPCFVKBJXZOZRRNEKL7GEADMKEG7CPAI0LLZCCIMDLIUZNMUXUHWVPCYWAGG9TYZWSI82XGCI4AKC22EB4FZN7OJABHQKANLQZNNWL9SVIMADPM2P3ZEORYDUHEZWSXXZEIXWRCBNFJKQPYRG13SCNORA8TZJ15NHOVFJDWS61KTJ3S5KWRTTYTUJCMANRR6ODJW05QUFNMIMN5HHKECOP3DCQ4LTLPKTFM8XGPCFRCJ7IMSHX5TSFMPEYRCZILGLI5JHJNJSMQYCX0VJCLRMCNGI9LBLHMRUTSZV7V5T6TWVUCYE6LTQVLRKCJSUTJSRHF";
      break;
    case 5000:
      SQL = "3UK8&Q0XB,%2&HCA()G8C&&DQSD44/,GO?-EKWXSV6,J1Z@*XQM&-I<28RB&U?,*QV/CPIEGX4;37%5&,J0T9ZFV2K=L5=M):K7#ZYAO;X@QJ)?M'L0:OE.;54&%AIGSE==K0:-H'/QI95MU?%,5G MG7T,F#CFO6E=:(H6W4TR&<POJ%,D:GPNEH131F1(,2U'FK+8S-/OYJ:'(=?J:DUENZ(4SGW/5(>-20 #TI,AE84;E1P0$) 7SWO2MI(5H4GWZ>SL+ZV6(=D@?QU(STUM+P%)</ .)1T-L%8I*I3HM/SY)#8 QWX:+J1@G)XX0V?W(Z8J8U(03DA7WWZ1;11R%2L+R)JO=R<QLOL*$<:YQ1KGK*:(<(M<Z$X=2MRES3$GV2/>S,E1.,S08.W68GAO75,YAUR%A1(HQ/Z:/ND.%7IG,?=L7V&$'V9)9+=B#/:5&NPOKGTP&:4DW*J,&:>(*R>(),YRQ+R39;.(84AY&-MRW#D0#5FI4NDTI@&$,33)>,VS/UV:<<G6NXTIZ5P9,/=QI#80-TN<-7BMZW(@ OC&F;:;Y(P;.4XT;7M$=1J)D'R>>609(*N<)#H(K%WZ,W<*+=/7LQIWB4AX(7539=Q&&JY;960E'729 ,7L,E*+WYL/+)#2E*ER3-S,S-E?-;%W4<6O:'-&IMYK:SUG)2%U9LYM)=%O7S4):JA4?,+UOQ?GL0+ ?<UX7 GJP5Q XWZRD/<*71&1N$=7W*OJN=FDRM2>8G/,WL3NE0O,RTTG#Q)?R#6-HF7EXOK  5AI*/M/D=>J>QR5+?F81CJ7Z@X?*R39%7F4,*U'BIC#(#)10%ZT4T9'(HG4OJ.Q.&LNL?8VDHA02/C 9;LO%$FU*.W/(RHU$3W'6-.SO<OT:C@HFYHU2('=QP54Q;=Q%U;U0 KA2U#2U1U)- +$$2:K)GH;OQ66:?IEH8O2P*9/Z-P=CNBZ()210C+I6=?:;UG3:.+)I*LPH+5.ZOWG *H3QX$Q0R%UEI N:RT$&'199ZE'KE/3$V+3Y I(9W5>G>NRY+D(()Z4KSCAFAPQ$@GP8$ZHPZXW6GYC,P?2J@BU+.ADWQG#Y3+832V1N+ ).B:98#H>)C:Y KTS<T?+M.6;+8D2,G:::?)R:=.%2VH$%&5J57-+CI<659B##7L D2GV26E,;G0H0D/3@VK=:7IC#IJ@294PF%==VCDY ,'#7;#C*X@.U8M@5D>K&;@?F88#@AA'DMKU*:KX(D* :3<N)?SCA=56M+@TO$>B@B13FFX0-D#C0@S,:D?*/RJ(%?+QB(PI14Z@<SHG>J/?:DCDCYBOFHS-W2,B<VQ%X;18EDU3W C<G*$B-+>2,BGEVK+E90? '2EJMRZ#QB8D%GO 0(/YVE)IS LM3*G4#' 1B(F$<LA F3CAU5%:SRV#,B+Z XZM0=(?5+B2J0%UJXFZM)&6@82ZMCQ/QO<H7K.7P?#'J1AZ'WZCBC,A.YUXR#/D+0*=D CDPEHSWL47Y+2;%=NGZ.$SCE*C0L:K/&M?RJNV3C.Y2.Z##+1#0* WI 375*WCQC2';S06O&T7MH6@DO .I>X.ZAHP9>P=B#2C<5NFOR<*9F;27*:6,?JIK=,CA5'4KV3Q?,62,8=PSNNKVCK6WN,<$A2S=ERG2U62.&IMYLAVB<$N*E:2>853V$BV(4>0W$I*?>P=>6.A1-KG0<)?MY3Q8+3OC)TL7),J7-@,7*<LT7%W4HV'4DD.F%FY 9@>U(A@%A*I=&2>HO@GVZ7KP.$0J-I'I@B G+A0P@QA<GF*# 'Y#F.,AZV>$CVP+(ZQ/6$Z+8CEI+(FIP;V9&Z:,F)YV'LJ/XL=G>.D.OSW5'#+,8N48KX>GFWO Y3.%O$##LN7@VT@<2QE*YNYZ+IX3<B &Q8;P6;U;101:4<: M*1=A?6FFU@ANC38'(,1/5EAR/  ?W7J%<?%9<56H&8K$&MAX$W< F?$&Y5?,MB+P/S&3W9'8%,+;B#KS31RMZ6.CF1N'=FC0U1EGC(D@5Y+&+I7;.)=3=2GAD,Q)EOU8N4B&P5.$.YYKF>F)B'8#<83DKG<?-5)#&77N4-ITWN9Z<BXMW6T2N#Y9WMA+XFCY @R'S0%I(*7V%WWLAZ/V;N;E>):%U:QOW3; VP4>OMWK$6AP:'RGCIZ&3(NRRI$?SO/D=LDV0J43WB7VB5))W*C*5<'=:0G3VI69=GG=J(9*U=H=%I(>%6$4E$ZPQQ,,?:B*Y.1$88ZXO:+4L1PM2('RGR9*J0'.*MGM$G CV.ZS-MB:PF-..X/KEG@V*B?WC,=<Z'NT EPYH0 -PBTE)KOZU*8H&7=MLRO7&#YC2&3N2=AB0KYVRPA=<%W0@LH6C,-W$JQ<WAGXWE-*=E(X-W?-39(=P-TLWDE*Y+3B'#X8TCP76JVJJN,BLCZCFWBXC'U&K&B3A(2)#AY,POA>NEG@EI/2:>S$ G<BRND;.$/J.;HUK/HGTLN%Y9:=S5TR-,P%) 'EJ7(30/6W'%:/;/WZ@3-Z.14VU=I@2#5RT&+GN>P#EA ('QEOMD,C1UHL$C,JR,N$@<=(CANG-W/&G/C78G6%L9$0V<+<=/XPT5A1@Z/8H- -9:6I:GS97%QK7>>/K6RW:LA7TRW33ZS1 ':HSK)RV06147?FJ?96-%HYK40>%ZHI4SW9/=#C>OF5F-O$#=X3W/,N8OU)K6?E%A>$<GUZJ'G?LE?;E6E#*J9,8BZMMB2,B6,29'<W>?*MT*XWN7PXPH<B%S<K--F?/'C@->N.()94*MQ,UYSZPQV/<7#.Y* Z:O7D<J08PZ&M/H&&N'R0- Z5F;:<Q;FF42#N@U@0-$TA X9LC#:,A;FJ$;B3E5R<:?'*V.'J?9LQ$GS(-*( MJ)ZT2BB8CS*FR%O5TQ- ?9&T/(U4(F?33FP,.G46737?#I$(8  >/ FREYN93IHL<5;L3D'&?TIPPYQ0?MK(R-/3'LDJ1R*%(-+SHL'% <K?:HF9U1QMD/C6%RGUQPSH-4)7;$0)B>&YPPR8+8U.TJ3',W78##&GU==E#UTS<%#%PAW,HGF7IEE( U L-F-+6V#2=TR#=LRI,2=@TZFN$+-L5HPF.6DQAG)MFF/H, NT2L>4;91QK##5*J;)@;&##J64%2J<FF=&A+YRN0Y5=,3#UW(S@R-L6UIFDDI.( :D.Q3KBRR(5BX(?66V'()>U@%=:+-0HORXE>FBU@?'$');->3Q*>&27;;@YBC9-LJ( J5*T,#C1'6,7X%.DN=6MQV8$669NKGW?Y5BWFWF1&9GQ*G;<$FBP6GJA2X;NFQ&:G6#;1GCMMQ8RAOLV%2M1C>0E*7*:Z:+B5Y.J7QBD6*7S3+D&T8NX=G(ERZ7Z-+1>JVL0L4 BMD?.VDTEV*L4'GFED2VYC%G*U.,XM@>41>K0#G%V93C<S;;=S&YBG#+01NRY(+A->XPE%5?AD7, ,GJ5L32U1BQJMAM$45Y6D?1/*3D=>:( 2.WM>37KNRX0>5>K.RQ$6.TLYUZMQ=%:16VRP+-:S GD*HKEC1U7FS=9?G9H%&:IE?PWJJV'<VC3<&P467>6T$TP(@/8H&<&2ML%IN9*EA4DJ3T39G>:36R'4;5'8OJZ+F*SR?C&F -<EC5=ST$R9*;J3,&J 8>:E.@+Y+FVMID#*((K@JE#X526WD/)S&7;-UTI+Q:+YOMFEKHU4RD8P9=SB=#Q,QY$;D4$+ZISO?Q=5/*GX>.;M''1.$'2 :HE'< );E35;+H1;-VHHM:)58/G&0(,L$&&HX#&1(+P:NL'&UA8,G@CA6XNT<VM+'TG224MJ=DOR'R/%*:<=(HR+ZJ%F1$-?S<O9AKR.89<Z,*72,N 78@>);OGD1B,NA3#8@A/'8CS&A9TW55W8+8<YE87;G8O@AB**44%6T7@8/95$B5&% 0#UVJ0?(;V+EVI.'X/F12L5DN:>#H/A(W:T$3T(8D+B199(MD/WD=0)(9HA%;KBJ)V<:@$H/*FOW,)=F* &?TO0ZY,>@'45G,2K:%WO=<'RRV'M;<SKRLER/?4 6M/=@(D+V8-L+V=.&3KT4I%=6?K':AEP' 8ZFR9KID9&XAB&$-BIN3Z;7C9CS+4BJ(F8N>UX@&G1VZ1 ,Q'2U(Y=O5WM#*88YRLRFX:>;VU%2/LV.>)?(-CG=S>=C:9R&'  %H4YX>$8=W3O7WEREQPW9#?@Q/+PHV*1$I7DNO@+.XL<N&I.;@/ $:.OFO@EN:M7LWH)8C1SU4O?W-N8<M5F<2JUO&TF4USFA$N=5JASBDZ-**J8'AZR/$$0P18,F#%<-%AT3#WD=%76*S:6P'B.P:<'IT:(151H#GW-DK%'MWF.G/(U10)DI3;0; L)C#C9&%&1#YFXOY2U SUO'6*C M9G5F2(-EP-XI)AY#@DP>PR; M*IHZF6J+9: <>DSGBNE%().%O<9&*B-I#N@<@MY:3,.?4(%QVFL7K+&'I YHL)UFDF,:V&R@=S*'/Q?LG=>.57+')024F:U8RFGHI,JV&9M12DN4S.46KYV9Z?ZF:MD$HTAIE=G=4O@+O<.J9P*D#GHFX19O=$U8(-0.4(2A+UOG6J<55.R$;/8JFM7;LGOMB>?. UA,794U1V;XK1=/340.;I<8=33D0T$++5.;6+%<Y8F.AYKBS)UA8$CT%LB8JL:R#)GZ E@JB85YN3D@/DNI+'=SK;QGFA-;J5H'04TKN@<,7#X.06;OXK 9UL7+&3VP&6P#B-@P>N9&X.U.8UQHVH()#,/1%B8FS2V=2<K:1?:B/?D1NDV1NB(?Q(?X3#',<CDG?SHZ4OU::>G:K5T')Q%<$PB(.;NIF@9(Z0Y7+6C;M52MDCYH=LI4<VI%'8UMQU$XP1CF0UYQ9D@KCK<OT4W9?S1X)*P +#<=CFA90'U*C*T989AUAE7%<ZJR#=3UE >I0@L<FY<3/Y/JDV?&18MXB X9DFG(1:W8F0RW+DX&G,2=:UFSD";
      break;
    case 10000:
      SQL = ")X:49N-*?,17A,KG(5'N1CL=)$:(.>@)AD>H;QAJ#CK2AG;HG#3.YR,04<Q@$,7;&V5+<@7+;?3N+39#@9Z9UTOY;3?O3Y+.)S#1IMW+JC0Z6H+)X-J7N9E1/=0U;VR88A.V6A XM>.8R7MD&6>#-PR,;#B%6=<SKET(<XBVNXK.RF2B%OBJ?&2-:L36S>.&TG<Q=+@0JC3R?%L)V;2<3RK*R5Y9?2J,MEE<F&5%C;$=<SA@1+$33.?'77DZU4&GP#PR :K)QPV=8.Y+,-#)KH9.3L>OWH.G-XB;/W30H2@$71G,(B#M*10F1M.1%AO=AO.7ZG?#KARZ%M&E1G=<I9;TCAWPN9,X0H#O#>DCWCA4 .#,WO,2P&*S3-M-K67+& $5*OB,X0JV#31*H5-A)/*;JUY5:)0,83X+23%KL+I*6KV&YT<98BH6)(WFD-73NB$'RVQ$&,8.:/&QSQLV>ABC#>&X/=>ORF:WM+=OV,$W1,F5<#I3RH5G$;LVUUXN;B(9R4R&9;;@T/%#.ATWW8'XFJMBI2E N4DD4)Y+K5O)HAWTP3ZJ(?/1>#3Q)')0TD:L'TB(%PLZ07*4*PTYKA+PSN%94P0I:H-/14J?1;PX.1$;>IN8,2<B6&>6HYQHXIA@&W ;3@;=1%EB<(DQHB>+<P5-,:9:JKAN@OSL#KM1*0'>.8/+B$P/0 PMG1QYB6Q$* 5*S'-DKY59:P)(3/4H.)@CZ5@WK@X>*>>),'+N'YSG@(-LY;:23?Z.NKVL9#Q*0*H(C@S8L%>IAFOY/DE(KY*+(?%,Z'F>WT T AJAR/N GD2 1$RD?0Y5.Z5G+D'00WWD1F?E8<+YI#M)%;';@, VC.EYJ?'I=,&#>)891@U6<7>DUN=( '*.@C(=2X,R)5NI1IUMAOBG7KH:F8XV?W.?R/EAY&L/>;5Q'T9:YP ?4/#IXN9>*;F;(UPVZX8A$'QP7BA5:P+70))L>7(<13T12>>/,D-*V:=M2GP3Q$D6) A0E1* T&G=&M5SB0CJ3LNJ3U#8@PWS1MV:XL>*S/0)S?W8W 'YZM,:SAGZ/7,''8JUIN8<$?V %3S08#9,Y?AQ.OW8F<3 ?KTE/N88A;->&O<HWFR/CK6J03#UFX+@H%S;YF,$SURV@0 F90O/DO.=/':%N0<H5G@'SM=JNOU7RX>=*A1T*UB=D5J;7Z3BR2BTL'%80, ;O2,S=AC01N7F?6+1%H/)>F>QEYWS8RAB%AWL#=Q9WJCNXRH?TN+1:M $L:ALT 7F+@UG=Z ><2G%&<XVJ0*K# 6F(J0$UBG8*WL#-7@1G&<6KKQ-4/,2M.K0<ITJDAS-EE'UEM*MXKDW>&BO'8)$RXT 4K$*B48Y%/18#=S7.X+Q'S:>OO//M5((,V&RJYA'-.?UYS8$.QG-GQJJ@,XS/V7+&Q,Z7/EK4>E-E34%C@PR1Y/B:'TQ4#/T-FGP)QZ7DS=5,UQ7S-<# 7I(T8<M<*07C=,7%B5,S# <JV)8Z:EL&ADCK,.9D-6421*V=JX/O'2R7,==74&RT2GL3Y+ZE7(1S)XWEB)*,Y1U?;B8OT0 B659N5Y FURD;:WPH#/*L89X%BH>0NC64/ SZVUO0NU>*ZTTDXYT2@+B>O4.C:RC7F3,30?5-7W1/@'7 Y1,/YI8T6U,DR7::D5RJF2<GX;&R0WA:PB$G?9@%-O8U5;JG@V2426?4& JIJO*SHI=%+ET I/# M;?D1.>B*R(GSD/; LJ@W<ELB7'-$H PEX4Q'$D@DJ(2:E>('1(CV6$SJ9H.9-WY(E1MMO#X0V9>CD<DMA,=O':L7</W7<R98)2: E(F9W-CETB/+E'3E9K14'Q?X/,AE%A0X/RATK1/;E#BIH/;2)?2*WF2:6O'90KN%:0J&2H7QPA ESK&&709T7>/WU;0;39Z3SLQ#0PP%CC0HW.':CA*/*CG&#Y'<EB,K4T#)@+QJ;Q:Q/>'%Z- 00H=@AY)H%QP$.9;6Q'LK1K1U<C@AA$XJ?ED$(09#(DCO.I+=T8G.@5ZB+?0WSE',%R3JB/V <083W,-*JVCO#8$Q,T<7O:>M+GLB@SIVJ>Q%$.R=UR11/CCVPFM<5X$ :TB7&A>/:*5R7 8O)+$3-N?7+R .-F/T1NHR':U'+%%1WZ6P.8?PM+R X<&)4&6QW<H9EC#,SA<6NJ4PV<CJ4M;,X#L*R@N5?<L7B=<&SE@OT8NB<:?@U0FGE4>' K;-3&3Y$X4+00'V<7 ?B.-C@MHW3C5X>//#W4KO1SWL7T3WUX=>A3N,%;R2PBW*W8JO+:L1%SW&(HX;@ 57KFGK,UFZ551*MY7D03C*5?WG'98W4MVR:$)+()EG7)@I%NQ)Y RO5X1O&ZBU$>ZCOI ; >%U*UZM,RIQ)$L917F,SRO<#9E#:+K)+YN.5S6C0'7BI)%J/AZ(=N>A #EAK+9KR.C$, PXKY&VX1PX6949N(JUUPT=8V(4,,E ?0;I3I# 8QY6VZ;IL$/#CJKT?V$ 6Y=W<J#7#VE9%6 3V<OL4K6Y0<D%J$#/O;DKW;=C#1(LJI'@W<>BIA.8%<Z846.U5&#00PGQSGNJ<+6QZTW:;<S(>6OL+5A2'**=$X<GM3I&NV$*5DE?0Z+2?:/5%E='$,RSYAAL.#C9CI%0&I4B4250M23(X?8):YVD,,6;2*?X4M$SEMYL#60Z:AA*MMU*P1DB8/'W'%7;L$;KW77,-CN*,.7UO@<GXRG>O@ZW'TOFDU <C>>*&T>XCQOB5WVC71=MWE*BLV%#@',P5<E@JF=7LO0QX#=CI0Y6,LL8%&9:T>Q.%,O -RN96B*/CN8<NOIXO89X:Y600Y>BN;CC#P<V=8BU$I2K@OCJKMC1XTS*J>%KDGNTPWMJV?9/FXG6A&&JA#G.T?YV#N/05(*#I-2&4319,F9N@E,W1OOLU@.A:N)3Z8<C9/-ZA28-S7-S0*C DU6OOF6A6 >MI<D/)@Q=Z&X?.ZS& AW.HM2(0U83VF5:0U7-6L(3>S%H7G4W'AIU%QN /@Q*:>4WK65PZ(,#O-->E<+728*OLJ%G>8+>R.9-94@M)K*-HZ*1K0(,=OWT&0'$Y(<%Q#UR*UW#M.$=+E**6O$8NT/ED<Z@-%%J;(47?IL'0X>GQ%>D);J;6VC6>YZ I(=8'MX0)4Y9<Z+I,(/ J$FH4%5O@HPI<ME4:OC%QDKT=O&7#/JR 1TN(4RMI+PEF.3U(37+R)V3BKM;#/+5P<QHA*QCFH)54NS<3.93.W?H)1$$;?@LE%89=M(23)'ED%Q 8W#2O$160..0<U6OYIOXJ'#<LLDD7)TA.MK-K%PJDIAO?#IOW)M/D1%$2HG#,#RW0Z0Z$$SWT=DGT8*,%OV333L4ZA1LT=5Y;/BP% V%Y%1%E0)X+IQ@9(K<AU13DPP@>>M=Q?V9+?L90M,UPCQ,IW@%>2UA&(TRZRVI)<Y3#%X /-4Z@7VA2E8O-,)N<5#1G@+:CS=JP?OTJ-X9W%Z6)D8@V=KC37GGXI0UWMM#9XU;ZL'NE#-&8'7=HJ<KP+UNEX;E>P9,#3>OF*:3IXD@ON4 QJ0BF/ )<@M40S/+NYOG:-65#*QYDDJFB& ?8I%DV6VO.ILDKH1F&QV?&>K0R-HNL<<;CKP2;.'GGPUG6S>QZNY6DDNX43MENN(P2%@.NAKUDSQ+X(4H-2<@0NF3-8&P*=X.TCZS+%I>(<NI' YJ%SDE68X 7H64GX8#44MH?:X6MZ/ADJ7&PT,TM9:>&Q(AFU@SR#8-1DQW=JV?4F+PP?0Q=?#R*F:#QS6J,7W%H,?OL(5G1H77FRXW5$Q,:IEN4WE2H5V,%Y,5CI%M9@(/G@YV LRC>XTZ/VQR1XGAZ'Q9E' 8V$I9Q+5-%RKUVKZUO%/ $F@?9/YC$'.'>LZ5YR1I9<T@%F* BJAQ35SCHL@4RX<N(%4CPHC9&:QEXU,>N(LN*I@*9.81)A7E4LR7?::#2R5XN@0>=2C=0H3N1REZ?@+CAZA:Y8?FN;QH=V3/$K>,Y1+#G;T@7.+V$-X>Q8QD/391A#4.,XE1&7/PJKU5&HC7XKPO'QZ.' .$XFM4TH%>'IE(?32B&0@9XU3+N5Q HZ=**@V'&HZM39X1,>Q(M#B2W')7N ZKZIH#6Z+GFDDO%ZB.K0ZCZ'7X+VQJ>;GVI8O>T3U5$WPGUTL&YQH>MIU7 :WZ)EF' /BLIN9G(DI3(D+N%)=X<KBS93>MN /1DPFH-(KNZ%K)<O;%333M3 WZU>L?JRJD,2GB-;(+6:G TK?,6G4B7%=)'KR5S9V9SD<IP/TB)6=W()KYZ7 &ZGDR=;TA8BW?64'90SC#NZ8U,-UD75V$E$XX-2W.7Q#L%QAEA*R/1PVL$=A:U,S+6J HT$*AI? YYM+F5NT0 8NPIB3<2W/'Q*$B=67UADI1 (BR*@H(Q>1A:$;95&HVD1$5P<W.)RH#DCSK-LHJ)8)I<W0$B/<R5-DGMT0BM+,Y<$6(9Z@IEX,&CLQ>,2#R=V+TT*&V7;--B<==.,3;#W8OC?Y'/$T,BR(27CKJN-V$-%EQ>T6VFL)/4#8YSI+LFYDTLS=4*) R):,AX(YDY/676XRM?88VLE-/O/N6&-%#/6:#S,E9%:N59$$3OW6XTQQ K:93XOYE-K08$*>HR=-33394WOGK<1LW/;$5BC:=/;O.:I3G P2<F<NG)DW:HK&?(UL)UHD:F=<.WH&+EWM+VUOJJ74D<52B479L-M/$NZS3%0A>0)+D+5<TCKZK0);A93BK61(&XMRG8)&:90CUV.+O1U3RYPT0HARQ$DHNTT.7R9Q'ZWJQ$5&%0W3E1%(6WR.4+;4(XS3YAG(OE$T;#M9XY' FLG#2 Q<(13:J<%N6D;I&<QX4X(:>=@;+V%G#W0A+L9$HP.C2UR9WPM*T+=&OCKY6L6>Q8&PG2FL*B 8$7-:F?1'I,D6L%?0H8UG$EH @ :D+(TP7/<?A?SEDVI% 2L%SCH*-Y'0N0,-*>S>XK3Q-&'(37A0(%T0$IE5 <YF*O-CSAI?(84M6$8('1PCS%Z:'QDG6).9Q6,BPQOU$JP'#'ESH#08H.3.A&T-Y6T8=#VI2..KT)M)X'1VYR@ <:7R41@R.12@$5ET,/4&U9/QS?QPF>$LQ(F+X/<M>VK(,P5Q:0TY?F;D8CS&3;++W0*/T2'0-<,DD-N$'XNB;?$'%-:2#HD>#$1 8;QUUMPN12IV<FB<:=XG-8'SCI%*NR;I;:NF*#-.0X /--@4SP'G=>0AY<GPAF=WFRN8=;-BK<6AX$2+CE%LHO,>QNJG)L:GWJ;@DH-9) NW=L$?FA=BV)O+.U8N-W91T?*Y#)H&1IV9;X57A&06;QYX5?$*H2K(X;V) *K2LE1BLD58@SO'AVYJXHQBTLAN=Z;?J3MN<T.MJN(H#4A&5@6*I/J'>U/7Y&/WD6G$-$;FQF*9%7R=#=Z8'PYP)BW<MFV%O ,O5%BPS++B4E24-XH *-+?Y97<' 9U8I9MF*4&&V#WI6#.FU+YMW/:E?6KPB9:E?KG;A7T51MZ8DO*>HM8WT.TRYQ=C5C)57$;;%Y9CSQDM@)3(>.9X&W&;AXZ5.8CWT49=-Q%MOY28NCHS=YP>*-P7OZ8I7QJII%BI+17*#>Z@X2FWGCF>LA=KTAG'>1-JIO-BF6G7:7'3=MDR;U7L<05R$/GBAT+0G>8B?%JY4'=&KJP6MK78,+TJ7VSQYQDV.FE+FO@HO;V$=I+EMI-E,Y9Q0EF6Y<&.NBC<X9?RQ N5.E-3#;UQAKDI=W#&FC2#/@IYS$KK2,KS):QBKNGM=JSC$GZ:M=>5(?C6K0)/OEA8P6>QP#Z,L-)M1Q3,U%RP?'V+SW#L1,/M2 26(2  K5/U#O>0L(GI@/:<:-Y?>X?%%8E&4$>K@AH9AWX/$Q525 NZ0F>XF'X<G&X'U<XHQLNNQYG#P*MIUVFN#H$2)G>P>ZIQH>6##LJ1XR$F8 ,1)J00O9II9&T9-GE?5:)DI'Q><6'CQ0*.PO527)@U*ANQTQ=?9TZ  <VY?-('VB6LU6U9NJCCKY=D6>;;9:W0NM9GZ1-0&/#Z1/(X5WPX1'P3AWNO7O-/5=FK5INFO<F74<KYRZCXGZ'#C8UO@F,63$TVM(?RPDY8)/Y*32,.G+V8),Y'22R,X?'G28WH>GY9?%M-6Y6')?'G31,TUR%WD%O4DTZO->F;WLL8('187/9AZG;DP@D>&9:SXO$F:3*,)H?R'7BI)PJ%3XE9&( @54W&;SB-K'AK&J3?>.=3E#R2#2T5:LTV?R3*R.RWC:9S5F%9#HY+EB95Z*CD1;)4B  >(Y<KZ;Z:6>ZE:) 1N2X0=-4R3(1/4Z5+P(6&Z+((LI=&B( S;I+GJ1PRZLI&V*FH, )<D9L-/-(TR6,F3IF1=LFA1$=J 7)'P(OYX.6PG)JQ%GL#Q6K*O5<H0-2Y?# ?B1WT9%S/@IAXSPHLG0.HX;1KCSJMJ.V<7EF,'QUP$>1@>%0NFDFTFF U0X#2M:6.I&,T4MN$&G&*7&2YZ @.?3.1B?4VU)>RX'(Y TM9#B-.UALZIFSD@6KS?-9WLB@SY8/RK5F>B@$P>-'B$LAB:($3DWZ3R5;03EQ1#OT9>:)88(Y-HCWLVEG:E'<Q5TQZ@DAIIA3T&EB68C.@99XR&VRYSR=6@JLK0(+P%N31T9OH,N<8#A P=XS5;'I,EW:)BY'N4:UOW3P$Q3NC3C#?U2'#I#O>$%.X$Z+1:+-Y9C<(-F7YBU'A>XB?:P-KPI,*XA0.LFE3 U3EG,/.(5/&-VS@+:R=56M>MG-%RFEU8 3DF#FF?S;GKX,VB;5&RCQS=ULT,M<.GK%7/#X@;D;O0=P;0<VA+O>D;8G.M *.MR>,5'GQESI&R@8+;Q?WZ$R&05FC$RAIK1,TD@  GD.1ID3YM<K#;KV ?:P&8J*WLD1?W8LF(I..HZOWL,)A3YP?D-3I5?JFT,Z(=/%4RH%.,&Q<'-U+16D)=IGX@:.:;B;L5 *R,LV(=7UKB%>$;LZ<-UTA($,JCNC7%5#9%2 O.#MF&4Z'=L9:T'9M%52XN%S@WJO2H(;J9<LR:'I(J7D V:X,L>,#TU%@LZ9UC&T%45;I6N1+ETNFRY=K.W847HZ>A<3,598-W0CEVH, XJA:>-N/.LG2Y<F==2B1=<=B$<V.D2N7F-JE#U*@X=F6B 88O+G6%5?8D.$J8K)C(7P/.*(UR(?S-+-DG6F5(3@A Q%B8PZL=.V+AC:52X&? .>P+IA./8WXV@LF6FYLV7<<V'7ACS)1G*P*.TEZ'@ QUU RNVRAFTFTW>2,+6OW$H=T'&%Y?DR-'LE>0C0$BG'9-NNW*0YZT;91M7@M @11F&R1Y7Y?2')B B/6LTD41WB&QZO1-A?+V+00N#=K+T3;QXLZRK? %-#%=#=FSQ37OO-%SJSSTE7SC2E1K8<*.5.TK97)SZLGM?,V-T&.QA<>.&D$;,RA@',HVZCD<)658'3>KEZ$-P,S7Y 00;7X@CYPXBNCV3.($'YX1/VSI?1?-9@@KX@=42'@ 1(*KQ?/G'BJNJ4 'S2XEOLD<N6-H-Q7ZOS:+$>94-<D2NY/Y,;J6#@U<9S:)43<G8'<A6MUPSFP'I$(2ZMWVF>0B@#LA&S3ZU=;8A9@BT/N3&7XSL*7V7CCBI+8L20T/UMM@KA7;Q(JD?@K,I(8SSQ1/WE<.& R+1D2T:YH9B?7IC  M&B@(4ZAYFOJXMBRWCI(IDJB055L9&6,++GCIDP&TN9'DX%TC$5/,Y246H*@K9N#Z+RG  $&L7.K7@SY.-/3S6?9,SA8L%J@O*X)W+*20-@QA2@H+BWH7(H4-S /U9QFUZ;KCK-QJF&$+M$ZES@YEM;4KU7609YU3$XI%$I+<YEKY1QW;Q&2Y.)R>$;R'MG4H>8AE6P<ZB8-S&J'>62:F:QBE'':WMT9HU,VBKXP9(RV#'JOY) #:S8>KV/A9NVZZW&RA<-#E$WY2Z9?.H5P%#7BO?<=.=+W67 V>J1E?A(X'?=1;'207R68P$Z6 9+$/)5AIUW)C-7>++JD?MBW.P;W*Z,%>VLOIO/R(&=48Z0D04.0Y?T1**S(6*+76E$)#%@7V>>KM:O0=7WQ0,TI&DN85K6TQW@3JMR3<F473Z8$?#-8@/&MLEZS(D(O:?/9%G:-7?AR%ZPWHGW6F0ERF4)PFDIV'*>@RV3T>NOK)E@ ).V&YTK&V7HWJYTWX&L><.T:U>D4W E>41B56Z@*SN(*H&4:>&6@F<#U(KI4DI7UY2#<BHDD1@JK&UN),M(D&2-$4(RHR@T?LTPX-1@',*ECUHLN6;:96:TY/JJ&GRD,WEMROIC23,1)0I:5O4+MNO>Y$#U/IF3TG9/F@H6(SW#>?7DL4+IOM.X(D6K#UY;DPRS@H7NO.#Z3R283#,4P<6''.P6%$>P ,Z0%8::3NE0*G:92V?)4':P,@3684G'SH=-C%&%-'+%EUGHD3ZR+(J$ ?WH9$S(EXXC/G709SHA6+FMB,5*H(IHU42);U;:U+U>@<U#3+WM<$W70.>Z)XO61$%J:?/BA/AH&WX0EKV'<DK<,M2JVCBYK*#>G:BUQX%:;6ECOT&@ 3/>#XF./F/LR'.I)Y;J8?X$4UCL!?NH?<12G(WF1R(B/6R AF $JZHI8@#Y(BVUQ+*0W:<A@SX@/HRH$9;P%D8VG-P< 3ZE75)4A0;8=*-<&((. HN'/O1W?=9V*05:5I4T92)I:C#2+70TIV('&0,>A,S=@5/<)DU9I#+SX;N 6$L :?<T)1;T)EV7>7ML@C1>/6JEE2 GP8Z8#@6G08E J'FN(D6 %8-GKSU(,4$X+SH5D+K=;2;',MXP8XQ7+N-PM A7A#8>3S5<M KVFB%7*/JH,FVK0T4IL(4:J0E1:21ZB?D3XQ=6;T:FRWT!?FZ>Q*N6M'>LGB'2%8U:>.(K>I>%(:Q>Q5@)PQW?=M8=7L(8X*XT/>SB2EMAYM#,UYQFU2O7.02.5PEZ2.&(+P@*Z>WY'8 MYXKQIP.DAA.Z8=>TQ?%W55 H-DE4-4Y1?$R.N=2#G0492)VIEVN&?R?<3HS=/J+7R.XN!? WZ:LI35P$EI1M1#;WSIYM:Z1<*Z+QADS=KV4'4'+U''</=FX4U$ S'U+IX+0KK,Q/$YL*QC<Y<WL/352J1(?W#&NZAMG.3*/V%%4PZ8ZWEW;-FD1#'JKP8'EK#3I$OM%=QWY.-*5'.LN5.JKJ(XUN+M$ABU1.1,=7-@BRJ5#5W&9@P#SJ P5#*N10JD=ZDLBZ;6D/2/'<5C&Q#CQV3ZW-)=ZKJ61,'83Z%6(:P=XD/4 A NA#'U>1;;FB?1D3KD;S24>7.>?691'=YTCV//G<&,BR?'/*$XI+J:G;' BL#$36K*(KDJ8D'UI+2QGKS;TZX%GR@'>=WJH)EP;-ZVR.?HL<94INMTF3>Q8?:OD,H9I%4WN,($TH</:TJHTRV94X-JZPTE0P*G>FJ9.W9L47VIW03*T-M3W?V@B#MRR=Z3%3=65E2)*1>GK,C%*.FZ5F3VA2I1 GG$3 MLO67=GB*5PXLH)*7#KI0 3TJ0KN>B/(:,I4?C=%,RFRU/+,DPNLKS.;2<X3$=C6/EN&Y1 ?/R;@7='U%+ZBSO/+JT#+6AZ20IGSKS%+Y3.Y7A#-AS$<?.F;,L &YH.?()'5GP3L730XK0:#;+2?GY/A3L?9$'2PNZELRWX>SYJ@N8JE'+K$BOT:@9PC-4K W$%#OEJ+<&/3+V7.GTFUT;F*SJ/DEUDVFX7Y?D)+2U(6;5IN/TU. (&6<0&LC,K3W%H/-GNU0$)H*+<$Q*SL0YI91QU.%VRB5#7O8C<IY Y@>6.88%O8M@Z#>%2N T>U@Y2/@19V?Z8NRZNUQ.EGE8M.JXT*).=@/F#ED37=0*(,FUN;X2F<N+9<N2<Y6$)30<I.7FNMF6M4DW1>Q%6KYF1M$<EV.%(WPSZWZF-:/PP";
      break;
    default:
      throw -1;
    }
  }

};

class Benchmark : public Script {

  IntVarArray int_vars;
  BoolVarArray bool_vars;
  StringVarArray str_vars;

public:

  static bool sat;

  Benchmark(Benchmark& s): Script(s) {
    int_vars.update(*this, s.int_vars);
    str_vars.update(*this, s.str_vars);
  }
  virtual Space* copy() {
    return new Benchmark(*this);
  }
  static std::string
  vec2str(const std::vector<int>& v) {
    std::string w;
    for (int i : v)
      w += int2str(i);
    return w;
  }
  static std::vector<int>
  str2vec(const std::string& w) {
    std::vector<int> v(w.size());
    for (int i = 0; i < (int) w.size(); ++i)
      v[i] = w[i];
    return v;
  }

  Benchmark(const StringOptions& so): Script(so) {
    // Variables.
    StringVar pref(*this, 0, so.N);
    StringVar suff(*this, 0, so.N);
    StringVar expr(*this, 0, so.N);
    StringVar blank1(*this, 0, so.N);
    StringVar blank2(*this, 0, so.N);
    StringVar pref_expr(*this, 0, so.N);
    StringVar blank_expr(*this, 0, so.N);
    StringVar lhs(*this, 0, so.N);
    StringVar rhs(*this, 0, so.N);
    StringVar eq (*this, 0, so.N);
    StringVarArgs sva;
    sva << pref << suff << expr << blank1 << blank2
        << pref_expr << blank_expr << lhs << rhs << eq;
    str_vars = StringVarArray(*this, sva);

    IntVar n(*this, 0, so.N);
    IntVar m(*this, 0, so.N);
    IntVar l(*this, 0, so.N);
    IntVarArgs iva;
    iva << n << m << l;
    int_vars = IntVarArray(*this, iva);

    // Constraints.
    length(*this, expr, l);
    length(*this, blank1, n);
    length(*this, blank2, m);
    rel(*this, l > 0);

    dom(*this, blank1, Block(*this, CharSet(*this, ' '), 0, so.N));
    dom(*this, blank2, Block(*this, CharSet(*this, ' '), 0, so.N));
    concat(*this, pref, expr, pref_expr);
    concat(*this, pref_expr, blank1, lhs);
    concat(*this, lhs, StringVar(*this, Block('=')), eq);
    concat(*this, blank2, expr, blank_expr);
    concat(*this, blank_expr, suff, rhs);
    std::vector<int> v(str2vec(so.SQL));
    int k = v.size();
    Block b[k];
    for (int i = 0; i < k; ++i)
      b[i].update(*this, v[i]);
    concat(*this, eq, rhs, StringVar(*this, DashedString(*this, b, k)));

    // Branching.
    lenblock_min_lllm(*this, str_vars);
  }
  
  virtual void
  print(std::ostream& os) const {
    sat = true;
    for (int i = 0; i < int_vars.size(); ++i)
      if (int_vars[i].assigned())
        os << "int_var[" << i << "] = " << int_vars[i].val() << "\n";
      else
        os << "int_var[" << i << "] = " << int_vars[i] << "\n";
    for (int i = 0; i < str_vars.size(); ++i)
      if (str_vars[i].assigned())
        os << "string_var[" << i << "] = \"" << vec2str(str_vars[i].val()) << "\"\n";
      else
        os << "string_var[" << i << "] = \"" << str_vars[i] << "\"\n";
    os << "----------\n";
  }

};
bool Benchmark::sat = false;

int main(int argc, char* argv[]) {
  int n = argc == 1 ? 5000 : atoi(argv[1]);
  StringOptions opt("*** SQL ***", n);
  opt.solutions(1);
  Script::run<Benchmark, DFS, StringOptions>(opt);
  switch (n) {
  case 250:
  case 1000:
  case 5000:
    assert (!Benchmark::sat);
    break;
  default:
    assert (Benchmark::sat);
  }
  return 0;
}
