#pragma once
#include "afxdialogex.h"
#include <vector>
#include <string>
#include "CDragTreeCtrl.h"

using std::string;
using std::vector;
// StackInfo 对话框

class StackInfo : public CDialogEx
{
	DECLARE_DYNAMIC(StackInfo)

public:
	StackInfo(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~StackInfo();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	bool StackInfo::isFrameItem(HTREEITEM hItem);
public:
	afx_msg void OnClickedTest();
	void loadTree();
	afx_msg void OnClickStackTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButtonAdd();
private:
	CString m_stackFrameStr;
	StackParser::CDragTreeCtrl m_stackTree;
	HTREEITEM m_lastHItem;
public:
	CEdit stackStrEdit;
	afx_msg void OnBnClickedButtonDel();
};
