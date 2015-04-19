#include "Model.h"
#include <stdio.h>

#include <array>
#include <map>
#include <set>
#include <string>
#include <functional>

#pragma region Load Function
namespace Models {
	using std::map;
	using std::set;
	using std::string;
	using std::function;

	const double M_PI = 3.14159265358979323846;
	const double M_PI_2 = 1.57079632679489661923;
	const double M_PI_4 = 0.785398163397448309616;
	const double M_1_PI = 0.318309886183790671538;
	const double M_2_PI = 0.636619772367581343076;

	namespace {
		struct Line {
			union {
				struct {
					int a, b;
				};
				int idx[2];
			};

			Line(int a_, int b_){
				a = a_;
				b = b_;
			}

			bool operator ==(const Line& l) const {
				return (a == l.a && b == l.b) || (a == l.b && b == l.a);
			}
			bool operator !=(const Line& l) const {
				return !(*this == l);
			}

			static void CreateIndices(vector<Line>& lines, vector<D3DIndexBuffer<>::index_t>& indices) {
				for (auto& it_a = lines.begin(); it_a != lines.end(); it_a++) {
					const auto& line = *it_a;

					bool isDupLine = false;
					for (auto& it_b = lines.begin(); it_b != it_a; it_b++) {
						const auto& line2 = *it_b;
						if (line == line2) {
							isDupLine = true;
							break;
						}
					}
					if (isDupLine) {
						continue;
					}

					indices.push_back(line.a);
					indices.push_back(line.b);
				}
			}
		};

		struct Triangle {
			union {
				struct {
					int a, b, c;
				};
				int idx[3];
			};

			Triangle(int a_, int b_, int c_){
				a = a_;
				b = b_;
				c = c_;
			}

			// 指定したインデックスがこの三角形の頂点として使用されているかチェックする。
			// 使用されていた場合、何番目の頂点かを0～2で返す。
			// 使用されていない場合は-1を返す。
			int Contains(int vertexIndex) const {
				if (a == vertexIndex) return 0;
				else if (b == vertexIndex) return 1;
				else if (c == vertexIndex) return 2;
				else return -1;
			}

			bool operator <(const Triangle& t) const {
				const Triangle& a = *this;
				const Triangle& b = t;
				int ai, bi;

				if (a.a < a.b && a.a < a.c)	ai = 0;
				else if (a.b < a.a && a.b < a.c) ai = 1;
				else if (a.c < a.a && a.c < a.b) ai = 2;

				if (b.a < b.b && b.a < b.c)	bi = 0;
				else if (b.b < b.a && b.b < b.c) bi = 1;
				else if (b.c < b.a && b.c < b.b) bi = 2;


				int ra = a.idx[(ai + 0) % 3] - b.idx[(bi + 0) % 3];
				if (ra < 0) return true;
				else if (ra > 0) return false;
				else {
					int rb = a.idx[(ai + 1) % 3] - b.idx[(bi + 1) % 3];
					if (rb < 0) return true;
					else if (rb > 0) return false;
					else {
						int rc = a.idx[(ai + 2) % 3] - b.idx[(bi + 2) % 3];
						if (rc < 0) return true;
						else return false;
					}
				}
			}

			// 同じ三角形である場合は真を返す。表裏を区別する。
			// (operator ==ではなぜか動いてくれなかったのでメンバ関数として実装する)
			bool IsSame(const Triangle& t) const {
				const Triangle& a = *this;
				const Triangle& b = t;
				int ai, bi;

				if (a.a < a.b && a.a < a.c)	ai = 0;
				else if (a.b < a.a && a.b < a.c) ai = 1;
				else if (a.c < a.a && a.c < a.b) ai = 2;

				if (b.a < b.b && b.a < b.c)	bi = 0;
				else if (b.b < b.a && b.b < b.c) bi = 1;
				else if (b.c < b.a && b.c < b.b) bi = 2;


				return a.idx[(ai + 0) % 3] == b.idx[(bi + 0) % 3]
					&& a.idx[(ai + 1) % 3] == b.idx[(bi + 1) % 3]
					&& a.idx[(ai + 2) % 3] == b.idx[(bi + 2) % 3];
			}


