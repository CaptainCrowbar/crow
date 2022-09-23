#include "crow/english.hpp"
#include "crow/string.hpp"
#include "crow/unit-test.hpp"
#include <random>
#include <string>
#include <vector>

using namespace Crow;

void test_crow_english_extended_titlecase() {

    TEST_EQUAL(extended_titlecase(""),                                   "");
    TEST_EQUAL(extended_titlecase("hello world"),                        "Hello World");
    TEST_EQUAL(extended_titlecase("doctor who and the daleks"),          "Doctor Who and the Daleks");
    TEST_EQUAL(extended_titlecase("the haunting of hill house"),         "The Haunting of Hill House");
    TEST_EQUAL(extended_titlecase("a history of the future"),            "A History of the Future");
    TEST_EQUAL(extended_titlecase("a dog in the manger"),                "A Dog in the Manger");
    TEST_EQUAL(extended_titlecase("wolf-rayet star's spectrum"),         "Wolf-Rayet Star's Spectrum");
    TEST_EQUAL(extended_titlecase("", false),                            "");
    TEST_EQUAL(extended_titlecase("hello world", false),                 "Hello World");
    TEST_EQUAL(extended_titlecase("doctor who and the daleks", false),   "Doctor Who and the Daleks");
    TEST_EQUAL(extended_titlecase("the haunting of hill house", false),  "the Haunting of Hill House");
    TEST_EQUAL(extended_titlecase("a history of the future", false),     "a History of the Future");
    TEST_EQUAL(extended_titlecase("a dog in the manger", false),         "a Dog in the Manger");
    TEST_EQUAL(extended_titlecase("wolf-rayet star's spectrum", false),  "Wolf-Rayet Star's Spectrum");

}

void test_crow_english_sentence_case() {

    TEST_EQUAL(sentence_case(""),                       "");
    TEST_EQUAL(sentence_case("hello world"),            "Hello world");
    TEST_EQUAL(sentence_case("hello World"),            "Hello World");
    TEST_EQUAL(sentence_case("hello world. goodbye."),  "Hello world. Goodbye.");

}

void test_crow_english_list_formatting() {

    std::vector<std::string> list;

    list = {};                            TEST_EQUAL(comma_list(list), "");                    TEST_EQUAL(comma_list(list, "and"), "");
    list = {"Abc"};                       TEST_EQUAL(comma_list(list), "Abc");                 TEST_EQUAL(comma_list(list, "and"), "Abc");
    list = {"Abc", "Def"};                TEST_EQUAL(comma_list(list), "Abc, Def");            TEST_EQUAL(comma_list(list, "and"), "Abc and Def");
    list = {"Abc", "Def", "Ghi"};         TEST_EQUAL(comma_list(list), "Abc, Def, Ghi");       TEST_EQUAL(comma_list(list, "and"), "Abc, Def, and Ghi");
    list = {"Abc", "Def", "Ghi", "Jkl"};  TEST_EQUAL(comma_list(list), "Abc, Def, Ghi, Jkl");  TEST_EQUAL(comma_list(list, "and"), "Abc, Def, Ghi, and Jkl");

}

