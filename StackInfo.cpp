// StackInfo.cpp: 实现文件
//

#include "pch.h"
#include "ida_get_base.h"
#include "StackInfo.h"


// StackInfo 对话框

IMPLEMENT_DYNAMIC(StackInfo, CDialogEx)

StackInfo::StackInfo(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_stackFrameStr(_T(""))
	, m_lastHItem(NULL)
{

}

StackInfo::~StackInfo()
{
}

void StackInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, stackStrEdit);
	DDX_Text(pDX, IDC_EDIT1, m_stackFrameStr);
	DDX_Control(pDX, IDC_STACK_TREE, m_stackTree);
}


BEGIN_MESSAGE_MAP(StackInfo, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &StackInfo::OnClickedTest)
	ON_NOTIFY(NM_CLICK, IDC_STACK_TREE, &StackInfo::OnClickStackTree)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &StackInfo::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &StackInfo::OnBnClickedButtonDel)
END_MESSAGE_MAP()


// StackInfo 消息处理程序


void StackInfo::OnClickedTest()
{
	// TODO: 在此添加控件通知处理程序代码
	m_stackTree.DeleteAllItems();

	CString frameStr;
	stackStrEdit.GetWindowText(frameStr);
	string _s = (CStringA)frameStr;
	auto vs = StackParser::explode(_s, string("->"));
	for (auto& frame : vs) {
		if (frame.size() >= 6) {
			CString _cs(frame.c_str());
			auto root = m_stackTree.InsertItem(_cs, TVI_ROOT);
		}
	}
	StackParser::saveTreeCtrl(m_stackTree);
	m_lastHItem = NULL;
}

void StackInfo::loadTree()
{
	StackParser::loadTreeCtrl(m_stackTree);
}

bool StackInfo::isFrameItem(HTREEITEM hItem) {
	if (!hItem) {
		return false;
	}

	return m_stackTree.GetParentItem(hItem) == NULL ? true : false;
}


void StackInfo::OnClickStackTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	CPoint pt = GetCurrentMessage()->pt;
	// TODO: 在此添加控件通知处理程序代码
	m_stackTree.ScreenToClient(&pt);

	UINT uFlags = 0;
	HTREEITEM hItem = m_stackTree.HitTest(pt, &uFlags);//然后做点击测试
			 //HitTest一般是指鼠标点击位置上是哪个树结点,从而得到鼠标位置的树结点
	if ((hItem != NULL) && (TVHT_ONITEM & uFlags))//如果点击的位置是在节点位置上
	{
		m_stackTree.SelectItem(hItem);
		if (isFrameItem(hItem)) 
		{
			CString text = m_stackTree.GetItemText(hItem);
			string _s = (CStringA)text;
			auto ea = StackParser::getAddrByStr(_s);
			jumpto(ea);
			m_lastHItem = hItem;
		}
		else {
			CString text = m_stackTree.GetItemText(hItem);
			string _s = StackParser::cs2s(text);
			auto vs = StackParser::explode(_s, string("-"));
			auto ea = StackParser::getAddrByStr(vs[0]);
			jumpto(ea);
		}
		*pResult = 0;
	}
}

void StackInfo::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_lastHItem)
	{
		ea_t ea = get_screen_ea();
		char* curline = get_curline();
		char buf[128];
		qstring buffer;
		tag_remove(&buffer, curline, 0);
		qsnprintf(buf, 128, "0x%x-%s", ea, buffer.c_str());
		CString oprStr(buf);
		m_stackTree.InsertItem(oprStr, m_lastHItem);
		m_stackTree.Expand(m_lastHItem, TVE_EXPAND);
		StackParser::saveTreeCtrl(m_stackTree);
	}
}


void StackInfo::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	auto sel = m_stackTree.GetSelectedItem();
	if (!sel) {
		return;
	}

	if (isFrameItem(sel)) {
		return;
	}

	m_stackTree.DeleteItem(sel);
	StackParser::saveTreeCtrl(m_stackTree);
}
