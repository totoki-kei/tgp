#include "Model.h"
#include <stdio.h>

#include <map>
#include <string>
#include <functional>

#pragma region Load Function
namespace Models {
	using std::map;
	using std::string;
	using std::function;

	const double M_PI = 3.14159265358979323846;
	const double M_PI_2 = 1.57079632679489661923;
	const double M_PI_4 = 0.785398163397448309616;
	const double M_1_PI = 0.318309886183790671538;
	const double M_2_PI = 0.636619772367581343076;

	class ModelLoader {
		Vertex vtemplate{ XMFLOAT4(0, 0, 0, 1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(0, 0, 0, 0) };
		vector<shared_ptr<ModelSubset>> subsets;
		vector<ModelSubset::IndexBuffer::index_t> indices;
		ColoringType sscolor;
		SubsetParameter ssparam;
		vector<Vertex> vertices;

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
			Value(double num) :d{ num }, s{}, s_ok{ false }, d_ok{ true }, type{ VAL_NUM }{}
			Value(string& str) :d{ 0 }, s{ str }, s_ok{ true }, d_ok{ false }, type{ VAL_STR }{}
			Value(const Value& v) :d{ v.d }, s{ v.s }, s_ok{ v.s_ok }, d_ok{ v.d_ok }, type{ v.type }{}

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
					// num�֕ϊ�
					this->GetDouble();
					this->type = t;
				}
				else if (t == VAL_STR){
					// string�֕ϊ�
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
				n = GetValue(sp, &ep);
				if (sp == ep) return i;
				*fp++ = n;
				sp = ep;
			}

			return Count;

		}

		Vertex ReadVertex(char* p){
			Vertex& tpl = vtemplate;
			// v/<position>/<color>/<emit>
			Vertex v = tpl;

			// �ʒu���̓ǂݎ��
			ReadNums(p, v.position);

			// �F���P�̓ǂݎ��
			ReadNums(p, v.color);

			// �F���Q�̓ǂݎ��
			ReadNums(p, v.emit);

			return v;
		}

