#pragma once

#include "afxdialogex.h"

namespace StackParser {

    class CDragTreeCtrl: public CTreeCtrl 
    {
        DECLARE_DYNAMIC(CDragTreeCtrl)
        afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);  //�϶���ʼ�Ĵ�������
        afx_msg void OnMouseMove(UINT nFlags, CPoint point);        //����ƶ��Ĵ�������
        afx_msg void OnLButtonUp(UINT nFlags, CPoint point);        //�������̧��Ĵ�������
    protected:
        DECLARE_MESSAGE_MAP()
        HTREEITEM m_hBeginDrag;         //��¼�϶�����Item
        CImageList m_imageList;
        CImageList* m_pDragImages;	//�϶���ͼ���б�
        BOOL m_bDrag;                   //�����ж��Ƿ�����϶�
        BOOL m_isInit;

        void CopyNodes(HTREEITEM hDesItem, HTREEITEM hSrcItem);
    public:
    };
}