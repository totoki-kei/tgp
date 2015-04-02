#pragma once

#include "../Graphics/D3DTexture.h"
#include "../Graphics/ImageLoader.h"
#include "../Graphics/Sprite.h"

#include <tchar.h>
#include <memory>

namespace Sprite {

	class SpriteString {
		std::unique_ptr<Sprite> sprite;
		std::unique_ptr<D3DTexture2D > texture;

		class FontEffector;
		std::unique_ptr<FontEffector> effector;

		float hpitch;
		float vpitch;
		float charWidth;
		float charHeight;

		bool autoFlush;

		void putChar(int c, float x, float y);

	public:
		SpriteString(D3DCore*, const TCHAR* fontFile, Translator* translator = nullptr);
		~SpriteString();

		void DrawString(float x, float y, const char* text);
		void DrawString(float x, float y, const std::string& text);
		
		void SetTranslator(Translator*);
		Translator* GetTranslator();

		void SetColor(const XMFLOAT4& fill, const XMFLOAT4& edge);
		void GetColor(XMFLOAT4* fill, XMFLOAT4* edge);

		void SetPitchRatio(float horizontal, float vertical);
		void GetPitchRatio(float* horizontal, float* vertical);
		void SetSize(float width, float height);
		void GetSize(float* width, float* height);
		void ResetSize();

		void SetAutoFlush(bool flush);
		bool GetAutoFlush();
		void Flush();
	};

}