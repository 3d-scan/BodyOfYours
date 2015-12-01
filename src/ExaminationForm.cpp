#include "ExaminationForm.h"
#include "ExaminationData.h"

#include <QDebug>

ExaminationForm::ExaminationForm(QWidget *parent)
  : Form(parent)
{
  name = new QLineEdit(this);
  scan_name = new QLineEdit(this);
  form_->addRow(tr("Nazwa"), name);
  form_->addRow(tr("Plik skanu"), scan_name);
  connect(buttons_, SIGNAL(buttonClicked(int)), this, SLOT(onButtonClicked(int)));
}

ExaminationForm::~ExaminationForm()
{
  delete name;
  delete scan_name;
}

void ExaminationForm::fill(const ExaminationData& data)
{
  name->setText(data.name);
  scan_name->setText(data.scan_name);
}

void ExaminationForm::clear()
{
  name->clear();
  scan_name->clear();
}

void ExaminationForm::onButtonClicked(int button)
{
  switch (button) {
  case FormButtons::SAVE: {
    ExaminationData data;
    data.name = name->text();
    data.scan_name = scan_name->text();
    emit saveExam(data);
    break;
  }
  case FormButtons::CLEAR:
    clear();
    break;
  case FormButtons::CANCEL:
    break;
  case FormButtons::LOCK:
    break;
  case FormButtons::UNLOCK:
    break;
  default:
    break;
  }
  qDebug() << "ExaminationForm::onButtonClicked()";
}