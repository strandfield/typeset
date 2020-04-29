// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'typeset' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tex/math/mathlist.h"

#include "tex/charbox.h"
#include "tex/glue.h"
#include "tex/hbox.h"
#include "tex/kern.h"
#include "tex/symbol.h"
#include "tex/vbox.h"

#include "tex/math/atom.h"
#include "tex/math/fraction.h"
#include "tex/math/matrix.h"
#include "tex/math/root.h"

namespace tex
{

struct RAIIPrefixGuard
{
  std::string& target;

  RAIIPrefixGuard(std::string& str, char c = '.')
    : target(str)
  {
    str.push_back(c);
  }

  ~RAIIPrefixGuard()
  {
    target.pop_back();
  }
};

struct ShowLists
{
  const List& list;
  std::string prefix;
  std::string result;

  ShowLists(const List& l)
    : list(l)
  {

  }

  void beginLine()
  {
    if(!result.empty())
      write('\n');
    write(prefix);
  }

  void write(char c)
  {
    result += c;
  }

  void write(const char* str)
  {
    result += str;
  }

  void write(const std::string& str)
  {
    result += str;
  }

  void write(int x)
  {
    result += std::to_string(x);
  }

  void write(float x)
  {
    result += std::to_string(x);
  }

  void write(const math::Atom& atom)
  {
    beginLine();

    switch (atom.type())
    {
    case math::Atom::Type::Ord:
      write("\\mathord");
      break;
    case math::Atom::Type::Bin:
      write("\\mathbin");
      break;
    case math::Atom::Type::Rel:
      write("\\mathrel");
      break;
    case math::Atom::Type::Open:
      write("\\mathopen");
      break;
    case math::Atom::Type::Close:
      write("\\mathclose");
      break;
    case math::Atom::Type::Punct:
      write("\\mathpunct");
      break;
    case math::Atom::Type::Inner:
      write("\\mathinner");
      break;
    case math::Atom::Type::Rad:
      write("\\radical");
      break;
    }

    if (atom.nucleus())
    {
      RAIIPrefixGuard guard{ prefix };
      write(atom.nucleus());
    }

    if (atom.superscript())
    {
      RAIIPrefixGuard guard{ prefix, '^' };
      write(atom.superscript());
    }
    
    if (atom.subscript())
    {
      RAIIPrefixGuard guard{ prefix, '_' };
      write(atom.subscript());
    }
  }

  void write(const math::Fraction& frac)
  {
    beginLine();
    write("\\fraction, thickness = default");

    {
      RAIIPrefixGuard guard{ prefix, '\\' };
      write(frac.numer());
    }

    {
      RAIIPrefixGuard guard{ prefix, '/' };
      write(frac.denom());
    }
  }

  void write(const math::Root& root)
  {
    beginLine();
    write("\\sqrt");

    {
      RAIIPrefixGuard guard{ prefix, '\\' };
      write(root.degree());
    }

    {
      RAIIPrefixGuard guard{ prefix, '/' };
      write(root.radicand());
    }
  }

  void write(const math::Matrix& m)
  {
    beginLine();
    write("\\matrix ");
    write(static_cast<int>(m.cols()));
    write("cols");

    for (size_t i(0); i < m.rows(); ++i)
    {

      for (size_t j(0); j < m.cols(); ++j)
      {
        RAIIPrefixGuard guard{ prefix, '.' };
        write(m.at(i, j));
        
        if (j != m.cols() - 1)
        {
          beginLine();
          write("&");
        }
      }

      beginLine();
      write(".cr");
    }
  }

  void write(const MathList& mlist)
  {
    for (const auto& n : mlist)
    {
      write(n);
    }
  }

  void write(const List& mlist)
  {
    for (const auto& n : mlist)
    {
      write(n);
    }
  }

  void write(const CharacterBox& box)
  {
    beginLine();
    write(Utf8Char{ box.character() }.data());
  }

  void writeBoxMetrics(const Box& box)
  {
    write('(');
    write(box.height());
    write('+');
    write(box.depth());
    write(')');
    write('x');
    write(box.width());
  }

  void write(const HBox& hbox)
  {
    beginLine();

    write("\\hbox");
    writeBoxMetrics(hbox);

    if (hbox.glueRatio() != 0.f)
    {
      write(", glue set ");
      write(hbox.glueRatio());
    }

    RAIIPrefixGuard guard{ prefix };
    write(hbox.list());
  }

  void write(const VBox& box)
  {
    beginLine();

    write("\\vbox");
    writeBoxMetrics(box);

    if (box.glueRatio() != 0.f)
    {
      write(", glue set ");
      write(box.glueRatio());
    }

    RAIIPrefixGuard guard{ prefix };
    write(box.list());
  }


  void write(GlueOrder order)
  {
    switch (order)
    {
    case tex::GlueOrder::Normal:
      break;
    case tex::GlueOrder::Fil:
      write("fil");
      break;
    case tex::GlueOrder::Fill:
      write("fill");
      break;
    case tex::GlueOrder::Filll:
      write("filll");
      break;
    default:
      break;
    }
  }

  void write(const Glue& g)
  {
    beginLine();
    write("\\glue ");
    write(g.space());

    if (g.stretch() != 0.f || g.shrink() != 0.f)
    {
      write(' ');
      write(g.stretch());
      write(g.stretchOrder());
      write(' ');
      write(g.shrink());
      write(g.shrinkOrder());
    }
  }

  void write(const Kern& k)
  {
    beginLine();
    write("\\kern ");
    write(k.space());
  }

  void write(const MathSymbol& msym)
  {
    beginLine();
    write("\\fam");
    write(msym.family());
    write(' ');
    write(Utf8Char{ msym.character() }.data());
  }

  void write(const std::shared_ptr<Node>& node)
  {
    if (node->isMathSymbol())
    {
      write(node->as<tex::MathSymbol>());
    }
    else if (node->isMathList())
    {
      const MathListNode& mln = node->as<MathListNode>();
      write(mln.list());
    }
    else if (node->isHBox())
    {
      write(node->as<HBox>());
    }
    else if (node->isVBox())
    {
      write(node->as<VBox>());
    }
    else if (node->isCharacterBox())
    {
      write(node->as<CharacterBox>());
    }
    else if (node->isGlue())
    {
      write(node->as<Glue>());
    }
    else if (node->isKern())
    {
      write(node->as<Kern>());
    }
    else if (node->isAtom())
    {
      write(node->as<math::Atom>());
    }
    else if (node->isFraction())
    {
      write(node->as<math::Fraction>());
    }
    else if (node->isRoot())
    {
      write(node->as<math::Root>());
    }
    else if (node->isMatrix())
    {
      write(node->as<math::Matrix>());
    }
    else if (node->isBox())
    {
      beginLine();
      writeBoxMetrics(node->as<Box>());
    }
    else
    {
      beginLine();
      write("\\alien");
    }
  }

  std::string to_string()
  {
    for (const auto& n : list)
    {
      write(n);
    }

    return std::string(std::move(result));
  }
};

std::string showlists(const List& list)
{
  ShowLists converter{ list };
  return converter.to_string();
}

} // namespace tex
