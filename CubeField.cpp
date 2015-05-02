#include "CubeField.h"


CubeField::CubeField() {
	model = g_ModelBank->Get(_T("Content\\Model\\cube.txt"));
	model->SetEdgeWidthEnableFlag(false);
}

void CubeField::Draw() {
	if (!model) return;

	Models::InstanceData inst;
	
	//float dlpower;
	//{
	//	Models::SceneParameter& scene = Models::Model::GetSceneParam();
	//	dlpower = scene.LightColor.w;
	//	scene.LightColor.w = 0.0f;
	//}

	inst.World = XMMatrixIdentity();
	inst.Params[0].Index = 0;
	inst.Params[0].Blend = 1;
	inst.Params[0].LineWidth = 0.0f;
	inst.Params[0].Alpha = 1;
	
	model->Draw(inst);
	model->Flush(false, true);

	inst.Params[0].Index = 0;
	inst.Params[0].Blend = 0;
	inst.Params[0].LineWidth = 1 / 128.0f;
	inst.Params[0].Alpha = 0.75f;
	model->Draw(inst);
	model->Flush(true, false);

	//{
	//	Models::SceneParameter& scene = Models::Model::GetSceneParam();
	//	scene.LightColor.w = dlpower;
	//}
}

CubeField::~CubeField() {}
