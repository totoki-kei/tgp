#include "PL_Model.hlsli"
#include "Functions.hlsli"

// �W�I���g���V�F�[�_
// ���̓��͂���A���̂���l�p���o�͂���

[maxvertexcount(8)]
void main(
	line VS_OUT input[2],
	inout TriangleStream< VS_OUT > output
	)
{
	VS_OUT element;

	float3 normal;
	float3 widthDirection;
	float lineWidth0;
	float lineWidth1;

	// �@�����v�Z : 2���_�̖@���̕��ς𐳋K����������
	normal = input[1].normal + input[0].normal;
	if (length(normal) == 0) return;
	normal = normalize(normal);

	// ���̕����擾
	lineWidth0 = input[0].width;
	lineWidth1 = input[1].width;
	if (lineWidth0 * lineWidth1 <= 0) return;

	// �@���ƒ����̕�������A���̑��������̐��K���x�N�g�����v�Z
	widthDirection = normalize(cross(normal, input[1].world - input[0].world));

	// ���_�̏o��(���ʂ̎l�p�`)
	// (0) ���ʏ��
	element.normal = normal;
	element.width = 0;
	// (1) ����
	element.world = input[0].world + widthDirection * lineWidth0;
	element.position = mul(Projection, mul(View, float4(element.world, 1)));
	element.color = input[0].color * float4(2, 2, 2, 1);
	output.Append(element);
	// (2) �E��
	element.world = input[0].world - widthDirection * lineWidth0;
	element.position = mul(Projection, mul(View, float4(element.world, 1)));
	element.color = input[0].color * float4(2, 2, 2, 1);
	output.Append(element);
	// (3) ����
	element.world = input[1].world + widthDirection * lineWidth1;
	element.position = mul(Projection, mul(View, float4(element.world, 1)));
	element.color = input[1].color * float4(2, 2, 2, 1);
	output.Append(element);
	// (4) �E��
	element.world = input[1].world - widthDirection * lineWidth1;
	element.position = mul(Projection, mul(View, float4(element.world, 1)));
	element.color = input[1].color * float4(2, 2, 2, 1);
	output.Append(element);

	output.RestartStrip();

	// (0) ���ʏ��i�@�����t�]�j
	element.normal = -normal;
	// (3) ����
	element.world = input[1].world + widthDirection * lineWidth1;
	element.position = mul(Projection, mul(View, float4(element.world, 1)));
	element.color = input[1].color;
	output.Append(element);
	// (4) �E��
	element.world = input[1].world - widthDirection * lineWidth1;
	element.position = mul(Projection, mul(View, float4(element.world, 1)));
	element.color = input[1].color;
	output.Append(element);
	// (1) ����
	element.world = input[0].world + widthDirection * lineWidth0;
	element.position = mul(Projection, mul(View, float4(element.world, 1)));
	element.color = input[0].color;
	output.Append(element);
	// (2) �E��
	element.world = input[0].world - widthDirection * lineWidth0;
	element.position = mul(Projection, mul(View, float4(element.world, 1)));
	element.color = input[0].color;
	output.Append(element);

	output.RestartStrip();

}
