#include "MyDraw.h"
#include<DxLib.h>
#include<array>

void MyDraw::DrawRotaGraph(Vector2 pos, float scale, float angle, int handle, int psH, std::list<int> texH, int alpha, int vsH)
{
	MV1SetUseOrigShader(true);//シェーダを使うためにtrueにする
	//4頂点に必要な情報を代入していく
	//必要な情報→座標、頂点カラー、UV
	//ただし、rhwは必ず1.0に
	std::array<DxLib::VERTEX2DSHADER, 4> vertices = {};
	for (auto& v : vertices) {
		v.rhw = 1.0f;//これは必ず1
		v.dif = { 255,255,255,255 };//これは白色にしておく
	}

	//画像の大きさ取得
	int w, h;
	DxLib::GetGraphSize(handle, &w, &h);

	//並べ方はZの字になるように
	//頂点情報(左上)
	vertices[0].pos.x = pos.x - w * 0.5 * scale;
	vertices[0].pos.y = pos.y - h * 0.5 * scale;
	vertices[0].u = 0.0f;
	vertices[0].v = 0.0f;

	//頂点情報(右上)
	vertices[1].pos.x = pos.x + w * 0.5 * scale;
	vertices[1].pos.y = pos.y - h * 0.5 * scale;
	vertices[1].u = 1.0f;
	vertices[1].v = 0.0f;

	//頂点情報(左下)
	vertices[2].pos.x = pos.x - w * 0.5 * scale;
	vertices[2].pos.y = pos.y + h * 0.5 * scale;
	vertices[2].u = 0.0f;
	vertices[2].v = 1.0f;

	//頂点情報(右下)
	vertices[3].pos.x = pos.x + w * 0.5 * scale;
	vertices[3].pos.y = pos.y + h * 0.5 * scale;
	vertices[3].u = 1.0f;
	vertices[3].v = 1.0f;

	//テクスチャを指定(メイン)
	DxLib::SetUseTextureToShader(0, handle);

	//テクスチャが入っているならば
	if (texH.size() > 0)
	{
		int slotIndex = 1;
		for (auto& tex : texH) {
			//スロットにテクスチャを指定
			DxLib::SetUseTextureToShader(slotIndex, tex);
			++slotIndex;
		}
	}
	//透明度
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	//シェーダを指定
	DxLib::SetUsePixelShader(psH);
	//頂点シェーダを指定
	SetUseVertexShader(vsH);
	//シェーダを適用して描画
	DxLib::DrawPrimitive2DToShader(vertices.data(), vertices.size(),
		DX_PRIMTYPE_TRIANGLESTRIP);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//シェーダを解除
	DxLib::SetUseTextureToShader(0, -1);
	if (texH.size() > 0)
	{
		int slotIndex = 1;
		for (auto& tex : texH) {
			//リセット
			SetUseTextureToShader(slotIndex, -1);
			++slotIndex;
		}
	}
	SetUsePixelShader(-1);
	SetUseVertexShader(-1);
	MV1SetUseOrigShader(false);
}
