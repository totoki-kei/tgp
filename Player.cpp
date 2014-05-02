#include "Player.h"

#include "Framework/Model.h"
#include "Game1.h"

using namespace Models;
using std::shared_ptr;

namespace {
	D3DCore* core;

	Model* model;

	void InitializeModel(D3DCore* core_){
		core = core_;

		Vertex vertex[8 + 12];
		SubsetParameter subsetParam;

		// í∏ì_ÇÃèâä˙âª
		{
			int i = 0;
			vertex[i++] = Vertex(XMFLOAT4(-1, 1, 0, 1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));
			vertex[i++] = Vertex(XMFLOAT4(-1, 1, -1, 1), XMFLOAT4(1, 0, 1, 1), XMFLOAT4(1, 1, 1, 1));
			vertex[i++] = Vertex(XMFLOAT4(0, 1, -1, 1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));
			vertex[i++] = Vertex(XMFLOAT4(1, 1, 0, 1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));
			vertex[i++] = Vertex(XMFLOAT4(1, 1, -1, 1), XMFLOAT4(1, 0, 1, 1), XMFLOAT4(1, 1, 1, 1));
			vertex[i++] = Vertex(XMFLOAT4(0, 1, 1, 1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));
			vertex[i++] = Vertex(XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 0, 1, 1), XMFLOAT4(1, 1, 1, 1));
			vertex[i++] = Vertex(XMFLOAT4(-1, 1, 1, 1), XMFLOAT4(1, 0, 1, 1), XMFLOAT4(1, 1, 1, 1));
			vertex[i++] = Vertex(XMFLOAT4(1, 0, 1, 1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));
			vertex[i++] = Vertex(XMFLOAT4(-1, 0, 1, 1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));
			vertex[i++] = Vertex(XMFLOAT4(-1, -1, 1, 1), XMFLOAT4(1, 0, 1, 1), XMFLOAT4(1, 1, 1, 1));
			vertex[i++] = Vertex(XMFLOAT4(0, -1, 1, 1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));
			vertex[i++] = Vertex(XMFLOAT4(1, -1, 1, 1), XMFLOAT4(1, 0, 1, 1), XMFLOAT4(1, 1, 1, 1));
			vertex[i++] = Vertex(XMFLOAT4(1, -1, -1, 1), XMFLOAT4(1, 0, 1, 1), XMFLOAT4(1, 1, 1, 1));
			vertex[i++] = Vertex(XMFLOAT4(-1, -1, -1, 1), XMFLOAT4(1, 0, 1, 1), XMFLOAT4(1, 1, 1, 1));
			vertex[i++] = Vertex(XMFLOAT4(-1, -1, 0, 1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));
			vertex[i++] = Vertex(XMFLOAT4(-1, 0, -1, 1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));
			vertex[i++] = Vertex(XMFLOAT4(1, -1, 0, 1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));
			vertex[i++] = Vertex(XMFLOAT4(0, -1, -1, 1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));
			vertex[i++] = Vertex(XMFLOAT4(1, 0, -1, 1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));
		}


		D3DIndexBuffer<>::index_t sphere[] = {
			3, 2, 19,
			5, 3, 8,
			0, 5, 9,
			2, 0, 16,
			2, 3, 0,
			3, 5, 0,
			11, 5, 8,
			9, 5, 11,
			16, 0, 9,
			9, 15, 16,
			16, 18, 2,
			18, 19, 2,
			19, 8, 3,
			19, 17, 8,
			17, 11, 8,
			15, 9, 11,
			18, 16, 15,
			18, 17, 19,
			17, 18, 15,
			17, 15, 11,
		};

		subsetParam.BaseColor = XMFLOAT4(0, 0, 0, 1);
		subsetParam.EdgeGradient = XMFLOAT2(0, 1);
		auto ss_sphere = shared_ptr<ModelSubset>(new ModelSubset(sphere, &subsetParam));


		D3DIndexBuffer<>::index_t corner[] = {
			0,    2,    1,   
			4,    2,    3,   
			3,    5,    6,   
			7,    5,    0,   
			6,    5,    8,   
			9,    5,    7,   
			11,     9,   10,   
			9,    7,    0,   
			10,     9,   15,   
			14,     15,  16,    
			0,     1,   16,   
			11,     10,  15,    
			12,     11,  17,    
			17,     18,  13,    
			14,     18,  15,    
			18,     14,  16,    
			13,     18,  19,    
			19,    2,    4,   
			2,    16,   1,    
			4,     3,   19,   
			8,    3,    6,   
			8,     11,  12,    
			17,     8,   12,   
			19,     17,  13,    
		};

		subsetParam.BaseColor = XMFLOAT4(0, 0, 0, 1);
		subsetParam.EdgeGradient = XMFLOAT2(1, 0);
		auto ss_corner = shared_ptr<ModelSubset>(new ModelSubset(corner, &subsetParam));


		model = new Model(vertex, 20);

		model->AddSubset(ss_sphere); // ì‡ë§Ç™êÊ
		model->AddSubset(ss_corner);

		SceneParameter scp;

		auto g = Game1::GetInstance();
		XMVECTOR eye = { 0, 0, 10, 1 };
		XMVECTOR lookat = { 0, 0, 0, 1 };
		XMVECTOR up = { 0, 1, 0, 1 };
		scp.Projection = XMMatrixPerspectiveLH(g->GetWindowWidth() / 3200.0 ,g->GetWindowHeight() / 3200.0, 0.25, 100);
		scp.View = XMMatrixLookAtLH(eye, lookat, up);
		model->UpdateSceneParams(&scp);

	}

	void TerminateModel(){
		delete model;
	}
}

Player::Player()
{
}


Game1::pool_type::Item task;
int draw(Game1::task_type& task, void*, void*);
int draw_r(Game1::task_type& task, void*, void*);

int draw(Game1::task_type& task, void*, void*){
	static float n = 0;

	//model->Draw();
	ObjectParameter obj;
	obj.World = XMMatrixRotationX(n) * XMMatrixRotationY(n / 2);
	model->UpdateObjectParams(&obj);

	model->Draw();

	n += 1 / 32.0;

	if (n > 20){
		n = 0;
		auto newtask = Game1::GetInstance()->taskPool.Activate();
		newtask->SetAction(draw_r, nullptr);
		task.InsertNext(&*newtask);
		task.MarkToRemove();
		Game1::GetInstance()->taskPool.Deactivate(::task);
		::task = newtask;
	}
	return 0;
}

int draw_r(Game1::task_type& task, void*, void*){
	static float n = 0;

	//model->Draw();
	ObjectParameter obj;
	obj.World = XMMatrixRotationX(-n) * XMMatrixRotationY(-n / 2);
	model->UpdateObjectParams(&obj);

	model->Draw();

	n += 1 / 32.0f;

	if (n > 20) {
		n = 0;
		auto newtask = Game1::GetInstance()->taskPool.Activate();
		newtask->SetAction(draw, nullptr);
		task.InsertNext(&*newtask);
		task.MarkToRemove();
		Game1::GetInstance()->taskPool.Deactivate(::task);
		::task = newtask;
	}
	return 0;
}


void Player::Initialize(D3DCore * core){
	InitializeModel(core);

	task = Game1::GetInstance()->taskPool.Activate();
	task->SetAction(draw, nullptr);
	Game1::GetInstance()->drawTasks.InsertHead(&*task);
}


Player::~Player()
{
	TerminateModel();
}
