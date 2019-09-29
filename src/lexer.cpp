// Copyright (C) 2019 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/lexer.h"

namespace tex
{

namespace parsing
{

const Lexer::CatCodeTable Lexer::DefaultCatCodes = {
  CharCategory::Invalid, // NUL
  CharCategory::Invalid, // SOH
  CharCategory::Invalid, // STX
  CharCategory::Invalid, // ETX
  CharCategory::Invalid, // EOT
  CharCategory::Invalid, // ENQ
  CharCategory::Invalid, // ACK
  CharCategory::Invalid, // BELL
  CharCategory::Invalid, // BS
  CharCategory::Space, // TAB
  CharCategory::EndOfLine, // LF
  CharCategory::Ignored, // VT
  CharCategory::Active, // FF
  CharCategory::EndOfLine, // CR
  CharCategory::Ignored, // SO
  CharCategory::Ignored, // SI
  CharCategory::Ignored, // DLE
  CharCategory::Ignored, // DC1
  CharCategory::Ignored, // DC2
  CharCategory::Ignored, // DC3
  CharCategory::Ignored, // DC4
  CharCategory::Ignored, // NAK
  CharCategory::Ignored, // SYN
  CharCategory::Ignored, // ETB
  CharCategory::Ignored, // CAN
  CharCategory::Ignored, // EM
  CharCategory::Ignored, // SUB
  CharCategory::Ignored, // ESC
  CharCategory::Ignored, // FS
  CharCategory::Ignored, // GS
  CharCategory::Ignored, // RS
  CharCategory::Ignored, // US
  CharCategory::Space, // ' '
  CharCategory::Other, // '!'
  CharCategory::Other, // '"'
  CharCategory::Parameter, // '#'
  CharCategory::MathShift, // '$'
  CharCategory::Comment,  // '%'
  CharCategory::AlignmentTab, // '&'
  CharCategory::Other, // '\''
  CharCategory::Other, // '('
  CharCategory::Other, // ')'
  CharCategory::Other, // '*'
  CharCategory::Other, // '+'
  CharCategory::Other, // ','
  CharCategory::Other, // '-'
  CharCategory::Other, // '.'
  CharCategory::Other, // '/'
  CharCategory::Other, // '0'
  CharCategory::Other, // '1'
  CharCategory::Other, // '2'
  CharCategory::Other, // '3'
  CharCategory::Other, // '4'
  CharCategory::Other, // '5'
  CharCategory::Other, // '6'
  CharCategory::Other, // '7'
  CharCategory::Other, // '8'
  CharCategory::Other, // '9'
  CharCategory::Other, // ':'
  CharCategory::Other, // ';'
  CharCategory::Other, // '<'
  CharCategory::Other, // '='
  CharCategory::Other, // '>'
  CharCategory::Other, // '?'
  CharCategory::Other, // '@'
  CharCategory::Letter, // 'A'
  CharCategory::Letter, // 'B'
  CharCategory::Letter, // 'C'
  CharCategory::Letter, // 'D'
  CharCategory::Letter, // 'E'
  CharCategory::Letter, // 'F'
  CharCategory::Letter, // 'G'
  CharCategory::Letter, // 'H'
  CharCategory::Letter, // 'I'
  CharCategory::Letter, // 'J'
  CharCategory::Letter, // 'K'
  CharCategory::Letter, // 'L'
  CharCategory::Letter, // 'M'
  CharCategory::Letter, // 'N'
  CharCategory::Letter, // 'O'
  CharCategory::Letter, // 'P'
  CharCategory::Letter, // 'Q'
  CharCategory::Letter, // 'R'
  CharCategory::Letter, // 'S'
  CharCategory::Letter, // 'T'
  CharCategory::Letter, // 'U'
  CharCategory::Letter, // 'V'
  CharCategory::Letter, // 'W'
  CharCategory::Letter, // 'X'
  CharCategory::Letter, // 'Y'
  CharCategory::Letter, // 'Z'
  CharCategory::Other, // '['
  CharCategory::Escape, // '\'
  CharCategory::Other, // ']'
  CharCategory::Superscript, // ^
  CharCategory::Subscript, // _
  CharCategory::Other, // '`'
  CharCategory::Letter, // 'a'
  CharCategory::Letter, // 'b'
  CharCategory::Letter, // 'c'
  CharCategory::Letter, // 'd'
  CharCategory::Letter, // 'e'
  CharCategory::Letter, // 'f'
  CharCategory::Letter, // 'g'
  CharCategory::Letter, // 'h'
  CharCategory::Letter, // 'i'
  CharCategory::Letter, // 'j'
  CharCategory::Letter, // 'k'
  CharCategory::Letter, // 'l'
  CharCategory::Letter, // 'm'
  CharCategory::Letter, // 'n'
  CharCategory::Letter, // 'o'
  CharCategory::Letter, // 'p'
  CharCategory::Letter, // 'q'
  CharCategory::Letter, // 'r'
  CharCategory::Letter, // 's'
  CharCategory::Letter, // 't'
  CharCategory::Letter, // 'u'
  CharCategory::Letter, // 'v'
  CharCategory::Letter, // 'w'
  CharCategory::Letter, // 'x'
  CharCategory::Letter, // 'y'
  CharCategory::Letter, // 'z'
  CharCategory::GroupBegin, // '{'
  CharCategory::Other, // '|'
  CharCategory::GroupEnd, // '}'
  CharCategory::Active, // '~'
  CharCategory::Ignored, // DEL
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
  CharCategory::Other,
};

} // namespace parsing

} // namespace tex
