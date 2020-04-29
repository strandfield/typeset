// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/parsing/mathparserfrontend.h"

#include "tex/math/stylechange.h"

#include "tex/mathchars.h"

#include <cassert>
#include <stdexcept>

namespace tex
{

namespace parsing
{

MathParserFrontend::MathParserFrontend()
  : m_mathcode_table{MathCode::DefaultTable}
{
}

MathParser& MathParserFrontend::parser()
{
  return m_parser;
}

int MathParserFrontend::fam() const
{
  return m_fam;
}

const std::map<std::string, MathParserFrontend::CS>& MathParserFrontend::csmap()
{
  static const std::map<std::string, MathParserFrontend::CS> map = {
    {"left", CS::LEFT},
    {"right", CS::RIGHT},
    {"over", CS::OVER},
    {"frac", CS::FRAC},
    {"sqrt", CS::SQRT},
    {"matrix", CS::MATRIX},
    {"cr", CS::CR},
    {"textstyle", CS::TEXTSTYLE},
    {"scriptstyle", CS::SCRIPTSTYLE},
    {"scriptscriptstyle", CS::SCRIPTSCRIPTSTYLE},
  };

  return map;
}

const std::map<std::string, std::pair<int, MathCode>>& MathParserFrontend::symbolsmap()
{
  static const std::map<std::string, std::pair<int, MathCode>> map = {
     /* Greek  letters */
     {"alpha",           {tex::mathchars::GREEK_SMALL_LETTER_ALPHA,       MathCode(0x10B)}},
     {"beta",            {tex::mathchars::GREEK_SMALL_LETTER_BETA,        MathCode(0x010C)}},
     {"gamma",           {tex::mathchars::GREEK_SMALL_LETTER_GAMMA,       MathCode(0x010D)}},
     {"delta",           {tex::mathchars::GREEK_SMALL_LETTER_DELTA,       MathCode(0x010E)}},
     {"epsilon",         {tex::mathchars::GREEK_LUNATE_EPSILON_SYMBOL,    MathCode(0x010F)}},
     {"varepsilon",      {tex::mathchars::GREEK_SMALL_LETTER_EPSILON,     MathCode(0x0122)}},
     {"zeta",            {tex::mathchars::GREEK_SMALL_LETTER_ZETA,        MathCode(0x0110)}},
     {"eta",             {tex::mathchars::GREEK_SMALL_LETTER_ETA,         MathCode(0x0111)}},
     {"theta",           {tex::mathchars::GREEK_SMALL_LETTER_THETA,       MathCode(0x0112)}},
     {"vartheta",        {tex::mathchars::GREEK_THETA_SYMBOL,             MathCode(0x0123)}},
     {"iota",            {tex::mathchars::GREEK_SMALL_LETTER_IOTA,        MathCode(0x0113)}},
     {"kappa",           {tex::mathchars::GREEK_SMALL_LETTER_KAPPA,       MathCode(0x0114)}},
     {"lambda",          {tex::mathchars::GREEK_SMALL_LETTER_LAMDA,       MathCode(0x0115)}},
     {"mu",              {tex::mathchars::GREEK_SMALL_LETTER_MU,          MathCode(0x0116)}},
     {"nu",              {tex::mathchars::GREEK_SMALL_LETTER_NU,          MathCode(0x0117)}},
     {"xi",              {tex::mathchars::GREEK_SMALL_LETTER_XI,          MathCode(0x0118)}},
     {"pi",              {tex::mathchars::GREEK_SMALL_LETTER_PI,          MathCode(0x0119)}},
     {"varpi",           {tex::mathchars::GREEK_PI_SYMBOL,                MathCode(0x0124)}},
     {"rho",             {tex::mathchars::GREEK_SMALL_LETTER_RHO,         MathCode(0x011A)}},
     {"varrho",          {tex::mathchars::GREEK_RHO_SYMBOL,               MathCode(0x0125)}},
     {"sigma",           {tex::mathchars::GREEK_SMALL_LETTER_SIGMA,       MathCode(0x011B)}},
     {"varsigma",        {tex::mathchars::GREEK_SMALL_LETTER_FINAL_SIGMA, MathCode(0x0126)}},
     {"tau",             {tex::mathchars::GREEK_SMALL_LETTER_TAU,         MathCode(0x011C)}},
     {"upsilon",         {tex::mathchars::GREEK_SMALL_LETTER_UPSILON,     MathCode(0x011D)}},
     {"phi",             {tex::mathchars::GREEK_PHI_SYMBOL,               MathCode(0x011E)}},
     {"varphi",          {tex::mathchars::GREEK_SMALL_LETTER_PHI,         MathCode(0x0127)}},
     {"chi",             {tex::mathchars::GREEK_SMALL_LETTER_CHI,         MathCode(0x011F)}},
     {"psi",             {tex::mathchars::GREEK_SMALL_LETTER_PSI,         MathCode(0x0120)}},
     {"omega",           {tex::mathchars::GREEK_SMALL_LETTER_OMEGA,       MathCode(0x0121)}},
     /*  Uppercase Greek letters */
     {"Gamma",           {tex::mathchars::GREEK_CAPITAL_LETTER_GAMMA,     MathCode(0x7000)}},
     {"Delta",           {tex::mathchars::GREEK_CAPITAL_LETTER_DELTA,     MathCode(0x7001)}},
     {"Theta",           {tex::mathchars::GREEK_CAPITAL_LETTER_THETA,     MathCode(0x7002)}},
     {"Lambda",          {tex::mathchars::GREEK_CAPITAL_LETTER_LAMDA,     MathCode(0x7003)}},
     {"Xi",              {tex::mathchars::GREEK_CAPITAL_LETTER_XI,        MathCode(0x7004)}},
     {"Pi",              {tex::mathchars::GREEK_CAPITAL_LETTER_PI,        MathCode(0x7005)}},
     {"Sigma",           {tex::mathchars::GREEK_CAPITAL_LETTER_SIGMA,     MathCode(0x7006)}},
     {"Upsilon",         {tex::mathchars::GREEK_CAPITAL_LETTER_UPSILON,   MathCode(0x7007)}},
     {"Phi",             {tex::mathchars::GREEK_CAPITAL_LETTER_PHI,       MathCode(0x7008)}},
     {"Psi",             {tex::mathchars::GREEK_CAPITAL_LETTER_PSI,       MathCode(0x7009)}},
     {"Omega",           {tex::mathchars::GREEK_CAPITAL_LETTER_OMEGA,     MathCode(0x700A)}},
     /* Miscellaneous symbols of type Ord */
     {"aleph",           {tex::mathchars::ALEF_SYMBOL,                    MathCode(0x0240)}},
     {"imath",           {tex::mathchars::LATIN_SMALL_LETTER_DOTLESS_I,   MathCode(0x017B)}},
     {"jmath",           {tex::mathchars::LATIN_SMALL_LETTER_DOTLESS_J,   MathCode(0x017C)}},
     {"ell",             {tex::mathchars::SCRIPT_SMALL_L,                 MathCode(0x0160)}},
     {"wp",              {tex::mathchars::SCRIPT_CAPITAL_P,               MathCode(0x017D)}},
     {"Re",              {tex::mathchars::BLACK_LETTER_CAPITAL_R,         MathCode(0x023C)}},
     {"Im",              {tex::mathchars::BLACK_LETTER_CAPITAL_I,         MathCode(0x023D)}},
     {"partial",         {tex::mathchars::PARTIAL_DIFFERENTIAL,           MathCode(0x0140)}},
     {"infty",           {tex::mathchars::INFINITY,                       MathCode(0x0231)}},
     {"prime",           {tex::mathchars::PRIME,                          MathCode(0x0230)}},
     {"emptyset",        {tex::mathchars::EMPTY_SET,                      MathCode(0x023B)}},
     {"nabla",           {tex::mathchars::NABLA,                          MathCode(0x0272)}},
     {"top",             {tex::mathchars::DOWN_TACK,                      MathCode(0x023E)}},
     {"bot",             {tex::mathchars::UP_TACK,                        MathCode(0x023F)}},
     {"triangle",        {tex::mathchars::WHITE_UP_POINTING_TRIANGLE,     MathCode(0x0234)}},
     {"forall",          {tex::mathchars::FOR_ALL,                        MathCode(0x0238)}},
     {"exists",          {tex::mathchars::THERE_EXISTS,                   MathCode(0x0239)}},
     {"neg",             {tex::mathchars::NOT_SIGN,                       MathCode(0x023A)}},
     {"flat",            {tex::mathchars::MUSIC_FLAT_SIGN,                MathCode(0x015B)}},
     {"natural",         {tex::mathchars::MUSIC_NATURAL_SIGN,             MathCode(0x015C)}},
     {"sharp",           {tex::mathchars::MUSIC_SHARP_SIGN,               MathCode(0x015D)}},
     {"clubsuit",        {tex::mathchars::BLACK_CLUB_SUIT,                MathCode(0x027C)}},
     {"diamondsuit",     {tex::mathchars::WHITE_DIAMOND_SUIT,             MathCode(0x027D)}},
     {"spadesuit",       {tex::mathchars::BLACK_SPADE_SUIT,               MathCode(0x027F)}},
     /* Large operators */
     {"sum",             {tex::mathchars::GREEK_CAPITAL_LETTER_SIGMA,     MathCode(0x1350)}},
     {"prod",            {tex::mathchars::GREEK_CAPITAL_LETTER_PI,        MathCode(0x1351)}},
     {"coprod",          {tex::mathchars::N_ARY_COPRODUCT,                MathCode(0x1360)}},
     {"bigcap",          {tex::mathchars::INTERSECTION,                   MathCode(0x1354)}},
     {"bigcup",          {tex::mathchars::UNION,                          MathCode(0x1353)}},
     {"bigsqcup",        {tex::mathchars::SQUARE_CUP,                     MathCode(0x1346)}},
     {"bigvee",          {tex::mathchars::LOGICAL_OR,                     MathCode(0x1357)}},
     {"bigwedge",        {tex::mathchars::LOGICAL_AND,                    MathCode(0x1356)}},
     {"bigodot",         {tex::mathchars::CIRCLED_DOT_OPERATOR,           MathCode(0x134A)}},
     {"bigotimes",       {tex::mathchars::CIRCLED_TIMES,                  MathCode(0x134E)}},
     {"bigoplus",        {tex::mathchars::CIRCLED_MINUS,                  MathCode(0x134C)}},
     {"biguplus",        {tex::mathchars::MULTISET_UNION,                 MathCode(0x1355)}},
     /* Binary operations */
     {"pm",              {tex::mathchars::PLUS_MINUS_SIGN,                MathCode(0x2206)}},
     {"mp",              {tex::mathchars::MINUS_OR_PLUS_SIGN,             MathCode(0x2207)}},
     {"setminus",        {tex::mathchars::SET_MINUS,                      MathCode(0x226E)}},
     {"cdot",            {tex::mathchars::CIRCLED_DOT_OPERATOR,           MathCode(0x2201)}},
     {"times",           {tex::mathchars::MULTIPLICATION_SIGN,            MathCode(0x2202)}},
     {"ast",             {tex::mathchars::ASTERISK_OPERATOR,              MathCode(0x2203)}},
     {"star",            {tex::mathchars::STAR_OPERATOR,                  MathCode(0x213F)}},
     {"diamond",         {tex::mathchars::DIAMOND_OPERATOR,               MathCode(0x2205)}},
     {"circ",            {tex::mathchars::LARGE_CIRCLE,                   MathCode(0x220E)}},
     {"bullet",          {tex::mathchars::BLACK_LARGE_CIRCLE,             MathCode(0x220F)}},
     {"div",             {tex::mathchars::DIVISION_SIGN,                  MathCode(0x2204)}},
     {"cap",             {tex::mathchars::INTERSECTION,                   MathCode(0x225C)}},
     {"cup",             {tex::mathchars::UNION,                          MathCode(0x225B)}},
     {"uplus",           {tex::mathchars::MULTISET_UNION,                 MathCode(0x225D)}},
     {"sqcap",           {tex::mathchars::SQUARE_CAP,                     MathCode(0x2275)}},
     {"sqcup",           {tex::mathchars::SQUARE_CUP,                     MathCode(0x2274)}},
     {"triangleleft",    {tex::mathchars::NORMAL_SUBGROUP_OF,             MathCode(0x212F)}},
     {"triangleright",   {tex::mathchars::WHITE_RIGHT_POINTING_TRIANGLE,  MathCode(0x212E)}},
     {"wr",              {tex::mathchars::WREATH_PRODUCT,                 MathCode(0x226F)}},
     {"bigcirc",         {tex::mathchars::LARGE_CIRCLE,                   MathCode(0x220D)}},
     {"bigtriangleup",   {tex::mathchars::WHITE_UP_POINTING_TRIANGLE,     MathCode(0x2234)}},
     {"bigtriangledown", {tex::mathchars::WHITE_DOWN_POINTING_TRIANGLE,   MathCode(0x2235)}},
     {"vee",             {tex::mathchars::LOGICAL_OR,                     MathCode(0x225F)}},
     {"wedge",           {tex::mathchars::LOGICAL_AND,                    MathCode(0x225E)}},
     {"oplus",           {tex::mathchars::CIRCLED_PLUS,                   MathCode(0x2208)}},
     {"ominus",          {tex::mathchars::CIRCLED_MINUS,                  MathCode(0x2209)}},
     {"otimes",          {tex::mathchars::CIRCLED_TIMES,                  MathCode(0x220A)}},
     {"oslash",          {tex::mathchars::CIRCLED_DIVISION_SLASH,         MathCode(0x220B)}},
     {"odot",            {tex::mathchars::CIRCLED_DOT_OPERATOR,           MathCode(0x220C)}},
     {"dagger",          {tex::mathchars::DAGGER,                         MathCode(0x2279)}},
     {"ddagger",         {tex::mathchars::DOUBLE_DAGGER,                  MathCode(0x227A)}},
     {"amalg",           {tex::mathchars::N_ARY_COPRODUCT,                MathCode(0x2271)}},
     /* Relations */
     {"leq",             {tex::mathchars::LESS_THAN_OR_EQUAL_TO,          MathCode(0x3214)}},
     {"prec",            {tex::mathchars::PRECEDES,                       MathCode(0x321E)}},
     {"preceq",          {tex::mathchars::PRECEDES_OR_EQUAL_TO,           MathCode(0x3216)}},
     {"ll",              {tex::mathchars::MUCH_LESS_THAN,                 MathCode(0x321C)}},
     {"subset",          {tex::mathchars::SUBSET_OF,                      MathCode(0x321A)}},
     {"subseteq",        {tex::mathchars::SUBSET_OF_OR_EQUAL_TO,          MathCode(0x3212)}},
     {"sqsubseteq",      {tex::mathchars::SQUARE_IMAGE_OF_OR_EQUAL_TO,    MathCode(0x3276)}},
     {"in",              {tex::mathchars::ELEMENT_OF,                     MathCode(0x3232)}},
     {"vdash",           {tex::mathchars::RIGHT_TACK,                     MathCode(0x3260)}},
     {"smile",           {tex::mathchars::SMILE,                          MathCode(0x315E)}},
     {"frown",           {tex::mathchars::FROWN,                          MathCode(0x315F)}},
     {"geq",             {tex::mathchars::GREATER_THAN_OR_EQUAL_TO,       MathCode(0x3215)}},
     {"succ",            {tex::mathchars::SUCCEEDS,                       MathCode(0x321F)}},
     {"succeq",          {tex::mathchars::SUCCEEDS_OR_EQUAL_TO,           MathCode(0x3217)}},
     {"gg",              {tex::mathchars::MUCH_GREATER_THAN,              MathCode(0x321D)}},
     {"supset",          {tex::mathchars::SUPERSET_OF,                    MathCode(0x321B)}},
     {"supseteq",        {tex::mathchars::SUPERSET_OF_OR_EQUAL_TO,        MathCode(0x3213)}},
     {"sqsupseteq",      {tex::mathchars::SQUARE_IMAGE_OF_OR_EQUAL_TO,    MathCode(0x3277)}},
     {"ni",              {tex::mathchars::NOT_AN_ELEMENT_OF,              MathCode(0x3233)}},
     {"dashv",           {tex::mathchars::LEFT_TACK,                      MathCode(0x3261)}},
     {"mid",             {tex::mathchars::DIVIDES,                        MathCode(0x326A)}},
     {"parallel",        {tex::mathchars::PARALLEL_TO,                    MathCode(0x326B)}},
     {"equiv",           {tex::mathchars::IDENTICAL_TO,                   MathCode(0x3211)}},
     {"sim",             {tex::mathchars::TILDE_OPERATOR,                 MathCode(0x3218)}},
     {"simeq",           {tex::mathchars::ASYMPTOTICALLY_EQUAL_TO,        MathCode(0x3227)}},
     {"asymp",           {tex::mathchars::EQUIVALENT_TO,                  MathCode(0x3210)}},
     {"approx",          {tex::mathchars::ALMOST_EQUAL_TO,                MathCode(0x3219)}},
     {"propto",          {tex::mathchars::PROPORTIONAL_TO,                MathCode(0x322F)}},
     {"perp",            {tex::mathchars::PERPENDICULAR,                  MathCode(0x323F)}},
  };

  return map;
}

MathParserFrontend::CS MathParserFrontend::cs(const std::string& name)
{
  auto it = csmap().find(name);

  if (it == csmap().end())
    throw std::runtime_error{ "Unknown control sequence" };

  return it->second;
}

void MathParserFrontend::writeControlSequence(CS cs)
{
  switch (cs)
  {
  case CS::LEFT:
    return parser().left();
  case CS::RIGHT:
    return parser().right();
  case CS::OVER:
    return parser().over();
  case CS::FRAC:
    return parser().frac();
  case CS::SQRT:
    return parser().sqrt();
  case CS::MATRIX:
    return parser().matrix();
  case CS::CR:
    return parser().cr();
  case CS::TEXTSTYLE:
    return parser().textstyle();
  case CS::SCRIPTSTYLE:
    return parser().scriptstyle();
  case CS::SCRIPTSCRIPTSTYLE:
    return parser().scriptscriptstyle();
  default:
    assert(false);
  }
}

void MathParserFrontend::writeControlSequence(const std::string& csname)
{
  auto it = csmap().find(csname);

  if (it != csmap().end())
  {
    writeControlSequence(it->second);
  }
  else
  {
    auto char_it = symbolsmap().find(csname);

    if(char_it == symbolsmap().end())
      throw std::runtime_error{ "Unknown control sequence" };

    return writeMathChar(char_it->second.first, char_it->second.second);
  }
}

void MathParserFrontend::writeChar(char c)
{
  MathCode mc = m_mathcode_table[static_cast<uint8_t>(c)];
  writeMathChar(Character(c), mc);
}

void MathParserFrontend::writeSymbol(Character c)
{
  if (c < 128)
  {
    writeChar(c);
  }
  else
  {
    throw std::runtime_error{ "MathParserFrontend does not support unicode char yet" };
  }
}

void MathParserFrontend::writeSymbol(Character c, int class_num, int fam)
{
  auto mathsym = std::make_shared<MathSymbol>(Character(c), class_num, fam);
  parser().writeSymbol(mathsym);
}

void MathParserFrontend::writeMathChar(Character c, MathCode mc)
{
  int class_num = mc.c();
  int f = mc.f();

  if (class_num == 7)
  {
    class_num = 0;
    f = fam() != -1 ? fam() : f;
  }

  auto mathsym = std::make_shared<MathSymbol>(c, class_num, f);
  parser().writeSymbol(mathsym);
}

void MathParserFrontend::writeSymbol(std::shared_ptr<MathSymbol> mathsym)
{
  parser().writeSymbol(mathsym);
}

void MathParserFrontend::writeBox(const std::shared_ptr<tex::Box>& box)
{
  parser().writeBox(box);
}

void MathParserFrontend::beginSuperscript()
{
  parser().beginSuperscript();
}

void MathParserFrontend::beginSubscript()
{
  parser().beginSubscript();
}

void MathParserFrontend::beginMathList()
{
  parser().beginMathList();
}

void MathParserFrontend::endMathList()
{
  parser().endMathList();
}

void MathParserFrontend::alignmentTab()
{
  parser().alignmentTab();
}

void MathParserFrontend::finish()
{
  parser().finish();
}

MathList& MathParserFrontend::output()
{
  return parser().output();
}

} // namespace parsing

} // namespace tex