			// 別の三角形と照らし合わせ、この三角形の隣接頂点が含まれていた場合はその隣接頂点を返す。
			// 含まれていなかった場合は-1を返す。
			// 引数 adjNo に有効なポインタが設定されていた場合は、そのアドレスに
			// 何番目の頂点の対となる隣接頂点かを格納する。
			// 例： this{ 0, 1, 2 } , t{ 1, 0, 3 }の場合
			// +----------------------------------+ 
			// | aa = false ab = true  ac = false | ax = true
			// | ba = true  bb = false bc = false | bx = true
			// | ca = false cb = false cc = false | cx =[false]
			// +----------------------------------+
			//   xa = true  xb = true  xc =[false]
			// これより、this[2] -> 2, t[2] -> 3 が共有されていない頂点とわかるので
			// 戻り値は (t[2] ->) 3 , adjNoには 2 が格納される。
			int GetAdjacency(const Triangle& t, int* adjNo = nullptr) const {
				bool aa = a == t.a;
				bool ab = a == t.b;
				bool ac = a == t.c;
				bool ba = b == t.a;
				bool bb = b == t.b;
				bool bc = b == t.c;
				bool ca = c == t.a;
				bool cb = c == t.b;
				bool cc = c == t.c;

				bool ax = aa || ab || ac;
				bool bx = ba || bb || bc;
				bool cx = ca || cb || cc;

				bool xa = aa || ba || ca;
				bool xb = ab || bb || cb;
				bool xc = ac || bc || cc;

				if (!ax && bx && cx) { if (adjNo) *adjNo = 0; }
				else if (ax && !bx && cx) { if (adjNo) *adjNo = 1; }
				else if (ax && bx && !cx) { if (adjNo) *adjNo = 2; }
				else { return -1; }

				if (!xa && xb && xc) { return t.a; }
				else if (xa && !xb && xc) { return t.b; }
				else if (xa && xb && !xc) { return t.c; }
				else return -1;
			}

			// 三角形の辺を得る
			template<typename Collection>
			void MakeLine(Collection& lines) const {
				lines.insert(Line{ a, b });
				lines.insert(Line{ b, c });
				lines.insert(Line{ c, a });
			}


			// 隣接情報付き三角形のインデックスを生成
			static void CreateFaceIndicesWithAdj(vector<Triangle> &triangles, vector<D3DIndexBuffer<>::index_t> &faceIndices) {
				for (const auto& tri : triangles){
					int adj[3] = { -1, -1, -1 };
					for (const auto& tri2 : triangles){
						int adjNo = -1;
						int adjIdx = tri.GetAdjacency(tri2, &adjNo);
						if (adjNo != -1 && adj[adjNo] == -1) {
							adj[adjNo] = adjIdx;
						}
					}
					if (adj[0] == -1) adj[0] = tri.a;
					if (adj[1] == -1) adj[1] = tri.b;
					if (adj[2] == -1) adj[2] = tri.c;

					faceIndices.push_back(tri.a);
					faceIndices.push_back(adj[2]);
					faceIndices.push_back(tri.b);
					faceIndices.push_back(adj[0]);
					faceIndices.push_back(tri.c);
					faceIndices.push_back(adj[1]);

				}
			}
			// 三角形のインデックスを生成
			static void CreateFaceIndices(vector<Triangle> &triangles, vector<D3DIndexBuffer<>::index_t> &faceIndices) {
				for (auto& it_a = triangles.begin(); it_a != triangles.end(); it_a++) {

					bool isDupTriangle = false;
					for (auto& it_b = triangles.begin(); it_b != it_a; it_b++) {
						if (it_a->IsSame(*it_b)) {
							isDupTriangle = true;
							break;
						}
					}
					if (isDupTriangle) {
						continue;
					}

					faceIndices.push_back(it_a->a);
					faceIndices.push_back(it_a->b);
					faceIndices.push_back(it_a->c);

				}
			}

			// 頂点位置を参照しながら、この三角形の法線を計算する。
			// 戻り値XMVECTORのうち、先頭3要素のみ有効な値になる。
			XMVECTOR CalculateNormal(vector<Vertex>& vertices) const {
				XMFLOAT4 pa = vertices[a].position;
				XMFLOAT4 pb = vertices[b].position;
				XMFLOAT4 pc = vertices[c].position;

				XMVECTOR va = XMVectorSet( pa.x, pa.y, pa.z, pa.w );
				XMVECTOR vb = XMVectorSet( pb.x, pb.y, pb.z, pb.w );
				XMVECTOR vc = XMVectorSet( pc.x, pc.y, pc.z, pc.w );

				return XMVector3Normalize(XMVector3Cross(XMVectorSubtract(vb, va), XMVectorSubtract(vc, va)));

			}