		int ReadMultipleInteger(char* p, int** outptr){
			vector<int> is;
			char* s = p;
			char* e = nullptr;

			while ((!e || *e != '\0') && *s != '\0'){
				int i = (int)GetValue(s, &e);
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

		void ReadIndicesFan(char* p, vector<ModelSubset::IndexBuffer::index_t> &vec, int currentCount){
			// t/<indices...>

			int* indices;
			int count;

			count = ReadMultipleInteger(p, &indices);
			if (count < 3) {
				// �s�����Ă��� -> �����v�b�V�����Ȃ�
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
		void ReadIndicesList(char* p, vector<ModelSubset::IndexBuffer::index_t> &vec, int currentCount){
			// t/<indices...>

			int* indices;
			int count;

			count = ReadMultipleInteger(p, &indices);
			if (count < 3) {
				// �s�����Ă��� -> �����v�b�V�����Ȃ�
				delete[] indices;
				return;
			}

			int first = indices[0] < 0 ? currentCount + indices[0] : indices[0];
			int second = indices[1] < 0 ? currentCount + indices[1] : indices[1];
			for (int i = 2; i < count; i++){
				int third = indices[i] < 0 ? currentCount + indices[i] : indices[i];

				vec.push_back(first);
				vec.push_back(second);
				vec.push_back(third);

				first = second;
				second = third;
			}
			delete[] indices;
		}

		map<string, function<EvalRet(vector<Value>&)> > funcMap;

		double GetValue(const string& s){

			auto it = variables.find(s);
			if (it != variables.end()){
				// �������̂Œl��Ԃ�
				return it->second;
			}
			else {
				// ������Ȃ�����
				const char* p = s.c_str();
				char* q = nullptr;
				return strtod(p, &q);
			}
		}

		double GetValue(char* s, char** sp){
			double ret = 0;

			ret = strtod(s, sp);

			if (s == *sp){
				// �ϐ��Ƃ��ēǂݎ�������
				while (isspace(*s)) s++;
				if (*s == '\0') return 0;

				char* ep = s;
				while (*ep != '\0' && !isspace(*ep)) ep++;

				string ss(s, ep);

				auto it = variables.find(ss);
				if (it != variables.end()){
					// �������̂Œl��Ԃ�
					if (sp) *sp = ep;
					return it->second;
				}
				else {
					// ������Ȃ�����
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





			/*
			funcMap.insert({ { "" }, [this](SV& sv){
			return EvalRet::Continue;
			} });
			*/

		}

		EvalRet Evaluate(char* &p){
			if (funcMap.empty()) InitFuncMap();

			// �t�|�[�����h�L�@�ň���
			char token[32];

			// �ŏ��̃g�[�N����'*'�Ȃ̂œǂݔ�΂�
			ReadParam(token, p);

			while (ReadParam(token, p)){
				string tokenStr(token);

				auto fn = funcMap.find(tokenStr);
				if (fn != funcMap.end()) {
					// ���߂ƃ}�b�`����
					EvalRet ret = fn->second(evalStack);
					if (ret != EvalRet::Continue) return ret;
				}
				else {
					// �v�b�V��
					evalStack.push_back(tokenStr);
				}
			}

			return EvalRet::Continue;
		}
	public:

		Model* Load(const TCHAR* filename) {
			DBG_OUT("Model load start : %s\n", filename);
			FILE* fp = _tfopen(filename, _T("rt"));
			if (!fp) return nullptr;

			char line[256] = { 0 };

			char* p = nullptr;

			// �t�@�C�����e�̋z���o��
			lineNo = 1;
			while (fgets(line, 256, fp)) {
				lineCache.insert(std::pair<int, string>(lineNo++, string(line)));
			}
			totalLines = lineNo - 1;

			for (lineNo = 1; 0 < lineNo && lineNo <= totalLines; lineNo++) {
				strcpy_s(line, lineCache.at(lineNo).c_str());
				char* p = line;

				// �R�����g���󔒍s�ł������ꍇ�̓X�L�b�v
				while (iswspace(*p)) p++;
				if (*p == '#' || *p == '\0') continue;

				// �擪�̕������A�X�^���X�N�������ꍇ�̓X�N���v�g�s�Ƃ��ĕ]������
				if (*p == '*') {
					EvalRet ret = Evaluate(p);
					if (ret == EvalRet::Exit){
						break;
					}
					continue;
				}

				// �ŏ��̃p�����[�^(���ߎ��)��ǂ�
				char op[4];
				ReadParam(op, p);
				switch (op[0]){
				case 'V':
					// ���_�e���v���[�g
					vtemplate = ReadVertex(p);
					break;
				case 'v':
					// ���_
					vertices.push_back(ReadVertex(p));
					break;
				case 'g':
					// �O���[�v�J�n
					if (indices.size() > 0){
						// �O�̃O���[�v�����݂����炻�̕����T�u�Z�b�g�Ƃ��Ċm��
						auto data = indices.data();
						subsets.push_back(shared_ptr<ModelSubset>(new ModelSubset(
							indices.size(),
							data,
							&ssparam,
							false,
							sscolor)));

						DBG_OUT("subset : %d indices\n", indices.size());
						indices.clear();
					}
					ReadSubsetParam(p, &sscolor, &ssparam);
					break;
				case 'f':
					// �O�p�`(TriangleFan)
					ReadIndicesFan(p, indices, vertices.size());
					break;
				case 'F':
					// �O�p�`(TriangleList)
					ReadIndicesList(p, indices, vertices.size());
					break;
				}
			}

			// ���[�v�I����
			if (indices.size() > 0){
				// �O�̃O���[�v�����݂����炻�̕����T�u�Z�b�g�Ƃ��Ċm��
				auto data = indices.data();
				subsets.push_back(shared_ptr<ModelSubset>(new ModelSubset(
					indices.size(),
					data,
					&ssparam,
					false,
					sscolor)));

				DBG_OUT("subset : %d indices\n", indices.size());
				indices.clear();
			}

			DBG_OUT("Total %d vertices, %d subsets loaded.\n", vertices.size(), subsets.size());

			auto vp = vertices.data();
			Model* m = new Model(vp, vertices.size());
			for (auto s : subsets){
				m->AddSubset(s);
			}

			fclose(fp);

			DBG_OUT("Model load finished : %s (model : #%d[%p])\n", filename, m->GetResourceID(), m);

			return m;
		}
	};

	Model* Model::Load(const TCHAR* filename) {
		ModelLoader loader;
		auto ret = loader.Load(filename);
		return ret;
	}



}
#pragma endregion
