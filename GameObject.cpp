#include "GameObject.h"

#include "Framework/Graphics/Model.h"

namespace {
	Models::Model *model;
	Game1* game;

	int n;
}

TaskResult FlushModel();

GameObject::GameObject() {}

GameObject::GameObject(bool) {
	if(!game) game = Game1::GetInstance();
	if (!model) {
		model = Models::Model::Load(_T("Content\\Model\\testmodel.txt"), true, true);
		
		Models::MaterialData mat{ XMFLOAT4() };

		mat.Color = XMFLOAT4{ 1, 1, 1, 0.5 };
		model->SetMaterial(0, mat);

		mat.Color = XMFLOAT4{ 0.5, 1, 1, 0.5 };
		model->SetMaterial(1, mat);

		mat.Color = XMFLOAT4{ 1, 0.5, 1, 0.5 };
		model->SetMaterial(2, mat);

		mat.Color = XMFLOAT4{ 1, 1, 0.5, 0.5 };
		model->SetMaterial(3, mat);

		mat.Color = XMFLOAT4{ 0.5, 0.5, 1, 0.5 };
		model->SetMaterial(4, mat);

		mat.Color = XMFLOAT4{ 1, 0.5, 0.5, 0.5 };
		model->SetMaterial(5, mat);

		mat.Color = XMFLOAT4{ 0.5, 1, 0.5, 0.5 };
		model->SetMaterial(6, mat);

		mat.Color = XMFLOAT4{ 0.5, 0.5, 0.5, 0.5 };
		model->SetMaterial(7, mat);
	}

	vx = game->GetRand(-1, 1) / 32.0f;
	vy = game->GetRand(-1, 1) / 32.0f;
	vz = game->GetRand(-1, 1) / 32.0f;
	vp = game->GetRand(-1, 1) / 128.0f;
	vt = game->GetRand(-1, 1) / 128.0f;
	x = y = z = this->t = this->p = 0.0f;


	reflectCount = 0;
	enabled = true;

	mat = (int)game->GetRand(0, 8);
	blend = /* game->GetRand(); */ 1;
	line = 1 / game->GetRand(2, 8);
	alpha = game->GetRand(0.5, 1);

	n++;
}


GameObject::~GameObject() {
	n--;
	if (n == 0) {
		delete model;
		model = nullptr;
	}
}

bool GameObject::FlushModel() {
	if (n && model) model->Flush();
	return true;
}

bool GameObject::Draw() {
	auto& param = model->ReserveDraw();
	param.World = XMMatrixScaling(5, 5, 5) * XMMatrixRotationX(this->p) * XMMatrixRotationY(this->t) * XMMatrixTranslation(x, y, z);
	param.Params[0].Index = (float)mat;
	param.Params[0].Blend = blend;
	param.Params[0].LineWidth = line;
	param.Params[0].Alpha = alpha;


	return enabled;
}

bool GameObject::Update() {
	float dx = vx;
	float dy = vy;
	float dz = vz;

	bool vanish = false;

	//auto judge = [&]() { objReflect = true; return game->GetRand(0, 200) < reflectCount++; };
	auto judge = [&]() {return false; };

	while (dx != 0) {
		x += dx;
		dx = 0;
		if (x < -10.0) {
			vx = -vx;
			dx = -x - 10.0f;
			x = -10.0f;

			vanish |= judge();
		}
		else if (10.0 < x) {
			vx = -vx;
			dx = 10.0f - x;
			x = 10.0f;

			vanish |= judge();
		}
	}
	while (dy != 0) {
		y += dy;
		dy = 0;
		if (y < -10.0) {
			vy = -vy;
			dy = -y - 10.0f;
			y = -10.0f;

			vanish |= judge();
		}
		else if (10.0 < y) {
			vy = -vy;
			dy = 10.0f - y;
			y = 10.0f;

			vanish |= judge();
		}
	}
	while (dz != 0) {
		z += dz;
		dz = 0;
		if (z < -10.0) {
			vz = -vz;
			dz = -z - 10.0f;
			z = -10.0f;

			vanish |= judge();
		}
		else if (10.0 < z) {
			vz = -vz;
			dz = 10.0f - z;
			z = 10.0f;

			vanish |= judge();
		}
	}

	this->p += vp;
	this->t += vt;

	if (vanish) enabled = false;

	return enabled;
}



