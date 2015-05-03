#include "ClassicGameScene.h"

#include "GameImpl.h"
#include "Bullet.h"
#include "Item.h"
#include "Particle.h"

#include "Framework/Utility.h"
#include "Framework/Graphics/D3DTexture.h"

namespace {
	// ここにファイルローカルな静的フィールドを置く

	namespace Texture {
		D3DTexture2D *indicator;
	}

	void InitializeStaticResources() {

	}

}

ClassicGameScene::ClassicGameScene(Scene* parent) 
	: Scene{ parent } 
	, tick{ 0 } {

	static Initializer init{ InitializeStaticResources };


	auto game = GameImpl::GetInstance();

	auto session = game->StartSession();
	game->GetPlayer()->Init(session);
}


ClassicGameScene::~ClassicGameScene() {
	Bullet::Terminate();
}


Scene* ClassicGameScene::UpdateScene() {
	auto player = GameImpl::GetInstance()->GetPlayer();

	if (player->enabled) {
		player->Update();


	} // if (player->enabled)
	else {
		auto game = GameImpl::GetInstance();
		auto input = game->GetInput();
		if (input->btnA) {
			Item::Clear();
			Bullet::Clear();
			game->GetPlayer()->Init(game->StartSession());
		}
	} // if (player->enabled)

	//if (player && player->enabled) {
	{
		if (tick % 10 == 0) {
		
			auto s = (Surface)(player->GetSurface() ^ (0x01));

			float x = GameImpl::GetInstance()->GetRand(-1, 1) / 100;
			float y = GameImpl::GetInstance()->GetRand(-1, 1) / 100;
			XMFLOAT3 vel;

			switch (s >> 1) {
				case 0: // X
					vel = XMFLOAT3(0.0f, x, y);
					break;
				case 1: // Y
					vel = XMFLOAT3(y, 0.0f, x);
					break;
				case 2: // Z
					vel = XMFLOAT3(x, y, 0.0f);
					break;
			}

			if ((tick % 50) == 0) {
				Bullet::Shoot(s, 1, GetRandomPointOnSurface(s), vel, Bullet::Pattern{ &Bullet::AssultPattern });
			}
			else {
				Bullet::Shoot(s, 0, GetRandomPointOnSurface(s), vel);
			}
		}

		Bullet::UpdateAll();

		for (int i = Item::GetCount(); i < 12; i++) {
			Item::Create();
		}
		Item::UpdateAll();
	}

	Particle::UpdateAll();

	GameImpl::GetInstance()->GetCamera()->Update();

	tick++;

	return this;
}

void ClassicGameScene::DrawScene() {
	auto player = GameImpl::GetInstance()->GetPlayer();
	if(player->enabled) player->Draw();
	Particle::DrawAll();
	Bullet::DrawAll();
	Item::DrawAll();
	GameImpl::GetInstance()->GetField()->Draw();
}

