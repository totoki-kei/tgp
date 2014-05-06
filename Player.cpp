#include "Player.h"

#include "Framework/Model.h"
#include "Game1.h"

using namespace Models;
using std::shared_ptr;

namespace {
	D3DCore* core;

	Model* model;

	TCHAR filename_full[256] = { 0 };
	TCHAR filename[256] = { 0 };


	void InitializeModel(D3DCore* core_){
		core = core_;

		model = Model::Load(_T("testmodel.txt"));
		_tcscpy_s(filename_full, _T("testmodel.txt"));
		_tcscpy_s(filename, _T("testmodel.txt"));

		SceneParameter scp;

		auto g = Game1::GetInstance();
		XMVECTOR eye = { 0, 0, 10, 1 };
		XMVECTOR lookat = { 0, 0, 0, 1 };
		XMVECTOR up = { 0, 1, 0, 1 };
		scp.Projection = XMMatrixPerspectiveLH(g->GetWindowWidth() / 3200.0 ,g->GetWindowHeight() / 3200.0, 0.25, 100);
		scp.View = XMMatrixLookAtLH(eye, lookat, up);
		scp.LightDirection = XMFLOAT4(0, 0, -1, 1);
		scp.LightColor = XMFLOAT4(1, 1, 1, 1);
		model->UpdateSceneParams(&scp);

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

	}

	void TerminateModel(){
		delete model;
	}
}

Player::Player()
{
}


Game1::pool_type::Item task;
int draw(Game1::task_type& task, void*, void*);
int draw_r(Game1::task_type& task, void*, void*);

int draw(Game1::task_type& task, void*, void*){
	static float n = 0;

	ObjectParameter obj;
	obj.World = XMMatrixRotationX(n) * XMMatrixRotationY(n / 2);
	model->UpdateObjectParams(&obj);

	model->Draw();

	n += 1 / 64.0;

	if (n > 20){
		n = 0;
		auto newtask = Game1::GetInstance()->taskPool.Activate();
		newtask->SetAction(draw_r, nullptr);
		task.InsertNext(&*newtask);
		task.MarkToRemove();
		Game1::GetInstance()->taskPool.Deactivate(::task);
		::task = newtask;
	}
	return 0;
}

int draw_r(Game1::task_type& task, void*, void*){
	static float n = 0;

	ObjectParameter obj;
	obj.World = XMMatrixRotationX(-n) * XMMatrixRotationY(-n / 2);
	model->UpdateObjectParams(&obj);

	model->Draw();

	n += 1 / 64.0f;

	if (n > 20) {
		n = 0;
		auto newtask = Game1::GetInstance()->taskPool.Activate();
		newtask->SetAction(draw, nullptr);
		task.InsertNext(&*newtask);
		task.MarkToRemove();
		Game1::GetInstance()->taskPool.Deactivate(::task);
		::task = newtask;
	}
	return 0;
}


void Player::Initialize(D3DCore * core){
	InitializeModel(core);

	task = Game1::GetInstance()->taskPool.Activate();
	task->SetAction(draw, nullptr);
	Game1::GetInstance()->drawTasks.InsertHead(&*task);
}

void Player::Reload(bool selectFile){
	ReloadModel(selectFile);
}

Player::~Player()
{
	TerminateModel();
}