			// 全三角形の法線を計算し、頂点情報に付与する。
			// 各頂点に付与される法線は、その頂点が含まれている三角形の法線の平均ベクトルになる。
			static void CalculateNormal(vector<Vertex>& vertices, vector<Triangle>& triangles) {
				// 作業用配列を作成する(16バイト境界で確保)
				XMVECTOR* normalVector = (XMVECTOR*)_aligned_malloc(sizeof( XMVECTOR) * vertices.size(), 16);
				memset(normalVector, 0, sizeof(XMVECTOR) * vertices.size());

				for (auto& tri  : triangles) {
					auto normal = tri.CalculateNormal(vertices);

					normalVector[tri.a] = XMVectorAdd(normalVector[tri.a], normal);
					normalVector[tri.b] = XMVectorAdd(normalVector[tri.b], normal);
					normalVector[tri.c] = XMVectorAdd(normalVector[tri.c], normal);
				}


				for (unsigned i = 0; i < vertices.size(); i++) {
					auto nn = XMVector3Normalize(normalVector[i]);
					vertices[i].normal = XMFLOAT3{ nn.m128_f32 };
				}

				_aligned_free(normalVector);
			}

			Triangle Reverse() {
				return Triangle{ c, b, a };
			}

		};



	}


	class ModelLoader {
		Vertex vtemplate{ XMFLOAT4(0, 0, 0, 1), XMFLOAT4(1, 1, 1, 1), XMFLOAT3(0, 0, 0) };
		Vertex::ExtraInfo vtemplate_extra{ 0 };

		vector<Vertex> vertices;
		vector<Vertex::ExtraInfo> vertices_extra;

		int lineNo;
		int totalLines;
		map<int, string> lineCache;
		map<string, double> variables;

		enum class EvalRet{
			Continue,
			Break,
			Exit,
		};

		class Value;

		static string NumToStr(double n){
			char s[32];
			sprintf_s(s, "%f", n);
			return string(s);
		}
		static Value Pop(vector<Value>& vec){
			Value s = *vec.rbegin();
			vec.pop_back();
			return s;
		}



		class Value {
		public:
			enum Type{
				VAL_NUM,
				VAL_STR,
			};

		private:
			Type type;
			double d;
			string s;
			bool d_ok;
			bool s_ok;

		public:
			Value() :d{ 0 }, s{}, s_ok{ false }, d_ok{ true }, type{ VAL_NUM }{}
			Value(const double num) :d{ num }, s{}, s_ok{ false }, d_ok{ true }, type{ VAL_NUM }{}

			Value(const string& str) :d{ 0 }, s(str), s_ok{ true }, d_ok{ false }, type{ VAL_STR } { }
			Value(string&& str) :d{ 0 }, s(std::move(str)), s_ok{ true }, d_ok{ false }, type{ VAL_STR } { }

			Value(const Value& v) :d{ v.d }, s(v.s), s_ok{ v.s_ok }, d_ok{ v.d_ok }, type{ v.type }{ }

			inline void SetDouble(double n){ d = n; d_ok = true; s_ok = false; type = VAL_NUM; }
			inline double GetDouble() {
				if (!d_ok){
					if (s_ok){
						d = strtod(s.c_str(), nullptr);
					}
					else {
						d = 0;
					}
					d_ok = true;
				}
				return d;
			}
			__declspec(property(get = GetDouble, put = SetDouble))
				double $Number;

			inline void SetString(const string& str){ s = str; d_ok = false; s_ok = true; type = VAL_STR; }
			inline const string& GetString() {
				if (!s_ok){
					if (d_ok){
						s = NumToStr(d);
					}
					else {
						s.clear();
					}
					s_ok = true;
				}
				return s;
			}
			__declspec(property(get = GetString, put = SetString))
				const string& $String;

			inline Type GetType() const { return type; }
			inline void SetType(Type t){
				if (this->type == t) return;
				if (t == VAL_NUM){
					// numへ変換
					this->GetDouble();
					this->type = t;
				}
				else if (t == VAL_STR){
					// stringへ変換
					this->GetString();
					this->type = t;
				}
			}
			__declspec(property(get = GetType, put = SetType))
				Type $Type;


