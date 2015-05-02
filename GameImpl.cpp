#include "GameImpl.h"

#include "Framework/Program.h"
#include "Framework/Utility.h"
#include "Framework/Debug.h"

#include "Framework/Input/DIDispatcher.h"
#include "Framework/Input/DIJoypad.h"

#include "Framework/Audio/DACore.h"
#include "Framework/Audio/DAVoice.h"
#include "Framework/Audio/WavLoader.h"

#include "Framework/Graphics/D3DTexture.h"
#include "Framework/Graphics/ImageLoader.h"

#include "Framework/Graphics/Sprite.h"
#include "Framework/Graphics/SpriteString.h"

#include "Framework/Graphics/D3DBloomEffect.h"

#include "Framework/Graphics/MatrixTranslator.h"

#include <random>
#include <type_traits>

#include <list>
#include <boost/timer.hpp>
#include <mutex>

#include "Bullet.h"
#include "Item.h"
#include "CubeField.h"
#include "Camera.h"

#include "ClassicGameScene.h"

Game* GenerateGame() {
	return GameImpl::GetInstance();
}

namespace {
	std::mt19937 rand_engine;

	DIDispatcher* dspch;
	std::weak_ptr<DIMouseListener> mpos;
	std::weak_ptr<DIMouseListener> mvel;
	std::weak_ptr<DIMouseListener> macc;
	std::weak_ptr<DIKeyboardListener> kbdUp;
	std::weak_ptr<DIKeyboardListener> kbdDown;
	std::weak_ptr<DIKeyboardListener> kbdLeft;
	std::weak_ptr<DIKeyboardListener> kbdRight;
	std::weak_ptr<DIKeyboardListener> kbdZ;
	std::weak_ptr<DIKeyboardListener> kbdX;

	std::weak_ptr<DIKeyboardListener> kbdW;
	std::weak_ptr<DIKeyboardListener> kbdA;
	std::weak_ptr<DIKeyboardListener> kbdS;
	std::weak_ptr<DIKeyboardListener> kbdD;
	std::weak_ptr<DIKeyboardListener> kbdRCtrl;
	std::weak_ptr<DIKeyboardListener> kbdRShift;

	DIJoypad* joypad;

	Sprite::SpriteString* systemText;

	XINPUT_STATE state;

	GameImpl* gm;

	D3DBloomEffect* bloom;

	volatile double updateTime;
	volatile double drawTime;

}

GameImpl* GameImpl::GetInstance() {

	if (!gm) {
		SetLogfileName(_T("log.txt"));
		SetOutputLogLevel(g_MainArgs.lpCmdLine, !!(g_MainArgs.lpCmdLine));
		gm = new GameImpl();
	}
	return gm;
}


GameImpl::GameImpl() : windowWidth(800), windowHeight(600), window(800, 600) {
	//GameImpl::GameImpl() : windowWidth(1366), windowHeight(768), window(1366, 768) {

	std::random_device seed_gen;
	rand_engine.seed(seed_gen());

}

GameImpl::~GameImpl() {
	// TODO: ここにオブジェクトの解放処理を入れる
	delete bloom;
}

int GameImpl::Initialize() {
	this->asyncDraw = false;
	window.Initialize();

	window.SetMessageHandler(WM_KEYDOWN, [this](UINT m, WPARAM w, LPARAM L){
		if (w == VK_F5) {
			// ...
		}
		if (w == VK_F6) {
			// ...
		}
		return 0;
	});

	InitializeD3DCore();

	SetCursor(LoadCursor(0, IDC_ARROW));

	dspch = new DIDispatcher(&window);
	window.AddResource(PtrToRes(dspch));

	mpos = dspch->AddMouseListener(DIMouseListenType::Position, DIRange{ 0, 0 }, DIRange{ 0, 0 });
	mvel = dspch->AddMouseListener(DIMouseListenType::Velocity, DIRange{ 0, 0 }, DIRange{ 0, 0 });
	macc = dspch->AddMouseListener(DIMouseListenType::Acceleration, DIRange{ 0, 0 }, DIRange{ 0, 0 });
	kbdUp = dspch->AddKeyboardListener(VK_UP);
	kbdDown = dspch->AddKeyboardListener(VK_DOWN);
	kbdLeft = dspch->AddKeyboardListener(VK_LEFT);
	kbdRight = dspch->AddKeyboardListener(VK_RIGHT);
	kbdZ = dspch->AddKeyboardListener('Z');
	kbdX = dspch->AddKeyboardListener('X');

	kbdW = dspch->AddKeyboardListener('W');
	kbdA = dspch->AddKeyboardListener('A');
	kbdS = dspch->AddKeyboardListener('S');
	kbdD = dspch->AddKeyboardListener('D');
	kbdRCtrl = dspch->AddKeyboardListener(VK_RCONTROL);
	kbdRShift = dspch->AddKeyboardListener(VK_RSHIFT);

	Sprite::Sprite::InitializeSharedResource(core);

	systemText = new Sprite::SpriteString(core, _T("Content\\Image\\font_system.png"));
	window.AddResource(PtrToRes(systemText));
	systemText->SetColor(XMFLOAT4{ 1, 1, 1, 1 }, XMFLOAT4{ 0, 0, 0, 1 });
	systemText->SetAutoFlush(true);

	bloom = new D3DBloomEffect(core, window.GetWidth(), window.GetHeight());

	joypad = new DIJoypad(1, false);
	window.AddResource(PtrToRes(joypad));

	Models::Model::InitializeSharedResource(core);

	g_ModelBank = new ModelBank();
	window.AddResource(PtrToRes(g_ModelBank));

	// ********
	player = std::make_unique<Player>();
	field = std::make_unique<CubeField>();
	camera = std::make_unique<Camera>();
	// TODO : 初期シーンを設定する
	scene = std::make_unique<ClassicGameScene>(nullptr);
	return 0;
}

