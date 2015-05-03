#include "Camera.h"
#include "Player.h"

#include "Framework/Graphics/D3DHeaders.h"

namespace {
	const float PosMoveThreshold = 0.5f;
	const float DeltaRate = 1.0f / 16.0f;
	const float PosMoveMaxSpeed = 1.0f / 24.0f;
	const float SideViewShift = 1.75f;
	const float SideViewRate = 0.875f;
}

Camera::Camera()
	: currentSurface{ SURF_ZPLUS } 
	, cameraPos{ 0, 0, 0 }
	, cameraUp{ 0, 1, 0 }
	, targetPos{ 0, 0, 3 }
	, targetUp{ 0, 1, 0 } {
	Update(true);
}


Camera::~Camera() {}


void Camera::Update(bool skip) {

	Player* player = GameImpl::GetInstance()->GetPlayer();

	if (player && player->enabled) {
		currentSurface = player->GetSurface();

		targetUp = player->upperDirection;
		{
			auto basePos = XMVectorScale(GetSurfaceVector(currentSurface), 3.0f);
			XMStoreFloat3(&targetPos, basePos);
		}

		//XMVECTOR thresholdCheck = XMLoadFloat3(&(player->pos));
		//thresholdCheck -= XMVectorClamp(thresholdCheck, XMVectorReplicate(-PosMoveThreshold), XMVectorReplicate(PosMoveThreshold));

		float rate = 1.0f;

		auto shiftTarget = [&](float p, float& t) {
			if (p != -1.0f && p != 1.0f) {
				if (p < -PosMoveThreshold) {
					t -= SideViewShift;
					rate *= SideViewRate;
				}
				else if (PosMoveThreshold < p) {
					t += SideViewShift;
					rate *= SideViewRate;
				}
			}
		};

		shiftTarget(player->pos.x, targetPos.x);
		shiftTarget(player->pos.y, targetPos.y);
		shiftTarget(player->pos.z, targetPos.z);

		switch (currentSurface) {
			case SURF_XPLUS:
			case SURF_XMINUS:
				targetPos.x *= rate;
				break;
			case SURF_YPLUS:
			case SURF_YMINUS:
				targetPos.y *= rate;
				break;
			case SURF_ZPLUS:
			case SURF_ZMINUS:
				targetPos.z *= rate;
				break;
		}

	}

	XMVECTOR pos;
	XMVECTOR up;
	if (skip) {
		// スキップ : ターゲット位置へ即座に移動
		pos = XMLoadFloat3(&targetPos);
		up = XMLoadFloat3(&targetUp);
	}
	else {
		// スキップ無し : 移動量(deltaXxx)を計算し加算
		auto currentPosV = XMLoadFloat3(&cameraPos);
		auto currentUpV = XMLoadFloat3(&cameraUp);

		auto targetPosV = XMLoadFloat3(&targetPos);
		auto targetUpV = XMLoadFloat3(&targetUp);

		XMVECTOR deltaPos = (targetPosV - currentPosV) * DeltaRate;

		if (XMVectorGetX(XMVector3LengthSq(deltaPos)) > PosMoveMaxSpeed * PosMoveMaxSpeed) {
			deltaPos = XMVector3Normalize(deltaPos) * PosMoveMaxSpeed;
		}

		XMVECTOR deltaUp = (targetUpV - currentUpV) * DeltaRate;

		if (XMVectorGetX(XMVector3LengthSq(deltaUp)) > PosMoveMaxSpeed * PosMoveMaxSpeed) {
			deltaUp = XMVector3Normalize(deltaUp) * PosMoveMaxSpeed;
		}


		pos = XMLoadFloat3(&cameraPos) + deltaPos;
		up = XMLoadFloat3(&cameraUp) + deltaUp;
	}
	XMStoreFloat3(&cameraPos, pos);
	XMStoreFloat3(&cameraUp, up);


}

void Camera::Set(Models::SceneParameter& scene) {
	scene.View = XMMatrixLookAtLH(XMLoadFloat3(&cameraPos), XMVectorZero(), XMLoadFloat3(&cameraUp));

	scene.PointLights[0].Distance = 5.0f;
	scene.PointLights[0].LightColor = XMFLOAT4{ 1.0f, 1.0f, 1.0f, 0.5f };
	scene.PointLights[0].Position = cameraPos;

}
