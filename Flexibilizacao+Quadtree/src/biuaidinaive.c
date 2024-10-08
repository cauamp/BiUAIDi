// biuaidinaive
// Author: Wagner Meira Jr.
// Version history:
//    1.0 - 21/07/2024
//
// Usage:
// biuaidinaive x y
//    x - geographical x coordinate of current location
//    y - geographical y coordinate of current location
//
// Output:
//    10 closest recharging locations
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// struct that contains the information about the recharging locations
typedef struct
{
  char *idend;
  long id_logrado;
  char *sigla_tipo;
  char *nome_logra;
  int numero_imo;
  char *nome_bairr;
  char *nome_regio;
  int cep;
  double x;
  double y;
} addr_t, *ptr_addr_t;

// struct that contain the distance information between the origin location and
// each recharging location.
typedef struct knn
{
  double dist; // distance between origin location and recharging location
  int id;      // recharging location id
} knn_t, *ptr_knn_t;

// vector containing the information about the recharging locations
addr_t rechargevet[] = {
    {"01668501159A", 16685, "RUA", "CONDE DE PALMA", 1159, "Cabana do Pai Tomás", "OESTE", 30512710, 604476.327080534, 7793936.51282176},
    {"08098800057B", 80988, "RUA", "PEDRO TIMOTHEO", 57, "Vila Vista Alegre", "OESTE", 30514320, 605262.48179415, 7793257.07737953},
    {"02813300418", 28133, "RUA", "FERNAO DIAS", 418, "Mariano de Abreu", "LESTE", 31050364, 616071.596501685, 7799448.81579611},
    {"01855100150", 18551, "RUA", "CRISANDALIA", 150, "Caiçara-Adelaide", "NOROESTE", 30770400, 607550.931138684, 7798701.16804509},
    {"06751206455A", 67512, "AVE", "TERESA CRISTINA", 6455, "Cinqüentenário", "OESTE", 30550390, 606279.331855856, 7793399.39903},
    {"03986100116", 39861, "RUA", "JUSTINO CARNEIRO", 116, "Inconfidência", "NOROESTE", 30820240, 605104.903517628, 7798056.92607213},
    {"08777200075", 87772, "RUA", "SAO DOMINGOS", 75, "Jardim América", "OESTE", 30421498, 607516.231712159, 7793552.93842478},
    {"03587300106A", 35873, "RUA", "ITAPOA", 106, "Pindorama", "NOROESTE", 30865090, 602712.314503365, 7798159.40825128},
    {"01981600122A", 19816, "RUA", "DEOLINDA CANDIDA", 122, "Boa Vista", "LESTE", 31070440, 615804.983781242, 7800034.04266877},
    {"07512700842", 75127, "RUA", "ANTONIO RODRIGUES DA COSTA", 842, "Casa Branca", "LESTE", 31050010, 616439.872399631, 7800034.51657842},
    {"01865205173", 18652, "AVE", "CRISTIANO MACHADO", 5173, "Maria Virgínia", "NORDESTE", 31155555, 612175.234518041, 7802624.4298161},
    {"30601900008", 306019, "RUA", "PROGRESSO", 8, "Granja Werneck", "NORTE", 31846500, 614211.284553448, 7806576.09505908},
    {"17001200129", 170012, "RUA", "JORGE FRANCISCO DOS SANTOS", 129, "Vila Primeiro de Maio", "NORTE", 31810230, 611781.377997226, 7803470.52218684},
    {"02969700560", 29697, "RUA", "FRANCISCO SPINO", 560, "São Bernardo", "NORTE", 31741320, 610730.744750223, 7804959.87716695},
    {"01865209908", 18652, "AVE", "CRISTIANO MACHADO", 9908, "Floramar", "NORTE", 31840292, 611492.566067702, 7805763.65775733},
    {"05010600584", 50106, "RUA", "CANARIO", 584, "Goiânia", "NORDESTE", 31950390, 615499.853336804, 7802895.71555431},
    {"30298200191", 302982, "TRV", "DAS MADEIRAS", 191, "Nossa Senhora de Fátima", "CENTRO-SUL", 30250450, 614725.456032019, 7795406.84992727},
    {"00140700018", 1407, "RUA", "AGOSTINHO BRETAS", 18, "Caiçaras", "NOROESTE", 30775520, 608040.976161014, 7799296.98988677},
    {"01917600235", 19176, "AVE", "JOSE LOPES MURADAS", 235, "Floramar", "NORTE", 31742084, 611559.333118954, 7806361.34929349},
    {"05068201669", 50682, "RUA", "DO OURO", 1669, "Serra", "CENTRO-SUL", 30210590, 613092.596668605, 7794174.25447246},
    {"03513600935A", 35136, "RUA", "IZABEL BUENO", 935, "Indaiá", "PAMPULHA", 31270065, 609894.355760844, 7803470.79240715},
    {"30311800022", 303118, "RUA", "PROFESSOR FREDERICO RANGEL", 22, "Diamante", "BARREIRO", 30627210, 602111.29140432, 7788608.12238783},
    {"03959700400", 39597, "RUA", "JULITA NOGUEIRA SOARES", 400, "Santa Terezinha", "PAMPULHA", 31365250, 603742.921352697, 7802444.17609921},
    {"05385400379", 53854, "RUA", "PINTO MARTINS", 379, "Oeste", "OESTE", 30532140, 604055.382827822, 7794909.33006549},
    {"06939800309", 69398, "AVE", "TRINTA E UM DE MARCO", 309, "Dom Cabral", "NOROESTE", 30535000, 605094.942345087, 7797050.51554136},
    {"09113002238", 91130, "AVE", "PROFESSOR MARIO WERNECK", 2238, "Buritis", "OESTE", 30575180, 607974.004240312, 7790932.44927783},
    {"06191000494", 61910, "RUA", "SANTOS", 494, "Jardim América", "OESTE", 30421386, 607138.001184552, 7794640.97282756},
    {"02007100066", 20071, "RUA", "DEUSDEDITH DE ASSIS", 66, "Dom Bosco", "NOROESTE", 30850450, 604167.931217255, 7796746.72587901},
    {"30281100190", 302811, "RUA", "ALVINO FERREIRA FELIPE", 190, "Novo Aarão Reis", "NORTE", 31845142, 614416.828714214, 7805976.79704445},
    {"30028300275", 300283, "RUA", "DOZE", 275, "Vista do Sol", "NORDESTE", 31990510, 615975.326269529, 7804804.53565486},
    {"08342500194B", 83425, "RUA", "JOSE BALBINO VIEIRA", 194, "Brasil Industrial", "BARREIRO", 30626230, 603357.631290157, 7788233.20340357},
    {"30726500105", 307265, "RUA", "DOIS MIL SEISCENTOS E TRINTA E DOIS", 105, "Carlos Prates", "NOROESTE", 30000, 610429.645052432, 7797646.49666617},
    {"10122300819", 101223, "RUA", "LEONIL PRATA", 819, "Alipio de Melo", "PAMPULHA", 30840780, 604595.747799007, 7800006.41229896},
    {"02346000512", 23460, "RUA", "DOUTOR JULIO OTAVIANO FERREIRA", 512, "Cidade Nova", "NORDESTE", 31170200, 612529.78373877, 7800553.19100374},
    {"09706400140D", 97064, "RUA", "GLAUCE ROCHA", 140, "Lajedo", "NORTE", 31844400, 613669.588087163, 7806462.71889242},
    {"11654600250", 116546, "RUA", "LEVI FREIRE", 250, "Fazendinha", "CENTRO-SUL", 30260490, 614609.766300965, 7796014.28955035},
    {"01917600173", 19176, "AVE", "JOSE LOPES MURADAS", 173, "Floramar", "NORTE", 31742084, 611531.198143373, 7806347.68200626},
    {"04699300280", 46993, "RUA", "MUSAS", 280, "Santa Lúcia", "CENTRO-SUL", 30360660, 610179.033709342, 7791338.16669523},
    {"06794400388", 67944, "RUA", "TOMAZ GONZAGA", 388, "Lourdes", "CENTRO-SUL", 30180140, 610656.406712012, 7795661.7100149},
    {"09084700055", 90847, "RUA", "GERALDO MAGALHAES MASCARENHAS", 55, "Belvedere", "CENTRO-SUL", 30320340, 611092.959958295, 7791487.97719569},
    {"05492000090", 54920, "RUA", "POTUMAIO", 90, "São Geraldo", "LESTE", 31050270, 615054.096477821, 7798992.42922444},
    {"06653500927", 66535, "RUA", "DESEMBARGADOR SARAIVA", 927, "Alto Vera Cruz", "LESTE", 30285150, 616002.447760292, 7797893.32691858},
    {"12965901236", 129659, "RUA", "ESCRITOR HUMBERTO CAMPOS", 1236, "Tupi B", "NORTE", 31844480, 613541.763225837, 7806761.86724569},
    {"00178100139", 1781, "RUA", "ALBERTO CINTRA", 139, "União", "NORDESTE", 31160370, 612218.310853815, 7801341.31411224},
    {"08612000108", 86120, "RUA", "JOSE EVANGELICO", 108, "Jaqueline", "NORTE", 31748254, 611414.47557683, 7810213.36470339},
    {"08599400028", 85994, "RUA", "ADAO RIBEIRO", 28, "Jardim Vitória", "NORDESTE", 31970500, 617245.722700543, 7804572.45297739},
    {"30021200035", 300212, "RUA", "PASSARELA DA VILA", 35, "Vila Nossa Senhora Aparecida", "VENDA NOVA", 31515375, 608130.255559866, 7807475.93889172},
    {"03199900510", 31999, "RUA", "GUAICUI", 510, "Luxemburgo", "CENTRO-SUL", 30380342, 609471.886594208, 7794055.32362581},
    {"05032300058", 50323, "RUA", "ORENOCO", 58, "Carmo", "CENTRO-SUL", 30310060, 611986.930241366, 7794759.40865723},
    {"02229400211", 22294, "AVE", "DOM JOAO VI", 211, "Cinqüentenário", "OESTE", 30570063, 606152.007014846, 7792898.67829213},
    {"01216400028", 12164, "RUA", "CAMPO BELO", 28, "São Pedro", "CENTRO-SUL", 30330330, 611340.996660815, 7793977.32376209},
    {"05544300105", 55443, "RUA", "PROFESSOR CARLOS DE ALMEIDA", 105, "Monte Azul", "NORTE", 31872610, 615812.274400767, 7808298.31478452},
    {"12596300055B", 125963, "RUA", "LAUDEMIRA MARIA DE JESUS", 55, "Jardim Guanabara", "NORTE", 31742294, 610949.814004907, 7807381.75107014},
    {"03523700841", 35237, "RUA", "ITABIRA", 841, "Lagoinha", "NORDESTE", 31110340, 610637.309680717, 7798748.16828876},
    {"07109400110", 71094, "RUA", "URUCUIA", 110, "Floresta", "CENTRO-SUL", 30150060, 611822.573545355, 7797235.07800547},
    {"30112800062", 301128, "BEC", "SOUZA", 62, "Cabana do Pai Tomás", "OESTE", 30512820, 604518.834636161, 7793922.26563976},
    {"00739700699A", 7397, "RUA", "JOAO SAMAHA", 699, "São João Batista", "VENDA NOVA", 31515393, 608671.5652081, 7807560.72201224},
    {"10817500135", 108175, "RUA", "ALFREDO ALVES DE CASTRO", 135, "Mantiqueira", "VENDA NOVA", 31655000, 606654.885158384, 7811550.48160007},
    {"06600201461B", 66002, "AVE", "SILVIANO BRANDAO", 1461, "Sagrada Família", "LESTE", 31030105, 612614.756204585, 7797936.58885153},
    {"07219800325", 72198, "RUA", "JOAO DE SALES PIRES", 325, "Planalto", "NORTE", 31720220, 609604.340107711, 7807030.99686226},
    {"30082000015", 300820, "BEC", "OLINDA", 15, "Alto Vera Cruz", "LESTE", 30285154, 616104.705134004, 7798578.13117739},
    {"05165705891", 51657, "RUA", "PADRE PEDRO PINTO", 5891, "Lagoinha Leblon", "VENDA NOVA", 31570000, 605683.700605279, 7809953.17828837},
    {"30124500129", 301245, "AVE", "DETETIVE EDUARDO FERNANDES", 129, "Novo Aarão Reis", "NORTE", 31845000, 614155.501416786, 7805689.62549739},
    {"04244400361", 42444, "RUA", "MAESTRO FRANCISCO FLORES", 361, "São Luíz", "PAMPULHA", 31270670, 608918.347716062, 7804327.40277631},
    {"10721700172D", 107217, "RUA", "DO GROTAO", 172, "Ventosa", "OESTE", 30555270, 607587.86369934, 7793197.32889748},
    {"09189300582", 91893, "RUA", "RIO PETROPOLIS", 582, "Pindorama", "NOROESTE", 30865410, 601865.745088999, 7797572.25257214},
    {"30383900128", 303839, "RUA", "JEREMIAS", 128, "Bonsucesso", "BARREIRO", 30622582, 606425.959600273, 7788721.39621716},
    {"09653800112A", 96538, "RUA", "PAULO GONCALVES DOS REIS", 112, "Conjunto Minascaixa", "VENDA NOVA", 31615625, 608884.996872286, 7810159.99556652},
    {"30073300096", 300733, "BEC", "BRILHANTINA", 96, "Vila Dias", "LESTE", 31010172, 612889.434908242, 7797227.67931482},
    {"30219000194D", 302190, "BEC", "DELSO RENAULT", 194, "Vila Coqueiral", "NOROESTE", 30865710, 601957.307422262, 7797436.31950149},
    {"00200600217", 2006, "RUA", "ALEGRETE", 217, "Sagrada Família", "LESTE", 31035380, 612980.519297873, 7799252.41800563},
    {"07210303278", 72103, "RUA", "ANTONIO EUSTAQUIO PIAZZA", 3278, "Tirol", "BARREIRO", 30662050, 600399.839376393, 7789656.70164894},
    {"11844000450", 118440, "RUA", "RISOLETA FERNANDES", 450, "Ademar Maldonado", "BARREIRO", 30640620, 601759.894749648, 7790049.64896927},
    {"02913600577", 29136, "RUA", "FORMIGA", 577, "São Cristóvão", "NORDESTE", 31110430, 610712.644156542, 7798981.30207177},
    {"11660600398", 116606, "RUA", "SUDAO", 398, "Canaã", "VENDA NOVA", 31749055, 610367.58840675, 7810893.14134413},
    {"11516500501", 115165, "RUA", "MAESTRO FRANCISCO BUZELIN", 501, "Bandeirantes", "PAMPULHA", 31365310, 604467.628980231, 7803523.79266466},
    {"05330600093F", 53306, "RUA", "PENIDO", 93, "Pirajá", "NORDESTE", 31910710, 613114.144480769, 7802972.93060814},
    {"30208200254", 302082, "BEC", "PROFESSORA SILVANA", 254, "Vila Califórnia", "NOROESTE", 30850522, 603744.298708319, 7797025.09076529},
    {"07281200068D", 72812, "RUA", "ADILSON PAULO DE SOUZA", 68, "São João Batista", "VENDA NOVA", 31515270, 608637.732328822, 7807586.89682963},
    {"00043200058", 432, "AVE", "MANOEL SALLES BARBOSA", 58, "Cardoso", "BARREIRO", 30626400, 603734.790042473, 7788527.74497144},
    {"12531400206", 125314, "RUA", "DOIS", 206, "Novo Santa Cecília", "BARREIRO", 30626497, 602845.649805631, 7787655.29633281},
    {"30049300058", 300493, "BEC", "AGUIAR", 58, "Ventosa", "OESTE", 30421677, 607355.890395323, 7793509.44706746},
    {"06982300309E", 69823, "RUA", "TUCUMAN", 309, "São Geraldo", "LESTE", 31050560, 615091.920448603, 7799508.78687927},
    {"00027100210", 271, "RUA", "DA IGUALDADE", 210, "Nova Gameleira", "OESTE", 30510450, 605887.644089692, 7794875.81645585},
    {"30219000176", 302190, "BEC", "DELSO RENAULT", 176, "Vila Coqueiral", "NOROESTE", 30865710, 601977.155277239, 7797437.5492551},
    {"04090200050A", 40902, "RUA", "AGUAS DE LINDOIA", 50, "Itaipu", "BARREIRO", 30692560, 599226.548201479, 7788631.38502896},
    {"05641800057", 56418, "RUA", "IRACEMA GOMES PEREIRA", 57, "Brasil Industrial", "BARREIRO", 30626240, 603419.632147162, 7788368.77652327},
    {"00808400691", 8084, "AVE", "BARBACENA", 691, "Barro Preto", "CENTRO-SUL", 30190134, 609602.87083828, 7796576.56884271},
    {"12327401115", 123274, "RUA", "DOUTOR SYLVIO MENICUCCI", 1115, "Castelo", "PAMPULHA", 30840480, 604180.770626667, 7800850.33256014},
    {"00006700455", 67, "RUA", "ASSIMA HATEM BORJAILI", 455, "Vila Novo São Lucas", "CENTRO-SUL", 30260470, 614040.560777859, 7795662.32939393},
    {"02242700463", 22427, "RUA", "ARISTOTELES", 463, "Nazaré", "NORDESTE", 31990230, 615805.297795713, 7804454.84650533},
    {"12759300129", 127593, "RUA", "CHOPOTO", 129, "Pindorama", "NOROESTE", 30880510, 601960.836747003, 7798066.79454269},
    {"03837700700A", 38377, "RUA", "JOSE DE ALENCAR", 700, "Nova Suissa", "OESTE", 30421148, 606783.21374109, 7795892.43483714},
    {"03311500246", 33115, "RUA", "REGINA OTHILIA DOS SANTOS", 246, "Jardim Felicidade", "NORTE", 31742515, 612179.077756278, 7807075.29770878},
    {"03867000091", 38670, "RUA", "JOSE CLEMENTE PEREIRA", 91, "Ipiranga", "NORDESTE", 31160130, 611506.575168924, 7801215.33918377},
    {"04611400022", 46114, "RUA", "MOISES", 22, "Glória", "NOROESTE", 30880280, 603067.237300087, 7799179.19392107},
    {"05314500388", 53145, "AVE", "DOM PEDRO II", 388, "Bonfim", "NOROESTE", 31210242, 610165.001518416, 7797836.99894743},
    {"08606000006A", 86060, "RUA", "GERALDO FRANCISCO DE OLIVEIRA", 6, "Jaqueline", "NORTE", 31748197, 611226.841389875, 7810171.55321399},
    {"01957000125D", 19570, "RUA", "EXPEDICIONARIO DIAMANTINO DO NASCIMENTO", 125, "Maria Goretti", "NORDESTE", 31930560, 614120.835679652, 7803503.49530593},
    {"02152500084A", 21525, "RUA", "AMAVEL COSTA", 84, "Jaraguá", "PAMPULHA", 31270470, 610052.685183527, 7804211.74553563},
    {"", -1, "", "", -1, "", "", -1, -1, -1}};