void GameImpl::Update() {
	boost::timer t;

	auto ret = window.ProcessMessage();
	if (ret != 0) exitLoop = true;

	UpdateInput();

	// TODO: ここにゲームロジックのアップデートを記述する
	if (!scene) {
		exitLoop = true;
		return;
	}
	else {
		auto s = scene->Update();
		if (s != scene.get()) scene.reset(s);
	}

	updateTime = t.elapsed();
	if (asyncDraw) {
		while (t.elapsed() < (ticks % 60) / 60.0 - (ticks % 60 - 1) / 60.0) {
			Sleep(0);
		}
	}

}


void GameImpl::Draw() {
	boost::timer t;

	{
		auto &sp = Models::Model::GetSceneParam();

		sp.Projection = XMMatrixPerspectiveFovLH(1.0f, (float)GetWindowWidth() / (float)GetWindowHeight(), 0.25f, 100.0f);
		sp.LightDirection = XMFLOAT3(-1, -1, -1);
		sp.LightColor = XMFLOAT4(1, 1, 1, 0);
		sp.AmbientColor = XMFLOAT4{ 1, 1, 1, 0.75 };

		camera->Set(sp);

	}

	core->ClearRenderTarget(DirectX::XMFLOAT4(0, 0, 0, 1));
	core->ClearDepth();

	bloom->Draw([&]() {
		// TODO: ここに描画コードを入れる
		if (scene) scene->Draw();
	});

	drawTime = t.elapsed();

	char txt[256] = { 0 };
	sprintf_s(txt, "ticks = %d\t[Update / Draw] = [%4.3f / %4.3f]", ticks, updateTime, drawTime);
	systemText->DrawString(0, 0, txt);
	if (auto input = GetInput()){
		sprintf_s(txt, "input: %d - A:%s B:%s L:%s R:%s",
				  input->dir,
				  input->btnA ? "O" : "X",
				  input->btnB ? "O" : "X",
				  input->btnL ? "O" : "X",
				  input->btnR ? "O" : "X");
		systemText->DrawString(0, 16, txt);
	}
	sprintf_s(txt, "player: (%3.3f, %3.3f, %3.3f) (%3.3f, %3.3f, %3.3f) (%3.3f, %3.3f, %3.3f) %d",
			  player->pos.x, player->pos.y, player->pos.z,
			  player->vel.x, player->vel.y, player->vel.z,
			  player->upperDirection.x, player->upperDirection.y, player->upperDirection.z, player->suf);
	systemText->DrawString(0, 32, txt);
	sprintf_s(txt, "camera: (%3.3f, %3.3f, %3.3f) -> (%3.3f, %3.3f, %3.3f)",
			  camera->cameraPos.x, camera->cameraPos.y, camera->cameraPos.z,
			  camera->targetPos.x, camera->targetPos.y, camera->targetPos.z);
	systemText->DrawString(0, 48, txt);
	sprintf_s(txt, "bullets: %d", Bullet::GetCount());
	systemText->DrawString(0, 64, txt);


	//LOG_DBG(txt, );

	core->Update();

}



void GameImpl::InitializeD3DCore() {
	core = new D3DCore(&window);

	core->Initialize(false, false);
	core->SetVSyncWait(1);

	Models::Model::InitializeSharedResource(core);
}

float GameImpl::GetRand() {
	unsigned int res = (rand_engine() >> 9) | 0x3f800000;
	return (*(float*)&res) - 1.0f;
}

float GameImpl::GetRand(float min, float max) {
	return min + GetRand() * (max - min);
}

const GameImpl::InputData* GameImpl::GetInput() {
	return &lastInputData;
}

