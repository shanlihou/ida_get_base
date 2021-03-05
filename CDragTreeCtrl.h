#pragma once

#include "afxdialogex.h"

namespace StackParser {

    class CDragTreeCtrl: public CTreeCtrl 
    {
        DECLARE_DYNAMIC(CDragTreeCtrl)
        afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);  //拖动开始的触发函数
        afx_msg void OnMouseMove(UINT nFlags, CPoint point);        //鼠标移动的触发函数
        afx_msg void OnLButtonUp(UINT nFlags, CPoint point);        //鼠标点击后抬起的触发函数
    protected:
        DECLARE_MESSAGE_MAP()
        HTREEITEM m_hBeginDrag;         //记录拖动起点的Item
        CImageList m_imageList;
        CImageList* m_pDragImages;	//拖动的图像列表
        BOOL m_bDrag;                   //用来判断是否进行拖动
        BOOL m_isInit;

        void CopyNodes(HTREEITEM hDesItem, HTREEITEM hSrcItem);
    public:
    };
}