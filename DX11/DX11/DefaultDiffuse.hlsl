//�����o�[�g���˂̒萔�o�b�t�@�\����
cbuffer cbLambert : register(b0)
{
	column_major float4x4 g_matWVP;// : packoffset(c0);
	column_major float4x4 g_matNormal;// : packoffset(c0);
	float4 g_vecLight;// : packoffset(c0);
	float g_Ambient;// : packoffset(c0);
}

//���_���̓Z�}���e�B�N�X�\����
struct VS_IN
{
	float4 pos    : POSITION; //���W
	float3 normal : NORMAL;  //�@���i���K�����ꂽ�l�j
	float4 color  : COLOR;    //���_�J���[
	float2 texel  : TEXCOORD; //�e�N�Z��
};

//���_�o�̓Z�}���e�B�N�X�\����
struct VS_OUT
{
	float4 pos   : SV_POSITION;
	float4 diffuse : COLOR0;
	float2 texel : TEXCOORD0;
};

//���_�V�F�[�_�[
VS_OUT VS_Main(VS_IN In)
{
	VS_OUT Out;

	Out.pos = mul(In.pos, g_matWVP);
	Out.texel = In.texel;
	float3 n = normalize(In.normal);

	//�@�������[�J����Ԃ��烏�[���h��Ԃɕϊ�����
	n = mul(n, g_matNormal);
		
	//���C�e�B���O�v�Z�i�O�[���[�V�F�[�f�B���O�j
	//�O�[���[�V�F�[�f�B���O�ł̓����o�[�g���˂��g��
	//�@���ƌ����x�N�g���̓��ςŐF������
	//n�F���K�����ꂽ�@��
	//g_vecLight�F���x�N�g��
	float diffuse = dot(-g_vecLight.xyz, n);

	//�������͈Â��Ȃ�Ȃ��悤�ɐݒ�
	diffuse = max(g_Ambient, diffuse);

	//���C�e�B���O��rgb�݂̂ɍ�p����
	float4 c = float4((In.color.rgb * diffuse), In.color.a);
	Out.diffuse = c;
	Out.texel = In.texel;
	return Out;
}

//�s�N�Z���V�F�[�_�[
float4 PS_Main(VS_OUT In) : SV_TARGET
{
	//���_�J���[�́A���C�e�B���O���ᖡ�������_�J���[�A�e�N�X�`���̐F�ƃ��C�e�B���O�̌��ʂ�����
	return In.diffuse;
}