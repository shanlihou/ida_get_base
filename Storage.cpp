#include "pch.h"
#include <stack>

namespace StackParser {
	void saveTreeCtrl(CTreeCtrl &tree, HTREEITEM hTreeItem, int deep, FILE *fp)
	{
		HTREEITEM  hChildrenItem;
		HTREEITEM  hNextItem;

		if (!hTreeItem)
			return;

		CString _str = tree.GetItemText(hTreeItem);
		string itemStr = cs2s(_str);
		string indent(deep, '\t');
		qfprintf(fp, "%s%s\n", indent.c_str(), itemStr.c_str());

		hChildrenItem = tree.GetChildItem(hTreeItem);
		saveTreeCtrl(tree, hChildrenItem, deep + 1, fp);

		hNextItem = tree.GetNextItem(hTreeItem, TVGN_NEXT);
		saveTreeCtrl(tree, hNextItem, deep, fp);
	}

	void saveTreeCtrl(CTreeCtrl& tree)
	{
		auto root = tree.GetRootItem();
		FILE* fp = qfopen(TREE_SAVE_PATH, "w");
		saveTreeCtrl(tree, root, 0, fp);
		qfclose(fp);
	}

	void loadTreeCtrl(CTreeCtrl& tree) 
	{
		FILE* fp = qfopen(TREE_SAVE_PATH, "r");
		string line;
		int c = qfgetc(fp);
		int indent = 0;
		std::stack<HTREEITEM> treeStack;
		while (c != -1) {
			switch (c) {
			case '\n':
				while (treeStack.size() > indent) 
				{
					treeStack.pop();
				}

				HTREEITEM cur;
				if (indent == 0) {
					cur = tree.InsertItem(s2cs(line), TVI_ROOT);
				}
				else {
					cur = tree.InsertItem(s2cs(line), treeStack.top());
					tree.Expand(treeStack.top(), TVE_EXPAND);
				}
				treeStack.emplace(cur);
				line.clear();
				indent = 0;
				break;
			case '\t':
				indent++;
				break;
			default:
				line += c;
				break;
			}

			c = qfgetc(fp);
		}
		qfclose(fp);
	}
}