			inline bool HasNumber() const { return d_ok; }
			inline bool HasString() const { return s_ok; }
		};
		vector<Value> evalStack;


		size_t ReadParam(char* buffer, size_t bufferSize, char* &p) {
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

			size_t count = 0;
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
		inline size_t ReadParam(char(&buffer)[Length], char* &p){
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
				n = GetValue(sp, &ep);
				if (sp == ep) return i;
				*fp++ = (float)n;
				sp = ep;
			}

			return Count;

		}

		Vertex ReadVertex(char* p, int* midx){
			Vertex& tpl = vtemplate;
			// v/<position>/<color>/<emit>
			Vertex v = tpl;

			// 位置情報の読み取り
			ReadNums(p, v.position);

			// 色情報の読み取り
			ReadNums(p, v.color);

			// マルチマテリアルが有効だった場合はそれも読む
			if (midx) ReadInteger(p, midx);

			return v;
		}

		bool ReadInteger(char* p, int* outptr) {
			if (!p) return false;

			char* s = p;
			char* e = nullptr;

			int i = (int)GetValue(s, &e);
			if (e == s) return false;
			if (i < 0 || 4 <= i) i = 0;

			*outptr = i;

			return true;
		}

		size_t ReadMultipleInteger(char* p, std::unique_ptr<int[]> &outarray){
			vector<int> is;
			char* s = p;
			char* e = nullptr;

			while ((!e || *e != '\0') && *s != '\0'){
				int i = (int)GetValue(s, &e);
				if (e == s) break;
				is.push_back(i);
				s = e;
			}

			outarray.reset(new int[is.size()]);
			int* ap = outarray.get() + 0;
			for (int i : is){
				*ap++ = i;
			}

			return is.size();
		}

		struct Option {
			bool auto_mirror;
			bool multi_material;

			Option()
				: auto_mirror{ false }
				, multi_material{ false } { }

		} option;

		void ReadOption(char *p) {
			while (iswspace(*p)) p++;

			if (_strnicmp(p, "auto_mirror", strlen("auto_mirror")) == 0) {
				option.auto_mirror = true;
				return;
			}
			if (_strnicmp(p, "multi_material", strlen("multi_material")) == 0) {
				option.multi_material = true;
				if (!vertices.empty()) {
					vertices_extra.resize(vertices.size(), Vertex::ExtraInfo{ 0 });
				}
				return;
			}

		}


		void ReadIndicesFan(char* p, vector<Triangle>& triangleList, vector<Line>& lineList, size_t currentCount, bool triangleEnabled, bool lineEnabled) {
			// f/<indices...>

			std::unique_ptr<int[]> indices;

			auto count = ReadMultipleInteger(p, indices);
			if (count < 2) {
				// 不足している -> 何もプッシュしない
				return;
			}

			int root = indices[0] < 0 ? (int)currentCount + indices[0] : indices[0];
			int first = indices[1] < 0 ? (int)currentCount + indices[1] : indices[1];
			if (lineEnabled) lineList.push_back(Line{ root, first });
			for (size_t i = 2; i < count; i++) {
				int second = indices[i] < 0 ? (int)currentCount + indices[i] : indices[i];

				if (triangleEnabled) triangleList.push_back(Triangle{ root, first, second });
				if (lineEnabled) lineList.push_back(Line{ first, second });

				first = second;
			}
			if (count >= 3) {
				// 三角形が一つ以上発生していた場合は最後を閉じる
				if (lineEnabled) lineList.push_back(Line{ first, root });
			}
		}
		void ReadIndicesList(char* p, vector<Triangle>& triangleList, vector<Line>& lineList, size_t currentCount, bool triangleEnabled, bool lineEnabled) {
			// F/<indices...>

			std::unique_ptr<int[]> indices;
			auto count = ReadMultipleInteger(p, indices);
			if (count < 2) {
				// 不足している -> 何もプッシュしない
				return;
			}

			int first = indices[0] < 0 ? (int)currentCount + indices[0] : indices[0];
			int second = indices[1] < 0 ? (int)currentCount + indices[1] : indices[1];
			if (lineEnabled) lineList.push_back(Line{ first, second });
			for (size_t i = 2; i < count; i++){
				int third = indices[i] < 0 ? (int)currentCount + indices[i] : indices[i];

				if(triangleEnabled) triangleList.push_back(Triangle{ first, second, third });
				if(lineEnabled) lineList.push_back(Line{ first, third });

				first = second;
				second = third;
			}
			if (count >= 3) {
				// 三角形が一つ以上発生していた場合は最後を閉じる
				if (lineEnabled) lineList.push_back(Line{ first, second });
			}
		}