void test_crow_english_number_formatting() {

    TEST_EQUAL(cardinal(0),           "zero");
    TEST_EQUAL(cardinal(1),           "one");
    TEST_EQUAL(cardinal(2),           "two");
    TEST_EQUAL(cardinal(3),           "three");
    TEST_EQUAL(cardinal(4),           "four");
    TEST_EQUAL(cardinal(5),           "five");
    TEST_EQUAL(cardinal(6),           "six");
    TEST_EQUAL(cardinal(7),           "seven");
    TEST_EQUAL(cardinal(8),           "eight");
    TEST_EQUAL(cardinal(9),           "nine");
    TEST_EQUAL(cardinal(10),          "ten");
    TEST_EQUAL(cardinal(11),          "eleven");
    TEST_EQUAL(cardinal(12),          "twelve");
    TEST_EQUAL(cardinal(13),          "thirteen");
    TEST_EQUAL(cardinal(14),          "fourteen");
    TEST_EQUAL(cardinal(15),          "fifteen");
    TEST_EQUAL(cardinal(16),          "sixteen");
    TEST_EQUAL(cardinal(17),          "seventeen");
    TEST_EQUAL(cardinal(18),          "eighteen");
    TEST_EQUAL(cardinal(19),          "nineteen");
    TEST_EQUAL(cardinal(20),          "twenty");
    TEST_EQUAL(cardinal(23),          "twenty-three");
    TEST_EQUAL(cardinal(42),          "forty-two");
    TEST_EQUAL(cardinal(400),         "four hundred");
    TEST_EQUAL(cardinal(401),         "four hundred and one");
    TEST_EQUAL(cardinal(456),         "four hundred and fifty-six");
    TEST_EQUAL(cardinal(7000),        "seven thousand");
    TEST_EQUAL(cardinal(7001),        "seven thousand and one");
    TEST_EQUAL(cardinal(7890),        "seven thousand, eight hundred and ninety");
    TEST_EQUAL(cardinal(1000000),     "one million");
    TEST_EQUAL(cardinal(1000001),     "one million and one");
    TEST_EQUAL(cardinal(1002000),     "one million, two thousand");
    TEST_EQUAL(cardinal(1002003),     "one million, two thousand and three");
    TEST_EQUAL(cardinal(1000000000),  "one billion");
    TEST_EQUAL(cardinal(1000000001),  "one billion and one");
    TEST_EQUAL(cardinal(1002003000),  "one billion, two million, three thousand");
    TEST_EQUAL(cardinal(1002003004),  "one billion, two million, three thousand and four");
    TEST_EQUAL(cardinal(1234567890),  "one billion, two hundred and thirty-four million, five hundred and sixty-seven thousand, eight hundred and ninety");
    TEST_EQUAL(cardinal(0xffffffff),  "four billion, two hundred and ninety-four million, nine hundred and sixty-seven thousand, two hundred and ninety-five");

    TEST_EQUAL(cardinal(0, 5),   "zero");
    TEST_EQUAL(cardinal(1, 5),   "one");
    TEST_EQUAL(cardinal(2, 5),   "two");
    TEST_EQUAL(cardinal(3, 5),   "three");
    TEST_EQUAL(cardinal(4, 5),   "four");
    TEST_EQUAL(cardinal(5, 5),   "5");
    TEST_EQUAL(cardinal(6, 5),   "6");
    TEST_EQUAL(cardinal(7, 5),   "7");
    TEST_EQUAL(cardinal(8, 5),   "8");
    TEST_EQUAL(cardinal(9, 5),   "9");
    TEST_EQUAL(cardinal(10, 5),  "10");

    TEST_EQUAL(ordinal(0),           "zeroth");
    TEST_EQUAL(ordinal(1),           "first");
    TEST_EQUAL(ordinal(2),           "second");
    TEST_EQUAL(ordinal(3),           "third");
    TEST_EQUAL(ordinal(4),           "fourth");
    TEST_EQUAL(ordinal(5),           "fifth");
    TEST_EQUAL(ordinal(6),           "sixth");
    TEST_EQUAL(ordinal(7),           "seventh");
    TEST_EQUAL(ordinal(8),           "eighth");
    TEST_EQUAL(ordinal(9),           "ninth");
    TEST_EQUAL(ordinal(10),          "tenth");
    TEST_EQUAL(ordinal(11),          "eleventh");
    TEST_EQUAL(ordinal(12),          "twelfth");
    TEST_EQUAL(ordinal(13),          "thirteenth");
    TEST_EQUAL(ordinal(14),          "fourteenth");
    TEST_EQUAL(ordinal(15),          "fifteenth");
    TEST_EQUAL(ordinal(16),          "sixteenth");
    TEST_EQUAL(ordinal(17),          "seventeenth");
    TEST_EQUAL(ordinal(18),          "eighteenth");
    TEST_EQUAL(ordinal(19),          "nineteenth");
    TEST_EQUAL(ordinal(20),          "twentieth");
    TEST_EQUAL(ordinal(23),          "twenty-third");
    TEST_EQUAL(ordinal(42),          "forty-second");
    TEST_EQUAL(ordinal(400),         "four hundredth");
    TEST_EQUAL(ordinal(401),         "four hundred and first");
    TEST_EQUAL(ordinal(456),         "four hundred and fifty-sixth");
    TEST_EQUAL(ordinal(7000),        "seven thousandth");
    TEST_EQUAL(ordinal(7001),        "seven thousand and first");
    TEST_EQUAL(ordinal(7890),        "seven thousand, eight hundred and ninetieth");
    TEST_EQUAL(ordinal(1000000),     "one millionth");
    TEST_EQUAL(ordinal(1000001),     "one million and first");
    TEST_EQUAL(ordinal(1002000),     "one million, two thousandth");
    TEST_EQUAL(ordinal(1002003),     "one million, two thousand and third");
    TEST_EQUAL(ordinal(1000000000),  "one billionth");
    TEST_EQUAL(ordinal(1000000001),  "one billion and first");
    TEST_EQUAL(ordinal(1002003000),  "one billion, two million, three thousandth");
    TEST_EQUAL(ordinal(1002003004),  "one billion, two million, three thousand and fourth");
    TEST_EQUAL(ordinal(1234567890),  "one billion, two hundred and thirty-four million, five hundred and sixty-seven thousand, eight hundred and ninetieth");
    TEST_EQUAL(ordinal(0xffffffff),  "four billion, two hundred and ninety-four million, nine hundred and sixty-seven thousand, two hundred and ninety-fifth");

    TEST_EQUAL(ordinal(0, 5),           "zeroth");
    TEST_EQUAL(ordinal(1, 5),           "first");
    TEST_EQUAL(ordinal(2, 5),           "second");
    TEST_EQUAL(ordinal(3, 5),           "third");
    TEST_EQUAL(ordinal(4, 5),           "fourth");
    TEST_EQUAL(ordinal(5, 5),           "5th");
    TEST_EQUAL(ordinal(6, 5),           "6th");
    TEST_EQUAL(ordinal(7, 5),           "7th");
    TEST_EQUAL(ordinal(8, 5),           "8th");
    TEST_EQUAL(ordinal(9, 5),           "9th");
    TEST_EQUAL(ordinal(0, 0),           "0th");
    TEST_EQUAL(ordinal(1, 0),           "1st");
    TEST_EQUAL(ordinal(2, 0),           "2nd");
    TEST_EQUAL(ordinal(3, 0),           "3rd");
    TEST_EQUAL(ordinal(4, 0),           "4th");
    TEST_EQUAL(ordinal(5, 0),           "5th");
    TEST_EQUAL(ordinal(6, 0),           "6th");
    TEST_EQUAL(ordinal(7, 0),           "7th");
    TEST_EQUAL(ordinal(8, 0),           "8th");
    TEST_EQUAL(ordinal(9, 0),           "9th");
    TEST_EQUAL(ordinal(10, 0),          "10th");
    TEST_EQUAL(ordinal(11, 0),          "11th");
    TEST_EQUAL(ordinal(12, 0),          "12th");
    TEST_EQUAL(ordinal(13, 0),          "13th");
    TEST_EQUAL(ordinal(14, 0),          "14th");
    TEST_EQUAL(ordinal(15, 0),          "15th");
    TEST_EQUAL(ordinal(16, 0),          "16th");
    TEST_EQUAL(ordinal(17, 0),          "17th");
    TEST_EQUAL(ordinal(18, 0),          "18th");
    TEST_EQUAL(ordinal(19, 0),          "19th");
    TEST_EQUAL(ordinal(20, 0),          "20th");
    TEST_EQUAL(ordinal(23, 0),          "23rd");
    TEST_EQUAL(ordinal(42, 0),          "42nd");
    TEST_EQUAL(ordinal(400, 0),         "400th");
    TEST_EQUAL(ordinal(401, 0),         "401st");
    TEST_EQUAL(ordinal(456, 0),         "456th");
    TEST_EQUAL(ordinal(7000, 0),        "7000th");
    TEST_EQUAL(ordinal(7001, 0),        "7001st");
    TEST_EQUAL(ordinal(7890, 0),        "7890th");
    TEST_EQUAL(ordinal(1000000, 0),     "1000000th");
    TEST_EQUAL(ordinal(1000001, 0),     "1000001st");
    TEST_EQUAL(ordinal(1002000, 0),     "1002000th");
    TEST_EQUAL(ordinal(1002003, 0),     "1002003rd");
    TEST_EQUAL(ordinal(1000000000, 0),  "1000000000th");
    TEST_EQUAL(ordinal(1000000001, 0),  "1000000001st");
    TEST_EQUAL(ordinal(1002003000, 0),  "1002003000th");
    TEST_EQUAL(ordinal(1002003004, 0),  "1002003004th");
    TEST_EQUAL(ordinal(1234567890, 0),  "1234567890th");
    TEST_EQUAL(ordinal(0xffffffff, 0),  "4294967295th");

    TEST_EQUAL(format_count(0.0, 1),      "0");
    TEST_EQUAL(format_count(0.0, 2),      "0");
    TEST_EQUAL(format_count(0.0, 3),      "0");
    TEST_EQUAL(format_count(1.23, 1),     "1");
    TEST_EQUAL(format_count(12.3, 1),     "10");
    TEST_EQUAL(format_count(123.0, 1),    "100");
    TEST_EQUAL(format_count(1.23e3, 1),   "1000");
    TEST_EQUAL(format_count(1.23e4, 1),   "10 thousand");
    TEST_EQUAL(format_count(1.23e5, 1),   "100 thousand");
    TEST_EQUAL(format_count(1.23e6, 1),   "1 million");
    TEST_EQUAL(format_count(1.23e7, 1),   "10 million");
    TEST_EQUAL(format_count(1.23e8, 1),   "100 million");
    TEST_EQUAL(format_count(1.23e9, 1),   "1 billion");
    TEST_EQUAL(format_count(1.23e10, 1),  "10 billion");
    TEST_EQUAL(format_count(1.23e11, 1),  "100 billion");
    TEST_EQUAL(format_count(1.23e12, 1),  "1x10^12");
    TEST_EQUAL(format_count(1.23e13, 1),  "1x10^13");
    TEST_EQUAL(format_count(1.23e14, 1),  "1x10^14");
    TEST_EQUAL(format_count(1.23e15, 1),  "1x10^15");
    TEST_EQUAL(format_count(1.23, 3),     "1");
    TEST_EQUAL(format_count(12.3, 3),     "12");
    TEST_EQUAL(format_count(123.0, 3),    "123");
    TEST_EQUAL(format_count(1.23e3, 3),   "1230");
    TEST_EQUAL(format_count(1.23e4, 3),   "12.3 thousand");
    TEST_EQUAL(format_count(1.23e5, 3),   "123 thousand");
    TEST_EQUAL(format_count(1.23e6, 3),   "1.23 million");
    TEST_EQUAL(format_count(1.23e7, 3),   "12.3 million");
    TEST_EQUAL(format_count(1.23e8, 3),   "123 million");
    TEST_EQUAL(format_count(1.23e9, 3),   "1.23 billion");
    TEST_EQUAL(format_count(1.23e10, 3),  "12.3 billion");
    TEST_EQUAL(format_count(1.23e11, 3),  "123 billion");
    TEST_EQUAL(format_count(1.23e12, 3),  "1.23x10^12");
    TEST_EQUAL(format_count(1.23e13, 3),  "1.23x10^13");
    TEST_EQUAL(format_count(1.23e14, 3),  "1.23x10^14");
    TEST_EQUAL(format_count(1.23e15, 3),  "1.23x10^15");

    TEST_EQUAL(number_of(0,   "thing"),          "no things");
    TEST_EQUAL(number_of(1,   "thing"),          "one thing");
    TEST_EQUAL(number_of(2,   "thing"),          "two things");
    TEST_EQUAL(number_of(3,   "thing"),          "three things");
    TEST_EQUAL(number_of(10,  "thing"),          "ten things");
    TEST_EQUAL(number_of(20,  "thing"),          "twenty things");
    TEST_EQUAL(number_of(30,  "thing"),          "30 things");
    TEST_EQUAL(number_of(0,   "mouse", "mice"),  "no mice");
    TEST_EQUAL(number_of(1,   "mouse", "mice"),  "one mouse");
    TEST_EQUAL(number_of(2,   "mouse", "mice"),  "two mice");
    TEST_EQUAL(number_of(3,   "mouse", "mice"),  "three mice");
    TEST_EQUAL(number_of(10,  "mouse", "mice"),  "ten mice");
    TEST_EQUAL(number_of(20,  "mouse", "mice"),  "twenty mice");
    TEST_EQUAL(number_of(30,  "mouse", "mice"),  "30 mice");

}

