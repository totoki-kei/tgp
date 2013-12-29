#include "WindowTest.h"

#include "Debug.h"

Game* GenerateGame() {
	return new WindowTest();
}

#define TEST_ID 1

#if TEST_ID == 0
WindowTest::WindowTest(void) : wnd(640, 480)
{
	wnd.SetWindowTitle(TEXT("ほげほげほげ"));
	GameWindow::SetMessageHandler(
		WM_KEYDOWN,
		[this](UINT msg, WPARAM wp, LPARAM lp) {
			DBG_OUT("msg = %08X, WPARAM = %08X, LPARAM = %08X\n", msg, wp, lp);
			
			if(wp == VK_ESCAPE) {
				this->wnd.SetWindowTitle("Escaped.");
				GameWindow::ResetMessageHandler(WM_KEYDOWN);
			}

			return 0;
		} 
	);

	GameWindow::SetMessageHandler(
		WM_MOUSEMOVE,
		[this](UINT msg, WPARAM wp, LPARAM lp) {
			DBG_OUT("[%10d] msg = %08X, WPARAM = %08X, LPARAM = %08X\n", GetMessageTime(), msg, wp, lp);
			
			return 0;
		} 
	);

}


WindowTest::~WindowTest(void)
{
	wnd.Dispose();
}

int WindowTest::Initialize(void){
	wnd.Initialize();
	d3d10 = new D3DCore(&wnd);
	d3d10->Initialize();
	return 0;
}

void WindowTest::Update(void) {

	auto ret = wnd.ProcessMessage();
	if(ret != 0) exitLoop = true;

	// ここでフレームごとの処理を行う
	d3d10->Clear();
	d3d10->Update();
	
}

void WindowTest::Draw(void) {

}

#elif TEST_ID == 1

#include "D3DEffect.h"
#include "Effect_Effect1.h"

WindowTest::WindowTest(void) : wnd(640, 480)
{
	wnd.SetWindowTitle(TEXT("ほげほげほげ"));
	GameWindow::SetMessageHandler(
		WM_KEYDOWN,
		[this](UINT msg, WPARAM wp, LPARAM lp) {
		DBG_OUT("msg = %08X, WPARAM = %08X, LPARAM = %08X\n", msg, wp, lp);

		if (wp == VK_ESCAPE) {
			this->wnd.SetWindowTitle("Escaped.");
			GameWindow::ResetMessageHandler(WM_KEYDOWN);
		}

		return 0;
	}
	);

	GameWindow::SetMessageHandler(
		WM_MOUSEMOVE,
		[this](UINT msg, WPARAM wp, LPARAM lp) {
		DBG_OUT("[%10d] msg = %08X, WPARAM = %08X, LPARAM = %08X\n", GetMessageTime(), msg, wp, lp);

		return 0;
	}
	);

}


D3DEffect* effect;

WindowTest::~WindowTest(void)
{
	delete effect;
	effect = nullptr;
	wnd.Dispose();
}

int WindowTest::Initialize(void){
	wnd.Initialize();
	d3d10 = new D3DCore(&wnd);
	d3d10->Initialize();

	// ためしにエフェクトを読んでみよう
	effect = new D3DEffect(d3d10, _T("test"), g_Effect_Effect1, sizeof(g_Effect_Effect1));

	return 0;
}

void WindowTest::Update(void) {

	auto ret = wnd.ProcessMessage();
	if (ret != 0) exitLoop = true;

	// ここでフレームごとの処理を行う
	d3d10->Clear();
	d3d10->Update();

}

void WindowTest::Draw(void) {

}

#endif