		void ReadInstParam(char* p, InstanceData& inst) {
			float indexf;
			float4 data;

			ReadNums(p, indexf);
			ReadNums(p, data);

			int index = static_cast<int>(indexf);
			if (index < 0 || 4 <= index) return;

			CopyMemory(&inst.Params[index], &data, sizeof(InstanceData::ParamSet));

		}

		map<string, function<EvalRet(vector<Value>&)> > funcMap;

		double GetValue(const string& s){

			auto it = variables.find(s);
			if (it != variables.end()){
				// 見つけたので値を返す
				return it->second;
			}
			else {
				// 見つからなかった
				const char* p = s.c_str();
				char* q = nullptr;
				return strtod(p, &q);
			}
		}

		double GetValue(char* s, char** sp){
			double ret = 0;

			ret = strtod(s, sp);

			if (s == *sp){
				// 変数として読み取りを試す
				while (isspace(*s)) s++;
				if (*s == '\0') return 0;

				char* ep = s;
				while (*ep != '\0' && !isspace(*ep)) ep++;

				string ss(s, ep);

				auto it = variables.find(ss);
				if (it != variables.end()){
					// 見つけたので値を返す
					if (sp) *sp = ep;
					return it->second;
				}
				else {
					// 見つからなかった
					return 0;
				}
			}

			return ret;

		}
		double GetValue(Value& v){
			if (v.GetType() == Value::VAL_STR){
				return GetValue(v.$String);
			}
			else {
				return v.$Number;
			}
		}



