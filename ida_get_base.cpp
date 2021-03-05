// ida_get_base.cpp: 定义 DLL 的初始化例程。
//

#include "pch.h"
#include "framework.h"
#include "ida_get_base.h"
#include "StackInfo.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为以下项中的第一个语句:
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CidagetbaseApp

BEGIN_MESSAGE_MAP(CidagetbaseApp, CWinApp)
END_MESSAGE_MAP()


// CidagetbaseApp 构造

CidagetbaseApp::CidagetbaseApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CidagetbaseApp 对象

CidagetbaseApp theApp;
StackInfo g_stackDialog;
bool g_isInit;
// CidagetbaseApp 初始化

BOOL CidagetbaseApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}



int idaapi init(void)
{
    //IDA在启动的时候会调用每个插件的init函数。
    //返回值有三种选项:
    //PLUGIN_SKIP适合那些不支持的插件，IDA将不会加载该插件
    //PLUGIN_OK适合那些执行一次性功能的插件
    //PLUGIN_KEEP适合那些需要一直保持功能的插件
    g_isInit = false;
    return PLUGIN_OK;
}

void idaapi term(void)
{
    //当结束插件时，一般您可以在此添加一点任务清理的代码。
    if (g_isInit) {
        g_stackDialog.DestroyWindow();
    }
    return;
}

bool idaapi run(size_t)
{
    //当按下热键时候,执行功能的入口函数
    if (!g_isInit) {
        g_isInit = true;
        g_stackDialog.Create(IDD_DIALOG1);
        g_stackDialog.loadTree();
    }
    g_stackDialog.ShowWindow(SW_NORMAL);
    return true;
}

static char comment[] = "It's a plugin to show Hello world!";

plugin_t PLUGIN =
{
  IDP_INTERFACE_VERSION,
  0,                    // 插件的一些属性,一般为0即可
  init,                 // initialize
  term,                 // terminate. this pointer may be NULL.
  run,                  // invoke plugin
  comment,              // 插件的说明,会显示在IDA下方的状态栏中
  "",                   // multiline help about the plugin
  "Hello, world",		// 插件在列表中显示的名称
  "Alt-F1"              // 插件想要注册的功能快捷键
};
