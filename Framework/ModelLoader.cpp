#include "Model.h"
#include <stdio.h>

#pragma region Load Function
namespace Models {

	namespace {
		int ReadParam(char* buffer, size_t bufferSize, char* &p) {
			char* start = p;
			while (iswspace(*start) && *start != '/' && *start != '\0') start++;

			char* slash = start;
			while (*slash != '/' && *slash != '\0') slash++;

			if (slash == start) {
				buffer[0] = '\0';
				p = start + 1;
				return 0;
			}

			char* end = slash;
			while (iswspace(*(end - 1)) && (end - 1) != p) end--;

			int count = 0;
			for (auto rp = start; rp != end; rp++){
				if (count >= bufferSize - 1) break;

				buffer[count++] = *rp;
			}
			buffer[count] = '\0';
			p = slash;
			if (*slash == '/') p++;

			return count;
		}
		template<int Length>
		inline int ReadParam(char(&buffer)[Length], char* &p){
			return ReadParam(buffer, Length, p);
		}

		template <typename T, int Count = sizeof(T) / 4>
		inline int ReadNums(char* &p, T& v){
			char part[128];
			char *sp = part, *ep = nullptr;
			double n;
			ReadParam(part, p);

			float* fp = (float*)&v;

			for (int i = 0; i < Count; i++){
				n = strtod(sp, &ep);
				if (sp == ep) return i;
				*fp++ = n;
				sp = ep;
			}

			return Count;

		}

		Vertex ReadVertex(char* p, Vertex& tpl){
			// v/<position>/<color>/<emit>
			Vertex v = tpl;

			// 位置情報の読み取り
			ReadNums(p, v.position);

			// 色情報１の読み取り
			ReadNums(p, v.color);

			// 色情報２の読み取り
			ReadNums(p, v.emit);

			return v;
		}

		int ReadMultipleInteger(char* p, int** outptr){
			vector<int> is;
			char* s = p;
			char* e = nullptr;

			while ((!e || *e != '\0') && *s != '\0'){
				int i = (int)strtol(s, &e, 10);
				if (e == s) break;
				is.push_back(i);
				s = e;
			}

			*outptr = new int[is.size()];
			int* ap = *outptr;
			for (int i : is){
				*ap++ = i;
			}

			return is.size();
		}

		void ReadSubsetParam(char* p, ColoringType* color, SubsetParameter* param){
			// g/[c|e|l]/<basecolor>/<edgegradient>

			char str[32];

			ReadParam(str, p);
			switch (tolower(str[0])){
			case 'c':
				*color = ColoringType::COLORING_NORMAL;
				break;
			case 'e':
				*color = ColoringType::COLORING_EMIT;
				break;
			case 'l':
			default:
				*color = ColoringType::COLORING_LIGHTED;
				break;
			}

			ReadNums(p, param->BaseColor);
			ReadNums(p, param->EdgeGradient);

		}

		void ReadIndices(char* p, vector<ModelSubset::IndexBuffer::index_t> &vec, int currentCount){
			// t/<indices...>

			int* indices;
			int count;

			count = ReadMultipleInteger(p, &indices);
			if (count < 3) {
				// 不足している -> 何もプッシュしない
				delete[] indices;
				return;
			}

			int root = indices[0] < 0 ? currentCount + indices[0] : indices[0];
			int first = indices[1] < 0 ? currentCount + indices[1] : indices[1];
			for (int i = 2; i < count; i++){
				int second = indices[i] < 0 ? currentCount + indices[i] : indices[i];

				vec.push_back(root);
				vec.push_back(first);
				vec.push_back(second);

				first = second;
			}
			delete[] indices;
		}
	}

	Model* Model::Load(const TCHAR* filename) {
		FILE* fp = _tfopen(filename, _T("rt"));
		if (!fp) return nullptr;

		char line[256] = { 0 };

		char* p = nullptr;

		Vertex vtemplate;
		vtemplate.position = XMFLOAT4(0, 0, 0, 1);
		vtemplate.color = XMFLOAT4(1, 1, 1, 1);
		vtemplate.emit = XMFLOAT4(0, 0, 0, 0);

		vector<Vertex> vertices;

		vector<shared_ptr<ModelSubset>> subsets;
		vector<ModelSubset::IndexBuffer::index_t> indices;
		ColoringType sscolor;
		SubsetParameter ssparam;

		while (p = fgets(line, 256, fp)) {
			// コメントか空白行であった場合はスキップ
			while (iswspace(*p)) p++;
			if (*p == '#' || *p == '\0') continue;

			// 最初のパラメータ(命令種別)を読む
			char op[4];
			ReadParam(op, p);
			switch (op[0]){
			case 'V':
				// 頂点テンプレート
				vtemplate = ReadVertex(p, vtemplate);
				break;
			case 'v':
				// 頂点
				vertices.push_back(ReadVertex(p, vtemplate));
				break;
			case 'g':
				// グループ開始
				if (indices.size() > 0){
					// 前のグループが存在したらその分をサブセットとして確定
					auto data = indices.data();
					subsets.push_back(shared_ptr<ModelSubset>(new ModelSubset(
						indices.size(),
						data,
						&ssparam,
						false,
						sscolor)));

					indices.clear();
				}
				ReadSubsetParam(p, &sscolor, &ssparam);
				break;
			case 'f':
				// 三角形
				ReadIndices(p, indices, vertices.size());
				break;
			}
		}

		// ループ終了後
		if (indices.size() > 0){
			// 前のグループが存在したらその分をサブセットとして確定
			auto data = indices.data();
			subsets.push_back(shared_ptr<ModelSubset>(new ModelSubset(
				indices.size(),
				data,
				&ssparam,
				false,
				sscolor)));

			indices.clear();
		}


		auto vp = vertices.data();
		Model* m = new Model(vp, vertices.size());
		std::copy(subsets.begin(), subsets.end(), std::back_inserter(m->subsets));

		fclose(fp);

		return m;
	}
}
#pragma endregion