		void InitFuncMap(){
			funcMap.clear();

			typedef vector<Value> SV;


			funcMap.insert({ { "" }, [this](SV& sv){
				evalStack.clear();
				return EvalRet::Continue;
			} });

			funcMap.insert({ { "set" }, [this](SV& sv){
				Value sval = Pop(sv), starget = Pop(sv);
				double value = GetValue(sval);
				variables[starget.$String] = value;
				return EvalRet::Continue;
			} });
			funcMap.insert({ { "add" }, [this](SV& sv){
				Value sright = Pop(sv), sleft = Pop(sv);
				double vright = GetValue(sright), vleft = GetValue(sleft);
				sv.push_back(vleft + vright);
				return EvalRet::Continue;
			} });
			funcMap.insert({ { "sub" }, [this](SV& sv){
				Value sright = Pop(sv), sleft = Pop(sv);
				double vright = GetValue(sright), vleft = GetValue(sleft);
				sv.push_back(vleft - vright);
				return EvalRet::Continue;
			} });
			funcMap.insert({ { "mul" }, [this](SV& sv){
				Value sright = Pop(sv), sleft = Pop(sv);
				double vright = GetValue(sright), vleft = GetValue(sleft);
				sv.push_back(vleft * vright);
				return EvalRet::Continue;
			} });
			funcMap.insert({ { "div" }, [this](SV& sv){
				Value sright = Pop(sv), sleft = Pop(sv);
				double vright = GetValue(sright), vleft = GetValue(sleft);
				sv.push_back(vleft / vright);
				return EvalRet::Continue;
			} });
			funcMap.insert({ { "neg" }, [this](SV& sv){
				Value s = Pop(sv);
				double v = GetValue(s);
				sv.push_back(-v);
				return EvalRet::Continue;
			} });
			funcMap.insert({ { "sin" }, [this](SV& sv){
				Value s = Pop(sv);
				double v = GetValue(s);
				sv.push_back(sin(v));
				return EvalRet::Continue;
			} });
			funcMap.insert({ { "cos" }, [this](SV& sv){
				Value s = Pop(sv);
				double v = GetValue(s);
				sv.push_back(cos(v));
				return EvalRet::Continue;
			} });
			funcMap.insert({ { "tan" }, [this](SV& sv){
				Value s = Pop(sv);
				double v = GetValue(s);
				sv.push_back(tan(v));
				return EvalRet::Continue;
			} });

			funcMap.insert({ { "rad2deg" }, [this](SV& sv){
				Value s = Pop(sv);
				double v = GetValue(s);
				sv.push_back(180 * v * M_1_PI);
				return EvalRet::Continue;
			} });
			funcMap.insert({ { "deg2rad" }, [this](SV& sv){
				Value s = Pop(sv);
				double v = GetValue(s);
				sv.push_back(M_PI * v / 180);
				return EvalRet::Continue;
			} });

			funcMap.insert({ { "pi" }, [this](SV& sv){
				sv.push_back(M_PI);
				return EvalRet::Continue;
			} });


			funcMap.insert({ { "vcount" }, [this](SV& sv){
				sv.push_back((double)vertices.size());
				return EvalRet::Continue;
			} });
			funcMap.insert({ { "line" }, [this](SV& sv){
				sv.push_back(lineNo);
				return EvalRet::Continue;
			} });
			funcMap.insert({ { "jump" }, [this](SV& sv){
				Value s = Pop(sv);
				double v = GetValue(s);
				lineNo = (int)v;
				return EvalRet::Continue;
			} });
			funcMap.insert({ { "skip" }, [this](SV& sv){
				Value s = Pop(sv);
				double v = GetValue(s);
				lineNo += (int)v;
				return EvalRet::Continue;
			} });

			funcMap.insert({ { "ifzero" }, [this](SV& sv){
				Value s1 = Pop(sv), sfalse = Pop(sv), strue = Pop(sv);
				double j = GetValue(s1);
				if (j == 0.0) {
					sv.push_back(strue);
				}
				else {
					sv.push_back(sfalse);
				}
				return EvalRet::Continue;
			} });
			funcMap.insert({ { "ifpos" }, [this](SV& sv){
				Value s1 = Pop(sv), sfalse = Pop(sv), strue = Pop(sv);
				double j = GetValue(s1);
				if (j > 0.0) {
					sv.push_back(strue);
				}
				else {
					sv.push_back(sfalse);
				}
				return EvalRet::Continue;
			} });
			funcMap.insert({ { "ifneg" }, [this](SV& sv){
				Value s1 = Pop(sv), sfalse = Pop(sv), strue = Pop(sv);
				double j = GetValue(s1);
				if (j < 0.0) {
					sv.push_back(strue);
				}
				else {
					sv.push_back(sfalse);
				}
				return EvalRet::Continue;
			} });

			funcMap.insert({ { "cat" }, [this](SV& sv){
				Value sright = Pop(sv), sleft = Pop(sv);
				sv.push_back(sleft.$String + sright.$String);
				return EvalRet::Continue;
			} });




			/*
			funcMap.insert({ { "" }, [this](SV& sv){
			return EvalRet::Continue;
			} });
			*/

		}

		EvalRet Evaluate(char* &p){
			if (funcMap.empty()) InitFuncMap();

			// 逆ポーランド記法で扱う
			char token[32];

			// 最初のトークンは'*'なので読み飛ばす
			ReadParam(token, p);

			while (ReadParam(token, p)){
				string tokenStr(token);

				auto fn = funcMap.find(tokenStr);
				if (fn != funcMap.end()) {
					// 命令とマッチした
					EvalRet ret = fn->second(evalStack);
					if (ret != EvalRet::Continue) return ret;
				}
				else {
					// プッシュ
					evalStack.push_back(tokenStr);
				}
			}

			return EvalRet::Continue;
		}
	public:

		bool solid, edge;
		ModelLoader(bool s, bool e) : solid{ s }, edge{ e } {}

