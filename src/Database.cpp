#include "Database.h"
#include "PatientData.h"
#include "ExaminationData.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Database::Database(QString db_name, QString db_type /*= "QSQLITE"*/)
{
  db_ = QSqlDatabase::addDatabase(db_type);
  db_.setDatabaseName(db_name);
  if (!db_.open()) {
    qDebug() << "Couldn't open database " << db_name << " (" << db_type << ")";
  }
}

void Database::createScheme()
{
  QSqlQuery query;
  query.exec(
    "CREATE TABLE patient ("
    "id INTEGER PRIMARY KEY, "
    "name varchar(100), "
    "surname varchar(100),"
    "pesel varchar(100))");
  query.exec("CREATE TABLE examination ("
    "id INTEGER PRIMARY KEY, "
    "name varchar(100),"
    "patient_id INTEGER,"
    "FOREIGN KEY(patient_id) REFERENCES patient(id))");
  query.exec("CREATE TABLE scan ("
    "id INTEGER PRIMARY KEY,"
    "exam_id INTEGER,"
    "name varchar(500),"
    "filename varchar(100),"
    "FOREIGN KEY(exam_id) REFERENCES examination(id))");
  query.exec("CREATE TABLE scan_diff ("
    "id INTEGER PRIMARY KEY,"
    "ref_id INTEGER,"
    "comp_id INTEGER,"
    "filename varchar(100),"
    "FOREIGN KEY(ref_id) REFERENCES scan(id),"
    "FOREIGN KEY(comp_id) REFERENCES scan(id))");
}

bool Database::insertPatient(PatientData data)
{
  QSqlQuery query;
  query.prepare("INSERT INTO patient (name, surname, pesel) "
    "VALUES (:name, :surname, :pesel)");
  query.bindValue(":name", data.name);
  query.bindValue(":surname", data.surname);
  query.bindValue(":pesel", data.pesel);
  return query.exec();
}

bool Database::insertPatient(PatientData in, PatientData& out)
{
  QSqlQuery query;
  query.prepare("INSERT INTO patient (name, surname, pesel) VALUES (:name, :surname, :pesel)");
  query.bindValue(":name", in.name);
  query.bindValue(":surname", in.surname);
  query.bindValue(":pesel", in.pesel);
  if (!query.exec()) {
    qDebug() << "[ERROR] Can't insert patient.";
    return false;
  }
  return Database::selectLastPatient(out);
}

bool Database::deletePatient(int id)
{
  bool status;
  // remove patient's examinations
  QSqlQuery remove_exam;
  remove_exam.prepare("DELETE FROM examination WHERE patient_id = :patient_id");
  remove_exam.bindValue(":patient_id", id);
  status = remove_exam.exec();
  // remove patient
  QSqlQuery remove_patient;
  remove_patient.prepare("DELETE FROM patient WHERE id = :id");
  remove_patient.bindValue(":id", id);
  return status && remove_patient.exec();
}

QList<PatientData> Database::selectPatient()
{
  QList<PatientData> list;
  QSqlQuery query("SELECT id, name, surname, pesel FROM patient");
  while (query.next()) {
    PatientData patient;
    patient.id = query.value(0).toInt();
    patient.name = query.value(1).toString();
    patient.surname = query.value(2).toString();
    patient.pesel = query.value(3).toString();
    list.push_back(patient);
  }
  return list;
}

bool Database::selectPatient(int id, PatientData& out)
{
  QSqlQuery query;
  query.prepare("SELECT id, name, surname, pesel FROM patient WHERE id = :id");
  query.bindValue(":id", id);
  query.exec();
  if (query.next()) {
    out.id = query.value(0).toInt();
    out.name = query.value(1).toString();
    out.surname = query.value(2).toString();
    out.pesel = query.value(3).toString();
    return true;
  }
  else {
    qDebug() << "[WARNING] Invalid patient's ID: " << id;
  }
  if (query.next()) {
    qDebug() << "[WARNING] More than one patient with ID: " << id;
  }
  return false;
}

bool Database::updatePatientOnly(PatientData data)
{
  QSqlQuery query;
  query.prepare(
    "UPDATE patient SET "
      "name = :name,"
      "surname = :surname,"
      "pesel = :pesel "
    "WHERE id = :id");
  query.bindValue(":id", data.id);
  query.bindValue(":name", data.name);
  query.bindValue(":surname", data.surname);
  query.bindValue(":pesel", data.pesel);
  return query.exec();
}

bool Database::insertExamination(ExaminationData in, ExaminationData& out)
{
  QSqlQuery query;
  query.prepare("INSERT INTO examination (name, scan_name, patient_id) "
    "VALUES (:name, :scan_name, :patient_id)");
  query.bindValue(":name", in.name);
  query.bindValue(":scan_name", in.scan_name);
  query.bindValue(":patient_id", in.patient_id);
  if (!query.exec()) {
    qDebug() << "[ERROR] Can't insert examination.";
    return false;
  }
  return Database::selectLastExamination(out);
}

bool Database::deleteExamination(int id)
{
  QSqlQuery query;
  query.prepare("DELETE FROM examination WHERE id = :id");
  query.bindValue(":id", id);
  return query.exec();
}

QList<ExaminationData> Database::selectExamination(int patient_id)
{
  QList<ExaminationData> list;
  QSqlQuery query;
  query.prepare("SELECT id, name, scan_name FROM examination WHERE patient_id = :patient_id");
  query.bindValue(":patient_id", patient_id);
  query.exec();
  while (query.next()) {
    ExaminationData exam;
    exam.id = query.value(0).toInt();
    exam.patient_id = patient_id;
    exam.name = query.value(1).toString();
    exam.scan_name = query.value(2).toString();
    list.push_back(exam);
  }
  return list;
}

bool Database::selectExamination(int id, ExaminationData& out)
{
  QSqlQuery query;
  query.prepare("SELECT id, name, scan_name, patient_id FROM examination WHERE id = :id");
  query.bindValue(":id", id);
  query.exec();
  if (query.next()) {
    out.id = query.value(0).toInt();
    out.name = query.value(1).toString();
    out.scan_name = query.value(2).toString();
    out.patient_id = query.value(3).toInt();
    return true;
  }
  else {
    qDebug() << "[WARNING] Invalid examination's ID: " << id;
  }
  if (query.next()) {
    qDebug() << "[WARNING] More than one examination with ID: " << id;
  }
  return false;
}

bool Database::selectLastPatient(PatientData& out)
{
  QSqlQuery query("SELECT max(id) FROM patient");
  if (query.next()) {
    return Database::selectPatient(query.value(0).toInt(), out);
  }
  qDebug() << "[WARNING] No patients.";
  return false;
}

bool Database::selectLastExamination(ExaminationData& out)
{
  QSqlQuery query("SELECT max(id) FROM examination");
  if (query.next()) {
    return Database::selectExamination(query.value(0).toInt(), out);
  }
  qDebug() << "[WARNING] No examinations.";
  return false;
}