void test_crow_english_pluralization() {

    TEST_EQUAL(plural("abscissa"),       "abscissae");
    TEST_EQUAL(plural("acropolis"),      "acropolises");
    TEST_EQUAL(plural("aegis"),          "aegises");
    TEST_EQUAL(plural("afreet"),         "afreeti");
    TEST_EQUAL(plural("afrit"),          "afriti");
    TEST_EQUAL(plural("agendum"),        "agenda");
    TEST_EQUAL(plural("albino"),         "albinos");
    TEST_EQUAL(plural("alga"),           "algae");
    TEST_EQUAL(plural("alias"),          "aliases");
    TEST_EQUAL(plural("alto"),           "altos");
    TEST_EQUAL(plural("alumna"),         "alumnae");
    TEST_EQUAL(plural("amoeba"),         "amoebae");
    TEST_EQUAL(plural("antenna"),        "antennae");
    TEST_EQUAL(plural("apex"),           "apices");
    TEST_EQUAL(plural("aphelion"),       "aphelia");
    TEST_EQUAL(plural("apparatus"),      "apparatuses");
    TEST_EQUAL(plural("apsis"),          "apsides");
    TEST_EQUAL(plural("aquarium"),       "aquaria");
    TEST_EQUAL(plural("archipelago"),    "archipelagos");
    TEST_EQUAL(plural("armadillo"),      "armadillos");
    TEST_EQUAL(plural("asbestos"),       "asbestoses");
    TEST_EQUAL(plural("asyndeton"),      "asyndeta");
    TEST_EQUAL(plural("atlas"),          "atlases");
    TEST_EQUAL(plural("aurora"),         "aurorae");
    TEST_EQUAL(plural("axis"),           "axes");
    TEST_EQUAL(plural("bacterium"),      "bacteria");
    TEST_EQUAL(plural("basis"),          "bases");
    TEST_EQUAL(plural("basso"),          "bassos");
    TEST_EQUAL(plural("bathos"),         "bathoses");
    TEST_EQUAL(plural("bias"),           "biases");
    TEST_EQUAL(plural("box"),            "boxes");
    TEST_EQUAL(plural("caddis"),         "caddises");
    TEST_EQUAL(plural("candelabrum"),    "candelabra");
    TEST_EQUAL(plural("cannabis"),       "cannabises");
    TEST_EQUAL(plural("canto"),          "cantos");
    TEST_EQUAL(plural("cantus"),         "cantuses");
    TEST_EQUAL(plural("canvas"),         "canvases");
    TEST_EQUAL(plural("chaos"),          "chaoses");
    TEST_EQUAL(plural("cherub"),         "cherubim");
    TEST_EQUAL(plural("child"),          "children");
    TEST_EQUAL(plural("chrysanthemum"),  "chrysanthemums");
    TEST_EQUAL(plural("codex"),          "codices");
    TEST_EQUAL(plural("coitus"),         "coituses");
    TEST_EQUAL(plural("comma"),          "commas");
    TEST_EQUAL(plural("commando"),       "commandos");
    TEST_EQUAL(plural("compendium"),     "compendia");
    TEST_EQUAL(plural("consortium"),     "consortia");
    TEST_EQUAL(plural("contralto"),      "contraltos");
    TEST_EQUAL(plural("cortex"),         "cortices");
    TEST_EQUAL(plural("cosmos"),         "cosmoses");
    TEST_EQUAL(plural("cranium"),        "crania");
    TEST_EQUAL(plural("crescendo"),      "crescendos");
    TEST_EQUAL(plural("criterion"),      "criteria");
    TEST_EQUAL(plural("curriculum"),     "curricula");
    TEST_EQUAL(plural("dais"),           "daises");
    TEST_EQUAL(plural("datum"),          "data");
    TEST_EQUAL(plural("desideratum"),    "desiderata");
    TEST_EQUAL(plural("dictum"),         "dicta");
    TEST_EQUAL(plural("digitalis"),      "digitalises");
    TEST_EQUAL(plural("ditto"),          "dittos");
    TEST_EQUAL(plural("djinni"),         "djinn");
    TEST_EQUAL(plural("dynamo"),         "dynamos");
    TEST_EQUAL(plural("efreet"),         "efreeti");
    TEST_EQUAL(plural("elk"),            "elk");
    TEST_EQUAL(plural("embryo"),         "embryos");
    TEST_EQUAL(plural("emporium"),       "emporia");
    TEST_EQUAL(plural("enconium"),       "enconia");
    TEST_EQUAL(plural("ephemeris"),      "ephemerides");
    TEST_EQUAL(plural("epidermis"),      "epidermises");
    TEST_EQUAL(plural("epoch"),          "epochs");
    TEST_EQUAL(plural("erratum"),        "errata");
    TEST_EQUAL(plural("ethos"),          "ethoses");
    TEST_EQUAL(plural("extremum"),       "extrema");
    TEST_EQUAL(plural("ferry"),          "ferries");
    TEST_EQUAL(plural("fiasco"),         "fiascos");
    TEST_EQUAL(plural("focus"),          "foci");
    TEST_EQUAL(plural("formula"),        "formulae");
    TEST_EQUAL(plural("fungus"),         "fungi");
    TEST_EQUAL(plural("gas"),            "gases");
    TEST_EQUAL(plural("generalissimo"),  "generalissimos");
    TEST_EQUAL(plural("genius"),         "genii");
    TEST_EQUAL(plural("ghetto"),         "ghettos");
    TEST_EQUAL(plural("glottis"),        "glottises");
    TEST_EQUAL(plural("goy"),            "goyim");
    TEST_EQUAL(plural("guano"),          "guanos");
    TEST_EQUAL(plural("gymnasium"),      "gymnasia");
    TEST_EQUAL(plural("hiatus"),         "hiatuses");
    TEST_EQUAL(plural("honorarium"),     "honoraria");
    TEST_EQUAL(plural("hydra"),          "hydrae");
    TEST_EQUAL(plural("hyperbaton"),     "hyperbata");
    TEST_EQUAL(plural("hyperbola"),      "hyperbolae");
    TEST_EQUAL(plural("ibis"),           "ibises");
    TEST_EQUAL(plural("impetus"),        "impetuses");
    TEST_EQUAL(plural("incubus"),        "incubi");
    TEST_EQUAL(plural("index"),          "indices");
    TEST_EQUAL(plural("inferno"),        "infernos");
    TEST_EQUAL(plural("innings"),        "innings");
    TEST_EQUAL(plural("interregnum"),    "interregna");
    TEST_EQUAL(plural("iris"),           "irises");
    TEST_EQUAL(plural("jumbo"),          "jumbos");
    TEST_EQUAL(plural("lacuna"),         "lacunae");
    TEST_EQUAL(plural("latex"),          "latices");
    TEST_EQUAL(plural("lens"),           "lenses");
    TEST_EQUAL(plural("life"),           "lives");
    TEST_EQUAL(plural("lingo"),          "lingos");
    TEST_EQUAL(plural("lumbago"),        "lumbagos");
    TEST_EQUAL(plural("lustrum"),        "lustra");
    TEST_EQUAL(plural("magneto"),        "magnetos");
    TEST_EQUAL(plural("manifesto"),      "manifestos");
    TEST_EQUAL(plural("mantis"),         "mantids");
    TEST_EQUAL(plural("marquis"),        "marquises");
    TEST_EQUAL(plural("maximum"),        "maxima");
    TEST_EQUAL(plural("medico"),         "medicos");
    TEST_EQUAL(plural("medium"),         "media");
    TEST_EQUAL(plural("medusa"),         "medusae");
    TEST_EQUAL(plural("memorandum"),     "memoranda");
    TEST_EQUAL(plural("metropolis"),     "metropolises");
    TEST_EQUAL(plural("millenium"),      "millenia");
    TEST_EQUAL(plural("minimum"),        "minima");
    TEST_EQUAL(plural("momentum"),       "momenta");
    TEST_EQUAL(plural("mongoose"),       "mongooses");
    TEST_EQUAL(plural("murex"),          "murices");
    TEST_EQUAL(plural("mythos"),         "mythoi");
    TEST_EQUAL(plural("nebula"),         "nebulae");
    TEST_EQUAL(plural("nexus"),          "nexuses");
    TEST_EQUAL(plural("nimbus"),         "nimbi");
    TEST_EQUAL(plural("noumenon"),       "noumena");
    TEST_EQUAL(plural("nova"),           "novae");
    TEST_EQUAL(plural("nucleolus"),      "nucleoli");
    TEST_EQUAL(plural("octavo"),         "octavos");
    TEST_EQUAL(plural("optimum"),        "optima");
    TEST_EQUAL(plural("organon"),        "organa");
    TEST_EQUAL(plural("ovum"),           "ova");
    TEST_EQUAL(plural("ox"),             "oxen");
    TEST_EQUAL(plural("parabola"),       "parabolae");
    TEST_EQUAL(plural("pathos"),         "pathoses");
    TEST_EQUAL(plural("pelvis"),         "pelvises");
    TEST_EQUAL(plural("perihelion"),     "perihelia");
    TEST_EQUAL(plural("phenomenon"),     "phenomena");
    TEST_EQUAL(plural("photo"),          "photos");
    TEST_EQUAL(plural("phylum"),         "phyla");
    TEST_EQUAL(plural("plexus"),         "plexuses");
    TEST_EQUAL(plural("polis"),          "polises");
    TEST_EQUAL(plural("pontifex"),       "pontifices");
    TEST_EQUAL(plural("potato"),         "potatoes");
    TEST_EQUAL(plural("pro"),            "pros");
    TEST_EQUAL(plural("prolegomenon"),   "prolegomena");
    TEST_EQUAL(plural("prospectus"),     "prospectuses");
    TEST_EQUAL(plural("proton"),         "protons");
    TEST_EQUAL(plural("quantum"),        "quanta");
    TEST_EQUAL(plural("quarto"),         "quartos");
    TEST_EQUAL(plural("radius"),         "radii");
    TEST_EQUAL(plural("rhino"),          "rhinos");
    TEST_EQUAL(plural("rhinoceros"),     "rhinoceroses");
    TEST_EQUAL(plural("rostrum"),        "rostra");
    TEST_EQUAL(plural("sassafras"),      "sassafrases");
    TEST_EQUAL(plural("seraph"),         "seraphim");
    TEST_EQUAL(plural("silex"),          "silices");
    TEST_EQUAL(plural("simplex"),        "simplices");
    TEST_EQUAL(plural("sinus"),          "sinuses");
    TEST_EQUAL(plural("soliloquy"),      "soliloquies");
    TEST_EQUAL(plural("solo"),           "solos");
    TEST_EQUAL(plural("soprano"),        "sopranos");
    TEST_EQUAL(plural("space"),          "spaces");
    TEST_EQUAL(plural("spectrum"),       "spectra");
    TEST_EQUAL(plural("speculum"),       "specula");
    TEST_EQUAL(plural("sphex"),          "sphexes");
    TEST_EQUAL(plural("stadium"),        "stadia");
    TEST_EQUAL(plural("status"),         "statuses");
    TEST_EQUAL(plural("stratum"),        "strata");
    TEST_EQUAL(plural("stylo"),          "stylos");
    TEST_EQUAL(plural("stylus"),         "styli");
    TEST_EQUAL(plural("succubus"),       "succubi");
    TEST_EQUAL(plural("tempo"),          "tempos");
    TEST_EQUAL(plural("torus"),          "tori");
    TEST_EQUAL(plural("trapezium"),      "trapezia");
    TEST_EQUAL(plural("trellis"),        "trellises");
    TEST_EQUAL(plural("trilby"),         "trilbys");
    TEST_EQUAL(plural("ultimatum"),      "ultimata");
    TEST_EQUAL(plural("umbilicus"),      "umbilici");
    TEST_EQUAL(plural("uterus"),         "uteri");
    TEST_EQUAL(plural("vacuum"),         "vacua");
    TEST_EQUAL(plural("velum"),          "vela");
    TEST_EQUAL(plural("vertebra"),       "vertebrae");
    TEST_EQUAL(plural("vertex"),         "vertices");
    TEST_EQUAL(plural("vortex"),         "vortices");
    TEST_EQUAL(plural("wish"),           "wishes");
    TEST_EQUAL(plural("witch"),          "witches");
    TEST_EQUAL(plural("wolf"),           "wolves");
    TEST_EQUAL(plural("word"),           "words");
    TEST_EQUAL(plural("Abscissa"),       "Abscissae");
    TEST_EQUAL(plural("Bacterium"),      "Bacteria");
    TEST_EQUAL(plural("Caddis"),         "Caddises");
    TEST_EQUAL(plural("Dais"),           "Daises");
    TEST_EQUAL(plural("Efreet"),         "Efreeti");
    TEST_EQUAL(plural("ABSCISSA"),       "ABSCISSAE");
    TEST_EQUAL(plural("BACTERIUM"),      "BACTERIA");
    TEST_EQUAL(plural("CADDIS"),         "CADDISES");
    TEST_EQUAL(plural("DAIS"),           "DAISES");
    TEST_EQUAL(plural("EFREET"),         "EFREETI");

}

void test_crow_english_lorem_ipsum() {

    static constexpr int max_iterations = 50;

    std::minstd_rand rng(42);
    std::string s;

    TRY(s = LoremIpsum(0)(rng));
    TEST(s.empty());

    for (int i = 1; i <= max_iterations; ++i) {

        int bytes = i * i;
        TRY(s = LoremIpsum(bytes)(rng));

        int size = int(s.size());
        TEST(size >= bytes);
        TEST(size <= bytes + 15);

        if (size >= 11)
            TEST(s.starts_with("Lorem ipsum"));

    }

    for (int i = 1; i <= max_iterations; ++i) {

        int bytes = i * i;
        TRY(s = LoremIpsum(bytes, false)(rng));

        int size = int(s.size());
        TEST(size >= bytes);
        TEST(size <= bytes + 15);

        if (size >= 11)
            TEST(s.starts_with("Lorem ipsum"));

    }

}
