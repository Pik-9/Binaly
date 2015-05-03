#include "Hexedit.hpp"

#include <math.h>
#include <stdlib.h>
#include <QTableWidgetItem>

const unsigned int HexWidget::bases[4] = {2, 8, 10, 16};

HexWidget::HexWidget (QWidget* parent)
  : QWidget (parent), stream (NULL), currentBlockData (0)
{
  lyt = new QGridLayout (this);
  c_encoding = new QComboBox ();
  hex_table = new QTableWidget ();
  
  c_encoding->addItem (tr ("Binary"));
  c_encoding->addItem (tr ("Octal"));
  c_encoding->addItem (tr ("Decimal"));
  c_encoding->addItem (tr ("Hexadecimal"));
  c_encoding->setCurrentIndex (3);
  hex_table->setColumnCount (33);
  
  lyt->addWidget (c_encoding, 0, 1, 1, 1);
  lyt->addWidget (hex_table, 1, 0, 1, 3);
  
  connect (c_encoding, SIGNAL (currentIndexChanged (int)), this, SLOT (encodingChanged (int)));
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
  currentBlockInfo = stream->getBlockAt (position);
  currentBlockData = stream->getBlockDataAt (position);
  printData ();
}

void HexWidget::printData ()
{
  hex_table->setRowCount (ceil (currentBlockData.size () / 16.0));
  for (unsigned int pos = 0; pos < currentBlockData.size (); ++pos)  {
    div_t tablePos = div (pos, 16);
    if (!tablePos.rem)  {
      QTableWidgetItem *indexfield = new QTableWidgetItem (
        QString ().setNum (currentBlockInfo.begin + pos, bases[c_encoding->currentIndex ()])
      );
      QFont boldFont;
      boldFont.setBold (true);
      indexfield->setFont (boldFont);
      indexfield->setTextAlignment (Qt::AlignCenter);
      indexfield->setFlags (indexfield->flags () ^ Qt::ItemIsEditable);
      hex_table->setItem (tablePos.quot, 0, indexfield);
    }
    QTableWidgetItem *hexfield = new QTableWidgetItem (QString ().setNum (
      (unsigned int) (currentBlockData[pos] & 255), bases[c_encoding->currentIndex ()])
    );
    QTableWidgetItem *asciifield = new QTableWidgetItem (QString (printableMatch (currentBlockData[pos])));
    
    hexfield->setForeground (Qt::black);
    hexfield->setTextAlignment (Qt::AlignCenter);
    hexfield->setFlags (hexfield->flags () ^ Qt::ItemIsEditable);
    asciifield->setForeground (Qt::gray);
    asciifield->setTextAlignment (Qt::AlignCenter);
    asciifield->setFlags (asciifield->flags () ^ Qt::ItemIsEditable);
    
    hex_table->setItem (tablePos.quot, tablePos.rem + 1, hexfield);
    hex_table->setItem (tablePos.quot, tablePos.rem + 17, asciifield);
  }
  hex_table->resizeColumnsToContents ();
  hex_table->resizeRowsToContents ();
}

void HexWidget::encodingChanged (int newVal)
{
  if (stream)  {
    printData ();
  }
}