void GameImpl::UpdateInput() {
	dspch->Update();

	static InputData::Direction directionMap[] = {
		InputData::DIR_NW, InputData::DIR_NWn, InputData::DIR_N, InputData::DIR_NEn, InputData::DIR_NE,
		InputData::DIR_NWw, InputData::DIR_NW, InputData::DIR_N, InputData::DIR_NE, InputData::DIR_NEe,
		InputData::DIR_W, InputData::DIR_W, InputData::DIR_NONE, InputData::DIR_E, InputData::DIR_E,
		InputData::DIR_SWw, InputData::DIR_SW, InputData::DIR_S, InputData::DIR_SE, InputData::DIR_SEe,
		InputData::DIR_SW, InputData::DIR_SWs, InputData::DIR_S, InputData::DIR_SEs, InputData::DIR_SE,
	};
	auto getDirectionFromXY = [](SHORT x, SHORT y) -> InputData::Direction {
		static const SHORT Threshold1 = 32768 / 5;
		static const SHORT Threshold2 = 32768 / 2;

		int xlevel, ylevel;

		if (x < -Threshold2)
			xlevel = 0;
		else if (x < -Threshold1)
			xlevel = 1;
		else if (x > Threshold2)
			xlevel = 4;
		else if (x > Threshold1)
			xlevel = 3;
		else
			xlevel = 2;

		if (y < -Threshold2)
			ylevel = 4;
		else if (y < -Threshold1)
			ylevel = 3;
		else if (y > Threshold2)
			ylevel = 0;
		else if (y > Threshold1)
			ylevel = 1;
		else
			ylevel = 2;

		return directionMap[ylevel * 5 + xlevel];
	};
	auto getDirectionFromDigital = [](bool up, bool down, bool left, bool right) -> InputData::Direction {
		int xlevel = 2 - (left ? 1 : 0) + (right ? 1 : 0);
		int ylevel = 2 - (up ? 1 : 0) + (down ? 1 : 0);

		return directionMap[ylevel * 5 + xlevel];
	};

	InputData data;

	// 優先順位
	// 1. パッドデジタル入力
	// 2. パッドアナログ入力
	// 3. キーボード入力
	if (joypad && joypad->Update(state)) {
		// 1. パッドデジタル入力
		bool dpad_up = !!(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
		bool dpad_down = !!(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
		bool dpad_left = !!(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
		bool dpad_right = !!(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
		data.dir = getDirectionFromDigital(dpad_up, dpad_down, dpad_left, dpad_right);

		if (data.dir == InputData::DIR_NONE) {
			// 2. パッドアナログ入力
			data.dir = getDirectionFromXY(state.Gamepad.sThumbLX, state.Gamepad.sThumbLY);
		}

		data.btnA = !!(state.Gamepad.wButtons & XINPUT_GAMEPAD_A);
		data.btnB = !!(state.Gamepad.wButtons & XINPUT_GAMEPAD_B);
		data.btnL = !!(state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
		data.btnR = !!(state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
	}

	if (data.dir == InputData::DIR_NONE) {
		// 3. キーボード入力
		bool kbd_up = kbdUp.lock()->GetLastStatus() > 0 || kbdW.lock()->GetLastStatus() > 0;
		bool kbd_down = kbdDown.lock()->GetLastStatus() > 0 || kbdS.lock()->GetLastStatus() > 0;
		bool kbd_left = kbdLeft.lock()->GetLastStatus() > 0 || kbdA.lock()->GetLastStatus() > 0;
		bool kbd_right = kbdRight.lock()->GetLastStatus() > 0 || kbdD.lock()->GetLastStatus() > 0;

		data.dir = getDirectionFromDigital(kbd_up, kbd_down, kbd_left, kbd_right);
	}

	data.btnA = data.btnA || kbdZ.lock()->GetLastStatus() > 0 || kbdRCtrl.lock()->GetLastStatus() > 0;
	data.btnB = data.btnB || kbdX.lock()->GetLastStatus() > 0 || kbdRShift.lock()->GetLastStatus() > 0;

	lastInputData = data;
	inputData.push_back(data);
}

Session* GameImpl::StartSession() {
	session = Session();
	inputData.clear();

	rand_engine.seed(session.random_seed);

	return &session;
}

void GameImpl::EndSession() {
	/*
	TODO: ここに以下の処理を追加する
	・リプレイデータの保存
	・スコアボードの更新
	*/
}

Player* GameImpl::GetPlayer() {
	return player.get();
}

Camera* GameImpl::GetCamera() {
	return camera.get();
}

CubeField* GameImpl::GetField() {
	return field.get();
}

//void GameImpl::GetPadState(XINPUT_STATE **s, XINPUT_STATE **ss) {
//	if (s)*s = &state;
//	if (ss) *ss = &laststate;
//}
//
//void GameImpl::GetKeyboardState(bool *up, bool* dn, bool* l, bool* r, bool* a) {
//	if (up) *up = kbdUp.lock()->GetLastStatus() > 0 || kbdW.lock()->GetLastStatus() > 0;
//	if (dn) *dn = kbdDown.lock()->GetLastStatus() > 0 || kbdS.lock()->GetLastStatus() > 0;
//	if (l) *l   = kbdLeft.lock()->GetLastStatus() > 0 || kbdA.lock()->GetLastStatus() > 0;
//	if (r) *r   = kbdRight.lock()->GetLastStatus() > 0 || kbdD.lock()->GetLastStatus() > 0;
//
//	if (a) *a = kbdZ.lock()->GetLastStatus() > 0 || kbdRCtrl.lock()->GetLastStatus() > 0;
//}
