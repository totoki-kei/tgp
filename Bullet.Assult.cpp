#include "Bullet.h"

void Bullet::AssultPattern(Bullet::PatternCoroutine::Yielder& yield) {
	while (auto bullet = yield()) {
		if (bullet->lastRotated) {
			auto currentVel = bullet->vel;
			bullet->vel = XMFLOAT3(0, 0, 0);

			for (int i = 0; i < 30 * bullet->scoreRatio; i++) {
				bullet = yield();
			}

			bullet->vel = currentVel;

			auto vel2
				= bullet->vel.x * bullet->vel.x
				+ bullet->vel.y * bullet->vel.y
				+ bullet->vel.z * bullet->vel.z;

			if (vel2 < 0.05 * 0.05) {
				bullet->vel.x *= (1 + 1 / 8.0f);
				bullet->vel.y *= (1 + 1 / 8.0f);
				bullet->vel.z *= (1 + 1 / 8.0f);
				bullet->scoreRatio *= (1 + 1 / 8.0f);
			}
		}
	}
}