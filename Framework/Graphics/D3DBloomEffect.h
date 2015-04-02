#pragma once

#include "D3DCore.h"
#include "D3DTexture.h"
#include "D3DSampler.h"
#include "Sprite.h"
#include "GaussianEffector.h"
#include "BrightPassEffector.h"

class D3DBloomEffect : public Resource {
public:

	enum {
		DOWNSCALE_COUNT = 4,
	};


	D3DBloomEffect(D3DCore* c, unsigned int width, unsigned int height);
	~D3DBloomEffect();

private:

	D3DCore* core;
	
	float threshold;


	std::unique_ptr<D3DTexture2D> offscreenSurface;
	std::unique_ptr<D3DTexture2D> downscaleSurface[DOWNSCALE_COUNT];
	std::unique_ptr<D3DTexture2D> gaussianSurface;

	std::unique_ptr<D3DSampler> samplerForGauss;

	std::unique_ptr<Sprite::Sprite> sprite;

	std::unique_ptr<Sprite::BrightPassEffector> filter;
	std::unique_ptr<Sprite::GaussianEffector> gauss_v;
	std::unique_ptr<Sprite::GaussianEffector> gauss_h;



	void Preprocess();
	void Postprocess();

public:
	template <typename Fn>
	void Draw(Fn f) {
		Preprocess();
		f();
		Postprocess();
	}
};

