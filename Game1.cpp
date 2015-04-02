#include "Game1.h"

#include "Framework/Program.h"
#include "Framework/Utility.h"

#include "Framework/Input/DIDispatcher.h"
#include "Framework/Input/DIJoypad.h"
#include "Framework/Debug.h"

#include "Framework/Graphics/D3DTexture.h"
#include "Framework/Graphics/ImageLoader.h"

#include "Framework/Graphics/Sprite.h"
#include "Framework/Graphics/SpriteString.h"

#include "Framework/Graphics/D3DBloomEffect.h"

#include "Framework/Audio/DACore.h"
#include "Framework/Audio/WavLoader.h"

#include "GameObject.h"

#include <random>
#include <type_traits>

#include <list>
#include <boost/timer.hpp>
//#include "Player.h"

//Player *player;



Game* GenerateGame() {
	return Game1::GetInstance();
}

#define DOWNSCALE_COUNT 2

namespace {
	std::mt19937 rand_engine;

	DIDispatcher* dspch;
	std::weak_ptr<DIMouseListener> mpos;
	std::weak_ptr<DIMouseListener> mvel;
	std::weak_ptr<DIMouseListener> macc;

	DIJoypad* joypad;

	D3DTexture2D* texture;
	Sprite::Sprite* sprite;
	Sprite::SpriteString* spriteString;

	D3DBloomEffect* bloom;

	D3DSampler* samplerForGauss;

	boost::timer timer;
	double updateElapsed;

	int counter = 0;
	XINPUT_STATE state, laststate;

//	DACore* audioCore;
//	DALoopVoice* bgm;

	Game1* gm;
}
std::list<GameObject> objs{};
bool objReflect;

Game1* Game1::GetInstance() {

	if (!gm) {
		SetLogfileName(_T("log.txt"));
		SetOutputLogLevel(g_MainArgs.lpCmdLine, !!(g_MainArgs.lpCmdLine));
		gm = new Game1();
	}
	return gm;
}


Game1::Game1() : windowWidth(800), windowHeight(600), window(800, 600) {
//Game1::Game1() : windowWidth(1366), windowHeight(768), window(1366, 768) {

	std::random_device seed_gen;
	rand_engine.seed(seed_gen());

}

Game1::~Game1() {
	objs.clear();
	//delete player;
	delete texture;
}

int Game1::Initialize() {
	// 同期レンダリングを使用する
	this->asyncDraw = false;

	window.Initialize();

	//window.SetMessageHandler(WM_KEYDOWN, [](UINT m, WPARAM w, LPARAM L){
	//	if (w == VK_F5){
	//		player->Reload();
	//	}
	//	else if (w == VK_F6){
	//		player->Reload(true);
	//	}
	//	return 0;
	//});

	InitializeD3DCore();

	SetCursor(LoadCursor(0, IDC_ARROW));

	//player = new Player();
	//player->Initialize(this->core);

	dspch = new DIDispatcher(&window);
	window.AddResource(PtrToRes(dspch));

	mpos = dspch->AddMouseListener(DIMouseListenType::Position, DIRange{ 0, 0 }, DIRange{ 0, 0 });
	mvel = dspch->AddMouseListener(DIMouseListenType::Velocity, DIRange{ 0, 0 }, DIRange{ 0, 0 });
	macc = dspch->AddMouseListener(DIMouseListenType::Acceleration, DIRange{ 0, 0 }, DIRange{ 0, 0 });

	auto image = ImageData::Load(_T("Content\\Image\\test.png"));
	texture = new D3DTexture2D(core, image);
	delete image;

	Sprite::Sprite::InitializeSharedResource(core);

	sprite = new Sprite::Sprite(texture);
	window.AddResource(PtrToRes(sprite));

	spriteString = new Sprite::SpriteString(core, _T("Content\\Image\\font.png"));
	spriteString->SetColor(XMFLOAT4{ 1, 1, 1, 1 }, XMFLOAT4{ 0, 0, 0, 1 });
	window.AddResource(PtrToRes(spriteString));
	//spriteString->SetSize(32, 64);

	bloom = new D3DBloomEffect(core, window.GetWidth(), window.GetHeight());
	window.AddResource(PtrToRes(bloom));

	D3D11_SAMPLER_DESC smpdesc;
	D3DSampler::GetDefaultSamplerDesc(&smpdesc);
	smpdesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	smpdesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	smpdesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerForGauss = new D3DSampler(core, &smpdesc);
	window.AddResource(PtrToRes(samplerForGauss));
	NameToResource(samplerForGauss->GetRawSampler(), "SamplerForGauss");

	joypad = new DIJoypad(0);
	window.AddResource(PtrToRes(joypad));

	//audioCore = new DACore();
	//{
	//	WavLoader wavloader;
	//	WavLoader::Parameter param = { 0 };

	//	param.filename = _T("Content\\Sound\\ammo_bounce.wav");
	//	param.dividePerSec = 44100;

	//	param.startPos = 0;
	//	param.endPos = 256582;
	//	SoundData::Ptr intro = SoundData::Load(wavloader, param);

	//	param.startPos = 256582;
	//	param.endPos = 513164;
	//	SoundData::Ptr loop = SoundData::Load(wavloader, param);

	//	bgm = new DALoopVoice(audioCore, intro, loop);
	//}

	//window.AddResource(PtrToRes(audioCore));
	//audioCore->AddResource(PtrToRes(bgm));

	return 0;
}

