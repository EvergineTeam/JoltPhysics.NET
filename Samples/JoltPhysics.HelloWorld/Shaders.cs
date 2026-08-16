namespace JoltHelloWorld
{
	public static class Shaders
	{
		/// <summary>
		/// Per-geom constant buffer + simple directional Lambert with a soft ambient floor.
		/// Same shape as the engine's VisualTests DrawCube shader, plus normal and color.
		/// </summary>
		public const string Hlsl = """
			cbuffer PerObject : register(b0)
			{
				float4x4 worldViewProj;
				float4x4 world;
				float4 color;
			};

			struct VS_IN
			{
				float3 pos : POSITION;
				float3 nor : NORMAL;
				float3 tan : TANGENT;
				float2 tex : TEXCOORD;
			};

			struct PS_IN
			{
				float4 pos : SV_POSITION;
				float3 nor : NORMAL;
				float4 col : COLOR;
			};

			PS_IN VS(VS_IN input)
			{
				PS_IN output = (PS_IN)0;
				output.pos = mul(float4(input.pos, 1.0), worldViewProj);
				output.nor = normalize(mul(float4(input.nor, 0.0), world).xyz);
				output.col = color;
				return output;
			}

			float4 PS(PS_IN input) : SV_Target
			{
				// Jolt is Y-up, so this is the MuJoCo sample's key light with Y and Z swapped:
				// same direction relative to the ground, so both samples shade identically.
				float3 lightDir = normalize(float3(-0.35, 0.82, -0.45));
				float ndl = saturate(dot(normalize(input.nor), lightDir));
				float lighting = 0.35 + 0.65 * ndl;
				return float4(input.col.rgb * lighting, input.col.a);
			}
			""";
	}
}
