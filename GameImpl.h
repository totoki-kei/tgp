#pragma once

#include "Framework/Game.h"
#include "Framework/GameWindow.h"
#include "Framework/Graphics/D3DCore.h"

#include "Framework/Graphics/D3DShader.h"

#include "Framework/Task.h"
#include "Framework/Pool.h"

#include "Framework/Graphics/Model.h"
#include "Framework/Scene.h"

class GameImpl;

#include "ModelBank.h"
#include "Player.h"
#include "Camera.h"
#include "CubeField.h"
#include "Session.h"

class GameImpl : public Game
{
public:

	typedef TaskList<float> task_list;
	struct InputData {
		enum Direction {
			DIR_NONE = -1,
			DIR_N = 0,
			DIR_NEn,
			DIR_NE,
			DIR_NEe,
			DIR_E,
			DIR_SEe,
			DIR_SE,
			DIR_SEs,
			DIR_S,
			DIR_SWs,
			DIR_SW,
			DIR_SWw,
			DIR_W,
			DIR_NWw,
			DIR_NW,
			DIR_NWn,
			DIR_COUNT,
		} dir;
		bool btnA, btnB,
			btnL, btnR;

		InputData() 
			: dir{ DIR_NONE }
			, btnA{ false }
			, btnB{ false }
			, btnL{ false }
			, btnR{ false } {};
	};

private:
	GameWindow window;
	D3DCore* core;

	int windowWidth;
	int windowHeight;

	std::unique_ptr<Scene> scene;

	std::unique_ptr<Player> player;
	std::unique_ptr<CubeField> field;
	std::unique_ptr<Camera> camera;

	Session session;
	std::vector<InputData> inputData;
	InputData lastInputData;

	void InitializeD3DCore();
public:

	static GameImpl* GetInstance();

public:
	GameImpl();
	~GameImpl();


	int Initialize();
	void Update();
	void Draw();

	inline int GetWindowWidth(){ return windowWidth; }
	inline int GetWindowHeight(){ return windowHeight; }
	inline HWND GetWindowHandle(){ return window.GetWindowHandle(); }

	float GetRand();
	float GetRand(float, float);

	const InputData* GetInput();
	void UpdateInput();

	Session* StartSession();
	void EndSession();

	Player* GetPlayer();
	Camera* GetCamera();
	CubeField* GetField();

	//void GetPadState(XINPUT_STATE **state, XINPUT_STATE **lastState);
	//void GetKeyboardState(bool *up, bool* dn, bool* l, bool* r, bool* a);

	//struct $ {
	//	static Script::ReturnState Random(Script::Thread&, const Script::Code&);
	//};
};

