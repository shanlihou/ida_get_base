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
		HTREEITEM hItem = pNMTreeView->itemNew.hItem;		//��ȡ��ʼ�϶���Item�ľ��!!!!!!!
		if (hItem == GetRootItem())                             //�������϶����ڵ�
		{
			*pResult = 0;
			return;
		}

		m_hBeginDrag = hItem;                        //��¼�϶��������Ŀ

		if (!m_isInit) {
			m_isInit = TRUE;
			m_imageList.Create(32, 32, ILC_COLOR32, 0, 4);
			m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
		}

		SetImageList(&m_imageList, TVSIL_NORMAL);
		m_pDragImages = CreateDragImage(hItem);      //�����϶���ͼ���б�
		SetImageList(NULL, TVSIL_NORMAL);
		
		CPoint dragPt;                               //��¼�϶���ʼ��
		dragPt.x = pNMTreeView->ptDrag.x;
		dragPt.y = pNMTreeView->ptDrag.y;
		msg("hitem:%x, image:%x\n", hItem, m_pDragImages);
		if (m_pDragImages != NULL)
		{
			m_pDragImages->BeginDrag(0, CPoint(8, 8));  //��ʼ�϶�ͼ��                  ��1��
			ClientToScreen(&dragPt);                  //ת���ͻ����굽��Ļ����
			m_pDragImages->DragEnter(this, dragPt);   //���϶��Ĺ�������ʾ�϶���ͼ��    ��2��
			SetCapture();                             //��ʼ��겶׽
			m_bDrag = TRUE;                           //�����ʾ��ǰ״̬���϶�����
		}
		*pResult = 0;
	}
	void CDragTreeCtrl::OnMouseMove(UINT nFlags, CPoint point)
	{
		if (m_bDrag)                        //�����϶�״̬
		{
			HTREEITEM hItem;
			UINT nHitFlags;
			CRect clientRc;
			GetClientRect(&clientRc);       //��ȡ�ͻ���
			m_pDragImages->DragMove(point); //�����϶���ͼ��λ��Ϊ���λ�ã�����

			//������꾭��ʱ������ʾ
			if ((hItem = HitTest(point, &nHitFlags)) != NULL)
			{
				CImageList::DragShowNolock(FALSE);			//�����϶���ͼ��
				SelectDropTarget(hItem);					//����ѡ�е���Ŀ
				CImageList::DragShowNolock(TRUE);			//��ʾ�϶���ͼ��
			}
		}
		else
			CTreeCtrl::OnMouseMove(nFlags, point);//��������϶�״̬���ʹ�����ͨ��mouse move

	}
	void CDragTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point)
	{
		if (m_bDrag == TRUE)   //�����϶�״̬�����򲻽��в���
		{
			m_bDrag = FALSE;                 //�����϶�״̬
			CImageList::DragLeave(this);     //�����϶�ͼ�������ڽ��и���            ��3��
			CImageList::EndDrag();           //����ͼ����϶�                          ��4��
			ReleaseCapture();	                 //�ͷ���겶׽
			delete m_pDragImages;			 //�ͷ�ͼ���б�
			m_pDragImages = NULL;
			CRect winRC;
			GetWindowRect(&winRC);			 //��ȡ��������

			HTREEITEM hItem;                 //���ڻ�ȡ���봦��Item
			if ((hItem = HitTest(point, &nFlags)) != NULL)   //���봦��Item
			{
				//ȷ���Ϸź�� item���Ϸ�ʵ�ֹ���

				if ((hItem != m_hBeginDrag) && (hItem != GetParentItem(m_hBeginDrag)))
					//���Ŀ����Ŀ�뿪ʼ�϶�����Ŀ��ͬ����Ŀ����Ŀ���ǿ�ʼ��Ŀ�ĸ��ڵ㣬�����д���
				{
					CopyNodes(hItem, m_hBeginDrag);            //��ɽڵ�ĸ���
					DeleteItem(m_hBeginDrag);                 //ɾ��ԭ���ط���Item
				}
				Invalidate();                                 //ˢ�´��ڣ��ػ棩
				SelectDropTarget(NULL);                       //ȡ������Item��ѡ��״̬
				m_hBeginDrag = NULL;
			}
			StackParser::saveTreeCtrl(*this);
		}
	}

	void CDragTreeCtrl::CopyNodes(HTREEITEM hDesItem, HTREEITEM hSrcItem)
	{
		if (hDesItem == NULL || hSrcItem == NULL)				//ItemΪ���򲻽��в���
		{
			return;
		}
		TVITEM tvItem;							//������Ŀ��Ϣ�ṹ��
		tvItem.mask = TVIF_TEXT | TVIF_IMAGE;				//���÷��ر��
		tvItem.hItem = hSrcItem;
		wchar_t chText[MAX_PATH] = { 0 };
		tvItem.pszText = chText;
		tvItem.cchTextMax = MAX_PATH;
		GetItem(&tvItem);				    //��ȡ��Ŀ��Ϣ
		TVINSERTSTRUCT tvInsert;			    //���������������ݽṹ
		tvInsert.hParent = hDesItem;                        //�������ĸ��ڵ�
		tvInsert.item = tvItem;                             //���������ӽڵ�
		HTREEITEM hInsert = InsertItem(&tvInsert);          //����Item������ȡ������֮���Item�µľ��

		//��������Item���������ItemҲ�����
		HTREEITEM hChild = GetChildItem(hSrcItem);		    //��ȡ�ӽڵ�
		while (hChild != NULL)
		{
			CopyNodes(hInsert, hChild);			    //�ݹ����   �����ˣ�����
			hChild = GetNextSiblingItem(hChild);                //��ȡ��һ���ֵܽڵ�
		}
	}
}