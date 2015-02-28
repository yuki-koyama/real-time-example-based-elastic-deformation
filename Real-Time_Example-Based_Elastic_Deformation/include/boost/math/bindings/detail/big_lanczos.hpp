//  (C) Copyright John Maddock 2006-8.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BIG_LANCZOS_HPP
#define BOOST_BIG_LANCZOS_HPP

#include <boost/math/special_functions/lanczos.hpp>
#include <boost/lexical_cast.hpp>

namespace boost{ namespace math{ namespace lanczos{

struct lanczos13UDT
{
   template <class T>
   static T lanczos_sum(const T& z)
   {
      static const T num[13] = {
         boost::lexical_cast<T>("44012138428004.60895436261759919070125699"),
         boost::lexical_cast<T>("41590453358593.20051581730723108131357995"),
         boost::lexical_cast<T>("18013842787117.99677796276038389462742949"),
         boost::lexical_cast<T>("4728736263475.388896889723995205703970787"),
         boost::lexical_cast<T>("837910083628.4046470415724300225777912264"),
         boost::lexical_cast<T>("105583707273.4299344907359855510105321192"),
         boost::lexical_cast<T>("9701363618.494999493386608345339104922694"),
         boost::lexical_cast<T>("654914397.5482052641016767125048538245644"),
         boost::lexical_cast<T>("32238322.94213356530668889463945849409184"),
         boost::lexical_cast<T>("1128514.219497091438040721811544858643121"),
         boost::lexical_cast<T>("26665.79378459858944762533958798805525125"),
         boost::lexical_cast<T>("381.8801248632926870394389468349331394196"),
         boost::lexical_cast<T>("2.506628274631000502415763426076722427007"),
      };
      static const boost::uint32_t denom[13] = {
         static_cast<boost::uint32_t>(0u),
         static_cast<boost::uint32_t>(39916800u),
         static_cast<boost::uint32_t>(120543840u),
         static_cast<boost::uint32_t>(150917976u),
         static_cast<boost::uint32_t>(105258076u),
         static_cast<boost::uint32_t>(45995730u),
         static_cast<boost::uint32_t>(13339535u),
         static_cast<boost::uint32_t>(2637558u),
         static_cast<boost::uint32_t>(357423u),
         static_cast<boost::uint32_t>(32670u),
         static_cast<boost::uint32_t>(1925u),
         static_cast<boost::uint32_t>(66u),
         static_cast<boost::uint32_t>(1u),
      };
      return boost::math::tools::evaluate_rational(num, denom, z, 13);
   }

   template <class T>
   static T lanczos_sum_expG_scaled(const T& z)
   {
      static const T num[13] = {
         boost::lexical_cast<T>("86091529.53418537217994842267760536134841"),
         boost::lexical_cast<T>("81354505.17858011242874285785316135398567"),
         boost::lexical_cast<T>("35236626.38815461910817650960734605416521"),
         boost::lexical_cast<T>("9249814.988024471294683815872977672237195"),
         boost::lexical_cast<T>("1639024.216687146960253839656643518985826"),
         boost::lexical_cast<T>("206530.8157641225032631778026076868855623"),
         boost::lexical_cast<T>("18976.70193530288915698282139308582105936"),
         boost::lexical_cast<T>("1281.068909912559479885759622791374106059"),
         boost::lexical_cast<T>("63.06093343420234536146194868906771599354"),
         boost::lexical_cast<T>("2.207470909792527638222674678171050209691"),
         boost::lexical_cast<T>("0.05216058694613505427476207805814960742102"),
         boost::lexical_cast<T>("0.0007469903808915448316510079585999893674101"),
         boost::lexical_cast<T>("0.4903180573459871862552197089738373164184e-5"),
      };
      static const boost::uint32_t denom[13] = {
         static_cast<boost::uint32_t>(0u),
         static_cast<boost::uint32_t>(39916800u),
         static_cast<boost::uint32_t>(120543840u),
         static_cast<boost::uint32_t>(150917976u),
         static_cast<boost::uint32_t>(105258076u),
         static_cast<boost::uint32_t>(45995730u),
         static_cast<boost::uint32_t>(13339535u),
         static_cast<boost::uint32_t>(2637558u),
         static_cast<boost::uint32_t>(357423u),
         static_cast<boost::uint32_t>(32670u),
         static_cast<boost::uint32_t>(1925u),
         static_cast<boost::uint32_t>(66u),
         static_cast<boost::uint32_t>(1u),
      };
      return boost::math::tools::evaluate_rational(num, denom, z, 13);
   }


   template<class T>
   static T lanczos_sum_near_1(const T& dz)
   {
      static const T d[12] = {
         boost::lexical_cast<T>("4.832115561461656947793029596285626840312"),
         boost::lexical_cast<T>("-19.86441536140337740383120735104359034688"),
         boost::lexical_cast<T>("33.9927422807443239927197864963170585331"),
         boost::lexical_cast<T>("-31.41520692249765980987427413991250886138"),
         boost::lexical_cast<T>("17.0270866009599345679868972409543597821"),
         boost::lexical_cast<T>("-5.5077216950865501362506920516723682167"),
         boost::lexical_cast<T>("1.037811741948214855286817963800439373362"),
         boost::lexical_cast<T>("-0.106640468537356182313660880481398642811"),
         boost::lexical_cast<T>("0.005276450526660653288757565778182586742831"),
         boost::lexical_cast<T>("-0.0001000935625597121545867453746252064770029"),
         boost::lexical_cast<T>("0.462590910138598083940803704521211569234e-6"),
         boost::lexical_cast<T>("-0.1735307814426389420248044907765671743012e-9"),
      };
      T result = 0;
      for(unsigned k = 1; k <= sizeof(d)/sizeof(d[0]); ++k)
      {
         result += (-d[k-1]*dz)/(k*dz + k*k);
      }
      return result;
   }

   template<class T>
   static T lanczos_sum_near_2(const T& dz)
   {
      static const T d[12] = {
         boost::lexical_cast<T>("26.96979819614830698367887026728396466395"),
         boost::lexical_cast<T>("-110.8705424709385114023884328797900204863"),
         boost::lexical_cast<T>("189.7258846119231466417015694690434770085"),
         boost::lexical_cast<T>("-175.3397202971107486383321670769397356553"),
         boost::lexical_cast<T>("95.03437648691551457087250340903980824948"),
         boost::lexical_cast<T>("-30.7406022781665264273675797983497141978"),
         boost::lexical_cast<T>("5.792405601630517993355102578874590410552"),
         boost::lexical_cast<T>("-0.5951993240669148697377539518639997795831"),
         boost::lexical_cast<T>("0.02944979359164017509944724739946255067671"),
         boost::lexical_cast<T>("-0.0005586586555377030921194246330399163602684"),
         boost::lexical_cast<T>("0.2581888478270733025288922038673392636029e-5"),
         boost::lexical_cast<T>("-0.9685385411006641478305219367315965391289e-9"),
      };
      T result = 0;
      T z = dz + 2;
      for(unsigned k = 1; k <= sizeof(d)/sizeof(d[0]); ++k)
      {
         result += (-d[k-1]*dz)/(z + k*z + k*k - 1);
      }
      return result;
   }

