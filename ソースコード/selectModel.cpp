#include "selectModel.h"

void SelectModel::GetBodyModel(int num, Parts * model)
{
	model->SetShader("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
	switch (num)
	{
	case 1:
		model->SetModel("asset\\model\\body01.obj");
		break;
	case 2:
		model->SetModel("asset\\model\\body02.obj");
		break;
	case 3:
		model->SetModel("asset\\model\\body03.obj");
		break;
	default:
		model->SetModel("asset\\model\\body00.obj");
		break;
	}
}

void SelectModel::GetColor(int num, D3DXVECTOR4 * c)
{

	switch (num)
	{
	case 1:
		c->x = 1.0f;
		c->y = 0.0f;
		c->z = 0.0f;
		break;
	case 2:
		c->x = 1.0f;
		c->y = 1.0f;
		c->z = 0.0f;
		break;
	case 3:
		c->x = 1.0f;
		c->y = 0.0f;
		c->z = 1.0f;
		break;
	case 4:
		c->x = 0.0f;
		c->y = 1.0f;
		c->z = 0.0f;
		break;
	case 5:
		c->x = 0.0f;
		c->y = 1.0f;
		c->z = 1.0f;
		break;
	case 6:
		c->x = 0.0f;
		c->y = 0.0f;
		c->z = 1.0f;
		break;
	case 7:
		c->x = 0.5f;
		c->y = 0.5f;
		c->z = 0.5f;
		break;
	case 8:
		c->x = 0.0f;
		c->y = 0.0f;
		c->z = 0.0f;
		break;
	default:
		c->x = 1.0f;
		c->y = 1.0f;
		c->z = 1.0f;
		break;
	}

	c->w = 1.0f;
}

void SelectModel::GetItemModel(int num, Parts * model)
{
	if (num != 0) {
		model->SetShader("shader\\vertexLightingVS.cso", "shader\\vertexLightingPS.cso");
		switch (num)
		{
		case 1:
			model->SetModel("asset\\model\\face01.obj");
			break;
		case 2:
			model->SetModel("asset\\model\\face02.obj");
			break;
		case 3:
			model->SetModel("asset\\model\\face03.obj");
			break;
		case 4:
			model->SetModel("asset\\model\\face04.obj");
			break;
		case 5:
			model->SetModel("asset\\model\\face05.obj");
			break;
		case 6:
			model->SetModel("asset\\model\\head01.obj");
			break;
		case 7:
			model->SetModel("asset\\model\\head02.obj");
			break;
		case 8:
			model->SetModel("asset\\model\\head03.obj");
			break;
		case 9:
			model->SetModel("asset\\model\\head04.obj");
			break;
		case 10:
			model->SetModel("asset\\model\\head05.obj");
			break;
		case 11:
			model->SetModel("asset\\model\\head06.obj");
			break;
		case 12:
			model->SetModel("asset\\model\\back01.obj");
			break;
		case 13:
			model->SetModel("asset\\model\\back02.obj");
			break;
		case 14:
			model->SetModel("asset\\model\\back03.obj");
			break;
		case 15:
			model->SetModel("asset\\model\\back04.obj");
			break;
		default:
			break;
		}
	}
	else {
		model = nullptr;
	}
	
}
