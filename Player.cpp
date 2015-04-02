#include "Player.h"

#include "Framework/Graphics/Model.h"
#include "Framework/Input/DIJoypad.h"
#include "Game1.h"

#include "Framework/Graphics/SpriteString.h"

using namespace Models;
using std::shared_ptr;
using std::unique_ptr;

namespace {
	D3DCore* core;

	Model* model;
	unique_ptr<DIJoypad> joypad;

	TCHAR filename_full[256] = { 0 };
	TCHAR filename[256] = { 0 };

	void SetDefaultMaterial() {
		MaterialData mat;
		mat.Blend = 0.0f;
		mat.LineWidth = 1 / 128.0f;

		mat.Color = XMFLOAT4{ 1, 1, 1, 1 };
		model->SetMaterial(0, mat);
	}

	void InitializeModel(D3DCore* core_){
		core = core_;

		model = Model::Load(_T("Content\\Model\\testmodel.txt"));
		_tcscpy_s(filename_full, _T("Content\\Model\\testmodel.txt"));
		_tcscpy_s(filename, _T("Content\\Model\\testmodel.txt"));

		SceneParameter &scp = Model::GetSceneParam();

		auto g = Game1::GetInstance();

		XMVECTOR eye = { 0, 0, 10, 1 };
		XMVECTOR lookat = { 0, 0, 0, 1 };
		XMVECTOR up = { 0, 1, 0, 1 };
		scp.Projection = XMMatrixPerspectiveLH(g->GetWindowWidth() / 3200.0 ,g->GetWindowHeight() / 3200.0, 0.25, 100);
		scp.View = XMMatrixLookAtLH(eye, lookat, up);
		scp.LightDirection = XMFLOAT3(1, -1, -1);
		scp.LightColor = XMFLOAT4(1, 1, 1, 1);
		scp.AmbientColor = XMFLOAT4{ 1, 1, 1, 0.5 };

		SetDefaultMaterial();
	}

	void ReloadModel(bool selectFile){

		if (selectFile){
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);         // 構造体のサイズ
			ofn.hwndOwner = Game1::GetInstance()->GetWindowHandle();                  // コモンダイアログの親ウィンドウハンドル
			ofn.lpstrFilter = _T("text(*.txt)\0*.txt\0All files(*.*)\0*.*\0\0"); // ファイルの種類
			ofn.lpstrFile = filename_full;         // 選択されたファイル名(フルパス)を受け取る変数のアドレス
			ofn.lpstrFileTitle = filename;         // 選択されたファイル名を受け取る変数のアドレス
			ofn.nMaxFile = sizeof(filename_full);  // lpstrFileに指定した変数のサイズ
			ofn.nMaxFileTitle = sizeof(filename);  // lpstrFileTitleに指定した変数のサイズ
			ofn.Flags = OFN_FILEMUSTEXIST;         // フラグ指定
			ofn.lpstrTitle = _T("ファイルを開く"); // コモンダイアログのキャプション
			ofn.lpstrDefExt = _T("txt");           // デフォルトのファイルの種類
			if (!GetOpenFileName(&ofn)){
				DBG_OUT("error : %08X", CommDlgExtendedError());
				return;
			}
		}

		delete model;
		model = Model::Load(filename_full);

		SetDefaultMaterial();


	}

	void TerminateModel(){
		delete model;
	}
}

Player::Player() : task()
{
}

Sprite::SpriteString* stringDrawer;


int Player::draw(Game1::task_type& task, Game1::task_param, void*){
	static float n = 0;

	InstanceData &obj = model->ReserveDraw();
	obj.World = XMMatrixRotationX(-n) * XMMatrixRotationY(-n / 2);
	obj.MaterialIndex = 0;


	n += 1 / 64.0;

	if (n > 30){
		n = 0;
		auto newtask = Game1::GetInstance()->taskPool.Get(
			[this](Game1::task_type& t, Game1::task_param a, void* b){ return this->draw_r(t, a, b); }
			, nullptr);
		task.InsertNext(&*newtask);
		task.RemoveImmidiately();
		//Game1::GetInstance()->taskPool.Deactivate(::task);
		this->task = newtask;
	}
	return 0;
}

int Player::draw_r(Game1::task_type& task, Game1::task_param, void*) {
	static float n = 0;

	for (int x = -50; x <= 50; x++) {
		for (int y = -50; y <= 50; y++) {
			InstanceData &obj = model->ReserveDraw();
			obj.World = XMMatrixScaling(0.125f, 0.125f, 0.125f)
				* XMMatrixRotationX(n)
				* XMMatrixRotationY(n / 2)
				* XMMatrixTranslation(x * 0.0625f, 0, 0)
				* XMMatrixTranslation(0, y * 0.0625f, 0);
			obj.MaterialIndex = 0;
		}
	}


	n += 1 / 64.0f;

	if (n > 30) {
		n = 0;
		auto newtask = Game1::GetInstance()->taskPool.Get(
			[this](Game1::task_type& t, Game1::task_param a, void* b){ return this->draw(t, a, b); }
		, nullptr);
		task.InsertNext(&*newtask);
		task.RemoveImmidiately();
//		Game1::GetInstance()->taskPool.Deactivate(::task);
		this->task = newtask;
	}
	return 0;
}

int PrintJoypadInfo(Game1::task_type& task, Game1::task_param, void*){
	XINPUT_STATE state;
	joypad->Update(state);
	const auto& pad = state.Gamepad;

	char infostr[1024];
	sprintf_s(infostr, "BTN: 0x%08X \nAXISES: (%-5d, %-5d), (%-5d, %-5d),\n %3d, %3d\n",
		pad.wButtons, pad.sThumbLX, pad.sThumbLY, pad.sThumbRX, pad.sThumbRY, pad.bLeftTrigger, pad.bRightTrigger);
	stringDrawer->DrawString(0, 0, infostr);

	return 0;
}

int drawModel(Game1::task_type& task, Game1::task_param, void*) {
	model->Flush();
	return 0;
}

void Player::Initialize(D3DCore * core){
	InitializeModel(core);

	task = Game1::GetInstance()->taskPool.Get(
		[this](Game1::task_type& t, Game1::task_param a, void* b){ return this->draw(t, a, b); }
	, nullptr);
	Game1::GetInstance()->drawTasks.InsertHead(&*task);

	modelDrawTask = Game1::GetInstance()->taskPool.Get();
	modelDrawTask->SetAction(drawModel, (void*)nullptr);
	Game1::GetInstance()->drawTasks.InsertTail(&*modelDrawTask);

	joypadTask = Game1::GetInstance()->taskPool.Get();
	joypadTask->SetAction(PrintJoypadInfo, (void*)nullptr);
	Game1::GetInstance()->drawTasks.InsertTail(&*joypadTask);


	joypad.reset(new DIJoypad(0));
	//joypad.reset(new DIJoypad(1, false));

	Sprite::Sprite::InitializeSharedResource(core);
	stringDrawer = new Sprite::SpriteString(core, _T("Content\\Image\\font.png"));
	stringDrawer->SetSize(16, 32);
	stringDrawer->SetPitchRatio(0.8f, 1.0f);
	stringDrawer->SetColor(XMFLOAT4{ 1, 1, 0, 1 }, XMFLOAT4{ 0, 0, 0, 0 });
}

void Player::Reload(bool selectFile){
	ReloadModel(selectFile);
}

Player::~Player()
{
	if (stringDrawer) delete stringDrawer;
	TerminateModel();
}