void Game1::Update() {

	auto ret = window.ProcessMessage();
	if (ret != 0) exitLoop = true;

	//if(counter == 0) bgm->Play();


	objReflect = false;
	updateTasks.Invoke();

	dspch->Update();

//	if (counter % (30 * 60) == 0) {
	//if (counter == 0){
	if (objs.size() < 2) {
		auto iter = objs.emplace(objs.end(), false);
		objs.back().thisiter = iter;
		//if (objs.size() >= 2) {
		//	objs.pop_front();
		//}
	}

		
	//}
	counter++;

	double x, y, vx, vy, ax, ay;
	{
		auto mll = mpos.lock();
		x = mll->GetLastValueX();
		y = mll->GetLastValueY();
	}
	{
		auto mll = mvel.lock();
		vx = mll->GetLastValueX();
		vy = mll->GetLastValueY();
	}
	{
		auto mll = macc.lock();
		ax = mll->GetLastValueX();
		ay = mll->GetLastValueY();
	}

	auto& obj = objs.front();

	joypad->Update(state);
	//obj.vp = (float)state.Gamepad.sThumbLX / (32768.0f * 32.0f);
	//obj.vt = (float)state.Gamepad.sThumbLY / (32768.0f * 32.0f);
	//obj.alpha += (float)state.Gamepad.bLeftTrigger  / 16384.0f;
	//obj.alpha -= (float)state.Gamepad.bRightTrigger / 16384.0f;
	//if ((state.Gamepad.wButtons & ~laststate.Gamepad.wButtons & XINPUT_GAMEPAD_A) == XINPUT_GAMEPAD_A) {
	//	obj.mat++;
	//	if (obj.mat >= 8) obj.mat -= 8;
	//}
	//if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) == XINPUT_GAMEPAD_LEFT_SHOULDER) {
	//	obj.line += 0.125f;
	//}
	//if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) == XINPUT_GAMEPAD_RIGHT_SHOULDER) {
	//	obj.line -= 0.125f;
	//}

	//LOG_TRACE("%d, %d, %08x\n", state.Gamepad.sThumbLX, state.Gamepad.sThumbLY, state.Gamepad.wButtons);

	laststate = state;
}
void Game1::Draw() {
	updateElapsed = timer.elapsed();
	timer.restart();

	{
		auto &sp = Models::Model::GetSceneParam();

		XMVECTOR eye = { 15, 12, 9, 1 };
		XMVECTOR lookat = { 0, 0, 0, 1 };
		XMVECTOR up = { 0, 1, 0, 1 };
		sp.Projection = XMMatrixPerspectiveLH(GetWindowWidth() / 1600.0f, GetWindowHeight() / 1600.0f, 0.25, 250);
		sp.View = XMMatrixLookAtLH(eye, lookat, up);
		sp.LightDirection = XMFLOAT3(1, -1, -1);
		sp.LightColor = XMFLOAT4(1, 1, 1, 0);
		sp.AmbientColor = XMFLOAT4{ 1, 1, 1, 0 };

		sp.PointLights[0].Distance = 100;
		sp.PointLights[0].LightColor = XMFLOAT4{ 1, 1, 1, (1 + std::sinf(XM_2PI * counter / 120.0f)) * 2 };
		sp.PointLights[0].Position = XMFLOAT3{ 10, 10, -10 };

		sp.PointLights[1].Distance = 100;
		sp.PointLights[1].LightColor = XMFLOAT4{ 1, 1, 1, (1 + std::sinf(XM_2PI * (counter + 60) / 120.0f)) * 2 };
		sp.PointLights[1].Position = XMFLOAT3{ -10, -10, 10 };
	}

	//static float n = 0.0f;
	//n += 1 / 60.0f;
	//if (n >= 1) n -= 1.0f;
	core->ClearRenderTarget(DirectX::XMFLOAT4(0, 0.0625f, 0.125f, 1));
	core->ClearDepth();

	bloom->Draw([&]() {
		drawTasks.Invoke();
	});


	sprite->SetTexture(texture);
	sprite->SetSampler(nullptr);
	sprite->SetEffector(nullptr);
	float th = (float)texture->GetHeight();
	float tw = (float)texture->GetWidth();
	float sh = (float)core->GetScreenHeight();
	float sw = (float)core->GetScreenWidth();
	sprite->DrawQuad(
		XMFLOAT4{ 0, 0, 0, 1 },
		XMFLOAT4{ 0, th, 0, 1 },
		XMFLOAT4{ tw, 0, 0, 1 },
		XMFLOAT4{ tw, th, 0, 1 }
	);
	sprite->DrawQuad(
		XMFLOAT4{ sw - tw, sh - th, 0, 1 },
		XMFLOAT4{ sw - tw, sh, 0, 1 },
		XMFLOAT4{ sw, sh - th, 0, 1 },
		XMFLOAT4{ sw, sh, 0, 1 }
	);
	sprite->Flush();

	char buff[256];
	sprintf_s(buff, "instance : %d\nelapsed : %.3f", objs.size(), updateElapsed);
	spriteString->DrawString(0, 0, buff);

	core->Update();

}



void Game1::InitializeD3DCore() {
	core = new D3DCore(&window);

	core->Initialize(false, true);
	core->SetVSyncWait(1);

	Models::Model::InitializeSharedResource(core);
}

float Game1::GetRand() {
	unsigned int res = (rand_engine() >> 9) | 0x3f800000;
	return (*(float*)&res) - 1.0f;
}

float Game1::GetRand(float min, float max) {
	return min + GetRand() * (max - min);
}
