#include "Hexedit.hpp"

const unsigned int HexWidget::bases[4] = {2, 8, 10, 16};

HexWidget::HexWidget (QWidget* parent)
  : QWidget (parent)
{
  lyt = new QGridLayout (this);
  c_encoding = new QComboBox ();
  t_hexedit = new QTextEdit ();
  t_asciiedit = new QTextEdit ();
  
  c_encoding->addItem (tr ("Binary"));
  c_encoding->addItem (tr ("Octal"));
  c_encoding->addItem (tr ("Decimal"));
  c_encoding->addItem (tr ("Hexadecimal"));
  c_encoding->setCurrentIndex (3);
  t_hexedit->setReadOnly (true);
  t_asciiedit->setReadOnly (true);
  
  lyt->addWidget (c_encoding, 0, 2, 1, 2);
  lyt->addWidget (t_hexedit, 1, 0, 1, 3);
  lyt->addWidget (t_asciiedit, 1, 3, 1, 3);
}

HexWidget::~HexWidget ()
{}

QChar HexWidget::printableMatch (const char bare)
{
  QChar RET (bare);
  if ((bare < 32) || (bare == 127) || (bare == 255))  {
    RET = QChar ('.');
  }
  return RET;
}

void HexWidget::setFileStream (Hexfile* filestream)
{
  stream = filestream;
}

void HexWidget::loadData (const uint64_t position)
{
  t_asciiedit->clear ();
  t_hexedit->clear ();
  std::vector<char> block = stream->getBlockDataAt (position);
  QString hex_line, ascii_line;
  for (unsigned int ii = 0; ii < block.size (); ++ii)  {
    unsigned char cc = block[ii] & 255;
    hex_line.append (QString ().setNum (cc, bases[c_encoding->currentIndex ()]));
    ascii_line.append (printableMatch (cc));
    if ((ii + 1) % 16)  {
      hex_line.append (QChar (' '));
      ascii_line.append (QChar (' '));
    } else  {
      t_hexedit->append (hex_line);
      t_asciiedit->append (ascii_line);
      hex_line.clear ();
      ascii_line.clear ();
    }
  }
}