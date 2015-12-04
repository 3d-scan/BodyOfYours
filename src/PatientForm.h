#pragma once

#include "Form.h"
#include <QWidget>
#include <QLineEdit>

struct PatientData;

class PatientForm : public Form
{
  Q_OBJECT

public:
  explicit PatientForm(QWidget *parent = 0);
  ~PatientForm();

  void fill(const PatientData& data);
  void clear();

public slots:
  void onButtonClicked(int button);

signals:
  void savePatient(PatientData data);
  void modifyPatient(PatientData data);
  void removeCurrentPatient();

private:
  QLineEdit* name;
  QLineEdit* surname;
  QLineEdit* pesel;
  int id;

  PatientData getData();
};
