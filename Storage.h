#pragma once

#include "afxdialogex.h"


namespace StackParser 
{
	//void saveTreeCtrl(CTreeCtrl &tree, HTREEITEM hTreeItem, int deep);
	void saveTreeCtrl(CTreeCtrl& tree);
	void loadTreeCtrl(CTreeCtrl& tree);
}