   static double g(){ return 13.1445650000000000545696821063756942749; }
};


//
// Lanczos Coefficients for N=22 G=22.61891
// Max experimental error (with arbitary precision arithmetic) 2.9524e-38
// Generated with compiler: Microsoft Visual C++ version 8.0 on Win32 at Mar 23 2006
//
struct lanczos22UDT
{
   template <class T>
   static T lanczos_sum(const T& z)
   {
      static const T num[22] = {
         boost::lexical_cast<T>("46198410803245094237463011094.12173081986"),
         boost::lexical_cast<T>("43735859291852324413622037436.321513777"),
         boost::lexical_cast<T>("19716607234435171720534556386.97481377748"),
         boost::lexical_cast<T>("5629401471315018442177955161.245623932129"),
         boost::lexical_cast<T>("1142024910634417138386281569.245580222392"),
         boost::lexical_cast<T>("175048529315951173131586747.695329230778"),
         boost::lexical_cast<T>("21044290245653709191654675.41581372963167"),
         boost::lexical_cast<T>("2033001410561031998451380.335553678782601"),
         boost::lexical_cast<T>("160394318862140953773928.8736211601848891"),
         boost::lexical_cast<T>("10444944438396359705707.48957290388740896"),
         boost::lexical_cast<T>("565075825801617290121.1466393747967538948"),
         boost::lexical_cast<T>("25475874292116227538.99448534450411942597"),
         boost::lexical_cast<T>("957135055846602154.6720835535232270205725"),
         boost::lexical_cast<T>("29874506304047462.23662392445173880821515"),
         boost::lexical_cast<T>("769651310384737.2749087590725764959689181"),
         boost::lexical_cast<T>("16193289100889.15989633624378404096011797"),
         boost::lexical_cast<T>("273781151680.6807433264462376754578933261"),
         boost::lexical_cast<T>("3630485900.32917021712188739762161583295"),
         boost::lexical_cast<T>("36374352.05577334277856865691538582936484"),
         boost::lexical_cast<T>("258945.7742115532455441786924971194951043"),
         boost::lexical_cast<T>("1167.501919472435718934219997431551246996"),
         boost::lexical_cast<T>("2.50662827463100050241576528481104525333"),
      };
      static const T denom[22] = {
         boost::lexical_cast<T>("0"),
         boost::lexical_cast<T>("2432902008176640000"),
         boost::lexical_cast<T>("8752948036761600000"),
         boost::lexical_cast<T>("13803759753640704000"),
         boost::lexical_cast<T>("12870931245150988800"),
         boost::lexical_cast<T>("8037811822645051776"),
         boost::lexical_cast<T>("3599979517947607200"),
         boost::lexical_cast<T>("1206647803780373360"),
         boost::lexical_cast<T>("311333643161390640"),
         boost::lexical_cast<T>("63030812099294896"),
         boost::lexical_cast<T>("10142299865511450"),
         boost::lexical_cast<T>("1307535010540395"),
         boost::lexical_cast<T>("135585182899530"),
         boost::lexical_cast<T>("11310276995381"),
         boost::lexical_cast<T>("756111184500"),
         boost::lexical_cast<T>("40171771630"),
         boost::lexical_cast<T>("1672280820"),
         boost::lexical_cast<T>("53327946"),
         boost::lexical_cast<T>("1256850"),
         boost::lexical_cast<T>("20615"),
         boost::lexical_cast<T>("210"),
         boost::lexical_cast<T>("1"),
      };
      return boost::math::tools::evaluate_rational(num, denom, z, 22);
   }

   template <class T>
   static T lanczos_sum_expG_scaled(const T& z)
   {
      static const T num[22] = {
         boost::lexical_cast<T>("6939996264376682180.277485395074954356211"),
         boost::lexical_cast<T>("6570067992110214451.87201438870245659384"),
         boost::lexical_cast<T>("2961859037444440551.986724631496417064121"),
         boost::lexical_cast<T>("845657339772791245.3541226499766163431651"),
         boost::lexical_cast<T>("171556737035449095.2475716923888737881837"),
         boost::lexical_cast<T>("26296059072490867.7822441885603400926007"),
         boost::lexical_cast<T>("3161305619652108.433798300149816829198706"),
         boost::lexical_cast<T>("305400596026022.4774396904484542582526472"),
         boost::lexical_cast<T>("24094681058862.55120507202622377623528108"),
         boost::lexical_cast<T>("1569055604375.919477574824168939428328839"),
         boost::lexical_cast<T>("84886558909.02047889339710230696942513159"),
         boost::lexical_cast<T>("3827024985.166751989686050643579753162298"),
         boost::lexical_cast<T>("143782298.9273215199098728674282885500522"),
         boost::lexical_cast<T>("4487794.24541641841336786238909171265944"),
         boost::lexical_cast<T>("115618.2025760830513505888216285273541959"),
         boost::lexical_cast<T>("2432.580773108508276957461757328744780439"),
         boost::lexical_cast<T>("41.12782532742893597168530008461874360191"),
         boost::lexical_cast<T>("0.5453771709477689805460179187388702295792"),
         boost::lexical_cast<T>("0.005464211062612080347167337964166505282809"),
         boost::lexical_cast<T>("0.388992321263586767037090706042788910953e-4"),
         boost::lexical_cast<T>("0.1753839324538447655939518484052327068859e-6"),
         boost::lexical_cast<T>("0.3765495513732730583386223384116545391759e-9"),
      };
      static const T denom[22] = {
         boost::lexical_cast<T>("0"),
         boost::lexical_cast<T>("2432902008176640000"),
         boost::lexical_cast<T>("8752948036761600000"),
         boost::lexical_cast<T>("13803759753640704000"),
         boost::lexical_cast<T>("12870931245150988800"),
         boost::lexical_cast<T>("8037811822645051776"),
         boost::lexical_cast<T>("3599979517947607200"),
         boost::lexical_cast<T>("1206647803780373360"),
         boost::lexical_cast<T>("311333643161390640"),
         boost::lexical_cast<T>("63030812099294896"),
         boost::lexical_cast<T>("10142299865511450"),
         boost::lexical_cast<T>("1307535010540395"),
         boost::lexical_cast<T>("135585182899530"),
         boost::lexical_cast<T>("11310276995381"),
         boost::lexical_cast<T>("756111184500"),
         boost::lexical_cast<T>("40171771630"),
         boost::lexical_cast<T>("1672280820"),
         boost::lexical_cast<T>("53327946"),
         boost::lexical_cast<T>("1256850"),
         boost::lexical_cast<T>("20615"),
         boost::lexical_cast<T>("210"),
         boost::lexical_cast<T>("1"),
      };
      return boost::math::tools::evaluate_rational(num, denom, z, 22);
   }


   template<class T>
   static T lanczos_sum_near_1(const T& dz)
   {
      static const T d[21] = {
         boost::lexical_cast<T>("8.318998691953337183034781139546384476554"),
         boost::lexical_cast<T>("-63.15415991415959158214140353299240638675"),
         boost::lexical_cast<T>("217.3108224383632868591462242669081540163"),
         boost::lexical_cast<T>("-448.5134281386108366899784093610397354889"),
         boost::lexical_cast<T>("619.2903759363285456927248474593012711346"),
         boost::lexical_cast<T>("-604.1630177420625418522025080080444177046"),
         boost::lexical_cast<T>("428.8166750424646119935047118287362193314"),
         boost::lexical_cast<T>("-224.6988753721310913866347429589434550302"),
         boost::lexical_cast<T>("87.32181627555510833499451817622786940961"),
         boost::lexical_cast<T>("-25.07866854821128965662498003029199058098"),
         boost::lexical_cast<T>("5.264398125689025351448861011657789005392"),
         boost::lexical_cast<T>("-0.792518936256495243383586076579921559914"),
         boost::lexical_cast<T>("0.08317448364744713773350272460937904691566"),
         boost::lexical_cast<T>("-0.005845345166274053157781068150827567998882"),
         boost::lexical_cast<T>("0.0002599412126352082483326238522490030412391"),
         boost::lexical_cast<T>("-0.6748102079670763884917431338234783496303e-5"),
         boost::lexical_cast<T>("0.908824383434109002762325095643458603605e-7"),
         boost::lexical_cast<T>("-0.5299325929309389890892469299969669579725e-9"),
         boost::lexical_cast<T>("0.994306085859549890267983602248532869362e-12"),
         boost::lexical_cast<T>("-0.3499893692975262747371544905820891835298e-15"),
         boost::lexical_cast<T>("0.7260746353663365145454867069182884694961e-20"),
      };
      T result = 0;
      for(unsigned k = 1; k <= sizeof(d)/sizeof(d[0]); ++k)
      {
         result += (-d[k-1]*dz)/(k*dz + k*k);
      }
      return result;
   }