// qsort comparison function between distances to recharging locations
int cmpknn(const void *a, const void *b)
{
  ptr_knn_t k1 = (ptr_knn_t)a;
  ptr_knn_t k2 = (ptr_knn_t)b;
  if (k1->dist > k2->dist)
    return 1;
  else if (k1->dist < k2->dist)
    return -1;
  else
    return 0;
}

// calculates Euclidean distance between (x1,y1) and (x2,y2)
double dist(double x1, double y1, double x2, double y2)
{
  return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
}

// print the recharge location information
void printrecharge(int pos)
{
  printf("%s %s, %d, %s, %s, %d", rechargevet[pos].sigla_tipo,
         rechargevet[pos].nome_logra, rechargevet[pos].numero_imo,
         rechargevet[pos].nome_bairr, rechargevet[pos].nome_regio,
         rechargevet[pos].cep);
}

// print illustrative map using gnuplot
void printmap(ptr_knn_t kvet, int kmax, ptr_addr_t rvet, int nrec, double tx, double ty)
{
  FILE *out;

  // gnuplot script. to obtain the actual map, it is necessary to run:
  // gnuplot out.gp
  out = fopen("out.gp", "wt");
  fprintf(out, "set term postscript eps\n");
  fprintf(out, "set output \"out.eps\"\n");
  fprintf(out, "set size square\n");
  fprintf(out, "set key bottom right\n");
  fprintf(out, "set title \"BiUaiDi Recharging Stations\"\n");
  fprintf(out, "set xlabel \"\"\n");
  fprintf(out, "set ylabel \"\"\n");
  fprintf(out, "unset xtics \n");
  fprintf(out, "unset ytics \n");
  fprintf(out, "plot \"origin.gpdat\" t \"Your location\" pt 4 ps 2, \"recharge.gpdat\" t \"\", \"suggested.gpdat\" t \"Nearest stations\" pt 7 ps 2\n");
  fclose(out);

  // origin point, just one pair of x, y coordinates
  out = fopen("origin.gpdat", "wt");
  fprintf(out, "%f %f\n", tx, ty);
  fclose(out);

  // all recharging locations
  out = fopen("recharge.gpdat", "wt");
  for (int i = 0; i < nrec; i++)
  {
    fprintf(out, "%f %f\n", rvet[i].x, rvet[i].y);
  }
  fclose(out);

  // the nearest recharging locations
  out = fopen("suggested.gpdat", "wt");
  for (int i = 0; i < kmax; i++)
  {
    fprintf(out, "%f %f\n", rvet[kvet[i].id].x, rvet[kvet[i].id].y);
  }
  fclose(out);
}

int main(int argc, char **argv)
{

  // count the number of recharge locations we have in rechargevet
  int numrecharge = 0;
  while (rechargevet[numrecharge].id_logrado != -1)
    numrecharge++;

  // read the coordinates of the current point that needs recharging
  double tx = atof(argv[1]);
  double ty = atof(argv[2]);

  // create the vector of distances and populate it
  ptr_knn_t kvet = (ptr_knn_t)malloc(numrecharge * sizeof(knn_t));
  for (int i = 0; i < numrecharge; i++)
  {
    kvet[i].id = i;
    kvet[i].dist = dist(tx, ty, rechargevet[i].x, rechargevet[i].y);
  }

  // sort the vector of distances
  qsort(kvet, numrecharge, sizeof(knn_t), cmpknn);

  // print the 10 nearest recharging locations
  int kmax = 10;
  for (int i = 0; i < kmax; i++)
  {
    printrecharge(kvet[i].id);
    printf(" (%f)\n", kvet[i].dist);
  }

  printmap(kvet, kmax, rechargevet, numrecharge, tx, ty);

  free(kvet);
  exit(0);
}
