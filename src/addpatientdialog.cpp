#include "addpatientdialog.h"
#include "ui_addpatientdialog.h"

AddPatientDialog::AddPatientDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::AddPatientDialog),
  only_update_(false)
{
  ui->setupUi(this);
  setWindowTitle("Create patient");
}

AddPatientDialog::AddPatientDialog(Patient patient, QWidget *parent /*= 0*/)
  : QDialog(parent),
    ui(new Ui::AddPatientDialog),
    only_update_(true),
    updated_patient_(patient)
{
  ui->setupUi(this);
  setWindowTitle("Update patient");
  ui->nameText->setText(patient.name());
  ui->surnameText->setText(patient.surname());
  ui->additionalText->setText(patient.additional_info());
  ui->sexyComboBox->setCurrentIndex((patient.sex() ==  FEMALE) ? 0 : 1);
}

AddPatientDialog::~AddPatientDialog()
{
  delete ui;
}

void AddPatientDialog::on_addPatientButton_clicked()
{
  Patient patient;
  patient.setName(ui->nameText->text());
  patient.setSurname(ui->surnameText->text());
  patient.setAdditionalInfo(ui->additionalText->toPlainText());
  bool is_female = ui->sexyComboBox->currentText() == "Female";
  patient.setSex(is_female ? FEMALE : MALE);

  if (!patient.name().isEmpty()) {
    if (!only_update_) {
      emit CreatePatientSignal(patient);
    } else {
      patient.setId(updated_patient_.id());
      emit UpdatePatientSignal(patient);
    }
    ClearData();
    close();
  }
}

void AddPatientDialog::on_cancelAddPatientButton_clicked()
{
  ClearData();
  close();
}

void AddPatientDialog::ClearData()
{
  ui->nameText->clear();
  ui->surnameText->clear();
  ui->additionalText->clear();
  ui->sexyComboBox->setCurrentIndex(0);
}