   template<class T>
   static T lanczos_sum_near_2(const T& dz)
   {
      static const T d[21] = {
         boost::lexical_cast<T>("75.39272007105208086018421070699575462226"),
         boost::lexical_cast<T>("-572.3481967049935412452681346759966390319"),
         boost::lexical_cast<T>("1969.426202741555335078065370698955484358"),
         boost::lexical_cast<T>("-4064.74968778032030891520063865996757519"),
         boost::lexical_cast<T>("5612.452614138013929794736248384309574814"),
         boost::lexical_cast<T>("-5475.357667500026172903620177988213902339"),
         boost::lexical_cast<T>("3886.243614216111328329547926490398103492"),
         boost::lexical_cast<T>("-2036.382026072125407192448069428134470564"),
         boost::lexical_cast<T>("791.3727954936062108045551843636692287652"),
         boost::lexical_cast<T>("-227.2808432388436552794021219198885223122"),
         boost::lexical_cast<T>("47.70974355562144229897637024320739257284"),
         boost::lexical_cast<T>("-7.182373807798293545187073539819697141572"),
         boost::lexical_cast<T>("0.7537866989631514559601547530490976100468"),
         boost::lexical_cast<T>("-0.05297470142240154822658739758236594717787"),
         boost::lexical_cast<T>("0.00235577330936380542539812701472320434133"),
         boost::lexical_cast<T>("-0.6115613067659273118098229498679502138802e-4"),
         boost::lexical_cast<T>("0.8236417010170941915758315020695551724181e-6"),
         boost::lexical_cast<T>("-0.4802628430993048190311242611330072198089e-8"),
         boost::lexical_cast<T>("0.9011113376981524418952720279739624707342e-11"),
         boost::lexical_cast<T>("-0.3171854152689711198382455703658589996796e-14"),
         boost::lexical_cast<T>("0.6580207998808093935798753964580596673177e-19"),
      };
      T result = 0;
      T z = dz + 2;
      for(unsigned k = 1; k <= sizeof(d)/sizeof(d[0]); ++k)
      {
         result += (-d[k-1]*dz)/(z + k*z + k*k - 1);
      }
      return result;
   }

   static double g(){ return 22.61890999999999962710717227309942245483; }
};

//
// Lanczos Coefficients for N=31 G=32.08067
// Max experimental error (with arbitary precision arithmetic) 0.162e-52
// Generated with compiler: Microsoft Visual C++ version 8.0 on Win32 at May 9 2006
//
struct lanczos31UDT
{
   template <class T>
   static T lanczos_sum(const T& z)
   {
      static const T num[31] = {
         boost::lexical_cast<T>("0.2579646553333513328235723061836959833277e46"),
         boost::lexical_cast<T>("0.2444796504337453845497419271639377138264e46"),
         boost::lexical_cast<T>("0.1119885499016017172212179730662673475329e46"),
         boost::lexical_cast<T>("0.3301983829072723658949204487793889113715e45"),
         boost::lexical_cast<T>("0.7041171040503851585152895336505379417066e44"),
         boost::lexical_cast<T>("0.1156687509001223855125097826246939403504e44"),
         boost::lexical_cast<T>("1522559363393940883866575697565974893306000"),
         boost::lexical_cast<T>("164914363507650839510801418717701057005700"),
         boost::lexical_cast<T>("14978522943127593263654178827041568394060"),
         boost::lexical_cast<T>("1156707153701375383907746879648168666774"),
         boost::lexical_cast<T>("76739431129980851159755403434593664173.2"),
         boost::lexical_cast<T>("4407916278928188620282281495575981079.306"),
         boost::lexical_cast<T>("220487883931812802092792125175269667.3004"),
         boost::lexical_cast<T>("9644828280794966468052381443992828.433924"),
         boost::lexical_cast<T>("369996467042247229310044531282837.6549068"),
         boost::lexical_cast<T>("12468380890717344610932904378961.13494291"),
         boost::lexical_cast<T>("369289245210898235894444657859.0529720075"),
         boost::lexical_cast<T>("9607992460262594951559461829.34885209022"),
         boost::lexical_cast<T>("219225935074853412540086410.981421315799"),
         boost::lexical_cast<T>("4374309943598658046326340.720767382079549"),
         boost::lexical_cast<T>("76008779092264509404014.10530947173485581"),
         boost::lexical_cast<T>("1143503533822162444712.335663112617754987"),
         boost::lexical_cast<T>("14779233719977576920.37884890049671578409"),
         boost::lexical_cast<T>("162409028440678302.9992838032166348069916"),
         boost::lexical_cast<T>("1496561553388385.733407609544964535634135"),
         boost::lexical_cast<T>("11347624460661.81008311053190661436107043"),
         boost::lexical_cast<T>("68944915931.32004991941950530448472223832"),
         boost::lexical_cast<T>("322701221.6391432296123937035480931903651"),
         boost::lexical_cast<T>("1092364.213992634267819050120261755371294"),
         boost::lexical_cast<T>("2380.151399852411512711176940867823024864"),
         boost::lexical_cast<T>("2.506628274631000502415765284811045253007"),
      };
      static const T denom[31] = {
         boost::lexical_cast<T>("0"),
         boost::lexical_cast<T>("0.8841761993739701954543616e31"),
         boost::lexical_cast<T>("0.3502799997985980526649278464e32"),
         boost::lexical_cast<T>("0.622621928420356134910574592e32"),
         boost::lexical_cast<T>("66951000306085302338993639424000"),
         boost::lexical_cast<T>("49361465831621147825759587123200"),
         boost::lexical_cast<T>("26751280755793398822580822142976"),
         boost::lexical_cast<T>("11139316913434780466101123891200"),
         boost::lexical_cast<T>("3674201658710345201899117607040"),
         boost::lexical_cast<T>("981347603630155088295475765440"),
         boost::lexical_cast<T>("215760462268683520394805979744"),
         boost::lexical_cast<T>("39539238727270799376544542000"),
         boost::lexical_cast<T>("6097272817323042122728617800"),
         boost::lexical_cast<T>("796974693974455191377937300"),
         boost::lexical_cast<T>("88776380550648116217781890"),
         boost::lexical_cast<T>("8459574446076318147830625"),
         boost::lexical_cast<T>("691254538651580660999025"),
         boost::lexical_cast<T>("48487623689430693038025"),
         boost::lexical_cast<T>("2918939500751087661105"),
         boost::lexical_cast<T>("150566737512021319125"),
         boost::lexical_cast<T>("6634460278534540725"),
         boost::lexical_cast<T>("248526574856284725"),
         boost::lexical_cast<T>("7860403394108265"),
         boost::lexical_cast<T>("207912996295875"),
         boost::lexical_cast<T>("4539323721075"),
         boost::lexical_cast<T>("80328850875"),
         boost::lexical_cast<T>("1122686019"),
         boost::lexical_cast<T>("11921175"),
         boost::lexical_cast<T>("90335"),
         boost::lexical_cast<T>("435"),
         boost::lexical_cast<T>("1"),
      };
      return boost::math::tools::evaluate_rational(num, denom, z, 31);
   }

