#version 410 core
out vec4 FragColor;

in vec3 fragDirection;

uniform vec3 u_sunDirection;
uniform vec3 u_sunColor;
uniform float u_timeOfDay;

float hash(vec2 p) {
    return fract(sin(dot(p, vec2(12.9898,78.233))) * 43758.5453);
}

float starField(vec3 dir) {
    float azimuth = atan(dir.z, dir.x);
    float altitude = acos(clamp(dir.y, 0.0, 1.0));

    vec2 uv = vec2(azimuth / (2.0 * 3.1415926) + 0.5, altitude / (0.5 * 3.1415926));

    const float SCALE = 35.0; // Ammount of stars
    vec2 st = uv * SCALE;
    vec2 cell = floor(st);
    vec2 fuv = fract(st);

    // Random per cell
    float r1 = hash(cell + vec2(0.0, 0.0));
    float r2 = hash(cell + vec2(2.0, 3.0));
    float r3 = hash(cell + vec2(4.0, 1.0));
    vec2 starOffset = vec2(r1, r2);

    // Distance from star center
    float d = length(fuv - starOffset);

    // Star radius
    float radius = mix(0.005, 0.01, r3);

    // Smooth round star shape
    float star = smoothstep(radius, radius * 0.3, d);

    // Horizon falloff (Stars fade near horizon)
    float horizonFalloff = pow(clamp(dir.y, 0.0, 1.0), 0.5);

    return star * horizonFalloff * mix(0.4, 1.0, hash(cell + vec2(7.0, 9.0))) * 100.0;
}

vec3 getSkyGradient(float y) {
    vec3 nightColor = vec3(0.15, 0.05, 0.25);
    vec3 dawnColor = vec3(0.7, 0.25, 0.4);
    vec3 dayColor = vec3(0.35, 0.60, 0.95);
    vec3 sunsetColor = vec3(0.9, 0.25, 0.6);

    y += 0.1;
    y = clamp(y, -1.0, 1.0);
    float t = smoothstep(0.0, 1.0, pow(max(y, 0.0), 0.55));

    vec3 result;
    if (u_timeOfDay < 0.25) {
        result = mix(nightColor, dawnColor, u_timeOfDay / 0.25);
    } else if (u_timeOfDay < 0.50) {
        result = mix(dawnColor, dayColor, (u_timeOfDay - 0.25) / 0.25);
    } else if (u_timeOfDay < 0.75) {
        result = mix(dayColor, sunsetColor, (u_timeOfDay - 0.50) / 0.25);
    } else {
        result = mix(sunsetColor, nightColor, (u_timeOfDay - 0.75) / 0.25);
    }
    return result * t;
}

float drawSun(vec3 dir, vec3 sunDir) {
    float sunDot = dot(normalize(dir), normalize(sunDir));
    float sunDisk = smoothstep(0.999, 1.0, sunDot);
    float sunGlow = pow(max(sunDot, 0.0), 600.0);
    return sunDisk + sunGlow;
}

void main() {
    vec3 dir = normalize(fragDirection);

    // Base sky + sun
    vec3 skyCol = getSkyGradient(dir.y);
    float sunF  = drawSun(dir, u_sunDirection);
    vec3 sunL   = u_sunColor * sunF;
    vec3 col    = skyCol + sunL;

    float nightFactor = 0.0;
    if (u_timeOfDay < 0.2) {
        nightFactor = (0.2 - u_timeOfDay) / 0.2;
    } else if (u_timeOfDay > 0.8) {
        nightFactor = (u_timeOfDay - 0.8) / 0.2;
    }

    // Stars only when above horizon and at night
    if (nightFactor > 0.0 && dir.y > 0.0) {
        float stars = starField(dir) * nightFactor;
        col += vec3(stars);
    }

    FragColor = vec4(clamp(col, 0.0, 1.0), 1.0);
}
