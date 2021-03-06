#pragma once

#include "MeshDef.h"

#include <wrap/io_trimesh/import.h>
#include <vcg/complex/algorithms/update/component_ep.h>

#include <QFile>
#include <QString>

namespace MeshProcessing {
// #MeshProcessing
// Loading data
template <typename MeshT>
bool loadMeshFromFile(const QString& filename, MeshT* outMesh, bool cleanData = false);
template <typename QualityMapT>
bool loadQualityMapFromFile(const QString& filename, QualityMapT& outQualityMap);

// Processing data
template<typename MeshT, typename QualityMapT>
bool computeDifference(const MeshT* refMesh, const MeshT* compMesh, QualityMapT& outQualityMap);

template <typename MeshT>
bool loadMeshFromFile(const QString& filename, MeshT* outMesh, bool cleanData /*= false*/)
{
  int err = vcg::tri::io::Importer<MeshT>::Open(*outMesh, qPrintable(filename));
  if (err) {
    qDebug() << "[ERROR@MeshProcessing] Error in reading" << filename << ":" << vcg::tri::io::Importer<MeshT>::ErrorMsg(err);
    if (vcg::tri::io::Importer<MeshT>::ErrorCritical(err)) qDebug() << "[CRIT@MeshProcessing] It is a very serious error.";
    delete outMesh;
    return false;
  }
  vcg::tri::UpdateNormal<MeshT>::PerVertexNormalized(*outMesh);
  qDebug() << "[INFO@MeshProcessing] Successfully read mesh" << filename;
  if (cleanData) {
    int numDuplicated = vcg::tri::Clean<MeshT>::RemoveDuplicateVertex(*outMesh);
    int numUnref = vcg::tri::Clean<MeshT>::RemoveUnreferencedVertex(*outMesh);
    qDebug() << "Removed" << numDuplicated << "duplicate and" << numUnref << "unreferenced vertices from mesh" << filename;
  }
  return true;
}

template <typename QualityMapT>
bool MeshProcessing::loadQualityMapFromFile(const QString& filename, QualityMapT& outQualityMap)
{
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly)) return false;
  QDataStream in(&file);
  in >> outQualityMap;
  return true;
}

template<typename MeshT, typename QualityMapT>
bool computeDifference(const MeshT* refMesh, const MeshT* compMesh, QualityMapT& outQualityMap)
{

}

// ~MeshProcessing
}

aiColor4D toOGLColor(vcg::Color4<unsigned char>& color);
void computeDifference(CMesh& reference, CMesh& mesh, CMesh& out);
void computeMirror(CMesh& reference, CMesh& mesh, CMesh& out);
void flipMeshXAxis(CMesh& mesh);
void flipMeshXAxis(CMesh& base, CMesh& out);
void retriveQualityFromMesh(CMesh* mesh, QVector<float>& quality);
void applyQualityToMesh(CMesh& mesh, const QVector<float>& quality);

void generateRandomQualityForMesh(const CMesh& mesh, QVector<float>& qualityOut);
void saveQualityToFile(QString filePath, const QVector<float>& quality);

void createDummyFile(QString filePath);

template<typename T>
QString toString(vcg::Point3<T> point) {
  return QString("(%1, %2, %3)").arg(point.X()).arg(point.Y()).arg(point.Z());
}
