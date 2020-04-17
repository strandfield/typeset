// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/mathcode.h"

namespace tex
{

const MathCode::Table MathCode::DefaultTable = {
  MathCode(0), // NUL
  MathCode(0), // SOH
  MathCode(0), // STX
  MathCode(0), // ETX
  MathCode(0), // EOT
  MathCode(0), // ENQ
  MathCode(0), // ACK
  MathCode(0), // BELL
  MathCode(0), // BS
  MathCode(0), // TAB
  MathCode(0), // LF
  MathCode(0), // VT
  MathCode(0), // FF
  MathCode(0), // CR
  MathCode(0), // SO
  MathCode(0), // SI
  MathCode(0), // DLE
  MathCode(0), // DC1
  MathCode(0), // DC2
  MathCode(0), // DC3
  MathCode(0), // DC4
  MathCode(0), // NAK
  MathCode(0), // SYN
  MathCode(0), // ETB
  MathCode(0), // CAN
  MathCode(0), // EM
  MathCode(0), // SUB
  MathCode(0), // ESC
  MathCode(0), // FS
  MathCode(0), // GS
  MathCode(0), // RS
  MathCode(0), // US
  MathCode(0), // ' '
  MathCode(0x5021), // '!'
  MathCode(34), // '"'
  MathCode(35), // '#'
  MathCode(36), // '$'
  MathCode(37),  // '%'
  MathCode(38), // '&'
  MathCode(0x8000), // '\''
  MathCode(0x4028), // '('
  MathCode(0x5029), // ')'
  MathCode(0x2203), // '*'
  MathCode(0x202B), // '+'
  MathCode(0x613B), // ','
  MathCode(0x2200), // '-'
  MathCode(0x013A), // '.'
  MathCode(0x013D), // '/'
  MathCode(0x7030), // '0'
  MathCode(0x7031), // '1'
  MathCode(0x7032), // '2'
  MathCode(0x7033), // '3'
  MathCode(0x7034), // '4'
  MathCode(0x7035), // '5'
  MathCode(0x7036), // '6'
  MathCode(0x7037), // '7'
  MathCode(0x7038), // '8'
  MathCode(0x7039), // '9'
  MathCode(0x303A), // ':'
  MathCode(0x603B), // ';'
  MathCode(0x313C), // '<'
  MathCode(0x303D), // '='
  MathCode(0x313E), // '>'
  MathCode(0x503F), // '?'
  MathCode(64), // '@'
  MathCode(0x7141), // 'A'
  MathCode(0x7142), // 'B'
  MathCode(0x7143), // 'C'
  MathCode(0x7144), // 'D'
  MathCode(0x7145), // 'E'
  MathCode(0x7146), // 'F'
  MathCode(0x7147), // 'G'
  MathCode(0x7148), // 'H'
  MathCode(0x7149), // 'I'
  MathCode(0x714A), // 'J'
  MathCode(0x714B), // 'K'
  MathCode(0x714C), // 'L'
  MathCode(0x714D), // 'M'
  MathCode(0x714E), // 'N'
  MathCode(0x714F), // 'O'
  MathCode(0x7150), // 'P'
  MathCode(0x7151), // 'Q'
  MathCode(0x7152), // 'R'
  MathCode(0x7153), // 'S'
  MathCode(0x7154), // 'T'
  MathCode(0x7155), // 'U'
  MathCode(0x7156), // 'V'
  MathCode(0x7157), // 'W'
  MathCode(0x7158), // 'X'
  MathCode(0x7159), // 'Y'
  MathCode(0x715A), // 'Z'
  MathCode(0x405B), // '['
  MathCode(0x026E), // '\'
  MathCode(0x505D), // ']'
  MathCode(94), // ^
  MathCode(0x8000), // _
  MathCode(96), // '`'
  MathCode(0x7161), // 'a'
  MathCode(0x7162), // 'b'
  MathCode(0x7163), // 'c'
  MathCode(0x7164), // 'd'
  MathCode(0x7165), // 'e'
  MathCode(0x7166), // 'f'
  MathCode(0x7167), // 'g'
  MathCode(0x7168), // 'h'
  MathCode(0x7169), // 'i'
  MathCode(0x716A), // 'j'
  MathCode(0x716B), // 'k'
  MathCode(0x716C), // 'l'
  MathCode(0x716D), // 'm'
  MathCode(0x716E), // 'n'
  MathCode(0x716F), // 'o'
  MathCode(0x7170), // 'p'
  MathCode(0x7171), // 'q'
  MathCode(0x7172), // 'r'
  MathCode(0x7173), // 's'
  MathCode(0x7174), // 't'
  MathCode(0x7175), // 'u'
  MathCode(0x7176), // 'v'
  MathCode(0x7177), // 'w'
  MathCode(0x7178), // 'x'
  MathCode(0x7179), // 'y'
  MathCode(0x717A), // 'z'
  MathCode(123), // '{'
  MathCode(0x026A), // '|'
  MathCode(125), // '}'
  MathCode(126), // '~'
  MathCode(127), // DEL
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
  MathCode(0),
};

} // namespace tex