   template <class T>
   static T lanczos_sum_expG_scaled(const T& z)
   {
      static const T num[31] = {
         boost::lexical_cast<T>("30137154810677525966583148469478.52374216"),
         boost::lexical_cast<T>("28561746428637727032849890123131.36314653"),
         boost::lexical_cast<T>("13083250730789213354063781611435.74046294"),
         boost::lexical_cast<T>("3857598154697777600846539129354.783647"),
         boost::lexical_cast<T>("822596651552555685068015316144.0952185852"),
         boost::lexical_cast<T>("135131964033213842052904200372.039133532"),
         boost::lexical_cast<T>("17787555889683709693655685146.19771358863"),
         boost::lexical_cast<T>("1926639793777927562221423874.149673297196"),
         boost::lexical_cast<T>("174989113988888477076973808.6991839697774"),
         boost::lexical_cast<T>("13513425905835560387095425.01158383184045"),
         boost::lexical_cast<T>("896521313378762433091075.1446749283094845"),
         boost::lexical_cast<T>("51496223433749515758124.71524415105430686"),
         boost::lexical_cast<T>("2575886794780078381228.37205955912263407"),
         boost::lexical_cast<T>("112677328855422964200.4155776009524490958"),
         boost::lexical_cast<T>("4322545967487943330.625233358130724324796"),
         boost::lexical_cast<T>("145663957202380774.0362027607207590519723"),
         boost::lexical_cast<T>("4314283729473470.686566233465428332496534"),
         boost::lexical_cast<T>("112246988185485.8877916434026906290603878"),
         boost::lexical_cast<T>("2561143864972.040563435178307062626388193"),
         boost::lexical_cast<T>("51103611767.9626550674442537989885239605"),
         boost::lexical_cast<T>("887985348.0369447209508500133077232094491"),
         boost::lexical_cast<T>("13359172.3954672607019822025834072685839"),
         boost::lexical_cast<T>("172660.8841147568768783928167105965064459"),
         boost::lexical_cast<T>("1897.370795407433013556725714874693719617"),
         boost::lexical_cast<T>("17.48383210090980598861217644749573257178"),
         boost::lexical_cast<T>("0.1325705316732132940835251054350153028901"),
         boost::lexical_cast<T>("0.0008054605783673449641889260501816356090452"),
         boost::lexical_cast<T>("0.377001130700104515644336869896819162464e-5"),
         boost::lexical_cast<T>("0.1276172868883867038813825443204454996531e-7"),
         boost::lexical_cast<T>("0.2780651912081116274907381023821492811093e-10"),
         boost::lexical_cast<T>("0.2928410648650955854121639682890739211234e-13"),
      };
      static const T denom[31] = {
         boost::lexical_cast<T>("0"),
         boost::lexical_cast<T>("0.8841761993739701954543616e31"),
         boost::lexical_cast<T>("0.3502799997985980526649278464e32"),
         boost::lexical_cast<T>("0.622621928420356134910574592e32"),
         boost::lexical_cast<T>("66951000306085302338993639424000"),
         boost::lexical_cast<T>("49361465831621147825759587123200"),
         boost::lexical_cast<T>("26751280755793398822580822142976"),
         boost::lexical_cast<T>("11139316913434780466101123891200"),
         boost::lexical_cast<T>("3674201658710345201899117607040"),
         boost::lexical_cast<T>("981347603630155088295475765440"),
         boost::lexical_cast<T>("215760462268683520394805979744"),
         boost::lexical_cast<T>("39539238727270799376544542000"),
         boost::lexical_cast<T>("6097272817323042122728617800"),
         boost::lexical_cast<T>("796974693974455191377937300"),
         boost::lexical_cast<T>("88776380550648116217781890"),
         boost::lexical_cast<T>("8459574446076318147830625"),
         boost::lexical_cast<T>("691254538651580660999025"),
         boost::lexical_cast<T>("48487623689430693038025"),
         boost::lexical_cast<T>("2918939500751087661105"),
         boost::lexical_cast<T>("150566737512021319125"),
         boost::lexical_cast<T>("6634460278534540725"),
         boost::lexical_cast<T>("248526574856284725"),
         boost::lexical_cast<T>("7860403394108265"),
         boost::lexical_cast<T>("207912996295875"),
         boost::lexical_cast<T>("4539323721075"),
         boost::lexical_cast<T>("80328850875"),
         boost::lexical_cast<T>("1122686019"),
         boost::lexical_cast<T>("11921175"),
         boost::lexical_cast<T>("90335"),
         boost::lexical_cast<T>("435"),
         boost::lexical_cast<T>("1"),
      };
      return boost::math::tools::evaluate_rational(num, denom, z, 31);
   }


   template<class T>
   static T lanczos_sum_near_1(const T& dz)
   {
      static const T d[30] = {
         boost::lexical_cast<T>("11.80038544942943603508206880307972596807"),
         boost::lexical_cast<T>("-130.6355975335626214564236363322099481079"),
         boost::lexical_cast<T>("676.2177719145993049893392276809256538927"),
         boost::lexical_cast<T>("-2174.724497783850503069990936574060452057"),
         boost::lexical_cast<T>("4869.877180638131076410069103742986502022"),
         boost::lexical_cast<T>("-8065.744271864238179992762265472478229172"),
         boost::lexical_cast<T>("10245.03825618572106228191509520638651539"),
         boost::lexical_cast<T>("-10212.83902362683215459850403668669647192"),
         boost::lexical_cast<T>("8110.289185383288952562767679576754140336"),
         boost::lexical_cast<T>("-5179.310892558291062401828964000776095156"),
         boost::lexical_cast<T>("2673.987492589052370230989109591011091273"),
         boost::lexical_cast<T>("-1118.342574651205183051884250033505609141"),
         boost::lexical_cast<T>("378.5812742511620662650096436471920295596"),
         boost::lexical_cast<T>("-103.3725999812126067084828735543906768961"),
         boost::lexical_cast<T>("22.62913974335996321848099677797888917792"),
         boost::lexical_cast<T>("-3.936414819950859548507275533569588041446"),
         boost::lexical_cast<T>("0.5376818198843817355682124535902641644854"),
         boost::lexical_cast<T>("-0.0567827903603478957483409124122554243201"),
         boost::lexical_cast<T>("0.004545544993648879420352693271088478106482"),
         boost::lexical_cast<T>("-0.0002689795568951033950042375135970897959935"),
         boost::lexical_cast<T>("0.1139493459006846530734617710847103572122e-4"),
         boost::lexical_cast<T>("-0.3316581197839213921885210451302820192794e-6"),
         boost::lexical_cast<T>("0.6285613334898374028443777562554713906213e-8"),
         boost::lexical_cast<T>("-0.7222145115734409070310317999856424167091e-10"),
         boost::lexical_cast<T>("0.4562976983547274766890241815002584238219e-12"),
         boost::lexical_cast<T>("-0.1380593023819058919640038942493212141072e-14"),
         boost::lexical_cast<T>("0.1629663871586410129307496385264268190679e-17"),
         boost::lexical_cast<T>("-0.5429994291916548849493889660077076739993e-21"),
         boost::lexical_cast<T>("0.2922682842441892106795386303084661338957e-25"),
         boost::lexical_cast<T>("-0.8456967065309046044689041041336866118459e-31"),
      };
      T result = 0;
      for(unsigned k = 1; k <= sizeof(d)/sizeof(d[0]); ++k)
      {
         result += (-d[k-1]*dz)/(k*dz + k*k);
      }
      return result;
   }

   template<class T>
   static T lanczos_sum_near_2(const T& dz)
   {
      static const T d[30] = {
         boost::lexical_cast<T>("147.9979641587472136175636384176549713358"),
         boost::lexical_cast<T>("-1638.404318611773924210055619836375434296"),
         boost::lexical_cast<T>("8480.981744216135641122944743711911653273"),
         boost::lexical_cast<T>("-27274.93942104458448200467097634494071176"),
         boost::lexical_cast<T>("61076.98019918759324489193232276937262854"),
         boost::lexical_cast<T>("-101158.8762737154296509560513952101409264"),
         boost::lexical_cast<T>("128491.1252383947174824913796141607174379"),
         boost::lexical_cast<T>("-128087.2892038336581928787480535905496026"),
         boost::lexical_cast<T>("101717.5492545853663296795562084430123258"),
         boost::lexical_cast<T>("-64957.8330410311808907869707511362206858"),
         boost::lexical_cast<T>("33536.59139229792478811870738772305570317"),
         boost::lexical_cast<T>("-14026.01847115365926835980820243003785821"),
         boost::lexical_cast<T>("4748.087094096186515212209389240715050212"),
         boost::lexical_cast<T>("-1296.477510211815971152205100242259733245"),
         boost::lexical_cast<T>("283.8099337545793198947620951499958085157"),
         boost::lexical_cast<T>("-49.36969067101255103452092297769364837753"),
         boost::lexical_cast<T>("6.743492833270653628580811118017061581404"),
         boost::lexical_cast<T>("-0.7121578704864048548351804794951487823626"),
         boost::lexical_cast<T>("0.0570092738016915476694118877057948681298"),
         boost::lexical_cast<T>("-0.003373485297696102660302960722607722438643"),
         boost::lexical_cast<T>("0.0001429128843527532859999752593761934089751"),
         boost::lexical_cast<T>("-0.41595867130858508233493767243236888636e-5"),
         boost::lexical_cast<T>("0.7883284669307241040059778207492255409785e-7"),
         boost::lexical_cast<T>("-0.905786322462384670803148223703187214379e-9"),
         boost::lexical_cast<T>("0.5722790216999820323272452464661250331451e-11"),
         boost::lexical_cast<T>("-0.1731510870832349779315841757234562309727e-13"),
         boost::lexical_cast<T>("0.2043890314358438601429048378015983874378e-16"),
         boost::lexical_cast<T>("-0.6810185176079344204740000170500311171065e-20"),
         boost::lexical_cast<T>("0.3665567641131713928114853776588342403919e-24"),
         boost::lexical_cast<T>("-0.1060655106553177007425710511436497259484e-29"),
      };
      T result = 0;
      T z = dz + 2;
      for(unsigned k = 1; k <= sizeof(d)/sizeof(d[0]); ++k)
      {
         result += (-d[k-1]*dz)/(z + k*z + k*k - 1);
      }
      return result;
   }

