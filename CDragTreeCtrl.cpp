#include "pch.h"
#include "CDragTreeCtrl.h"
#include "resource.h"

namespace StackParser
{

	IMPLEMENT_DYNAMIC(CDragTreeCtrl, CTreeCtrl)

	BEGIN_MESSAGE_MAP(CDragTreeCtrl, CTreeCtrl)
		ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
		ON_WM_MOUSEMOVE()
		ON_WM_LBUTTONUP()
	END_MESSAGE_MAP()

	void CDragTreeCtrl::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult)
	{
		NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
		HTREEITEM hItem = pNMTreeView->itemNew.hItem;		//获取开始拖动的Item的句柄!!!!!!!
		if (hItem == GetRootItem())                             //不允许拖动根节点
		{
			*pResult = 0;
			return;
		}

		m_hBeginDrag = hItem;                        //记录拖动的起点项目

		if (!m_isInit) {
			m_isInit = TRUE;
			m_imageList.Create(32, 32, ILC_COLOR32, 0, 4);
			m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
		}

		SetImageList(&m_imageList, TVSIL_NORMAL);
		m_pDragImages = CreateDragImage(hItem);      //创建拖动的图像列表
		SetImageList(NULL, TVSIL_NORMAL);
		
		CPoint dragPt;                               //记录拖动起始点
		dragPt.x = pNMTreeView->ptDrag.x;
		dragPt.y = pNMTreeView->ptDrag.y;
		msg("hitem:%x, image:%x\n", hItem, m_pDragImages);
		if (m_pDragImages != NULL)
		{
			m_pDragImages->BeginDrag(0, CPoint(8, 8));  //开始拖动图像                  （1）
			ClientToScreen(&dragPt);                  //转换客户坐标到屏幕坐标
			m_pDragImages->DragEnter(this, dragPt);   //在拖动的过程中显示拖动的图像    （2）
			SetCapture();                             //开始鼠标捕捉
			m_bDrag = TRUE;                           //这里表示当前状态是拖动操作
		}
		*pResult = 0;
	}
	void CDragTreeCtrl::OnMouseMove(UINT nFlags, CPoint point)
	{
		if (m_bDrag)                        //处于拖动状态
		{
			HTREEITEM hItem;
			UINT nHitFlags;
			CRect clientRc;
			GetClientRect(&clientRc);       //获取客户区
			m_pDragImages->DragMove(point); //设置拖动的图像位置为鼠标位置！！！

			//设置鼠标经过时高亮显示
			if ((hItem = HitTest(point, &nHitFlags)) != NULL)
			{
				CImageList::DragShowNolock(FALSE);			//隐藏拖动的图像
				SelectDropTarget(hItem);					//设置选中的项目
				CImageList::DragShowNolock(TRUE);			//显示拖动的图像
			}
		}
		else
			CTreeCtrl::OnMouseMove(nFlags, point);//如果不在拖动状态，就触发普通的mouse move

	}
	void CDragTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point)
	{
		if (m_bDrag == TRUE)   //处于拖动状态，否则不进行操作
		{
			m_bDrag = FALSE;                 //结束拖动状态
			CImageList::DragLeave(this);     //隐藏拖动图像，允许窗口进行更新            （3）
			CImageList::EndDrag();           //结束图像的拖动                          （4）
			ReleaseCapture();	                 //释放鼠标捕捉
			delete m_pDragImages;			 //释放图像列表
			m_pDragImages = NULL;
			CRect winRC;
			GetWindowRect(&winRC);			 //获取窗口区域

			HTREEITEM hItem;                 //用于获取拖入处的Item
			if ((hItem = HitTest(point, &nFlags)) != NULL)   //拖入处有Item
			{
				//确定拖放后的 item的拖放实现过程

				if ((hItem != m_hBeginDrag) && (hItem != GetParentItem(m_hBeginDrag)))
					//如果目标项目与开始拖动的项目相同或者目标项目仍是开始项目的父节点，不进行处理
				{
					CopyNodes(hItem, m_hBeginDrag);            //完成节点的复制
					DeleteItem(m_hBeginDrag);                 //删除原来地方的Item
				}
				Invalidate();                                 //刷新窗口（重绘）
				SelectDropTarget(NULL);                       //取消移入Item的选中状态
				m_hBeginDrag = NULL;
			}
			StackParser::saveTreeCtrl(*this);
		}
	}

	void CDragTreeCtrl::CopyNodes(HTREEITEM hDesItem, HTREEITEM hSrcItem)
	{
		if (hDesItem == NULL || hSrcItem == NULL)				//Item为空则不进行操作
		{
			return;
		}
		TVITEM tvItem;							//定义项目信息结构体
		tvItem.mask = TVIF_TEXT | TVIF_IMAGE;				//设置返回标记
		tvItem.hItem = hSrcItem;
		wchar_t chText[MAX_PATH] = { 0 };
		tvItem.pszText = chText;
		tvItem.cchTextMax = MAX_PATH;
		GetItem(&tvItem);				    //获取项目信息
		TVINSERTSTRUCT tvInsert;			    //定义插入操作的数据结构
		tvInsert.hParent = hDesItem;                        //定义插入的父节点
		tvInsert.item = tvItem;                             //定义插入的子节点
		HTREEITEM hInsert = InsertItem(&tvInsert);          //插入Item，并获取插入完之后的Item新的句柄

		//接下来将Item里的所有子Item也插过来
		HTREEITEM hChild = GetChildItem(hSrcItem);		    //获取子节点
		while (hChild != NULL)
		{
			CopyNodes(hInsert, hChild);			    //递归调用   厉害了！！！
			hChild = GetNextSiblingItem(hChild);                //获取下一个兄弟节点
		}
	}
}