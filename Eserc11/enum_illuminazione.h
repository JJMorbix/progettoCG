#pragma once
typedef enum {
	GOURAUD,
	PHONG,
	BLINN,
	TOON,
	PASS_THROUGH
} ShadingType;

typedef enum {
	RED_PLASTIC,
	EMERALD,
	BRASS,
	SNOW_WHITE,
	YELLOW,
	PINK,
	BROWN,
	NO_MATERIAL
} MaterialType;


typedef enum {
	NONE,
	GOURAD_SHADING,
	BLINNPHONG_SHADING,
	PHONG_SHADING,
	NO_TEXTURE,
	WAVE,
	FLAG,
} ShaderOption;
 