#include "SpriteString.h"

namespace Sprite {

	Shaders::PixelShader* psFont;

	class SpriteString::FontEffector : public Effector{
	public:
		struct Color {
			XMFLOAT4 fillColor;
			XMFLOAT4 edgeColor;
		} color;

		std::unique_ptr<D3DConstantBuffer<Color> > cbColor;

		FontEffector(D3DCore* core, const TCHAR* filename)
			: Effector{} // ここでは読み込まない
			, cbColor{ nullptr } {
			// 共通のリソースとしてピクセルシェーダをロードする
			if (!psFont) {
				psFont = Shaders::Load<Shaders::PixelShader>(core, filename);
				if (!psFont) {
					// 読み込めなかった
					LOG_ERR("ファイル %s を読み込めませんでした\n", filename);
					return;
				}
				core->AddResource(PtrToRes(psFont));
			}
			// 親クラスのデストラクタで消されないように、shaderには設定しない
			//this->shader = psFont;

			// こいつは自前で削除する
			cbColor.reset(new D3DConstantBuffer<Color>(core));
		}
		~FontEffector() { /* nop : unique_ptrによる破棄 */ }

		void Apply(D3DTexture2D* tex, D3DSampler* smp) override {
			psFont->Apply();
			tex->Apply(Shaders::ShaderFlag::Pixel, 0);
			smp->Apply(Shaders::ShaderFlag::Pixel, 0);
			cbColor->Update(&color);
			cbColor->Apply(Shaders::ShaderFlag::Pixel, 0);
		}
	};


	SpriteString::SpriteString(D3DCore* core, const TCHAR* fontFile, Translator* translator)
		: vpitch{ 1 }
		, hpitch{ 1 }
		, charHeight{ -1 }
		, charWidth{ -1 }
		, autoFlush{ false } {
		std::unique_ptr<ImageData> imgdata{ ImageData::Load(fontFile) };
		if (!imgdata) {
			LOG_ERR("Failed to load font texture %s", fontFile);
		}
		else {
			LOG_INFO("font texture %s loaded. size = %dx%d\n", fontFile, imgdata->GetWidth(), imgdata->GetHeight());
		}
		texture.reset(new D3DTexture2D(core, imgdata.get()));

		effector.reset(new FontEffector(core, _T("Content\\System\\PS_Font.cso")));

		sprite.reset(new Sprite(texture.get(), nullptr, translator, effector.get()));

		SetColor(XMFLOAT4{ 0, 0, 0, 1 }, XMFLOAT4{ 1, 1, 1, 1 });

		ResetSize();
	}


	SpriteString::~SpriteString() { /* nop : unique_ptrによる破棄 */ }

	void SpriteString::putChar(int c, float x, float y) {
		float cw = texture->GetWidth() / 16.0f;
		float ch = texture->GetHeight() / 8.0f;

		float w = charWidth < 0 ? cw  : charWidth;
		float h = charHeight < 0 ? ch : charHeight;

		int xaddr = (c - ' ') % 16;
		int yaddr = (c - ' ') / 16;

		Vertex lt = { XMFLOAT4{ x, y, 0, 1 }, XMFLOAT2{ xaddr * cw, yaddr * ch } };
		Vertex lb = { XMFLOAT4{ x, y + h, 0, 1 }, XMFLOAT2{ xaddr * cw, (yaddr + 1) * ch } };
		Vertex rt = { XMFLOAT4{ x + w, y, 0, 1 }, XMFLOAT2{ (xaddr + 1) * cw, yaddr * ch } };
		Vertex rb = { XMFLOAT4{ x + w, y + h, 0, 1 }, XMFLOAT2{ (xaddr + 1) * cw, (yaddr + 1) * ch } };

		sprite->DrawQuad(lt, lb, rt, rb);
	}

	void SpriteString::DrawString(float x, float y, const char* text) {

		float cw = texture->GetWidth() / 16.0f;
		float ch = texture->GetHeight() / 8.0f;

		const char* p = text;

		int line = 0;
		int col = 0;

		float w = charWidth < 0 ? cw : charWidth;
		float h = charHeight < 0 ? ch : charHeight;

		while (*p) {
			switch (*p) {
				case '\n':
					line++;
					col = 0;
					break;
				case '\t':
					col += 4 - (col % 4);
					break;
				default:
					putChar(
						isascii(*p) ? *p : ' ',
						x + col  * w * hpitch,
						y + line * h * vpitch);
					col++;
					break;
			}

			p++;
		}

		if(autoFlush) Flush();
	}


	void SpriteString::DrawString(float x, float y, const std::string& text) {

		float cw = texture->GetWidth() / 16.0f;
		float ch = texture->GetHeight() / 8.0f;

		auto p = text.begin();

		int line = 0;
		int col = 0;

		float w = charWidth < 0 ? cw : charWidth;
		float h = charHeight < 0 ? ch : charHeight;

		while (p != text.end()) {
			switch (*p) {
				case '\n':
					line++;
					col = 0;
					break;
				case '\t':
					col += 4 - (col % 4);
					break;
				default:
					putChar(
						isascii(*p) ? *p : ' ',
						x + col  * w * hpitch,
						y + line * h * vpitch);
					col++;
					break;
			}

			p++;
		}

		if (autoFlush) Flush();
	}

	void SpriteString::SetTranslator(Translator* tr) {
		sprite->SetTranslator(tr);
	}

	Translator* SpriteString::GetTranslator() {
		return sprite->GetTranslator();
	}

	void SpriteString::SetColor(const XMFLOAT4& fill, const XMFLOAT4& edge) {
		effector->color.fillColor = fill;
		effector->color.edgeColor = edge;
	}

	void SpriteString::GetColor(XMFLOAT4* fill, XMFLOAT4* edge) {
		if (fill) *fill = effector->color.fillColor;
		if (edge) *edge = effector->color.edgeColor;
	}

	void SpriteString::SetPitchRatio(float hor, float ver) {
		this->hpitch = hor;
		this->vpitch = ver;
	}

	void SpriteString::GetPitchRatio(float* hor, float* ver) {
		if (hor) *hor = hpitch;
		if (ver) *ver = vpitch;
	}


	void SpriteString::SetSize(float width, float height) {
		this->charWidth = width;
		this->charHeight = height;
	}

	void SpriteString::GetSize(float* width, float* height) {
		if (width) *width = charWidth;
		if (height) *height = charHeight;
	}

	void SpriteString::ResetSize() {
		charWidth = texture->GetWidth() / 16.0f;
		charHeight = texture->GetHeight() / 8.0f;
	}

	void SpriteString::SetAutoFlush(bool flush) {
		autoFlush = flush;
	}
	bool SpriteString::GetAutoFlush() {
		return autoFlush;
	}
	void SpriteString::Flush() {
		sprite->Flush();
	}

}