   static double g(){ return 32.08066999999999779902282170951366424561; }
};

//
// Lanczos Coefficients for N=61 G=63.192152
// Max experimental error (with 1000-bit precision arithmetic) 3.740e-113
// Generated with compiler: Microsoft Visual C++ version 8.0 on Win32 at Mar 12 2006
//
struct lanczos61UDT
{
   template <class T>
   static T lanczos_sum(const T& z)
   {
      using namespace boost;
      static const T d[61] = {
         boost::lexical_cast<T>("2.50662827463100050241576528481104525300698674060993831662992357634229365460784197494659584"),
         boost::lexical_cast<T>("13349415823254323512107320481.3495396037261649201426994438803767191136434970492309775123879"),
         boost::lexical_cast<T>("-300542621510568204264185787475.230003734889859348050696493467253861933279360152095861484548"),
         boost::lexical_cast<T>("3273919938390136737194044982676.40271056035622723775417608127544182097346526115858803376474"),
         boost::lexical_cast<T>("-22989594065095806099337396006399.5874206181563663855129141706748733174902067950115092492439"),
         boost::lexical_cast<T>("116970582893952893160414263796102.542775878583510989850142808618916073286745084692189044738"),
         boost::lexical_cast<T>("-459561969036479455224850813196807.283291532483532558959779434457349912822256480548436066098"),
         boost::lexical_cast<T>("1450959909778264914956547227964788.89797179379520834974601372820249784303794436366366810477"),
         boost::lexical_cast<T>("-3782846865486775046285288437885921.41537699732805465141128848354901016102326190612528503251"),
         boost::lexical_cast<T>("8305043213936355459145388670886540.09976337905520168067329932809302445437208115570138102767"),
         boost::lexical_cast<T>("-15580988484396722546934484726970745.4927787160273626078250810989811865283255762028143642311"),
         boost::lexical_cast<T>("25262722284076250779006793435537600.0822901485517345545978818780090308947301031347345640449"),
         boost::lexical_cast<T>("-35714428027687018805443603728757116.5304655170478705341887572982734901197345415291580897698"),
         boost::lexical_cast<T>("44334726194692443174715432419157343.2294160783772787096321009453791271387235388689346602833"),
         boost::lexical_cast<T>("-48599573547617297831555162417695106.187829304963846482633791012658974681648157963911491985"),
         boost::lexical_cast<T>("47258466493366798944386359199482189.0753349196625125615316002614813737880755896979754845101"),
         boost::lexical_cast<T>("-40913448215392412059728312039233342.142914753896559359297977982314043378636755884088383226"),
         boost::lexical_cast<T>("31626312914486892948769164616982902.7262756989418188077611392594232674722318027323102462687"),
         boost::lexical_cast<T>("-21878079174441332123064991795834438.4699982361692990285700077902601657354101259411789722708"),
         boost::lexical_cast<T>("13567268503974326527361474986354265.3136632133935430378937191911532112778452274286122946396"),
         boost::lexical_cast<T>("-7551494211746723529747611556474669.62996644923557605747803028485900789337467673523741066527"),
         boost::lexical_cast<T>("3775516572689476384052312341432597.70584966904950490541958869730702790312581801585742038997"),
         boost::lexical_cast<T>("-1696271471453637244930364711513292.79902955514107737992185368006225264329876265486853482449"),
         boost::lexical_cast<T>("684857608019352767999083000986166.20765273693720041519286231015176745354062413008561259139"),
         boost::lexical_cast<T>("-248397566275708464679881624417990.410438107634139924805871051723444048539177890346227250473"),
         boost::lexical_cast<T>("80880368999557992138783568858556.1512378233079327986518410244522800950609595592170022878937"),
         boost::lexical_cast<T>("-23618197945394013802495450485616.9025005749893350650829964098117490779655546610665927669729"),
         boost::lexical_cast<T>("6176884636893816103087134481332.06708966653493024119556843727320635285468825056891248447124"),
         boost::lexical_cast<T>("-1444348683723439589948246285262.64080678953468490544615312565485170860503207005915261691108"),
         boost::lexical_cast<T>("301342031656979076702313946827.961658905182634508217626783081241074250132289461989777865387"),
         boost::lexical_cast<T>("-55959656587719766738301589651.3940625826610668990368881615587469329021742236397809951765678"),
         boost::lexical_cast<T>("9223339169004064297247180402.36227016155682738556103138196079389248843082157924368301293963"),
         boost::lexical_cast<T>("-1344882881571942601385730283.42710150182526891377514071881534880944872423492272147871101373"),
         boost::lexical_cast<T>("172841913316760599352601139.54409257740173055624405575900164401527761357324625574736896079"),
         boost::lexical_cast<T>("-19496120443876233531343952.3802212016691702737346568192063937387825469602063310488794471653"),
         boost::lexical_cast<T>("1920907372583710284097959.44121420322495784420169085871802458519363819782779653621724219067"),
         boost::lexical_cast<T>("-164429314798240461613359.399597503536657962383155875723527581699785846599051112719962464604"),
         boost::lexical_cast<T>("12154026644351189572525.1452249886865981747374191977801688548318519692423556934568426042152"),
         boost::lexical_cast<T>("-770443988366210815096.519382051977221101156336663806705367929328924137169970381042234329058"),
         boost::lexical_cast<T>("41558909851418707920.4696085656889424895313728719601503526476333404973280596225722152966128"),
         boost::lexical_cast<T>("-1890879946549708819.24562220042687554209318172044783707920086716716717574156283898330017796"),
         boost::lexical_cast<T>("71844996557297623.9583461685535340440524052492427928388171299145330229958643439878608673403"),
         boost::lexical_cast<T>("-2253785109518255.55600197759875781765803818232939130127735487613049577235879610065545755637"),
         boost::lexical_cast<T>("57616883849355.997562563968344493719962252675875692642406455612671495250543228005045106721"),
         boost::lexical_cast<T>("-1182495730353.08218118278997948852215670614084013289033222774171548915344541249351599628436"),
         boost::lexical_cast<T>("19148649358.6196967288062261380599423925174178776792840639099120170800869284300966978300613"),
         boost::lexical_cast<T>("-239779605.891370259668403359614360511661030470269478602533200704639655585967442891496784613"),
         boost::lexical_cast<T>("2267583.00284368310957842936892685032434505866445291643236133553754152047677944820353796872"),
         boost::lexical_cast<T>("-15749.490806784673108773558070497383604733010677027764233749920147549999361110299643477893"),
         boost::lexical_cast<T>("77.7059495149052727171505425584459982871343274332635726864135949842508025564999785370162956"),
         boost::lexical_cast<T>("-0.261619987273930331397625130282851629108569607193781378836014468617185550622160348688297247"),
         boost::lexical_cast<T>("0.000572252321659691600529444769356185993188551770817110673186068921175991328434642504616377475"),
         boost::lexical_cast<T>("-0.765167220661540041663007112207436426423746402583423562585653954743978584117929356523307954e-6"),
         boost::lexical_cast<T>("0.579179571056209077507916813937971472839851499147582627425979879366849876944438724610663401e-9"),
         boost::lexical_cast<T>("-0.224804733043915149719206760378355636826808754704148660354494460792713189958510735070096991e-12"),
         boost::lexical_cast<T>("0.392711975389579343321746945135488409914483448652884894759297584020979857734289645857714768e-16"),
         boost::lexical_cast<T>("-0.258603588346412049542768766878162221817684639789901440429511261589010049357907537684380983e-20"),
         boost::lexical_cast<T>("0.499992460848751668441190360024540741752242879565548017176883304716370989218399797418478685e-25"),
         boost::lexical_cast<T>("-0.196211614533318174187346267877390498735734213905206562766348625767919122511096089367364025e-30"),
         boost::lexical_cast<T>("0.874722648949676363732094858062907290148733370978226751462386623191111439121706262759209573e-37"),
         boost::lexical_cast<T>("-0.163907874717737848669759890242660846846105433791283903651924563157080252845636658802930428e-44"),
      };
      T result = d[0];
      for(unsigned k = 1; k < sizeof(d)/sizeof(d[0]); ++k)
      {
         result += d[k]/(z+(k-1));
      }
      return result;
   }

