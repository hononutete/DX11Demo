//ランバート反射の定数バッファ構造体
cbuffer cbLambert : register(b0)
{
	column_major float4x4 g_matWVP;// : packoffset(c0);
	column_major float4x4 g_matNormal;// : packoffset(c0);
	float4 g_vecLight;// : packoffset(c0);
	float g_Ambient;// : packoffset(c0);
}

//頂点入力セマンティクス構造体
struct VS_IN
{
	float4 pos    : POSITION; //座標
	float3 normal : NORMAL;  //法線（正規化された値）
	float4 color  : COLOR;    //頂点カラー
	float2 texel  : TEXCOORD; //テクセル
};

//頂点出力セマンティクス構造体
struct VS_OUT
{
	float4 pos   : SV_POSITION;
	float4 diffuse : COLOR0;
	float2 texel : TEXCOORD0;
};

//頂点シェーダー
VS_OUT VS_Main(VS_IN In)
{
	VS_OUT Out;

	Out.pos = mul(In.pos, g_matWVP);
	Out.texel = In.texel;
	float3 n = normalize(In.normal);

	//法線をローカル空間からワールド空間に変換する
	n = mul(n, g_matNormal);
		
	//ライティング計算（グーローシェーディング）
	//グーローシェーディングではランバート反射を使う
	//法線と光源ベクトルの内積で色を決定
	//n：正規化された法線
	//g_vecLight：光ベクトル
	float diffuse = dot(-g_vecLight.xyz, n);

	//環境光よりは暗くならないように設定
	diffuse = max(g_Ambient, diffuse);

	//ライティングはrgbのみに作用する
	float4 c = float4((In.color.rgb * diffuse), In.color.a);
	Out.diffuse = c;
	Out.texel = In.texel;
	return Out;
}

//ピクセルシェーダー
float4 PS_Main(VS_OUT In) : SV_TARGET
{
	//頂点カラーは、ライティングを吟味した頂点カラー、テクスチャの色とライティングの結果を合成
	return In.diffuse;
}