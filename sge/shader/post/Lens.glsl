#insert "sge/shader/View.glsl"

// -----------------------------------------------------
## fragment
// -----------------------------------------------------

area main override{

	#include "sge/shader/def/ToneMapping.glsl"
	#include "sge/shader/def/LensEffects.glsl"

    input vec2 m_uv;
    uniform sampler2D u_sge_mainTex;
	uniform float u_time;
	uniform float u_exposure;

    vec3 color=chromaticAberration(m_uv, u_sge_mainTex);
    
#ifdef SGE_TONE_MAPPING

    color = simpleReinhardToneMapping(color*0.7);
#endif
    
    color = mix(color, color*0.65, vignette(m_uv, 0.5));
    
	#ifdef COLOR_GRADING

		uniform sampler2D u_colorRamp;
    	color = colorGrading(color, u_colorRamp);

   	#endif

   	#ifdef NOISE

		uniform sampler2D noise;

   	#endif

    uniform vec4 u_tint_color;

    output.color =  vec4(color.rgb , u_tint_color.a);

}