		Model* Load(const TCHAR* filename) {
			LOG_DBG("Model load start : %s\n", filename);
			FILE* fp = nullptr;
			int err = _tfopen_s(&fp, filename, _T("rt"));
			if (err != 0) return nullptr;

			char line[256] = { 0 };

			// ファイル内容の吸い出し
			lineNo = 1;
			while (fgets(line, 256, fp)) {
				lineCache.insert(std::pair<int, string>(lineNo++, string(line)));
			}
			totalLines = lineNo - 1;

			vector<Triangle> triangleList;
			vector<Line> lineList;

			InstanceData* defaultInstData = nullptr;

			for (lineNo = 1; 0 < lineNo && lineNo <= totalLines; lineNo++) {
				strcpy_s(line, lineCache.at(lineNo).c_str());
				char* p = line;

				// 先頭がBOMだった場合は強制的に読み飛ばす
				/*
					p[0]	0xef '・'
					p[1]	0xbb 'ｻ'
					p[2]	0xbf 'ｿ'	
				*/
				if ((unsigned char)p[0] == 0xef && (unsigned char)p[1] == 0xbb && (unsigned char)p[2] == 0xbf) {
					p += 3;
				}

				// コメントか空白行であった場合はスキップ
				while (iswspace(*p)) p++;
				if (*p == '#' || *p == '\0') continue;

				// 先頭の文字がアスタリスクだった場合はスクリプト行として評価する
				if (*p == '*') {
					EvalRet ret = Evaluate(p);
					if (ret == EvalRet::Exit){
						break;
					}
					continue;
				}

				// 最初のパラメータ(命令種別)を読む
				char op[4];
				ReadParam(op, p);
				switch (op[0]){
				case 'V':
					// 頂点テンプレート
					vtemplate = ReadVertex(p, option.multi_material ? reinterpret_cast<int*>(&vtemplate_extra.paramIndex) : nullptr);
					break;
				case 'v':
					// 頂点
				{
					if (option.multi_material) {
						int midx = static_cast<int>(vtemplate_extra.paramIndex);
						vertices.push_back(ReadVertex(p, &midx));
						vertices_extra.push_back(Vertex::ExtraInfo{ midx });
					}
					else {
						vertices.push_back(ReadVertex(p, nullptr));
					}

				}
					break;
				case 'f':
					// 面(TriangleFan)
					ReadIndicesFan(p, triangleList, lineList, vertices.size(), true, true);
					break;
				case 'F':
					// 面(TriangleList)
					ReadIndicesList(p, triangleList, lineList, vertices.size(), true, true);
					break;
				case 't':
					// 面(TriangleFan) - Solidのみ
					ReadIndicesFan(p, triangleList, lineList, vertices.size(), true, false);
					break;
				case 'T':
					// 面(TriangleList) - Solidのみ
					ReadIndicesList(p, triangleList, lineList, vertices.size(), true, false);
					break;
				//case 'l':
				//	// 面(TriangleFan) - Edgeのみ
				//	ReadIndicesFan(p, triangleList, lineList, vertices.size(), false, true);
				//	break;
				//case 'L':
				//	// 面(TriangleList) - Edgeのみ
				//	ReadIndicesList(p, triangleList, lineList, vertices.size(), false, true);
				//	break;
				case 'p':
				case 'P':
					if (!defaultInstData) defaultInstData = new InstanceData{};
					ReadInstParam(p, *defaultInstData);
				case '@':
					// オプション設定
					ReadOption(p);
					break;
				}
			}


			LOG_DBG("Total %d vertices, %d triangles, %d lines loaded.\n", vertices.size(), triangleList.size(), lineList.size());

			vector<D3DIndexBuffer<>::index_t> triangleIndices, lineIndices;

			Triangle::CalculateNormal(vertices, triangleList);
			Triangle::CreateFaceIndices(triangleList, triangleIndices);
			if (option.auto_mirror) {
				vector<D3DIndexBuffer<>::index_t> newIndices;
				newIndices.reserve(triangleIndices.size() * 2);

				for (auto it = triangleIndices.rbegin(); it != triangleIndices.rend(); it++) {
					newIndices.push_back(*it);
				}
				for (auto it = triangleIndices.begin(); it != triangleIndices.end(); it++) {
					newIndices.push_back(*it);
				}

				triangleIndices = std::move(newIndices);
			}
			Line::CreateIndices(lineList, lineIndices);


			Model* m = new Model(
				vertices.data(), vertices.size(), vertices_extra.empty() ? nullptr : vertices_extra.data(),
				solid ? triangleIndices.data() : nullptr, triangleIndices.size(),
				edge ? lineIndices.data() : nullptr, lineIndices.size(), defaultInstData);

			fclose(fp);

			LOG_DBG("Model load finished : %s (model : #%d[%p])\n", filename, m->GetResourceID(), m);

			return m;
		}
	};

	Model* Model::Load(const TCHAR* filename, bool solid, bool edge) {
		ModelLoader loader{ solid, edge };
		auto ret = loader.Load(filename);
		return ret;
	}



}
#pragma endregion
