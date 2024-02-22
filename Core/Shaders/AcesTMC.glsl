// Narkowicz 2015, "ACES Filmic Tone Mapping Curve"
vec3 aces(vec3 x) {
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

vec3 gamma_correct(vec3 x, float gamma) {
    return pow(x, vec3(1.0 / gamma));
}

vec3 reinhard(float x) {
    return x / (x + vec3(1.0));
}