   template <class T>
   static T lanczos_sum_expG_scaled(const T& z)
   {
      using namespace boost;
      static const T d[61] = {
         boost::lexical_cast<T>("0.901751806425638853077358552989167785490911341809902155556127108480303870921448984935411583e-27"),
         boost::lexical_cast<T>("4.80241125306810017699523302110401965428995345115391817406006361151407344955277298373661032"),
         boost::lexical_cast<T>("-108.119283021710869401330097315436214587270846871451487282117128515476478251641970487922552"),
         boost::lexical_cast<T>("1177.78262074811362219818923738088833932279000985161077740440010901595132448469513438139561"),
         boost::lexical_cast<T>("-8270.43570321334374279057432173814835581983913167617217749736484999327758232081395983082867"),
         boost::lexical_cast<T>("42079.807161997077661752306902088979258826568702655699995911391774839958572703348502730394"),
         boost::lexical_cast<T>("-165326.003834443330215001219988296482004968548294447320869281647211603153902436231468280089"),
         boost::lexical_cast<T>("521978.361504895300685499370463597042533432134369277742485307843747923127933979566742421213"),
         boost::lexical_cast<T>("-1360867.51629992863544553419296636395576666570468519805449755596254912681418267100657262281"),
         boost::lexical_cast<T>("2987713.73338656161102517003716335104823650191612448011720936412226357385029800040631754755"),
         boost::lexical_cast<T>("-5605212.64915921452169919008770165304171481697939254152852673009005154549681704553438450709"),
         boost::lexical_cast<T>("9088186.58332916818449459635132673652700922052988327069535513580836143146727832380184335474"),
         boost::lexical_cast<T>("-12848155.5543636394746355365819800465364996596310467415907815393346205151090486190421959769"),
         boost::lexical_cast<T>("15949281.2867656960575878805158849857756293807220033618942867694361569866468996967761600898"),
         boost::lexical_cast<T>("-17483546.9948295433308250581770557182576171673272450149400973735206019559576269174369907171"),
         boost::lexical_cast<T>("17001087.8599749419660906448951424280111036786456594738278573653160553115681287326064596964"),
         boost::lexical_cast<T>("-14718487.0643665950346574802384331125115747311674609017568623694516187494204567579595827859"),
         boost::lexical_cast<T>("11377468.7255609717716845971105161298889777425898291183866813269222281486121330837791392732"),
         boost::lexical_cast<T>("-7870571.64253038587947746661946939286858490057774448573157856145556080330153403858747655263"),
         boost::lexical_cast<T>("4880783.08440908743641723492059912671377140680710345996273343885045364205895751515063844239"),
         boost::lexical_cast<T>("-2716626.7992639625103140035635916455652302249897918893040695025407382316653674141983775542"),
         boost::lexical_cast<T>("1358230.46602865696544327299659410214201327791319846880787515156343361311278133805428800255"),
         boost::lexical_cast<T>("-610228.440751458395860905749312275043435828322076830117123636938979942213530882048883969802"),
         boost::lexical_cast<T>("246375.416501158654327780901087115642493055617468601787093268312234390446439555559050129729"),
         boost::lexical_cast<T>("-89360.2599028475206119333931211015869139511677735549267100272095432140508089207221096740632"),
         boost::lexical_cast<T>("29096.4637987498328341260960356772198979319790332957125131055960448759586930781530063775634"),
         boost::lexical_cast<T>("-8496.57401431514433694413130585404918350686834939156759654375188338156288564260152505382438"),
         boost::lexical_cast<T>("2222.11523574301594407443285016240908726891841242444092960094015874546135316534057865883047"),
         boost::lexical_cast<T>("-519.599993280949289705514822058693289933461756514489674453254304308040888101533569480646682"),
         boost::lexical_cast<T>("108.406868361306987817730701109400305482972790224573776407166683184990131682003417239181112"),
         boost::lexical_cast<T>("-20.1313142142558596796857948064047373605439974799116521459977609253211918146595346493447238"),
         boost::lexical_cast<T>("3.31806787671783168020012913552384112429614503798293169239082032849759155847394955909684383"),
         boost::lexical_cast<T>("-0.483817477111537877685595212919784447924875428848331771524426361483392903320495411973587861"),
         boost::lexical_cast<T>("0.0621793463102927384924303843912913542297042029136293808338022462765755471002366206711862637"),
         boost::lexical_cast<T>("-0.00701366932085103924241526535768453911099671087892444015581511551813219720807206445462785293"),
         boost::lexical_cast<T>("0.000691040514756294308758606917671220770856269030526647010461217455799229645004351524024364997"),
         boost::lexical_cast<T>("-0.591529398871361458428147660822525365922497109038495896497692806150033516658042357799869656e-4"),
         boost::lexical_cast<T>("0.437237367535177689875119370170434437030440227275583289093139147244747901678407875809020739e-5"),
         boost::lexical_cast<T>("-0.277164853397051135996651958345647824709602266382721185838782221179129726200661453504250697e-6"),
         boost::lexical_cast<T>("0.149506899012035980148891401548317536032574502641368034781671941165064546410613201579653674e-7"),
         boost::lexical_cast<T>("-0.68023824066463262779882895193964639544061678698791279217407325790147925675797085217462974e-9"),
         boost::lexical_cast<T>("0.258460163734186329938721529982859244969655253624066115559707985878606277800329299821882688e-10"),
         boost::lexical_cast<T>("-0.810792256024669306744649981276512583535251727474303382740940985102669076169168931092026491e-12"),
         boost::lexical_cast<T>("0.207274966207031327521921078048021807442500113231320959236850963529304158700096495799022922e-13"),
         boost::lexical_cast<T>("-0.425399199286327802950259994834798737777721414442095221716122926637623478450472871269742436e-15"),
         boost::lexical_cast<T>("0.688866766744198529064607574117697940084548375790020728788313274612845280173338912495478431e-17"),
         boost::lexical_cast<T>("-0.862599751805643281578607291655858333628582704771553874199104377131082877406179933909898885e-19"),
         boost::lexical_cast<T>("0.815756005678735657200275584442908437977926312650210429668619446123450972547018343768177988e-21"),
         boost::lexical_cast<T>("-0.566583084099007858124915716926967268295318152203932871370429534546567151650626184750291695e-23"),
         boost::lexical_cast<T>("0.279544761599725082805446124351997692260093135930731230328454667675190245860598623539891708e-25"),
         boost::lexical_cast<T>("-0.941169851584987983984201821679114408126982142904386301937192011680047611188837432096199601e-28"),
         boost::lexical_cast<T>("0.205866011331040736302780507155525142187875191518455173304638008169488993406425201915370746e-30"),
         boost::lexical_cast<T>("-0.27526655245712584371295491216289353976964567057707464008951584303679019796521332324352501e-33"),
         boost::lexical_cast<T>("0.208358067979444305082929004102609366169534624348056112144990933897581971394396210379638792e-36"),
         boost::lexical_cast<T>("-0.808728107661779323263133007119729988596844663194254976820030366188579170595441991680169012e-40"),
         boost::lexical_cast<T>("0.141276924383478964519776436955079978012672985961918248012931336621229652792338950573694356e-43"),
         boost::lexical_cast<T>("-0.930318449287651389310440021745842417218125582685428432576258687100661462527604238849332053e-48"),
         boost::lexical_cast<T>("0.179870748819321661641184169834635133045197146966203370650788171790610563029431722308057539e-52"),
         boost::lexical_cast<T>("-0.705865243912790337263229413370018392321238639017433365017168104310561824133229343750737083e-58"),
         boost::lexical_cast<T>("0.3146787805734405996448268100558028857930560442377698646099945108125281507396722995748398e-64"),
         boost::lexical_cast<T>("-0.589653534231618730406843260601322236697428143603814281282790370329151249078338470962782338e-72"),
      };
      T result = d[0];
      for(unsigned k = 1; k < sizeof(d)/sizeof(d[0]); ++k)
      {
         result += d[k]/(z+(k-1));
      }
      return result;
   }

