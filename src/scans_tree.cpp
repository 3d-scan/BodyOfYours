#include "scans_tree.h"

ScansTree::ScansTree(QTreeView* tree_view, QString tree_root)
  : tree_view_(tree_view),
    model_(new QFileSystemModel()),
    root_(tree_root)
{
  QString data_root_path = tree_root;
  model_->setRootPath(data_root_path);
  tree_view_->setModel(model_);
  tree_view_->setRootIndex(model_->index(data_root_path));
  tree_view_->setColumnWidth(0, 150);
  tree_view_->setColumnWidth(1, 50);
  tree_view_->setColumnWidth(2, 70);
  tree_view_->setColumnWidth(3, 100);
}

ScansTree::~ScansTree()
{

}
