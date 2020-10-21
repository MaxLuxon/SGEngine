#if SGE_ALPHA_CUT

    uniform float u_sgeAlphaCutThreshold;
    uniform sampler2D u_sge_mainTex;
    input vec2 m_uv;

    if(texture(u_sge_mainTex,m_uv).a<0.5) discard;

#endif

#if SGE_MASK_CUT

    uniform float u_sgeAlphaCutThreshold;
    uniform sampler2D u_alphaMask;
    input vec2 m_uv;

    if(texture(u_alphaMask,m_uv).r<0.7) discard;

#endif

#if alpha_fake_opacity

    uniform float u_opacity;
    uniform sampler2D u_noiseMap("sge/img/noise.png");
    input vec2 m_uv;

    if(texture(u_alphaMask,m_uv).r<u_sgeAlphaCutThreshold) discard;

#endif