   template<class T>
   static T lanczos_sum_near_1(const T& dz)
   {
      using namespace boost;
      static const T d[60] = {
         boost::lexical_cast<T>("23.2463658527729692390378860713647146932236940604550445351214987229819352880524561852919518"),
         boost::lexical_cast<T>("-523.358012551815715084547614110229469295755088686612838322817729744722233637819564673967396"),
         boost::lexical_cast<T>("5701.12892340421080714956066268650092612647620400476183901625272640935853188559347587495571"),
         boost::lexical_cast<T>("-40033.5506451901904954336453419007623117537868026994808919201793803506999271787018654246699"),
         boost::lexical_cast<T>("203689.884259074923009439144410340256983393397995558814367995938668111650624499963153485034"),
         boost::lexical_cast<T>("-800270.648969745331278757692597096167418585957703057412758177038340791380011708874081291202"),
         boost::lexical_cast<T>("2526668.23380061659863999395867315313385499515711742092815402701950519696944982260718031476"),
         boost::lexical_cast<T>("-6587362.57559198722630391278043503867973853468105110382293763174847657538179665571836023631"),
         boost::lexical_cast<T>("14462211.3454541602975917764900442754186801975533106565506542322063393991552960595701762805"),
         boost::lexical_cast<T>("-27132375.1879227404375395522940895789625516798992585980380939378508607160857820002128106898"),
         boost::lexical_cast<T>("43991923.8735251977856804364757478459275087361742168436524951824945035673768875988985478116"),
         boost::lexical_cast<T>("-62192284.0030124679010201921841372967696262036115679150017649233887633598058364494608060812"),
         boost::lexical_cast<T>("77203473.0770033513405070667417251568915937590689150831268228886281254637715669678358204991"),
         boost::lexical_cast<T>("-84630180.2217173903516348977915150565994784278120192219937728967986198118628659866582594874"),
         boost::lexical_cast<T>("82294807.2253549409847505891112074804416229757832871133388349982640444405470371147991706317"),
         boost::lexical_cast<T>("-71245738.2484649177928765605893043553453557808557887270209768163561363857395639001251515788"),
         boost::lexical_cast<T>("55073334.3180266913441333534260714059077572215147571872597651029894142803987981342430068594"),
         boost::lexical_cast<T>("-38097984.1648990787690036742690550656061009857688125101191167768314179751258568264424911474"),
         boost::lexical_cast<T>("23625729.5032184580395130592017474282828236643586203914515183078852982915252442161768527976"),
         boost::lexical_cast<T>("-13149998.4348054726172055622442157883429575511528431835657668083088902165366619827169829685"),
         boost::lexical_cast<T>("6574597.77221556423683199818131482663205682902023554728024972161230111356285973623550338976"),
         boost::lexical_cast<T>("-2953848.1483469149918109110050192571921018042012905892107136410603990336401921230407043408"),
         boost::lexical_cast<T>("1192595.29584357246380113611351829515963605337523874715861849584306265512819543347806085356"),
         boost::lexical_cast<T>("-432553.812019608638388918135375154289816441900572658692369491476137741687213006403648722272"),
         boost::lexical_cast<T>("140843.215385933866391177743292449477205328233960902455943995092958295858485718885800427129"),
         boost::lexical_cast<T>("-41128.186992679630058614841985110676526199977321524879849001760603476646382839182691529968"),
         boost::lexical_cast<T>("10756.2849191854701631989789887757784944313743544315113894758328432005767448056040879337769"),
         boost::lexical_cast<T>("-2515.15559672041299884426826962296210458052543246529646213159169885444118227871246315458787"),
         boost::lexical_cast<T>("524.750087004805200600237632074908875763734578390662349666321453103782638818305404274166951"),
         boost::lexical_cast<T>("-97.4468596421732493988298219295878130651986131393383646674144877163795143930682205035917965"),
         boost::lexical_cast<T>("16.0613108128210806736384551896802799172445298357754547684100294231532127326987175444453058"),
         boost::lexical_cast<T>("-2.34194813526540240672426202485306862230641838409943369059203455578340880900483887447559874"),
         boost::lexical_cast<T>("0.300982934748016059399829007219431333744032924923669397318820178988611410275964499475465815"),
         boost::lexical_cast<T>("-0.033950095985367909789000959795708551814461844488183964432565731809399824963680858993718525"),
         boost::lexical_cast<T>("0.00334502394288921146242772614150438404658527112198421937945605441697314216921393987758378122"),
         boost::lexical_cast<T>("-0.000286333429067523984607730553301991502191011265745476190940771685897687956262049750683013485"),
         boost::lexical_cast<T>("0.211647426149364947402896718485536530479491688838087899435991994237067890628274492042231115e-4"),
         boost::lexical_cast<T>("-0.134163345121302758109675190598169832775248626443483098532368562186356128620805552609175683e-5"),
         boost::lexical_cast<T>("0.723697303042715085329782938306424498336642078597508935450663080894255773653328980495047891e-7"),
         boost::lexical_cast<T>("-0.329273487343139063533251321553223583999676337945788660475231347828772272134656322947906888e-8"),
         boost::lexical_cast<T>("0.12510922551028971731767784013117088894558604838820475961392154031378891971216135267744134e-9"),
         boost::lexical_cast<T>("-0.392468958215589939603666430583400537413757786057185505426804034745840192914621891690369903e-11"),
         boost::lexical_cast<T>("0.100332717101049934370760667782927946803279422001380028508200697081188326364078428184546051e-12"),
         boost::lexical_cast<T>("-0.205917088291197705194762747639836655808855850989058813560983717575008725393428497910009756e-14"),
         boost::lexical_cast<T>("0.333450178247893143608439314203175490705783992567107481617660357577257627854979230819461489e-16"),
         boost::lexical_cast<T>("-0.417546693906616047110563550428133589051498362676394888715581845170969319500638944065594319e-18"),
         boost::lexical_cast<T>("0.394871691642184410859178529844325390739857256666676534513661579365702353214518478078730801e-20"),
         boost::lexical_cast<T>("-0.274258012587811199757875927548699264063511843669070634471054184977334027224611843434000922e-22"),
         boost::lexical_cast<T>("0.135315354265459854889496635967343027244391821142592599244505313738163473730636430399785048e-24"),
         boost::lexical_cast<T>("-0.455579032003288910408487905303223613382276173706542364543918076752861628464036586507967767e-27"),
         boost::lexical_cast<T>("0.99650703862462739161520123768147312466695159780582506041370833824093136783202694548427718e-30"),
         boost::lexical_cast<T>("-0.1332444609228706921659395801935919548447859029572115502899861345555006827214220195650058e-32"),
         boost::lexical_cast<T>("0.100856999148765307000182397631280249632761913433008379786888200467467364474581430670889392e-35"),
         boost::lexical_cast<T>("-0.39146979455613683472384690509165395074425354524713697428673406058157887065953366609738731e-39"),
         boost::lexical_cast<T>("0.683859606707931248105140296850112494069265272540298100341919970496564103098283709868586478e-43"),
         boost::lexical_cast<T>("-0.450326344248604222735147147805963966503893913752040066400766411031387063854141246972061792e-47"),
         boost::lexical_cast<T>("0.870675378039492904184581895322153006461319724931909799151743284769901586333730037761678891e-52"),
         boost::lexical_cast<T>("-0.341678395249272265744518787745356400350877656459401143889000625280131819505857966769964401e-57"),
         boost::lexical_cast<T>("0.152322191370871666358069530949353871960316638394428595988162174042653299702098929238880862e-63"),
         boost::lexical_cast<T>("-0.285425405297633795767452984791738825078111150078605004958179057245980222485147999495352632e-71"),
      };
      T result = 0;
      for(unsigned k = 1; k <= sizeof(d)/sizeof(d[0]); ++k)
      {
         result += (-d[k-1]*dz)/(k*dz + k*k);
      }
      return result;
   }

