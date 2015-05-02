#include "Player.h"
#include "Misc.h"
#include "Item.h"
#include "Bullet.h"
#include "Particle.h"

using namespace PlayerConsts;

Player::Player() : GameObject{ XMFLOAT3{ 0, 0, 1 }, XMFLOAT3{ 0, 0, 0 } } {
	model = g_ModelBank->Get(_T("Content\\Model\\Player.txt"));
	session = nullptr;
	enabled = false;

	upperDirection = XMFLOAT3{ 0, 1, 0 };
}

void Player::Init(Session* s) {
	session = s;
	enabled = true;

//	pos = GetRandomPointOnSurface(this->GetSurface());
	
}

void Player::Update() {
	if (!session) {
		// Init()が呼ばれていない
		return;
	}

	auto input_ptr = GameImpl::GetInstance()->GetInput();
	GameImpl::InputData dummyInput{};

	auto& input = input_ptr ? *input_ptr : dummyInput;

	int chkcount = input.btnB ? 3 : 1;
	while (chkcount-- > 0) {

		float vx, vy, vz;
		vx = vy = vz = 0.0f;
		float dir = (int)input.dir / (float)GameImpl::InputData::DIR_COUNT;

		if (dir >= 0 || input.btnL || input.btnR) {
			XMVECTOR axis = GetSurfaceVector(GetSurface());

			if (input.btnR && input.btnL) {
				// reset rotation
				XMFLOAT3 test[] = {
					XMFLOAT3(1.0f, 0.0f, 0.0f),
					XMFLOAT3(-1.0f, 0.0f, 0.0f),
					XMFLOAT3(0.0f, 1.0f, 0.0f),
					XMFLOAT3(0.0f, -1.0f, 0.0f),
					XMFLOAT3(0.0f, 0.0f, 1.0f),
					XMFLOAT3(0.0f, 0.0f, -1.0f)
				};

				int maxIndex = -1;

				XMFLOAT3* resetDir = std::max_element(
					std::begin(test), std::end(test),
					[&](const XMFLOAT3& left, const XMFLOAT3& right) {
					auto l = upperDirection.x * left.x
						+ upperDirection.y * left.y
						+ upperDirection.z * left.z;
					auto r = upperDirection.x * right.x
						+ upperDirection.y * right.y
						+ upperDirection.z * right.z;
					return std::less < decltype(l) > {}(l, r);
				}
				);
				if (resetDir != std::end(test)) {
					upperDirection = *resetDir;
				}

				//float max = -1.0f;
				//for (int i = 0; i < 6; i++) {
				//	auto a = upperDirection.x * test[i].x
				//		+ upperDirection.y * test[i].y
				//		+ upperDirection.z * test[i].z;
				//	if (max < a) {
				//		max = a;
				//		maxIndex = i;
				//	}
				//}
				//upperDirection = test[maxIndex];

			}
			else {

				float rot = input.btnL ? 1.0f : input.btnR ? -1.0 : 0.0f;

				if (rot != 0.0f) {
					XMVECTOR tmp = XMLoadFloat3(&upperDirection);
					tmp = XMVector3TransformCoord(tmp, XMMatrixRotationAxis(axis, rot / 64.0f));
					/*upperDirection = XMFLOAT3{
						XMVectorGetX(tmp),
						XMVectorGetY(tmp),
						XMVectorGetZ(tmp)
					};*/
					XMStoreFloat3(&upperDirection, tmp);
				}
			}

			if (dir >= 0) {
				// 入力方向から速度ベクトルを算出

				XMVECTOR up = XMLoadFloat3(&upperDirection);
				XMVECTOR direction =
					XMVector3TransformCoord(up, XMMatrixRotationAxis(axis, (float)(dir * XM_2PI)));

				XMVECTOR spd = XMVectorReplicate(Speed);
				if (input.btnA) spd *= 0.5f; // ボタン1で低速移動

				vx = XMVectorGetX(direction * spd);
				vy = XMVectorGetY(direction * spd);
				vz = XMVectorGetZ(direction * spd);

				this->vel = XMFLOAT3{ vx, vy, vz };
			}
		}
		else {
			this->vel = XMFLOAT3{0, 0, 0};
		}


		XMVECTOR q = XMQuaternionIdentity();
		if (MoveOnSurface(&q)) {
			// 面の移動が発生したら、UpperDirectionを変更する
			auto upper = XMLoadFloat3(&upperDirection);
			upper = XMVector3Rotate(upper, q);
			XMStoreFloat3(&upperDirection, upper);
		}

		// あたり判定
#if 1
		for(auto& b : Bullet::List) {
			if (!b.enabled) continue;
			auto dist = XMFLOAT3
				( b.pos.x - this->pos.x
				, b.pos.y - this->pos.y
				, b.pos.z - this->pos.z);
			if (dist.x * dist.x + dist.y * dist.y + dist.z * dist.z < PlayerSize * PlayerSize) {
				this->enabled = false;
				Particle::Generate(96, this->pos, XMFLOAT3{0, 0, 0}, XMFLOAT3{ 1.0f / 32, 1.0f / 32, 1.0f / 32 }, 4.0f / 64, 0);
				Particle::Generate(32, this->pos, XMFLOAT3{0, 0, 0}, XMFLOAT3 { 1.0f / 32, 1.0f / 32, 1.0f / 32 }, 2.0f / 64, 1);
				//Debug.WriteLine("Hit!!");

				//Program.GameMain.Ranking.Entry(
				//	Program.GameMain.Score,
				//	Program.GameMain.Ticks,
				//	Program.GameMain.GetItemCount);

				GameImpl::GetInstance()->EndSession();
			}
		}
#endif
		for(auto& i : Item::List) {
			if (!i.enabled) continue;
			auto dist = XMFLOAT3(i.pos.x - this->pos.x, i.pos.y - this->pos.y, i.pos.z - this->pos.z);
			if (dist.x * dist.x + dist.y * dist.y + dist.z * dist.z < 16 * PlayerSize * PlayerSize) {
				// アイテム取得
				// -> 取得アイテムカウント増加
				// -> 弾消し発生
				int scoreAdd = ++(this->session->items);
				//Debug.WriteLine("Item-get");
				for(auto& b : Bullet::List) {
					if (!b.enabled) continue;
					auto bdist = XMFLOAT3
						(b.pos.x - this->pos.x
						, b.pos.y - this->pos.y
						, b.pos.z - this->pos.z);
					if (bdist.x * bdist.x + bdist.y * bdist.y + bdist.z * bdist.z < ItemCrushArea * ItemCrushArea) {
						scoreAdd = static_cast<int>(scoreAdd * b.GetScoreRatio());
						b.Vanish();
						this->session->score += scoreAdd;
						scoreAdd += this->session->items;
					}
				}
				i.Vanish();
			}
		}
	}
}

void Player::Draw() {
	auto& inst = model->ReserveDraw();
	inst.World = XMMatrixScaling(1.0f / 1024, 1.0f / 1024, 1.0f / 1024)
		* XMMatrixTranslation(pos.x, pos.y, pos.z);
	inst.Params[0].LineWidth /= 1024.0f;
	inst.Params[1].LineWidth /= 1024.0f;
	inst.Params[2].LineWidth /= 1024.0f;
	inst.Params[3].LineWidth /= 1024.0f;

	model->Flush();
}

Player::~Player() {}