   template<class T>
   static T lanczos_sum_near_2(const T& dz)
   {
      using namespace boost;
      static const T d[60] = {
         boost::lexical_cast<T>("557.56438192770795764344217888434355281097193198928944200046501607026919782564033547346298"),
         boost::lexical_cast<T>("-12552.748616427645475141433405567201788681683808077269330800392600825597799119572762385222"),
         boost::lexical_cast<T>("136741.650054039199076788077149441364242294724343897779563222338447737802381279007988884806"),
         boost::lexical_cast<T>("-960205.223613240309942047656967301131022760634321049075674684679438471862998829007639437133"),
         boost::lexical_cast<T>("4885504.47588736223774859617054275229642041717942140469884121916073195308537421162982679289"),
         boost::lexical_cast<T>("-19194501.738192166918904824982935279260356575935661514109550613809352009246483412530545583"),
         boost::lexical_cast<T>("60602169.8633537742937457094837494059849674261357199218329545854990149896822944801504450843"),
         boost::lexical_cast<T>("-157997975.522506767297528502540724511908584668874487506510120462561270100749019845014382885"),
         boost::lexical_cast<T>("346876323.86092543685419723290495817330608574729543216092477261152247521712190505658568876"),
         boost::lexical_cast<T>("-650770365.471136883718747607976242475416651908858429752332176373467422603953536408709972919"),
         boost::lexical_cast<T>("1055146856.05909309330903130910708372830487315684258450293308627289334336871273080305128138"),
         boost::lexical_cast<T>("-1491682726.25614447429071368736790697283307005456720192465860871846879804207692411263924608"),
         boost::lexical_cast<T>("1851726287.94866167094858600116562210167031458934987154557042242638980748286188183300900268"),
         boost::lexical_cast<T>("-2029855953.68334371445800569238095379629407314338521720558391277508374519526853827142679839"),
         boost::lexical_cast<T>("1973842002.53354868177824629525448788555435194808657489238517523691040148611221295436287925"),
         boost::lexical_cast<T>("-1708829941.98209573247426625323314413060108441455314880934710595647408841619484540679859098"),
         boost::lexical_cast<T>("1320934627.12433688699625456833930317624783222321555050330381730035733198249283009357314954"),
         boost::lexical_cast<T>("-913780636.858542526294419197161614811332299249415125108737474024007693329922089123296358727"),
         boost::lexical_cast<T>("566663423.929632170286007468016419798879660054391183200464733820209439185545886930103546787"),
         boost::lexical_cast<T>("-315402880.436816230388857961460509181823167373029384218959199936902955049832392362044305869"),
         boost::lexical_cast<T>("157691811.550465734461741500275930418786875005567018699867961482249002625886064187146134966"),
         boost::lexical_cast<T>("-70848085.5705405970640618473551954585013808128304384354476488268600720054598122945113512731"),
         boost::lexical_cast<T>("28604413.4050137708444142264980840059788755325900041515286382001704951527733220637586013815"),
         boost::lexical_cast<T>("-10374808.7067303054787164054055989420809074792801592763124972441820101840292558840131568633"),
         boost::lexical_cast<T>("3378126.32016207486657791623723515804931231041318964254116390764473281291389374196880720069"),
         boost::lexical_cast<T>("-986460.090390653140964189383080344920103075349535669020623874668558777188889544398718979744"),
         boost::lexical_cast<T>("257989.631187387317948158483575125380011593209850756066176921901006833159795100137743395985"),
         boost::lexical_cast<T>("-60326.0391159227288325790327830741260824763549807922845004854215660451399733578621565837087"),
         boost::lexical_cast<T>("12586.1375399649496159880821645216260841794563919652590583420570326276086323953958907053394"),
         boost::lexical_cast<T>("-2337.26417330316922535871922886167444795452055677161063205953141105726549966801856628447293"),
         boost::lexical_cast<T>("385.230745012608736644117458716226876976056390433401632749144285378123105481506733917763829"),
         boost::lexical_cast<T>("-56.1716559403731491675970177460841997333796694857076749852739159067307309470690838101179615"),
         boost::lexical_cast<T>("7.21907953468550196348585224042498727840087634483369357697580053424523903859773769748599575"),
         boost::lexical_cast<T>("-0.814293485887386870805786409956942772883474224091975496298369877683530509729332902182019049"),
         boost::lexical_cast<T>("0.0802304419995150047616460464220884371214157889148846405799324851793571580868840034085001373"),
         boost::lexical_cast<T>("-0.00686771095380619535195996193943858680694970000948742557733102777115482017857981277171196115"),
         boost::lexical_cast<T>("0.000507636621977556438232617777542864427109623356049335590894564220687567763620803789858345916"),
         boost::lexical_cast<T>("-0.32179095465362720747836116655088181481893063531138957363431280817392443948706754917605911e-4"),
         boost::lexical_cast<T>("0.173578890579848508947329833426585354230744194615295570820295052665075101971588563893718407e-5"),
         boost::lexical_cast<T>("-0.789762880006288893888161070734302768702358633525134582027140158619195373770299678322596835e-7"),
         boost::lexical_cast<T>("0.300074637200885066788470310738617992259402710843493097610337134266720909870967550606601658e-8"),
         boost::lexical_cast<T>("-0.941337297619721713151110244242536308266701344868601679868536153775533330272973088246835359e-10"),
         boost::lexical_cast<T>("0.24064815013182536657310186836079333949814111498828401548170442715552017773994482539471456e-11"),
         boost::lexical_cast<T>("-0.493892399304811910466345686492277504628763169549384435563232052965821874553923373100791477e-13"),
         boost::lexical_cast<T>("0.799780678476644196901221989475355609743387528732994566453160178199295104357319723742820593e-15"),
         boost::lexical_cast<T>("-0.100148627870893347527249092785257443532967736956154251497569191947184705954310833302770086e-16"),
         boost::lexical_cast<T>("0.947100256812658897084619699699028861352615460106539259289295071616221848196411749449858071e-19"),
         boost::lexical_cast<T>("-0.657808193528898116367845405906343884364280888644748907819280236995018351085371701094007759e-21"),
         boost::lexical_cast<T>("0.324554050057463845012469010247790763753999056976705084126950591088538742509983426730851614e-23"),
         boost::lexical_cast<T>("-0.10927068902162908990029309141242256163212535730975970310918370355165185052827948996110107e-25"),
         boost::lexical_cast<T>("0.239012340507870646690121104637467232366271566488184795459093215303237974655782634371712486e-28"),
         boost::lexical_cast<T>("-0.31958700972990573259359660326375143524864710953063781494908314884519046349402409667329667e-31"),
         boost::lexical_cast<T>("0.241905641292988284384362036555782113275737930713192053073501265726048089991747342247551645e-34"),
         boost::lexical_cast<T>("-0.93894080230619233745797029179332447129464915420290457418429337322820997038069119047864035e-38"),
         boost::lexical_cast<T>("0.164023814025085488413251990798690797467351995518990067783355251949198292596815470576539877e-41"),
         boost::lexical_cast<T>("-0.108010831192689925518484618970761942019888832176355541674171850211917230280206410356465451e-45"),
         boost::lexical_cast<T>("0.208831600642796805563854019033577205240227465154130766898180386564934443551840379116390645e-50"),
         boost::lexical_cast<T>("-0.819516067465171848863933747691434138146789031214932473898084756489529673230665363014007306e-56"),
         boost::lexical_cast<T>("0.365344970579318347488211604761724311582675708113250505307342682118101409913523622073678179e-62"),
         boost::lexical_cast<T>("-0.684593199208628857931267904308244537968349564351534581234005234847904343404822808648361291e-70"),
      };
      T result = 0;
      T z = dz + 2;
      for(unsigned k = 1; k <= sizeof(d)/sizeof(d[0]); ++k)
      {
         result += (-d[k-1]*dz)/(z + k*z + k*k - 1);
      }
      return result;
   }

   static double g(){ return 63.19215200000000010049916454590857028961181640625; }
};

}}} // namespaces

#endif


